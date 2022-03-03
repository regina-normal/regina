
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

    unsigned long rank = h2.rank();
    unsigned long dim = countTriangles();

    std::vector<VectorInt> dualBasis, primalBasis;
    dualBasis.reserve(dim);
    primalBasis.reserve(dim);
    for (unsigned long i = 0; i < rank; ++i) {
        dualBasis.emplace_back(h2.freeRep(i));
        primalBasis.emplace_back(map * dualBasis.back());
    }

    std::vector<bool> sign(dim);
    for (size_t i = 0; i < dim; ++i) {
        const auto emb = triangle(i)->front();

        if (emb.simplex()->orientation() > 0)
            sign[i] = emb.vertices().sign();
        else
            sign[i] = - emb.vertices().sign();
    }

    MatrixInt form(rank, rank);

    for (unsigned long i = 0; i < rank; ++i)
        for (unsigned long j = i; j < rank; ++j) {
            size_t intn = 0;
            for (unsigned long k = 0; k < dim; ++k) {
                Integer count = dualBasis[i][k] * primalBasis[j][k];
                if (count != 0) {
                    if (sign[k] > 0)
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
