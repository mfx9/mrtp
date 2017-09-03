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
#include <string>
#include <fstream>
using namespace std;

#include "utils.hpp"

/*
 * Macros for bit masks.
 */
#define MAKE_MASK(bit) (1 << bit)

#define CHECK_BIT(flags, bit) ((flags >> bit) & 1)


#define MAX_LINES       8
#define MAX_TOKENS      4
#define MAX_COMPONENTS  (MAX_TOKENS - 1)

/*
 * Bit masks.
 */
#define TP_TEXT            0
#define TP_REAL            1
#define TP_VECTOR          2
#define TP_CHECK_ZERO      3
#define TP_CHECK_POSITIVE  4
#define TP_OPTIONAL        5

#define BIT_TEXT            MAKE_MASK (TP_TEXT)
#define BIT_REAL            MAKE_MASK (TP_REAL)
#define BIT_VECTOR          MAKE_MASK (TP_VECTOR)
#define BIT_CHECK_ZERO      MAKE_MASK (TP_CHECK_ZERO)
#define BIT_CHECK_POSITIVE  MAKE_MASK (TP_CHECK_POSITIVE)
#define BIT_OPTIONAL        MAKE_MASK (TP_OPTIONAL)

/*
 * Custom data types.
 */
enum ParserCode_t {codeOK, codeUnknown, codeType, codeSize, codeMissing, 
    codeRedundant, codeFilename, codeValue, codeConflict};

enum ParserStatus_t {statusOK, statusFail};
enum ParserParameter_t {parameterReal, parameterText};
enum ParserMode_t {modeOpen, modeRead};

typedef unsigned int Flags_t;


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
    ParserParameter_t type_[MAX_LINES];
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

    Entry *Next ();
    void SetNext (Entry *next);
    void CopyTo (Entry *other);
    void Print ();
    void Clear ();

    bool AddText (string *key, string *text, unsigned int ntext);
    bool AddReal (string *key, double *real, unsigned int nreal);
    void SetLabel (string *label);

    bool PopData (string *key, ParserParameter_t *type, 
        double *real, string *text);
    void GetLabel (string *label);
};

class Parser {
    string   filename_;
    ParserStatus_t status_;
    
    Entry   *entries_;
    unsigned int nentries_;
    /*
     * Private methods.
     */
    unsigned int AddEntry (Entry *temp);

    ParserCode_t CreateEntry (string *id, string collect[][MAX_TOKENS],
        unsigned int sizes[], unsigned int ncol,
            unsigned int *errline, string *errmsg,
                Entry *entry);
public:
    Parser (string *filename);
    ~Parser ();
    void Parse ();
    ParserStatus_t Status ();
    unsigned int GetNumberEntries ();
    unsigned int PopEntry (Entry *entry);
};

/*
 * Structures to store templates.
 */
struct TemplateParameter {
    char id, replace;
    string label;
    Flags_t flags;
};

struct TemplateItem {
    string id;
    const TemplateParameter *templ;
    unsigned int ntempl;
};

/*
 * Global constants.
 */
extern const TemplateItem kItems[];
extern const unsigned int kSizeItems;


#endif /* _PARSER_H */
