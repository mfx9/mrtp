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
void CWorld::AddCamera (CCamera *camera) {
    cameras_.push_back (camera);
}

/*
================
AddLight
================
*/
void CWorld::AddLight (CLight *light) {
    lights_.push_back (light);
}

/*
================
AddPlane
================
*/
void CWorld::AddPlane (CPlane *plane) {
    actors_.push_back (plane);
}

/*
================
AddSphere
================
*/
void CWorld::AddSphere (CSphere *sphere) {
    actors_.push_back (sphere);
}

/*
================
AddCylinder
================
*/
void CWorld::AddCylinder (CCylinder *cylinder) {
    actors_.push_back (cylinder);
}

/*
================
GetLight
================
*/
CLight *CWorld::GetLight () {
    return lights_.back ();
}

/*
================
GetCamera
================
*/
CCamera *CWorld::GetCamera () {
    return cameras_.back ();
}

/*
================
GetActors
================
*/
vector<Actor *> *CWorld::GetActors () {
    return &actors_;
}
