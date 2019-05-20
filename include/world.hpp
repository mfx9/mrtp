/* File      : world.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _WORLD_H
#define _WORLD_H

#include <memory>
#include <vector>
#include <list>
#include "cpptoml.h"

#include "actor.hpp"
#include "camera.hpp"
#include "cylinder.hpp"
#include "light.hpp"
#include "plane.hpp"
#include "sphere.hpp"


namespace mrtp {

enum WorldStatus_t {ws_ok, ws_fail, ws_err_camera, ws_err_light, 
                    ws_err_no_actors};

class World {
  public:
    World(const char *path): path_(path) {}
    ~World() {}
    WorldStatus_t initialize();

    Camera *ptr_camera_;
    Light *ptr_light_;
    std::vector<Actor *> ptr_actors_;

  private:
    WorldStatus_t load_planes(std::shared_ptr<cpptoml::table_array> table);
    WorldStatus_t load_spheres(std::shared_ptr<cpptoml::table_array> table);
    WorldStatus_t load_cylinders(std::shared_ptr<cpptoml::table_array> table);

    const char *path_;
    std::list<Camera> cameras_;
    std::list<Light> lights_;
    std::list<Plane> planes_;
    std::list<Sphere> spheres_;
    std::list<Cylinder> cylinders_;
};

} //end namespace mrtp

#endif //_WORLD_H