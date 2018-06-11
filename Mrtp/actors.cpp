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
Base class
================
*/
Actor::Actor () {
}

Actor::~Actor () {
}

bool Actor::HasShadow () {
    return hasShadow_;
}

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
Plane

Copy-constructor
================
*/
Plane::Plane (const Plane &old) {
    center_ = old.center_;
    normal_ = old.normal_;
    tx_ = old.tx_;
    ty_ = old.ty_;
    scale_ = old.scale_;
    reflect_ = old.reflect_;
    hasShadow_ = old.hasShadow_;
    texture_ = old.texture_;
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
