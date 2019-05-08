/* File      : actor.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _ACTOR_H
#define _ACTOR_H

#include <Eigen/Core>

#include "pixel.hpp"
#include "texture.hpp"


class Actor {
protected:
    bool hasShadow_;
    float reflect_;
    Texture *texture_;

    //Private methods
    static float solve_quadratic (float a, float b, float c, float mint, float maxt);
    static Eigen::Vector3f generate_unit_vector (Eigen::Vector3f *vector);

public:
    Actor ();
    bool has_shadow ();
    float get_reflect ();

    virtual ~Actor ();
    virtual float solve (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float mind, float maxd) = 0;
    virtual Pixel pick_pixel (Eigen::Vector3f *hit, Eigen::Vector3f *normal) = 0;
    virtual Eigen::Vector3f calculate_normal (Eigen::Vector3f *hit) = 0;
};

#endif /* !_ACTOR_H */
