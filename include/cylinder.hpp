/* File      : cylinder.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _CYLINDER_H
#define _CYLINDER_H

#include <Eigen/Core>

#include "actor.hpp"


class Cylinder : public Actor {
public:
    Cylinder (float *center, float *direction, float radius, float span, float reflect, const char *texture);
    ~Cylinder() {}
    float solve (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float mind, float maxd);
    Pixel pick_pixel (Eigen::Vector3f *hit, Eigen::Vector3f *normal);
    Eigen::Vector3f calculate_normal (Eigen::Vector3f *hit);

private:
    Eigen::Vector3f  A_;
    Eigen::Vector3f  B_;
    Eigen::Vector3f  tx_;
    Eigen::Vector3f  ty_;
    float  R_;
    float  span_;
};

#endif  //!_CYLINDER_H
