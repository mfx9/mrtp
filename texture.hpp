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
#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>
using namespace std;

#include "png.hpp"
using namespace png;

#include "color.hpp"


class Texture {
    unsigned int width_, height_;
    Color   *data_;
    string   filename_;
    Texture *next_;

public:
    Texture (string *filename);
    ~Texture ();

    void Allocate ();
    bool CheckFilename (string *filename);
    Color *GetColor (double fracx, double fracy, 
        double scale);

    Texture *GetNext ();
    void SetNext (Texture *next);
};

#endif /* _TEXTURE_H */
