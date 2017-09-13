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

/*
 * Macros.
 */
#define FLOAT_TO_BYTE(color) (unsigned char) (color * 255.0f)
#define BYTE_TO_FLOAT(color) ((float) color) / 255.0f

#define TRIM_LIMITS(x, min, max) { \
   if (x < min) { \
       x = min; \
   } \
   else if (x > max) { \
       x = max; \
   } }

#define TRIM_COLOR(red, green, blue) { \
    TRIM_LIMITS (red,   0.0f, 1.0f); \
    TRIM_LIMITS (green, 0.0f, 1.0f); \
    TRIM_LIMITS (blue,  0.0f, 1.0f); \
        }


class Color {
    float red_, green_, blue_;

public:
    ~Color ();
    Color ();
    Color (unsigned char red, unsigned char green, unsigned char blue);
    Color (float red, float green, float blue);

    void Set (float red, float green, float blue);
    void Set (unsigned char red, unsigned char green, unsigned char blue);
    void Get (float *red, float *green, float *blue);
    void Get (unsigned char *red, unsigned char *green, unsigned char *blue);
    void CopyTo (Color *other);
    void Scale_InPlace (double scale);
    void Zero ();
};

#endif /* _COLOR_H */
