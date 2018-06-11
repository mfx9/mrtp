/* File      : texture.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <iostream>

#include "texture.hpp"
#include "png.hpp"

using namespace std;
using namespace png;

static const float kRealToByte = 255.0f;
static const float kByteToReal = 1.0f / kRealToByte;


/*
================
Texture
================
*/
Texture::Texture (const string &filename) {
    filename_ = filename;
}

/*
================
Texture

Copy-constructor
================
*/
Texture::Texture (const Texture &old) {
    width_ = old.width_;
    height_ = old.height_;
    filename_ = old.filename_;
    data_ = old.data_;
}

/*
================
~Texture
================
*/
Texture::~Texture () {
}

/*
================
PickPixel

fracx, fracy are within a range of <0..1> and
define fractions of the x- and y-dimension 
of a texture.

A reasonable scale for a 256x256 texture is 0.15.
================
*/
Pixel *Texture::PickPixel (float fracx, float fracy, float scale) {
    unsigned u = ((unsigned) (fracx * width_ * scale)) % width_;
    unsigned v = ((unsigned) (fracy * height_ * scale)) % height_;

    return &data_[u+v*width_];
}

/*
================
CheckFilename
================
*/
bool Texture::CheckFilename (const string &filename) {
    return (filename == filename_);
}

/*
================
Load

Allocates memory and loads pixel data from 
a PNG file
================
*/
void Texture::Load () {
    const char *filename = filename_.c_str ();
    image<rgb_pixel> image (filename);

    width_ = image.get_width ();
    height_ = image.get_height ();

    data_.reserve (width_ * height_);

    for (int i=0; i<height_; i++) {
        rgb_pixel *in = &image[i][0];

        for (int j=0; j<width_; j++, in++) {
            Pixel out;
            out << (float) in->red, (float) in->green, (float) in->blue;
            out *= kByteToReal;

            data_.push_back (out);
        }
    }
}
