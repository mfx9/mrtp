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


class CWorld {
    std::vector<Actor *> actors_;
    std::vector<CLight *> lights_;
    std::vector<CCamera *> cameras_;

public:
    void AddCamera (CCamera *camera);
    void AddLight (CLight *light);
    void AddPlane (CPlane *plane);
    void AddSphere (CSphere *sphere);
    void AddCylinder (CCylinder *cylinder);

    //Used for binding to renderer
    CLight *GetLight ();
    CCamera *GetCamera ();
    std::vector<Actor *> *GetActors ();
};

#endif /* !_WORLD_H */
