/* File      : cylinder.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <Eigen/Geometry>
#include <cmath>

#include "cylinder.hpp"


namespace mrtp {

Cylinder::Cylinder(Eigen::Vector3f *center, Eigen::Vector3f *direction, float radius, 
                   float span, float reflect, const char *texture) {
    A_ = *center;
    B_ = *direction;
    B_ *= (1.0f / B_.norm());
    R_ = radius;
    span_ = span;
    reflect_ = reflect;
    has_shadow_ = true;

    ty_ = generate_unit_vector(&B_);
    tx_ = ty_.cross(B_);
    tx_ *= (1.0f / tx_.norm());

    texture_ = textureCollector.add(texture);
}

/*
Capital letters are vectors.
  A       Origin    of cylinder
  B       Direction of cylinder
  O       Origin    of ray
  D       Direction of ray
  P       Hit point on cylinder's surface
  X       Point on cylinder's axis closest to the hit point
  t       Distance between ray's      origin and P
  alpha   Distance between cylinder's origin and X

 (P - X) . B = 0
 |P - X| = R  => (P - X) . (P - X) = R^2

 P = O + t * D
 X = A + alpha * B
 T = O - A
 ...
 2t * (T.D - alpha * D.B)  +  t^2 - 2 * alpha * T.B  +
     +  alpha^2  =  R^2 - T.T
 a = T.D
 b = D.B
 d = T.B
 f = R^2 - T.T

 t^2 * (1 - b^2)  +  2t * (a - b * d)  -
     -  d^2 - f = 0    => t = ...
 alpha = d + t * b
*/
float Cylinder::solve(Eigen::Vector3f *O, Eigen::Vector3f *D, float mind, float maxd) {
    Eigen::Vector3f tmp = (*O) - A_;

    float a = D->dot(tmp);
    float b = D->dot(B_);
    float d = tmp.dot(B_);
    float f = (R_ * R_) - tmp.dot(tmp);

    // Solving a quadratic equation for t
    float aa = 1.0f - (b * b);
    float bb = 2.0f * (a - b * d);
    float cc = -(d * d) - f;
    float t = solve_quadratic(aa, bb, cc, mind, maxd);

    if (t > 0.0f) {
        // Check if the cylinder is finite
        if (span_ > 0.0f) {
            float alpha = d + t * b;
            if ((alpha < -span_) || (alpha > span_)) {
                return -1.0f;
            }
        }
    }
    return t;
}

Eigen::Vector3f Cylinder::calculate_normal(Eigen::Vector3f *hit) {
    // N = Hit - [B . (Hit - A)] * B
    Eigen::Vector3f tmp = (*hit) - A_;
    float alpha = B_.dot(tmp);
    Eigen::Vector3f bar = A_ + alpha * B_;
    Eigen::Vector3f normal = (*hit) - bar;

    return (normal * (1.0f / normal.norm()));
}

Pixel Cylinder::pick_pixel(Eigen::Vector3f *hit, Eigen::Vector3f *normal) {
    Eigen::Vector3f tmp = (*hit) - A_;
    float alpha = tmp.dot(B_);
    float dot = normal->dot(tx_);
    float fracx = acos(dot) / M_PI;
    float fracy = alpha / (2.0f * M_PI * R_);

    return texture_->pick_pixel(fracx, fracy, 1.0f);
}

} //end namespace mrtp
