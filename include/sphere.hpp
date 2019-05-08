/* File      : sphere.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _SPHERE_H
#define _SPHERE_H

#include <Eigen/Core>

#include "actor.hpp"


class Sphere : public Actor {
    Eigen::Vector3f   center_;
    Eigen::Vector3f   tx_;
    Eigen::Vector3f   ty_;
    Eigen::Vector3f   tz_;
    float   R_;

public:
    Sphere (float *center, float radius, float *axis, float reflect, const char *texture);
    float solve (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float mind, float maxd);
    Pixel pick_pixel (Eigen::Vector3f *hit, Eigen::Vector3f *normal);
    Eigen::Vector3f calculate_normal (Eigen::Vector3f *hit);
};

#endif /* !_SPHERE_H */
