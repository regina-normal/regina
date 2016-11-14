
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

#include "algebra/abeliangroup.h"
#include "manifold/handlebody.h"

namespace regina {

AbelianGroup* Handlebody::homology() const {
    AbelianGroup* ans = new AbelianGroup();
    if (nHandles)
        ans->addRank(nHandles);
    return ans;
}

std::ostream& Handlebody::writeName(std::ostream& out) const {
    if (nHandles == 0)
        out << "B3";
    else if (nHandles == 1) {
        if (orientable)
            out << "B2 x S1";
        else
            out << "B2 x~ S1";
    } else {
        if (orientable)
            out << "Handle-Or(" << nHandles << ')';
        else
            out << "Handle-Nor(" << nHandles << ')';
    }
    return out;
}

std::ostream& Handlebody::writeTeXName(std::ostream& out) const {
    if (nHandles == 0)
        out << "B^3";
    else if (nHandles == 1) {
        if (orientable)
            out << "B^2 \\times S^1";
        else
            out << "B^2 \\twisted S^1";
    } else {
        if (orientable)
            out << "\\mathit{Handle-Or}(" << nHandles << ')';
        else
            out << "\\mathit{Handle-Nor}(" << nHandles << ')';
    }
    return out;
}

} // namespace regina

