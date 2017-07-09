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


Texture::Texture () {
    data   = NULL;
    width  = 0;
    height = 0;
}

Texture::~Texture () {
    if (data != NULL) {
        delete[] data;
    }
}

bool Texture::LoadFromPNG (string filename) {
    if (data != NULL) {
        return false;
    }
    /*
     * Open texture file.
     */
    const char *fn = filename.c_str ();
    image< rgb_pixel > image (fn);
    /*
     * Allocate texture buffer.
     */
    width  = image.get_width ();
    height = image.get_height ();
    data   = new Color [(size_t) (width * height)];
    /*
     * Copy data file->buffer.
     */
    unsigned char  r, g, b;
    unsigned int   i, j;
    rgb_pixel     *pixel;
    Color         *color;

    color = &data[0];
    for (i = 0; i < height; i++) {
        pixel = &image[i][0];

        for (j = 0; j < width; j++) {
            r = pixel->red;
            g = pixel->green;
            b = pixel->blue;
            pixel++;
            color->Set (r, g, b);
            color++;
        }
    }
    return true;
}

void Texture::Generate (Color *cola, Color *colb,
        unsigned int w, unsigned int h) {
}
