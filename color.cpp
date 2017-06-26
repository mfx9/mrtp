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


Color::~Color () {
}

Color::Color () {
    COLOR_ZERO ();
}

#ifdef FLOAT_COLOR
Color::Color (float r, float g, float b) {
    COLOR_TRIM (r, g, b);
    red   = r;
    green = g;
    blue  = b;
}
#else
Color::Color (unsigned char r, 
        unsigned char g, unsigned char b) {
    red   = r;
    green = g;
    blue  = b;
}
#endif

#ifdef FLOAT_COLOR
void Color::Set (float r, float g, float b) {
    COLOR_TRIM (r, g, b);
    red   = r;
    green = g;
    blue  = b;
}
#else
void Color::Set (unsigned char r, 
        unsigned char g, unsigned char b) {
    red   = r;
    green = g;
    blue  = b;
}
#endif

void Color::Zero () {
    COLOR_ZERO ();
}

void Color::CopyTo (Color *other) {
    other->red   = red;
    other->green = green;
    other->blue  = blue;
}

void Color::Convert (unsigned char *cr, 
        unsigned char *cg, unsigned char *cb) {
    #ifdef FLOAT_COLOR
    *cr = FLOAT_TO_BYTE (red);
    *cg = FLOAT_TO_BYTE (green);
    *cb = FLOAT_TO_BYTE (blue);
    #else
    *cr = red;
    *cg = green;
    *cb = blue;
    #endif
}

void Color::Scale_InPlace (double scale) {
    /*
     * Scale is between <0, 1>
     */
    if (scale < 0.)
        scale = 0.;
    else if (scale > 1.)
        scale = 1.;
    #ifdef FLOAT_COLOR
    red   = red   * (float) scale;
    green = green * (float) scale;
    blue  = blue  * (float) scale;
    #else
    red = FLOAT_TO_BYTE (BYTE_TO_FLOAT 
        (red) * (float) scale);
    green = FLOAT_TO_BYTE (BYTE_TO_FLOAT 
        (green) * (float) scale);
    blue = FLOAT_TO_BYTE (BYTE_TO_FLOAT 
        (blue) * (float) scale);
    #endif
}
