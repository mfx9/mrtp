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


Parser::Parser (char *filename) {
    ifstream config (filename);
    string line, tokens[MAX_TOKENS];
    unsigned int ntokens;

    if (config.is_open ()) {
        while (getline (config, line)) {
            /*
             * plane(x0 = 0.0000; y0 = 0.0000; z0 = 0.0000; A = 0.0000; B = 0.0000; C = 1.0000; color1 = WHITE; color2 = RED; texscale = 2.0000)
             * camera(x = 11.0000; y = 4.0000; z = 7.0000; lx = 0.0000; ly = 0.0000; lz = 0.0000; rot = 0.0000)
             * light(x = -3.0000; y = 3.0000; z = 10.0000)
             * cylinderxy(x0 = 0.0000; y0 = 0.0000; R = 1.0000; color = RED)
             * sphere(x0 = 7.0000; y0 = 0.0000; z0 = 0.6000; R = 0.6000; color = WHITE)
             * sphere(x0 = 6.8470; y0 = 1.4554; z0 = 0.6000; R = 0.6000; color = RED)
             *  (...)
             *
             */
            if (line.find ("camera") != string::npos) {
                ntokens = TokenizeLine (line, tokens);
            }

        }
        config.close ();
    }
    else {
        /* Warn that file cannot be opened. */
    }
}

Parser::~Parser () {
}

unsigned int Parser::TokenizeLine (string line, 
    string *tokens) {
}
