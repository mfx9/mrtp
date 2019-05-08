/* File      : plane.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _PLANE_H
#define _PLANE_H

#include <Eigen/Core>

#include "actor.hpp"


class Plane : public Actor {
public:
    Plane (float *center, float *normal, float scale, float reflect, const char *texture);
    ~Plane() {}
    float solve (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float mind, float maxd);
    Pixel pick_pixel (Eigen::Vector3f *hit, Eigen::Vector3f *normal);
    Eigen::Vector3f calculate_normal (Eigen::Vector3f *hit);

private:
    Eigen::Vector3f   center_;
    Eigen::Vector3f   normal_;
    Eigen::Vector3f   tx_;
    Eigen::Vector3f   ty_;
    float   scale_;
};

#endif /* !_PLANE_H */
