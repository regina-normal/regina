
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include <algorithm>
#include <cstdlib>
#include "triangulation/ngeneralisomorphism.tcc"
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

namespace regina {

// Instatiate all templates from the .tcc file.
template void NGeneralIsomorphism<3>::writeTextShort(std::ostream&) const;
template void NGeneralIsomorphism<3>::writeTextLong(std::ostream&) const;
template bool NGeneralIsomorphism<3>::isIdentity() const;
template NGeneralIsomorphism<3>::NGeneralIsomorphism(
    const NGeneralIsomorphism<3>&);
template NIsomorphism*
    NGeneralIsomorphism<3>::randomInternal<NIsomorphism>(unsigned);

NTriangulation* NIsomorphism::apply(const NTriangulation* original) const {
    if (original->getNumberOfTetrahedra() != nSimplices_)
        return 0;

    if (nSimplices_ == 0)
        return new NTriangulation();

    NTriangulation* ans = new NTriangulation();
    NTetrahedron** tet = new NTetrahedron*[nSimplices_];
    unsigned long t;
    int f;

    NPacket::ChangeEventSpan span(ans);
    for (t = 0; t < nSimplices_; t++)
        tet[t] = ans->newTetrahedron();

    for (t = 0; t < nSimplices_; t++)
        tet[simpImage_[t]]->setDescription(
            original->getTetrahedron(t)->getDescription());

    const NTetrahedron *myTet, *adjTet;
    unsigned long adjTetIndex;
    NPerm4 gluingPerm;
    for (t = 0; t < nSimplices_; t++) {
        myTet = original->getTetrahedron(t);
        for (f = 0; f < 4; f++)
            if ((adjTet = myTet->adjacentTetrahedron(f))) {
                // We have an adjacent tetrahedron.
                adjTetIndex = original->tetrahedronIndex(adjTet);
                gluingPerm = myTet->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjTetIndex > t || (adjTetIndex == t &&
                        gluingPerm[f] > f))
                    tet[simpImage_[t]]->joinTo(facetPerm_[t][f],
                        tet[simpImage_[adjTetIndex]],
                        facetPerm_[adjTetIndex] * gluingPerm *
                            facetPerm_[t].inverse());
            }
    }

    return ans;
}

void NIsomorphism::applyInPlace(NTriangulation* tri) const {
    if (tri->getNumberOfTetrahedra() != nSimplices_)
        return;

    if (nSimplices_ == 0)
        return;

    NTriangulation staging;
    NTetrahedron** tet = new NTetrahedron*[nSimplices_];
    unsigned long t;
    int f;

    NPacket::ChangeEventSpan span1(&staging);
    for (t = 0; t < nSimplices_; t++)
        tet[t] = staging.newTetrahedron();

    for (t = 0; t < nSimplices_; t++)
        tet[simpImage_[t]]->setDescription(
            tri->getTetrahedron(t)->getDescription());

    const NTetrahedron *myTet, *adjTet;
    unsigned long adjTetIndex;
    NPerm4 gluingPerm;
    for (t = 0; t < nSimplices_; t++) {
        myTet = tri->getTetrahedron(t);
        for (f = 0; f < 4; f++)
            if ((adjTet = myTet->adjacentTetrahedron(f))) {
                // We have an adjacent tetrahedron.
                adjTetIndex = tri->tetrahedronIndex(adjTet);
                gluingPerm = myTet->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjTetIndex > t || (adjTetIndex == t &&
                        gluingPerm[f] > f))
                    tet[simpImage_[t]]->joinTo(facetPerm_[t][f],
                        tet[simpImage_[adjTetIndex]],
                        facetPerm_[adjTetIndex] * gluingPerm *
                            facetPerm_[t].inverse());
            }
    }

    NPacket::ChangeEventSpan span2(tri);
    tri->removeAllTetrahedra();
    tri->swapContents(staging);
}

} // namespace regina

