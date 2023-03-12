
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <list>
#include <thread>
#include "enumerate/doubledescription.h"
#include "enumerate/hilbertdual.h"
#include "enumerate/hilbertprimal.h"
#include "hypersurface/normalhypersurfaces.h"
#include "maths/matrix.h"
#include "progress/progresstracker.h"
#include "triangulation/dim4.h"
#include "utilities/xmlutils.h"

namespace regina {

void NormalHypersurfaces::swap(NormalHypersurfaces& other) {
    if (std::addressof(other) == this)
        return;

    ChangeEventSpan span1(*this);
    ChangeEventSpan span2(other);

    surfaces_.swap(other.surfaces_);
    triangulation_.swap(other.triangulation_);
    std::swap(coords_, other.coords_);
    std::swap(which_, other.which_);
    std::swap(algorithm_, other.algorithm_);
}

void NormalHypersurfaces::writeTextShort(std::ostream& out) const {
    out << surfaces_.size();

    if (which_.has(regina::HS_EMBEDDED_ONLY))
        out << " embedded,";
    else if (which_.has(regina::HS_IMMERSED_SINGULAR))
        out << " embedded / immersed / singular,";
    else
        out << " unknown,";

    if (which_.has(regina::HS_VERTEX))
        out << " vertex";
    else if (which_.has(regina::HS_FUNDAMENTAL))
        out << " fundamental";
    else if (which_.has(regina::HS_CUSTOM))
        out << " custom";
    else if (which_.has(regina::HS_LEGACY))
        out << " legacy";
    else
        out << " unknown";

    out << " hypersurface";
    if (surfaces_.size() != 1)
        out << 's';
    out << " (" << HyperInfo::name(coords_) << ')';
}

void NormalHypersurfaces::writeTextLong(std::ostream& out) const {
    if (which_.has(regina::HS_EMBEDDED_ONLY))
        out << "Embedded,";
    else if (which_.has(regina::HS_IMMERSED_SINGULAR))
        out << "Embedded / immersed / singular,";
    else
        out << "Unknown,";

    if (which_.has(regina::HS_VERTEX))
        out << " vertex";
    else if (which_.has(regina::HS_FUNDAMENTAL))
        out << " fundamental";
    else if (which_.has(regina::HS_CUSTOM))
        out << " custom";
    else if (which_.has(regina::HS_LEGACY))
        out << " legacy";
    else
        out << " unknown";

    out << " hypersurfaces\n";

    out << "Coordinates: " << HyperInfo::name(coords_) << '\n';

    size_t n = surfaces_.size();
    out << "Number of hypersurfaces is " << n << '\n';
    for (const NormalHypersurface& s : surfaces_) {
        s.writeTextShort(out);
        out << '\n';
    }
}

bool NormalHypersurfaces::operator == (const NormalHypersurfaces& other) const {
    size_t n = surfaces_.size();
    if (n != other.surfaces_.size())
        return false;
    if (surfaces_.empty())
        return other.surfaces_.empty();
    if (other.surfaces_.empty())
        return false;

    // Both lists have the same size and are non-empty.
    // Our algorithm will be to sort and then compare.
    auto* lhs = new const NormalHypersurface*[n];
    auto* rhs = new const NormalHypersurface*[n];

    const NormalHypersurface** ptr = lhs;
    for (const auto& s : surfaces_)
        *ptr++ = std::addressof(s);

    ptr = rhs;
    for (const auto& s : other.surfaces_)
        *ptr++ = std::addressof(s);

    auto cmp = [](const NormalHypersurface* x, const NormalHypersurface* y) {
        return (*x) < (*y);
    };
    std::sort(lhs, lhs + n, cmp);
    std::sort(rhs, rhs + n, cmp);

    bool ans = std::equal(lhs, lhs + n, rhs,
            [](const NormalHypersurface* x, const NormalHypersurface* y) {
        return (*x) == (*y);
    });

    delete[] lhs;
    delete[] rhs;
    return ans;
}

} // namespace regina

