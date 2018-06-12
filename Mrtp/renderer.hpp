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
#include "world.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "actors.hpp"


class CRenderer {
    Camera  *camera_;
    Light   *light_;

    std::vector<Actor *> *actors_;

    std::vector<Pixel> buffer_;
    int    width_;
    int    height_;

    float   maxdist_;   //Distance at which the light is fully quenched
    float   shadow_;   //Factor between <0..1> defining the darkness of shadows
    float   cutoff_;   //Maximum distance reached by rays
    int     maxdepth_;  //Maximum number of recursions of every reflected ray
    bool    reflshadow_;   //If true, reflect shadows from shadowed surfaces

    float  fov_;
    float  ratio_;
    float  perspective_;
    int    nthreads_;

    //Private methods
    bool SolveShadows (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float maxdist);
    Actor *SolveHits (Eigen::Vector3f *origin, Eigen::Vector3f *direction, float *currd);
    void TraceRay_r (Eigen::Vector3f *origin, Eigen::Vector3f *direction, int depth, float mixing, Pixel *color);
    void RenderBlock (int block, int nlines);

public:
    CRenderer (CWorld *world, int width, int height, float fov, float distance, float shadowfactor, int maxdepth, int reflshadow, int nthreads);
    ~CRenderer ();
    float Render ();
    void WriteScene (char *filename);
};

#endif /* !_RENDERER_H */
