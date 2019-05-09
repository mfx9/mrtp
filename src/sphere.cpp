/* File      : sphere.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <Eigen/Geometry>
#include <cmath>
#include "sphere.hpp"

using namespace Eigen;


namespace mrtp {

Sphere::Sphere(float *center, float radius, float *axis, float reflect,
               const char *texture) {
    center_ = *(Vector3f *)center;

    R_ = radius;
    has_shadow_ = true;
    reflect_ = reflect;

    ty_ = *(Vector3f *)axis;
    ty_ *= (1.0f / ty_.norm());
    Vector3f tmp = generate_unit_vector(&ty_);
    tx_ = tmp.cross(ty_);
    tx_ *= (1.0f / tx_.norm());
    tz_ = ty_.cross(tx_);
    tz_ *= (1.0f / tz_.norm());

    texture_ = textureCollector.add(texture);
}

float Sphere::solve(Vector3f *origin, Vector3f *direction, float mind,
                    float maxd) {
    Vector3f t = (*origin) - center_;
    float a = direction->dot(*direction);
    float b = 2.0f * direction->dot(t);
    float c = t.dot(t) - (R_ * R_);

    return solve_quadratic(a, b, c, mind, maxd);
}

Vector3f Sphere::calculate_normal(Vector3f *hit) {
    Vector3f normal = (*hit) - center_;
    return (normal * (1.0f / normal.norm()));
}

/*
Guidelines:
https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html
*/
Pixel Sphere::pick_pixel(Vector3f *hit, Vector3f *normal) {
    float dot = normal->dot(ty_);
    float phi = acos(-dot);
    float fracy = phi / M_PI;

    dot = normal->dot(tx_);
    float theta = acos(dot / sin(phi)) / (2.0f * M_PI);
    dot = normal->dot(tz_);
    float fracx = (dot > 0.0f) ? theta : (1.0f - theta);

    return texture_->pick_pixel(fracx, fracy, 1.0f);
}

} //end namespace mrtp