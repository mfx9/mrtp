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
#include "buffer.hpp"

using namespace std;
using namespace png;


Buffer::Buffer (unsigned int width, 
        unsigned int height) {
    data_   =  NULL;
    width_  =  width;
    height_ =  height;
}

void Buffer::Allocate () {
    data_ = new Color [(size_t) (width_ * height_)];
}

Buffer::~Buffer () {
    if (data_ != NULL) {
        delete[] data_;
    }
}

Color *Buffer::Pointer (unsigned int vline) {
    return &data_[(size_t) (vline * width_)];
}

void Buffer::Clear () {
    unsigned int i = 0;
    Color  *bp = &data_[0];

    for (; i < (width_ * height_); i++, bp++) {
        bp->Zero ();
    }
}

void Buffer::WriteToPNG (string *filename) {
    unsigned char red, green, blue;
    unsigned int  i, j;
    Color *bp;

    image< rgb_pixel > image (width_, height_);
    rgb_pixel  *pixel;

    bp = &data_[0];
    for (i = 0; i < height_; i++) {

        pixel = &image[i][0];
        for (j = 0; j < width_; j++, bp++, pixel++) {
            bp->Get (&red, &green, &blue);
            pixel->red   = red;
            pixel->green = green;
            pixel->blue  = blue;
        }
    }
    const char *fn = (*filename).c_str ();
    image.write (fn);
}
