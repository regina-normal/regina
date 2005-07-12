
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include <algorithm>
#include "census/ngluingperms.h"
#include "triangulation/ntriangulation.h"

namespace regina {

NGluingPerms::NGluingPerms(const NGluingPerms& cloneMe) :
        pairing(cloneMe.pairing) {
    unsigned nTet = cloneMe.getNumberOfTetrahedra();

    permIndices = new int[nTet * 4];
    std::copy(cloneMe.permIndices, cloneMe.permIndices + nTet * 4, permIndices);
}

NTriangulation* NGluingPerms::triangulate() const {
    unsigned nTet = getNumberOfTetrahedra();

    NTriangulation* ans = new NTriangulation;
    NTetrahedron** tet = new NTetrahedron*[nTet];
    std::generate(tet, tet + nTet, FuncNew<NTetrahedron>());

    unsigned t, face;
    for (t = 0; t < nTet; t++)
        for (face = 0; face < 4; face++)
            if ((! pairing->isUnmatched(t, face)) &&
                    (! tet[t]->getAdjacentTetrahedron(face)))
                tet[t]->joinTo(face, tet[pairing->dest(t, face).tet],
                    gluingPerm(t, face));

    for (t = 0; t < nTet; t++)
        ans->addTetrahedron(tet[t]);

    delete[] tet;
    return ans;
}

int NGluingPerms::gluingToIndex(const NTetFace& source,
        const NPerm& gluing) const {
    NPerm permS3 = NPerm(pairing->dest(source).face, 3) * gluing *
        NPerm(source.face, 3);
    return (std::find(allPermsS3, allPermsS3 + 6, permS3) - allPermsS3);
}

int NGluingPerms::gluingToIndex(unsigned tet, unsigned face,
        const NPerm& gluing) const {
    NPerm permS3 = NPerm(pairing->dest(tet, face).face, 3) * gluing *
        NPerm(face, 3);
    return (std::find(allPermsS3, allPermsS3 + 6, permS3) - allPermsS3);
}

void NGluingPerms::dumpData(std::ostream& out) const {
    out << pairing->toTextRep() << std::endl;

    unsigned tet, face;
    for (tet = 0; tet < getNumberOfTetrahedra(); tet++)
        for (face = 0; face < 4; face++) {
            if (tet || face)
                out << ' ';
            out << permIndex(tet, face);
        }
    out << std::endl;
}

} // namespace regina

