/* File      : camera.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <Eigen/Geometry>
#include <cmath>
#include "camera.hpp"

using namespace Eigen;


namespace mrtp {

Camera::Camera(float eye[], float lookat[], float roll) {
    eye_ << eye[0], eye[1], eye[2];
    lookat_ << lookat[0], lookat[1], lookat[2];
    roll_ = roll;
}

void Camera::update_eye(float eye[]) {
    eye_ << eye[0], eye[1], eye[2];
}

void Camera::update_lookat(float lookat[]) {
    lookat_ << lookat[0], lookat[1], lookat[2];
}

void Camera::calculate_window(int width, int height, float perspective) {
    // i is a vector between the camera and the center of the window
    Vector3f i = lookat_ - eye_;
    i *= (1.0f / i.norm());

    Vector3f k;
    k << 0.0f, 0.0f, 1.0f;

    Vector3f j = i.cross(k);
    j *= (1.0f / j.norm());

    k = j.cross(i);
    k *= (1.0f / k.norm());

    // Rotate camera around the i axis
    float roll = roll_ * M_PI / 180.0f;
    float sina = sin(roll);
    float cosa = cos(roll);

    Vector3f jp = cosa * j + sina * k;
    Vector3f kp = -sina * j + cosa * k;

    j = jp;
    k = kp;

    // Calculate the central point of the window
    Vector3f center = eye_ + perspective * i;

    // Find three corners of the window
    wo_ = center - 0.5f * j + 0.5f * k;

    Vector3f h = wo_ + j;
    Vector3f v = wo_ - k;

    // Find vectors spanning the window
    wh_ = (1.0f / (float)width) * (h - wo_);
    wv_ = (1.0f / (float)height) * (v - wo_);
}

Vector3f Camera::calculate_origin(int windowx, int windowy) {
    return (wo_ + (float)windowx * wh_ + (float)windowy * wv_);
}

Vector3f Camera::calculate_direction(Vector3f *origin) {
    Vector3f direction = (*origin) - eye_;
    return (direction * (1.0f / direction.norm()));
}

} //end namespace mrtp