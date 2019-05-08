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
Light::Light (float *center) {
    center_ = (Vector3f *)center;
}

/*
================
calculate_ray

Calculates a ray between a hit point and 
a source of light
================
*/
Vector3f Light::calculate_ray (Vector3f *hit) {
    return ((*center_) - (*hit));
}
