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
#ifndef _WORLD_H
#define _WORLD_H

#include <cstddef>  /* NULL pointer. */
#include <string>
#include <iostream>

#include "vector.hpp"
#include "actors.hpp"
#include "camera.hpp"
#include "parser.hpp"
#include "color.hpp"
#include "texture.hpp"


class World {
    Parser  *parser_;
    Camera  *camera_;
    Light   *light_;

    Plane        *planes_;
    unsigned int  nplanes_;

    Sphere       *spheres_;
    unsigned int  nspheres_;

    Cylinder     *cylinders_;
    unsigned int  ncylinders_;

    Texture      *textures_;
    unsigned int  ntextures_;

public:
    World (Parser *parser);
    ~World ();
    void Initialize ();

    void GetCamera (Camera **camera);
    void GetLight (Light **light);
    void GetActors (Plane **planes, Sphere **spheres, Cylinder **cylinders);

    void AddPlane (Vector *center, Vector *normal, double texscale, 
                   double reflect, Color *color, Texture *texture);
    unsigned int PopPlane ();

    void AddSphere (Vector *center, double radius, Vector *axis, 
                    double reflect, Color *color, Texture *texture);
    unsigned int PopSphere ();

    void AddCylinder (Vector *center, Vector *direction, double radius, 
                      double span, double reflect, Color *color, 
                      Texture *texture);
    unsigned int PopCylinder ();

    Texture *AddTexture (std::string *filename);
    unsigned int PopTexture ();
};

#endif /* _WORLD_H */
