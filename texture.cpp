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
    next_     =  NULL;
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

Color *Texture::GetColor (double fracx, double fracy, 
        double scale) {
    unsigned int u, v;
    /*
     * fracx, fracy are within a range of <0..1> and
     * define fractions of the x- and y-dimension 
     * of a texture.
     *
     * A reasonable scale for a 256x256 texture is 0.15.
     */
    u = ((unsigned int) (fracx * width_ * scale)) % width_;

    v = ((unsigned int) (fracy * height_ * scale)) % height_;

    return &data_[(size_t) (u + v * width_)];
}

Texture *Texture::GetNext () {
    return next_;
}

void Texture::SetNext (Texture *next) {
    next_ = next;
}

bool Texture::CheckFilename (string *filename) {
    return ((*filename) == filename_);
}

void Texture::Allocate () {
    /*
     * Load color data from a PDB file.
     *
     * No checking (for file, memory, etc.) 
     * is done.
     *
     */
    const char *fn = (filename_).c_str ();
    image< rgb_pixel >  image (fn);

    width_  = image.get_width ();
    height_ = image.get_height ();

    size_t size = (size_t) (width_ * height_);
    data_ = new Color [size];

    /*
     * Copy data file -> buffer.
     *
     */
    unsigned char  red, green, blue;
    unsigned int   i, j;
    rgb_pixel     *pixel;
    Color         *color;

    color = &data_[0];
    for (i = 0; i < height_; i++) {

        pixel = &image[i][0];
        for (j = 0; j < width_; j++, pixel++, color++) {
            red   = pixel->red;
            green = pixel->green;
            blue  = pixel->blue;
            color->Set (red, green, blue);
        }
    }
}
