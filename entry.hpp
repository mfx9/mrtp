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
#ifndef _ENTRY_H
#define _ENTRY_H

#include <string>
#include <vector>
#include "motifs.hpp"  /* Used for: EntryID_t */

enum ItemType_t {itemNumerical, itemTextual};


class Entry {
    EntryID_t  id_;

    std::vector<ItemType_t>   items_;

    std::vector<std::string>  textual_;
    std::vector<unsigned int> sizesTextual_;

    std::vector<double>       numerical_;
    std::vector<unsigned int> sizesNumerical_;

    unsigned int nitems_;
    unsigned int currentItem_;
    unsigned int currentTextual_;
    unsigned int currentNumerical_;

public:
    Entry ();
    ~Entry ();

    void SetID (EntryID_t id);
    void AddTextual (std::vector<std::string> *tokens);
    void AddNumerical (std::vector<double> *tokens);

    bool CheckID (EntryID_t id);
    void StartQuery ();
    bool Query (double **numerical, std::string **textual);
};

#endif /* !_ENTRY_H */
