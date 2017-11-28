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
#include <cmath>
#include "vector.hpp"


Vector::~Vector () {
}

Vector::Vector () {
    x_ = 0.0f;
    y_ = 0.0f;
    z_ = 0.0f;
}

Vector::Vector (const double x, const double y, 
                const double z) {
    x_ = x;
    y_ = y;
    z_ = z;
}

Vector::Vector (const double *flat) {
    x_ = flat[0];
    y_ = flat[1];
    z_ = flat[2];
}

void Vector::Set (const double x, const double y, 
                  const double z) {
    x_ = x;
    y_ = y;
    z_ = z;
}

void Vector::Set (const double *flat) {
    x_ = flat[0];
    y_ = flat[1];
    z_ = flat[2];
}

void Vector::Get (double *x, double *y, double *z) const {
    (*x) = x_;
    (*y) = y_;
    (*z) = z_;
}

void Vector::Get (double *flat) const {
    flat[0] = x_;
    flat[1] = y_;
    flat[2] = z_;
}

void Vector::CopyTo (Vector *other) const {
    other->x_ = x_;
    other->y_ = y_;
    other->z_ = z_;
}

double Vector::Magnitude () const {
    return sqrt (x_ * x_ + y_ * y_ + z_ * z_);
}

void Vector::Scale_InPlace (const double scale) {
    x_ *= scale;
    y_ *= scale;
    z_ *= scale;
}

double Vector::Normalize_InPlace () {
    double magnitude, scale;

    magnitude = sqrt (x_ * x_ + y_ * y_ + z_ * z_);
    if (magnitude != 0.0f) {
        scale = 1.0f / magnitude;
        x_ *= scale;
        y_ *= scale;
        z_ *= scale;
    }
    else {
        x_ = 0.0f;
        y_ = 0.0f;
        z_ = 0.0f;
    }
    return magnitude;
}

Vector Vector::operator+ (const Vector &other) const {
    Vector T (x_ + other.x_, y_ + other.y_, z_ + other.z_);
    return T;
}

Vector Vector::operator- (const Vector &other) const {
    Vector T (x_ - other.x_, y_ - other.y_, z_ - other.z_);
    return T;
}

Vector Vector::operator^ (const Vector &other) const {
    /* Cross product. */
    Vector T (
        y_ * other.z_ - z_ * other.y_,
        z_ * other.x_ - x_ * other.z_,
        x_ * other.y_ - y_ * other.x_ );
    return T;
}

double Vector::operator* (const Vector &other) const {
    /* Dot product. */
    return (x_ * other.x_ + y_ * other.y_ + z_ * other.z_);
}

Vector Vector::operator* (const double scale) const {
    /* Scaling. */
    Vector T (x_ * scale, y_ * scale, z_ * scale);
    return T;
}

void Vector::GenerateUnitVector (Vector *unit) const {
    /*
     * Method finds the smallest component of a vector
     * and generates an "associated" unit vector.
     *
     * A cross product of the vector with its "associated" 
     * vector should give a non-zero vector.
     *
     */
    double x = (x_ < 0.0f) ? -x_ : x_;
    double y = (y_ < 0.0f) ? -y_ : y_;
    double z = (z_ < 0.0f) ? -z_ : z_;

    unit->Set (0.0f, 0.0f, 1.0f);
    if (x < y) {
        if (x < z) {
            unit->Set (1.0f, 0.0f, 0.0f);
        }
    }
    else {  /* if ( x >= y) */
        if (y < z) {
            unit->Set (0.0f, 1.0f, 0.0f);
        }
    }
}

void Vector::Reflect (const Vector *normal, Vector *reflected) const {
    /*
     * r' = r - (2 * (r . N)) * N
     */
    Vector T;

    T = (*normal) * (2.0f * ((*this) * (*normal)));
    (*reflected) = (*this) - T;
}

/*
#define PRINT_PRECISION  4

void Vector::Print () const {
    cout << std::fixed << std::showpoint;
    cout << std::setprecision (PRINT_PRECISION);
    cout << "X: " << x_
        << ", Y: " << y_
        << ", Z: " << z_ << std::endl;
}
*/
