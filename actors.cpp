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


/*****************************
 *          Planes           *
 *****************************/
Plane::Plane () {
    next_ = NULL;
}

Plane::~Plane () {
}

Plane::Plane (Vector *center, Vector *normal, double scale, 
        Texture *texture) {
    center->CopyTo (&center_);
    texture_ = texture;
    scale_   = scale;
    next_    = NULL;

    normal->CopyTo (&normal_);
    normal_.Normalize_InPlace ();
    /*
     * Prepare texturing.
     *
     */
    Vector T;
    normal_.GenerateUnitVector (&T);

    texturex_ = T ^ normal_;
    texturex_.Normalize_InPlace ();

    texturey_ = normal_ ^ texturex_;
    texturey_.Normalize_InPlace ();
}

void Plane::DetermineColor (Vector *hit, Color *color) {
    Vector  V;
    double  vx, vy;
    Color  *cp;

    V = (*hit) - center_;
    /*
     * Calculate components of V (dot products).
     * 
     */
    vx = V * texturex_;
    vy = V * texturey_;
    cp = texture_->GetColor (vx, vy, scale_);

    /*
     * Finalize.
     */
    cp->CopyTo (color);
}

double Plane::Solve (Vector *origin, Vector *direction, 
        double mind, double maxd) {
    double bar, d = -1.0;

    bar = (*direction) * normal_;
    if IS_NOT_ZERO (bar) {
        d = -((*origin - center_) * normal_) / bar;
        if ((d < mind) || (d > maxd)) {
            d = -1.0;
        }
    }
    return d;
}

void Plane::GetNormal (Vector *normal) {
    normal_.CopyTo (normal);
}

Plane *Plane::GetNext () {
    return next_;
}

void Plane::SetNext (Plane *plane) {
    next_ = plane;
}


/*****************************
 *          Spheres          *
 *****************************/
Sphere::Sphere () {
    next_ = NULL;
}

Sphere::~Sphere () {
}

Sphere::Sphere (Vector *center, double radius, Vector *axis, 
        Texture *texture) {
    center->CopyTo (&center_);
    radius_  = radius;
    texture_ = texture;
    next_    = NULL;

    /*
     * Prepare texturing.
     *
     */
    axis->CopyTo (&texturey_);
    texturey_.Normalize_InPlace ();

    Vector T;
    texturey_.GenerateUnitVector (&T);
    texturex_ = T ^ texturey_;
    texturex_.Normalize_InPlace ();
}

double Sphere::Solve (Vector *origin, Vector *direction, 
        double mind, double maxd) {
    Vector T;
    T = (*origin) - center_;

    double a, b, c, d;
    a  = (*direction) * (*direction);
    b  = 2.0 * (*direction * T);
    c  = (T * T) - (radius_ * radius_);

    SOLVE_QUADRATIC (a, b, c, d, mind, maxd);
    return d;
}

void Sphere::GetNormal (Vector *hit, Vector *normal) {
    Vector T;

    T = (*hit) - center_;
    T.Normalize_InPlace ();
    T.CopyTo (normal);
}

void Sphere::DetermineColor (Vector *normal, Color *color) {
    Color  *cp;
    double  dot, fracx, fracy;
    /*
     * Texture mapping.
     */
    dot   = texturex_ * (*normal);
    fracx = acos (dot) / M_PI;
    dot   = texturey_ * (*normal);
    fracy = acos (dot) / M_PI;

    cp = texture_->GetColor (fracx, fracy, 1.0);
    cp->CopyTo (color);
}

Sphere *Sphere::GetNext () {
    return next_;
}

void Sphere::SetNext (Sphere *sphere) {
    next_ = sphere;
}


/*****************************
 *         Cylinders         *
 *****************************/
Cylinder::Cylinder () {
    next_ = NULL;
}

Cylinder::~Cylinder () {
}

Cylinder::Cylinder (Vector *origin, Vector *direction, 
        double radius, Texture *texture) {
    /*
     * Radius, origin, etc.
     */
    origin->CopyTo (&A_);
    radius_  = radius;
    texture_ = texture;
    next_    = NULL;

    /*
     * Direction of the cylinder.
     */
    direction->CopyTo (&B_);
    B_.Normalize_InPlace ();

    /*
     * Prepare texturing.
     *
     */
    Vector T;
    B_.GenerateUnitVector (&T);

    texturex_ = T ^ B_;
    texturex_.Normalize_InPlace ();

    B_.CopyTo (&texturey_);
}

double Cylinder::Solve (Vector *O, Vector *D,
        double mind, double maxd) {
    /*
     * Capital letters are vectors.
     *   A       Origin    of cylinder
     *   B       Direction of cylinder
     *   O       Origin    of ray
     *   D       Direction of ray
     *   P       Hit point on cylinder's surface
     *   X       Point on cylinder's axis closest to the hit point
     *   t       Distance between ray's      origin and P
     *   alpha   Distance between cylinder's origin and X
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
    T = (*O) - A_;

    double a, b, d, f;
    a  = T  * (*D);
    b  = B_ * (*D);
    d  = T  * B_;
    f  = (radius_ * radius_) - (T * T);

    /* 
     * Solving a quadratic equation for t. 
     */
    double aa, bb, cc, t;
    aa = 1.0 - (b * b);
    bb = 2.0 * (a - b * d);
    cc = -(d * d) - f;

    SOLVE_QUADRATIC (aa, bb, cc, t, mind, maxd);
    if (t > 0.0) {
        alpha_ = d + t * b;
    }
    return t;
}

void Cylinder::GetNormal (Vector *hit, Vector *normal) {
    Vector T, Q, N;

    B_.CopyTo (&T);
    T.Scale_InPlace (alpha_);
    Q = A_ + T;
    N = (*hit) - Q;
    N.Normalize_InPlace ();
    N.CopyTo (normal);
}

void Cylinder::DetermineColor (Vector *normal,
        Color *color) {
    Color  *cp;
    double  dot, fracx, fracy;
    /* 
     * Texture mapping.
     */
    dot   = texturex_ * (*normal);
    fracx = acos (dot) / M_PI;
    fracy = alpha_ / (2.0 * M_PI * radius_);

    cp = texture_->GetColor (fracx, fracy, 1.0);
    cp->CopyTo (color);
}

Cylinder *Cylinder::GetNext () {
    return next_;
}

void Cylinder::SetNext (Cylinder *cylinder) {
    next_ = cylinder;
}


/*****************************
 *           Light           *
 *****************************/
Light::Light (Vector *origin) {
    origin->CopyTo (&position_);
}

Light::~Light () {
}

void Light::GetToLight (Vector *hit, Vector *tolight) {
    Vector T = position_ - (*hit);
    T.CopyTo (tolight);
}
