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
    { "position" ,  ""       ,  BIT_VECTOR   },
    { "target"   ,  ""       ,  BIT_VECTOR   },
    { "roll"     ,  ""       ,  BIT_REAL     },
    };

const TemplateParameter kLight[] = {
    { "position" ,  ""       ,  BIT_VECTOR   },
    };

const TemplateParameter kPlane[] = {
    { "center"   ,  ""       ,  BIT_VECTOR                   },
    { "normal"   ,  ""       ,  BIT_VECTOR | BIT_CHECK_ZERO  },
    { "scale"    ,  ""       ,  BIT_REAL | BIT_CHECK_POSITIVE},
    { "color"    ,  "texture",  BIT_VECTOR                   },
    { "texture"  ,  "color"  ,  BIT_TEXT                     },
    };

const TemplateParameter kSphere[] = {
    { "position" ,  ""       ,  BIT_VECTOR                   },
    { "radius"   ,  ""       ,  BIT_REAL | BIT_CHECK_POSITIVE},
    { "axis"     ,  ""       ,  BIT_VECTOR | BIT_CHECK_ZERO  },
    { "color"    ,  "texture",  BIT_VECTOR                   },
    { "texture"  ,  "color"  ,  BIT_TEXT                     },
    };

const TemplateParameter kCylinder[] = {
    { "center"   ,  ""       ,  BIT_VECTOR                   },
    { "direction",  ""       ,  BIT_VECTOR | BIT_CHECK_ZERO  },
    { "radius"   ,  ""       ,  BIT_REAL | BIT_CHECK_POSITIVE},
    { "color"    ,  "texture",  BIT_VECTOR                   },
    { "texture"  ,  "color"  ,  BIT_TEXT                     },
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
