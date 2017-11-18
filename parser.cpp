/*
 *  Mrtp: A simple raytracing tool.
 *  Copyright (C) 2017  Mikolaj Feliks <mikolaj.feliks@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "parser.hpp"

using namespace std;


/**** Tables. ****/

struct MotifParameter {
    char       id;
    char       replace;
    string     label;
    string     defaults;
    Bitmask_t  flags;
};

struct MotifActor {
    string        id;
    bool          mandatory;
    unsigned int  nallowed;
    unsigned int  nmotif;

    const MotifParameter *motif;
};

struct MotifMessage {
    ParserCode_t  code;
    string        message;
};

const MotifParameter kCamera[] = {
    { 1,  0,  "position",  "", BIT_VECTOR },
    { 2,  0,  "target",  "", BIT_VECTOR },
    { 3,  0,  "roll",  "0.0", BIT_REAL | BIT_OPTIONAL },
    };

const MotifParameter kLight[] = {
    { 1,  0,  "position",  "", BIT_VECTOR},
    };

const MotifParameter kPlane[] = {
    { 1,  0,  "center",  "", BIT_VECTOR },
    { 2,  0,  "normal",  "", BIT_VECTOR | BIT_CHECK_ZERO },
    { 3,  0,  "scale",  "", BIT_REAL | BIT_CHECK_POSITIVE },
    { 4,  0,  "reflect",  "0.0", BIT_REAL | BIT_CHECK_POSITIVE | BIT_OPTIONAL },
    { 5,  6,  "color",  "", BIT_VECTOR },
    { 6,  5,  "texture",  "", BIT_TEXT },
    };

const MotifParameter kSphere[] = {
    { 1,  0, "position",  "", BIT_VECTOR },
    { 2,  0, "radius",  "", BIT_REAL | BIT_CHECK_POSITIVE },
    { 3,  0, "axis",  "0.0  0.0  1.0", BIT_VECTOR | BIT_CHECK_ZERO | BIT_OPTIONAL },
    { 4,  0, "reflect",  "0.0", BIT_REAL | BIT_CHECK_ZERO_ONE | BIT_OPTIONAL },
    { 5,  6, "color",  "", BIT_VECTOR },
    { 6,  5, "texture",  "", BIT_TEXT },
    };

const MotifParameter kCylinder[] = {
    { 1,  0, "center",  "", BIT_VECTOR },
    { 2,  0, "direction",  "", BIT_VECTOR | BIT_CHECK_ZERO },
    { 3,  0, "radius",  "", BIT_REAL | BIT_CHECK_POSITIVE },
    { 4,  0, "span",  "-1.0", BIT_REAL | BIT_CHECK_ZERO | BIT_OPTIONAL },
    { 5,  0, "reflect",  "0.0", BIT_REAL | BIT_CHECK_ZERO_ONE | BIT_OPTIONAL },
    { 6,  7, "color",  "", BIT_VECTOR },
    { 7,  6, "texture",  "", BIT_TEXT },
    };

const MotifActor kActors[] = {
    {"camera", true, 1, (unsigned int) (sizeof (kCamera) / sizeof (kCamera[0])), kCamera },

    {"light", true, 1, (unsigned int) (sizeof (kLight) / sizeof (kLight[0])), kLight },

    {"plane", false, 0, (unsigned int) (sizeof (kPlane) / sizeof (kPlane[0])), kPlane },

    {"sphere", false, 0, (unsigned int) (sizeof (kSphere) / sizeof (kSphere[0])), kSphere },

    {"cylinder", false, 0, (unsigned int) (sizeof (kCylinder) / sizeof (kCylinder[0])), kCylinder },
    };

const unsigned int kSizeActors =
        (unsigned int) sizeof (kActors) / sizeof (kActors[0]);

const MotifMessage kErrorMessages[] = {
    { codeUnknown, "Unrecognized parameter." }, 
    { codeType, "Wrong type of component(s)." }, 
    { codeSize, "Wrong number of components." },
    { codeMissing, "Missing parameter." }, 
    { codeRepeated, "Repeated parameter." }, 
    { codeFilename, "File not found or invalid filename." }, 
    { codeValue, "Invalid value(s)." }, 
    { codeConflict, "Conflicting parameters." } ,
};

const unsigned int kSizeMessages =
        (unsigned int) sizeof (kErrorMessages) / sizeof (kErrorMessages[0]);


/**** Local functions. ****/

static bool TokenizeLine (const string *line, string *tokens,
                          unsigned int *ntokens, unsigned int maxtokens);
static bool ConvertTokens (const string *tokens, unsigned int ntokens, 
                           double *out);
static bool CheckFilename (string *in, string *ext, string *out);


/**** Parser. ****/

Parser::Parser (string *path) {
    path_  = *path;
    nentries_ = 0;
}

Parser::~Parser () {
    if (nentries_ > 0) {
        /* Destroy all entries. */

        list<Entry *>::iterator  current, last;
        Entry  *entry;

        current = entries_.begin ();
        last = entries_.end ();

        do {
            entry = *current;
            delete entry;
            nentries_--;
        } while ((++current) != last);
    }
}

ParserStatus_t Parser::Check (unsigned int *nentries) {
    *nentries = nentries_;

    return status_;
}

void Parser::StartQuery () {
    current_ = 0;
}

bool Parser::Query (Entry **entry) {
    if (current_ == nentries_) {
        return false;
    }

    list<Entry *>::iterator  item;
    unsigned int i;

    item = entries_.begin ();
    for (i = 0; i < current_; i++) {
        item++;
    }
    *entry = *item;
    current_++;
    return true;
}

ParserCode_t Parser::CreateEntry (string *id, string collect[][MAX_TOKENS],
                                  unsigned int sizes[], unsigned int ncol, 
                                  Entry *entry) {
    bool          check, found;
    string        label, filename, extension;
    double        output[MAX_COMPONENTS];
    unsigned int  i, j, k, ntokens;
    Bitmask_t     checklist;

    const MotifParameter *motif, *othermotif;
    const MotifActor      *item;

    string  tokens[MAX_COMPONENTS];
    /*
     * Assign the label (camera, light, etc.)
     */
    entry->SetLabel (id);

    /*
     * Find a motifate for the current item.
     */
    item = kActors;
    for (i = 0; i < kSizeActors; i++, item++) {
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
        /*
         * Find a motifate for the current collectible.
         */
        found = false;
        motif = item->motif;
        for (j = 0; j < item->nmotif; j++, motif++) {
            if (motif->label == label) {
                found = true;
                break;
            }
        }
        if (!found) {
            return codeUnknown;
        }
        /*
         * Check if the parameter has been processed.
         *
         * Otherwise, mark it as processed.
         */
        if (CHECK_BIT (checklist, j)) {
            return codeRepeated;
        }
        checklist |= MAKE_MASK (j);

        /*
         * Check for conflicting parameters.
         */
        othermotif = item->motif;
        for (k = 0; k < item->nmotif; k++, othermotif++) {
            if (j != k) {
                if (motif->replace == othermotif->id) {
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
        if (CHECK_BIT (motif->flags, flagVector)) {
            ntokens = 4;
        }
        if (sizes[i] != ntokens) {
            return codeSize;
        }

        if (!CHECK_BIT (motif->flags, flagText)) {
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
            if (CHECK_BIT (motif->flags, flagCheckZero)) {
                check = false;
                for (j = 0; j < (ntokens - 1); j++) {
                    if (output[j] != 0.0f) {
                        check = true;
                        break;
                    }
                }
                if (!check) {
                    return codeValue;
                }
            }
            else if (CHECK_BIT (motif->flags, flagCheckPositive)) {
                check = false;
                for (j = 0; j < (ntokens - 1); j++) {
                    if (output[j] > 0.0f) {
                        check = true;
                        break;
                    }
                }
                if (!check) {
                    return codeValue;
                }
            }
            else if (CHECK_BIT (motif->flags, flagCheckZeroOne)) {
                check = false;
                for (j = 0; j < (ntokens - 1); j++) {
                    if ((output[j] >= 0.0f) && (output[j] <= 1.0f)) {
                        check = true;
                        break;
                    }
                }
                if (!check) {
                    return codeValue;
                }
            }
            entry->AddNumerical (&label, output, (ntokens - 1));
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
            entry->AddTextual (&label, &filename, 1);
        }
    }

    /*
     * Check if all parameters or their alternatives are present.
     */
    motif = item->motif;
    for (i = 0; i < item->nmotif; i++, motif++) {
        if (!CHECK_BIT (checklist, i)) {
            /*
             * Parameter is not present.
             */
            if (CHECK_BIT (motif->flags, flagOptional)) {
                /*
                 * Parameter is optional, load defaults.
                 */
                TokenizeLine (&motif->defaults, tokens, &ntokens, MAX_COMPONENTS);

                if (!CHECK_BIT (motif->flags, flagText)) {
                    ConvertTokens (tokens, ntokens, output);
                    entry->AddNumerical (&motif->label, output, ntokens);
                }
                else {
                    entry->AddTextual (&motif->label, tokens, ntokens);
                }
            }
            else {
                /*
                 * Parameter is not optional, check if it is replaceable.
                 */
                if (motif->replace) {
                    /*
                     * Check if the alternative parameter is present.
                     */
                    othermotif = item->motif;
                    for (j = 0; j < item->nmotif; j++, othermotif++) {
                        if (i != j) {
                            if (motif->replace == othermotif->id) {
                                break;
                            }
                        }
                    }
                    if (!CHECK_BIT (checklist, j)) {
                        /*
                         * Alternative parameter is needed, but it is also missing.
                         */
                        return codeMissing;
                    }
                }
                else {
                    /*
                     * Parameter is not present, not optional and not replaceable.
                     */
                    return codeMissing;
                }
            }
        }
    }
    return codeOK;
}

void Parser::Parse () {
    string        tokens[MAX_TOKENS];
    string        collect[MAX_LINES][MAX_TOKENS];
    string        line, item;

    unsigned int  i, nlines, start;
    unsigned int  npar, sizes[MAX_LINES];
    unsigned int  ntokens;
    unsigned int  counters[kSizeActors], nallowed, total;

    ParserMode_t  mode;
    ParserCode_t  code;
    bool          check;
    Entry        *entry;

    /*
     * Initialize.
     */
    for (i = 0; i < kSizeActors; i++) {
        counters[i] = 0;
    }
    status_ = statusFail;
    mode    = modeOpen;
    nlines  = 0;

    /*
     * Open the input file.
     */
    const char *fn = path_.c_str ();
    ifstream config (fn);

    if (!config.is_open ()) {
        cerr << "File \"" << path_ << "\" cannot be opened." << endl;
        return;
    }


    while (getline (config, line)) {
        nlines++;

        check = TokenizeLine (&line, tokens, &ntokens, MAX_TOKENS);
        if (!check) {
            cerr << "Line " << nlines << ": Too many tokens." << endl;
            config.close ();
            return;
        }

        if (ntokens > 0) {
             /* Not a blank line. */

            if (mode == modeOpen) {
                item = tokens[0];

                check = false;
                for (i = 0; i < kSizeActors; i++) {
                    if (kActors[i].id == item) {
                        check = true;
                        npar  = 0;
                        start = nlines;
                        mode  = modeRead;

                        nallowed = kActors[i].nallowed;
                        counters[i]++;

                        if (nallowed && (counters[i] > nallowed)) {
                            cerr << "Line " << nlines << 
                                ": Too many entries of " << item << "." << endl;
                            config.close ();
                            return;
                        }
                        break;
                    }
                }
                if (!check) {
                    cerr << "Line " << nlines << ": Unrecognized item \"" 
                        << item << "\"." << endl;
                    config.close ();
                    return;
                }
            }
            else {  /* if (mode == modeRead) */
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
                entry = new Entry ();

                code = CreateEntry (&item, collect, sizes, npar, entry);
                if (code != codeOK) {
                    cerr << "In entry at line " << start << ": ";

                    for (i = 0; i < kSizeMessages; i++) {
                        if (kErrorMessages[i].code == code) {
                            cerr << kErrorMessages[i].message << endl;
                            break;
                        }
                    }
                    config.close ();
                    return;
                }
                entries_.push_back (entry);
                nentries_++;
            }
        }
    }
    config.close ();

    for (i = 0; i < kSizeActors; i++) {
        if (kActors[i].mandatory && (counters[i] < 1)) {
            cerr << "No entries found for " << kActors[i].id << "." << endl;
            return;
        }
    }

    total = 0;
    for (i = 0; i < kSizeActors; i++) {
        if (!kActors[i].mandatory) {
            total += counters[i];
        }
    }
    if (total < 1) {
        cerr << "Scene contains no actors." << endl;
        return;
    }
    status_ = statusOK;
}


/**** Entry. ****/

Entry::Entry () {
    npar_ = 0;
}

Entry::~Entry () {
}

bool Entry::CheckLabel (string label) {
    return (label == label_) ? true : false;
}

void Entry::SetLabel (string *label) {
    label_ = (*label);
}

void Entry::AddTextual (const string *key, const string *text, 
                        unsigned int ntext) {
    unsigned int i;

    for (i = 0; i < ntext; i++) {
        text_[npar_][i] = text[i];
    }
    keys_[npar_++] = (*key);
}

void Entry::AddNumerical (const string *key, double *real, 
                          unsigned int nreal) {
    unsigned int i;

    for (i = 0; i < nreal; i++) {
        real_[npar_][i] = real[i];
    }
    keys_[npar_++] = (*key);
}

void Entry::StartQuery () {
    /*
     * Method must be called before calling Query().
     */
    current_ = 0;
}

bool Entry::Query (string *key, double **numerical, string **textual) {
    /*
     * Connect the parser with the actual initialization 
     * of actors.
     *
     * The method is supposed to be called iteratively
     * in a while type of loop, until false is returned.
     *
     */
    if (current_ == npar_) {
        return false;
    }
    *key = keys_[current_];
    *textual = &text_[current_][0];
    *numerical = &real_[current_][0];

    current_++;

    return true;
}


/**** Utility functions. ****/

bool TokenizeLine (const string *line, string *tokens,
                   unsigned int *ntokens, unsigned int maxtokens) {
    /*
     * Split a line to tokens.
     *
     * Remove redundant blank characters and comments.
     */
    size_t  i, ti;
    bool    stop;
    string  chars, token;

    chars = (*line);
    i = chars.find ("#");

    if (i != string::npos) {
        chars.resize (i);
    }
    ti = 0;
    stop = false;

    while (!stop) {
        i = chars.find (" ");
        if (i == string::npos) {
            i = chars.find ("\t");
        }
        token = "";
        if (i != string::npos) {
            if (i > 0) {
                token = chars.substr (0, i);
            }
            chars = chars.substr (i + 1);
        }
        else {
            token = chars;
            stop = true;
        }
        if (token != "") {
            tokens[ti] = token;
            if (++ti > (size_t) maxtokens) {
                return false;
            }
        }
    }
    *ntokens = (unsigned int) ti;
    return true;
}

bool ConvertTokens (const string *tokens, unsigned int ntokens, 
                    double *out) {
    /*
     * Convert tokens to real numbers.
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

bool CheckFilename (string *in, string *out, string *ext) {
    /*
     * Check for a valid filename.
     */
    string filename = (*in);
    unsigned int position = filename.length () - 1;

    if ((filename.at (0) != '"') || (filename.at (position) != '"')) {
        return false;
    }
    string trim = filename.substr (1, position - 1);
    (*out) = trim;
    /*
     * Check for a valid extension.
     */
    if (ext != NULL) {
        string extension = trim.substr (position - 4, 3);
        if (extension != (*ext)) {
            return false;
        }
    }
    /*
     * Check if the file exists.
     */
    const char *text = trim.c_str ();
    ifstream file (text);

    if (!file.good ()) {
        return false;
    }
    return true;
}
