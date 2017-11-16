/*
 *  Mrtp: A simple raytracing tool.
 *  Copyright (C) 2017  Mikolaj Feliks <mikolaj.feliks@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _RENDERER_H
#define _RENDERER_H


#include <cstddef>  /* NULL pointer. */
#include <string>
#include <cmath>

#ifdef _OPENMP
#include <omp.h>
#endif /* _OPENMP */

#include "color.hpp"
#include "vector.hpp"
#include "actors.hpp"
#include "world.hpp"
#include "buffer.hpp"

/*
 * Macros.
 */
#define DEG_TO_RAD(angle) (angle * M_PI / 180.0f)

enum LightModel_t {lightNone, lightLinear, lightQuadratic};


class Renderer {
    World   *world_;
    Buffer  *buffer_;

    /*
     * model: selects the light model (quenching of light with
     * increasing distance).
     *
     * maxdist: a distance at which the light is fully
     * quenched, unless the model is "none".
     *
     * shadow: a factor between <0..1>, defines the darkness
     * of shadows.
     *
     * cutoff: the maximum distance reached by rays.
     *
     * maxdepth: the maximum number of recursions of every 
     * reflected ray.
     *
     * relfshadow: if true, reflect shadows from shadowed surfaces.
     */
    LightModel_t  model_;
    double        maxdist_, shadow_, cutoff_;
    unsigned int  maxdepth_;
    bool          reflshadow_;

    double  fov_, ratio_, perspective_;
    unsigned int width_, height_, nthreads_;

    /* 
     * Private methods. 
     */
    bool SolveShadows (Vector *origin, Vector *direction, double maxdist, 
                       Actor *actor);
    bool SolveHits (Vector *origin, Vector *direction, Actor *actor, 
                    Actor **hitactor, double *currd);
    void TraceRay_r (Vector *origin, Vector *direction, 
                     unsigned int depth, double mixing, Color *color);
    void RenderBlock (Vector *vw, Vector *vh, Vector *vo, Vector *eye,
                      unsigned int block, unsigned int nlines);

public:
    Renderer (World *world, unsigned int width,
              unsigned int height, double fov,
              double distance, double shadowfactor, 
              LightModel_t lightmodel, unsigned int maxdepth, 
              bool reflshadow, unsigned int nthreads);
    ~Renderer ();

    void Render ();
    void Initialize ();
    void SaveFrame (std::string *path);
};

#endif /* _RENDERER_H */
