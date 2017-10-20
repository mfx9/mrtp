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
#ifndef _ACTORS_H
#define _ACTORS_H

#include <cstddef>  /* NULL pointer. */
#include <cmath>

#include "vector.hpp"
#include "color.hpp"
#include "texture.hpp"

/*
 * Macro to solve a quadratic equation for t.
 *
 * Since t is a scale in: P = O + t*D, return
 * only the smaller t and within the limits of (mint, maxt).
 *
 * Otherwise return -1.
 */
#define SOLVE_QUADRATIC(a, b, c, t, mint, maxt) \
    double delta; \
    delta = b * b - 4.0f * a * c; \
    if (delta < 0.0f) { \
        t = -1.0f; \
    } \
    else { \
        if (delta != 0.0f) { \
            double sqdelta, ta, tb; \
            sqdelta = sqrt (delta); \
            t  = 0.5f / a; \
            ta = (-b - sqdelta) * t; \
            tb = (-b + sqdelta) * t; \
            t  = (ta < tb) ? ta : tb; \
        } \
        else { \
            t = -b / (2.0f * a); \
        } \
        if ((t < mint) || (t > maxt)) { \
            t = -1.0f; \
        } \
    }


class Sphere {
    Vector   center_, texturex_, 
        texturey_, texturez_;
    double   radius_;
    Color    color_;
    Texture *texture_;
    Sphere  *next_;

public:
    ~Sphere ();
    Sphere ();
    Sphere (Vector *center, double radius, Vector *axis,
        Color *color, Texture *texture);

    double  Solve (Vector *origin, Vector *direction, double mind, double maxd);
    void    GetNormal (Vector *hit, Vector *normal);
    void    DetermineColor (Vector *normal, Color *color);
    Sphere *Next ();
    void    SetNext (Sphere *sphere);
};

class Plane {
    Vector   center_, normal_,
        texturex_, texturey_;
    double   scale_;
    Color    color_;
    Texture *texture_;
    Plane   *next_;

public:
    ~Plane ();
    Plane ();
    Plane (Vector *center, Vector *normal, double scale, 
        Color *color, Texture *texture);

    double  Solve (Vector *origin, Vector *direction, double mind, double maxd);
    void    GetNormal (Vector *normal);
    void    DetermineColor (Vector *hit, Color *color);
    Plane  *Next ();
    void    SetNext (Plane *plane);
};

class Cylinder {
    Vector    A_, B_,
        texturex_, texturey_;
    double    radius_, span_;
    Color     color_;
    Texture  *texture_;
    Cylinder *next_;

public:
    ~Cylinder ();
    Cylinder ();
    Cylinder (Vector *center, Vector *direction, 
        double radius, double span, Color *color, Texture *texture);

    double    Solve (Vector *O, Vector *D, double mind, double maxd);
    void      GetNormal (Vector *hit, Vector *normal);
    void      DetermineColor (Vector *hit, Vector *normal, Color *color);
    Cylinder *Next ();
    void      SetNext (Cylinder *cylinder);
};

class Light {
    Vector  position_;

public:
    Light (Vector *origin);
    ~Light ();
    void GetToLight (Vector *hit, Vector *tolight);
};


#endif /* _ACTORS_H */
