/* File      : plane.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <Eigen/Geometry>
#include "plane.hpp"

using namespace Eigen;

namespace mrtp {

/*
================
Plane

Constructs a plane
================
*/
Plane::Plane(float *center, float *normal, float scale, float reflect,
             const char *texture) {
    normal_ = *(Vector3f *)normal;
    normal_ *= (1.0f / normal_.norm());

    reflect_ = reflect;
    scale_ = scale;

    center_ = *(Vector3f *)center;
    has_shadow_ = false;

    Vector3f tmp = generate_unit_vector(&normal_);

    tx_ = tmp.cross(normal_);
    tx_ *= (1.0f / tx_.norm());

    ty_ = normal_.cross(tx_);
    ty_ *= (1.0f / ty_.norm());

    texture_ = textureCollector.add(texture);
}

/*
================
pick_pixel

Picks a pixel from a plane's texture
================
*/
Pixel Plane::pick_pixel(Vector3f *hit, Vector3f *normal) {
    Vector3f v = (*hit) - center_;

    // Calculate components of v (dot products)
    float vx = v.dot(tx_);
    float vy = v.dot(ty_);

    return texture_->pick_pixel(vx, vy, scale_);
}

/*
================
solve

solves the intersection of a ray and a plane
================
*/
float Plane::solve(Vector3f *origin, Vector3f *direction, float mind,
                   float maxd) {
    float bar = direction->dot(normal_);

    if (bar != 0.0f) {
        Vector3f tmp = (*origin) - center_;
        float d = -tmp.dot(normal_) / bar;

        if ((d >= mind) && (d <= maxd)) {
            return d;
        }
    }
    return -1.0f;
}

/*
================
calculate_normal

Returns a normal to a plane
================
*/
Vector3f Plane::calculate_normal(Vector3f *hit) { return normal_; }

} //end namespace mrtp