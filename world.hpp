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
#ifndef _WORLD_H
#define _WORLD_H

#include <cmath>
#include <cstdio>
#include "vector.hpp"
#include "actors.hpp"
#include "buffer.hpp"
#include "color.hpp"


#define DEBUG_WORLD    1

#define HIT_NULL       0
#define HIT_PLANE      1
#define HIT_SPHERE     2
#define HIT_CYLINDER   3
#define MAX_DISTANCE  60.0


class Camera {
    Vector  eye, lookat;
    double  width, heigth;
    double  ratio;   /*  width/height  */
    double  perspective;
    double  rotation;
public:
    Camera (Vector &origin, Vector &target,
        unsigned int w, unsigned int h, double fov, 
        double rot);
    ~Camera ();
    void CalculateVectors (Vector *vw, Vector *vh,
        Vector *vo);
    void GetDimensions (unsigned int *w,
        unsigned int *h);
    void GetEye (Vector *vector);
};

class World {
    Camera    *camera;
    Light     *light;
    /* One-way linked lists, not continuous 
        tables. */
    Plane     *planes;
    Sphere    *spheres;
    Cylinder  *cylinders;
    unsigned int nplanes, nspheres, 
        ncylinders;
    Buffer    *buffer;
public:
    World (char *filename);
    ~World ();
    unsigned int AddPlane (Vector &center, Vector &normal,
        Color &colora, Color &colorb, double texscale);
    unsigned int AddSphere (Vector &center, double radius,
        Color &color);
    unsigned int PopPlane ();
    unsigned int PopSphere ();
    void TraceRay (Vector &origin, Vector &direction,
        Color *color);
    void Render ();
    void WritePNG (char *filename);
};

#endif /* _WORLD_H */
