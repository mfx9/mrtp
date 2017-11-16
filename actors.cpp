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
#include "actors.hpp"

/**** Local functions. ****/
static double SolveQuadratic (double a, double b, double c, double mint, 
                              double maxt);

/**** Base class. ****/
Actor::Actor () {
    next_ = NULL;
    texture_ = NULL;
    color_.Zero ();
}

Actor::~Actor () {
}

Actor *Actor::Next () {
    return next_;
}

void Actor::SetNext (Actor *next) {
    next_ = next;
}

bool Actor::Reflective (double *reflect) {
    if (reflective_) {
        *reflect = reflect_;
    }
    return reflective_;
}


/**** Planes. ****/

Plane::Plane (Vector *center, Vector *normal, double scale, 
              double reflect, Color *color, Texture *texture) {
    center->CopyTo (&center_);

    texture_ = texture;
    scale_ = scale;
    next_ = NULL;
    reflect_ = reflect;
    reflective_ = (reflect_ > 0.0f) ? true : false;

    normal->CopyTo (&normal_);
    normal_.Normalize_InPlace ();

    if (texture_ == NULL) {
        color->CopyTo (&color_);
    }
    else {
        Vector T;
        normal_.GenerateUnitVector (&T);
        
        tx_ = T ^ normal_;
        tx_.Normalize_InPlace ();
        
        ty_ = normal_ ^ tx_;
        ty_.Normalize_InPlace ();
    }
}

void Plane::DetermineColor (Vector *hit, Vector *normal, 
                            Color *color) {
    Vector  V;
    double  vx, vy;
    Color  *cp;

    if (texture_ == NULL) {
        cp = &color_;
    }
    else {
        V  = (*hit) - center_;
        /* Calculate components of V (dot products). */
        vx = V * tx_;
        vy = V * ty_;
        cp = texture_->GetColor (vx, vy, scale_);
    }
    cp->CopyTo (color);
}

double Plane::Solve (Vector *origin, Vector *direction, 
                     double mind, double maxd) {
    double bar, d = -1.0f;
    Vector T;

    bar = (*direction) * normal_;
    if (bar != 0.0f) {
        T = (*origin) - center_;
        d = -(T * normal_) / bar;
        if ((d < mind) || (d > maxd)) {
            d = -1.0f;
        }
    }
    return d;
}

void Plane::GetNormal (Vector *hit, Vector *normal) {
    normal_.CopyTo (normal);
}


/**** Spheres. ****/

Sphere::Sphere (Vector *center, double radius, Vector *axis, 
                double reflect, Color *color, Texture *texture) {
    center->CopyTo (&center_);

    R_ = radius;
    texture_ = texture;
    next_ = NULL;
    reflect_ = reflect;
    reflective_ = (reflect_ > 0.0f) ? true : false;

    if (texture_ == NULL) {
        color->CopyTo (&color_);
    }
    else {
        axis->CopyTo (&ty_);
        ty_.Normalize_InPlace ();
    
        Vector T;
        ty_.GenerateUnitVector (&T);
        tx_ = T ^ ty_;
        tx_.Normalize_InPlace ();
    
        tz_ = ty_ ^ tx_;
        tz_.Normalize_InPlace ();
    }
}

double Sphere::Solve (Vector *origin, Vector *direction, 
                      double mind, double maxd) {
    Vector T;
    T = (*origin) - center_;

    double a, b, c;
    a  = (*direction) * (*direction);
    b  = 2.0f * (*direction * T);
    c  = (T * T) - (R_ * R_);

    return SolveQuadratic (a, b, c, mind, maxd);
}

void Sphere::GetNormal (Vector *hit, Vector *normal) {
    (*normal) = (*hit) - center_;
    normal->Normalize_InPlace ();
}

void Sphere::DetermineColor (Vector *hit, Vector *normal, 
                             Color *color) {
    Color  *cp;
    double  phi, theta, dot, fracx, fracy;

    if (texture_ == NULL) {
        cp = &color_;
    }
    else {
        /*
         * Guidelines from:
         * https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
         *
         */
        dot   = ty_ * (*normal);
        phi   = acos (-dot);
        fracy = phi / M_PI;
        
        dot   = (*normal) * tx_;
        theta = acos (dot / sin (phi)) / (2.0f * M_PI);
        dot   = tz_ * (*normal);
        if (dot > 0.0f) {
            fracx = theta;
        }
        else {
            fracx = 1.0f - theta;
        }
        cp = texture_->GetColor (fracx, fracy, 1.0f);
    }
    cp->CopyTo (color);
}


/**** Cylinders. ****/

Cylinder::Cylinder (Vector *center, Vector *direction, 
                    double radius, double span, double reflect, 
                    Color *color, Texture *texture) {
    center->CopyTo (&A_);

    R_ = radius;
    span_ = span;
    texture_ = texture;
    next_ = NULL;
    reflect_ = reflect;
    reflective_ = (reflect_ > 0.0f) ? true : false;

    direction->CopyTo (&B_);
    B_.Normalize_InPlace ();

    if (texture_ == NULL) {
        color->CopyTo (&color_);
    }
    else {
        B_.GenerateUnitVector (&ty_);
        tx_ = ty_ ^ B_;
        tx_.Normalize_InPlace ();
    }
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
    f  = (R_ * R_) - (T * T);

    /* 
     * Solving a quadratic equation for t. 
     */
    double aa, bb, cc, t, alpha;
    aa = 1.0f - (b * b);
    bb = 2.0f * (a - b * d);
    cc = -(d * d) - f;

    t = SolveQuadratic (aa, bb, cc, mind, maxd);
    if (t > 0.0f) {
        /*
         * Check if the cylinder is finite.
         */
        if (span_ > 0.0f) {
            alpha = d + t * b;
            if ((alpha < -span_) || (alpha > span_)) {
                return -1.0f;
            }
        }
    }
    return t;
}

void Cylinder::GetNormal (Vector *hit, Vector *normal) {
    /*
     * N = Hit - [B . (Hit - A)] * B
     */
    Vector T, Q;
    double alpha;

    T = (*hit) - A_;
    alpha = B_ * T;

    T = B_ * alpha;
    Q = A_ + T;
    (*normal) = (*hit) - Q;
    normal->Normalize_InPlace ();
}

void Cylinder::DetermineColor (Vector *hit, Vector *normal, 
                               Color *color) {
    Color  *cp;

    if (texture_ == NULL) {
        cp = &color_;
    }
    else {
        double  dot, fracx, fracy, alpha;
        Vector  T;

        T = (*hit) - A_;
        alpha = B_ * T;
        dot   = tx_ * (*normal);
        fracx = acos (dot) / M_PI;
        fracy = alpha / (2.0f * M_PI * R_);
        cp    = texture_->GetColor (fracx, fracy, 1.0f);
    }
    cp->CopyTo (color);
}


/**** Light. ****/

Light::Light (Vector *origin) {
    origin->CopyTo (&position_);
}

Light::~Light () {
}

void Light::LightRay (Vector *hit, Vector *ray) {
    Vector T = position_ - (*hit);
    T.CopyTo (ray);
}


/**** Utility functions. ****/

double SolveQuadratic (double a, double b, double c, double mint, 
                       double maxt) {
    /*
     * Solve a quadratic equation for t.
     *
     * Since t is a scale in: P = O + t*D, return
     * only the smaller t and within the limits of (mint, maxt).
     *
     * Otherwise return -1.
     */
    double delta, sqdelta, ta, tb, t;

    delta = b * b - 4.0f * a * c;
    if (delta < 0.0f) {
        t = -1.0f;
    }
    else {
        if (delta != 0.0f) {
            sqdelta = sqrt (delta);
            t  = 0.5f / a;
            ta = (-b - sqdelta) * t;
            tb = (-b + sqdelta) * t;
            t  = (ta < tb) ? ta : tb;
        }
        else {
            t = -b / (2.0f * a);
        }
        if ((t < mint) || (t > maxt)) {
            t = -1.0f;
        }
    }
    return t;
}
