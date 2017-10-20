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

#include <cstddef>  /* NULL pointer. */
#include <string>

#include "png.hpp"
#include "color.hpp"


class Buffer {
    Color  *data_;
    unsigned int width_, height_;

public:
    Buffer (unsigned int width, unsigned int height);
    ~Buffer ();
    void   Allocate ();
    void   Clear ();
    Color *Pointer (unsigned int vline);
    void   WriteToPNG (std::string *filename);
};

#endif /* _BUFFER_H */
