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
#ifndef _COLOR_H
#define _COLOR_H

#define FLOAT_COLOR  1


#define FLOAT_TO_BYTE(c) (unsigned char) (c * 255.0)
#define BYTE_TO_FLOAT(c) ((float) c) * (1.0 / 255.0)

#define COLOR_TRIM(r, g, b) { if (r > 1.0) r = 1.0; \
    else if (r < 0.0) r = 0.0; \
        \
    if (g > 1.0) g = 1.0; \
    else if (g < 0.0) g = 0.0; \
        \
    if (b > 1.0) b = 1.0; \
    else if (b < 0.0) b = 0.0; }

#ifdef FLOAT_COLOR
#define COLOR_ZERO() { red = 0.0; green = 0.0; \
    blue = 0.0; }
#else
#define COLOR_ZERO() { red = 0; green = 0; \
    blue = 0; }
#endif


class Color {
    #ifdef FLOAT_COLOR
        float red, green, blue;
    #else
        unsigned char red, green,
            blue;
    #endif
public:
    Color ();
    ~Color ();
    /*
     * Set color from floats.
     */
    Color (float r, float g, 
        float b);
    void Set (float r, float g, 
        float b);
    /*
     * Set color from bytes.
     */
    Color (unsigned char r, unsigned char g, 
        unsigned char b);
    void Set (unsigned char r, unsigned char g, 
        unsigned char b);

    void Zero ();
    void CopyTo (Color *other);
    void GetBytes (unsigned char *cr, unsigned char *cg,
        unsigned char *cb);
    void GetFloats (float *fr, float *fg,
        float *fb);
    void Scale_InPlace (double scale);
};

#endif /* _COLOR_H */
