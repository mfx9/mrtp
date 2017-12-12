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
#ifndef _MOTIFS_H
#define _MOTIFS_H

#include <string>
#include <bitset>

#define MAX_FLAGS 16


enum EntryID_t {entryCamera, entryLight, entryPlane, entrySphere, entryCylinder};

enum ParameterFlag_t {flagText, flagReal, flagVector, flagOptional, flagCheckZero, 
                      flagCheckPositive, flagCheckZeroOne};

struct MotifParameter {
    int             id;
    int             replace;
    std::string     label;
    std::string     defaults;
    std::bitset<MAX_FLAGS> flags;
};

struct MotifEntry {
    EntryID_t     id;
    std::string   label;
    bool          mandatory;
    int           nallowed;
    int           nparameters;
    const MotifParameter *parameters;
};


extern const MotifEntry kEntries[];
extern const unsigned int kSizeEntries;

#endif /* !_MOTIFS_H */
