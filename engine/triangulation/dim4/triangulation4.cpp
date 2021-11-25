
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"
#include "utilities/xmlutils.h"

namespace regina {

Triangulation<4>::Triangulation(const std::string& description) {
    try {
        *this = fromIsoSig(description);
        return;
    } catch (const InvalidArgument&) {
    }
}

long Triangulation<4>::eulerCharManifold() const {
    // Begin with V - E + F - T + P.
    // This call to eulerCharTri() also ensures that the skeleton has
    // been calculated.
    long ans = eulerCharTri();

    // Truncate any ideal vertices.
    if (ideal_) {
        for (auto bc : boundaryComponents())
            if (bc->isIdeal()) {
                // Because our 4-manifold triangulation is valid, all
                // vertex links in the 3-manifold boundary must be
                // spheres or discs.  We can therefore use V - E + F - T
                // on this boundary component.
                ans += bc->vertex(0)->link_->eulerCharTri() - 1;
            }
    }

    return ans;
}

Triangulation<4>::Triangulation(const Triangulation& X, bool cloneProps) :
        TriangulationBase<4>(X, cloneProps),
        knownSimpleLinks_(X.knownSimpleLinks_) /* always cloned */ {
    // For other properties, the user gets to decide:
    if (! cloneProps)
        return;

    prop_ = X.prop_;

    // We do not need to copy skeletal properties (e.g., ideal_), since this
    // is computed on demand with the rest of the skeleton.
}

void Triangulation<4>::clearAllProperties() {
    clearBaseProperties();

    if (! topologyLock_) {
        knownSimpleLinks_ = false;
        prop_.H2_.reset();
    }
}

void Triangulation<4>::swap(Triangulation<4>& other) {
    if (&other == this)
        return;

    ChangeEventSpan span1(*this);
    ChangeEventSpan span2(other);

    // Note: swapBaseData() calls Snapshottable::swap().
    swapBaseData(other);

    // Properties stored directly:
    std::swap(knownSimpleLinks_, other.knownSimpleLinks_);
    std::swap(ideal_, other.ideal_);

    // Properties stored using std::... helper classes:
    prop_.H2_.swap(other.prop_.H2_);
}

} // namespace regina
