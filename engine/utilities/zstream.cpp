
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#include <climits>
#include <cstdio>
#include "utilities/zstream.h"

namespace regina {

const int ZBuffer::zEOF = EOF;

std::streamsize ZBuffer::xsgetn(char* s, std::streamsize n) {
    std::streamsize read = 0;
    
    if (next != -1) {
        *s = static_cast<char>(next);
        next = -1;

        --n;
        ++s;
        read = 1;
    }
    
    int ret;
    while (n > 0) {
        // Be careful: std::streamsize could be a larger integer type than
        // zlib's unsigned int.
        ret = gzread(file, s,
            (n > UINT_MAX ? UINT_MAX : static_cast<unsigned>(n)));
        if (ret == -1)
            return zEOF;

        n -= ret;
        s += ret;
        read += ret;
    }
    
    return read;
}

int ZBuffer::close() {
    if (file) {
        int ans = gzclose(file);
        file = 0;
        return (ans == Z_OK ? 0 : zEOF);
    } else
        return 0;
}

int ZBuffer::open(const char* path, const char* mode) {
    if (file)
        close();
    file = gzopen(path, mode);
    next = -1;
    return (file ? 0 : zEOF);
}
    
void ZBuffer::showError(std::ostream& out) {
    if (file) {
        int err;
        out << "(De)compression error: " << gzerror(file, &err) << std::endl;
    } else
        out << "(De)compression error: No open file." << std::endl;
}

} // namespace regina
