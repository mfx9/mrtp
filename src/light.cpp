/* File      : light.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include "light.hpp"


namespace mrtp {

Light::Light(float center[]) { center_ << center[0], center[1], center[2]; }

void Light::update_center(float center[]) { center_ << center[0], center[1], center[2]; }

Eigen::Vector3f Light::calculate_ray(Eigen::Vector3f *hit) { return (center_ - (*hit)); }

} //end namespace mrtp
