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
#ifndef _BUFFER_H
#define _BUFFER_H

#include <string>
#include "color.hpp"


class Buffer {
    Color  *data_;
    unsigned int width_, height_;

public:
    Buffer (const unsigned int width, const unsigned int height);
    ~Buffer ();

    void Allocate ();
    void Write (std::string *filename);
    Color *Pointer (const unsigned int vline) const;
};

#endif /* !_BUFFER_H */
