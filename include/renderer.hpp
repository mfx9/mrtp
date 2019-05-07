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


class CRenderer {
    CCamera  *camera_;
    CLight   *light_;

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
    bool SolveShadows (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float maxdist);
    Actor *SolveHits (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float *currd);
    Pixel TraceRay_r (Eigen::Vector3f *origin, Eigen::Vector3f *direction, int depth);
    void RenderBlock (int block, int nlines);

public:
    CRenderer (CWorld *world, int width, int height, float fov, float distance, float shadow, float bias, int maxdepth, int nthreads);
    ~CRenderer ();
    float Render ();
    void WriteScene (char *filename);
};

#endif /* !_RENDERER_H */
