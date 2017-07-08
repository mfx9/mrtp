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
#include "actors.hpp"


/*
 * Planes.
 */
Plane::Plane () {
}

Plane::~Plane () {
}

Plane::Plane (Vector *c, Vector *n, Color *cola, 
        Color *colb, double texscale) {
    next = NULL;
    c->CopyTo (&center);
    n->CopyTo (&normal);
    normal.Normalize_InPlace ();
    /*
     * Texturing.
     */
    cola->CopyTo (&colora);
    colb->CopyTo (&colorb);
    tscale = texscale;

    Vector k;
    normal.CopyTo (&k);

    Vector i (1., 0., 0.), j;
    j = k ^ i;
    j.Normalize_InPlace ();
    j.CopyTo (&texy);

    i = j ^ k;
    i.Normalize_InPlace ();
    i.CopyTo (&texx);
}

Plane *Plane::GetNext () {
    return next;
}

void Plane::SetNext (Plane *plane) {
    next = plane;
}

void Plane::DetermineColor (Vector *inter, 
        Color *col) {
    Vector v;
    double vx, vy, scale;
    int tx, ty;
    Color *cp;

    v = *(inter) - center;
    /*
     * . Calculate components of v (dot
     *  products).
     */
    vx    = v * texx;
    vy    = v * texy;
    scale = 1. / tscale;
    tx    = (int) (vx * scale);
    ty    = (int) (vy * scale);
    if (vx > 0.)
        tx--;
    if (vy > 0.)
        ty--;
    if (!(tx & 1)) {
        if (!(ty & 1))
            cp = &colora;
        else
            cp = &colorb;
    } else {
        if (!(ty & 1))
            cp = &colorb;
        else
            cp = &colora;
    }
    cp->CopyTo (col);
}

double Plane::Solve (Vector *origin, Vector *direction, 
        double mind, double maxd) {
    double bar, d = -1.;

    bar = (*direction) * normal;
    if IS_NOT_ZERO (bar) {
        d = -((*origin - center) * normal) / bar;
        if ((d < mind) || (d > maxd))
            d = -1.;
    }
    return d;
}

void Plane::GetNormal (Vector *n) {
    normal.CopyTo (n);
}

/*
 * Spheres.
 */
Sphere::Sphere () {
}

Sphere::~Sphere () {
}

Sphere::Sphere (Vector *c, double radius,
        Color *col) {
    c->CopyTo (&center);
    next = NULL;
    R = radius;
    col->CopyTo (&color);
}

Sphere *Sphere::GetNext () {
    return next;
}

void Sphere::SetNext (Sphere *sphere) {
    next = sphere;
}

void Sphere::DetermineColor (Color *col) {
    color.CopyTo (col);
}

double Sphere::Solve (Vector *origin, Vector *direction, 
        double mind, double maxd) {
    Vector oc;
    oc = (*origin) - center;

    double a, b, c;
    a  = direction->DotSelf ();
    b  = 2. * (*direction * oc);
    c  = oc.DotSelf () - (R * R);

    double delta, d;
    delta = b * b - 4. * a * c;
    if (delta < 0.) {
        d = -1;
    }
    else {
        if IS_ZERO (delta) {
            d = -b / (2. * a);
        }
        else {
            double sd, t, da, db;
            sd = sqrt (delta);
            t  = .5 / a;
            da = (-b - sd) * t;
            db = (-b + sd) * t;
            d  = (da < db) ? da : db;
        }
        if ((d < mind) || (d > maxd))
            d = -1;
    }
    return d;
}

void Sphere::GetNormal (Vector *hit, Vector *n) {
    Vector subtract = (*hit) - center;
    subtract.Normalize_InPlace ();
    subtract.CopyTo (n);
}

/*
 * Cylinders.
 */
Cylinder::Cylinder () {
}

Cylinder::~Cylinder () {
}

/*
 * Light.
 */
Light::Light (Vector *origin) {
    origin->CopyTo (&position);
}

Light::~Light () {
}

void Light::GetToLight (Vector *hit, Vector *tolight) {
    Vector subtract = position - (*hit);
    subtract.CopyTo (tolight);
}
