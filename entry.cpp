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
#include "entry.hpp"

using namespace std;


Entry::Entry () {
    nitems_ = 0;
}

Entry::~Entry () {
}

bool Entry::CheckID (EntryID_t id) {
    return (id == id_) ? true : false;
}

void Entry::SetID (EntryID_t id) {
    id_ = id;
}

void Entry::AddTextual (const string text[], unsigned int ntext) {
    for (unsigned int i = 0; i < ntext; i++) {
        textual_.push_back (text[i]);
    }
    sizesTextual_.push_back (ntext);

    items_.push_back (itemTextual);
    nitems_++;
}

void Entry::AddNumerical (double real[], unsigned int nreal) {
    for (unsigned int i = 0; i < nreal; i++) {
        numerical_.push_back (real[i]);
    }
    sizesNumerical_.push_back (nreal);

    items_.push_back (itemNumerical);
    nitems_++;
}

void Entry::StartQuery () {
    /* Method must be called before calling Query(). */

    currentItem_ = 0;
    currentTextual_ = 0;
    currentNumerical_ = 0;
}

bool Entry::Query (double **numerical, string **textual) {
    /*
     * Connects the parser with the actual initialization 
     * of actors.
     *
     * Call this method iteratively in a while loop, 
     * until false is returned.
     */
    unsigned int index = 0, i;

    if (currentItem_ == nitems_) {
        return false;
    }
    if (items_[currentItem_] == itemNumerical) {
        for (i = 0; i < currentNumerical_; i++) {
            index += sizesNumerical_[i];
        }
        *numerical = &numerical_[index];
        currentNumerical_++;
    }
    else {
        for (i = 0; i < currentTextual_; i++) {
            index += sizesTextual_[i];
        }
        *textual = &textual_[index];
        currentTextual_++;
    }
    currentItem_++;

    return true;
}
