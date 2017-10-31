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
#include "color.hpp"


Color::~Color () {
}

Color::Color () {
}

Color::Color (float red, float green, 
        float blue) {
    /* TRIM_COLOR (red, green, blue); */

    red_   = red;
    green_ = green;
    blue_  = blue;
}

Color::Color (unsigned char red, 
        unsigned char green, unsigned char blue) {
    red_   = BYTE_TO_FLOAT (red);
    green_ = BYTE_TO_FLOAT (green);
    blue_  = BYTE_TO_FLOAT (blue);
}

void Color::Set (float red, float green, 
        float blue) {
    /* TRIM_COLOR (red, green, blue); */

    red_   = red;
    green_ = green;
    blue_  = blue;
}

void Color::Set (unsigned char red, 
        unsigned char green, unsigned char blue) {
    red_   = BYTE_TO_FLOAT (red);
    green_ = BYTE_TO_FLOAT (green);
    blue_  = BYTE_TO_FLOAT (blue);
}

void Color::Get (unsigned char *red, 
        unsigned char *green, unsigned char *blue) {
    *red   = FLOAT_TO_BYTE (red_);
    *green = FLOAT_TO_BYTE (green_);
    *blue  = FLOAT_TO_BYTE (blue_);
}

void Color::Get (float *red, float *green, 
        float *blue) {
    *red   = red_;
    *green = green_;
    *blue  = blue_;
}

void Color::CopyTo (Color *other) {
    other->red_   = red_;
    other->green_ = green_;
    other->blue_  = blue_;
}

void Color::Scale_InPlace (double scale) {
    TRIM_LIMITS (scale, 0.0f, 1.0f);

    red_   = red_   * (float) scale;
    green_ = green_ * (float) scale;
    blue_  = blue_  * (float) scale;
}

void Color::Zero () {
    red_   = 0.0f;
    green_ = 0.0f;
    blue_  = 0.0f;
}
