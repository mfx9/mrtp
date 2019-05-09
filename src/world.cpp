/* File      : world.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include "world.hpp"

using namespace std;

namespace mrtp {

void World::add_camera(Camera *camera) { cameras_.push_back(camera); }

void World::add_light(Light *light) { lights_.push_back(light); }

void World::add_plane(Plane *plane) { actors_.push_back(plane); }

void World::add_sphere(Sphere *sphere) { actors_.push_back(sphere); }

void World::add_cylinder(Cylinder *cylinder) { actors_.push_back(cylinder); }

Light *World::get_light() { return lights_.back(); }

Camera *World::get_camera() { return cameras_.back(); }

vector<Actor *> *World::get_actors() { return &actors_; }

} //end namespace mrtp