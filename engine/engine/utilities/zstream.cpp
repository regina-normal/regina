
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "utilities/zstream.h"

namespace regina {

const int ZBuffer::zEOF = EOF;

std::streamsize ZBuffer::xsgetn(char* s, std::streamsize n) {
    if (next == -1) {
        int ans = gzread(file, s, n);
        return (ans == -1 ? zEOF : ans);
    } else {
        *s = (char)next;
        int ans = gzread(file, s + 1, n - 1);
        next = -1;
        return (ans == -1 ? zEOF : ans + 1);
    }
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
