/* File      : actors.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <cmath>
#include <Eigen/Geometry>

#include "actors.hpp"

using namespace Eigen;
using namespace std;


/*
================
SolveQuadratic 

Solves a quadratic equation for t.

Since t is a scale in: P = O + t*D, returns
only the smaller t and within the limits of (mint, maxt).

Otherwise, returns -1.
================
*/
static float SolveQuadratic (float a, float b, float c, float mint, float maxt) {
    float t = -1.0f;
    float delta = b * b - 4.0f * a * c;

    if (delta >= 0.0f) {
        if (delta > 0.0f) {
            float sqdelta = sqrt (delta);
            float tmp = 0.5f / a;
            float ta = (-b - sqdelta) * tmp;
            float tb = (-b + sqdelta) * tmp;
            t = (ta < tb) ? ta : tb;
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

/*
================
GenerateUnitVector

Finds the smallest component of a vector and generates 
an "associated" unit vector.

A cross product of the vector with its "associated" 
vector should give a non-zero vector.
================
*/
static Vector3f GenerateUnitVector (Vector3f *vector) {
    float tx = (*vector)[0];
    float ty = (*vector)[1];
    float tz = (*vector)[2];

    float x = (tx < 0.0f) ? -tx : tx;
    float y = (ty < 0.0f) ? -ty : ty;
    float z = (tz < 0.0f) ? -tz : tz;

    Vector3f unit;
    unit << 0.0f, 0.0f, 1.0f;

    if (x < y) {
        if (x < z) {
            unit << 1.0f, 0.0f, 0.0f;
        }
    }
    else {  // if ( x >= y)
        if (y < z) {
            unit << 0.0f, 1.0f, 0.0f;
        }
    }
    return unit;
}

/*
================
Actor
================
*/
Actor::Actor () {
}

/*
================
~Actor
================
*/
Actor::~Actor () {
}

/*
================
HasShadow

Returns true if an actor casts shadow
================
*/
bool Actor::HasShadow () {
    return hasShadow_;
}

/*
================
Reflective

Returns the reflection coefficient
================
*/
float Actor::Reflective () {
    return reflect_;
}

/*
================
Plane

Constructs a plane
================
*/
Plane::Plane (Vector3f *center, Vector3f *normal, float scale, float reflect, Texture *texture) {
    normal_ = *normal;
    normal_ *= (1.0f / normal_.norm ());

    reflect_ = reflect;
    texture_ = texture;
    scale_ = scale;

    center_ = *center;
    hasShadow_ = false;

    Vector3f tmp = GenerateUnitVector (&normal_);

    tx_ = tmp.cross (normal_);
    tx_ *= (1.0f / tx_.norm ());

    ty_ = normal_.cross (tx_);
    ty_ *= (1.0f / ty_.norm ());
}

/*
================
PickPixel

Picks a pixel from a plane's texture
================
*/
Pixel *Plane::PickPixel (Vector3f *hit, Vector3f *normal) {
    Vector3f v = (*hit) - center_;

    //Calculate components of v (dot products)
    float vx = v.dot (tx_);
    float vy = v.dot (ty_);

    return texture_->PickPixel (vx, vy, scale_);
}

/*
================
Solve

Solves the intersection of a ray and a plane
================
*/
float Plane::Solve (Vector3f *origin, Vector3f *direction, float mind, float maxd) {
    float bar = direction->dot (normal_);

    if (bar != 0.0f) {
        Vector3f tmp = (*origin) - center_;
        float d = -tmp.dot (normal_) / bar;

        if ((d >= mind) && (d <= maxd)) {
            return d;
        }
    }
    return -1.0f;
}

/*
================
CalculateNormal

Returns a normal to a plane
================
*/
Vector3f Plane::CalculateNormal (Vector3f *hit) {
    return normal_;
}

/*
================
Sphere

Constructs a sphere
================
*/
Sphere::Sphere (Vector3f *center, float radius, Vector3f *axis, float reflect, Texture *texture) {
    center_ = *center;

    R_ = radius;
    texture_ = texture;
    hasShadow_ = true;
    reflect_ = reflect;

    ty_ = *axis;
    ty_ *= (1.0f / ty_.norm ());

    Vector3f tmp = GenerateUnitVector (&ty_);

    tx_ = tmp.cross (ty_);
    tx_ *= (1.0f / tx_.norm ());
    
    tz_ = ty_.cross (tx_);
    tz_ *= (1.0f / tz_.norm ()); 
}

/*
================
Solve

Solves the intersection of a ray and a sphere
================
*/
float Sphere::Solve (Vector3f *origin, Vector3f *direction, float mind, float maxd) {
    Vector3f t = (*origin) - center_;

    float a = direction->dot (*direction);
    float b = 2.0f * direction->dot (t);
    float c = t.dot (t) - (R_ * R_);

    return SolveQuadratic (a, b, c, mind, maxd);
}

/*
================
CalculateNormal

Calculates normal to a sphere
================
*/
Vector3f Sphere::CalculateNormal (Vector3f *hit) {
    Vector3f normal = (*hit) - center_;

    return (normal * (1.0f / normal.norm ()));
}

/*
================
PickPixel

Picks a pixel from a spheres's texture

Guidelines:
https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
================
*/
Pixel *Sphere::PickPixel (Vector3f *hit, Vector3f *normal) {
    float dot   = normal->dot (ty_);
    float phi   = acos (-dot);
    float fracy = phi / M_PI;
        
    dot = normal->dot (tx_);
    float theta = acos (dot / sin (phi)) / (2.0f * M_PI);

    dot = normal->dot (tz_);
    float fracx = (dot > 0.0f) ? theta : (1.0f - theta);

    return texture_->PickPixel (fracx, fracy, 1.0f);
}

/*
================
Cylinder

Constructs a cylinder
================
*/
Cylinder::Cylinder (Vector3f *center, Vector3f *direction, float radius, 
                    float span, float reflect, Texture *texture) {
    A_ = *center;

    B_ = *direction;
    B_ *= (1.0f / B_.norm ());

    R_ = radius;
    span_ = span;
    texture_ = texture;
    reflect_ = reflect;
    hasShadow_ = true;

    ty_ = GenerateUnitVector (&B_);
    tx_ = ty_.cross (B_);
    tx_ *= (1.0f / tx_.norm ());
}

/*
================
Solve

Solves the intersection of a ray and a cylinder

Capital letters are vectors.
  A       Origin    of cylinder
  B       Direction of cylinder
  O       Origin    of ray
  D       Direction of ray
  P       Hit point on cylinder's surface
  X       Point on cylinder's axis closest to the hit point
  t       Distance between ray's      origin and P
  alpha   Distance between cylinder's origin and X

 (P - X) . B = 0
 |P - X| = R  => (P - X) . (P - X) = R^2

 P = O + t * D
 X = A + alpha * B
 T = O - A
 ...
 2t * (T.D - alpha * D.B)  +  t^2 - 2 * alpha * T.B  +
     +  alpha^2  =  R^2 - T.T
 a = T.D
 b = D.B
 d = T.B
 f = R^2 - T.T

 t^2 * (1 - b^2)  +  2t * (a - b * d)  -
     -  d^2 - f = 0    => t = ...
 alpha = d + t * b
================
*/
float Cylinder::Solve (Vector3f *O, Vector3f *D, float mind, float maxd) {
    Vector3f tmp = (*O) - A_;

    float a = D->dot (tmp);
    float b = D->dot (B_);
    float d = tmp.dot (B_);
    float f = (R_ * R_) - tmp.dot (tmp);

    //Solving a quadratic equation for t
    float aa = 1.0f - (b * b);
    float bb = 2.0f * (a - b * d);
    float cc = -(d * d) - f;
    float t  = SolveQuadratic (aa, bb, cc, mind, maxd);

    if (t > 0.0f) {
        //Check if the cylinder is finite
        if (span_ > 0.0f) {
            float alpha = d + t * b;

            if ((alpha < -span_) || (alpha > span_)) {
                return -1.0f;
            }
        }
    }

    return t;
}

/*
================
CalculateNormal

Calculates normal to a cylinder

N = Hit - [B . (Hit - A)] * B
================
*/
Vector3f Cylinder::CalculateNormal (Vector3f *hit) {
    Vector3f tmp = (*hit) - A_;
    float alpha = B_.dot (tmp);

    Vector3f bar = A_ + alpha * B_;
    Vector3f normal = (*hit) - bar;

    return (normal * (1.0f / normal.norm ()));
}

/*
================
PickPixel

Picks a pixel from a cylinders's texture
================
*/
Pixel *Cylinder::PickPixel (Vector3f *hit, Vector3f *normal) {
    Vector3f tmp = (*hit) - A_;

    float alpha = tmp.dot (B_);
    float dot   = normal->dot (tx_);
    float fracx = acos (dot) / M_PI;
    float fracy = alpha / (2.0f * M_PI * R_);

    return texture_->PickPixel (fracx, fracy, 1.0f);
}
