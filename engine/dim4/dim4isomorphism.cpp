
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
#include "dim4/dim4isomorphism.h"
#include "dim4/dim4triangulation.h"

namespace regina {

void Dim4Isomorphism::writeTextShort(std::ostream& out) const {
    out << "Isomorphism between 4-manifold triangulations";
}

void Dim4Isomorphism::writeTextLong(std::ostream& out) const {
    for (unsigned i = 0; i < nPentachora_; ++i)
        out << i << " -> " << pentImage_[i] << " (" << facetPerm_[i] << ")\n";
}

bool Dim4Isomorphism::isIdentity() const {
    for (unsigned p = 0; p < nPentachora_; ++p) {
        if (pentImage_[p] != static_cast<int>(p))
            return false;
        if (! facetPerm_[p].isIdentity())
            return false;
    }
    return true;
}

Dim4Triangulation* Dim4Isomorphism::apply(
        const Dim4Triangulation* original) const {
    if (original->getNumberOfPentachora() != nPentachora_)
        return 0;

    if (nPentachora_ == 0)
        return new Dim4Triangulation();

    Dim4Pentachoron** pent = new Dim4Pentachoron*[nPentachora_];
    unsigned long p;
    int f;

    for (p = 0; p < nPentachora_; ++p)
        pent[p] = new Dim4Pentachoron();

    for (p = 0; p < nPentachora_; ++p)
        pent[pentImage_[p]]->setDescription(
            original->getPentachoron(p)->getDescription());

    const Dim4Pentachoron *myPent, *adjPent;
    unsigned long adjPentIndex;
    NPerm5 gluingPerm;
    for (p = 0; p < nPentachora_; ++p) {
        myPent = original->getPentachoron(p);
        for (f = 0; f < 5; ++f)
            if ((adjPent = myPent->adjacentPentachoron(f))) {
                // We have an adjacent pentachoron.
                adjPentIndex = original->pentachoronIndex(adjPent);
                gluingPerm = myPent->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjPentIndex > p || (adjPentIndex == p &&
                        gluingPerm[f] > f))
                    pent[pentImage_[p]]->joinTo(facetPerm_[p][f],
                        pent[pentImage_[adjPentIndex]],
                        facetPerm_[adjPentIndex] * gluingPerm *
                            facetPerm_[p].inverse());
            }
    }

    Dim4Triangulation* ans = new Dim4Triangulation();
    for (p = 0; p < nPentachora_; ++p)
        ans->addPentachoron(pent[p]);
    return ans;
}

void Dim4Isomorphism::applyInPlace(Dim4Triangulation* tri) const {
    if (tri->getNumberOfPentachora() != nPentachora_)
        return;

    if (nPentachora_ == 0)
        return;

    Dim4Pentachoron** pent = new Dim4Pentachoron*[nPentachora_];
    unsigned long p;
    int f;

    for (p = 0; p < nPentachora_; ++p)
        pent[p] = new Dim4Pentachoron();

    for (p = 0; p < nPentachora_; ++p)
        pent[pentImage_[p]]->setDescription(
            tri->getPentachoron(p)->getDescription());

    const Dim4Pentachoron *myPent, *adjPent;
    unsigned long adjPentIndex;
    NPerm5 gluingPerm;
    for (p = 0; p < nPentachora_; ++p) {
        myPent = tri->getPentachoron(p);
        for (f = 0; f < 5; ++f)
            if ((adjPent = myPent->adjacentPentachoron(f))) {
                // We have an adjacent pentachoron.
                adjPentIndex = tri->pentachoronIndex(adjPent);
                gluingPerm = myPent->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjPentIndex > p || (adjPentIndex == p &&
                        gluingPerm[f] > f))
                    pent[pentImage_[p]]->joinTo(facetPerm_[p][f],
                        pent[pentImage_[adjPentIndex]],
                        facetPerm_[adjPentIndex] * gluingPerm *
                            facetPerm_[p].inverse());
            }
    }

    // Don't do too many updates in quick succession, this can confuse
    // the UI badly...
    NPacket::ChangeEventBlock block(tri);
    tri->removeAllPentachora();
    for (p = 0; p < nPentachora_; ++p)
        tri->addPentachoron(pent[p]);
}

Dim4Isomorphism::Dim4Isomorphism(const Dim4Isomorphism& cloneMe) :
        ShareableObject(),
        nPentachora_(cloneMe.nPentachora_),
        pentImage_(cloneMe.nPentachora_ > 0 ?
            new int[cloneMe.nPentachora_] : 0),
        facetPerm_(cloneMe.nPentachora_ > 0 ?
            new NPerm5[cloneMe.nPentachora_] : 0) {
    std::copy(cloneMe.pentImage_, cloneMe.pentImage_ + nPentachora_,
        pentImage_);
    std::copy(cloneMe.facetPerm_, cloneMe.facetPerm_ + nPentachora_,
        facetPerm_);
}

Dim4Isomorphism* Dim4Isomorphism::random(unsigned nPentachora) {
    Dim4Isomorphism* ans = new Dim4Isomorphism(nPentachora);

    // Randomly choose the destination pentachora.
    unsigned i;
    for (i = 0; i < nPentachora; i++)
        ans->pentImage_[i] = i;
    std::random_shuffle(ans->pentImage_, ans->pentImage_ + nPentachora);

    // Randomly choose the individual permutations.
    for (i = 0; i < nPentachora; i++)
        ans->facetPerm_[i] = NPerm5::S5[rand() % 120];

    return ans;
}

} // namespace regina

