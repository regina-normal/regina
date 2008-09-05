
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "packet/npdf.h"
#include "utilities/base64.h"
#include "utilities/xmlutils.h"

#define BASE64_LINE_LEN 76

namespace regina {

void NPDF::reset() {
    if (data_) {
        if (alloc_ == OWN_MALLOC)
            ::free(data_);
        else
            delete[] data_;
    }

    data_ = 0;
    size_ = 0;
    alloc_ = OWN_NEW;

    fireChangedEvent();
}

void NPDF::reset(char* data, size_t size, OwnershipPolicy alloc) {
    // Out with the old data.
    if (data_) {
        if (alloc_ == OWN_MALLOC)
            ::free(data_);
        else
            delete[] data_;
    }

    // In with the new.
    if (data) {
        data_ = data;
        size_ = size;

        if (alloc == DEEP_COPY) {
            data_ = static_cast<char*>(::malloc(size_));
            ::memcpy(data_, static_cast<const char*>(data), size_);
            alloc_ = OWN_MALLOC;
        } else
            alloc_ = alloc;
    } else {
        data_ = 0;
        size_ = 0;
        alloc_ = OWN_NEW;
    }

    fireChangedEvent();
}

void NPDF::writePacket(NFile&) const {
    // I have absolutely no intention of supporting the old pre-2002 binary
    // format, especially since it is about to be removed from Regina entirely.
}

NPDF* NPDF::readPacket(NFile&, NPacket*) {
    // Likewise.  Since we don't support PDF packets in the old binary files,
    // just return an oops.
    return 0;
}

void NPDF::writeXMLPacketData(std::ostream& out) const {
    char* base64;
    size_t len64 = base64Encode(data_, size_, &base64);
    if (! base64) {
        out << "  <pdf encoding=\"null\"></pdf>\n";
        return;
    }

    out << "  <pdf encoding=\"base64\">\n";
    while (len64 > BASE64_LINE_LEN) {
        out.write(base64, BASE64_LINE_LEN);
        out << std::endl;

        base64 += BASE64_LINE_LEN;
        len64 -= BASE64_LINE_LEN;
    }
    if (len64 > 0) {
        out.write(base64, len64);
        out << std::endl;
    }
    out << "  </pdf>\n";

    delete[] base64;
}

} // namespace regina

