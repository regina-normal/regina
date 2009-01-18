
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <cstdio>
#include <sys/stat.h>

#include "foreign/pdf.h"
#include "packet/npdf.h"

namespace regina {

NPDF* readPDF(const char* filename) {
    // Use FILE* so we can call fstat().

    // Open the file.
    FILE* in = fopen(filename, "rb");
    if (! in)
        return 0;

    // Get the file size.
    struct stat s;
    if (fstat(fileno(in), &s)) {
        fclose(in);
        return 0;
    }
    size_t size = s.st_size;

    if (size == 0) {
        fclose(in);
        return new NPDF();
    }

    // Read the file contents.
    char* data = new char[size];
    if (fread(data, 1, size, in) != size) {
        fclose(in);
        delete[] data;
        return 0;
    }

    // Is there more to the file that we weren't expecting?
    char c;
    if (fread(&c, 1, 1, in) > 0) {
        fclose(in);
        delete[] data;
        return 0;
    }

    // All good!
    fclose(in);
    return new NPDF(data, size, NPDF::OWN_NEW);
}

bool writePDF(const char* filename, const NPDF& pdf) {
    // Use FILE* for symmetry with readPDF().

    // Open the file.
    FILE* out = fopen(filename, "wb");
    if (!out)
        return false;

    // Is there anything to write?
    const char* data = pdf.data();
    if (data) {
        size_t size = pdf.size();
        if (fwrite(data, 1, size, out) != size) {
            fclose(out);
            return false;
        }
    }

    // All done.
    fclose(out);
    return true;
}

} // namespace regina
