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


#define MAX_PARM     32

#define MODE_OPEN     1
#define MODE_READ     2

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

#define CYLINDER_AX      0x1
#define CYLINDER_AY      0x2
#define CYLINDER_AZ      0x4
#define CYLINDER_BX      0x8
#define CYLINDER_BY      0x10
#define CYLINDER_BZ      0x20
#define CYLINDER_R       0x40
#define CYLINDER_COL_R   0x80
#define CYLINDER_COL_G   0x100
#define CYLINDER_COL_B   0x200
#define CYLINDER_COMPLETE  (CYLINDER_AX | CYLINDER_AY | CYLINDER_AZ  \
    | CYLINDER_BX | CYLINDER_BY | CYLINDER_BZ  \
    | CYLINDER_R | CYLINDER_COL_R | CYLINDER_COL_G | CYLINDER_COL_B)


#define LABEL_CAMERA_X    "x"
#define LABEL_CAMERA_Y    "y"
#define LABEL_CAMERA_Z    "z"
#define LABEL_CAMERA_LX   "lx"
#define LABEL_CAMERA_LY   "ly"
#define LABEL_CAMERA_LZ   "lz"
#define LABEL_CAMERA_ROT  "rot"

#define LABEL_LIGHT_X  "x"
#define LABEL_LIGHT_Y  "y"
#define LABEL_LIGHT_Z  "z"

#define LABEL_PLANE_X0      "x0"
#define LABEL_PLANE_Y0      "y0"
#define LABEL_PLANE_Z0      "z0"
#define LABEL_PLANE_A       "A"
#define LABEL_PLANE_B       "B"
#define LABEL_PLANE_C       "C"
#define LABEL_PLANE_SCALE   "texscale"
#define LABEL_PLANE_COLA_R  "cola_r"
#define LABEL_PLANE_COLA_G  "cola_g"
#define LABEL_PLANE_COLA_B  "cola_b"
#define LABEL_PLANE_COLB_R  "colb_r"
#define LABEL_PLANE_COLB_G  "colb_g"
#define LABEL_PLANE_COLB_B  "colb_b"

#define LABEL_SPHERE_X0      "x0"
#define LABEL_SPHERE_Y0      "y0"
#define LABEL_SPHERE_Z0      "z0"
#define LABEL_SPHERE_R       "R"
#define LABEL_SPHERE_COL_R   "col_r"
#define LABEL_SPHERE_COL_G   "col_g"
#define LABEL_SPHERE_COL_B   "col_b"

#define LABEL_CYLINDER_AX      "ax"
#define LABEL_CYLINDER_AY      "ay"
#define LABEL_CYLINDER_AZ      "az"
#define LABEL_CYLINDER_BX      "bx"
#define LABEL_CYLINDER_BY      "by"
#define LABEL_CYLINDER_BZ      "bz"
#define LABEL_CYLINDER_R       "R"
#define LABEL_CYLINDER_COL_R   "col_r"
#define LABEL_CYLINDER_COL_G   "col_g"
#define LABEL_CYLINDER_COL_B   "col_b"


class Entry {
    string  label;
    string *keys;
    double *values;
    unsigned int nitems;
    Entry *next;
public:
    Entry (string title, string *labels,
        double *params, unsigned int npairs);
    Entry ();
    ~Entry ();
    Entry *GetNext ();
    void SetNext (Entry *n);
    void CopyTo (Entry *other);
    bool GetPair (string *key, double *value, 
        unsigned int *i);
    void GetLabel (string *title);
};

class Parser {
    string filename;
    int status;
    /*
     * Entries are a linked list.
     */
    Entry *entries;
    unsigned int nentries;

    bool CheckPlane (string *labels, double *params,
        unsigned int npairs);
    bool CheckSphere (string *labels, double *params,
        unsigned int npairs);
    bool CheckCylinder (string *labels, double *params,
        unsigned int npairs);
    bool CheckLight (string *labels, double *params,
        unsigned int npairs);
    bool CheckCamera (string *labels, double *params,
        unsigned int npairs);
    unsigned int AddEntry (string title, string *lab,
        double *par, unsigned int np);
public:
    Parser (string fn);
    ~Parser ();
    void Parse ();
    int GetStatus ();
    unsigned int PopEntry (Entry *entry);
};

#endif /* _PARSER_H */
