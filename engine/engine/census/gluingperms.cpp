
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

#include <strstream>

#include "census/ncensus.h"
#include "progress/nprogresstypes.h"

void NCensus::selectGluingPerms() {
    NTetFace face(0, 0);
    if (dest(face).isBoundary(nTetrahedra)) {
        // There are no permutations to choose!
        trySolution();
        return;
    }

    orientation[0] = 1;

    NPerm gluing;
    while (! face.isBeforeStart()) {
        // Have we been cancelled?
        if (progress && progress->isCancelled())
            return;

        // Play nicely with the other children.
        yield();

        // When moving to the next permutation, be sure to preserve the
        // orientation of the permutation if necessary.
        if (orientability.hasFalse() || dest(face).face == 0)
            joinPermIndex(face)++;
        else
            joinPermIndex(face) += 2;

        if (joinPermIndex(face) >= 6) {
            // Out of ideas for this face.
            // Head back down to the previous face.
            joinPermIndex(face) = -1;
            if (tet[face.tet]->getAdjacentTetrahedron(face.face))
                tet[face.tet]->unjoin(face.face);
            face--;
            while ((! face.isBeforeStart()) &&
                    (dest(face).isBoundary(nTetrahedra) || dest(face) < face))
                face--;
            continue;
        }

        // We are sitting on a new permutation to try.
        gluing = NPerm(dest(face).face, 3) * allPermsS3[joinPermIndex(face)] *
            NPerm(face.face, 3);
        if (tet[face.tet]->getAdjacentTetrahedron(face.face))
            tet[face.tet]->unjoin(face.face);
        tet[face.tet]->joinTo(face.face, tet[dest(face).tet], gluing);

        // Fix the orientation if appropriate.
        if (dest(face).face == 0) {
            // It's the first time we've hit this tetrahedron.
            if ((joinPermIndex(face) + (face.face == 3 ? 0 : 1) +
                    (dest(face).face == 3 ? 0 : 1)) % 2 == 0)
                orientation[dest(face).tet] = -orientation[face.tet];
            else
                orientation[dest(face).tet] = orientation[face.tet];
        }

        // Move on to the next face.
        face++;
        while (face.tet < (int)nTetrahedra &&
                (dest(face).isBoundary(nTetrahedra) || dest(face) < face))
            face++;

        // If we're at the end, try the solution and step back.
        if (face.tet == (int)nTetrahedra) {
            trySolution();

            // Back to the previous face.
            face--;
            while ((! face.isBeforeStart()) &&
                    (dest(face).isBoundary(nTetrahedra) || dest(face) < face))
                face--;
        } else if ((! orientability.hasFalse()) && dest(face).face > 0) {
            // Be sure to get the orientation right.
            if (orientation[face.tet] == orientation[dest(face).tet])
                joinPermIndex(face) = 1;
            else
                joinPermIndex(face) = 0;
            
            if ((face.face == 3 ? 0 : 1) + (dest(face).face == 3 ? 0 : 1) == 1)
                joinPermIndex(face) = (joinPermIndex(face) + 1) % 2;

            joinPermIndex(face) -= 2;
        }
    }
}

void NCensus::trySolution() {
    // Run through the automorphisms and check we are in canonical form
    // for the permutations.
    // Skip the first automorphism because this will always be the identity.
    std::list<NIsomorphismIndexed*>::iterator it = allAutomorphisms.begin();
    for (it++; it != allAutomorphisms.end(); it++) {
        // Have we been cancelled?
        if (progress && progress->isCancelled())
            return;

        // Play nicely with the other children.
        yield();

        if (cmpPermsWithPreImage(**it) > 0)
            return;
    }

    // Check properties of the triangulation.
    working.gluingsHaveChanged();
    if (! working.isValid())
        return;
    if (working.isIdeal()) {
        if (! finiteness.hasFalse())
            return;
    } else {
        if (! finiteness.hasTrue())
            return;
    }
    if ((! orientability.hasTrue()) && working.isOrientable())
        return;

    // Put it in the census!
    // Make sure it has a charming label.
    NPacket* item = new NTriangulation(working);
    ostrstream out;
    out << "Item " << whichSoln << '\0';
    item->setPacketLabel(parent->makeUniqueLabel(out.str()));
    out.freeze(0);

    parent->insertChildLast(item);
    whichSoln++;
}

int NCensus::cmpPermsWithPreImage(const NIsomorphism& automorph) {
    NTetFace faceDest, faceImage;
    NPerm myPerm, yourPerm;
    int order;
    for (NTetFace face(0, 0); face.tet < (int)nTetrahedra; face++) {
        faceDest = dest(face);
        faceImage = automorph[face];
        if (faceDest.isBoundary(nTetrahedra) || faceDest < face)
            continue;
        myPerm = tet[face.tet]->getAdjacentTetrahedronGluing(face.face);
        yourPerm = automorph.facePerm(faceDest.tet).inverse()
            * tet[faceImage.tet]->getAdjacentTetrahedronGluing(faceImage.face)
            * automorph.facePerm(face.tet);
        order = myPerm.compareWith(yourPerm);
        if (order < 0)
            return -1;
        else if (order > 0)
            return 1;
    }
    return 0;
}

