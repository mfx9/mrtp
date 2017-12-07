/*
 * Mrtp: A simple raytracing tool.
 * Copyright (C) 2017  Mikolaj Feliks <mikolaj.feliks@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cmath>
#include "camera.hpp"

const double kDegreeToRadian = M_PI / 180.0f;


Camera::Camera (Vector *origin, Vector *target, const double roll) {
    origin->CopyTo (&eye_);
    target->CopyTo (&lookat_);

    rotation_ = kDegreeToRadian * roll;
}

Camera::~Camera () {
}

void Camera::CalculateWindow (const unsigned int width, const unsigned int height, 
                              const double perspective) {
    /*
     * Calculate vectors that span the window.
     */

    /*
     * i is a vector between the camera and
     * the center of the window.
     */
    Vector i = lookat_ - eye_;
    i.Normalize_InPlace ();

    /*
     * if (i.x == 0.0 && i.y == 0.0)
     *     return false;
     */
    Vector j, k (0.0f, 0.0f, 1.0f);
    j = i ^ k;
    j.Normalize_InPlace ();

    /*
     * Flip the cross-product instead?
     */
    j.Scale_InPlace (-1.0f);
    k = i ^ j;
    k.Normalize_InPlace ();

    /*
     * Apply camera rotation around
     * the i axis.
     *
     * j' = cosa * j + sin * k
     * k' = (-sina) * j + cosa * k
     */
    double sina = sin (rotation_), 
        cosa = cos (rotation_);
    Vector T, Q, jp, kp;

    T  = j * cosa;
    Q  = k * sina;
    jp = T + Q;
    T  = j * (-sina);
    Q  = k * cosa;
    kp = T + Q;
    jp.CopyTo (&j);
    kp.CopyTo (&k);
    /*
     * Calculate the central point of the window.
     */
    Vector center = (i * perspective) + eye_;

    /*
     * Modify vectors (use ratio_ ?).
     */
    j.Scale_InPlace (0.5f);
    k.Scale_InPlace (0.5f);

    /*
     * Find three corners of the window.
     */
    wo_ = center + j + k;

    Vector ww = center - j + k;
    Vector wh = center + j - k;

    /*
     * Find vectors spanning the window.
     */
    wh_ = (ww - wo_) * (1.0f / (double) width);
    wv_ = (wh - wo_) * (1.0f / (double) height);
}

void Camera::CalculateRay (const unsigned int windowx, const unsigned int windowy, 
                           Vector *origin, Vector *direction) const {
    Vector horizontal, vertical;

    horizontal = wh_ * (double) windowx;
    vertical = wv_ * (double) windowy;

    (*origin) = wo_ + horizontal + vertical;
    (*direction) = (*origin) - eye_;
    direction->Normalize_InPlace ();
}
