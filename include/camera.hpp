/* File      : camera.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _CAMERA_H
#define _CAMERA_H

#include <Eigen/Core>


class Camera {
    //Pointers to Cython variables
    Eigen::Vector3f  *eye_;
    Eigen::Vector3f  *lookat_;
    float            *roll_;

    Eigen::Vector3f  wo_;
    Eigen::Vector3f  wh_;
    Eigen::Vector3f  wv_;

public:
    Camera (float *center, float *target, float *roll);
    ~Camera ();
    void calculate_window (int width, int height, float perspective);

    Eigen::Vector3f calculate_origin (int windowx, int windowy);
    Eigen::Vector3f calculate_direction (Eigen::Vector3f *origin);
};

#endif /* !_CAMERA_H */
