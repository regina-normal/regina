
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include "triangulation/facepair.h"

namespace regina {

FacePair& FacePair::operator ++ () {
    // Codes: 0, 1, 2, 3, 6, 7, 11, 15.
    if (code_ < 3 || code_ == 6) // alt test: code & 3 != 3
        ++code_;
    else if (code_ > 6) // alt test: code & 12 != 0
        code_ += 4;
    else
        code_ = 6;
    return *this;
}

FacePair& FacePair::operator -- () {
    // Codes: 0, 1, 2, 3, 6, 7, 11, 15.
    if (code_ < 6 || code_ == 7)
        --code_;
    else if (code_ > 7)
        code_ -= 4;
    else
        code_ = 3;
    return *this;
}

} // namespace regina

