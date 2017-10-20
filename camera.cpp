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
#include "camera.hpp"


Camera::Camera (Vector *origin, Vector *target,
        unsigned int width, unsigned int height, double fov, 
        double roll) {
    /*
     * Constructor.
     *
     */
    origin->CopyTo (&eye_);
    target->CopyTo (&lookat_);

    width_       = (double) width;
    height_      = (double) height;
    rotation_    = DEG_TO_RAD (roll);
    ratio_       = width / height;
    perspective_ = ratio_ / (2.0 * tan (DEG_TO_RAD (fov / 2.0)));
}

Camera::~Camera () {
}

void Camera::GetDimensions (unsigned int *width, 
        unsigned int *height) {
    *width  = width_;
    *height = height_;
}

void Camera::GetEye (Vector *vector) {
    eye_.CopyTo (vector);
}

void Camera::CalculateVectors (Vector *vw, Vector *vh,
        Vector *vo) {
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
    Vector j, k (0.0, 0.0, 1.0);
    j = i ^ k;
    j.Normalize_InPlace ();

    /*
     * Flip the cross-product instead?
     */
    j.Scale_InPlace (-1.0);
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
    Vector center = (i * perspective_) + eye_;

    /*
     * Modify vectors (use ratio_ ?).
     */
    j.Scale_InPlace (0.5);
    k.Scale_InPlace (0.5);

    /*
     * Find three corners of the window.
     */
    Vector wo = center + j + k;
    Vector ww = center - j + k;
    Vector wh = center + j - k;

    /*
     * Find vectors spanning the window.
     */
    wo.CopyTo (vo);

    Vector horiz = (ww - wo) * (1.0 / width_ );
    horiz.CopyTo (vw);

    Vector verti = (wh - wo) * (1.0 / height_);
    verti.CopyTo (vh);
}
