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
        LightModel_t lightmodel) {
    /* 
     * Initialize. 
     */
    nplanes_    = 0;
    nspheres_   = 0;
    ncylinders_ = 0;
    ntextures_  = 0;

    /*
     * Set all pointers to NULL.
     */
    planes_     = NULL;
    spheres_    = NULL;
    cylinders_  = NULL;
    textures_   = NULL;
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

        /* DEBUG
        entry.Print (); */

        /*
         * Add a camera.
         */
        if (label == "camera") {
            Vector position, target;
            double roll;

            while (entry.PopData (&key, &type, reals, texts)) {
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

            entry.PopData (&key, &type, reals, texts);
            position.Set (reals);
            light_ = new Light (&position);
        }

        /*
         * Add a plane.
         */
        else if (label == "plane") {
            Vector    center, normal;
            double    scale;
            Texture  *texture;

            while (entry.PopData (&key, &type, reals, texts)) {
                if (key == "center") {
                    center.Set (reals);
                }
                else if (key == "normal") {
                    normal.Set (reals);
                }
                else if (key == "scale") {
                    scale = reals[0];
                }
                else if (key == "color") {
                }
                else {  /* if (key == "texture") */
                    texture = AddTexture (&texts[0]);
                }
            }
            AddPlane (&center, &normal, scale, texture);
        }

        /*
         * Add a sphere.
         */
        else if (label == "sphere") {
            Vector   position, axis;
            double   radius;
            Texture *texture;

            while (entry.PopData (&key, &type, reals, texts)) {
                if (key == "position") {
                    position.Set (reals);
                }
                else if (key == "radius") {
                    radius = reals[0];
                }
                else if (key == "axis") {
                    axis.Set (reals);
                }
                else if (key == "color") {
                }
                else {  /* if (key == "texture") */
                    texture = AddTexture (&texts[0]);
                }
            }
            AddSphere (&position, radius, &axis, texture);
        }

        /*
         * Add a cylinder.
         */
        else if (label == "cylinder") {
            Vector    origin, direction;
            double    radius;
            Texture  *texture;

            while (entry.PopData (&key, &type, reals, texts)) {
                if (key == "center") {
                    origin.Set (reals);
                }
                else if (key == "direction") {
                    direction.Set (reals);
                }
                else if (key == "radius") {
                    radius = reals[0];
                }
                else if (key == "color") {
                }
                else {  /* if (key == "texture") */
                    texture = AddTexture (&texts[0]);
                }
            }
            AddCylinder (&origin, &direction, radius, texture);
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

    /* Clear all textures. */
    do {
        PopTexture ();
    } while (ntextures_ > 0);
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

unsigned int World::PopTexture () {
    Texture *prev, *next, *last;

    if (ntextures_ > 0) {
        last = textures_;
        prev = NULL;
        while ((next = last->GetNext ()) != NULL) {
            prev = last;
            last = next;
        }
        if (prev != NULL) {
            prev->SetNext (NULL);
        }
        delete last;
        ntextures_--;
    }
    return ntextures_;
}

void World::AddPlane (Vector *center, Vector *normal, 
        double texscale, Texture *texture) {
    Plane *next, *last, *plane;

    plane = new Plane (center, normal, texscale, 
        texture);
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
    nplanes_++;
}

void World::AddSphere (Vector *center, double radius,
        Vector *axis, Texture *texture) {
    Sphere *next, *last, *sphere;

    sphere = new Sphere (center, radius, axis, 
        texture);
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
    nspheres_++;
}

void World::AddCylinder (Vector *origin, Vector *direction, 
        double radius, Texture *texture) {
    Cylinder *next, *last, *cylinder;

    cylinder = new Cylinder (origin, direction, radius,
        texture);
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
    ncylinders_++;
}

Texture *World::AddTexture (string *filename) {
    Texture *next, *last, *texture;
    bool  found;

    /*
     * Do not add a texture that already exists. 
     */
    if (ntextures_ > 0) {
        next  = textures_;
        found = false;
        do {
            last = next;
            if (last->CheckFilename (filename)) {
                found = true;
                break;
            }
            next = last->GetNext ();
        } while (next != NULL);
        if (found) {
            return last;
        }
    }

    /*
     * Create a new texture.
     */
    texture = new Texture (filename);
    texture->Allocate ();

    if (ntextures_ < 1) {
        textures_ = texture;
    }
    else {
        next = textures_;
        do {
            last = next;
            next = last->GetNext ();
        } while (next != NULL);
        last->SetNext (texture);
    }
    ntextures_++;
    return texture;
}

void World::TraceRay (Vector *origin, Vector *direction,
        Color *color) {
    Plane    *plane, *hitplane;
    Sphere   *sphere, *hitsphere;
    Cylinder *cylinder, *hitcylinder;
    double    dist, currd, dot, raylen, fade;
    HitCode_t hit;
    Vector    inter, tl, normal;
    Color     objcol;
    bool      isshadow;

    /*
     * Initialize.
     */
    color->Zero ();
    currd  = maxdist_;
    hit    = hitNull;

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
            hit      = hitPlane;
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
            hit       = hitSphere;
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
            hit         = hitCylinder;
        }
        cylinder = cylinder->GetNext ();
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
            hitcylinder->DetermineColor (&normal, &objcol);
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
        if (model_ == lightLinear) {
            fade = 1.0 - (raylen / maxdist_);
        }
        else if (model_ == lightQuadratic) {
            fade = 1.0 - sqr (raylen / maxdist_);
        }
        else {  /* if (model_ == lightNone) */
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
