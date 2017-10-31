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
#include "world.hpp"

using namespace std;


World::World (Parser *parser) {
    /* 
     * Initialize. 
     */
    nplanes_    = 0;
    nspheres_   = 0;
    ncylinders_ = 0;
    ntextures_  = 0;

    planes_     = NULL;
    spheres_    = NULL;
    cylinders_  = NULL;
    textures_   = NULL;

    camera_     = NULL;
    light_      = NULL;
    parser_     = parser;
}

void World::Initialize () {
    Entry   entry;
    string  label;

    string  key,
        texts[MAX_COMPONENTS];
    double  reals[MAX_COMPONENTS];
    ParserParameter_t  type;

    /*
     * Allocate camera, light, actors, etc.
     */
    parser_->StartQuery ();
    while (parser_->Query (&entry)) {

        entry.GetLabel (&label);
        entry.StartQuery ();

        /*
         * Add a camera.
         */
        if (label == "camera") {
            Vector position, target;
            double roll;

            while (entry.Query (&key, &type, reals, texts)) {
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
            camera_ = new Camera (&position, &target, roll);
        }

        /*
         * Add a light.
         */
        else if (label == "light") {
            Vector position;

            entry.Query (&key, &type, reals, texts);
            position.Set (reals);
            light_ = new Light (&position);
        }

        /*
         * Add a plane.
         */
        else if (label == "plane") {
            Vector    center, normal;
            double    scale;
            Color     color;
            Texture  *texture = NULL;

            while (entry.Query (&key, &type, reals, texts)) {
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
                    color.Set ((float) reals[0], (float) reals[1], 
                        (float) reals[2]);
                }
                else {  /* if (key == "texture") */
                    texture = AddTexture (&texts[0]);
                }
            }
            AddPlane (&center, &normal, scale, &color, texture);
        }

        /*
         * Add a sphere.
         */
        else if (label == "sphere") {
            Vector   position, axis;
            double   radius;
            Color    color;
            Texture *texture = NULL;

            while (entry.Query (&key, &type, reals, texts)) {
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
                    color.Set ((float) reals[0], (float) reals[1], 
                        (float) reals[2]);
                }
                else {  /* if (key == "texture") */
                    texture = AddTexture (&texts[0]);
                }
            }
            AddSphere (&position, radius, &axis, &color, texture);
        }

        /*
         * Add a cylinder.
         */
        else if (label == "cylinder") {
            Vector    center, direction;
            double    radius, span;
            Color     color;
            Texture  *texture = NULL;

            while (entry.Query (&key, &type, reals, texts)) {
                if (key == "center") {
                    center.Set (reals);
                }
                else if (key == "direction") {
                    direction.Set (reals);
                }
                else if (key == "radius") {
                    radius = reals[0];
                }
                else if (key == "span") {
                    span = reals[0];
                }
                else if (key == "color") {
                    color.Set ((float) reals[0], (float) reals[1], 
                        (float) reals[2]);
                }
                else {  /* if (key == "texture") */
                    texture = AddTexture (&texts[0]);
                }
            }
            AddCylinder (&center, &direction, radius, span, 
                &color, texture);
        }
    }
}

World::~World () {
    if (camera_ != NULL) {
        delete camera_;
    }
    if (light_  != NULL) {
        delete light_;
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
        while ((next = last->Next ()) != NULL) {
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
        while ((next = last->Next ()) != NULL) {
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
        while ((next = last->Next ()) != NULL) {
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
        while ((next = last->Next ()) != NULL) {
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
        double texscale, Color *color, Texture *texture) {
    Plane *next, *last, *plane;

    plane = new Plane (center, normal, texscale, 
        color, texture);
    if (nplanes_ < 1) {
        planes_ = plane;
    }
    else {
        next = planes_;
        do {
            last = next;
            next = last->Next ();
        } while (next != NULL);
        last->SetNext (plane);
    }
    nplanes_++;
}

void World::AddSphere (Vector *center, double radius,
        Vector *axis, Color *color, Texture *texture) {
    Sphere *next, *last, *sphere;

    sphere = new Sphere (center, radius, axis, 
        color, texture);
    if (nspheres_ < 1) {
        spheres_ = sphere;
    }
    else {
        next = spheres_;
        do {
            last = next;
            next = last->Next ();
        } while (next != NULL);
        last->SetNext (sphere);
    }
    nspheres_++;
}

void World::AddCylinder (Vector *center, Vector *direction, 
        double radius, double span, Color *color, Texture *texture) {
    Cylinder *next, *last, *cylinder;

    cylinder = new Cylinder (center, direction, radius,
        span, color, texture);
    if (ncylinders_ < 1) {
        cylinders_ = cylinder;
    }
    else {
        next = cylinders_;
        do {
            last = next;
            next = last->Next ();
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
            next = last->Next ();
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
            next = last->Next ();
        } while (next != NULL);
        last->SetNext (texture);
    }
    ntextures_++;
    return texture;
}

void World::GetCamera (Camera **camera) {
    *camera = camera_;
}

void World::GetLight (Light **light) {
    *light = light_;
}

void World::GetActors (Plane **planes, Sphere **spheres, 
        Cylinder **cylinders) {
    *planes = planes_;
    *spheres = spheres_;
    *cylinders = cylinders_;
}
