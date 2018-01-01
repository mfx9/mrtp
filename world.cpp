/*
 * Mrtp: A simple raytracing tool.
 * Copyright (C) 2017  Mikolaj Feliks <mikolaj.feliks@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cstddef>
#include <iostream>  /* DEBUG */
#include "vector.hpp"
#include "motifs.hpp"
#include "world.hpp"

using namespace std;


World::World (Parser *parser) {
    parser_ = parser;
}

void World::Initialize () {
    Entry  *entry;

    parser_->StartQuery ();
    while (parser_->Query (&entry)) {

        entry->StartQuery ();

        if (entry->CheckID (entryCamera)) {
            CreateCamera (entry);
        }
        else if (entry->CheckID (entryLight)) {
            CreateLight (entry);
        }
        else if (entry->CheckID (entryPlane)) {
            CreatePlane (entry);
        }
        else if (entry->CheckID (entrySphere)) {
            CreateSphere (entry);
        }
        else if (entry->CheckID (entryCylinder)) {
            CreateCylinder (entry);
        }
    }
}

World::~World () {
    /* Destroy all textures and actors. */
    Texture *texture;

    while ((texture = PopTexture ()) != (Texture *) NULL) {
        delete texture;
    }

    Actor *actor;
    while ((actor = PopActor ()) != (Actor *) NULL) {
        delete actor;
    }

    /* Destroy camera and light. */
    delete camera_;
    delete light_;
}

void World::CreateCamera (Entry *entry) {
    string  *textual;
    double  *numerical;

    entry->Query (&numerical, &textual);
    Vector position (numerical);

    entry->Query (&numerical, &textual);
    Vector target (numerical);

    entry->Query (&numerical, &textual);
    double roll = numerical[0];

    camera_ = new Camera (&position, &target, roll);
}

void World::CreateLight (Entry *entry) {
    string  *textual;
    double  *numerical;

    entry->Query (&numerical, &textual);
    Vector position (numerical);

    light_ = new Light (&position);
}

void World::CreatePlane (Entry *entry) {
    string  *textual;
    double  *numerical;

    entry->Query (&numerical, &textual);
    Vector center (numerical);

    entry->Query (&numerical, &textual);
    Vector normal (numerical);

    entry->Query (&numerical, &textual);
    double scale = numerical[0];

    entry->Query (&numerical, &textual);
    double reflect = numerical[0];

    entry->Query (&numerical, &textual);
    Color color ((real_t) numerical[0], (real_t) numerical[1], (real_t) numerical[2]);

    entry->Query (&numerical, &textual);
    Texture *texture = PushTexture (&textual[0]);

    Plane *plane = new Plane (&center, &normal, scale, reflect, &color, texture);
    PushActor (plane);
}

void World::CreateSphere (Entry *entry) {
    string  *textual;
    double  *numerical;

    entry->Query (&numerical, &textual);
    Vector position (numerical);

    entry->Query (&numerical, &textual);
    double radius = numerical[0];

    entry->Query (&numerical, &textual);
    Vector axis (numerical);

    entry->Query (&numerical, &textual);
    double reflect = numerical[0];

    entry->Query (&numerical, &textual);
    Color color ((real_t) numerical[0], (real_t) numerical[1], (real_t) numerical[2]);

    entry->Query (&numerical, &textual);
    Texture *texture = PushTexture (&textual[0]);

    Sphere *sphere = new Sphere (&position, radius, &axis, reflect, &color, texture);
    PushActor (sphere);
}

void World::CreateCylinder (Entry *entry) {
    string  *textual;
    double  *numerical;

    entry->Query (&numerical, &textual);
    Vector center (numerical);

    entry->Query (&numerical, &textual);
    Vector direction (numerical);

    entry->Query (&numerical, &textual);
    double radius = numerical[0];

    entry->Query (&numerical, &textual);
    double span = numerical[0];

    entry->Query (&numerical, &textual);
    double reflect = numerical[0];

    entry->Query (&numerical, &textual);
    Color color ((real_t) numerical[0], (real_t) numerical[1], (real_t) numerical[2]);

    entry->Query (&numerical, &textual);
    Texture *texture = PushTexture (&textual[0]);

    Cylinder *cylinder = new Cylinder (&center, &direction, radius, span, reflect, &color, texture);
    PushActor (cylinder);
}

void World::PushActor (Actor *actor) {
    Actor *last;

    if (actors_.empty ()) {
        actorsHead_ = actor;
        actor->SetNext ((Actor *) NULL);
    }
    else {
        last = actors_.back ();
        last->SetNext (actor);
    }
    actors_.push_back (actor);
}

Actor *World::PopActor () {
    Actor *actor = (Actor *) NULL;

    while (!actors_.empty ()) {
        actor = actors_.back ();
        actors_.pop_back ();
    }
    return actor;
}

Texture *World::PushTexture (string *filename) {
    Texture  *texture;

    if (*filename == "!") {
        /* DEBUG
        cout << "Empty texture!" << endl; */
        return (Texture *) NULL;
    }
    /* Do not add a texture that already exists. */
    if (!textures_.empty ()) {
        list<Texture *>::iterator  current, last;

        current = textures_.begin ();
        last = textures_.end ();

        while (current != last) {
            texture = (*current);
            if (texture->CheckFilename (filename)) {
                return texture;
            }
            current++;
        }
    }
    /* Create a new texture. */
    texture = new Texture (filename);
    texture->Allocate ();
    textures_.push_back (texture);

    return texture;
}

Texture *World::PopTexture () {
    Texture *texture = (Texture *) NULL;

    if (!textures_.empty ()) {
        texture = textures_.back ();
        textures_.pop_back ();
    }
    return texture;
}

void World::Bind (Camera **camera, Light **light, Actor **actors) {
    *camera = camera_;
    *light = light_;
    *actors = actorsHead_;
}
