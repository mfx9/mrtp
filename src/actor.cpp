/* File      : actor.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <Eigen/Geometry>
#include <cmath>
#include "actor.hpp"

using namespace Eigen;
using namespace std;

namespace mrtp {

/*
================
has_shadow

Returns true if an actor casts shadow
================
*/
bool Actor::has_shadow() { return has_shadow_; }

/*
================
get_reflect

Returns the reflection coefficient
================
*/
float Actor::get_reflect() { return reflect_; }

/*
================
solve_quadratic

solves a quadratic equation for t.

Since t is a scale in: P = O + t*D, returns
only the smaller t and within the limits of (mint, maxt).

Otherwise, returns -1.
================
*/
float Actor::solve_quadratic(float a, float b, float c, float mint,
                             float maxt) {
    float t = -1.0f;
    float delta = b * b - 4.0f * a * c;

    if (delta >= 0.0f) {
        if (delta > 0.0f) {
            float sqdelta = sqrt(delta);
            float tmp = 0.5f / a;
            float ta = (-b - sqdelta) * tmp;
            float tb = (-b + sqdelta) * tmp;
            t = (ta < tb) ? ta : tb;
        } else {
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
generate_unit_vector

Finds the smallest component of a vector and generates
an "associated" unit vector.

A cross product of the vector with its "associated"
vector should give a non-zero vector.
================
*/
Vector3f Actor::generate_unit_vector(Vector3f *vector) {
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
    } else { // if ( x >= y)
        if (y < z) {
            unit << 0.0f, 1.0f, 0.0f;
        }
    }
    return unit;
}

} //end namespace mrtp