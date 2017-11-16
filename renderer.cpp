/*
 *  Mrtp: A simple raytracing tool.
 *  Copyright (C) 2017  Mikolaj Feliks <mikolaj.feliks@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "renderer.hpp"

using namespace std;


Renderer::Renderer (World *world, unsigned int width,
        unsigned int height, double fov,
        double distance, double shadowfactor, 
        LightModel_t lightmodel, unsigned int maxdepth, 
        bool reflshadow, unsigned int nthreads) {
    world_ = world;
    buffer_ = NULL;
    width_ = width;
    height_ = height;
    fov_ = fov;
    maxdist_ = distance;
    shadow_ = shadowfactor;
    model_ = lightmodel;
    maxdepth_ = maxdepth;
    reflshadow_ = reflshadow;
    nthreads_ = nthreads;
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
    perspective_ = ratio_ / (2.0f * tan (DEG_TO_RAD (fov_ / 2.0f)));
}

void Renderer::SaveFrame (string *path) {
    buffer_->WriteToPNG (path);
}

bool Renderer::SolveShadows (Vector *origin, Vector *direction, double maxdist, 
                             Actor *actor) {
    double distance;
    bool   hit = false;

    while (actor != NULL) {
        distance = actor->Solve (origin, direction, 0.0f, maxdist);
        if (distance > 0.0f) {
            hit = true;
            break;
        }
        actor = actor->Next ();
    }
    return hit;
}

bool Renderer::SolveHits (Vector *origin, Vector *direction, Actor *actor, 
                          Actor **hitactor, double *currd) {
    double distance;
    bool   hit = false;

    while (actor != NULL) {
        distance = actor->Solve (origin, direction, 0.0f, maxdist_);
        if ((distance > 0.0f) && (distance < (*currd))) {
            (*currd)    = distance;
            (*hitactor) = actor;
            hit         = true;
        }
        actor = actor->Next ();
    }
    return hit;
}

void Renderer::TraceRay_r (Vector *origin, Vector *direction, 
                           unsigned int depth, 
                           double mixing, Color *color) {
    Light     *light;
    Plane     *planes;
    Sphere    *spheres;
    Cylinder  *cylinders;

    Actor     *hitactor;
    double     currd, raylen, intensity, shadow, ambient, factor;
    Vector     inter, ray, normal, reflected;
    Color      objcol;
    bool       hitplane, hitsphere, hitcylinder, isshadow;

    world_->GetLight (&light);
    world_->GetActors (&planes, &spheres, &cylinders);

    currd = maxdist_;

    hitplane = SolveHits (origin, direction, planes, &hitactor, &currd);
    hitsphere = SolveHits (origin, direction, spheres, &hitactor, &currd);
    hitcylinder = SolveHits (origin, direction, cylinders, &hitactor, &currd);

    if (hitplane || hitsphere || hitcylinder) {
        /* Found an intersection. */
        inter = ((*direction) * currd) + (*origin);

        hitactor->GetNormal (&inter, &normal);
        hitactor->DetermineColor (&inter, &normal, &objcol);

        /* Find a vector between the intersecion and light. */
        light->LightRay (&inter, &ray);
        raylen = ray.Normalize_InPlace ();
        intensity = normal * ray;

        /* intensity = (intensity < 0.0f) ? 0.0f : ((intensity > 1.0f) ? 1.0f : intensity); */
        intensity = (intensity < 0.0f) ? 0.0f : intensity;

        /* Check if the intersection is in a shadow. 
         *
         * Planes cannot cast shadows, check only for spheres and cylinders.
         */
        shadow = 1.0f;
        isshadow = SolveShadows (&inter, &ray, raylen, spheres);
        if (!isshadow) {
            isshadow = SolveShadows (&inter, &ray, raylen, cylinders);
        }
        if (isshadow) {
            shadow = shadow_;
        }

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

void Renderer::RenderBlock (Vector *vw, Vector *vh, Vector *vo, Vector *eye,
                            unsigned int block, unsigned int nlines) {
    unsigned int i, j;
    Vector  horizontal, vertical, origin, direction;
    Color  *color;

    color = buffer_->Pointer (block * nlines);
    for (j = 0; j < nlines; j++) {
        for (i = 0; i < width_; i++, color++) {
            horizontal = (*vw) * (double) i;
            vertical   = (*vh) * (double) (j + block * nlines);

            origin     = (*vo) + horizontal + vertical;
            direction  = origin - (*eye);
            direction.Normalize_InPlace ();

            color->Zero ();
            TraceRay_r (&origin, &direction, 0, 1.0f, color);
        }
    }
}

void Renderer::Render () {
    Vector   vw, vh, vo, eye;
    Camera  *camera;

    world_->GetCamera (&camera);

    camera->CalculateVectors ((double) width_, (double) height_, 
        perspective_, &vw, &vh, &vo);
    camera->GetEye (&eye);

#ifdef _OPENMP
    if (nthreads_ == 1) {
        /*
         * Serial execution.
         */
        RenderBlock (&vw, &vh, &vo, &eye, 0, height_);
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
            RenderBlock (&vw, &vh, &vo, &eye, block, nlines);
        }
        nfill = height_ % nthreads_;
        if (nfill) {
            RenderBlock (&vw, &vh, &vo, &eye, block + 1, nfill);
        }
    }
#else
    /*
     * No OpenMP compiled in, always do serial execution.
     */
    RenderBlock (&vw, &vh, &vo, &eye, 0, height_);
#endif /* _OPENMP */
}
