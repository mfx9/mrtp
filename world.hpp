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

#define LIGHT_MODEL_NONE       0
#define LIGHT_MODEL_LINEAR     1
#define LIGHT_MODEL_QUADRATIC  2


class World {
    Buffer  *buffer_;

    Camera  *camera_;
    Light   *light_;

    /*
     * Model to quench light with increasing
     * distance.
     *
     * At maxdist, the light is fully quenched,
     * unless the model is "none". 
     */
    char    model_;
    double  maxdist_;

    /*
     * Shadow factor between <0..1>, defines how
     * "deep" shadows are.
     */
    double  shadow_;

    /*
     * Maximum distance reached by rays.
     */
    double  cutoff_;

    /*
     * Planes, spheres and cylinders are one-way
     * linked lists.
     */
    Plane        *planes_;
    unsigned int  nplanes_;

    Sphere       *spheres_;
    unsigned int  nspheres_;

    Cylinder     *cylinders_;
    unsigned int  ncylinders_;

    Parser    *parser_;
    double     fov_;
    unsigned int width_, height_;

public:
    World (Parser *parser, unsigned int width,
        unsigned int height, double fov,
        double distance, double shadowfactor, 
        char lightmodel);
    ~World ();
    bool Initialize ();

    /*
     * Manipulation of objects, adding, removing, etc.
     */
    unsigned int AddPlane (Vector *center, Vector *normal,
        Color *colora, Color *colorb, double texscale);
    unsigned int AddSphere (Vector *center, double radius,
        Color *color);
    unsigned int AddCylinder (Vector *A, Vector *B,
        double radius, Color *color);

    unsigned int PopPlane ();
    unsigned int PopSphere ();
    unsigned int PopCylinder ();

    /*
     * Rendering, writing output, etc.
     *
     */
    void TraceRay (Vector *origin, Vector *direction,
        Color *color);
    void Render ();
    void WritePNG (string *filename);
};

#endif /* _WORLD_H */
