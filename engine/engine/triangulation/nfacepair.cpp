
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "triangulation/nfacepair.h"

namespace regina {

NFacePair::NFacePair(int newFirst, int newSecond) {
    if (newFirst < newSecond) {
        first = newFirst;
        second = newSecond;
    } else {
        first = newSecond;
        second = newFirst;
    }
}

NFacePair NFacePair::complement() const {
    if (first > 1)
        return NFacePair(0, 1);
    else if (first == 1)
        return (second == 2 ? NFacePair(0, 3) : NFacePair(0, 2));
    else if (second == 1)
        return NFacePair(2, 3);
    else if (second == 2)
        return NFacePair(1, 3);
    else
        return NFacePair(1, 2);
}

void NFacePair::operator ++ (int) {
    if (second < 3)
        second++;
    else if (first < 3) {
        first++;
        if (first < 3)
            second = first + 1;
    }
}

void NFacePair::operator -- (int) {
    if (second > first + 1)
        second--;
    else if (first > 0) {
        first--;
        second = 3;
    } else
        second = 0;
}

} // namespace regina

