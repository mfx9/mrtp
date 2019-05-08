/* File      : light.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _LIGHT_H
#define _LIGHT_H

#include <Eigen/Core>


class Light {
    Eigen::Vector3f *center_;

public:
    Light (float *center);
    ~Light ();
    Eigen::Vector3f calculate_ray (Eigen::Vector3f *hit);
};

#endif /* !_LIGHT_H */
