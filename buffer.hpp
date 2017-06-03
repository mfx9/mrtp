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
#ifndef _BUFFER_H
#define _BUFFER_H

#include "png.hpp"
#include "color.hpp"

using namespace png;


class Buffer {
    Color *data;
    unsigned int width, heigth;
public:
    Buffer (unsigned int w,
        unsigned int h);
    ~Buffer ();
    void Clear ();
    void WriteToPNG (char *filename);
    void Text (char *text, unsigned int x,
        unsigned int y, Color &color);
    Color *GetPointer ();
};

#endif /* _BUFFER_H */
