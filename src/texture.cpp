/* File      : texture.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <cstring>

#include "png.hpp"
#include "texture.hpp"


namespace mrtp {

static const float kRealToByte = 255.0f;
static const float kByteToReal = 1.0f / kRealToByte;

TextureCollector textureCollector;


Texture::Texture(const char *path) { spath_ = path; }

/*
fracx, fracy are within a range of <0..1> and
define fractions of the x- and y-dimension
of a texture.
A reasonable scale for a 256x256 texture is 0.15.
*/
Pixel Texture::pick_pixel(float fracx, float fracy, float scale) {
    unsigned u = ((unsigned)(fracx * width_ * scale)) % width_;
    unsigned v = ((unsigned)(fracy * height_ * scale)) % height_;

    return data_[u + v * width_];
}

bool Texture::check_path(const char *path) {
    return strcmp(path, spath_.c_str()) == 0;
}

void Texture::load_texture() {
    png::image<png::rgb_pixel> image(spath_.c_str());

    width_ = image.get_width();
    height_ = image.get_height();
    data_.reserve(width_ * height_);

    for (int i = 0; i < height_; i++) {
        png::rgb_pixel *in = &image[i][0];

        for (int j = 0; j < width_; j++, in++) {
            Pixel out;
            out << (float)in->red, (float)in->green, (float)in->blue;
            out *= kByteToReal;
            data_.push_back(out);
        }
    }
}

/*
Adds a texture to a texture collector or reuses
one that already exists in the memory.
Returns a pointer to the texture.
*/
Texture *TextureCollector::add(const char *path) {
    for (std::list<Texture>::iterator t = textures_.begin(); t != textures_.end(); t++) {
        Texture *texture = &(*t);
        if (texture->check_path(path)) {
            return texture;
        }
    }
    Texture texture(path);
    textures_.push_back(texture);
    Texture *last = &textures_.back();
    last->load_texture();
    return last;
}

} //end namespace mrtp
