/* File      : renderer.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _RENDERER_H
#define _RENDERER_H

#include <Eigen/Core>
#include <vector>

#include "pixel.hpp"
#include "light.hpp"
#include "camera.hpp"
#include "actor.hpp"
#include "world.hpp"


class Renderer {
    Camera  *camera_;
    Light   *light_;

    std::vector<Actor *> *actors_;

    std::vector<Pixel> framebuffer_;
    int    width_;
    int    height_;

    float   maxdist_;
    float   shadow_;
    float   bias_;
    int     maxdepth_;

    float  fov_;
    float  ratio_;
    float  perspective_;
    int    nthreads_;

    //Private methods
    bool solve_shadows (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float maxdist);
    Actor *solve_hits (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float *currd);
    Pixel trace_ray_r (Eigen::Vector3f *origin, Eigen::Vector3f *direction, int depth);
    void render_block (int block, int nlines);

public:
    Renderer (World *world, int width, int height, float fov, float distance, float shadow, float bias, int maxdepth, int nthreads);
    ~Renderer ();
    float render_scene ();
    void write_scene (char *filename);
};

#endif /* !_RENDERER_H */
