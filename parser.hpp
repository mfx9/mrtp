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
#include "color.hpp"

using namespace std;


#define MODE_SEARCH   1
#define MODE_OPEN     2
#define MODE_READ     3

#define MAX_PARM      16
#define DEBUG_PARSER  1

#define STATUS_FAIL   0
#define STATUS_NEW    1
#define STATUS_OK     2


class ParsedSphere {
    double x0, y0, z0;
    double R;
    Color  color;
    ParsedSphere *next;
public:
    ParsedSphere ();
    ~ParsedSphere ();
    void SetNext (ParsedSphere *n);
    ParsedSphere *GetNext ();
    bool Init (string *labels, double *params, 
        unsigned int npairs);
};

class ParsedPlane {
    double x0, y0, z0;
    double A, B, C;
    double scale;
    Color  colora, colorb;
    ParsedPlane *next;
public:
    ParsedPlane ();
    ~ParsedPlane ();
    void SetNext (ParsedPlane *n);
    ParsedPlane *GetNext ();
    bool Init (string *labels, double *params, 
        unsigned int npairs);
};

class ParsedLight {
    double x0, y0, z0;
public:
    ParsedLight (ParsedLight *temp);
    ParsedLight ();
    ~ParsedLight ();
    bool Init (string *labels, double *params, 
        unsigned int npairs);
};

class ParsedCamera {
    double x0, y0, z0;
    double lx, ly, lz;
    double rot;
public:
    ParsedCamera (ParsedCamera *temp);
    ParsedCamera ();
    ~ParsedCamera ();
    bool Init (string *labels, double *params, 
        unsigned int npairs);
};

class Parser {
    string filename;
    int status;
    ParsedLight   *light;
    ParsedCamera  *camera;
    ParsedPlane   *planeroot;
    ParsedSphere  *sphereroot;
public:
    Parser (string fn);
    ~Parser ();
    void Parse ();
    int GetStatus ();
};

#endif /* _PARSER_H */
