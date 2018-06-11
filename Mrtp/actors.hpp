/* File      : actors.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _ACTORS_H
#define _ACTORS_H

#include <Eigen/Core>

#include "pixel.hpp"
#include "texture.hpp"


class Actor {  //Abstract class
protected:
    Texture  *texture_;
    bool      hasShadow_;
    float    reflect_;

public:
    Actor ();
    bool HasShadow ();
    float Reflective ();
    virtual ~Actor ();
    virtual float Solve (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float mind, float maxd) = 0;
    virtual Pixel *PickPixel (Eigen::Vector3f *hit, Eigen::Vector3f *normal) = 0;
    virtual Eigen::Vector3f CalculateNormal (Eigen::Vector3f *hit) = 0;
};


//=============================================================================
class Sphere : public Actor {
    Eigen::Vector3f   center_;
    Eigen::Vector3f   tx_;
    Eigen::Vector3f   ty_;
    Eigen::Vector3f   tz_;
    float   R_;

public:
    Sphere (Eigen::Vector3f *center, float radius, Eigen::Vector3f *axis, float reflect, Texture *texture);
    float Solve (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float mind, float maxd);
    Pixel *PickPixel (Eigen::Vector3f *hit, Eigen::Vector3f *normal);
    Eigen::Vector3f CalculateNormal (Eigen::Vector3f *hit);
};


//=============================================================================
class Plane : public Actor {
    Eigen::Vector3f   center_;
    Eigen::Vector3f   normal_;
    Eigen::Vector3f   tx_;
    Eigen::Vector3f   ty_;
    float   scale_;

public:
    Plane (Eigen::Vector3f *center, Eigen::Vector3f *normal, float scale, float reflect, Texture *texture);
    float Solve (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float mind, float maxd);
    Pixel *PickPixel (Eigen::Vector3f *hit, Eigen::Vector3f *normal);
    Eigen::Vector3f CalculateNormal (Eigen::Vector3f *hit);
};


//=============================================================================
class Cylinder : public Actor {
    Eigen::Vector3f  A_;
    Eigen::Vector3f  B_;
    Eigen::Vector3f  tx_;
    Eigen::Vector3f  ty_;
    float  R_;
    float  span_;

public:
    Cylinder (Eigen::Vector3f *center, Eigen::Vector3f *direction, float radius, float span, float reflect, Texture *texture);
    float Solve (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float mind, float maxd);
    Pixel *PickPixel (Eigen::Vector3f *hit, Eigen::Vector3f *normal);
    Eigen::Vector3f CalculateNormal (Eigen::Vector3f *hit);
};

#endif /* !_ACTORS_H */
