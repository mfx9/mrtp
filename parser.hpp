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

/* 
 * Converting strings to doubles. 
 */
#include <sstream>

using namespace std;


#define MAX_LINES       8
#define MAX_TOKENS      4
#define MAX_COMPONENTS  (MAX_TOKENS - 1)

#define MODE_OPEN    1
#define MODE_READ    2

#define TYPE_REAL    1
#define TYPE_TEXT    2

#define STATUS_FAIL  0
#define STATUS_NEW   1
#define STATUS_OK    2

#define CODE_OK          0
#define CODE_ALIEN       1
#define CODE_WRONG_TYPE  2
#define CODE_WRONG_SIZE  3
#define CODE_MISSING     4
#define CODE_REDUNDANT   5
#define CODE_FILENAME    6
#define CODE_NOT_FOUND   7

/*
 * Macros.
 */
#define IS_COMMENT(c) (c == '#')

#define IS_WHITE(c) ((c == ' ') || (c == '\t'))

#define IS_NOT_WHITE(c) ((c != ' ') && (c != '\t'))


class Entry {
    string  label_;
    Entry  *next_;

    /*
     * Parameter keys (position, center, etc.)
     * and the number of parameters.
     */
    string keys_[MAX_LINES];
    unsigned int npar_;

    /*
     * Type of each parameter (real or text).
     */
    char type_[MAX_LINES];
    /*
     * Real numbers (components of 3D vectors, 
     * etc.).
     */
    double real_[MAX_LINES][MAX_COMPONENTS];

    /*
     * Strings (usually filenames of texture
     * files).
     */
    string text_[MAX_LINES][MAX_COMPONENTS];

public:
    Entry (string *label);
    Entry ();
    ~Entry ();

    Entry *GetNext ();
    void SetNext (Entry *next);

    bool PopData (string *key, char *type, double *real, 
        string *text);
    void GetLabel (string *label);

    bool AddText (string *key, string *text, 
        unsigned int ntext);
    bool AddReal (string *key, double *real, 
            unsigned int nreal);
    void SetLabel (string *label);
    void CopyTo (Entry *other);
    void Print ();
    void Clear ();
};

class Parser {
    string   filename_;
    char     status_;
    Entry   *entries_;
    unsigned int nentries_;

    /*
     * Private methods.
     */
    unsigned int AddEntry (Entry *temp);

    bool TokenizeLine (string *line, string *tokens,
        unsigned int *ntokens);
    bool ConvertTokens (string *tokens,
        unsigned int ntokens, double *out);

    char CheckItem (string *item, string collect[][MAX_TOKENS],
        unsigned int sizes[], unsigned int npar,
            unsigned int *errline, string *errmsg,
                Entry *entry);
public:
    Parser (string *filename);
    ~Parser ();
    void Parse ();
    char GetStatus ();
    unsigned int GetNumberEntries ();
    unsigned int PopEntry (Entry *entry);
};

#endif /* _PARSER_H */
