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
    double bar, d = -1.0f;

    bar = (*direction) * normal;
    if IS_NOT_ZERO (bar) {
        d = -((*origin - center) * normal) / bar;

        if ((d < mind) || (d > maxd))
            d = -1.0f;
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
    Vector T;
    T = (*origin) - center;

    double a, b, c;
    a  = direction->DotSelf ();
    b  = 2.0f * (*direction * T);
    c  = T.DotSelf () - (R * R);

    double delta, sqdelta, ta, tb, d;
    SOLVE_QUADRATIC (a, b, c, delta, sqdelta, 
        ta, tb, d, mind, maxd);
    return d;
}

void Sphere::GetNormal (Vector *hit, Vector *n) {
    Vector T;

    T = (*hit) - center;
    T.Normalize_InPlace ();
    T.CopyTo (n);
}

/*
 * Cylinders.
 */
Cylinder::Cylinder () {
}

Cylinder::~Cylinder () {
}

Cylinder::Cylinder (Vector *a, Vector *b, double radius, 
        Color *col) {
    /*
     * Cylinder's direction.
     */
    Vector T;
    T = (*b) - (*a);
    T.Normalize_InPlace ();
    T.CopyTo (&B);
    /*
     * Radius and origin.
     */
    a->CopyTo (&A);
    R = radius;
    /*
     * Color, etc.
     */
    col->CopyTo (&color);
    next = NULL;
}

double Cylinder::Solve (Vector *O, Vector *D,
        double mind, double maxd) {
    /*
     * Capital letters are vectors.
     *   A     :  Origin    of cylinder
     *   B     :  Direction of cylinder
     *   O     :  Origin    of ray
     *   D     :  Direction of ray
     *   P     :  Hit point on cylinder's surface
     *   X     :  Point on cylinder's axis closest to the hit point
     *   t     :  Distance between ray's      origin and P
     *   alpha :  Distance between cylinder's origin and X
     *
     *  (P - X) . B = 0
     *  |P - X| = R  => (P - X) . (P - X) = R^2
     *
     *  P = O + t * D
     *  X = A + alpha * B
     *  T = O - A
     *  ...
     *  2t * (T.D - alpha * D.B)  +  t^2 - 2 * alpha * T.B  +
     *      +  alpha^2  =  R^2 - T.T
     *  a = T.D
     *  b = D.B
     *  d = T.B
     *  f = R^2 - T.T
     *
     *  t^2 * (1 - b^2)  +  2t * (a - b * d)  -
     *      -  d^2 - f = 0    => t = ...
     *  alpha = d + t * b
     *
     */
    Vector T;
    T = (*O) - A;

    double a, b, d, f;
    a  = T * (*D);
    b  = B * (*D);
    d  = T * B;
    f  = (R * R) - (T * T);

    /* Solving a quadratic equation for t. */
    double aa, bb, cc;
    aa = 1.0f - (b * b);
    bb = 2.0f * (a - b * d);
    cc = -(d * d) - f;

    double delta, sqdelta, ta, tb, t;
    SOLVE_QUADRATIC (aa, bb, cc, delta, sqdelta, 
        ta, tb, t, mind, maxd);
    if (t > 0.0f) {
        alpha = d + t * b;
    }
    return t;
}

void Cylinder::GetNormal (Vector *hit, Vector *n) {
    Vector T, Q, N;

    B.CopyTo (&T);
    T.Scale_InPlace (alpha);
    Q = A + T;
    N = (*hit) - Q;
    N.Normalize_InPlace ();
    N.CopyTo (n);
}

void Cylinder::DetermineColor (Color *col) {
    color.CopyTo (col);
}

Cylinder *Cylinder::GetNext () {
    return next;
}

void Cylinder::SetNext (Cylinder *cylinder) {
    next = cylinder;
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
