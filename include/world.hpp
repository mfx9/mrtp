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
    std::vector<Actor *> actors_;
    std::vector<Light *> lights_;
    std::vector<Camera *> cameras_;

public:
    void AddCamera (Camera *camera);
    void AddLight (Light *light);
    void AddPlane (Plane *plane);
    void AddSphere (Sphere *sphere);
    void AddCylinder (Cylinder *cylinder);

    //Used for binding to renderer
    Light *GetLight ();
    Camera *GetCamera ();
    std::vector<Actor *> *GetActors ();
};

#endif /* !_WORLD_H */
