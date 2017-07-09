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
#include "camera.hpp"


Camera::Camera (Vector *origin, Vector *target,
        unsigned int w, unsigned int h, double fov, 
        double rot) {
    origin->CopyTo (&eye);
    target->CopyTo (&lookat);
    width  = (double) w;
    height = (double) h;
    ratio  = w / h;
    perspective = ratio / (2. * tan (fov / 2. * M_PI / 180.));
    rotation = rot;
}

Camera::~Camera () {
}

void Camera::GetDimensions (unsigned int *w, 
        unsigned int *h) {
    *w = width;
    *h = height;
}

void Camera::GetEye (Vector *vector) {
    eye.CopyTo (vector);
}

void Camera::CalculateVectors (Vector *vw, Vector *vh,
        Vector *vo) {
    /*
     * . Method calculates vectors that span the window.
     */

    /*
     * . i is the vector between the camera and
     *  the center of the window.
     */
    Vector i;
    i = lookat - eye;
    i.Normalize_InPlace ();
    /*
     * if (i.x == 0. && i.y == 0.)
     *     return false;
     */

    Vector j, k (0., 0., 1.);
    j = i ^ k;
    j.Normalize_InPlace ();
    /*
     * . Flip the cross-product instead?
     */
    j.Scale_InPlace (-1.);
    k = i ^ j;
    k.Normalize_InPlace ();
    /*
     * . Apply camera rotation around
     *  the axis of i.
     */

    /*
     * . Calculate the central point of
     *  the window.
     */
    Vector center;
    center = (i * perspective) + eye;
    /*
     * . Modify vectors.
     */
    j.Scale_InPlace (.5);     //ratio * .5
    k.Scale_InPlace (.5);

    /*
     * . Find 3 corners of the window.
     */
    Vector wo, ww, wh;
    wo = center + j + k;
    ww = center - j + k;
    wh = center + j - k;

    /*
     * . Find vectors spanning the window.
     */
    Vector horiz, verti;
    horiz = (ww - wo) * (1. / width );
    horiz.CopyTo (vw);
    verti = (wh - wo) * (1. / height);
    verti.CopyTo (vh);
    wo.CopyTo (vo);
}
