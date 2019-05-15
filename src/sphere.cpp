/* File      : sphere.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <Eigen/Geometry>
#include <cmath>

#include "sphere.hpp"


namespace mrtp {

Sphere::Sphere(Eigen::Vector3f *center, float radius, Eigen::Vector3f *axis, 
               float reflect, const char *texture) {
    center_ = *center;
    R_ = radius;
    has_shadow_ = true;
    reflect_ = reflect;

    ty_ = *axis;
    ty_ *= (1.0f / ty_.norm());
    Eigen::Vector3f tmp = generate_unit_vector(&ty_);
    tx_ = tmp.cross(ty_);
    tx_ *= (1.0f / tx_.norm());
    tz_ = ty_.cross(tx_);
    tz_ *= (1.0f / tz_.norm());

    texture_ = textureCollector.add(texture);
}

float Sphere::solve(Eigen::Vector3f *origin, Eigen::Vector3f *direction, float mind,
                    float maxd) {
    Eigen::Vector3f t = (*origin) - center_;
    float a = direction->dot(*direction);
    float b = 2.0f * direction->dot(t);
    float c = t.dot(t) - (R_ * R_);

    return solve_quadratic(a, b, c, mind, maxd);
}

Eigen::Vector3f Sphere::calculate_normal(Eigen::Vector3f *hit) {
    Eigen::Vector3f normal = (*hit) - center_;
    return (normal * (1.0f / normal.norm()));
}

/*
Guidelines:
https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
*/
Pixel Sphere::pick_pixel(Eigen::Vector3f *hit, Eigen::Vector3f *normal) {
    float dot = normal->dot(ty_);
    float phi = std::acos(-dot);
    float fracy = phi / M_PI;

    dot = normal->dot(tx_);
    float theta = std::acos(dot / std::sin(phi)) / (2.0f * M_PI);
    dot = normal->dot(tz_);
    float fracx = (dot > 0.0f) ? theta : (1.0f - theta);

    return texture_->pick_pixel(fracx, fracy, 1.0f);
}

} //end namespace mrtp
