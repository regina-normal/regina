
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

#include "utilities/nbooleans.h"

#include <iostream>

const char NBoolSet::eltTrue = 1;
const char NBoolSet::eltFalse = 2;
const NBoolSet NBoolSet::sNone;
const NBoolSet NBoolSet::sTrue(true);
const NBoolSet NBoolSet::sFalse(false);
const NBoolSet NBoolSet::sBoth(true, true);

std::ostream& operator << (std::ostream& out, const NBoolSet& set) {
    if (set == NBoolSet::sNone)
        out << "{ }";
    else if (set == NBoolSet::sTrue)
        out << "{ true }";
    else if (set == NBoolSet::sFalse)
        out << "{ false }";
    else
        out << "{ true, false }";
    return out;
}

