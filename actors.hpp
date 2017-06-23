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


// #define DEBUG_ACTORS   1

#define TOLERANCE  .000001
#define IS_ZERO(x) ((x > -TOLERANCE) && (x < TOLERANCE))
#define IS_NOT_ZERO(x) ((x <= -TOLERANCE) || (x >= TOLERANCE))


class Actor {
public:
    Actor ();
    ~Actor ();
};

class Sphere : public Actor {
    Vector center;
    double R;
    Color color;
    Sphere *next;
public:
    Sphere (Vector *c, double radius,
        Color *col);
    double Solve (Vector *origin, Vector *direction, 
        double mind, double maxd);
    Sphere *GetNext ();
    void GetNormal (Vector *hit, Vector *n);
    void SetNext (Sphere *sphere);
    void DetermineColor (Color *col);
};

class Plane : public Actor {
    Vector center, normal;
    Vector texx, texy;
    Color colora, colorb;
    double tscale;
    Plane *next;
public:
    Plane (Vector *c, Vector *n, Color *cola, 
        Color *colb, double texscale);
    double Solve (Vector *origin, Vector *direction, 
        double mind, double maxd);
    Plane *GetNext ();
    void GetNormal (Vector *n);
    void SetNext (Plane *plane);
    void DetermineColor (Vector *inter, Color *col);
};

class Cylinder : public Actor {
    double m, n, R;
    Color color;
    char type;
public:
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
