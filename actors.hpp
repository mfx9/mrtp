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

/* NULL pointer. */
#include <cstddef>
#include <cmath>

#include "vector.hpp"
#include "color.hpp"
#include "texture.hpp"


/**** Light. ****/

class Light {
    Vector  position_;
    Color   color_;

public:
    Light (Vector *origin);
    ~Light ();
    void LightRay (Vector *hit, Vector *ray);
};


/**** Actors. ****/

class Actor {
protected:
    Color     color_;
    Texture  *texture_;
    Actor    *next_;
    bool      reflective_;
    double    reflect_;

public:
    Actor ();
    Actor *Next ();
    void SetNext (Actor *next);
    bool Reflective (double *reflect);

    virtual ~Actor ();
    virtual double Solve (Vector *origin, Vector *direction, double mind, 
                          double maxd) = 0;
    virtual void GetNormal (Vector *hit, Vector *normal) = 0;
    virtual void DetermineColor (Vector *hit, Vector *normal, 
                                 Color *color) = 0;
};

class Sphere : public Actor {
    Vector   center_;
    Vector   tx_;
    Vector   ty_;
    Vector   tz_;
    double   R_;

public:
    Sphere (Vector *center, double radius, Vector *axis,
            double reflect, Color *color, Texture *texture);

    double Solve (Vector *origin, Vector *direction, double mind, 
                  double maxd);
    void GetNormal (Vector *hit, Vector *normal);
    void DetermineColor (Vector *hit, Vector *normal, Color *color);
};

class Plane : public Actor {
    Vector   center_;
    Vector   normal_;
    Vector   tx_;
    Vector   ty_;
    double   scale_;

public:
    Plane (Vector *center, Vector *normal, double scale, 
           double reflect, Color *color, Texture *texture);

    double Solve (Vector *origin, Vector *direction, double mind, 
                  double maxd);
    void GetNormal (Vector *hit, Vector *normal);
    void DetermineColor (Vector *hit, Vector *normal, Color *color);
};

class Cylinder : public Actor {
    Vector  A_;
    Vector  B_;
    Vector  tx_;
    Vector  ty_;
    double  R_;
    double  span_;

public:
    Cylinder (Vector *center, Vector *direction, 
              double radius, double span, double reflect, Color *color, 
              Texture *texture);

    double Solve (Vector *origin, Vector *direction, double mind, 
                  double maxd);
    void GetNormal (Vector *hit, Vector *normal);
    void DetermineColor (Vector *hit, Vector *normal, Color *color);
};

#endif /* _ACTORS_H */
