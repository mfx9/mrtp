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
    Eigen::Vector3f   center_;
    Eigen::Vector3f   normal_;
    Eigen::Vector3f   tx_;
    Eigen::Vector3f   ty_;
    float   scale_;

public:
    Plane (float *center, float *normal, float scale, float reflect, const char *texture);
    float Solve (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float mind, float maxd);
    Pixel PickPixel (Eigen::Vector3f *hit, Eigen::Vector3f *normal);
    Eigen::Vector3f CalculateNormal (Eigen::Vector3f *hit);
};

#endif /* !_PLANE_H */
