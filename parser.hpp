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
#ifndef _PARSER_H
#define _PARSER_H

#include <cstddef>  /* NULL pointer. */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>

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
enum EntryID_t {entryCamera, entryLight, entryPlane, 
    entrySphere, entryCylinder};

enum ParameterFlag_t {flagText, flagReal, flagVector, flagOptional,
    flagCheckZero, flagCheckPositive, flagCheckZeroOne};

enum ParserCode_t {codeOK, codeUnknown, codeType, codeSize, codeMissing, 
    codeRepeated, codeFilename, codeValue, codeConflict};

enum ParserStatus_t {statusOK, statusFail};
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
#define BIT_CHECK_ZERO_ONE  MAKE_MASK (flagCheckZeroOne)


class Entry {
    EntryID_t  id_;
    unsigned int npar_, current_;

    /*
     * keys: parameter keys (position, center, etc).
     *
     * real: real numbers (components of 3D vectors, etc).
     *
     * text: strings (usually filenames of texture files).
     */
    std::string keys_[MAX_LINES];

    std::string text_[MAX_LINES][MAX_COMPONENTS];
    double real_[MAX_LINES][MAX_COMPONENTS];

public:
    Entry ();
    ~Entry ();

    void SetID (EntryID_t id);
    void AddTextual (const std::string *key, const std::string *text, 
                     unsigned int ntext);
    void AddNumerical (const std::string *key, double *real, 
                       unsigned int nreal);

    //void Print ();
    void StartQuery ();
    bool CheckID (EntryID_t id);
    bool Query (std::string *key, double **numerical, std::string **textual);
};


class Parser {
    std::string  path_;
    ParserStatus_t  status_;

    std::list<Entry *> entries_;
    std::list<Entry *>::iterator currentEntry_; 

    /* Private methods. */
    ParserCode_t CreateEntry (std::string *entryLabel, 
                              std::string collect[][MAX_TOKENS],
                              unsigned int sizes[], unsigned int ncol, 
                              Entry *entry);
    ParserCode_t PushParameter (Bitmask_t flags, std::string tokens[MAX_TOKENS], 
                                unsigned int size, Entry *entry);

public:
    Parser (std::string *path);
    ~Parser ();
    ParserStatus_t Check (unsigned int *nentries);

    void Parse ();
    void StartQuery ();
    bool Query (Entry **entry);
};

#endif /* _PARSER_H */
