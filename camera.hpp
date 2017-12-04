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
#ifndef _CAMERA_H
#define _CAMERA_H

#include "vector.hpp"


class Camera {
    Vector  eye_;
    Vector  lookat_;
    Vector  wo_;
    Vector  wh_;
    Vector  wv_;
    double  rotation_;

public:
    Camera (Vector *origin, Vector *target, const double roll);
    ~Camera ();

    void CalculateWindow (const unsigned int width, const unsigned int height, 
                          const double perspective);
    void CalculateRay (const unsigned int windowx, const unsigned int windowy, 
                       Vector *origin, Vector *direction) const;
};

#endif /* !_CAMERA_H */
