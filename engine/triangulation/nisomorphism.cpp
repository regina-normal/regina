
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
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"

namespace regina {

void NIsomorphism::writeTextShort(std::ostream& out) const {
    out << "Isomorphism between triangulations";
}

void NIsomorphism::writeTextLong(std::ostream& out) const {
    for (unsigned i = 0; i < nTetrahedra; i++)
        out << i << " -> " << tetImage(i) << " (" << facePerm(i) << ")\n";
}

bool NIsomorphism::isIdentity() const {
    for (unsigned t = 0; t < nTetrahedra; t++) {
        if (mTetImage[t] != static_cast<int>(t))
            return false;
        if (! facePerm(t).isIdentity())
            return false;
    }
    return true;
}

NTriangulation* NIsomorphism::apply(const NTriangulation* original) const {
    if (original->getNumberOfTetrahedra() != nTetrahedra)
        return 0;

    if (nTetrahedra == 0)
        return new NTriangulation();

    NTetrahedron** tet = new NTetrahedron*[nTetrahedra];
    unsigned long t;
    int f;

    for (t = 0; t < nTetrahedra; t++)
        tet[t] = new NTetrahedron();

    const NTetrahedron *myTet, *adjTet;
    unsigned long adjTetIndex;
    NPerm gluingPerm;
    for (t = 0; t < nTetrahedra; t++) {
        myTet = original->getTetrahedron(t);
        for (f = 0; f < 4; f++)
            if ((adjTet = myTet->adjacentTetrahedron(f))) {
                // We have an adjacent tetrahedron.
                adjTetIndex = original->tetrahedronIndex(adjTet);
                gluingPerm = myTet->adjacentGluing(f);

                // Make the gluing from one side only.
                if (adjTetIndex > t || (adjTetIndex == t &&
                        gluingPerm[f] > f))
                    tet[tetImage(t)]->joinTo(facePerm(t)[f],
                        tet[tetImage(adjTetIndex)],
                        facePerm(adjTetIndex) * gluingPerm *
                            facePerm(t).inverse());
            }
    }

    NTriangulation* ans = new NTriangulation();
    for (t = 0; t < nTetrahedra; t++)
        ans->addTetrahedron(tet[t]);
    return ans;
}

NIsomorphism* NIsomorphism::random(unsigned nTetrahedra) {
    NIsomorphismDirect* ans = new NIsomorphismDirect(nTetrahedra);

    // Randomly choose the destination tetrahedra.
    unsigned i;
    for (i = 0; i < nTetrahedra; i++)
        ans->mTetImage[i] = i;
    std::random_shuffle(ans->mTetImage, ans->mTetImage + nTetrahedra);

    // Randomly choose the individual permutations.
    for (i = 0; i < nTetrahedra; i++)
        ans->facePerm(i) = NPerm::S4[rand() % 24];

    return ans;
}

NIsomorphismDirect::NIsomorphismDirect(const NIsomorphism& cloneMe) :
        NIsomorphism(cloneMe.getSourceTetrahedra()),
        mFacePerm(cloneMe.getSourceTetrahedra()> 0 ?
            new NPerm[cloneMe.getSourceTetrahedra()] : 0) {
    for (unsigned i = 0; i < nTetrahedra; i++) {
        mTetImage[i] = cloneMe.tetImage(i);
        mFacePerm[i] = cloneMe.facePerm(i);
    }
}

NIsomorphismIndexed::NIsomorphismIndexed(const NIsomorphismIndexed& cloneMe) :
        NIsomorphism(cloneMe.nTetrahedra),
        mIndex(cloneMe.nTetrahedra > 0 ?
            new int[cloneMe.nTetrahedra] : 0) {
    for (unsigned i = 0; i < nTetrahedra; i++) {
        mTetImage[i] = cloneMe.mTetImage[i];
        mIndex[i] = cloneMe.mIndex[i];
    }
}

} // namespace regina

