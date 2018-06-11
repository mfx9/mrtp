/* File      : world.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _WORLD_H
#define _WORLD_H

#include <list>
#include <vector>

#include "light.hpp"
#include "camera.hpp"
#include "actors.hpp"
#include "texture.hpp"


class CWorld {
    std::vector<Actor *> actors_;

    //Use of lists is necessary as adding elements does not
    //cause reallocation, which would invalidate pointers 
    //in actors_

    std::list<Plane> planes_;
    std::list<Sphere> spheres_;
    std::list<Cylinder> cylinders_;

    std::list<Texture> textures_;

    Camera camera_;
    Light light_;

    //Private methods
    Texture *AddTexture (char *path);

public:
    void AddCamera (float *origin, float *target, float roll);
    void AddLight (float *origin);
    void AddPlane (float *center, float *normal, float scale, float reflect, char *path);
    void AddSphere (float *center, float *axis, float radius, float reflect, char *path);
    void AddCylinder (float *center, float *direction, float radius, float span, float reflect, char *path);

    //Used for binding to renderer
    Camera *GetCamera ();
    Light *GetLight ();
    std::vector<Actor *> *GetActors ();
};

#endif /* !_WORLD_H */
