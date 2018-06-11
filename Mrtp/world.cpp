/* File      : world.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include "world.hpp"

#include <iostream>
#include <Eigen/Core>

using namespace std;
using namespace Eigen;


/*
================
AddCamera
================
*/
void CWorld::AddCamera (float *origin, float *target, float roll) {
    camera_.Initialize ((Vector3f *)origin, (Vector3f *)target, roll);
}

/*
================
AddLight
================
*/
void CWorld::AddLight (float *origin) {
    light_.Initialize ((Vector3f *)origin);
}

/*
================
AddPlane
================
*/
void CWorld::AddPlane (float *center, float *normal, float scale, float reflect, char *path) {
    Texture *texture = AddTexture (path);

    Plane plane ((Vector3f *)center, (Vector3f *)normal, scale, reflect, texture);
    planes_.push_back (plane);

    Plane *last = &planes_.back ();
    actors_.push_back (last);
}

/*
================
AddSphere
================
*/
void CWorld::AddSphere (float *center, float *axis, float radius, float reflect, char *path) {
    //TODO
}

/*
================
AddCylinder
================
*/
void CWorld::AddCylinder (float *center, float *direction, float radius, float span, float reflect, char *path) {
    //TODO
}

/*
================
AddTexture

Adds a new texture or reuses one that already 
exists in the memory
================
*/
Texture *CWorld::AddTexture (char *path) {
    string filename (path);

    for (auto t=textures_.begin (); t!=textures_.end (); t++) {
        Texture *texture = &(*t);

        if (texture->CheckFilename (filename)) {
            return texture;
        }
    }

    Texture texture (filename);
    texture.Load ();

    textures_.push_back (texture);
    Texture *last = &textures_.back ();

    return last;
}

/*
================
GetCamera
================
*/
Camera *CWorld::GetCamera () {
    return &camera_;
}

/*
================
GetLight
================
*/
Light *CWorld::GetLight () {
    return &light_;
}

/*
================
GetActors
================
*/
std::vector<Actor *> *CWorld::GetActors () {
    return &actors_;
}
