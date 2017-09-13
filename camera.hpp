/*
 *  Mrtp: A simple raytracing tool.
 *  Copyright (C) 2017  Mikolaj Feliks <mikolaj.feliks@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef _CAMERA_H
#define _CAMERA_H

#include <cmath>
#include <cstdio>
#include "vector.hpp"

/*
 * Macros.
 */
#define DEG_TO_RAD(angle) (angle * M_PI / 180.0)


class Camera {
    Vector  eye_, lookat_;
    double  width_, height_,
        ratio_, perspective_, rotation_;

public:
    Camera (Vector *origin, Vector *target,
        unsigned int width, unsigned int height, 
        double fov, double roll);
    ~Camera ();

    void CalculateVectors (Vector *vw, Vector *vh, Vector *vo);
    void GetDimensions (unsigned int *width, unsigned int *height);
    void GetEye (Vector *vector);
};

#endif /* _CAMERA_H */
