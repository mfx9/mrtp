/* File      : renderer.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _RENDERER_H
#define _RENDERER_H

#include <Eigen/Core>
#include <vector>

#include "actor.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "pixel.hpp"
#include "world.hpp"

namespace mrtp {

class Renderer {
  public:
    Renderer(World *world, int width, int height, float fov, float distance,
             float shadow, float bias, int maxdepth, int nthreads);
    ~Renderer() {}
    float render_scene();
    void write_scene(const char *path);

  private:
    World *world_;
    std::vector<Pixel> framebuffer_;
    int width_;
    int height_;
    int maxdepth_;
    int nthreads_;
    float maxdist_;
    float shadow_;
    float bias_;
    float fov_;
    float ratio_;
    float perspective_;

    bool solve_shadows(Eigen::Vector3f *origin, Eigen::Vector3f *direction,
                       float maxdist);
    Actor *solve_hits(Eigen::Vector3f *origin, Eigen::Vector3f *direction,
                      float *currd);
    Pixel trace_ray_r(Eigen::Vector3f *origin, Eigen::Vector3f *direction,
                      int depth);
    void render_block(int block, int nlines);
};

} //end namespace mrtp

#endif //!_RENDERER_H
