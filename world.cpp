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
#include "world.hpp"

using namespace std;


World::World (Parser *parser) {
    parser_     = parser;
    camera_     = NULL;
    light_      = NULL;
    actors_     = NULL;
    textures_   = NULL;

    nactors_    = 0;
    ntextures_  = 0;
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
    Actor    *actor;
    Texture  *texture;

    /* Destroy textures, actors, camera, light. */
    while (ntextures_ > 0) {
        texture = PopTexture ();
        delete texture;
    }
    while (nactors_ > 0) {
        actor = PopActor ();
        delete actor;
    }

    if (camera_ != NULL) {
        delete camera_;
    }
    if (light_  != NULL) {
        delete light_;
    }
}

void World::CreateCamera (Entry *entry) {
    string   key, *textual;
    double  *numerical;

    entry->Query (&key, &numerical, &textual);
    Vector position (numerical);

    entry->Query (&key, &numerical, &textual);
    Vector target (numerical);

    entry->Query (&key, &numerical, &textual);
    double roll = numerical[0];

    camera_ = new Camera (&position, &target, roll);
}

void World::CreateLight (Entry *entry) {
    string   key, *textual;
    double  *numerical;

    entry->Query (&key, &numerical, &textual);
    Vector position (numerical);

    light_ = new Light (&position);
}

void World::CreatePlane (Entry *entry) {
    string   key, *textual;
    double  *numerical;

    entry->Query (&key, &numerical, &textual);
    Vector center (numerical);

    entry->Query (&key, &numerical, &textual);
    Vector normal (numerical);

    entry->Query (&key, &numerical, &textual);
    double scale = numerical[0];

    entry->Query (&key, &numerical, &textual);
    double reflect = numerical[0];

    entry->Query (&key, &numerical, &textual);
    Color color ((real_t) numerical[0], (real_t) numerical[1], 
        (real_t) numerical[2]);

    entry->Query (&key, &numerical, &textual);
    Texture *texture = PushTexture (&textual[0]);

    Plane *plane = new Plane (&center, &normal, scale, reflect, 
            &color, texture);
    PushActor (plane);
}

void World::CreateSphere (Entry *entry) {
    string   key, *textual;
    double  *numerical;

    entry->Query (&key, &numerical, &textual);
    Vector position (numerical);

    entry->Query (&key, &numerical, &textual);
    double radius = numerical[0];

    entry->Query (&key, &numerical, &textual);
    Vector axis (numerical);

    entry->Query (&key, &numerical, &textual);
    double reflect = numerical[0];

    entry->Query (&key, &numerical, &textual);
    Color color ((real_t) numerical[0], (real_t) numerical[1], 
        (real_t) numerical[2]);

    entry->Query (&key, &numerical, &textual);
    Texture *texture = PushTexture (&textual[0]);

    Sphere *sphere = new Sphere (&position, radius, &axis, reflect, 
            &color, texture);
    PushActor (sphere);
}

void World::CreateCylinder (Entry *entry) {
    string   key, *textual;
    double  *numerical;

    entry->Query (&key, &numerical, &textual);
    Vector center (numerical);

    entry->Query (&key, &numerical, &textual);
    Vector direction (numerical);

    entry->Query (&key, &numerical, &textual);
    double radius = numerical[0];

    entry->Query (&key, &numerical, &textual);
    double span = numerical[0];

    entry->Query (&key, &numerical, &textual);
    double reflect = numerical[0];

    entry->Query (&key, &numerical, &textual);
    Color color ((real_t) numerical[0], (real_t) numerical[1], 
        (real_t) numerical[2]);

    entry->Query (&key, &numerical, &textual);
    Texture *texture = PushTexture (&textual[0]);

    Cylinder *cylinder = new Cylinder (&center, &direction, radius, 
            span, reflect, &color, texture);
    PushActor (cylinder);
}

void World::PushActor (Actor *actor) {
    Actor *next, *last;

    if (nactors_ < 1) {
        actors_ = actor;
    }
    else {
        next = actors_;
        do {
            last = next;
            next = last->Next ();
        } while (next != NULL);
        last->SetNext (actor);
    }
    nactors_++;
}

Actor *World::PopActor () {
    Actor *prev, *next, *last, *pop = NULL;

    if (nactors_ > 0) {
        last = actors_;
        prev = NULL;
        while ((next = last->Next ()) != NULL) {
            prev = last;
            last = next;
        }
        if (prev != NULL) {
            prev->SetNext (NULL);
        }
        pop = last;
        nactors_--;
    }
    return pop;
}

Texture *World::PushTexture (string *filename) {
    Texture *next, *last, *texture;
    bool found;

    /* Do not add an empty texture. */
    if (*filename == "") {
        return NULL;
    }

    /* Check if the texture already exists. */
    if (ntextures_ > 0) {
        next  = textures_;
        found = false;
        do {
            last = next;
            if (last->CheckFilename (filename)) {
                found = true;
                break;
            }
            next = last->Next ();
        } while (next != NULL);
        if (found) {
            return last;
        }
    }

    /* Create a new texture. */
    texture = new Texture (filename);
    texture->Allocate ();

    if (ntextures_ < 1) {
        textures_ = texture;
    }
    else {
        next = textures_;
        do {
            last = next;
            next = last->Next ();
        } while (next != NULL);
        last->SetNext (texture);
    }
    ntextures_++;
    return texture;
}

Texture *World::PopTexture () {
    Texture *prev, *next, *last, *pop = NULL;

    if (ntextures_ > 0) {
        last = textures_;
        prev = NULL;
        while ((next = last->Next ()) != NULL) {
            prev = last;
            last = next;
        }
        if (prev != NULL) {
            prev->SetNext (NULL);
        }
        pop = last;
        ntextures_--;
    }
    return pop;
}

void World::AssignCamera (Camera **camera) {
    *camera = camera_;
}

void World::AssignLightActors (Light **light, Actor **actors) {
    *light = light_;
    *actors = actors_;
}
