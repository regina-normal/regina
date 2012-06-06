
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
#include "dim2/dim2isomorphism.h"
#include "dim2/dim2triangulation.h"
#include "triangulation/ngeneralisomorphism.tcc"

namespace regina {

// Instatiate all templates from the .tcc file.
template void NGeneralIsomorphism<2>::writeTextShort(std::ostream&) const;
template void NGeneralIsomorphism<2>::writeTextLong(std::ostream&) const;
template bool NGeneralIsomorphism<2>::isIdentity() const;
template NGeneralIsomorphism<2>::NGeneralIsomorphism(
    const NGeneralIsomorphism<2>&);
template Dim2Isomorphism*
    NGeneralIsomorphism<2>::randomInternal<Dim2Isomorphism>(unsigned);

Dim2Triangulation* Dim2Isomorphism::apply(
        const Dim2Triangulation* original) const {
    if (original->getNumberOfFaces() != nSimplices_)
        return 0;

    if (nSimplices_ == 0)
        return new Dim2Triangulation();

    Dim2Triangulation* ans = new Dim2Triangulation();
    Dim2Face** face = new Dim2Face*[nSimplices_];
    unsigned long p;
    int f;

    NPacket::ChangeEventSpan span(ans);
    for (p = 0; p < nSimplices_; ++p)
        face[p] = ans->newFace();

    for (p = 0; p < nSimplices_; ++p)
        face[simpImage_[p]]->setDescription(
            original->getFace(p)->getDescription());

    const Dim2Face *myFace, *adjFace;
    unsigned long adjFaceIndex;
    NPerm3 gluingPerm;
    for (p = 0; p < nSimplices_; ++p) {
        myFace = original->getFace(p);
        for (f = 0; f < 3; ++f)
            if ((adjFace = myFace->adjacentFace(f))) {
                // We have an adjacent face.
                adjFaceIndex = original->faceIndex(adjFace);
                gluingPerm = myFace->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjFaceIndex > p || (adjFaceIndex == p &&
                        gluingPerm[f] > f))
                    face[simpImage_[p]]->joinTo(facetPerm_[p][f],
                        face[simpImage_[adjFaceIndex]],
                        facetPerm_[adjFaceIndex] * gluingPerm *
                            facetPerm_[p].inverse());
            }
    }

    return ans;
}

void Dim2Isomorphism::applyInPlace(Dim2Triangulation* tri) const {
    if (tri->getNumberOfFaces() != nSimplices_)
        return;

    if (nSimplices_ == 0)
        return;

    Dim2Triangulation staging;
    NPacket::ChangeEventSpan span1(&staging);
    Dim2Face** face = new Dim2Face*[nSimplices_];
    unsigned long p;
    int f;

    for (p = 0; p < nSimplices_; ++p)
        face[p] = staging.newFace();

    for (p = 0; p < nSimplices_; ++p)
        face[simpImage_[p]]->setDescription(tri->getFace(p)->getDescription());

    const Dim2Face *myFace, *adjFace;
    unsigned long adjFaceIndex;
    NPerm3 gluingPerm;
    for (p = 0; p < nSimplices_; ++p) {
        myFace = tri->getFace(p);
        for (f = 0; f < 3; ++f)
            if ((adjFace = myFace->adjacentFace(f))) {
                // We have an adjacent face.
                adjFaceIndex = tri->faceIndex(adjFace);
                gluingPerm = myFace->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjFaceIndex > p || (adjFaceIndex == p &&
                        gluingPerm[f] > f))
                    face[simpImage_[p]]->joinTo(facetPerm_[p][f],
                        face[simpImage_[adjFaceIndex]],
                        facetPerm_[adjFaceIndex] * gluingPerm *
                            facetPerm_[p].inverse());
            }
    }

    NPacket::ChangeEventSpan span2(tri);
    tri->removeAllFaces();
    tri->swapContents(staging);
}

} // namespace regina

