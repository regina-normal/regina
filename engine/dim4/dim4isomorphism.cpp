
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

#include "dim4/dim4isomorphism.h"
#include "dim4/dim4triangulation.h"
#include "triangulation/ngeneralisomorphism.tcc"

namespace regina {

// Instatiate all templates from the .tcc file.
template void NGeneralIsomorphism<4>::writeTextShort(std::ostream&) const;
template void NGeneralIsomorphism<4>::writeTextLong(std::ostream&) const;
template bool NGeneralIsomorphism<4>::isIdentity() const;
template NGeneralIsomorphism<4>::NGeneralIsomorphism(
    const NGeneralIsomorphism<4>&);
template Dim4Isomorphism*
    NGeneralIsomorphism<4>::randomInternal<Dim4Isomorphism>(unsigned);

Dim4Triangulation* Dim4Isomorphism::apply(
        const Dim4Triangulation* original) const {
    if (original->getNumberOfPentachora() != nSimplices_)
        return 0;

    if (nSimplices_ == 0)
        return new Dim4Triangulation();

    Dim4Triangulation* ans = new Dim4Triangulation();
    Dim4Pentachoron** pent = new Dim4Pentachoron*[nSimplices_];
    unsigned long p;
    int f;

    NPacket::ChangeEventSpan span(ans);
    for (p = 0; p < nSimplices_; ++p)
        pent[p] = ans->newPentachoron();

    for (p = 0; p < nSimplices_; ++p)
        pent[simpImage_[p]]->setDescription(
            original->getPentachoron(p)->getDescription());

    const Dim4Pentachoron *myPent, *adjPent;
    unsigned long adjPentIndex;
    NPerm5 gluingPerm;
    for (p = 0; p < nSimplices_; ++p) {
        myPent = original->getPentachoron(p);
        for (f = 0; f < 5; ++f)
            if ((adjPent = myPent->adjacentPentachoron(f))) {
                // We have an adjacent pentachoron.
                adjPentIndex = original->pentachoronIndex(adjPent);
                gluingPerm = myPent->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjPentIndex > p || (adjPentIndex == p &&
                        gluingPerm[f] > f))
                    pent[simpImage_[p]]->joinTo(facetPerm_[p][f],
                        pent[simpImage_[adjPentIndex]],
                        facetPerm_[adjPentIndex] * gluingPerm *
                            facetPerm_[p].inverse());
            }
    }

    return ans;
}

void Dim4Isomorphism::applyInPlace(Dim4Triangulation* tri) const {
    if (tri->getNumberOfPentachora() != nSimplices_)
        return;

    if (nSimplices_ == 0)
        return;

    Dim4Triangulation staging;
    NPacket::ChangeEventSpan span1(&staging);
    Dim4Pentachoron** pent = new Dim4Pentachoron*[nSimplices_];
    unsigned long p;
    int f;

    for (p = 0; p < nSimplices_; ++p)
        pent[p] = staging.newPentachoron();

    for (p = 0; p < nSimplices_; ++p)
        pent[simpImage_[p]]->setDescription(
            tri->getPentachoron(p)->getDescription());

    const Dim4Pentachoron *myPent, *adjPent;
    unsigned long adjPentIndex;
    NPerm5 gluingPerm;
    for (p = 0; p < nSimplices_; ++p) {
        myPent = tri->getPentachoron(p);
        for (f = 0; f < 5; ++f)
            if ((adjPent = myPent->adjacentPentachoron(f))) {
                // We have an adjacent pentachoron.
                adjPentIndex = tri->pentachoronIndex(adjPent);
                gluingPerm = myPent->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjPentIndex > p || (adjPentIndex == p &&
                        gluingPerm[f] > f))
                    pent[simpImage_[p]]->joinTo(facetPerm_[p][f],
                        pent[simpImage_[adjPentIndex]],
                        facetPerm_[adjPentIndex] * gluingPerm *
                            facetPerm_[p].inverse());
            }
    }

    NPacket::ChangeEventSpan span2(tri);
    tri->removeAllPentachora();
    tri->swapContents(staging);
}

} // namespace regina

