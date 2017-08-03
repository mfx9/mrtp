/*
 *  Mrtp: A simple raytracing tool.
 *  Copyright (C) 2017  Mikolaj Feliks <mikolaj.feliks@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "world.hpp"


World::World (Parser *parser, unsigned int width,
        unsigned int height, double fov,
        double distance, double shadowfactor, 
        char lightmodel) {
    /* 
     * Initialize. 
     */
    nplanes_    = 0;
    nspheres_   = 0;
    ncylinders_ = 0;

    /*
     * Set all pointers to NULL.
     */
    planes_     = NULL;
    spheres_    = NULL;
    cylinders_  = NULL;
    buffer_     = NULL;
    camera_     = NULL;
    light_      = NULL;

    maxdist_  = distance;
    shadow_   = shadowfactor;
    model_    = lightmodel;

    parser_   = parser;
    width_    = width;
    height_   = height;
    fov_      = fov;
}

bool World::Initialize () {
    Entry   entry;
    string  label;
    unsigned int nentries;

    string  key,
        texts[MAX_COMPONENTS];
    double  reals[MAX_COMPONENTS];
    char    type;
    unsigned int i;

    /*
     * Allocate a buffer.
     */
    buffer_ = new Buffer (width_, height_);
    buffer_->Allocate ();

    /*
     * Allocate camera, light, actors, etc.
     */
    do {
        nentries = parser_->PopEntry (&entry);
        entry.GetLabel (&label);
        i = MAX_LINES;

        /* DEBUG
        entry.Print (); */

        /*
         * Add a camera.
         */
        if (label == "camera") {
            Vector position, target;
            double roll;

            while (entry.GetData (&key, &type, reals, texts, &i)) {
                if (key == "position") {
                    position.Set (reals);
                }
                else if (key == "target") {
                    target.Set (reals);
                }
                else {  /* if (key == "roll") */
                    roll = reals[0];
                }
            }
            camera_ = new Camera (&position, &target, width_, 
                height_, fov_, roll);
        }

        /*
         * Add a light.
         */
        else if (label == "light") {
            Vector position;

            entry.GetData (&key, &type, reals, texts, &i);
            position.Set (reals);
            light_ = new Light (&position);
        }

        /*
         * Add a plane.
         */
        else if (label == "plane") {
            Vector  center, normal;
            Color   ca, cb;
            double  scale;

            while (entry.GetData (&key, &type, reals, texts, &i)) {
                if (key == "center") {
                    center.Set (reals);
                }
                else if (key == "normal") {
                    normal.Set (reals);
                }
                else if (key == "scale") {
                    scale = reals[0];
                }
                else if (key == "cola") {
                    ca.Set ((float) reals[0], (float) reals[1], 
                        (float) reals[2]);
                }
                else {  /* if (key == "colb") */
                    cb.Set ((float) reals[0], (float) reals[1], 
                        (float) reals[2]);
                }
            }
            AddPlane (&center, &normal, &ca, &cb, scale);
        }

        /*
         * Add a sphere.
         */
        else if (label == "sphere") {
            Vector  position;
            double  radius;
            Color   color;

            while (entry.GetData (&key, &type, reals, texts, &i)) {
                if (key == "position") {
                    position.Set (reals);
                }
                else if (key == "radius") {
                    radius = reals[0];
                }
                else {  /* if (key == "color") */
                    color.Set ((float) reals[0], (float) reals[1], 
                        (float) reals[2]);
                }
            }
            AddSphere (&position, radius, &color);
        }

        /*
         * Add a cylinder.
         */
        else if (label == "cylinder") {
            Vector  axisa, axisb;
            double  radius;
            Color   color;

            while (entry.GetData (&key, &type, reals, texts, &i)) {
                if (key == "a") {
                    axisa.Set (reals);
                }
                else if (key == "b") {
                    axisb.Set (reals);
                }
                else if (key == "radius") {
                    radius = reals[0];
                }
                else {  /* if (key == "color") */
                    color.Set ((float) reals[0], (float) reals[1], 
                        (float) reals[2]);
                }
            }
            AddCylinder (&axisa, &axisb, radius, &color);
        }
    } while (nentries > 0);

    return true;
}

World::~World () {
    if (camera_ != NULL) {
        delete camera_;
    }
    if (light_  != NULL) {
        delete light_;
    }
    if (buffer_ != NULL) {
        delete buffer_;
    }

    /* Clear all planes. */
    do {
        PopPlane ();
    } while (nplanes_ > 0);

    /* Clear all spheres. */
    do {
        PopSphere ();
    } while (nspheres_ > 0);

    /* Clear all cylinders. */
    do {
        PopCylinder ();
    } while (ncylinders_ > 0);
}

unsigned int World::PopPlane () {
    Plane *prev, *next, *last;

    if (nplanes_ > 0) {
        last = planes_;
        prev = NULL;
        while ((next = last->GetNext ()) != NULL) {
            prev = last;
            last = next;
        }
        if (prev != NULL) {
            prev->SetNext (NULL);
        }
        delete last;
        nplanes_--;
    }
    return nplanes_;
}

unsigned int World::PopSphere () {
    Sphere *prev, *next, *last;

    if (nspheres_ > 0) {
        last = spheres_;
        prev = NULL;
        while ((next = last->GetNext ()) != NULL) {
            prev = last;
            last = next;
        }
        if (prev != NULL) {
            prev->SetNext (NULL);
        }
        delete last;
        nspheres_--;
    }
    return nspheres_;
}

unsigned int World::PopCylinder () {
    Cylinder *prev, *next, *last;

    if (ncylinders_ > 0) {
        last = cylinders_;
        prev = NULL;
        while ((next = last->GetNext ()) != NULL) {
            prev = last;
            last = next;
        }
        if (prev != NULL) {
            prev->SetNext (NULL);
        }
        delete last;
        ncylinders_--;
    }
    return ncylinders_;
}

unsigned int World::AddPlane (Vector *center, Vector *normal,
        Color *colora, Color *colorb, double texscale) {
    Plane *next, *last, *plane;

    plane = new Plane (center, normal, colora, 
        colorb, texscale);
    if (nplanes_ < 1) {
        planes_ = plane;
    }
    else {
        next = planes_;
        do {
            last = next;
            next = last->GetNext ();
        } while (next != NULL);
        last->SetNext (plane);
    }
    return (++nplanes_);
}

unsigned int World::AddSphere (Vector *center, double radius,
        Color *color) {
    Sphere *next, *last, *sphere;

    sphere = new Sphere (center, radius, color);
    if (nspheres_ < 1) {
        spheres_ = sphere;
    }
    else {
        next = spheres_;
        do {
            last = next;
            next = last->GetNext ();
        } while (next != NULL);
        last->SetNext (sphere);
    }
    return (++nspheres_);
}

unsigned int World::AddCylinder (Vector *A, Vector *B, 
        double radius, Color *color) {
    Cylinder *next, *last, *cylinder;

    cylinder = new Cylinder (A, B, radius, color);
    if (ncylinders_ < 1) {
        cylinders_ = cylinder;
    }
    else {
        next = cylinders_;
        do {
            last = next;
            next = last->GetNext ();
        } while (next != NULL);
        last->SetNext (cylinder);
    }
    return (++ncylinders_);
}

void World::TraceRay (Vector *origin, Vector *direction,
        Color *color) {
    Plane    *plane, *hitplane;
    Sphere   *sphere, *hitsphere;
    Cylinder *cylinder, *hitcylinder;
    double    dist, currd, dot, raylen, fade;
    char      hit;
    Vector    inter, tl, normal;
    Color     objcol;
    bool      isshadow;

    /*
     * Initialize.
     */
    color->Zero ();
    currd  = maxdist_;
    hit    = HIT_NULL;
    /*
     * Search for planes.
     */
    plane    = planes_;
    hitplane = NULL;
    while (plane != NULL) {
        dist = plane->Solve (origin, direction, 0., maxdist_);
        if ((dist > 0.) && (dist < currd)) {
            currd    = dist;
            hitplane = plane;
            hit      = HIT_PLANE;
        }
        plane = plane->GetNext ();
    }

    /*
     * Search for spheres.
     */
    sphere    = spheres_;
    hitsphere = NULL;
    while (sphere != NULL) {
        dist  = sphere->Solve (origin, direction, 0., maxdist_);
        if ((dist > 0.) && (dist < currd)) {
            currd     = dist;
            hitsphere = sphere;
            hit       = HIT_SPHERE;
        }
        sphere = sphere->GetNext ();
    }

    /*
     * Search for cylinders. 
     */
    cylinder    = cylinders_;
    hitcylinder = NULL;
    while (cylinder != NULL) {
        dist  = cylinder->Solve (origin, direction, 0., maxdist_);
        if ((dist > 0.) && (dist < currd)) {
            currd       = dist;
            hitcylinder = cylinder;
            hit         = HIT_CYLINDER;
        }
        cylinder = cylinder->GetNext ();
    }

    if (hit != HIT_NULL) {
        /*
         * Found intersection of the current ray 
         *   and an object.
         */
        inter = ((*direction) * currd) + (*origin);

        switch (hit) {
            case HIT_PLANE:
                hitplane->GetNormal (&normal);
                hitplane->DetermineColor (&inter, &objcol);
                break;
            case HIT_SPHERE:
                hitsphere->GetNormal (&inter, &normal);
                hitsphere->DetermineColor (&objcol);
                break;
            case HIT_CYLINDER:
                hitcylinder->GetNormal (&inter, &normal);
                hitcylinder->DetermineColor (&objcol);
                break;
        }
        /*
         * Find a vector between the intersection
         *   and light.
         *
         */
        light_->GetToLight (&inter, &tl);
        raylen = tl.Len ();
        tl.Normalize_InPlace ();
        dot = normal * tl;

        /*
         * Planes cannot cast shadows so check only for
         * spheres and cylinder.
         *
         */
        isshadow = false;
        sphere   = spheres_;
        while (sphere != NULL) {
            if (sphere != hitsphere) {
                dist = sphere->Solve (&inter, &tl, 0., raylen);
                if (dist > 0.) {
                    isshadow = true;
                    break;
                }
            }
            sphere = sphere->GetNext ();
        }
        if (!isshadow) {
            cylinder   = cylinders_;
            while (cylinder != NULL) {
                if (cylinder != hitcylinder) {
                    dist = cylinder->Solve (&inter, &tl, 0., raylen);
                    if (dist > 0.) {
                        isshadow = true;
                        break;
                    }
                }
                cylinder = cylinder->GetNext ();
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
        if (model_ == LIGHT_MODEL_LINEAR) {
            fade = 1.0 - (raylen / maxdist_);
        }
        else if (model_ == LIGHT_MODEL_QUADRATIC) {
            fade = 1.0 - sqr (raylen / maxdist_);
        }
        else {  /* if (model_ == LIGHT_MODEL_NONE) */
            fade = 1.0;
        }
        dot *= fade;
        objcol.Scale_InPlace (dot);
        /*
         * Put a pixel in the frame buffer.
         */
        objcol.CopyTo (color); 
    }
}

void World::Render () {
    Color *bp;
    Vector vw, vh, vo, eye,
        horiz, verti, origin, direction;
    unsigned int i, j;

    /*
     * Initialize.
     */
    camera_->CalculateVectors (&vw, &vh, &vo);
    camera_->GetEye (&eye);

    bp = buffer_->GetPointer ();
    /*
     * Trace a ray for every pixel.
     */
    for (j = 0; j < height_; j++) {
        for (i = 0; i < width_; i++) {
            horiz     = vw * (double) i;
            verti     = vh * (double) j;
            origin    = vo + horiz + verti;
            direction = origin - eye;
            direction.Normalize_InPlace ();

            TraceRay (&origin, &direction, bp++);
        }
    }
}

void World::WritePNG (string *filename) {
    /*
     * Save image.
     */
    buffer_->WriteToPNG (filename);
}
