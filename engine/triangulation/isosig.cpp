
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include "triangulation/isosig.h"
#include "utilities/sigutils.h"

namespace regina {

int IsoSigPrintable::generation(const std::string& sig) {
    Base64Decoder dec(sig.begin(), sig.end()); // strips whitespace

    try {
        // Read the size of the first component.
        auto sizeAndWidth = dec.decodeSize();
        if (sizeAndWidth.first == 0)
            return 2; // empty triangulation; could be either generation

        // Look at the next character: this will tell us whether we have a
        // first-generation or second-generation signature.
        char next = dec.peek();
        if (next == 0)
            return 0;
        if ((dec.decode(next) & 3) == 3)
            return 2; // we see the marker bits 11
        else
            return 1;
    } catch (const InvalidInput&) {
        return 0;
    }
}

size_t IsoSigPrintable::componentSize(const std::string& sig) {
    Base64Decoder dec(sig.begin(), sig.end()); // strips whitespace
    try {
        return dec.decodeSize().first;
    } catch (const InvalidInput&) {
        throw InvalidArgument("Invalid signature");
    }
}

} // namespace regina

