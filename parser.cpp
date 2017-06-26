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
        if      (labels[j] == LABEL_CAMERA_X   )  flags |= CAMERA_X;
        else if (labels[j] == LABEL_CAMERA_Y   )  flags |= CAMERA_Y;
        else if (labels[j] == LABEL_CAMERA_Z   )  flags |= CAMERA_Z;
        else if (labels[j] == LABEL_CAMERA_LX  )  flags |= CAMERA_LX;
        else if (labels[j] == LABEL_CAMERA_LY  )  flags |= CAMERA_LY;
        else if (labels[j] == LABEL_CAMERA_LZ  )  flags |= CAMERA_LZ;
        else if (labels[j] == LABEL_CAMERA_ROT )  flags |= CAMERA_ROT;
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
    return true;
}

bool Parser::CheckLight (string *labels, double *params, 
        unsigned int npairs) {
    unsigned int j, flags = 0x00;
    for (j = 0; j < npairs; j++) {
        if      (labels[j] == LABEL_LIGHT_X  ) flags |= LIGHT_X;
        else if (labels[j] == LABEL_LIGHT_Y  ) flags |= LIGHT_Y;
        else if (labels[j] == LABEL_LIGHT_Z  ) flags |= LIGHT_Z;
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
    return true;
}

bool Parser::CheckPlane (string *labels, double *params, 
        unsigned int npairs) {
    unsigned int j, flags = 0x00;
    for (j = 0; j < npairs; j++) {
        if      (labels[j] == LABEL_PLANE_X0  )  flags |= PLANE_X0;
        else if (labels[j] == LABEL_PLANE_Y0  )  flags |= PLANE_Y0;
        else if (labels[j] == LABEL_PLANE_Z0  )  flags |= PLANE_Z0;
        else if (labels[j] == LABEL_PLANE_A   )  flags |= PLANE_A;
        else if (labels[j] == LABEL_PLANE_B   )  flags |= PLANE_B;
        else if (labels[j] == LABEL_PLANE_C   )  flags |= PLANE_C;
        else if (labels[j] == LABEL_PLANE_SCALE   )  flags |= PLANE_SCALE;
        else if (labels[j] == LABEL_PLANE_COLA_R  )  flags |= PLANE_COLA_R;
        else if (labels[j] == LABEL_PLANE_COLA_G  )  flags |= PLANE_COLA_G;
        else if (labels[j] == LABEL_PLANE_COLA_B  )  flags |= PLANE_COLA_B;
        else if (labels[j] == LABEL_PLANE_COLB_R  )  flags |= PLANE_COLB_R;
        else if (labels[j] == LABEL_PLANE_COLB_G  )  flags |= PLANE_COLB_G;
        else if (labels[j] == LABEL_PLANE_COLB_B  )  flags |= PLANE_COLB_B;
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
    return true;
}

bool Parser::CheckSphere (string *labels, double *params, 
        unsigned int npairs) {
    unsigned int j, flags = 0x00;
    for (j = 0; j < npairs; j++) {
        if      (labels[j] == LABEL_SPHERE_X0  )  flags |= SPHERE_X0;
        else if (labels[j] == LABEL_SPHERE_Y0  )  flags |= SPHERE_Y0;
        else if (labels[j] == LABEL_SPHERE_Z0  )  flags |= SPHERE_Z0;
        else if (labels[j] == LABEL_SPHERE_R   )  flags |= SPHERE_R;
        else if (labels[j] == LABEL_SPHERE_COL_R )  flags |= SPHERE_COL_R;
        else if (labels[j] == LABEL_SPHERE_COL_G )  flags |= SPHERE_COL_G;
        else if (labels[j] == LABEL_SPHERE_COL_B )  flags |= SPHERE_COL_B;
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
    return true;
}

Parser::Parser (string fn) {
    status   = STATUS_NEW;
    filename = fn;
    nentries = 0;
}

Parser::~Parser () {
    if (nentries > 0) {
        /*
         * Remove all entries.
         *
         */
        do {
            PopEntry (NULL);
        } while (nentries > 0);
    }
}

int Parser::GetStatus () {
    return status;
}

unsigned int Parser::AddEntry (string title, string *lab, 
        double *par, unsigned int np) {
    Entry *entry, *next, *last;

    entry = new Entry (title, lab, par, np);
    if (nentries < 1) {
        entries = entry;
    }
    else {
        next = entries;
        do {
            last = next;
            next = last->GetNext ();
        } while (next != NULL);
        last->SetNext (entry);
    }
    return (++nentries);
}

unsigned int Parser::PopEntry (Entry *entry) {
    Entry *prev, *next, *last;

    if (nentries > 0) {
        last = entries;
        prev = NULL;
        while ((next = last->GetNext ()) != NULL) {
            prev = last;
            last = next;
        }
        if (prev != NULL)
            prev->SetNext (NULL);
        if (entry != NULL)
            last->CopyTo (entry);
        delete last;
        nentries--;
    }
    return nentries;
}

void Parser::Parse () {
    const char *fn = filename.c_str ();
    ifstream config (fn);

    string line, accu, prev, item;
    unsigned int i, j, nlines, len;
    char mode, c;

    string labels[MAX_PARM], values[MAX_PARM];
    unsigned int nlabel, nvalue;

    double test, params[MAX_PARM];
    stringstream convert ("test");

    unsigned int ncam, nlig;
    status = STATUS_FAIL;


    if (!config.is_open ()) {
        cout << "File " << filename << " cannot be opened." << endl;
        return;
    }
    mode   = MODE_OPEN;
    accu   = "";
    nlines = 0;
    ncam   = 0;
    nlig   = 0;

    while (getline (config, line)) {
        nlines++;
        i = 0;
        len = line.length ();
        while (i < len) {
            c = line.at (i++);
            if ((c == ' ') || (c == '\n') || (c == '\t'))
                continue;
            if (c == '#')
                break;
            accu += c;

            if (mode == MODE_OPEN) {
                if (c == '{') {
                    if ((prev == "camera") || (prev == "light") || (prev == "plane") || 
                            (prev == "sphere")) {
                        mode   = MODE_READ;
                        item   = prev;
                        nlabel = 0;
                        nvalue = 0;
                        accu   = "";
                    }
                    else {
                        cout << "Line " << nlines << ": Undefined item: \"" << prev << "\"" << endl;
                        config.close ();
                        return;
                    }
                }
            }
            else {  /* mode == MODE_READ */
                if (c == '}') {
                    values[nvalue++] = prev;
                    mode = MODE_OPEN;
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
                            cout << "Line " << nlines << ": Unable to convert \"" << values[j] << "\" to double." << endl;
                            config.close ();
                            return;
                        }
                        convert.clear ();
                        params[j++] = test;
                    }

                    if (item == "camera") {
                        if ((ncam++) > 1) {
                            cout << "Line " << nlines << ": Multiple camera entries." << endl;
                            config.close ();
                            return;
                        }
                        if (!CheckCamera (labels, params, nlabel)) {
                            config.close ();
                            return;
                        }
                    }
                    else if (item == "light") {
                        if ((nlig++) > 1) {
                            cout << "Line " << nlines << ": Multiple light entries." << endl;
                            config.close ();
                            return;
                        }
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
                    AddEntry (item, labels, params, nlabel);
                }
                else {
                    if (c == '=') {
                        labels[nlabel++] = prev;
                        accu = "";
                    }
                    else if (c == ';') {
                        values[nvalue++] = prev;
                        accu = "";
                    }
                    if ((nlabel == (MAX_PARM - 1)) || (nvalue == (MAX_PARM - 1))) {
                        cout << "Line " << nlines << ": Number of parameters exceeded." << endl;
                        config.close ();
                        return;
                    }
                }
            }
            prev = accu;
        }
    }
    config.close ();

    if (ncam < 1) {
        cout << "Camera not found." << endl;
        return;
    }
    if (nlig < 1) {
        cout << "Light not found." << endl;
        return;
    }
    status = STATUS_OK;
    cout << "Parsing complete, found " << nentries 
        << " entries." << endl;
}

Entry::Entry (string title, string *labels, double *params,
        unsigned int npairs) {
    unsigned int i;

    keys   = new string [npairs];
    values = new double [npairs];
    nitems = npairs;
    label  = title;
    for (i = 0; i < npairs; i++) {
        keys[i] = labels[i];
        values[i] = params[i];
    }
}

Entry::Entry () {
    nitems = 0;
}

Entry::~Entry () {
    if (nitems > 0) {
        delete[] keys;
        delete[] values;
    }
}

Entry *Entry::GetNext () {
    return next;
}

void Entry::SetNext (Entry *n) {
    next = n;
}

void Entry::CopyTo (Entry *other) {
    unsigned int i;

    if (other->nitems != nitems) {
        if (other->nitems > 0) {
            delete[] other->keys;
            delete[] other->values;
        }
        other->keys   = new string [nitems];
        other->values = new double [nitems];
        other->nitems = nitems;
    }
    other->label = label;
    other->next  = next;
    for (i = 0; i < nitems; i++) {
        other->keys[i] = keys[i];
        other->values[i] = values[i];
    }
}

bool Entry::GetPair (string *key, double *value,
        unsigned int *i) {
    /*
     * Before this method is called for the first time,
     *   i must be set to a high number (exceeding
     *   the actual number of pairs, for example 999).
     *
     * The method is supposed to be called iteratively
     *   in a while type of loop.
     *
     */
    unsigned int j;

    if (*i < 1)
        return false;
    if (*i > nitems) (*i) = nitems;
    j = (*i) - 1;
    (*key)   =   keys[j];
    (*value) = values[j];
    (*i)--;
    return true;
}

void Entry::GetLabel (string *title) {
    (*title) = label;
}
