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
#include "parser.hpp"


const TemplateParameter kCamera[] = {
    { 1,  0,  "position",  BIT_VECTOR },
    { 2,  0,  "target",  BIT_VECTOR },
    { 3,  0,  "roll",  BIT_REAL | BIT_OPTIONAL },
    };

const TemplateParameter kLight[] = {
    { 1,  0,  "position",  BIT_VECTOR},
    };

const TemplateParameter kPlane[] = {
    { 1,  0,  "center",  BIT_VECTOR },
    { 2,  0,  "normal",  BIT_VECTOR | BIT_CHECK_ZERO },
    { 3,  0,  "scale",  BIT_REAL | BIT_CHECK_POSITIVE },
    { 4,  5,  "color",  BIT_VECTOR },
    { 5,  4,  "texture",  BIT_TEXT },
    };

const TemplateParameter kSphere[] = {
    { 1,  0, "position",  BIT_VECTOR },
    { 2,  0, "radius",  BIT_REAL | BIT_CHECK_POSITIVE },
    { 3,  0, "axis",  BIT_VECTOR | BIT_CHECK_ZERO | BIT_OPTIONAL },
    { 4,  5, "color",  BIT_VECTOR },
    { 5,  4, "texture",  BIT_TEXT },
    };

const TemplateParameter kCylinder[] = {
    { 1,  0, "center",  BIT_VECTOR },
    { 2,  0, "direction",  BIT_VECTOR | BIT_CHECK_ZERO },
    { 3,  0, "radius",  BIT_REAL | BIT_CHECK_POSITIVE },
    { 4,  0, "span",  BIT_REAL | BIT_CHECK_ZERO | BIT_OPTIONAL },
    { 5,  6, "color",  BIT_VECTOR },
    { 6,  5, "texture",  BIT_TEXT },
    };

const TemplateItem kItems[] = {
    {"camera", kCamera,
        (unsigned int) (sizeof (kCamera) / sizeof (kCamera[0])) },

    {"light", kLight,
        (unsigned int) (sizeof (kLight) / sizeof (kLight[0])) },

    {"plane", kPlane,
        (unsigned int) (sizeof (kPlane) / sizeof (kPlane[0])) },

    {"sphere", kSphere,
        (unsigned int) (sizeof (kSphere) / sizeof (kSphere[0])) },

    {"cylinder", kCylinder,
        (unsigned int) (sizeof (kCylinder) / sizeof (kCylinder[0])) },
    };

const unsigned int kSizeItems =
        (unsigned int) sizeof (kItems) / sizeof (kItems[0]);
