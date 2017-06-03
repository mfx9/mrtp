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


/* #define CONVERT_COLOR(c) { (unsigned char) (c * 255.) } */

class Color {
    float red, green, blue;
public:
    Color ();
    Color (float r, float g, float b);
    /* Color (unsigned char r, unsigned char g, 
        unsigned char b); */
    ~Color ();
    void Set (float r, float g, float b);
    void CopyTo (Color *other);
    void Convert (unsigned char *cr, unsigned char *cg,
        unsigned char *cb);
    void Scale_InPlace (double scale);
};

#endif /* _COLOR_H */
