
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "algebra/nabeliangroup.h"
#include "dim4/dim4triangulation.h"
#include "maths/nmatrixint.h"

namespace regina {

// Placeholders until we can actually implement homology.

const NAbelianGroup& Dim4Triangulation::getHomologyH1() const {
    if (H1_.known())
        return *H1_.value();

    if (getNumberOfPentachora() == 0)
        return *(H1_ = new NAbelianGroup());

    // Calculate the first homology.
    if (! calculatedSkeleton_)
        calculateSkeleton();

    // Build a presentation matrix.
    // Each non-boundary not-in-forest tetrahedron is a generator.
    // Each non-boundary face is a relation.
    unsigned long nBdryFaces = 0;
    unsigned long nBdryTets = 0;
    for (BoundaryComponentIterator bit = boundaryComponents_.begin();
            bit != boundaryComponents_.end(); bit++) {
        nBdryFaces += (*bit)->faces_.size();
        nBdryTets += (*bit)->tetrahedra_.size();
    }
    long nGens = tetrahedra_.size() - nBdryTets
        - pentachora_.size() + components_.size();
    long nRels = faces_.size() - nBdryFaces;
    NMatrixInt pres(nRels, nGens);

    // Find out which tetrahedron corresponds to which generator.
    long* genIndex = new long[tetrahedra_.size()];
    long i = 0;
    for (TetrahedronIterator tit = tetrahedra_.begin();
            tit != tetrahedra_.end(); ++tit)
        if (! ((*tit)->isBoundary() || (*tit)->inDualMaximalForest()))
            genIndex[tit - tetrahedra_.begin()] = i++;

    // Run through each face and put the relations in the matrix.
    std::deque<Dim4FaceEmbedding>::const_iterator embit;
    Dim4Pentachoron* currPent;
    Dim4Tetrahedron* tet;
    int currPentFacet;
    long tetGenIndex;
    i = 0;
    for (FaceIterator fit = faces_.begin(); fit != faces_.end(); ++fit) {
        if ((*fit)->isBoundary())
            continue;

        // Put in the relation corresponding to this face.
        for (embit = (*fit)->emb_.begin();
                embit != (*fit)->emb_.end(); ++embit) {
            currPent = (*embit).getPentachoron();
            currPentFacet = (*embit).getVertices()[3];

            tet = currPent->tet_[currPentFacet];
            if (tet->inDualMaximalForest())
                continue;

            tetGenIndex = genIndex[tet->markedIndex()];
            if ((tet->emb_[0].getPentachoron() == currPent) &&
                    (tet->emb_[0].getTetrahedron() == currPentFacet))
                pres.entry(i, tetGenIndex) += 1;
            else
                pres.entry(i, tetGenIndex) -= 1;
        }

        ++i;
    }

    delete[] genIndex;

    // Build the group from the presentation matrix and tidy up.
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addGroup(pres);
    return *(H1_ = ans);
}

const NAbelianGroup& Dim4Triangulation::getHomologyH2() const {
    if (H2_.known())
        return *H2_.value();

    // TODO: Implement this..!

    return *(H2_ = new NAbelianGroup());
}

} // namespace regina
