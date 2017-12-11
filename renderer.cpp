/*
 * Mrtp: A simple raytracing tool.
 * Copyright (C) 2017  Mikolaj Feliks <mikolaj.feliks@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cstddef>
#include <cmath>
#include "renderer.hpp"

#ifdef _OPENMP
#include <omp.h>
#endif /* !_OPENMP */

using namespace std;

const double kDegreeToRadian = M_PI / 180.0f;


Renderer::Renderer (World *world, 
        unsigned int width, unsigned int height, double fov, 
        double distance, LightModel_t lightmodel, 
        double shadowfactor, 
        unsigned int maxdepth, bool reflshadow, 
        unsigned int nthreads) {
    buffer_ = NULL;
    width_ = width;
    height_ = height;
    fov_ = fov;

    maxdist_ = distance;
    model_ = lightmodel;
    shadow_ = shadowfactor;

    maxdepth_ = maxdepth;
    reflshadow_ = reflshadow;
    nthreads_ = nthreads;

    /* Assign pointers. */
    world->Bind (&camera_, &light_, &actors_);
}

Renderer::~Renderer () {
    if (buffer_ != NULL) {
        delete buffer_;
    }
}

void Renderer::Initialize () {
    if (buffer_ == NULL) {
        buffer_ = new Buffer (width_, height_);
        buffer_->Allocate ();
    }

    ratio_ = (double) width_ / (double) height_;
    perspective_ = ratio_ / (2.0f * tan (kDegreeToRadian * (fov_ / 2.0f)));
}

void Renderer::SaveFrame (string *path) {
    buffer_->Write (path);
}

bool Renderer::SolveShadows (Vector *origin, Vector *direction, 
                             double maxdist) {
    double  distance;
    bool    hit = false;
    Actor  *actor = actors_;

    while (actor != NULL) {
        if (actor->HasShadow ()) {
            distance = actor->Solve (origin, direction, 0.0f, maxdist);
            if (distance > 0.0f) {
                hit = true;
                break;
            }
        }
        actor = actor->Next ();
    }
    return hit;
}

bool Renderer::SolveHits (Vector *origin, Vector *direction, Actor **hitactor, 
                          double *currd) {
    double  distance;
    bool    hit = false;
    Actor  *actor = actors_;

    while (actor != NULL) {
        distance = actor->Solve (origin, direction, 0.0f, maxdist_);
        if ((distance > 0.0f) && (distance < (*currd))) {
            *currd    = distance;
            *hitactor = actor;
            hit       = true;
        }
        actor = actor->Next ();
    }
    return hit;
}

void Renderer::TraceRay_r (Vector *origin, Vector *direction, 
                           unsigned int depth, 
                           double mixing, Color *color) {
    Actor   *hitactor;
    double   currd, raylen, intensity, shadow, ambient, factor;
    Vector   inter, ray, normal, reflected;
    Color    objcol;
    bool     hit, isshadow;

    currd = maxdist_;
    hit = SolveHits (origin, direction, &hitactor, &currd);

    if (hit) {
        /* Found an intersection. */
        inter = ((*direction) * currd) + (*origin);

        hitactor->GetNormal (&inter, &normal);
        hitactor->DetermineColor (&inter, &normal, &objcol);

        /* Calculate the intensity of light. */
        intensity = light_->Intensity (&inter, &normal, &ray, &raylen);

        /* Check if the intersection is in a shadow. */
        isshadow = SolveShadows (&inter, &ray, raylen);
        shadow = (isshadow) ? shadow_ : 1.0f;

        /* Decrease light intensity for actors away from the light. */
        ambient = 1.0f;
        if (model_ != lightNone) {
            if (model_ == lightLinear) {
                ambient = 1.0f - (raylen / maxdist_);
            }
            else {  /* if (model_ == lightQuadratic) */
                ambient = 1.0f - pow (raylen / maxdist_, 2);
            }
        }

        /* Combine colors. */
        color->Combine_InPlace (&objcol, mixing * intensity * shadow * ambient);

        /* Quit if the surface is shadowed, but reflections from shadowed
         * surfaces are not allowed.
         */
        if (isshadow && (!reflshadow_)) {
            return;
        }

        /* Quit if the maximum level of recursion is reached. */
        if (++depth > maxdepth_) {
            return;
        }

        /* If the hit actor is reflective, trace a reflected ray. */
        if (hitactor->Reflective (&factor)) {
            direction->Reflect (&normal, &reflected);

            TraceRay_r (&inter, &reflected, depth, factor, color);
        }
    }
    /* No intersections found, quit. */
}

void Renderer::RenderBlock (unsigned int block, unsigned int nlines) {
    unsigned int i, j;
    Vector   origin, direction;
    Color   *color;

    color = buffer_->Pointer (block * nlines);
    for (j = 0; j < nlines; j++) {
        for (i = 0; i < width_; i++, color++) {
            camera_->CalculateRay (i, (j + block * nlines), &origin, &direction);

            color->Zero ();
            TraceRay_r (&origin, &direction, 0, 1.0f, color);
        }
    }
}

void Renderer::Render () {
    camera_->CalculateWindow (width_, height_, perspective_);

#ifdef _OPENMP
    if (nthreads_ == 1) {
        /*
         * Serial execution.
         */
        RenderBlock (0, height_);
    }
    else {
        /* 
         * Parallel execution. 
         *
         * Split the buffer into several horizontal blocks, 
         * each rendered by a separate thread.
         *
         * After each thread has finished, there may still be 
         * some left-over lines to render.
         *
         * NOTE: Methods that operate on actors (Solve, GetNormal, 
         * etc.) must be thread-safe.
         *
         * If nthreads=0, use as many threads as available.
         */
        unsigned int nlines, nfill, block;

        if (nthreads_ != 0) {
            omp_set_num_threads (nthreads_);
        }
        nlines = height_ / nthreads_;

        #pragma omp parallel for
        for (block = 0; block < nthreads_; block++) {
            RenderBlock (block, nlines);
        }
        nfill = height_ % nthreads_;
        if (nfill) {
            RenderBlock (block + 1, nfill);
        }
    }
#else
    /*
     * No OpenMP compiled in, always do serial execution.
     */
    RenderBlock (0, height_);
#endif /* !_OPENMP */
}
