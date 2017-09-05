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
#include "utils.hpp"


bool TokenizeLine (string *line, string *tokens,
        unsigned int *ntokens, unsigned int maxtokens) {
    /*
     * Split a line to tokens.
     *
     * Remove redundant blank characters and comments.
     */
    char    c, pc;
    string  accu;
    unsigned int i, ti, len;

    ti  = 0;
    len = line->length ();

    if (len > 0) {
        accu = "";
        pc   = line->at (0);
        for (i = 0; i < len; i++) {
            c = line->at (i);
            if (IS_COMMENT (c)) {
                break;
            }
            if (IS_NOT_WHITE (c)) {
                accu += c;
            }
            if ((IS_WHITE (c) && IS_NOT_WHITE (pc))
                    || (i == (len - 1))) {
                if (ti == maxtokens) {
                    return false;
                }
                tokens[ti++] = accu;
                accu = "";
            }
            pc = c;
        }
        if (accu != "") {
            tokens[ti++] = accu;
        }
    }
    (*ntokens) = ti;
    return true;
}

bool ConvertTokens (const string *tokens, unsigned int ntokens, 
        double *out) {
    /*
     * Convert tokens to real numbers.
     */
    double        test;
    unsigned int  i;
    stringstream  convert;

    for (i = 0; i < ntokens; i++) {
        convert.str (tokens[i]);
        convert >> test;
        if (!convert) {
            return false;
        }
        out[i] = test;
        convert.clear ();
    }
    return true;
}

bool CheckFilename (string *in, string *out, string *ext) {
    /*
     * Check for a valid filename.
     */
    string filename = (*in);
    unsigned int position = filename.length () - 1;

    if ((filename.at (0) != '"') || (filename.at (position) != '"')) {
        return false;
    }
    string trim = filename.substr (1, position - 1);
    (*out) = trim;

    /*
     * Check for a valid extension.
     */
    if (ext != NULL) {
        string extension = trim.substr (position - 4, 3);
        if (extension != (*ext)) {
            return false;
        }
    }

    /*
     * Check if the file exists.
     */
    const char *text = trim.c_str ();
    ifstream file (text);

    if (!file.good ()) {
        return false;
    }
    return true;
}
