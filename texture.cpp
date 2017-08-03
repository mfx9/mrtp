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
#include "texture.hpp"


Texture::Texture (string *filename) {
    /*
     * Constructor.
     */
    filename_ = (*filename);
    data_     =  NULL;
    width_    =  -1;
    height_   =  -1;
}

Texture::~Texture () {
    /*
     * Deallocate.
     */
    if (data_ != NULL) {
        delete[] data_;
    }
}

Color *Texture::GetPointer () {
    /*
     * Get a pointer to the beginning
     * of data.
     */
    return &data_[0];
}

bool Texture::Allocate () {
    /*
     * Allocate a texture and load data from 
     * a PNG file.
     *
     * No checking is done for whether the file
     * exists, etc.
     */
    const char *fn = (filename_).c_str ();
    image< rgb_pixel >  image (fn);

    width_  = image.get_width ();
    height_ = image.get_height ();

    size_t  size = (size_t) (width_ * height_);
    data_ = new Color [size];

    /*
     * Copy data file->buffer.
     */
    unsigned char  r, g, b;
    unsigned int   i, j;
    rgb_pixel     *pixel;
    Color         *color;

    color = &data_[0];
    for (i = 0; i < height_; i++) {
        pixel = &image[i][0];

        for (j = 0; j < width_; j++) {
            r = pixel->red;
            g = pixel->green;
            b = pixel->blue;
            pixel++;
            color->Set (r, g, b);
            color++;
        }
    }

    /*
     * Finalize.
     */
    return true;
}
