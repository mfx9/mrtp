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
#include "parser.hpp"


ParsedCamera::ParsedCamera () {
}

ParsedCamera::~ParsedCamera () {
}

ParsedCamera::ParsedCamera (ParsedCamera *temp) {
    x0  = temp->x0;
    y0  = temp->y0;
    z0  = temp->z0;
    lx  = temp->lx;
    ly  = temp->ly;
    lz  = temp->lz;
    rot = temp->rot;
}

bool ParsedCamera::Init (string *labels, double *params, 
        unsigned int npairs) {
    unsigned int j;
    for (j = 0; j < npairs; j++) {
        if      (labels[j] == "x"  ) x0  = params[j];
        else if (labels[j] == "y"  ) y0  = params[j];
        else if (labels[j] == "z"  ) z0  = params[j];
        else if (labels[j] == "lx" ) lx  = params[j];
        else if (labels[j] == "ly" ) ly  = params[j];
        else if (labels[j] == "lz" ) lz  = params[j];
        else if (labels[j] == "rot") rot = params[j];
        else {
            cout << "Undefined parameter in camera: \"" << labels[j] 
                << "\"" << endl;
            return false;
        }
    }
    #ifdef DEBUG_PARSER
    cout << "Completed camera." << endl;
    #endif
    return true;
}

ParsedLight::ParsedLight () {
}

ParsedLight::~ParsedLight () {
}

ParsedLight::ParsedLight (ParsedLight *temp) {
    x0  = temp->x0;
    y0  = temp->y0;
    z0  = temp->z0;
}

bool ParsedLight::Init (string *labels, double *params, 
        unsigned int npairs) {
    unsigned int j;
    for (j = 0; j < npairs; j++) {
        if      (labels[j] == "x"  ) x0  = params[j];
        else if (labels[j] == "y"  ) y0  = params[j];
        else if (labels[j] == "z"  ) z0  = params[j];
        else {
            cout << "Undefined parameter in light: \"" << labels[j] 
                << "\"" << endl;
            return false;
        }
    }
    #ifdef DEBUG_PARSER
    cout << "Completed light." << endl;
    #endif
    return true;
}

ParsedPlane::ParsedPlane () {
}

ParsedPlane::~ParsedPlane () {
}

void ParsedPlane::SetNext (ParsedPlane *n) {
    next = n;
}

ParsedPlane *ParsedPlane::GetNext () {
    return next;
}

bool ParsedPlane::Init (string *labels, double *params, 
        unsigned int npairs) {
    unsigned int j;
    float cola_r, cola_g, cola_b, colb_r, 
        colb_g, colb_b;

    for (j = 0; j < npairs; j++) {
        if      (labels[j] == "x0"  ) x0  = params[j];
        else if (labels[j] == "y0"  ) y0  = params[j];
        else if (labels[j] == "z0"  ) z0  = params[j];
        else if (labels[j] == "A"   ) A   = params[j];
        else if (labels[j] == "B"   ) B   = params[j];
        else if (labels[j] == "C"   ) C   = params[j];
        else if (labels[j] == "texscale") scale   = params[j];
        else if (labels[j] == "cola_r"  ) cola_r  = (float) params[j];
        else if (labels[j] == "cola_g"  ) cola_g  = (float) params[j];
        else if (labels[j] == "cola_b"  ) cola_b  = (float) params[j];
        else if (labels[j] == "colb_r"  ) colb_r  = (float) params[j];
        else if (labels[j] == "colb_g"  ) colb_g  = (float) params[j];
        else if (labels[j] == "colb_b"  ) colb_b  = (float) params[j];
        else {
            cout << "Undefined parameter in plane: \"" << labels[j] 
                << "\"" << endl;
            return false;
        }
    }
    colora.Set (cola_r, cola_g, cola_b);
    colorb.Set (colb_r, colb_g, colb_b);
    #ifdef DEBUG_PARSER
    cout << "Completed plane." << endl;
    #endif
    return true;
}

ParsedSphere::ParsedSphere () {
}

ParsedSphere::~ParsedSphere () {
}

void ParsedSphere::SetNext (ParsedSphere *n) {
    next = n;
}

ParsedSphere *ParsedSphere::GetNext () {
    return next;
}

bool ParsedSphere::Init (string *labels, double *params, 
        unsigned int npairs) {
    unsigned int j;
    float r, g, b;

    for (j = 0; j < npairs; j++) {
        if      (labels[j] == "x0"  ) x0  = params[j];
        else if (labels[j] == "y0"  ) y0  = params[j];
        else if (labels[j] == "z0"  ) z0  = params[j];
        else if (labels[j] == "R"   ) R   = params[j];
        else if (labels[j] == "col_r" )  r = (float) params[j];
        else if (labels[j] == "col_g" )  g = (float) params[j];
        else if (labels[j] == "col_b" )  b = (float) params[j];
        else {
            cout << "Undefined parameter in sphere: \"" << labels[j] 
                << "\"" << endl;
            return false;
        }
    }
    color.Set (r, g, b);
    #ifdef DEBUG_PARSER
    cout << "Completed sphere." << endl;
    #endif
    return true;
}

Parser::Parser (string fn) {
    filename = fn;
    status   = STATUS_NEW;
}

Parser::~Parser () {
    if (light != NULL) {
        delete light;
        #ifdef DEBUG_PARSER
        cout << "Freed parsed light." << endl;
        #endif
    }
    if (camera != NULL) {
        delete camera;
        #ifdef DEBUG_PARSER
        cout << "Freed parsed camera." << endl;
        #endif
    }
    if (planeroot != NULL) {
    }
    if (sphereroot != NULL) {
    }
}

int Parser::GetStatus () {
    return status;
}

void Parser::Parse () {
    const char *fn = filename.c_str ();
    ifstream config (fn);

    string line, accu, prev, item;
    unsigned int i, j, nlines;
    char mode, c;

    string labels[MAX_PARM], values[MAX_PARM];
    unsigned int nlabel, nvalue;

    double test, params[MAX_PARM];
    stringstream convert ("test");

    bool check;
    ParsedCamera  tempCamera;
    ParsedLight   tempLight;
    ParsedPlane   tempPlane;
    ParsedSphere  tempSphere;

    /*
     * Initialize.
     *
     * Assigning pointers to NULL causes segmentation 
     *   faults in the constructor (?).
     */
    light      = NULL;
    camera     = NULL;
    planeroot  = NULL;
    sphereroot = NULL;
    status     = STATUS_FAIL;

    if (config.is_open ()) {
        mode   = MODE_SEARCH;
        accu   = "";
        nlines = 0;
        while (getline (config, line)) {
            nlines++;
            i = 0;
            while (i < line.length ()) {
                c = line.at (i++);
                if ((c == ' ') || (c == '\n') || (c == '\t'))
                    continue;
                if (c == '#')
                    break;
                accu += c;

                switch (mode) {
                    case MODE_SEARCH:
                        if ((accu == "camera") || (accu == "light") || (accu == "plane") || 
                                (accu == "sphere") || (accu == "cylinder")) {
                            #ifdef DEBUG_PARSER
                            cout << "Reading " << accu << "..." << endl;
                            #endif
                            mode   = MODE_OPEN;
                            item   = accu;
                            nlabel = 0;
                            nvalue = 0;
                            accu   = "";
                        }
                        break;
                    case MODE_OPEN:
                        if (c == '(') {
                            mode = MODE_READ;
                            accu = "";
                        }
                        else {
                            cout << "Line " << nlines << ": Undefined characters." << endl;
                            config.close ();
                            return;
                        }
                        break;
                    case MODE_READ:
                        if (c == ')') {
                            values[nvalue++] = prev;
                            mode = MODE_SEARCH;
                            accu = "";
                            if (nlabel != nvalue) {
                                cout << "Line " << nlines << ": Numbers of labels and values do not match." << endl;
                                config.close ();
                                return;
                            }
                            j = 0;
                            while (j < nvalue) {
                                convert.str (values[j]);
                                convert >> test;
                                if (!convert) {
                                    cout << "Line " << nlines << ": Unable to convert " << values[j] << " to double." << endl;
                                    config.close ();
                                    return;
                                }
                                convert.clear ();
                                params[j++] = test;
                            }
                            #ifdef DEBUG_PARSER
                            for (j = 0; j < nlabel; j++) {
                                cout << labels[j] << "=" << values[j] << "(" << params[j] << ")" << endl;
                            }
                            cout << "Item is: " << item << endl;
                            #endif

                            if (item == "camera") {
                                check = tempCamera.Init (labels, params, nlabel);
                                if (!check) {
                                    config.close ();
                                    return;
                                }
                                camera = new ParsedCamera (&tempCamera);
                            }
                            else if (item == "light") {
                                check = tempLight.Init (labels, params, nlabel);
                                if (!check) {
                                    config.close ();
                                    return;
                                }
                                light = new ParsedLight (&tempLight);
                            }
                            else if (item == "plane") {
                                check = tempPlane.Init (labels, params, nlabel);
                                if (!check) {
                                    config.close ();
                                    return;
                                }
                            }
                            else if (item == "sphere") {
                                check = tempSphere.Init (labels, params, nlabel);
                                if (!check) {
                                    config.close ();
                                    return;
                                }
                            }
                            else if (item == "cylinder") {
                                /* Skip cylinders for now. */
                            }
                        }
                        else if (c == '=') {
                            labels[nlabel++] = prev;
                            accu = "";
                        }
                        else if (c == ';') {
                            values[nvalue++] = prev;
                            accu = "";
                        }
                        break;
                }
                prev = accu;
            }
        }
        config.close ();
    }
    else {
        cout << "File " << filename << " cannot be opened." << endl;
        return;
    }
    status = STATUS_OK;
}
