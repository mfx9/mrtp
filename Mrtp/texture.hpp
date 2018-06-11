/* File      : texture.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>
#include <vector>

#include "pixel.hpp"


class Texture {
    int width_;
    int height_;

    std::string filename_;

    std::vector<Pixel> data_;

public:
    Texture (const std::string &filename);
    Texture (const Texture &old);
    ~Texture ();
    void Load ();
    bool CheckFilename (const std::string &filename);
    Pixel *PickPixel (float fracx, float fracy, float scale);
};

#endif /* !_TEXTURE_H */
