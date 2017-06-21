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
#ifndef _PARSER_H
#define _PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>  /* Converting strings to doubles. */

using namespace std;


#define MODE_SEARCH   1
#define MODE_OPEN     2
#define MODE_READ     3

#define MAX_PARM      16
#define DEBUG_PARSER  1

#define STATUS_FAIL   0
#define STATUS_NEW    1
#define STATUS_OK     2

/*
 *  1     1     0x1     0b1
 *  2     2     0x2     0b10
 *  3     4     0x4     0b100
 *  4     8     0x8     0b1000
 *  5     16    0x10    0b10000
 *  6     32    0x20    0b100000
 *  7     64    0x40    0b1000000
 *  8     128   0x80    0b10000000
 *  9     256   0x100   0b100000000
 *  10    512   0x200   0b1000000000
 *  11    1024  0x400   0b10000000000
 *  12    2048  0x800   0b100000000000
 *  13    4096  0x1000  0b1000000000000
 *
 */
#define CAMERA_X     0x1
#define CAMERA_Y     0x2
#define CAMERA_Z     0x4
#define CAMERA_LX    0x8
#define CAMERA_LY    0x10
#define CAMERA_LZ    0x20
#define CAMERA_ROT   0x40
#define CAMERA_COMPLETE  (CAMERA_X | CAMERA_Y | CAMERA_Z \
    | CAMERA_LX | CAMERA_LY | CAMERA_LZ | CAMERA_ROT)

#define LIGHT_X     0x1
#define LIGHT_Y     0x2
#define LIGHT_Z     0x4
#define LIGHT_COMPLETE  (LIGHT_X | LIGHT_Y | LIGHT_Z)

#define PLANE_X0      0x1
#define PLANE_Y0      0x2
#define PLANE_Z0      0x4
#define PLANE_A       0x8
#define PLANE_B       0x10
#define PLANE_C       0x20
#define PLANE_SCALE   0x40
#define PLANE_COLA_R  0x80
#define PLANE_COLA_G  0x100
#define PLANE_COLA_B  0x200
#define PLANE_COLB_R  0x400
#define PLANE_COLB_G  0x800
#define PLANE_COLB_B  0x1000
#define PLANE_COMPLETE  (PLANE_X0 | PLANE_Y0 | PLANE_Z0 |  \
    PLANE_A | PLANE_B | PLANE_C | PLANE_SCALE | PLANE_COLA_R |  \
    PLANE_COLA_G | PLANE_COLA_B | PLANE_COLB_R |  \
    PLANE_COLB_G | PLANE_COLB_B)

#define SPHERE_X0      0x1
#define SPHERE_Y0      0x2
#define SPHERE_Z0      0x4
#define SPHERE_R       0x8
#define SPHERE_COL_R   0x10
#define SPHERE_COL_G   0x20
#define SPHERE_COL_B   0x40
#define SPHERE_COMPLETE  (SPHERE_X0 | SPHERE_Y0 | SPHERE_Z0  \
    | SPHERE_R | SPHERE_COL_R | SPHERE_COL_G | SPHERE_COL_B)


class Entry {
    string *keys;
    double *values;
    unsigned int nitems;
    Entry *next;
public:
    Entry (string *labels, double *params,
        unsigned int npairs);
    ~Entry ();
    Entry *GetNext ();
    void SetNext (Entry *n);
    void CopyTo (Entry *other);
};

class Parser {
    string filename;
    int status;
    /*
     * Spheres and planes are one-directional
     *   lists.
     */
    Entry *spheres, *planes;
    Entry *camera, *light;

    bool CheckPlane (string *labels, double *params,
        unsigned int npairs);
    bool CheckSphere (string *labels, double *params,
        unsigned int npairs);
    bool CheckLight (string *labels, double *params,
        unsigned int npairs);
    bool CheckCamera (string *labels, double *params,
        unsigned int npairs);
public:
    Parser (string fn);
    ~Parser ();
    void Parse ();
    int GetStatus ();
    unsigned int PopEntry (Entry *entry);
};

#endif /* _PARSER_H */
