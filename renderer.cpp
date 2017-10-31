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
        LightModel_t lightmodel, unsigned int nthreads) {
    world_ = world;
    buffer_ = NULL;
    width_ = width;
    height_ = height;
    fov_ = fov;
    maxdist_ = distance;
    shadow_ = shadowfactor;
    model_ = lightmodel;
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

void Renderer::TraceRay (Vector *origin, Vector *direction, 
        Color *color) {
    Light     *light;
    Plane     *planes, *plane, *hitplane;
    Sphere    *spheres, *sphere, *hitsphere;
    Cylinder  *cylinders, *cylinder, *hitcylinder;
    double     dist, currd, dot, raylen, fade;
    HitCode_t  hit;
    Vector     inter, tl, normal;
    Color      objcol;
    bool       isshadow;

    /*
     * Initialize.
     */
    world_->GetLight (&light);
    world_->GetActors (&planes, &spheres, &cylinders);

    color->Zero ();
    currd  = maxdist_;
    hit    = hitNull;

    /*
     * Search for planes.
     */
    plane    = planes;
    hitplane = NULL;
    while (plane != NULL) {
        dist = plane->Solve (origin, direction, 0.0f, maxdist_);
        if ((dist > 0.0f) && (dist < currd)) {
            currd    = dist;
            hitplane = plane;
            hit      = hitPlane;
        }
        plane = plane->Next ();
    }

    /*
     * Search for spheres.
     */
    sphere    = spheres;
    hitsphere = NULL;
    while (sphere != NULL) {
        dist  = sphere->Solve (origin, direction, 0.0f, maxdist_);
        if ((dist > 0.0f) && (dist < currd)) {
            currd     = dist;
            hitsphere = sphere;
            hit       = hitSphere;
        }
        sphere = sphere->Next ();
    }

    /*
     * Search for cylinders. 
     */
    cylinder    = cylinders;
    hitcylinder = NULL;
    while (cylinder != NULL) {
        dist  = cylinder->Solve (origin, direction, 0.0f, maxdist_);
        if ((dist > 0.0f) && (dist < currd)) {
            currd       = dist;
            hitcylinder = cylinder;
            hit         = hitCylinder;
        }
        cylinder = cylinder->Next ();
    }

    if (hit != hitNull) {
        /*
         * Found a ray/object intersection.
         */
        inter = ((*direction) * currd) + (*origin);

        if (hit == hitPlane) {
            hitplane->GetNormal (&normal);
            hitplane->DetermineColor (&inter, &objcol);
        }
        else if (hit == hitSphere) {
            hitsphere->GetNormal (&inter, &normal);
            hitsphere->DetermineColor (&normal, &objcol);
        }
        else if (hit == hitCylinder) {
            hitcylinder->GetNormal (&inter, &normal);
            hitcylinder->DetermineColor (&inter, &normal, &objcol);
        }
        /*
         * Find a vector between the intersection
         *   and light.
         *
         */
        light->GetToLight (&inter, &tl);
        raylen = tl.Len ();
        tl.Normalize_InPlace ();
        dot = normal * tl;

        /*
         * Planes cannot cast shadows so check only for
         * spheres and cylinder.
         *
         */
        isshadow = false;
        sphere   = spheres;
        while (sphere != NULL) {
            if (sphere != hitsphere) {
                dist = sphere->Solve (&inter, &tl, 0.0f, raylen);
                if (dist > 0.0f) {
                    isshadow = true;
                    break;
                }
            }
            sphere = sphere->Next ();
        }
        if (!isshadow) {
            cylinder   = cylinders;
            while (cylinder != NULL) {
                if (cylinder != hitcylinder) {
                    dist = cylinder->Solve (&inter, &tl, 0.0f, raylen);
                    if (dist > 0.0f) {
                        isshadow = true;
                        break;
                    }
                }
                cylinder = cylinder->Next ();
            }
        }

        if (isshadow) {
            dot *= shadow_;
        }
        /*
         * Decrease light intensity for objects further
         * away from the light.
         *
         */
        if (model_ == lightLinear) {
            fade = 1.0f - (raylen / maxdist_);
        }
        else if (model_ == lightQuadratic) {
            fade = 1.0f - pow (raylen / maxdist_, 2);
        }
        else {  /* if (model_ == lightNone) */
            fade = 1.0f;
        }
        dot *= fade;
        objcol.Scale_InPlace (dot);
        /*
         * Put a pixel in the frame buffer.
         */
        objcol.CopyTo (color); 
    }
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

            TraceRay (&origin, &direction, color);
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
