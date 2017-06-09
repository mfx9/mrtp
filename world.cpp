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


World::World (char *filename) {
    /* . Initialize. */
    nplanes    = 0;
    nspheres   = 0;
    ncylinders = 0;

    planes     = NULL;
    spheres    = NULL;
    cylinders  = NULL;
    /*
        Parsing file...
    */

    /* . Create temporary world. */
    Color black (0., 0., 0.);
    Color  blue (0., 0., 1.);
    Color green (0., 1., 0.);
    Color white (1., 1., 1.);
    Color   red (1., 0., 0.);


    Vector vcam (15., 0., 10.), vlook (0., 0., 0.);
    camera = new Camera (&vcam, &vlook, 
        800, 600, 70., 0.);

    Vector vlight (5., -5., 5.);
    light = new Light (&vlight);

    buffer = new Buffer (800, 600);

    Vector pcenter (0., 0., 0.), pnormal (0., 0., 1.);
    AddPlane (&pcenter, &pnormal, &blue, &green, 2.);

    Vector pcenter1 (-10., 0., 0.), pnormal1 (1., 0., 0.5);
    AddPlane (&pcenter1, &pnormal1, &red, &white, 2.);


    Vector scenter (0., -3., 1.);
    AddSphere (&scenter, 1., &white);

    Vector scenter1 (4., -3., 1.);
    AddSphere (&scenter1, 1., &white);

    Vector scenter2 (8., -3., 1.);
    AddSphere (&scenter2, 1., &white);


    Vector scenter3 (0., 3., 1.);
    AddSphere (&scenter3, 1., &white);

    Vector scenter4 (4., 3., 1.);
    AddSphere (&scenter4, 1., &white);

    Vector scenter5 (8., 3., 1.);
    AddSphere (&scenter5, 1., &white);
}

World::~World () {
    delete camera;
    delete light;
    delete buffer;

    /* . Clear all planes. */
    while (PopPlane ());

    /* . Clear all spheres. */
    while (PopSphere ());
}

unsigned int World::PopPlane () {
    Plane *prev, *next, *last;

    if (nplanes > 0) {
        last = planes;
        prev = NULL;
        while ((next = last->GetNext ()) != NULL) {
            prev = last;
            last = next;
        }
        if (prev != NULL)
            prev->SetNext (NULL);
        delete last;
        nplanes--;
    }
    /* . Returns zero if there are no planes left. */ 
    return nplanes;
}

unsigned int World::PopSphere () {
    Sphere *prev, *next, *last;

    if (nspheres > 0) {
        last = spheres;
        prev = NULL;
        while ((next = last->GetNext ()) != NULL) {
            prev = last;
            last = next;
        }
        if (prev != NULL)
            prev->SetNext (NULL);
        delete last;
        nspheres--;
    }
    /* . Returns zero if there are no spheres left. */ 
    return nspheres;
}

unsigned int World::AddPlane (Vector *center, Vector *normal,
        Color *colora, Color *colorb, double texscale) {
    Plane *next, *last, *plane;

    plane = new Plane (center, normal, colora, 
        colorb, texscale);
    if (nplanes < 1) {
        planes = plane;
    }
    else {
        next = planes;
        do {
            last = next;
            next = last->GetNext ();
        } while (next != NULL);
        last->SetNext (plane);
    }
    return (++nplanes);
}

unsigned int World::AddSphere (Vector *center, double radius,
        Color *color) {
    Sphere *next, *last, *sphere;

    sphere = new Sphere (center, radius, color);
    if (nspheres < 1) {
        spheres = sphere;
    }
    else {
        next = spheres;
        do {
            last = next;
            next = last->GetNext ();
        } while (next != NULL);
        last->SetNext (sphere);
    }
    return (++nspheres);
}

void World::TraceRay (Vector *origin, Vector *direction,
        Color *color) {
    Plane   *plane, *hitplane;
    Sphere  *sphere, *hitsphere;
    double  dist, currd, dot, raylen, fade;
    char    hit;
    Vector  inter, tl, normal;
    Color   objcol;
    bool    isshadow;

    /* . Initialize. */
    color->Set (0., 0., 0.);
    currd  = MAX_DISTANCE;
    hit    = HIT_NULL;

    /* . Search for planes. */
    plane    = planes;
    hitplane = NULL;
    while (plane != NULL) {
        dist = plane->Solve (origin, direction, 0., MAX_DISTANCE);
        if ((dist > 0.) && (dist < currd)) {
            currd    = dist;
            hitplane = plane;
            hit      = HIT_PLANE;
        }
        plane = plane->GetNext ();
    }

    /* . Search for spheres. */
    sphere    = spheres;
    hitsphere = NULL;
    while (sphere != NULL) {
        dist  = sphere->Solve (origin, direction, 0., MAX_DISTANCE);
        if ((dist > 0.) && (dist < currd)) {
            currd     = dist;
            hitsphere = sphere;
            hit       = HIT_SPHERE;
        }
        sphere = sphere->GetNext ();
    }

    /* . Search for cylinders. 

        Skip for now. */

    if (hit != HIT_NULL) {
        /*
         * . Intersection of the ray and object.
         */
        inter = (*(direction) * currd) + (*(origin));

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
                break;
        }

        /*
         * . Find a vector between the intersection
         *  and light.
         */
        light->GetToLight (&inter, &tl);
        tl.Normalize_InPlace ();
        dot = normal * tl;
        raylen = tl.Len ();

        /*
         * . Planes cannot cast shadows so check
         *    only for spheres.
         */
        isshadow = false;

        sphere = spheres;
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

        if (isshadow)
            dot *= SHADOW_FACTOR;
        fade = (1. / sqr (MAX_DISTANCE)) * sqr (currd);

        objcol.Scale_InPlace (dot);
        /*
         * . Put a pixel in the frame buffer.
         */
        objcol.CopyTo (color); 
    }
}

void World::Render () {
    Vector vw, vh, vo;
    camera->CalculateVectors (&vw, &vh, &vo);

    unsigned int width, heigth;
    camera->GetDimensions (&width, &heigth);

    Vector eye;
    camera->GetEye (&eye);

    Color *bp;
    bp = buffer->GetPointer ();

    Vector horiz, verti, origin, 
        direction;
    unsigned int i, j;

    for (j = 0; j < heigth; j++) {
        for (i = 0; i < width; i++) {
            horiz  = vw * (double) i;
            verti  = vh * (double) j;
            origin = vo + horiz + verti;

            direction = origin - eye;
            direction.Normalize_InPlace ();

            TraceRay (&origin, &direction, bp++);
        }
    }
}

void World::WritePNG (char *filename) {
    Color blue (0., 0., 1.);

    buffer->Text ("BUFFER TEST.", 0, 0, &blue);
    buffer->WriteToPNG (filename);
}
