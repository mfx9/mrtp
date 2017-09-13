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

#include <cstddef>  /* NULL pointer. */
#include <iostream>
#include <string>
#include <fstream>

#include "utils.hpp"


/*
 * Macros.
 */
#define MAKE_MASK(bit) (1 << bit)

#define CHECK_BIT(flags, bit) ((flags >> bit) & 1)


#define MAX_LINES       8
#define MAX_TOKENS      4
#define MAX_COMPONENTS  (MAX_TOKENS - 1)

/*
 * Custom data types.
 */
enum ParserFlag_t {flagText, flagReal, flagVector, flagOptional,
    flagCheckZero, flagCheckPositive};

enum ParserCode_t {codeOK, codeUnknown, codeType, codeSize, codeMissing, 
    codeRepeated, codeFilename, codeValue, codeConflict};

enum ParserStatus_t {statusOK, statusFail};
enum ParserParameter_t {parameterReal, parameterText};
enum ParserMode_t {modeOpen, modeRead};

typedef unsigned int Bitmask_t;

/*
 * Bit masks.
 */
#define BIT_TEXT            MAKE_MASK (flagText)
#define BIT_REAL            MAKE_MASK (flagReal)
#define BIT_VECTOR          MAKE_MASK (flagVector)
#define BIT_OPTIONAL        MAKE_MASK (flagOptional)
#define BIT_CHECK_ZERO      MAKE_MASK (flagCheckZero)
#define BIT_CHECK_POSITIVE  MAKE_MASK (flagCheckPositive)


class Entry {
    std::string  label_;
    Entry  *next_;
    unsigned int npar_, current_;

    /*
     * Parameter keys (position, center, etc).
     */
    std::string keys_[MAX_LINES];

    /*
     * Type of each parameter (real or text).
     */
    ParserParameter_t type_[MAX_LINES];

    /*
     * Real numbers (components of 3D vectors, etc).
     */
    double real_[MAX_LINES][MAX_COMPONENTS];

    /*
     * Strings (usually filenames of texture files).
     */
    std::string text_[MAX_LINES][MAX_COMPONENTS];

public:
    Entry (std::string *label);
    Entry ();
    ~Entry ();

    void   CopyTo (Entry *other);
    void   Print ();
    void   Clear ();
    bool   AddText (const std::string *key, const std::string *text, 
        unsigned int ntext);
    bool   AddReal (const std::string *key, double *real, unsigned int nreal);
    void   SetLabel (std::string *label);
    void   StartQuery ();
    bool   Query (std::string *key, ParserParameter_t *type, double *real, 
        std::string *text);
    void   GetLabel (std::string *label);
    Entry *Next ();
    void   SetNext (Entry *next);
};

class Parser {
    std::string   filename_;
    Entry   *entries_;
    unsigned int  nentries_, current_;
    ParserStatus_t  status_;

public:
    Parser (std::string *filename);
    ~Parser ();

    ParserStatus_t Status ();
    void Parse ();
    void StartQuery ();
    bool Query (Entry *entry);

    ParserCode_t CreateEntry (std::string *id, std::string collect[][MAX_TOKENS],
        unsigned int sizes[], unsigned int ncol, Entry *entry);
    unsigned int AddEntry (Entry *temp);
    unsigned int PopEntry (Entry *entry);
    unsigned int NumberEntries ();
};

/*
 * Structures to store templates.
 */
struct TemplateParameter {
    char         id, replace;
    std::string  label, defaults;
    Bitmask_t    flags;
};

struct TemplateItem {
    std::string  id;
    const TemplateParameter *templ;
    unsigned int ntempl;
};

/*
 * Global constants.
 */
extern const TemplateItem kItems[];
extern const unsigned int kSizeItems;


#endif /* _PARSER_H */
