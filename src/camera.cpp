/* File      : camera.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <cmath>
#include <Eigen/Geometry>

#include "camera.hpp"

using namespace Eigen;

static float kDegreeToRadian = M_PI / 180.0f;


/*
================
Camera
================
*/
Camera::Camera (float *center, float *target, float *roll) {
    eye_ = (Vector3f *)center;
    lookat_ = (Vector3f *)target;
    roll_ = roll;
}

/*
================
~Camera
================
*/
Camera::~Camera () {
}

/*
================
CalculateWindow

Calculates vectors that span a window
================
*/
void Camera::CalculateWindow (int width, int height, float perspective) {
    //i is a vector between the camera and the center of the window
    Vector3f i = (*lookat_) - (*eye_);
    i *= (1.0f / i.norm ());

    Vector3f k;
    k << 0.0f, 0.0f, 1.0f;

    Vector3f j = i.cross (k);
    j *= (1.0f / j.norm ());

    k = j.cross (i);
    k *= (1.0f / k.norm ());

    //Rotate camera around the i axis
    float roll = (*roll_) * kDegreeToRadian;
    float sina = sin (roll);
    float cosa = cos (roll);

    Vector3f jp = cosa * j + sina * k;
    Vector3f kp = -sina * j + cosa * k;

    j = jp;
    k = kp;

    //Calculate the central point of the window
    Vector3f center = (*eye_) + perspective * i;

    //Find three corners of the window
    wo_ = center - 0.5f * j + 0.5f * k;

    Vector3f h = wo_ + j;
    Vector3f v = wo_ - k;

    //Find vectors spanning the window
    wh_ = (1.0f / (float) width) * (h - wo_);
    wv_ = (1.0f / (float) height) * (v - wo_);
}

/*
================
CalculateOrigin
================
*/
Vector3f Camera::CalculateOrigin (int windowx, int windowy) {
    return (wo_ + (float) windowx * wh_ + (float) windowy * wv_);
}

/*
================
CalculateDirection
================
*/
Vector3f Camera::CalculateDirection (Vector3f *origin) {
    Vector3f direction = (*origin) - (*eye_);

    return (direction * (1.0f / direction.norm ()));
}
