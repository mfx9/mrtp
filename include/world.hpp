/* File      : world.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _WORLD_H
#define _WORLD_H

#include <vector>

#include "light.hpp"
#include "camera.hpp"
#include "actor.hpp"
#include "plane.hpp"
#include "sphere.hpp"
#include "cylinder.hpp"


class World {
public:
    void add_camera (Camera *camera);
    void add_light (Light *light);
    void add_plane (Plane *plane);
    void add_sphere (Sphere *sphere);
    void add_cylinder (Cylinder *cylinder);

    //Used for binding to renderer
    Light *get_light ();
    Camera *get_camera ();
    std::vector<Actor *> *get_actors ();

private:
    std::vector<Actor *> actors_;
    std::vector<Light *> lights_;
    std::vector<Camera *> cameras_;
};

#endif  //!_WORLD_H
