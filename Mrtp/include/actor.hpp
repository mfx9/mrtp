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
    static float SolveQuadratic (float a, float b, float c, float mint, float maxt);
    static Eigen::Vector3f GenerateUnitVector (Eigen::Vector3f *vector);

public:
    Actor ();
    bool HasShadow ();
    float Reflective ();

    virtual ~Actor ();
    virtual float Solve (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float mind, float maxd) = 0;
    virtual Pixel PickPixel (Eigen::Vector3f *hit, Eigen::Vector3f *normal) = 0;
    virtual Eigen::Vector3f CalculateNormal (Eigen::Vector3f *hit) = 0;
};

#endif /* !_ACTOR_H */
