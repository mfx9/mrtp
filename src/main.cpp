/* File      : main.cpp
 * Program   : mrtp
 * Copyright : Mikolaj Feliks  <mikolaj.feliks@gmail.com>
 * License   : LGPL v3  (http://www.gnu.org/licenses/gpl-3.0.en.html)
 */
#include <iostream>

#include "world.hpp"
#include "renderer.hpp"


int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Syntax: " << argv[0] << " scenefile.toml" << std::endl;
        return -1;
    }

    mrtp::World world(argv[1]);
    if (world.initialize() != mrtp::ws_ok) {
        std::cout << "Error initializing world"  << std::endl;
        return -2;
    }

    mrtp::Renderer renderer(&world, 640, 480, 93.0f, 60.0f, 0.25f, 0.001f, 3, 1);
    renderer.render_scene();
    renderer.write_scene("test.png");

    return 0;
}

