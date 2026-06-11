
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

#include <iomanip>
#include "utilities/bytesequence.h"

namespace regina {

std::ostream& operator << (std::ostream& out, const ByteSequence& bytes) {
    if (bytes.empty())
        return out;

    auto prevFlags = out.flags();
    char prevFill = out.fill();

    out << std::noshowbase << std::hex; // changes flags
    out.fill('0');

    // We need to cast to an integer type (to avoid character output), and in a
    // way that ensures that (for example) 0xff becomes 255 and not -1.  We
    // therefore cast twice: first to uint8_t, and then to unsigned int.
    bool sep = false;
    for (auto b : bytes) {
        if (sep)
            out << ':';
        else
            sep = true;
        out << std::setw(2) << static_cast<unsigned>(static_cast<uint8_t>(b));
    }

    out.flags(prevFlags);
    out.fill(prevFill);
    return out;
}

} // namespace regina

