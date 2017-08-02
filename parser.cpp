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


Parser::Parser (string *filename) {
    status_   = STATUS_NEW;
    filename_ = (*filename);
    nentries_ = 0;
}

Parser::~Parser () {
    if (nentries_ > 0) {
        /*
         * Remove all entries.
         *
         */
        do {
            PopEntry (NULL);
        } while (nentries_ > 0);
    }
}

char Parser::GetStatus () {
    return status_;
}

unsigned int Parser::AddEntry (Entry *temp) {
    Entry *entry, *next, *last;

    entry  = new Entry ();
    temp->CopyTo (entry);

    if (nentries_ < 1) {
        entries_ = entry;
    }
    else {
        next = entries_;
        do {
            last = next;
            next = last->GetNext ();
        } while (next != NULL);
        last->SetNext (entry);
    }
    return (++nentries_);
}

unsigned int Parser::PopEntry (Entry *entry) {
    Entry *prev, *next, *last;

    if (nentries_ > 0) {
        last = entries_;
        prev = NULL;
        while ((next = last->GetNext ()) != NULL) {
            prev = last;
            last = next;
        }
        if (prev != NULL) {
            prev->SetNext (NULL);
        }
        if (entry != NULL) {
            last->CopyTo (entry);
        }
        delete last;
        nentries_--;
    }
    return nentries_;
}

bool Parser::TokenizeLine (string *line, string *tokens,
        unsigned int *ntokens) {
    /*
     * Split a line to tokens.
     *
     * Remove redundant blank characters and comments.
     */
    char    c, pc;
    string  accu;
    unsigned int i, ti, len;

    ti  = 0;
    len = line->length ();

    if (len > 0) {
        accu = "";
        pc   = line->at (0);

        for (i = 0; i < len; i++) {
            c = line->at (i);
            if IS_COMMENT (c) {
                break;
            }
            if IS_NOT_WHITE (c) {
                accu += c;
            }
            if ((IS_WHITE (c) && IS_NOT_WHITE (pc))
                    || (i == (len - 1))) {
                if (ti == MAX_TOKENS) {
                    return false;
                }
                tokens[ti++] = accu;
                accu = "";
            }
            pc = c;
        }
        if (accu != "") {
            tokens[ti++] = accu;
        }
    }
    (*ntokens) = ti;
    return true;
}

bool Parser::ConvertTokens (string *tokens, 
        unsigned int ntokens, double *out) {
    /*
     * Try to convert tokens to real numbers.
     */
    double        test;
    unsigned int  i;
    stringstream  convert;

    for (i = 0; i < ntokens; i++) {
        convert.str (tokens[i]);
        convert >> test;
        if (!convert) {
            return false;
        }
        out[i] = test;
        convert.clear ();
    }
    return true;
}

char Parser::CheckItem (string *item, string collect[][MAX_TOKENS],
        unsigned int sizes[], unsigned int npar, unsigned int *errline, 
        string *errmsg, Entry *entry) {
    unsigned int  i, j;
    unsigned int  ntokens, nlabels;

    unsigned char flags = 0,
        pattern = 0;
    bool    check, found;
    string  label;

    double output[MAX_COMPONENTS];


    const string *labels,
        camera[] = {"position", "target", "roll"},
        light[] = {"position"},
        plane[] = {"center", "normal", "cola", "colb", "scale"},  // , "texture"
        sphere[] = {"position", "radius", "color"},
        cylinder[] = {"a", "b", "radius", "color"};

    if ((*item) == "camera") {
        labels = camera;
        nlabels = sizeof (camera) / sizeof (camera[0]);
    }
    else if ((*item) == "light") {
        labels = light;
        nlabels = sizeof (light) / sizeof (light[0]);
    }
    else if ((*item) == "plane") {
        labels = plane;
        nlabels = sizeof (plane) / sizeof (plane[0]);
    }
    else if ((*item) == "sphere") {
        labels = sphere;
        nlabels = sizeof (sphere) / sizeof (sphere[0]);
    }
    else {  /* if ((*item) == "cylinder") */
        labels = cylinder;
        nlabels = sizeof (cylinder) / sizeof (cylinder[0]);
    }

    /*
     * Assign the label.
     */
    entry->SetLabel (item);

    /*
     * Build a pattern.
     */
    for (i = 0; i < nlabels; i++) {
        pattern |= (unsigned char) (1 << i);
    }

    for (i = 0; i < npar; i++) {
        label = collect[i][0];
        /* DEBUG
        cout << (*item) << ": " << label << endl;  */

        (*errline) = i;
        /*
         * Check if the label exists.
         */
        found = false;
        for (j = 0; j < nlabels; j++) {
            if (labels[j] == label) {
                found = true;
            }
        }
        if (!found) {
            (*errmsg) = label;
            return CODE_ALIEN;
        }

        /*
         * Parameters are usually 3D vectors (including 
         * colors).
         */
        ntokens = 4;
        /*
         * Parameters that are not vectors.
         */
        if ((label == "roll") || (label == "scale")
                || (label == "radius") || (label == "texture")) {
            ntokens = 2;
        }

        if (sizes[i] != ntokens) {
            return CODE_WRONG_SIZE;
        }
        /*
         * Parameters are real numbers, except textures.
         */
        if (label != "texture") {
            check = ConvertTokens (&collect[i][1], (ntokens - 1), output);
            if (!check) {
                return CODE_WRONG_TYPE;
            }
            entry->AddReal (&label, output, (ntokens - 1));
        }
        else {
            entry->AddText (&label, &collect[i][1], 1);
        }

        /*
         * Check if all parameter lines are present.
         */
        for (j = 0; j < nlabels; j++) {
            if (labels[j] == label) {
                /*
                 * Check for redundant parameters.
                 */
                if ((flags >> j) & 1) {
                    (*errmsg) = label;
                    return CODE_REDUNDANT;
                }
                flags |= (unsigned char) (1 << j);
                break;
            }
        }
    }
    if (flags != pattern) {
        return CODE_MISSING;
    }
    return CODE_OK;
}

void Parser::Parse () {
    const char *fn = filename_.c_str ();
    ifstream config (fn);

    string tokens[MAX_TOKENS];
    unsigned int ntokens;

    string collect[MAX_LINES][MAX_TOKENS];
    unsigned int npar, sizes[MAX_LINES];

    unsigned int i, nlines,
        errline, start,
        ncam, nlig, nact;

    bool check;
    char mode, code;

    string line, item, msg;
    Entry  entry;


    status_ = STATUS_FAIL;

    if (!config.is_open ()) {
        cout << "File \"" << filename_ 
            << "\" cannot be opened." << endl;
        return;
    }
    /*
     * Initialize.
     */
    mode   = MODE_OPEN;
    nlines = 0;
    ncam   = 0;
    nlig   = 0;
    nact   = 0;


    while (getline (config, line)) {
        nlines++;

        check = TokenizeLine (&line, tokens, &ntokens);
        if (!check) {
            cout << "Line " << nlines << 
                ": Too many tokens." << endl;
            config.close ();
            return;
        }
        /* DEBUG 
        cout << nlines << ", " << ntokens << ": ";
        for (i = 0; i < ntokens; i++) {
            cout << "\"" << tokens[i] << "\" ";
        }
        cout << endl; */

        if (ntokens > 0) {
            /*
             * Not a blank line.
             *
             */
            if (mode == MODE_OPEN) {
                item = tokens[0];
                if ((item == "camera") || (item == "light")
                        || (item == "plane") || (item == "sphere")
                        || (item == "cylinder")) {
                    mode  = MODE_READ;
                    npar  = 0;
                    start = nlines;

                    if (item == "camera") {
                        if ((++ncam) > 1) {
                            cout << "Line " << nlines 
                                << ": Multiple camera entries." << endl;
                            config.close ();
                            return;
                        }
                    }
                    else if (item == "light") {
                        if ((++nlig) > 1) {
                            cout << "Line " << nlines 
                                << ": Multiple light entries." << endl;
                            config.close ();
                            return;
                        }
                    }
                    else {
                        nact++;
                    }
                }
                else {
                    cout << "Line " << nlines << ": Unrecognized item \"" 
                        << item << "\"." << endl;
                    config.close ();
                    return;
                }
            }
            else {
                if (npar == MAX_LINES) {
                    cout << "Line " << nlines << 
                        ": Too many parameter lines." << endl;
                    config.close ();
                    return;
                }
                for (i = 0; i < ntokens; i++) {
                    collect[npar][i] = tokens[i];
                }
                sizes[npar++] = ntokens;
            }
        }
        else {
            /*
             * Blank line.
             *
             * If the number of tokens is zero, but the line
             * contains a comment, it is not an actual 
             * blank line.
             */
            if (line.length () != 0) {
                continue;
            }

            if (mode == MODE_READ) {
                mode = MODE_OPEN;

                code = CheckItem (&item, collect, sizes, npar, 
                    &errline, &msg, &entry);
            
                if (code != CODE_OK) {
                    if (code == CODE_ALIEN) {
                        cout << "Line " << (start + errline + 1) 
                            << ": Unrecognized parameter \"" << msg << "\"." << endl;
                    }
                    else if (code == CODE_WRONG_TYPE) {
                        cout << "Line " << (start + errline + 1) 
                            << ": Wrong type of component(s)." << endl;
                    }
                    else if (code == CODE_WRONG_SIZE) {
                        cout << "Line " << (start + errline + 1) 
                            << ": Wrong number of components." << endl;
                    }
                    else if (code == CODE_MISSING) {
                        cout << "Line " << start << ": Missing parameter in " 
                            << item << "." << endl;
                    }
                    else {  /* if (code == CODE_REDUNDANT) */
                        cout << "Line " << (start + errline + 1) 
                            << ": Redundant parameter \"" << msg << "\"." << endl;
                    }
                    config.close ();
                    return;
                }
                AddEntry (&entry);
                entry.Clear ();
            }
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
    if (nact < 1) {
        cout << "Scene contains no actors." << endl;
        return;
    }
    status_ = STATUS_OK;
    cout << "Parsing complete, created " << nentries_ 
        << " entries." << endl;

    /* DEBUG
    Entry *ep = entries_;
    if (nentries_ > 0) {
        do {
            ep->Print ();
            ep = ep->GetNext ();
        } while (ep != NULL);
    } */
}


/*****************************
 *          Entries          *
 *****************************/
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

Entry *Entry::GetNext () {
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
            if (type_[i] == TYPE_REAL) {
                other->real_[i][j] = real_[i][j];
            }
            else {  /* if (type[i] == TYPE_TEXT) */
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
                if (type_[i] == TYPE_TEXT) {
                    if (text_[i][j] != "") {
                        cout << "\"" << text_[i] << "\" ";
                    }
                    else {
                        cout << "\"\" ";
                    }
                }
                else {  /* if (type[i] == TYPE_REAL) */
                    cout << real_[i][j] << " ";
                }
            }
            cout << endl;
        }
    }
}

bool Entry::AddText (string *key, string *text, 
        unsigned int ntext) {
    unsigned int i;

    for (i = 0; i < ntext; i++) {
        text_[npar_][i] = text[i];
    }
    type_[npar_] = TYPE_TEXT;
    keys_[npar_] = (*key);
    npar_++;

    return true;
}

bool Entry::AddReal (string *key, double *real, 
        unsigned int nreal) {
    unsigned int i;

    for (i = 0; i < nreal; i++) {
        real_[npar_][i] = real[i];
    }
    type_[npar_] = TYPE_REAL;
    keys_[npar_] = (*key);
    npar_++;

    return true;
}

bool Entry::GetData (string *key, char *type, double *reals, 
        string *texts, unsigned int *i) {
    /*
     * Connect the parser with the actual initialization 
     * of actors.
     *
     * Before this method is called for the first time,
     * i must be set to MAX_LINES.
     *
     * The method is supposed to be called iteratively
     * in a while type of loop, until false is returned.
     *
     */
    unsigned int j, k;

    if ((*i) < 1) {
        return false;
    }
    if ((*i) > npar_) {
        (*i) = npar_;
    }
    j = (*i) - 1;

    if (type_[j] == TYPE_REAL) {
        for (k = 0; k < MAX_COMPONENTS; k++) {
            reals[k] = real_[j][k];
        }
    }
    else {  /* (type_[j] == TYPE_TEXT) */
        for (k = 0; k < MAX_COMPONENTS; k++) {
            texts[k] = text_[j][k];
        }
    }
    (*key)  = keys_[j];
    (*type) = type_[j];

    /* DEBUG
    cout << "Key: " << (*key) << endl;
    if ((*type) == TYPE_TEXT) {
        cout << "Type: TEXT " << endl;
    }
    else {
        cout << "Type: REAL " << endl;
    }
    for (j = 0; j < MAX_COMPONENTS; j++) {
        if ((*type) == TYPE_TEXT) {
            cout << texts[j] << " ";
        }
        else {
            cout << reals[j] << " ";
        }
    }
    cout << endl;
    DEBUG ENDS */

    (*i)--;
    return true;
}
