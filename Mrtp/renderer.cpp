/* File      : renderer.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <Eigen/Geometry>
#include <iostream>
#include <cmath>

#ifdef _OPENMP
#include <omp.h>
#endif  // !_OPENMP

#include "renderer.hpp"
#include "png.hpp"

using namespace std;
using namespace png;
using namespace Eigen;

static const float kDegreeToRadian = M_PI / 180.0f;
static const float kRealToByte = 255.0f;


/*
================
CRenderer

Creates a renderer
================
*/
CRenderer::CRenderer (CWorld *world, int width, int height, float fov, float distance, 
                    float shadowfactor, int maxdepth, int reflshadow, int nthreads) {
    width_ = width;
    height_ = height;
    fov_ = fov;

    ratio_ = (float) width_ / (float) height_;
    perspective_ = ratio_ / (2.0f * tan (kDegreeToRadian * (fov_ / 2.0f)));

    maxdist_ = distance;
    shadow_ = shadowfactor;
    maxdepth_ = maxdepth;
    reflshadow_ = (reflshadow != 0) ? true : false;
    nthreads_ = nthreads;

    camera_ = world->GetCamera ();
    light_ = world->GetLight ();
    actors_ = world->GetActors ();

    Pixel dummy;
    buffer_.assign (width_ * height_, dummy);
}

/*
================
~CRenderer

Destroys a renderer
================
*/
CRenderer::~CRenderer () {
}

/*
================
WriteScene

Writes a rendered scene to a PNG file
================
*/
void CRenderer::WriteScene (char *filename) {
    image<rgb_pixel> image (width_, height_);
    Pixel *in = &buffer_[0];

    for (int i=0; i<height_; i++) {
        rgb_pixel *out = &image[i][0];

        for (int j=0; j<width_; j++, in++, out++) {
            Pixel bytes = kRealToByte * (*in);

            out->red = (unsigned char) bytes[0];
            out->green = (unsigned char) bytes[1];
            out->blue = (unsigned char) bytes[2];
        }
    }

    image.write (filename);
}

/*
================
SolveShadows

Checks for intersections with other actors between 
the hit actor and the source of light
================
*/
bool CRenderer::SolveShadows (Vector3f *origin, Vector3f *direction, float maxdist) {
    for (auto a=actors_->begin (); a!=actors_->end (); a++) {
        Actor *actor = *a;

        if (actor->HasShadow ()) {
            float distance = actor->Solve (origin, direction, 0.0f, maxdist);
            if (distance > 0.0f) {
                return true;
            }
        }
    }
    return false;
}

/*
================
SolveHits

Returns a pointer and a distance to the closest 
actor with which the ray intersects.

Returns a nullptr if there have been no 
intersections.
================
*/
Actor *CRenderer::SolveHits (Vector3f *origin, Vector3f *direction, float *currd) {
    Actor *hit = nullptr;

    for (auto a=actors_->begin (); a!=actors_->end (); a++) {
        Actor *actor = *a;
        float distance = actor->Solve (origin, direction, 0.0f, maxdist_);

        if ((distance > 0.0f) && (distance < (*currd))) {
            *currd = distance;
            hit = actor;
        }
    }
    return hit;
}

/*
================
TraceRay_r

Traces a ray and its reflected rays
================
*/
void CRenderer::TraceRay_r (Vector3f *origin, Vector3f *direction, int depth, float mixing, Pixel *pixel) {
    float currd = maxdist_;
    Actor *hitactor = SolveHits (origin, direction, &currd);

    if (hitactor) {
        Vector3f inter = ((*direction) * currd) + (*origin);

        //Calculate light intensity
        Vector3f ray = light_->CalculateRay (&inter);

        float raylen = ray.norm ();
        ray *= (1.0f / raylen);

        Vector3f normal = hitactor->CalculateNormal (&inter);
        float intensity = ray.dot (normal);

        //Check if the intersection is in a shadow
        bool isshadow = SolveShadows (&inter, &ray, raylen);
        float shadow = (isshadow) ? shadow_ : 1.0f;

        //Decrease light intensity for actors away from the light
        float ambient = 1.0f - pow (raylen / maxdist_, 2);

        //Combine pixels
        float lambda = mixing * intensity * shadow * ambient;

        Pixel *pick = hitactor->PickPixel (&inter, &normal);
        *pixel = (1.0f - lambda) * (*pixel) + lambda * (*pick);

        /*
        //Quit if the surface is shadowed, but reflections are turned off
        if (isshadow && (!reflshadow_)) {
            return;
        }

        //Quit if the maximum level of recursion is reached
        if (++depth > maxdepth_) {
            return;
        }

        //If the hit actor is reflective, trace a reflected ray
        float factor = hitactor->Reflective ();

        if (factor != 0.0f) {
            Vector3f reflected = (*direction) - (2.0f * direction->dot (normal)) * normal;

            TraceRay_r (&inter, &reflected, depth, factor, pixel);
        }
        */
    }
    //No intersections found, quit
}

/*
================
RenderBlock

Renders a rectangular block of the screen
================
*/
void CRenderer::RenderBlock (int block, int nlines) {
    Pixel *pixel = &buffer_[block*nlines*width_];

    for (int j=0; j<nlines; j++) {

        for (int i=0; i<width_; i++, pixel++) {
            Vector3f origin = camera_->CalculateOrigin (i, j+block*nlines);
            Vector3f direction = camera_->CalculateDirection (&origin);

            *pixel << 0.0f, 0.0f, 0.0f;

            TraceRay_r (&origin, &direction, 0, 1.0f, pixel);
        }
    }
}

/*
================
Render

Renders a scene.

In parallel mode, splits the buffer into several 
horizontal blocks, each rendered by a separate thread.

After each thread has finished, there may still be 
some left-over lines to render.

If nthreads=0, uses as many threads as available.
================
*/
void CRenderer::Render () {
    camera_->CalculateWindow (width_, height_, perspective_);

#ifdef _OPENMP
    if (nthreads_ == 1) {
        //Serial execution
        RenderBlock (0, height_);
    }
    else {
        //Parallel execution
        if (nthreads_ != 0) {
            omp_set_num_threads (nthreads_);
        }
        int nlines = height_ / nthreads_;

        #pragma omp parallel for
        for (int block=0; block<nthreads_; block++) {
            RenderBlock (block, nlines);
        }

        int nfill = height_ % nthreads_;
        if (nfill) {
            RenderBlock (block+1, nfill);
        }
    }
#else
    //No OpenMP compiled in, always do serial execution
    RenderBlock (0, height_);

#endif /* !_OPENMP */
}
