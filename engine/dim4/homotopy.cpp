
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include "dim4/dim4triangulation.h"

namespace regina {

const NGroupPresentation& Dim4Triangulation::getFundamentalGroup() const {
    if (fundGroup_.known())
        return *fundGroup_.value();

    NGroupPresentation* ans = new NGroupPresentation();

    if (getNumberOfPentachora() == 0)
        return *(fundGroup_ = ans);

    ensureSkeleton();

    // Each non-boundary not-in-forest tetrahedron is a generator.
    // Each non-boundary triangle is a relation.
    long nBdryTets = 0;
    for (BoundaryComponentIterator bit = boundaryComponents_.begin();
            bit != boundaryComponents_.end(); ++bit)
        nBdryTets += (*bit)->tetrahedra_.size();

    // Cast away all unsignedness in case we run into problems subtracting.
    long nGens = static_cast<long>(tetrahedra_.size()) - nBdryTets
        - static_cast<long>(size())
        + static_cast<long>(countComponents());

    // Insert the generators.
    ans->addGenerator(nGens);

    // Find out which tetrahedron corresponds to which generator.
    long* genIndex = new long[tetrahedra_.size()];
    long i = 0;
    for (TetrahedronIterator tit = tetrahedra_.begin();
            tit != tetrahedra_.end(); ++tit)
        if (! ((*tit)->isBoundary() || (*tit)->inMaximalForest()))
            genIndex[tit - tetrahedra_.begin()] = i++;

    // Run through each triangle and insert the corresponding relations.
    std::deque<Dim4TriangleEmbedding>::const_iterator embit;
    Dim4Pentachoron* pent;
    int facet;
    Dim4Tetrahedron* tet;
    NGroupExpression* rel;
    for (TriangleIterator fit = triangles_.begin(); fit != triangles_.end();
            ++fit) {
        if ((*fit)->isBoundary())
            continue;

        // Put in the relation corresponding to this triangle.
        rel = new NGroupExpression();
        for (embit = (*fit)->emb_.begin();
                embit != (*fit)->emb_.end(); ++embit) {
            pent = (*embit).getPentachoron();
            facet = (*embit).getVertices()[3];

            tet = pent->tet_[facet];
            if (tet->inMaximalForest())
                continue;

            // We define the "direction" for this dual edge to point
            // from embedding tet->emb_[0] to embedding tet->emb_[1].
            //
            // Test whether we are traversing this dual edge forwards or
            // backwards as we walk around the triangle (*fit).
            if ((tet->emb_[0].getPentachoron() == pent) &&
                    (tet->emb_[0].getTetrahedron() == facet))
                rel->addTermLast(genIndex[tet->markedIndex()], 1);
            else
                rel->addTermLast(genIndex[tet->markedIndex()], -1);
        }
        ans->addRelation(rel);
    }

    // Tidy up.
    delete[] genIndex;
    ans->intelligentSimplify();

    return *(fundGroup_ = ans);
}

} // namespace regina
