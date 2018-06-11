/* File      : light.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include "light.hpp"

using namespace Eigen;

/*
================
Light
================
*/
Light::Light () {
}

/*
================
~Light
================
*/
Light::~Light () {
}

/*
================
Initialize
================
*/
void Light::Initialize (Vector3f *origin) {
    origin_ = *origin;
}

/*
================
CalculateRay

Calculates a ray between a hit point and 
a source of light
================
*/
Vector3f Light::CalculateRay (Vector3f *hit) {
    return (origin_ - (*hit));
}
