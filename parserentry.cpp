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


Entry::Entry (string *label) {
    label_ = (*label);
    next_  = NULL;
    npar_  = 0;
}

Entry::Entry () {
    label_ = "default";
    next_  = NULL;
    npar_  = 0;
}

Entry::~Entry () {
}

void Entry::Clear () {
    npar_ = 0;
}

Entry *Entry::Next () {
    return next_;
}

void Entry::SetNext (Entry *next) {
    next_ = next;
}

void Entry::GetLabel (string *label) {
    (*label) = label_;
}

void Entry::SetLabel (string *label) {
    label_ = (*label);
}

void Entry::CopyTo (Entry *other) {
    unsigned int i, j;

    other->label_ = label_;
    other->npar_ = npar_;

    for (i = 0; i < npar_; i++) {
        other->keys_[i] = keys_[i];
        other->type_[i] = type_[i];

        for (j = 0; j < MAX_COMPONENTS; j++) {
            if (type_[i] == parameterReal) {
                other->real_[i][j] = real_[i][j];
            }
            else {  /* if (type[i] == parameterText) */
                other->text_[i][j] = text_[i][j];
            }
        }
    }
}

void Entry::Print () {
    unsigned int i, j;

    cout << "** Entry: " << label_ << endl;
    cout << "npar=" << npar_ << endl;

    if (npar_ > 0) {
        for (i = 0; i < npar_; i++) {
            cout << "Key " << keys_[i] << ": ";
        
            for (j = 0; j < MAX_COMPONENTS; j++) {
                if (type_[i] == parameterText) {
                    if (text_[i][j] != "") {
                        cout << "\"" << text_[i][j] << "\" ";
                    }
                    else {
                        cout << "\"\" ";
                    }
                }
                else {  /* if (type[i] == parameterReal) */
                    cout << real_[i][j] << " ";
                }
            }
            cout << endl;
        }
    }
}

bool Entry::AddText (const string *key, const string *text, 
        unsigned int ntext) {
    unsigned int i;

    for (i = 0; i < ntext; i++) {
        text_[npar_][i] = text[i];
    }
    type_[npar_] = parameterText;
    keys_[npar_] = (*key);
    npar_++;

    return true;
}

bool Entry::AddReal (const string *key, double *real, 
        unsigned int nreal) {
    unsigned int i;

    for (i = 0; i < nreal; i++) {
        real_[npar_][i] = real[i];
    }
    type_[npar_] = parameterReal;
    keys_[npar_] = (*key);
    npar_++;

    return true;
}

bool Entry::PopData (string *key, ParserParameter_t *type, double *reals, 
        string *texts) {
    /*
     * Connect the parser with the actual initialization 
     * of actors.
     *
     * The method is supposed to be called iteratively
     * in a while type of loop, until false is returned.
     *
     */
    unsigned int j, k;

    if (npar_ < 1) {
        return false;
    }
    j = (npar_ - 1);

    if (type_[j] == parameterReal) {
        for (k = 0; k < MAX_COMPONENTS; k++) {
            reals[k] = real_[j][k];
        }
    }
    else {  /* (type_[j] == parameterText) */
        for (k = 0; k < MAX_COMPONENTS; k++) {
            texts[k] = text_[j][k];
        }
    }
    (*key)  = keys_[j];
    (*type) = type_[j];

    /* DEBUG
    cout << "Key: " << (*key) << endl;
    if ((*type) == parameterText) {
        cout << "Type: TEXT " << endl;
    }
    else {
        cout << "Type: REAL " << endl;
    }
    for (j = 0; j < MAX_COMPONENTS; j++) {
        if ((*type) == parameterText) {
            cout << texts[j] << " ";
        }
        else {
            cout << reals[j] << " ";
        }
    }
    cout << endl;
    DEBUG ENDS */

    npar_--;
    return true;
}
