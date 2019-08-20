/* File      : cylinder.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _CYLINDER_H
#define _CYLINDER_H

#include <Eigen/Core>
#include "actor.hpp"


namespace mrtp {

class Cylinder : public Actor {
  public:
    Cylinder(Eigen::Vector3f *center, Eigen::Vector3f *direction, 
             float radius, float span, float reflect, const char *texture);
    virtual ~Cylinder();
    virtual float solve(Eigen::Vector3f *origin, Eigen::Vector3f *direction, 
                        float mind, float maxd) override;
    virtual Pixel pick_pixel(Eigen::Vector3f *hit, Eigen::Vector3f *normal) override;
    virtual Eigen::Vector3f calculate_normal(Eigen::Vector3f *hit) override;

  private:
    Eigen::Vector3f A_;
    Eigen::Vector3f B_;
    Eigen::Vector3f tx_;
    Eigen::Vector3f ty_;
    float R_;
    float span_;
};

} //namespace mrtp

#endif //_CYLINDER_H
