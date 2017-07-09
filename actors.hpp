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
#ifndef _ACTORS_H
#define _ACTORS_H

#include <cstdio>
#include <cmath>

#include "vector.hpp"
#include "color.hpp"
#include "texture.hpp"


// #define DEBUG_ACTORS   1

#define TOLERANCE  0.000001f
#define IS_ZERO(x) ((x > -TOLERANCE) && (x < TOLERANCE))
#define IS_NOT_ZERO(x) ((x <= -TOLERANCE) || (x >= TOLERANCE))

#define SOLVE_QUADRATIC(a, b, c, delta, sqdelta, ta, tb, t, mint, maxt) \
    delta = b * b - 4.0f * a * c; \
    if (delta < 0.0f) { \
        t = -1.0f; \
    } \
    else { \
        if IS_ZERO (delta) { \
            t = -b / (2.0f * a); \
        } \
        else { \
            sqdelta = sqrt (delta); \
            t  = 0.5f / a; \
            ta = (-b - sqdelta) * t; \
            tb = (-b + sqdelta) * t; \
            t  = (ta < tb) ? ta : tb; \
        } \
        if ((t < mint) || (t > maxt)) \
            t = -1.0f; \
    }

class Sphere {
    Vector  center;
    double  R;
    Color   color;
    Sphere *next;
public:
    ~Sphere ();
    Sphere ();
    Sphere (Vector *c, double radius,
        Color *col);

    double Solve (Vector *origin, Vector *direction, 
        double mind, double maxd);
    void GetNormal (Vector *hit, Vector *n);
    void DetermineColor (Color *col);

    Sphere *GetNext ();
    void SetNext (Sphere *sphere);
};

class Plane {
    Vector  center, normal;
    Vector  texx, texy;
    Color   colora, colorb;
    double  tscale;
    Texture *texture;
    bool    istexture;
    Plane   *next;
public:
    ~Plane ();
    Plane ();
    Plane (Vector *c, Vector *n, Color *cola, 
        Color *colb, double texscale);

    double Solve (Vector *origin, Vector *direction, 
        double mind, double maxd);
    void GetNormal (Vector *n);
    void DetermineColor (Vector *inter, Color *col);

    Plane *GetNext ();
    void SetNext (Plane *plane);
};

class Cylinder {
    Vector    A, B;
    double    R, alpha;
    Color     color;
    Cylinder *next;
public:
    ~Cylinder ();
    Cylinder ();
    /*
     * Points a and b define cylinder's axis.
     */
    Cylinder (Vector *a, Vector *b, double radius, 
        Color *col);

    double Solve (Vector *O, Vector *D,
        double mind, double maxd);
    void GetNormal (Vector *hit, Vector *n);
    void DetermineColor (Color *col);

    Cylinder *GetNext ();
    void SetNext (Cylinder *cylinder);
};

class Light {
    Vector  position;
public:
    Light (Vector *origin);
    ~Light ();
    void GetToLight (Vector *hit, 
        Vector *tolight);
};


#endif /* _ACTORS_H */
