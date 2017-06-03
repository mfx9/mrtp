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


Camera::Camera (Vector &origin, Vector &target,
        unsigned int w, unsigned int h, double fov, 
        double rot) {
    origin.CopyTo (&eye);
    target.CopyTo (&lookat);
    width  = (double) w;
    heigth = (double) h;
    ratio  = w / h;
    perspective = ratio / (2. * tan (fov / 2. * M_PI / 180.));
    rotation = rot;
}

Camera::~Camera () {
}

void Camera::GetDimensions (unsigned int *w, 
        unsigned int *h) {
    *w = width;
    *h = heigth;
}

void Camera::GetEye (Vector *vector) {
    eye.CopyTo (vector);
}

void Camera::CalculateVectors (Vector *vw, Vector *vh,
        Vector *vo) {
    /*
     * . Method calculates vectors that span the window.
     */

    /*
     * . i is the vector between the camera and
     *  the center of the window.
     */
    Vector i;
    i = lookat - eye;
    i.Normalize_InPlace ();
#ifdef DEBUG_WORLD
    printf ("%s: Basis vectors\n", __FILE__);
    Vector bi (1., 0., 0.), bj (0., 1., 0.), 
        bk (0., 0., 1.);
    bi.PrintXX ();
    bj.PrintXX ();
    bk.PrintXX ();
    printf ("%s: Camera position and target\n", __FILE__);
    eye.PrintXX ();
    lookat.PrintXX ();
#endif
    /*
     * if (i.x == 0. && i.y == 0.)
     *     return false;
     */

    Vector j, k (0., 0., 1.);
    j = i ^ k;
    j.Normalize_InPlace ();
    /*
     * . Flip the cross-product instead?
     */
    j.Scale_InPlace (-1.);
    k = i ^ j;
    k.Normalize_InPlace ();
#ifdef DEBUG_WORLD
    Vector temp;
    printf ("%s: Camera coordinate system (i, j, k)\n", __FILE__);
    temp = eye + i;
    temp.PrintXX ();
    temp = eye + j;
    temp.PrintXX ();
    temp = eye + k;
    temp.PrintXX ();
#endif

    /*
     * . Apply camera rotation around
     *  the axis of i.
     */

    /*
     * . Calculate the central point of
     *  the window.
     */
    Vector center;
    center = (i * perspective) + eye;
#ifdef DEBUG_WORLD
    /*
    printf ("** Center of screen **\n");
    center.PrintXX ();
    */
#endif

    /*
     * . Modify vectors.
     */
    j.Scale_InPlace (ratio * .5);
    k.Scale_InPlace (.5);

    /*
     * . Find 3 corners of the window.
     */
    Vector wo, ww, wh;
    wo = center + j + k;
    ww = center - j + k;
    wh = center + j - k;
#ifdef DEBUG_WORLD
    /*
    printf ("** Corners of window **\n");
    wo.PrintXX ();
    ww.PrintXX ();
    wh.PrintXX ();
    */
#endif

    /*
     * . Find vectors spanning the window.
     */
    Vector horiz, verti;
    horiz = (ww - wo) * (1. / width );
    horiz.CopyTo (vw);
    verti = (wh - wo) * (1. / heigth);
    verti.CopyTo (vh);
    wo.CopyTo (vo);
}

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


    Vector vcam (15., 0., 5.), vlook (0., 0., 0.);
    camera = new Camera (vcam, vlook, 
        640, 480, 70., 0.);

    Vector vlight (10., 10., 5.);
    light = new Light (vlight);

    buffer = new Buffer (640, 480);

    Vector pcenter (0., 0., 0.), pnormal (0., 0., 1.);
    AddPlane (pcenter, pnormal, blue, green, 2.);

    /*
    Vector pcenter1 (-20., 0., 0.), pnormal1 (-1., 0., 0.5);
    AddPlane (pcenter1, pnormal1, red, white, 2.);
    */

    Vector scenter (0., 0., 1.);
    AddSphere (scenter, 1., white);

    /*
    Vector scenter1 (-5., 5., 1.);
    AddSphere (scenter1, 1., black);

    Vector scenter2 (-5., 5., 1.);
    AddSphere (scenter2, 1., green);

    Vector scenter3 (5., -5., 1.);
    AddSphere (scenter3, 1., red);
    */
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
    Plane *prev, *curr, *next;

    if (nplanes > 0) {
        if (nplanes < 2) {
            delete planes;
        }
        else {
            prev = planes;
            curr = prev->GetNext ();
            do {
                prev = curr;
                next = curr->GetNext ();
            } while (next != NULL);
            delete curr;
            prev->SetNext (NULL);
        }
        nplanes--;
    }
    /* . Returns zero if there are no planes left. */ 
    return nplanes;
}

unsigned int World::PopSphere () {
    Sphere *prev, *curr, *next;

    if (nspheres > 0) {
        if (nspheres < 2) {
            delete spheres;
        }
        else {
            prev = spheres;
            curr = prev->GetNext ();
            do {
                prev = curr;
                next = curr->GetNext ();
            } while (next != NULL);
            delete curr;
            prev->SetNext (NULL);
        }
        nspheres--;
    }
    /* . Returns zero if there are no spheres left. */ 
    return nspheres;
}

unsigned int World::AddPlane (Vector &center, Vector &normal,
        Color &colora, Color &colorb, double texscale) {
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
    printf ("%s: Added plane %d\n", __FILE__, (nplanes + 1));
#endif
    return (++nplanes);
}

unsigned int World::AddSphere (Vector &center, double radius,
        Color &color) {
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
    printf ("%s: Added sphere %d\n", __FILE__, (nspheres + 1));
#endif
    return (++nspheres);
}

void World::TraceRay (Vector &origin, Vector &direction,
        Color *color) {
    Plane   *plane, *hitplane;
    Sphere  *sphere, *hitsphere;
    double  dist, currd, dot;
    char    hit;
    Vector  inter, tl, normal;
    Color   objcol;

    /* . Initialize. */
    color->Set (0., 0., 0.);
    currd  = MAX_DISTANCE;
    hit    = HIT_NULL;

    /* . Search for planes. */
    plane  = planes;
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
    sphere = spheres;
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
        inter = (direction * currd) + origin;

        switch (hit) {
            case HIT_PLANE:
                hitplane->GetNormal (&normal);
                hitplane->DetermineColor (inter, &objcol);
                break;
            case HIT_SPHERE:
                hitsphere->GetNormal (inter, &normal);
                hitsphere->DetermineColor (&objcol);
                break;
            case HIT_CYLINDER:
                break;
        }

        /*
         * . Find a vector between the intersection
         *  and light.
         */
        light->GetToLight (inter, &tl);
        tl.Normalize_InPlace ();

        dot = normal * tl;
        if (dot >= 0.) {
            objcol.Scale_InPlace (dot);
        }
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

            TraceRay (origin, direction, bp++);
        }
    }
}

void World::WritePNG (char *filename) {
    Color blue (0., 0., 1.);

    buffer->Text ("BUFFER TEST.", 0, 0, blue);
    buffer->WriteToPNG (filename);
}
