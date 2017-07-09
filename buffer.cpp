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


Buffer::Buffer (unsigned int w,
        unsigned int h) {
    data   = NULL;
    width  = w;
    height = h;
}

bool Buffer::Allocate () {
    if (data == NULL) {
        size_t size;
        size = (size_t) (width * height);
        data = new Color [size];
        return true;
    }
    return false;
}

Buffer::~Buffer () {
    if (data != NULL) {
        /*
         * Deallocate data.
         */
        delete[] data;
    }
}

Color *Buffer::GetPointer () {
    return &data[0];
}

void Buffer::Clear () {
    unsigned int i;
    Color *bp;

    for (bp = &data[0], i = 0; 
        i < width * height; i++, bp++) {
            bp->Zero ();
    }
}

void Buffer::WriteToPNG (string filename) {
    image< rgb_pixel > image (width, height);
    rgb_pixel *pixel;
    unsigned char r, g, b;
    unsigned int i, j;
    Color *bp;

    bp = &data[0];
    for (i = 0; i < height; i++) {
        pixel = &image[i][0];
        for (j = 0; j < width; j++) {
            bp->GetBytes (&r, &g, &b);
            bp++;
            pixel->red   = r;
            pixel->green = g;
            pixel->blue  = b;
            pixel++;
        }
    }
    image.write (filename.c_str ());
}

void Buffer::Text (string text, unsigned int x,
        unsigned int y, Color *color) {
    unsigned char *fp, letter, idx, byte;
    char *tp;
    Color *bp, *sav;
    int row, col;

    tp = &text[0];
    bp = &data[x + y * width];

    while (*tp != 0) {
        letter = *(tp++);
        idx  = (letter - ' ') & 63;
        fp   = &fontdata[idx << 3];
        sav  = bp;

        for (row = 7; row >= 0; row--) {
            byte = *(fp++);
            for (col = 7; col >= 0; col--) {
                if ((byte >> col) & 1)
                    color->CopyTo (bp);
                bp++;
            }
            bp += (width - 8);
        }
        bp = sav + 8;
    }
}
