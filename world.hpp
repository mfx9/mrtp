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
#include <string>
#include <iostream>

#include "vector.hpp"
#include "actors.hpp"
#include "buffer.hpp"
#include "color.hpp"
#include "camera.hpp"
#include "parser.hpp"


// #define DEBUG_WORLD     1
#define sqr(x) (x * x)

#define HIT_NULL        0
#define HIT_PLANE       1
#define HIT_SPHERE      2
#define HIT_CYLINDER    3

#define MAX_DISTANCE   60.
#define SHADOW_FACTOR    .25


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
    World (Parser *parser, unsigned int width,
        unsigned int heigth, double fov);
    ~World ();
    /*
     * Manipulation of objects, adding, 
     * removing, etc.
     *
     */
    unsigned int AddPlane (Vector *center, Vector *normal,
        Color *colora, Color *colorb, double texscale);
    unsigned int AddSphere (Vector *center, double radius,
        Color *color);

    unsigned int AddPlane_FromEntry (Entry *entry);
    unsigned int AddSphere_FromEntry (Entry *entry);
    unsigned int AddLight_FromEntry (Entry *entry);
    unsigned int AddCamera_FromEntry (Entry *entry,
        unsigned int width, unsigned int heigth, double fov);

    unsigned int PopPlane ();
    unsigned int PopSphere ();
    /*
     * Rendering, writing output, etc.
     *
     */
    void TraceRay (Vector *origin, Vector *direction,
        Color *color);
    void Render ();
    void WritePNG (string filename);
};

#endif /* _WORLD_H */
