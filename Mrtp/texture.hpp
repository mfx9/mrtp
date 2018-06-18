/* File      : texture.hpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <list>
#include <vector>
#include <string>

#include "pixel.hpp"


class Texture {
    int width_;
    int height_;

    std::vector<Pixel> data_;

    std::string spath_;

public:
    Texture (const char *path);
    ~Texture ();
    void Load ();
    bool CheckPath (const char *path);
    Pixel PickPixel (float fracx, float fracy, float scale);
};

class TextureCollector {
    std::list<Texture> textures_;

public:
    Texture *Add (const char *path);
};


extern TextureCollector textureCollector;

#endif /* !_TEXTURE_H */
