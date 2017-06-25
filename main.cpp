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
#include <iostream>
#include <sstream>
#include <string>
#include "world.hpp"
#include "parser.hpp"

using namespace std;


#define HELP_SPACING { cout << endl; }

/* Defaults. */
#define DEFAULT_OUTPUT  "output.png"
#define DEFAULT_WIDTH    640
#define DEFAULT_HEIGTH   480
#define DEFAULT_FOV       70.

/* Program limits. */
#define MIN_FOV        50.
#define MAX_FOV       170.
#define MIN_WIDTH     320
#define MAX_WIDTH    4096
#define MIN_HEIGTH    240
#define MAX_HEIGTH   3072

/* Exit codes. */
#define EXIT_OK    0
#define EXIT_FAIL  1


void HelpScreen (string program) {
    cout << "Usage: " << program << " [OPTION]... FILE" << endl;
    cout << "Options:\n"
            "    -h, --help\n"
            "      Print this help screen.\n\n"
            "    -q, --quiet\n"
            "      Supress all output.\n\n"
            "    -v, --version\n"
            "      Version of the program.\n\n"
            "    -r, --resolution\n"
            "      Resolution of the rendered image, for\n"
            "      example 640x480 (default), 1024x768, etc.\n\n"
            "    -f, --fov\n"
            "      Field of vision, in degrees (default is 70).\n\n"
            "    -o, --output\n"
            "      Filename for the rendered image, in PNG\n"
            "      format (default is \"output.png\").\n\n"
            "Example:\n";
    cout << "    " << program << " -r 1024x768 -o test.png test.txt" << endl;

    /*
    cout << "Options:" << endl;
    cout << "    -h, --help" << endl;
    cout << "      Print this help screen." << endl;
    HELP_SPACING;
    cout << "    -q, --quiet" << endl;
    cout << "      Supress all output." << endl;
    HELP_SPACING;
    cout << "    -v, --version" << endl;
    cout << "      Version of the program." << endl;
    HELP_SPACING;
    cout << "    -r, --resolution" << endl;
    cout << "      Resolution of the rendered image, for" << endl;
    cout << "      example " << DEFAULT_DISPLAY << " (default), 1024x768, etc." << endl;
    HELP_SPACING;
    cout << "    -f, --fov" << endl;
    cout << "      Field of vision, in degrees (default is " << DEFAULT_FOV << ")." << endl;
    HELP_SPACING;
    cout << "    -o, --output" << endl;
    cout << "      Filename for the rendered image, in PNG" << endl;
    cout << "      format (default is \"" << DEFAULT_OUTPUT << "\")." << endl;
    HELP_SPACING;
    cout << "Example:" << endl;
    cout << "    " << program << " -r 1024x768 -o test.png test.txt" << endl;
    */
}

int main (int argc, char **argv) {
    unsigned int i, len;
    string  text, next, foo, bar;
    stringstream convert ("test");
    size_t pos;
    bool   resok;
    char   c;

    /* 
     * Modifiable parameters.
     */
    bool    quiet  = false;
    string  input  = "";
    string  output = DEFAULT_OUTPUT;
    double  fov    = DEFAULT_FOV;
    unsigned int width = DEFAULT_WIDTH,
        heigth = DEFAULT_HEIGTH;

    if (argc < 2) {
        HelpScreen (argv[0]);
        return EXIT_FAIL;
    }
    for (i = 1; i < argc; i++) {
        text = argv[i]; 
        if ((text == "-h") || (text == "--help")) {
            HelpScreen (argv[0]);
            return EXIT_OK;
        }
        else if ((text == "-q") || (text == "--quiet")) {
            quiet = true;
        }
        else if ((text == "-v") || (text == "--version")) {
            cout << "Version: XXX" << endl;
            return EXIT_OK;
        }
        else if ((text == "-r") || (text == "--resolution")) {
            if (i == (argc - 1)) {
                cout << "Resolution not given." << endl;
                return EXIT_FAIL;
            }
            next  = argv[++i];
            pos   = next.find ('x');
            resok = false;
            if (pos != string::npos) {
                resok = true;
            }
            else {
                pos = next.find ('X');
                if (pos != string::npos) {
                    resok = true;
                }
            }
            if (resok) {
                foo = next.substr (0, pos);
                bar = next.substr (pos + 1, next.length () - pos - 1);
                convert.str (foo);
                convert >> width;
                if (!convert) {
                    resok = false;
                }
                else {
                    convert.clear ();
                    convert.str (bar);
                    convert >> heigth;
                    if (!convert) {
                        resok = false;
                    }
                    convert.clear ();
                }
                if ((width < MIN_WIDTH) || (width > MAX_WIDTH) ||
                    (heigth < MIN_HEIGTH) || (heigth > MAX_HEIGTH)) {
                    resok = false;
                }
            }
            if (!resok) {
                cout << "Invalid resolution." << endl;
                return EXIT_FAIL;
            }
        }
        else if ((text == "-f") || (text == "--fov")) {
            if (i == (argc - 1)) {
                cout << "Field of vision not given." << endl;
                return EXIT_FAIL;
            }
            next = argv[++i];
            convert.str (next);
            convert >> fov;
            if (!convert) {
                cout << "Unable to convert fov to double." << endl;
                return EXIT_FAIL;
            }
            convert.clear ();
            if ((fov < MIN_FOV) || (fov > MAX_FOV)) {
                cout << "Invalid fov." << endl;
                return EXIT_FAIL;
            }
        }
        else if ((text == "-o") || (text == "--output")) {
            if (i == (argc - 1)) {
                cout << "Output not given." << endl;
                return EXIT_FAIL;
            }
            output = argv[++i];
            /* Check for a valid filename. */
        }
        else {
            if (text.at (0) == '-') {
                cout << "Undefined option: \"" << text << "\"" << endl;
                return EXIT_FAIL;
            }
            input = text;
            /* Check for a valid filename. */
        }
    }

    if (input == "") {
        cout << "No input file." << endl;
        return EXIT_FAIL;
    }

    Parser parser (input);
    parser.Parse ();
    if (parser.GetStatus () != STATUS_OK) {
        cout << "Error parsing file: \"" << input 
            << "\"" << endl;
        return EXIT_FAIL;
    }

    World world (&parser, width, heigth, fov);
    world.Render ();
    world.WritePNG (output);

    return EXIT_OK;
}
