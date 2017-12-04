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
#ifndef _MOTIFS_H
#define _MOTIFS_H

#include <string>


typedef unsigned int Bitmask_t;

enum EntryID_t {entryCamera, entryLight, entryPlane, 
    entrySphere, entryCylinder};

enum ParameterFlag_t {flagText, flagReal, flagVector, flagOptional,
    flagCheckZero, flagCheckPositive, flagCheckZeroOne};

#define MAKE_MASK(bit) (1 << bit)

#define BIT_TEXT            MAKE_MASK (flagText)
#define BIT_REAL            MAKE_MASK (flagReal)
#define BIT_VECTOR          MAKE_MASK (flagVector)
#define BIT_OPTIONAL        MAKE_MASK (flagOptional)
#define BIT_CHECK_ZERO      MAKE_MASK (flagCheckZero)
#define BIT_CHECK_POSITIVE  MAKE_MASK (flagCheckPositive)
#define BIT_CHECK_ZERO_ONE  MAKE_MASK (flagCheckZeroOne)


struct MotifParameter {
    char         id;
    char         replace;
    std::string  label;
    std::string  defaults;
    Bitmask_t    flags;
};

struct MotifEntry {
    EntryID_t     id;
    std::string   label;
    bool          mandatory;
    unsigned int  nallowed;
    unsigned int  nparameters;

    const MotifParameter *parameters;
};

extern const MotifEntry kEntries[];
extern const unsigned int kSizeEntries;

#endif /* !_MOTIFS_H */
