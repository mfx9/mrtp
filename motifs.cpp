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
#include "motifs.hpp"

using namespace std;


const MotifParameter kCamera[] = {
    { 1,  0,  "position",  "", BIT_VECTOR },
    { 2,  0,  "target",  "", BIT_VECTOR },
    { 3,  0,  "roll",  "0.0", BIT_REAL | BIT_OPTIONAL },
    };

const MotifParameter kLight[] = {
    { 1,  0,  "position",  "", BIT_VECTOR},
    };

const MotifParameter kPlane[] = {
    { 1,  0,  "center",  "", BIT_VECTOR },
    { 2,  0,  "normal",  "", BIT_VECTOR | BIT_CHECK_ZERO },
    { 3,  0,  "scale",  "", BIT_REAL | BIT_CHECK_POSITIVE },
    { 4,  0,  "reflect",  "0.0", BIT_REAL | BIT_CHECK_ZERO_ONE | BIT_OPTIONAL },
    { 5,  6,  "color",  "0.0  1.0  0.0", BIT_VECTOR },
    { 6,  5,  "texture",  "", BIT_TEXT },
    };

const MotifParameter kSphere[] = {
    { 1,  0, "position",  "", BIT_VECTOR },
    { 2,  0, "radius",  "", BIT_REAL | BIT_CHECK_POSITIVE },
    { 3,  0, "axis",  "0.0  0.0  1.0", BIT_VECTOR | BIT_CHECK_ZERO | BIT_OPTIONAL },
    { 4,  0, "reflect",  "0.0", BIT_REAL | BIT_CHECK_ZERO_ONE | BIT_OPTIONAL },
    { 5,  6, "color",  "0.0  1.0  0.0", BIT_VECTOR },
    { 6,  5, "texture",  "", BIT_TEXT },
    };

const MotifParameter kCylinder[] = {
    { 1,  0, "center",  "", BIT_VECTOR },
    { 2,  0, "direction",  "", BIT_VECTOR | BIT_CHECK_ZERO },
    { 3,  0, "radius",  "", BIT_REAL | BIT_CHECK_POSITIVE },
    { 4,  0, "span",  "-1.0", BIT_REAL | BIT_CHECK_ZERO | BIT_OPTIONAL },
    { 5,  0, "reflect",  "0.0", BIT_REAL | BIT_CHECK_ZERO_ONE | BIT_OPTIONAL },
    { 6,  7, "color",  "0.0  1.0  0.0", BIT_VECTOR },
    { 7,  6, "texture",  "", BIT_TEXT },
    };

const MotifEntry kEntries[] = {
    {entryCamera, "camera", true, 1, (unsigned int) (sizeof (kCamera) / sizeof (kCamera[0])), kCamera },

    {entryLight, "light", true, 1, (unsigned int) (sizeof (kLight) / sizeof (kLight[0])), kLight },

    {entryPlane, "plane", false, 0, (unsigned int) (sizeof (kPlane) / sizeof (kPlane[0])), kPlane },

    {entrySphere, "sphere", false, 0, (unsigned int) (sizeof (kSphere) / sizeof (kSphere[0])), kSphere },

    {entryCylinder, "cylinder", false, 0, (unsigned int) (sizeof (kCylinder) / sizeof (kCylinder[0])), kCylinder },
    };

const unsigned int kSizeEntries =
        (unsigned int) sizeof (kEntries) / sizeof (kEntries[0]);
