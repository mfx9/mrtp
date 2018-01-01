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

static unsigned int TokenizeLine (const string *line, vector<string> *tokens);
static bool ConvertTokens (vector<string> *tokens, vector<double> *output);
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

ParserCode_t Parser::PushParameter (bitset<MAX_FLAGS> flags, vector<string> *tokens, 
                                    Entry *entry) {
    /* DEBUG
    cout << "Tokens: ";
    unsigned int size = tokens->size ();

    for (unsigned int i=0; i< size; i++) {
        cout << (*tokens)[i] << "   ";
    }
    cout << endl; */

    unsigned int   j, ntokens;
    string         extension, pathin, pathout;
    bool           check;
    vector<double> output;
    vector<string> text;


    if (flags[flagVector]) {
        ntokens = 3;
    }
    else {  /* if (flags[flagReal] || flags[flagText]) */
        ntokens = 1;
    }
    if (tokens->size () != ntokens) {
        return codeSize;
    }

    if (!flags[flagText]) {
        /* Parameter is a vector or real number. */

        check = ConvertTokens (tokens, &output);
        if (!check) {
            return codeType;
        }
        /* Check for invalid values. */
        check = true;

        if (flags[flagCheckZero]) {
            check = false;
            for (j = 0; j < ntokens; j++) {
                if (output[j] != 0.0f) {
                    check = true;
                    break;
                }
            }
        }
        else if (flags[flagCheckPositive]) {
            check = false;
            for (j = 0; j < ntokens; j++) {
                if (output[j] > 0.0f) {
                    check = true;
                    break;
                }
            }
        }
        else if (flags[flagCheckZeroOne]) {
            check = false;
            for (j = 0; j < ntokens; j++) {
                if ((output[j] >= 0.0f) && (output[j] <= 1.0f)) {
                    check = true;
                    break;
                }
            }
        }
        if (!check) {
            return codeValue;
        }
        entry->AddNumerical (&output);
    }
    else {
        /* Parameter is a texture. */
        pathin = tokens->at (0);
        extension = "png";

        check = CheckFilename (&pathin, &pathout, &extension);
        if (!check) {
            return codeFilename;
        }
        text.push_back (pathout);
        entry->AddTextual (&text);
    }
    return codeOK;
}

ParserCode_t Parser::CreateEntry (string *label, vector<string> *collected, 
                                  vector<unsigned int> *sizes, Entry *entry) {
    unsigned int   i, j, k, ci, cj, ncol;
    bool           found, foundOther;
    vector<bool>   checklist;
    vector<string> tokens;
    ParserCode_t   code;

    const MotifEntry     *motif;
    const MotifParameter *motifParameter, *motifParameterOther;


    motif = kEntries;
    ncol = sizes->size ();

    /* Find a motif for the current actor, camera, light. */
    while (motif->label != (*label)) {
        motif++;
    }
    entry->SetID (motif->id);
    /* DEBUG
    cout << "  ** Creating entry: " << motif->label << endl; */


    /* Check for unknown or repeated parameters. */
    for (i = 0; i < motif->nparameters; i++) {
        checklist.push_back (false);
    }

    for (ci = 0, i = 0; i < ncol; ci += sizes->at (i), i++) {
        motifParameter = motif->parameters;
        found = false;
        for (j = 0; j < motif->nparameters; j++, motifParameter++) {
            if (collected->at (ci) == motifParameter->label) {
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
        for (ci = 0, j = 0; j < ncol; ci += sizes->at (j), j++) {
            if (collected->at (ci) == motifParameter->label) {
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
            for (cj = 0, k = 0; k < ncol; cj += sizes->at (k), k++) {
                if (k != j) {
                    if (collected->at (cj) == motifParameterOther->label) {
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

            tokens.clear ();
            for (k = 1; k < sizes->at (j); k++) {
                tokens.push_back (collected->at (ci + k));
            }
            /* DEBUG
            cout << "Parameter: " << collected->at (ci) << "... "; */

            code = PushParameter (motifParameter->flags, &tokens, entry);
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

            tokens.clear ();
            TokenizeLine (&motifParameter->defaults, &tokens);
            /* DEBUG
            cout << "Default parameter: " << motifParameter->label << "... "; */
            PushParameter (motifParameter->flags, &tokens, entry);
        }
    }
    return codeOK;
}

void Parser::Parse () {
    vector<string>       tokens;
    vector<string>       collect;
    vector<unsigned int> sizes;
    vector<unsigned int> counters;

    string        line, item;
    unsigned int  i, nlines, start;
    unsigned int  ntokens;
    unsigned int  nallowed, total;

    ParserMode_t  mode;
    ParserCode_t  code;
    bool          check;
    Entry        *entry;


    /* Initialize. */
    for (i = 0; i < kSizeEntries; i++) {
        counters.push_back (0);
    }

    status_ = statusFail;
    mode    = modeOpen;
    nlines  = 0;

    /* Open the input file. */
    const char *fn = path_.c_str ();
    ifstream config (fn);

    if (!config.is_open ()) {
        cerr << "File \"" << path_ << "\" cannot be opened." << endl;
        return;
    }


    while (getline (config, line)) {
        nlines++;

        tokens.clear ();
        ntokens = TokenizeLine (&line, &tokens);
        if (ntokens > MAX_TOKENS) {
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
                        start = nlines;
                        mode  = modeRead;

                        collect.clear ();
                        sizes.clear ();

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
                    cerr << "Line " << nlines << ": Unrecognized item \"" << 
                        item << "\"." << endl;
                    config.close ();
                    return;
                }
            }
            else {  /* if (mode == modeRead) */
                if (sizes.size () == MAX_LINES) {
                    cerr << "Line " << nlines << ": Too many parameter lines." << endl;
                    config.close ();
                    return;
                }
                for (i = 0; i < ntokens; i++) {
                    collect.push_back (tokens[i]);
                }
                sizes.push_back (ntokens);
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

                code = CreateEntry (&item, &collect, &sizes, entry);
                if (code != codeOK) {
                    delete entry;
                    config.close ();

                    cerr << "In entry at line " << start << ": ";
                    for (i = 0; i < kSizeMessages; i++) {
                        if (kErrorMessages[i].code == code) {
                            cerr << kErrorMessages[i].message << endl;
                            break;
                        }
                    }
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

unsigned int TokenizeLine (const string *line, 
                           vector<string> *tokens) {
    size_t  i;
    bool    stop;
    string  chars, token;

    chars = (*line);
    i = chars.find ("#");
    if (i != string::npos) {
        chars.resize (i);
    }
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
            tokens->push_back (token);
        }
    }
    return tokens->size ();
}

bool ConvertTokens (vector<string> *tokens, vector<double> *output) {
    stringstream  convert;
    size_t  i;
    double  test;

    for (i = 0; i < tokens->size (); i++) {
        convert.str (tokens->at (i));
        convert >> test;
        if (!convert) {
            return false;
        }
        output->push_back (test);
        convert.clear ();
    }
    return true;
}

bool CheckFilename (string *in, string *out, string *ext) {
    /* Check for a valid filename. */
    string filename = (*in);

    /* Default empty filenames are OK. */
    if (filename == "!") {
        *out = filename;
        return true;
    }

    unsigned int position = filename.length () - 1;
    if ((filename.at (0) != '"') || (filename.at (position) != '"')) {
        return false;
    }
    string trim = filename.substr (1, position - 1);
    (*out) = trim;

    /* Check for a valid extension. */
    if (ext != NULL) {
        string extension = trim.substr (position - 4, 3);
        if (extension != (*ext)) {
            return false;
        }
    }

    /* Check if the file exists. */
    const char *text = trim.c_str ();
    ifstream file (text);

    if (!file.good ()) {
        return false;
    }
    return true;
}
