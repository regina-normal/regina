
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include "link/link.h"
#include "maths/matrix.h"
#include "maths/polynomial.h"

namespace regina {

/**
 * When building the crossing-by-region matrix for the Alexander polynomial,
 * we assign entries to regions around each crossing as follows:
 *
 *      ^
 *    t | -1
 *      |
 *  ---------
 *      |
 *   -t |  1
 *
 * If the same region appears twice around the same crossing, we add the two
 * corresponding terms.
 */

namespace {
    // The four entries in the diagram above, ordered in the way that
    // ModelLinkGraph orders arcs around each node.
    const Polynomial<Integer> alexanderCoeff[4] = { {0,1}, {-1}, {1}, {0,-1} };
}

const Polynomial<Integer>& Link::alexander() const {
    if (components_.size() != 1)
        throw FailedPrecondition("Regina can only compute Alexander "
            "polynomials for links with exactly one component");
    if (! isClassical())
        throw FailedPrecondition("Regina can only compute Alexander "
            "polynomials for classical knots, not virtual knots");

    if (alexander_.has_value())
        return *alexander_;

    if (size() == 0)
        return *(alexander_ = Polynomial<Integer>{1});

    // We build a matrix indexed by regions and cells.
    // We are required to ignore the columns for two adjacent cells; we will
    // make these the two cells immediately before the first crossing.
    //
    // Recall that, for each node in the model graph, arc 0 represents the
    // outgoing lower strand, and outgoing arcs are numbered 0,1,2,3 clockwise
    // around each node.

    Matrix<Polynomial<Integer>> m(size(), size());

    ModelLinkGraph graph(*this);
    const auto& cells = graph.cells();
    if (cells.genus() > 0)
        throw FailedPrecondition("Regina can only compute Alexander "
            "polynomials for knots in the 3-sphere");

    // We can start traversing the knot from any point, so we will start at
    // the lower strand from crossing 0.
    ModelLinkGraphNode* startNode = graph.node(0);
    size_t ignore[2] = { cells.cell(startNode->arc(2)),
        cells.cell(startNode->arc(3)) };
    if (ignore[0] > ignore[1])
        std::swap(ignore[0], ignore[1]);

    for (size_t i = 0; i < size(); ++i) {
        ModelLinkGraphNode* n = graph.node(i);

        for (int j = 0; j < 4; ++j) {
            size_t cell = cells.cell(n->arc(j));
            if (cell < ignore[0])
                m.entry(i, cell) += alexanderCoeff[j];
            else if (cell > ignore[0] && cell < ignore[1])
                m.entry(i, cell - 1) += alexanderCoeff[j];
            else if (cell > ignore[1])
                m.entry(i, cell - 2) += alexanderCoeff[j];
        }
    }

    auto ans = m.det();

    // Normalise by stripping out powers of t, and by making the constant
    // coefficient positive.
    if (! ans.isZero()) {
        size_t pow = 0;
        while (ans[pow] == 0)
            ++pow;
        if (pow > 0)
            ans.shift(-pow);

        if (ans[0] < 0)
            ans.negate();
    }

    return *(alexander_ = std::move(ans));
}

} // namespace regina

