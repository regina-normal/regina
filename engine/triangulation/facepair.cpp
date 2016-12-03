
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

FacePair::FacePair(int newFirst, int newSecond) {
    if (newFirst < newSecond) {
        first_ = newFirst;
        second_ = newSecond;
    } else {
        first_ = newSecond;
        second_ = newFirst;
    }
}

FacePair FacePair::complement() const {
    if (first_ > 1)
        return FacePair(0, 1);
    else if (first_ == 1)
        return (second_ == 2 ? FacePair(0, 3) : FacePair(0, 2));
    else if (second_ == 1)
        return FacePair(2, 3);
    else if (second_ == 2)
        return FacePair(1, 3);
    else
        return FacePair(1, 2);
}

void FacePair::operator ++ (int) {
    if (second_ < 3)
        second_++;
    else if (first_ < 3) {
        first_++;
        if (first_ < 3)
            second_ = first_ + 1;
    }
}

void FacePair::operator -- (int) {
    if (second_ > first_ + 1)
        second_--;
    else if (first_ > 0) {
        first_--;
        second_ = 3;
    } else
        second_ = 0;
}

} // namespace regina

