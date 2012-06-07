
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
    if (original->getNumberOfTriangles() != nSimplices_)
        return 0;

    if (nSimplices_ == 0)
        return new Dim2Triangulation();

    Dim2Triangulation* ans = new Dim2Triangulation();
    Dim2Triangle** triangle = new Dim2Triangle*[nSimplices_];
    unsigned long p;
    int f;

    NPacket::ChangeEventSpan span(ans);
    for (p = 0; p < nSimplices_; ++p)
        triangle[p] = ans->newTriangle();

    for (p = 0; p < nSimplices_; ++p)
        triangle[simpImage_[p]]->setDescription(
            original->getTriangle(p)->getDescription());

    const Dim2Triangle *myTri, *adjTri;
    unsigned long adjTriIndex;
    NPerm3 gluingPerm;
    for (p = 0; p < nSimplices_; ++p) {
        myTri = original->getTriangle(p);
        for (f = 0; f < 3; ++f)
            if ((adjTri = myTri->adjacentTriangle(f))) {
                // We have an adjacent triangle.
                adjTriIndex = original->triangleIndex(adjTri);
                gluingPerm = myTri->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjTriIndex > p || (adjTriIndex == p &&
                        gluingPerm[f] > f))
                    triangle[simpImage_[p]]->joinTo(facetPerm_[p][f],
                        triangle[simpImage_[adjTriIndex]],
                        facetPerm_[adjTriIndex] * gluingPerm *
                            facetPerm_[p].inverse());
            }
    }

    return ans;
}

void Dim2Isomorphism::applyInPlace(Dim2Triangulation* tri) const {
    if (tri->getNumberOfTriangles() != nSimplices_)
        return;

    if (nSimplices_ == 0)
        return;

    Dim2Triangulation staging;
    NPacket::ChangeEventSpan span1(&staging);
    Dim2Triangle** triangle = new Dim2Triangle*[nSimplices_];
    unsigned long p;
    int f;

    for (p = 0; p < nSimplices_; ++p)
        triangle[p] = staging.newTriangle();

    for (p = 0; p < nSimplices_; ++p)
        triangle[simpImage_[p]]->setDescription(tri->getTriangle(p)->
            getDescription());

    const Dim2Triangle *myTri, *adjTri;
    unsigned long adjTriIndex;
    NPerm3 gluingPerm;
    for (p = 0; p < nSimplices_; ++p) {
        myTri = tri->getTriangle(p);
        for (f = 0; f < 3; ++f)
            if ((adjTri = myTri->adjacentTriangle(f))) {
                // We have an adjacent triangle.
                adjTriIndex = tri->triangleIndex(adjTri);
                gluingPerm = myTri->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjTriIndex > p || (adjTriIndex == p &&
                        gluingPerm[f] > f))
                    triangle[simpImage_[p]]->joinTo(facetPerm_[p][f],
                        triangle[simpImage_[adjTriIndex]],
                        facetPerm_[adjTriIndex] * gluingPerm *
                            facetPerm_[p].inverse());
            }
    }

    NPacket::ChangeEventSpan span2(tri);
    tri->removeAllTriangles();
    tri->swapContents(staging);
}

} // namespace regina

