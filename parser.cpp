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

ParserStatus_t Parser::Status () {
    return status_;
}

unsigned int Parser::GetNumberEntries () {
    return nentries_;
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

ParserCode_t Parser::CreateEntry (string *id, string collect[][MAX_TOKENS],
        unsigned int sizes[], unsigned int ncol, unsigned int *errline, 
        string *errmsg, Entry *entry) {
    bool          check, found;
    string        label, filename, extension;
    double        output[MAX_COMPONENTS];
    unsigned int  i, j, k, ntokens, checklist;

    const TemplateParameter *templ, *othertempl;
    const TemplateItem      *item;

    /*
     * Assign the label (camera, light, etc.)
     */
    entry->SetLabel (id);

    /*
     * Find a template for the current item.
     */
    item = kItems;
    for (i = 0; i < kSizeItems; i++, item++) {
        if (item->id == (*id)) {
            break;
        }
    }

    /*
     * Run over all collectibles.
     */
    checklist = 0;
    for (i = 0; i < ncol; i++) {
        label = collect[i][0];

        (*errline) = i;
        /*
         * Find a template for the current collectible.
         */
        found = false;
        templ = item->templ;
        for (j = 0; j < item->ntempl; j++, templ++) {
            if (templ->label == label) {
                found = true;
                break;
            }
        }
        if (!found) {
            (*errmsg) = label;
            return codeUnknown;
        }
        /*
         * Check if the parameter has been processed.
         *
         * Otherwise, mark it as processed.
         */
        if (CHECK_BIT (checklist, j)) {
            (*errmsg) = label;
            return codeRedundant;
        }
        checklist |= MAKE_MASK (j);

        /*
         * Check for conflicting parameters.
         */
        othertempl = item->templ;
        for (k = 0; k < item->ntempl; k++, othertempl++) {
            if (j != k) {
                if (templ->replace == othertempl->id) {
                    if (CHECK_BIT (checklist, k)) {
                        return codeConflict;
                    }
                }
            }
        }

        /*
         * Parameters are usually 3D vectors (including colors).
         */
        ntokens = 2;
        if (CHECK_BIT (templ->flags, TP_VECTOR)) {
            ntokens = 4;
        }
        if (sizes[i] != ntokens) {
            return codeSize;
        }

        if (!CHECK_BIT (templ->flags, TP_TEXT)) {
            /*
             * Parameter is a vector or real number.
             */
            check = ConvertTokens (&collect[i][1], (ntokens - 1), output);
            if (!check) {
                return codeType;
            }
            /*
             * Check for invalid values.
             */
            if (CHECK_BIT (templ->flags, TP_CHECK_ZERO)) {
                check = false;
                for (j = 0; j < (ntokens - 1); j++) {
                    if (output[j] != 0.0) {
                        check = true;
                        break;
                    }
                }
                if (!check) {
                    return codeValue;
                }
            }
            else if (CHECK_BIT (templ->flags, TP_CHECK_POSITIVE)) {
                check = false;
                for (j = 0; j < (ntokens - 1); j++) {
                    if (output[j] > 0.0) {
                        check = true;
                        break;
                    }
                }
                if (!check) {
                    return codeValue;
                }
            }
            entry->AddReal (&label, output, (ntokens - 1));
        }
        else {
            /*
             * Parameter is a texture.
             */
            extension = "png";
            check = CheckFilename (&collect[i][1], &filename, &extension);
            if (!check) {
                return codeFilename;
            }
            entry->AddText (&label, &filename, 1);
        }
    }


    /*
     * Check if all parameters or their alternatives are present.
     */
    for (i = 0; i < ncol; i++) {
        label = collect[i][0];

        templ = item->templ;
        for (j = 0; j < item->ntempl; j++, templ++) {
            if (templ->label == label) {
                break;
            }
        }
        if (!CHECK_BIT (checklist, j)) {
            /*
             * Parameter is not present.
             *
             * Check if it is replaceable.
             */
            if (!templ->replace) {
                return codeMissing;
            }
            /*
             * Parameter is not present, but it is replaceable.
             *
             * Check if the alternative parameter is present.
             */
            othertempl = item->templ;
            for (k = 0; k < item->ntempl; k++, othertempl++) {
                if (j != k) {
                    if (templ->replace == othertempl->id) {
                        break;
                    }
                }
            }
            if (!CHECK_BIT (checklist, k)) {
                /*
                 * Alternative parameter is needed, but it is also missing.
                 */
                return codeMissing;
            }
        }
    }
    return codeOK;
}

void Parser::Parse () {
    string tokens[MAX_TOKENS];
    unsigned int ntokens;

    string collect[MAX_LINES][MAX_TOKENS];
    unsigned int npar, sizes[MAX_LINES];

    unsigned int i, nlines,
        errline, start,
        ncam, nlig, nact;

    bool check;
    ParserMode_t mode;
    ParserCode_t code;

    string line, item, msg;
    Entry  entry;

    /*
     * Initialize.
     */
    status_ = statusFail;
    mode    = modeOpen;
    nlines  = 0;
    ncam    = 0;
    nlig    = 0;
    nact    = 0;

    /*
     * Open the input file.
     */
    const char *fn = filename_.c_str ();
    ifstream config (fn);

    if (!config.is_open ()) {
        cerr << "File \"" << filename_ 
            << "\" cannot be opened." << endl;
        return;
    }


    while (getline (config, line)) {
        nlines++;

        check = TokenizeLine (&line, tokens, &ntokens, MAX_TOKENS);
        if (!check) {
            cerr << "Line " << nlines << 
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
            if (mode == modeOpen) {
                item = tokens[0];
                if ((item == "camera") || (item == "light")
                        || (item == "plane") || (item == "sphere")
                        || (item == "cylinder")) {
                    mode  = modeRead;
                    npar  = 0;
                    start = nlines;

                    if (item == "camera") {
                        if ((++ncam) > 1) {
                            cerr << "Line " << nlines 
                                << ": Multiple camera entries." << endl;
                            config.close ();
                            return;
                        }
                    }
                    else if (item == "light") {
                        if ((++nlig) > 1) {
                            cerr << "Line " << nlines 
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
                    cerr << "Line " << nlines << ": Unrecognized item \"" 
                        << item << "\"." << endl;
                    config.close ();
                    return;
                }
            }
            else {
                if (npar == MAX_LINES) {
                    cerr << "Line " << nlines << 
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

            if (mode == modeRead) {
                mode = modeOpen;

                code = CreateEntry (&item, collect, sizes, npar, &errline, 
                    &msg, &entry);
            
                if (code != codeOK) {
                    if (code == codeUnknown) {
                        cerr << "Line " << (start + errline + 1) 
                            << ": Unrecognized parameter \"" << msg << "\"." << endl;
                    }
                    else if (code == codeType) {
                        cerr << "Line " << (start + errline + 1) 
                            << ": Wrong type of component(s)." << endl;
                    }
                    else if (code == codeSize) {
                        cerr << "Line " << (start + errline + 1) 
                            << ": Wrong number of components." << endl;
                    }
                    else if (code == codeMissing) {
                        cerr << "Line " << start << ": Missing parameter in " 
                            << item << "." << endl;
                    }
                    else if (code == codeRedundant) {
                        cerr << "Line " << (start + errline + 1) 
                            << ": Redundant parameter \"" << msg << "\"." << endl;
                    }
                    else if (code == codeFilename) {
                        cerr << "Line " << (start + errline + 1) 
                            << ": File not found or invalid filename." << endl;
                    }
                    else if (code == codeValue) {
                        cerr << "Line " << (start + errline + 1) <<
                            ": Invalid value(s)." << endl;
                    }
                    else {  /* if (code == codeConflict) */
                        cerr << "Line " << (start + errline + 1) <<
                            ": Conflicting parameter." << endl;
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
        cerr << "Camera not found." << endl;
        return;
    }
    if (nlig < 1) {
        cerr << "Light not found." << endl;
        return;
    }
    if (nact < 1) {
        cerr << "Scene contains no actors." << endl;
        return;
    }
    status_ = statusOK;

    /* DEBUG
    Entry *ep = entries_;
    if (nentries_ > 0) {
        do {
            ep->Print ();
            ep = ep->GetNext ();
        } while (ep != NULL);
    } */
}
