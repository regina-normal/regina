
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
#include "census/ngluingperms.h"
#include "triangulation/ntriangulation.h"
#include "utilities/stringutils.h"

namespace regina {

NGluingPerms::NGluingPerms(const NGluingPerms& cloneMe) :
        pairing(cloneMe.pairing), inputError_(false) {
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
                    (! tet[t]->adjacentTetrahedron(face)))
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
    return (std::find(NPerm::S3, NPerm::S3 + 6, permS3) - NPerm::S3);
}

int NGluingPerms::gluingToIndex(unsigned tet, unsigned face,
        const NPerm& gluing) const {
    NPerm permS3 = NPerm(pairing->dest(tet, face).face, 3) * gluing *
        NPerm(face, 3);
    return (std::find(NPerm::S3, NPerm::S3 + 6, permS3) - NPerm::S3);
}

void NGluingPerms::dumpData(std::ostream& out) const {
    // Assuming nTets < 100, estimated worst case (32 * nTets) bytes total.
    // Don't quote me on this.

    // (5 * 4 * nTets bytes)
    out << pairing->toTextRep() << std::endl;

    // (3 * 4 * nTets bytes)
    unsigned tet, face;
    for (tet = 0; tet < getNumberOfTetrahedra(); tet++)
        for (face = 0; face < 4; face++) {
            if (tet || face)
                out << ' ';
            out << permIndex(tet, face);
        }
    out << std::endl;
}

NGluingPerms::NGluingPerms(std::istream& in) :
        pairing(0), permIndices(0), inputError_(false) {
    // Remember that we can safely abort before allocating arrays, since C++
    // delete tests for nullness.
    std::string line;

    // Skip initial whitespace to find the face pairing.
    while (true) {
        std::getline(in, line);
        if (in.eof()) {
            inputError_ = true; return;
        }
        line = regina::stripWhitespace(line);
        if (line.length() > 0)
            break;
    }

    pairing = NFacePairing::fromTextRep(line);
    if (! pairing) {
        inputError_ = true; return;
    }

    unsigned nTets = pairing->getNumberOfTetrahedra();
    if (nTets == 0) {
        inputError_ = true; return;
    }

    permIndices = new int[nTets * 4];

    unsigned tet, face;
    for (tet = 0; tet < nTets; tet++)
        for (face = 0; face < 4; face++) {
            in >> permIndex(tet, face);
            // Don't test the range of permIndex(tet, face) since the
            // gluing permutation set could still be under construction.
        }

    // Did we hit an unexpected EOF?
    if (in.eof())
        inputError_ = true;
}

} // namespace regina

