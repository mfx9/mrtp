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


bool Parser::CheckCamera (string *labels, double *params, 
        unsigned int npairs) {
    unsigned int j, flags = 0x00;
    for (j = 0; j < npairs; j++) {
        if      (labels[j] == "x"  ) flags |= CAMERA_X;
        else if (labels[j] == "y"  ) flags |= CAMERA_Y;
        else if (labels[j] == "z"  ) flags |= CAMERA_Z;
        else if (labels[j] == "lx" ) flags |= CAMERA_LX;
        else if (labels[j] == "ly" ) flags |= CAMERA_LY;
        else if (labels[j] == "lz" ) flags |= CAMERA_LZ;
        else if (labels[j] == "rot") flags |= CAMERA_ROT;
        else {
            cout << "Undefined parameter in camera: \"" << labels[j] 
                << "\"" << endl;
            return false;
        }
    }
    if (flags != CAMERA_COMPLETE) {
        cout << "Incomplete camera definition." << endl;
        return false;
    }
    #ifdef DEBUG_PARSER
    cout << "Completed camera." << endl;
    #endif
    return true;
}

bool Parser::CheckLight (string *labels, double *params, 
        unsigned int npairs) {
    unsigned int j, flags = 0x00;
    for (j = 0; j < npairs; j++) {
        if      (labels[j] == "x"  ) flags |= LIGHT_X;
        else if (labels[j] == "y"  ) flags |= LIGHT_Y;
        else if (labels[j] == "z"  ) flags |= LIGHT_Z;
        else {
            cout << "Undefined parameter in light: \"" << labels[j] 
                << "\"" << endl;
            return false;
        }
    }
    if (flags != LIGHT_COMPLETE) {
        cout << "Incomplete light definition." << endl;
        return false;
    }
    #ifdef DEBUG_PARSER
    cout << "Completed light." << endl;
    #endif
    return true;
}

bool Parser::CheckPlane (string *labels, double *params, 
        unsigned int npairs) {
    unsigned int j, flags = 0x00;
    for (j = 0; j < npairs; j++) {
        if      (labels[j] == "x0"  )  flags |= PLANE_X0;
        else if (labels[j] == "y0"  )  flags |= PLANE_Y0;
        else if (labels[j] == "z0"  )  flags |= PLANE_Z0;
        else if (labels[j] == "A"   )  flags |= PLANE_A;
        else if (labels[j] == "B"   )  flags |= PLANE_B;
        else if (labels[j] == "C"   )  flags |= PLANE_C;
        else if (labels[j] == "texscale")  flags |= PLANE_SCALE;
        else if (labels[j] == "cola_r"  )  flags |= PLANE_COLA_R;
        else if (labels[j] == "cola_g"  )  flags |= PLANE_COLA_G;
        else if (labels[j] == "cola_b"  )  flags |= PLANE_COLA_B;
        else if (labels[j] == "colb_r"  )  flags |= PLANE_COLB_R;
        else if (labels[j] == "colb_g"  )  flags |= PLANE_COLB_G;
        else if (labels[j] == "colb_b"  )  flags |= PLANE_COLB_B;
        else {
            cout << "Undefined parameter in plane: \"" << labels[j] 
                << "\"" << endl;
            return false;
        }
    }
    if (flags != PLANE_COMPLETE) {
        cout << "Incomplete plane definition." << endl;
        return false;
    }
    #ifdef DEBUG_PARSER
    cout << "Completed plane." << endl;
    #endif
    return true;
}

bool Parser::CheckSphere (string *labels, double *params, 
        unsigned int npairs) {
    unsigned int j, flags = 0x00;
    for (j = 0; j < npairs; j++) {
        if      (labels[j] == "x0"  ) flags |= SPHERE_X0;
        else if (labels[j] == "y0"  ) flags |= SPHERE_Y0;
        else if (labels[j] == "z0"  ) flags |= SPHERE_Z0;
        else if (labels[j] == "R"   ) flags |= SPHERE_R;
        else if (labels[j] == "col_r" )  flags |= SPHERE_COL_R;
        else if (labels[j] == "col_g" )  flags |= SPHERE_COL_G;
        else if (labels[j] == "col_b" )  flags |= SPHERE_COL_B;
        else {
            cout << "Undefined parameter in sphere: \"" << labels[j] 
                << "\"" << endl;
            return false;
        }
    }
    if (flags != SPHERE_COMPLETE) {
        cout << "Incomplete sphere definition." << endl;
        return false;
    }
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

    status = STATUS_FAIL;


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
                                if (!CheckCamera (labels, params, nlabel)) {
                                    config.close ();
                                    return;
                                }
                            }
                            else if (item == "light") {
                                if (!CheckLight (labels, params, nlabel)) {
                                    config.close ();
                                    return;
                                }
                            }
                            else if (item == "plane") {
                                if (!CheckPlane (labels, params, nlabel)) {
                                    config.close ();
                                    return;
                                }
                            }
                            else if (item == "sphere") {
                                if (!CheckSphere (labels, params, nlabel)) {
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
