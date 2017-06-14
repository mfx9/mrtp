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
#include "vector.hpp"


Vector::~Vector () {
}

Vector::Vector () {
}

Vector::Vector (double newx,
        double newy, double newz) {
    x = newx;
    y = newy;
    z = newz;
}

void Vector::Set (double newx,
        double newy, double newz) {
    x = newx;
    y = newy;
    z = newz;
}

void Vector::Print () {
    printf ("x=%7.3f    y=%7.3f    z=%7.3f\n", x, y, z);
}

double Vector::Len () {
    return sqrt (x * x + y * y + z * z);
}

double Vector::DotSelf () {
    return (x * x + y * y + z * z);
}

void Vector::Scale_InPlace (double scale) {
    x *= scale;
    y *= scale;
    z *= scale;
}

void Vector::Normalize_InPlace () {
    double len, scale;

    len = sqrt (x * x + y * y + z * z);
    if (len != 0.) {
        scale = 1. / len;
        x *= scale;
        y *= scale;
        z *= scale;
    }
}

void Vector::CopyTo (Vector *other) {
    other->x = x;
    other->y = y;
    other->z = z;
}

Vector Vector::operator+ (Vector &other) {
    Vector o;
    o.x = x + other.x;
    o.y = y + other.y;
    o.z = z + other.z;
    return o;
}

Vector Vector::operator- (Vector &other) {
    Vector o;
    o.x = x - other.x;
    o.y = y - other.y;
    o.z = z - other.z;
    return o;
}

Vector Vector::operator^ (Vector &other) {
    Vector o;
    o.x = y * other.z - z * other.y;
    o.y = z * other.x - x * other.z;
    o.z = x * other.y - y * other.x;
    return o;
}

double Vector::operator* (Vector &other) {
    return (x * other.x + y * other.y + z * other.z);
}

Vector Vector::operator* (double scale) {
    Vector o;
    o.x = x * scale;
    o.y = y * scale;
    o.z = z * scale;
    return o;
}
