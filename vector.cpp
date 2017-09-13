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
    x_ = 0.0f;
    y_ = 0.0f;
    z_ = 0.0f;
}

Vector::Vector (double x, double y, 
        double z) {
    x_ = x;
    y_ = y;
    z_ = z;
}

Vector::Vector (double *coor) {
    x_ = coor[0];
    y_ = coor[1];
    z_ = coor[2];
}

void Vector::Set (double x, double y, 
        double z) {
    x_ = x;
    y_ = y;
    z_ = z;
}

void Vector::Set (double *coor) {
    x_ = coor[0];
    y_ = coor[1];
    z_ = coor[2];
}

void Vector::CopyTo (Vector *other) {
    other->x_ = x_;
    other->y_ = y_;
    other->z_ = z_;
}

double Vector::Len () {
    return sqrt (x_ * x_ + y_ * y_ + z_ * z_);
}

void Vector::Scale_InPlace (double scale) {
    x_ *= scale;
    y_ *= scale;
    z_ *= scale;
}

void Vector::Normalize_InPlace () {
    double len, scale;

    len = sqrt (x_ * x_ + y_ * y_ + z_ * z_);
    if (len != 0.0f) {
        scale = 1.0f / len;
        x_ *= scale;
        y_ *= scale;
        z_ *= scale;
    }
    else {
        x_ = 0.0f;
        y_ = 0.0f;
        z_ = 0.0f;
    }
}

Vector Vector::operator+ (Vector &other) {
    Vector T (x_ + other.x_, y_ + other.y_, z_ + other.z_);
    return T;
}

Vector Vector::operator- (Vector &other) {
    Vector T (x_ - other.x_, y_ - other.y_, z_ - other.z_);
    return T;
}

Vector Vector::operator^ (Vector &other) {
    Vector T (
        y_ * other.z_ - z_ * other.y_,
        z_ * other.x_ - x_ * other.z_,
        x_ * other.y_ - y_ * other.x_ );
    return T;
}

double Vector::operator* (Vector &other) {
    return (x_ * other.x_ + y_ * other.y_ + z_ * other.z_);
}

Vector Vector::operator* (double scale) {
    Vector T (x_ * scale, y_ * scale, z_ * scale);
    return T;
}

void Vector::GenerateUnitVector (Vector *other) {
    /*
     * Method finds the smallest component of a vector
     * and generates an "associated" unit vector.
     *
     * A cross product of the vector with its "associated" 
     * vector should give a non-zero vector.
     *
     */
    double x = ABS (x_), y = ABS (y_), 
        z = ABS (z_);
    other->Set (0.0f, 0.0f, 1.0f);

    if (x < y) {
        if (x < z) {
            other->Set (1.0f, 0.0f, 0.0f);
        }
    }
    else {  /* if ( x >= y) */
        if (y < z) {
            other->Set (0.0f, 1.0f, 0.0f);
        }
    }
}

/*
void Vector::Print () {
    cout << std::fixed << std::showpoint;
    cout << std::setprecision (PRINT_PRECISION);
    cout << "X: " << x_
        << ", Y: " << y_
        << ", Z: " << z_ << std::endl;
}
*/
