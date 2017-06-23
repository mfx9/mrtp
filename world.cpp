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


World::World (Parser *parser) {
    /* 
     * Initialize. 
     */
    nplanes    = 0;
    nspheres   = 0;
    ncylinders = 0;

    planes     = NULL;
    spheres    = NULL;
    cylinders  = NULL;

    buffer = NULL;
    camera = NULL;
    light  = NULL;


    buffer = new Buffer (800, 600);

    Entry entry;
    string label;

    while (parser->PopEntry (&entry)) {
        entry.GetLabel (&label);
        #ifdef DEBUG_WORLD
        cout << "World: Reading entry \"" << label << "\"" << endl;
        #endif
        if (label == "camera")
            AddCamera_FromEntry (&entry);
        else if (label == "light")
            AddLight_FromEntry (&entry);
        else if (label == "plane")
            AddPlane_FromEntry (&entry);
        else if (label == "sphere")
            AddSphere_FromEntry (&entry);
    }
}

World::~World () {
    if (camera != NULL)
        delete camera;
    if (light  != NULL)
        delete light;
    if (buffer != NULL)
        delete buffer;

    /* Clear all planes. */
    while (PopPlane ());

    /* Clear all spheres. */
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
        #ifdef DEBUG_WORLD
        cout << "World: Removed plane." << endl;
        #endif
    }
    /* Returns zero if there are no planes left. */ 
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
        #ifdef DEBUG_WORLD
        cout << "World: Removed sphere." << endl;
        #endif
    }
    /* Returns zero if there are no spheres left. */ 
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
    #ifdef DEBUG_WORLD
    cout << "World: Added plane." << endl;
    #endif
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
    #ifdef DEBUG_WORLD
    cout << "World: Added sphere." << endl;
    #endif
    return (++nspheres);
}

unsigned int World::AddCamera_FromEntry (Entry *entry) {
    unsigned int i = 999;
    double  value;
    string  key;
    double  x0, y0, z0, lx, ly, lz, rot;

    if (camera == NULL) {
        while (entry->GetPair (&key, &value, &i)) {
            if      (key == LABEL_CAMERA_X   )  x0  = value;
            else if (key == LABEL_CAMERA_Y   )  y0  = value;
            else if (key == LABEL_CAMERA_Z   )  z0  = value;
            else if (key == LABEL_CAMERA_LX  )  lx  = value;
            else if (key == LABEL_CAMERA_LY  )  ly  = value;
            else if (key == LABEL_CAMERA_LZ  )  lz  = value;
            else if (key == LABEL_CAMERA_ROT )  rot = value;
        }
        Vector position (x0, y0, z0),
            lookat (lx, ly, lz);
        /* FIXME */
        camera = new Camera (&position, &lookat, 
            800, 600, 70., rot);
    }
    return 1;
}

unsigned int World::AddLight_FromEntry (Entry *entry) {
    unsigned int i = 999;
    double  value;
    string  key;
    double  x0, y0, z0;

    if (light == NULL) {
        while (entry->GetPair (&key, &value, &i)) {
            if      (key == LABEL_LIGHT_X  )  x0 = value;
            else if (key == LABEL_LIGHT_Y  )  y0 = value;
            else if (key == LABEL_LIGHT_Z  )  z0 = value;
        }
        Vector position (x0, y0, z0);
        light = new Light (&position);
    }
    return 1;
}

unsigned int World::AddPlane_FromEntry (Entry *entry) {
    unsigned int i = 999;
    double  value;
    string  key;
    double  x0, y0, z0, A, B, C, scale;
    float   cola_r, cola_g, cola_b, colb_r, 
                colb_g, colb_b;
    while (entry->GetPair (&key, &value, &i)) {
        if      (key == LABEL_PLANE_X0  )  x0 = value;
        else if (key == LABEL_PLANE_Y0  )  y0 = value;
        else if (key == LABEL_PLANE_Z0  )  z0 = value;
        else if (key == LABEL_PLANE_A   )  A  = value;
        else if (key == LABEL_PLANE_B   )  B  = value;
        else if (key == LABEL_PLANE_C   )  C  = value;
        else if (key == LABEL_PLANE_SCALE   )  scale  = value; 
        else if (key == LABEL_PLANE_COLA_R  )  cola_r = (float) value;
        else if (key == LABEL_PLANE_COLA_G  )  cola_g = (float) value;
        else if (key == LABEL_PLANE_COLA_B  )  cola_b = (float) value;
        else if (key == LABEL_PLANE_COLB_R  )  colb_r = (float) value;
        else if (key == LABEL_PLANE_COLB_G  )  colb_g = (float) value;
        else if (key == LABEL_PLANE_COLB_B  )  colb_b = (float) value;
    }
    Vector center (x0, y0, z0);
    Vector normal (A, B, C);
    Color cola (cola_r, cola_g, cola_b), 
        colb (colb_r, colb_g, colb_b);
    return AddPlane (&center, &normal, &cola, &colb, scale);
}

unsigned int World::AddSphere_FromEntry (Entry *entry) {
    unsigned int i = 999;
    double  value;
    string  key;
    double  x0, y0, z0, R;
    float   col_r, col_g, col_b;

    while (entry->GetPair (&key, &value, &i)) {
        if      (key == LABEL_SPHERE_X0  )  x0 = value;
        else if (key == LABEL_SPHERE_Y0  )  y0 = value;
        else if (key == LABEL_SPHERE_Z0  )  z0 = value;
        else if (key == LABEL_SPHERE_R   )  R  = value;
        else if (key == LABEL_SPHERE_COL_R  )  col_r = (float) value;
        else if (key == LABEL_SPHERE_COL_G  )  col_g = (float) value;
        else if (key == LABEL_SPHERE_COL_B  )  col_b = (float) value;
    }
    Vector center (x0, y0, z0);
    Color col (col_r, col_g, col_b);
    return AddSphere (&center, R, &col);
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

    /*
     * Initialize.
     */
    color->Set (0., 0., 0.);
    currd  = MAX_DISTANCE;
    hit    = HIT_NULL;
    /*
     * Search for planes.
     */
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

    /*
     * Search for spheres.
     */
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
                break;
        }
        /*
         * Find a vector between the intersection
         *   and light.
         *
         */
        light->GetToLight (&inter, &tl);
        tl.Normalize_InPlace ();
        dot = normal * tl;
        raylen = tl.Len ();

        /*
         * Planes cannot cast shadows so check
         *   only for spheres.
         *
         */
        isshadow = false;
        sphere   = spheres;
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
         * Put a pixel in the frame buffer.
         */
        objcol.CopyTo (color); 
    }
}

void World::Render () {
    Color *bp;
    Vector vw, vh, vo, eye,
        horiz, verti, origin, direction;
    unsigned int width, heigth, i, j;
    /*
     * Initialize.
     */
    camera->CalculateVectors (&vw, &vh, &vo);
    camera->GetDimensions (&width, &heigth);
    camera->GetEye (&eye);

    bp = buffer->GetPointer ();
    /*
     * Trace a ray for every pixel.
     */
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
