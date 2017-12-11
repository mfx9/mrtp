/*
 * Mrtp: A simple raytracing tool.
 * Copyright (C) 2017  Mikolaj Feliks <mikolaj.feliks@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include "parser.hpp"

using namespace std;


/**** Tables. ****/

struct MotifMessage {
    ParserCode_t  code;
    string        message;
};

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

static bool TokenizeLine (const string *line, unsigned int maxtokens,
                          string *tokens, unsigned int *ntokens);
static bool ConvertTokens (const string *tokens, unsigned int ntokens, 
                           double *out);
static bool CheckFilename (string *in, string *ext, string *out);


/**** Parser. ****/

Parser::Parser (string *path) {
    path_ = *path;
}

Parser::~Parser () {
    /* Destroy all entries. */
    Entry *entry;

    if (!entries_.empty ()) {
        entry = entries_.back ();
        delete entry;
        entries_.pop_back ();
    }
}

ParserStatus_t Parser::Check (unsigned int *nentries) {
    *nentries = entries_.size ();
    return status_;
}

void Parser::StartQuery () {
    currentEntry_ = entries_.begin ();
}

bool Parser::Query (Entry **entry) {
    if (entries_.end () == currentEntry_) {
        return false;
    }
    *entry = *currentEntry_;
    currentEntry_++;

    return true;
}

ParserCode_t Parser::PushParameter (bitset<MAX_BITS> flags, string tokens[MAX_TOKENS], 
                                    unsigned int size, Entry *entry) {
    unsigned int  j, ntokens;
    double        output[MAX_COMPONENTS];
    string        extension, path;
    bool          check;

    ntokens = (flags[flagVector]) ? 4 : 2;
    if (size != ntokens) {
        return codeSize;
    }

    if (!flags[flagText]) {
        /* Parameter is a vector or real number. */

        check = ConvertTokens (&tokens[1], (ntokens - 1), output);
        if (!check) {
            return codeType;
        }
        /* Check for invalid values. */
        check = true;

        if (flags[flagCheckZero]) {
            check = false;
            for (j = 0; j < (ntokens - 1); j++) {
                if (output[j] != 0.0f) {
                    check = true;
                    break;
                }
            }
        }
        else if (flags[flagCheckPositive]) {
            check = false;
            for (j = 0; j < (ntokens - 1); j++) {
                if (output[j] > 0.0f) {
                    check = true;
                    break;
                }
            }
        }
        else if (flags[flagCheckZeroOne]) {
            check = false;
            for (j = 0; j < (ntokens - 1); j++) {
                if ((output[j] >= 0.0f) && (output[j] <= 1.0f)) {
                    check = true;
                    break;
                }
            }
        }
        if (!check) {
            return codeValue;
        }
        entry->AddNumerical (output, (ntokens - 1));
    }
    else {
        /* Parameter is a texture. */

        extension = "png";
        check = CheckFilename (&tokens[1], &path, &extension);
        if (!check) {
            return codeFilename;
        }
        entry->AddTextual (&path, 1);
    }
    return codeOK;
}

ParserCode_t Parser::CreateEntry (string *entryLabel, string collected[][MAX_TOKENS],
                                  unsigned int sizes[], unsigned int ncol, 
                                  Entry *entry) {
    unsigned int  i, j, k, ntokens;
    bool          found, foundOther;
    bool          checklist[MAX_LINES];

    string        tokens[MAX_TOKENS];
    ParserCode_t  code;

    const MotifEntry     *motif;
    const MotifParameter *motifParameter, *motifParameterOther;


    /* Find a motif for the current actor, camera, light. */
    motif = kEntries;
    while (motif->label != (*entryLabel)) {
        motif++;
    }
    entry->SetID (motif->id);

    /* Check for unknown or repeated parameters. */
    for (i = 0; i < MAX_LINES; i++) {
        checklist[i] = false;
    }

    for (i = 0; i < ncol; i++) {
        motifParameter = motif->parameters;
        found = false;
        for (j = 0; j < motif->nparameters; j++, motifParameter++) {
            if (collected[i][0] == motifParameter->label) {
                found = true;
                break;
            }
        }
        if (!found) {
            return codeUnknown;
        }
        if (checklist[j]) {
            return codeRepeated;
        }
        checklist[j] = true;
    }

    /* Go over all parameters in the motif. */
    motifParameter = motif->parameters;

    for (i = 0; i < motif->nparameters; i++, motifParameter++) {
        found = false;
        for (j = 0; j < ncol; j++) {
            if (collected[j][0] == motifParameter->label) {
                found = true;
                break;
            }
        }

        if (motifParameter->replace) {
            /* Check if the alternative parameter is also present. */
            motifParameterOther = motif->parameters;
            
            for (k = 0; k < motif->nparameters; k++, motifParameterOther++) {
                if (k != i) {
                    if (motifParameter->replace == motifParameterOther->id) {
                        break;
                    }
                }
            }
            foundOther = false;
            for (k = 0; k < ncol; k++) {
                if (k != j) {
                    if (collected[k][0] == motifParameterOther->label) {
                        foundOther = true;
                        break;
                    }
                }
            }
        }

        if (found) {
            /* Check for conflicting parameters. */
            if (motifParameter->replace && foundOther) {
                return codeConflict;
            }
            code = PushParameter (motifParameter->flags, collected[j], sizes[j], entry);
            if (code != codeOK) {
                return code;
            }
            /* Parameter pushed. */
        }
        else {
            if (motifParameter->replace) {
                if (!foundOther) {
                    return codeMissing;
                }
            }
            else {
                if (!motifParameter->flags[flagOptional]) {
                    return codeMissing;
                }
            }
            /* Push defaults. */

            tokens[0] = motifParameter->label;
            TokenizeLine (&motifParameter->defaults, MAX_COMPONENTS, &tokens[1], &ntokens);
            PushParameter (motifParameter->flags, tokens, (ntokens + 1), entry);
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
    unsigned int  counters[kSizeEntries], nallowed, total;

    ParserMode_t  mode;
    ParserCode_t  code;
    bool          check;
    Entry        *entry;

    /*
     * Initialize.
     */
    for (i = 0; i < kSizeEntries; i++) {
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

        check = TokenizeLine (&line, MAX_TOKENS, tokens, &ntokens);
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
                for (i = 0; i < kSizeEntries; i++) {
                    if (kEntries[i].label == item) {
                        check = true;
                        npar  = 0;
                        start = nlines;
                        mode  = modeRead;

                        nallowed = kEntries[i].nallowed;
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
            }
        }
    }
    config.close ();

    for (i = 0; i < kSizeEntries; i++) {
        if (kEntries[i].mandatory && (counters[i] < 1)) {
            cerr << "No entries found for " << kEntries[i].id << "." << endl;
            return;
        }
    }

    total = 0;
    for (i = 0; i < kSizeEntries; i++) {
        if (!kEntries[i].mandatory) {
            total += counters[i];
        }
    }
    if (total < 1) {
        cerr << "Scene contains no actors." << endl;
        return;
    }
    status_ = statusOK;
}


/**** Utility functions. ****/

bool TokenizeLine (const string *line, unsigned int maxtokens, 
                   string *tokens, unsigned int *ntokens) {
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
