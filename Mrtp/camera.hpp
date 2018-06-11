/* File      : camera.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _CAMERA_H
#define _CAMERA_H

#include <Eigen/Core>


class Camera {
    Eigen::Vector3f  eye_;
    Eigen::Vector3f  lookat_;
    Eigen::Vector3f  wo_;
    Eigen::Vector3f  wh_;
    Eigen::Vector3f  wv_;
    float  roll_;

public:
    Camera ();
    ~Camera ();
    void Initialize (Eigen::Vector3f *origin, Eigen::Vector3f *target, float roll);
    void CalculateWindow (int width, int height, float perspective);
    Eigen::Vector3f CalculateOrigin (int windowx, int windowy);
    Eigen::Vector3f CalculateDirection (Eigen::Vector3f *origin);
};

#endif /* !_CAMERA_H */
