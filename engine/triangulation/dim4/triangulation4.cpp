
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

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "algebra/intersectionform.h"
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

    throw InvalidArgument("The given string could not be interpreted "
        "as representing a 4-dimensional triangulation");
}

long Triangulation<4>::eulerCharManifold() const {
    // Begin with V - E + F - T + P.
    // This call to eulerCharTri() also ensures that the skeleton has
    // been calculated.
    long ans = eulerCharTri();

    // Truncate any ideal vertices.
    if (vertexLinkSummary_ != 0) {
        // There *might* be ideal vertices.
        // (Actually, since this routine has validity as a precondition,
        // and since the skeleton has been computed by this point, we should
        // have vertexLinkSummary_ non-negative at this point, which means
        // there *are* ideal vertices.  But this guarantee is stronger than
        // we actually need here.)
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

Triangulation<4>::Triangulation(const Triangulation& src, bool cloneProps,
        bool cloneLocks) : TriangulationBase<4>(src, cloneProps, cloneLocks),
        vertexLinkSummary_(src.vertexLinkSummary_) /* always cloned */ {
    // For other properties, the user gets to decide:
    if (! cloneProps)
        return;

    prop_ = src.prop_;

    // We do not need to copy any properties that are computed on demand with
    // the rest of the skeleton; however, at the time of writing there
    // are no such properties in the Triangulation<4> class anyway.
}

void Triangulation<4>::clearAllProperties() {
    clearBaseProperties();

    if (! topologyLock_) {
        vertexLinkSummary_ = -1; /* not yet computed */
        prop_.H2_.reset();
    }
}

void Triangulation<4>::swap(Triangulation<4>& other) {
    if (&other == this)
        return;

    // We use a basic PacketChangeSpan here, not a richer ChangeAndClearSpan,
    // since we do not want to touch computed properties.  Our intention here
    // is to swap them, not clear them.
    PacketChangeSpan span1(*this);
    PacketChangeSpan span2(other);

    // Note: swapBaseData() calls Snapshottable::swap().
    swapBaseData(other);

    // Properties stored directly:
    std::swap(vertexLinkSummary_, other.vertexLinkSummary_);

    // Properties stored using std::... helper classes:
    prop_.H2_.swap(other.prop_.H2_);
}

IntersectionForm Triangulation<4>::intersectionForm() const {
    if (isEmpty())
        throw FailedPrecondition("Computing intersection form "
            "requires a non-empty triangulation.");
    if (! isValid())
        throw FailedPrecondition("Computing intersection form "
            "requires a valid triangulation.");
    if (! isOrientable())
        throw FailedPrecondition("Computing intersection form "
            "requires an orientable triangulation.");
    if (! isClosed())
        throw FailedPrecondition("Computing intersection form "
            "requires a closed triangulation.");

    MarkedAbelianGroup h2(dualBoundaryMap<2>(), dualBoundaryMap<3>());
    MatrixInt map = dualToPrimal<2>();

    size_t rank = h2.rank();
    size_t dim = countTriangles();

    std::vector<VectorInt> dualBasis, primalBasis;
    dualBasis.reserve(dim);
    primalBasis.reserve(dim);
    for (size_t i = 0; i < rank; ++i) {
        dualBasis.emplace_back(h2.freeRep(i));
        primalBasis.emplace_back(map * dualBasis.back());
    }

    std::vector<bool> sign(dim);
    for (size_t i = 0; i < dim; ++i) {
        const auto emb = triangle(i)->front();
        sign[i] = (emb.simplex()->orientation() == emb.vertices().sign());
    }

    MatrixInt form(rank, rank);

    for (size_t i = 0; i < rank; ++i)
        for (size_t j = i; j < rank; ++j) {
            for (size_t k = 0; k < dim; ++k) {
                Integer count = dualBasis[i][k] * primalBasis[j][k];
                if (count != 0) {
                    if (sign[k])
                        form.entry(i, j) += count;
                    else
                        form.entry(i, j) -= count;
                }
            }
            if (j != i)
                form.entry(j, i) = form.entry(i, j);
        }

    return IntersectionForm(std::move(form));
}

} // namespace regina
