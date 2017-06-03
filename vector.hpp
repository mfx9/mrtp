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
#ifndef _VECTOR_H
#define _VECTOR_H

#include <cstdio>
#include <cmath>


class Vector {
    double x, y, z;
public:
    Vector (double newx, double newy,
        double newz);
    Vector ();
    ~Vector ();
    void Print ();
    void PrintXX ();
    double Len ();
    double DotSelf ();
    void Scale_InPlace (double scale);
    void Normalize_InPlace ();
    void CopyTo (Vector *other);
    /*
     * . Operators:
     *    (+) Sum of two vectors
     *    (-) Difference of two vectors
     *    (^) Cross product
     *    (*) Dot product
     *    (*) Scaling
     */
    Vector operator+ (Vector &other);
    Vector operator- (Vector &other);
    Vector operator^ (Vector &other);
    double operator* (Vector &other);
    Vector operator* (double scale);
};

#endif /* _VECTOR_H */
