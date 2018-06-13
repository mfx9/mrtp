/* File      : cylinder.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <cmath>
#include <Eigen/Geometry>

#include "cylinder.hpp"

using namespace Eigen;


/*
================
CCylinder

Constructs a cylinder
================
*/
CCylinder::CCylinder (float *center, float *direction, float radius, 
                    float span, float reflect, const char *texture) {
    A_ = *(Vector3f *)center;

    B_ = *(Vector3f *)direction;
    B_ *= (1.0f / B_.norm ());

    R_ = radius;
    span_ = span;
    reflect_ = reflect;
    hasShadow_ = true;

    ty_ = GenerateUnitVector (&B_);
    tx_ = ty_.cross (B_);
    tx_ *= (1.0f / tx_.norm ());

    texture_ = textureCollector.Add (texture);
}

/*
================
Solve

Solves the intersection of a ray and a cylinder

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
================
*/
float CCylinder::Solve (Vector3f *O, Vector3f *D, float mind, float maxd) {
    Vector3f tmp = (*O) - A_;

    float a = D->dot (tmp);
    float b = D->dot (B_);
    float d = tmp.dot (B_);
    float f = (R_ * R_) - tmp.dot (tmp);

    //Solving a quadratic equation for t
    float aa = 1.0f - (b * b);
    float bb = 2.0f * (a - b * d);
    float cc = -(d * d) - f;
    float t  = SolveQuadratic (aa, bb, cc, mind, maxd);

    if (t > 0.0f) {
        //Check if the cylinder is finite
        if (span_ > 0.0f) {
            float alpha = d + t * b;

            if ((alpha < -span_) || (alpha > span_)) {
                return -1.0f;
            }
        }
    }
    return t;
}

/*
================
CalculateNormal

Calculates normal to a cylinder

N = Hit - [B . (Hit - A)] * B
================
*/
Vector3f CCylinder::CalculateNormal (Vector3f *hit) {
    Vector3f tmp = (*hit) - A_;
    float alpha = B_.dot (tmp);

    Vector3f bar = A_ + alpha * B_;
    Vector3f normal = (*hit) - bar;

    return (normal * (1.0f / normal.norm ()));
}

/*
================
PickPixel

Picks a pixel from a cylinders's texture
================
*/
Pixel CCylinder::PickPixel (Vector3f *hit, Vector3f *normal) {
    Vector3f tmp = (*hit) - A_;

    float alpha = tmp.dot (B_);
    float dot   = normal->dot (tx_);
    float fracx = acos (dot) / M_PI;
    float fracy = alpha / (2.0f * M_PI * R_);

    return texture_->PickPixel (fracx, fracy, 1.0f);
}
