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

Plane::Plane (Vector *center, Vector *normal, Color *colora, 
        Color *colorb, double scale, Texture *texture) {
    next_ = NULL;
    center->CopyTo (&center_);

    normal->CopyTo (&normal_);
    normal_.Normalize_InPlace ();

    colora->CopyTo (&colora_);
    colorb->CopyTo (&colorb_);
    scale_ = scale;

    /*
     * Prepare texturing.
     *
     */
    Vector T;

    normal_.CopyTo (&T);
    T.Associated_InPlace ();

    texturex_ = normal_ ^ T;
    texturex_.Normalize_InPlace ();

    texturey_ = normal_ ^ texturex_;
    texturey_.Normalize_InPlace ();

    /*
     * Assign a pointer to the texture (can be NULL).
     */
    texture_ = texture;
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

    if (texture_ != NULL) {
        /*
         * Use actual textures in texture mapping.
         */
        cp = texture_->GetColor (vx, vy, scale_);
    }
    else {
        /*
         * Generate a checkerboard pattern 
         * from two colors.
         */
        double  vx, vy, scale;
        int     tx, ty;

        scale = 1.0 / scale_;
        tx    = (int) (vx * scale);
        ty    = (int) (vy * scale);
        if (vx > 0.0) {
            tx--;
        }
        if (vy > 0.0) {
            ty--;
        }
        
        if (!(tx & 1)) {
            cp = (!(ty & 1)) ? &colora_ : &colorb_;
        }
        else {
            cp = (!(ty & 1)) ? &colorb_ : &colora_;
        }
    }

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

        if ((d < mind) || (d > maxd))
            d = -1.0;
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

Sphere::Sphere (Vector *center, double radius,
        Color *color) {
    center->CopyTo (&center_);
    color->CopyTo (&color_);
    next_   = NULL;
    radius_ = radius;
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

void Sphere::DetermineColor (Vector *hit, 
        Color *color) {
    color_.CopyTo (color);
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

Cylinder::Cylinder (Vector *a, Vector *b, double radius, 
        Color *color) {
    /*
     * Direction of the cylinder.
     */
    B_ = (*b) - (*a);
    B_.Normalize_InPlace ();

    /*
     * Radius and origin.
     */
    a->CopyTo (&A_);
    radius_ = radius;

    /*
     * Color, etc.
     */
    color->CopyTo (&color_);
    next_ = NULL;
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

    /* Solving a quadratic equation for t. */
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

void Cylinder::DetermineColor (Vector *hit, 
        Color *color) {
    color_.CopyTo (color);
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
