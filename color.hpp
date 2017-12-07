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
#ifndef _COLOR_H
#define _COLOR_H

typedef float real_t;
typedef unsigned char byte_t;


class Color {
    real_t  red_, green_, blue_;

public:
    ~Color ();
    Color ();
    Color (const real_t red, const real_t green, const real_t blue);

    void Set (const real_t *flat);
    void Set (const real_t red, const real_t green, const real_t blue);
    void Set (const byte_t red, const byte_t green, const byte_t blue);

    void Get (real_t *red, real_t *green, real_t *blue) const;
    void Get (byte_t *red, byte_t *green, byte_t *blue) const;

    void CopyTo (Color *other) const;
    void Scale_InPlace (const real_t scale);
    void Combine_InPlace (const Color *other, const real_t lambda);
    void Zero ();
};

#endif /* !_COLOR_H */
