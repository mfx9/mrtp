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

#include <list>
#include <string>

#include "actors.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "texture.hpp"
#include "parser.hpp"


class World {
    Parser  *parser_;
    Camera  *camera_;
    Light   *light_;

    std::list<Actor *> actors_;
    Actor *actorsHead_;

    std::list<Texture *> textures_;

    /*
     * Private methods.
     */
    void CreateCamera (Entry *entry);
    void CreateLight (Entry *entry);
    void CreatePlane (Entry *entry);
    void CreateSphere (Entry *entry);
    void CreateCylinder (Entry *entry);

    void PushActor (Actor *actor);
    Actor *PopActor ();

    Texture *PushTexture (std::string *filename);
    Texture *PopTexture ();

public:
    World (Parser *parser);
    ~World ();

    void Initialize ();
    void Bind (Camera **camera, Light **light, Actor **actors);
};

#endif /* _WORLD_H */
