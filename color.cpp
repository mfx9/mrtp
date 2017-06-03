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
#include "color.hpp"


Color::Color () {
    red = 0.; green = 0; blue = 0;
}

Color::Color (float r, float g, float b) {
    red = r; green = g; blue = b;
}

/*
Color::Color (unsigned char r, 
        unsigned char g, unsigned char b) {
    red   = float (r) / 255.;
    green = float (g) / 255.;
    blue  = float (b) / 255.;
}
*/

Color::~Color () {
}

void Color::CopyTo (Color *other) {
    other->red   = red;
    other->green = green;
    other->blue  = blue;
}

void Color::Convert (unsigned char *cr, unsigned char *cg,
        unsigned char *cb) {
    *cr = (unsigned char) (red   * 255.);
    *cg = (unsigned char) (green * 255.);
    *cb = (unsigned char) (blue  * 255.);
}

void Color::Set (float r, float g, float b) {
    red = r; green = g; blue = b;
}

void Color::Scale_InPlace (double scale) {
    /*
     * . Scale can only be between <0; 1>
     */
    if ((scale >= 0.) && (scale <= 1.)) {
        red   *= (float) scale;
        green *= (float) scale;
        blue  *= (float) scale;
    } else {
        red   = 0.;
        green = 0.;
        blue  = 0.;
    }
}
