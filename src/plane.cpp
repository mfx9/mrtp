/* File      : plane.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <Eigen/Geometry>

#include "plane.hpp"


namespace mrtp {

Plane::Plane(Eigen::Vector3f *center, Eigen::Vector3f *normal, float scale, 
             float reflect, const char *texture) {
    center_ = *center;
    normal_ = (1.0f / normal->norm()) * (*normal);
    scale_ = scale;
    reflect_ = reflect;
    has_shadow_ = false;

    Eigen::Vector3f tmp = generate_unit_vector(&normal_);
    tx_ = tmp.cross(normal_);
    tx_ *= (1.0f / tx_.norm());
    ty_ = normal_.cross(tx_);
    ty_ *= (1.0f / ty_.norm());

    texture_ = textureCollector.add(texture);
}

Pixel Plane::pick_pixel(Eigen::Vector3f *hit, Eigen::Vector3f *normal) {
    Eigen::Vector3f v = (*hit) - center_;
    // Calculate components of v (dot products)
    float vx = v.dot(tx_);
    float vy = v.dot(ty_);

    return texture_->pick_pixel(vx, vy, scale_);
}

float Plane::solve(Eigen::Vector3f *origin, Eigen::Vector3f *direction, float mind,
                   float maxd) {
    float bar = direction->dot(normal_);

    if (bar != 0.0f) {
        Eigen::Vector3f tmp = (*origin) - center_;
        float d = -tmp.dot(normal_) / bar;
        if ((d >= mind) && (d <= maxd)) {
            return d;
        }
    }
    return -1.0f;
}

Eigen::Vector3f Plane::calculate_normal(Eigen::Vector3f *hit) { return normal_; }

} //end namespace mrtp
