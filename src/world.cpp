/* File      : world.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include "world.hpp"

using namespace std;


/*
================
AddCamera
================
*/
void World::AddCamera (Camera *camera) {
    cameras_.push_back (camera);
}

/*
================
AddLight
================
*/
void World::AddLight (Light *light) {
    lights_.push_back (light);
}

/*
================
AddPlane
================
*/
void World::AddPlane (Plane *plane) {
    actors_.push_back (plane);
}

/*
================
AddSphere
================
*/
void World::AddSphere (Sphere *sphere) {
    actors_.push_back (sphere);
}

/*
================
AddCylinder
================
*/
void World::AddCylinder (Cylinder *cylinder) {
    actors_.push_back (cylinder);
}

/*
================
GetLight
================
*/
Light *World::GetLight () {
    return lights_.back ();
}

/*
================
GetCamera
================
*/
Camera *World::GetCamera () {
    return cameras_.back ();
}

/*
================
GetActors
================
*/
vector<Actor *> *World::GetActors () {
    return &actors_;
}
