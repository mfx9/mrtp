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

#include <cmath>
/*
#include <iomanip>
#include <iostream>
*/

/*
 * Macros.
 */
#define SIGN(x) ((x < 0.0f) ? -1.0f : 1.0f)
#define ABS(x) (x * (SIGN (x)))


class Vector {
    double x_, y_, z_;

public:
    ~Vector ();
    Vector ();
    Vector (double *coor);
    Vector (double x, double y, double z);

    void   Set (double *coor);
    void   Set (double x, double y, double z);
    void   CopyTo (Vector *other);
    void   Scale_InPlace (double scale);
    void   Normalize_InPlace ();
    void   GenerateUnitVector (Vector *other);
    void   Print ();
    double Len ();

    /* Sum of two vectors. */
    Vector operator+ (Vector &other);

    /* Difference of two vectors. */
    Vector operator- (Vector &other);

    /* Cross product. */
    Vector operator^ (Vector &other);

    /* Dot product. */
    double operator* (Vector &other);

    /* Scaling. */
    Vector operator* (double scale);
};

#endif /* _VECTOR_H */
