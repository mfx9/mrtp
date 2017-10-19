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
using namespace std;

#include <sstream>
#include <iomanip>
#include <string>
#include <ctime>

#ifdef _OPENMP
#include <omp.h>
#endif /* _OPENMP */

#include "world.hpp"
#include "parser.hpp"

/*
 * Default settings.
 */
#define DEFAULT_WIDTH     640
#define DEFAULT_HEIGHT    480
#define DEFAULT_FOV        70.0
#define DEFAULT_DISTANCE   60.0
#define DEFAULT_SHADOW      0.25
#define DEFAULT_MODEL     lightQuadratic

/*
 * Program limits.
 */
#define MIN_WIDTH     (DEFAULT_WIDTH  /  2)
#define MAX_WIDTH     (DEFAULT_WIDTH  * 10)
#define MIN_HEIGHT    (DEFAULT_HEIGHT /  2)
#define MAX_HEIGHT    (DEFAULT_HEIGHT * 10)
#define MIN_FOV          50.0
#define MAX_FOV         170.0

#ifdef _OPENMP
    #define DEFAULT_THREADS   1
    #define MIN_THREADS       0
    #define MAX_THREADS      64
#endif /* _OPENMP */

/*
 * Exit codes.
 */
enum ExitCode_t {exitOK, exitFail};


void HelpScreen (string program) {
    cout << "Usage: " << program << " [OPTION]... FILE" << endl;
    cout << "Options:\n"
            "    -h, --help\n"
            "      Print this help screen.\n\n"
            "    -q, --quiet\n"
            "      Supress all messages, except errors.\n\n"
            "    -r, --resolution\n"
            "      Resolution of the rendered image, for\n"
            "      example 640x480 (default), 1024x768, etc.\n\n"
            "    -o, --output\n"
            "      Filename for the rendered image, in PNG\n"
            "      format (default is \"output.png\").\n\n"
            "    -f, --fov\n"
            "      Field of vision, in degrees (default is 70).\n\n"
            "    -d, --distance\n"
            "      Distance to quench light (default is 60).\n\n"
            "    -m, --model\n"
            "      Light quenching model (none, linear, quadratic,\n"
            "      default is quadratic).\n\n"
            "    -s, --shadow\n"
            "      Shadow factor (default is 0.25).\n\n"
#ifdef _OPENMP
            "    -t, --threads\n"
            "      Number of parallel threads, for example 0 (auto),\n"
            "      1 (serial run, default), 2, 4, etc.\n\n"
#endif /* _OPENMP */
            "Example:\n";
    cout << "    " << program << " -r 1024x768 -o test.png test.inp" << endl;
}

int main (int argc, char **argv) {
    int     i;
    size_t  pos;
    bool    resok;
    string  text, next, foo, bar;
    stringstream convert;

    /* 
     * Modifiable parameters.
     */
    bool         quiet    =  false;
    string       input    =  "";
    string       output   =  "";
    double       fov      =  DEFAULT_FOV;
    double       distance =  DEFAULT_DISTANCE;
    double       shadow   =  DEFAULT_SHADOW;
    LightModel_t model    =  DEFAULT_MODEL;
    unsigned int width    =  DEFAULT_WIDTH;
    unsigned int height   =  DEFAULT_HEIGHT;
#ifdef _OPENMP
    unsigned int threads  =  DEFAULT_THREADS;
#endif /* _OPENMP */


    if (argc < 2) {
        HelpScreen (argv[0]);
        return exitFail;
    }
    for (i = 1; i < argc; i++) {
        text = argv[i]; 
        /*
         * Help screen.
         */
        if ((text == "-h") || (text == "--help")) {
            HelpScreen (argv[0]);
            return exitOK;
        }

        /*
         * Quiet mode.
         */
        else if ((text == "-q") || (text == "--quiet")) {
            quiet = true;
        }

        /*
         * Print version.
         */
        else if ((text == "-v") || (text == "--version")) {
            cout << "Version: XXX" << endl;
            return exitOK;
        }

        /*
         * Set resolution.
         */
        else if ((text == "-r") || (text == "--resolution")) {
            if (i == (argc - 1)) {
                cerr << "Resolution not given." << endl;
                return exitFail;
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
                    convert >> height;
                    if (!convert) {
                        resok = false;
                    }
                    convert.clear ();
                }
                if (resok) {
                    if ((width < MIN_WIDTH) || (width > MAX_WIDTH) ||
                        (height < MIN_HEIGHT) || (height > MAX_HEIGHT)) {
                        resok = false;
                    }
                }
            }
            if (!resok) {
                cerr << "Invalid resolution." << endl;
                return exitFail;
            }
        }

        /*
         * Set field of vision.
         */
        else if ((text == "-f") || (text == "--fov")) {
            if (i == (argc - 1)) {
                cerr << "Field of vision not given." << endl;
                return exitFail;
            }
            next = argv[++i];
            convert.str (next);
            convert >> fov;
            if (!convert) {
                cerr << "Unable to convert fov to double." << endl;
                return exitFail;
            }
            convert.clear ();
            if ((fov < MIN_FOV) || (fov > MAX_FOV)) {
                cerr << "Invalid fov." << endl;
                return exitFail;
            }
        }

        /*
         * Set output filename.
         */
        else if ((text == "-o") || (text == "--output")) {
            if (i == (argc - 1)) {
                cerr << "Output file not given." << endl;
                return exitFail;
            }
            output = argv[++i];
            /* Check for a valid filename. */
        }

        /*
         * Set light quenching model.
         */
        else if ((text == "-m") || (text == "--model")) {
            if (i == (argc - 1)) {
                cerr << "Light quenching model not given." << endl;
                return exitFail;
            }
            next = argv[++i];
            if (next == "none") {
                model = lightNone;
            }
            else if (next == "linear") {
                model = lightLinear;
            }
            else if (next == "quadratic") {
                model = lightQuadratic;
            }
            else {
                cerr << "Unsupported light model." << endl;
                return exitFail;
            }
        }

        /*
         * Set distance for light quenching model.
         */
        else if ((text == "-d") || (text == "--distance")) {
            if (i == (argc - 1)) {
                cerr << "Distance to quench light not given." << endl;
                return exitFail;
            }
            next = argv[++i];
            convert.str (next);
            convert >> distance;
            if (!convert) {
                cerr << "Unable to convert distance to double." << endl;
                return exitFail;
            }
        }

        /*
         * Set shadow factor.
         */
        else if ((text == "-s") || (text == "--shadow")) {
            if (i == (argc - 1)) {
                cerr << "Shadow factor not given." << endl;
                return exitFail;
            }
            next = argv[++i];
            convert.str (next);
            convert >> shadow;
            if (!convert) {
                cerr << "Unable to convert shadow to double." << endl;
                return exitFail;
            }
            if ((shadow < 0.0) || (shadow > 1.0)) {
                cerr << "Invalid shadow factor." << endl;
                return exitFail;
            }
        }

#ifdef _OPENMP
        /*
         * Set the number of threads.
         */
        else if ((text == "-t") || (text == "--threads")) {
            if (i == (argc - 1)) {
                cerr << "Number of threads not given." << endl;
                return exitFail;
            }
            next = argv[++i];
            convert.str (next);
            convert >> threads;
            if (!convert || (threads > MAX_THREADS)) {
                cerr << "Invalid number of threads." << endl;
                return exitFail;
            }
            if (!threads) {
                threads = omp_get_max_threads ();
            }
        }
#endif /* _OPENMP */

        /*
         * Get the input file.
         */
        else {
            if (text.at (0) == '-') {
                cerr << "Undefined option: \"" << text << "\"" << endl;
                return exitFail;
            }
            input = text;
            /* Check for a valid filename. */
        }
    }

    if (input == "") {
        cerr << "No input file." << endl;
        return exitFail;
    }

    if (output == "") {
        foo = input;
        pos = input.rfind (".inp");
        if (pos != string::npos) {
            foo = input.substr (0, pos);
        }
        output = foo + ".png";
    }

    /*
     * Parse the input file.
     */
    Parser parser (&input);
    parser.Parse ();
    if (parser.Status () != statusOK) {
        return exitFail;
    }
    if (!quiet) {
        i = parser.NumberEntries ();
        cout << "Parsing complete, created " << i << 
            " entries." << endl;
    }

    /*
     * Build a world.
     */
#ifdef _OPENMP
    World world (&parser, width, height, fov, distance, 
        shadow, model, threads);
#else
    World world (&parser, width, height, fov, distance, 
        shadow, model, 0);
#endif /* _OPENMP */
    world.Initialize ();
    if (!quiet) {
        cout << "Built world." << endl;
    }

    /*
     * Render.
     */
    unsigned int timeStart, timeStop;

    timeStart = clock ();
    if (!quiet) {
        cout << "Rendering..." << endl;
    }
    world.Render ();
    timeStop = clock ();

    /*
     * Finalize.
     */
    if (!quiet) {
        double timeUsed;

        timeUsed = double (timeStop - timeStart) / CLOCKS_PER_SEC;
#ifdef _OPENMP
        if (threads > 1) {
            /* Correct the CPU time for the number of threads. */
            timeUsed /= double (threads);
        }
#endif /* _OPENMP */
        cout << "OK. Elapsed time: " << setprecision (2) << 
            timeUsed << " sec" << endl;
    }
    world.WritePNG (&output);

    return exitOK;
}
