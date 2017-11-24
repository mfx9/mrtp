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
#ifndef _TEXTURE_H
#define _TEXTURE_H

/* NULL pointer. */
#include <cstddef>
#include <string>

#include "png.hpp"
#include "color.hpp"


class Texture {
    unsigned int width_, height_;
    std::string  filename_;
    Color  *data_;

public:
    Texture (std::string *filename);
    ~Texture ();

    void Allocate ();
    Color *GetColor (const double fracx, const double fracy, 
                     const double scale) const;
    bool CheckFilename (std::string *filename) const;
};

#endif /* _TEXTURE_H */
