
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

#include <sstream>
#include "census/ncensus.h"
#include "progress/nprogresstypes.h"

namespace regina {

void NCensus::selectGluingPerms(const NFacePairing* pairing,
        const NFacePairingIsoList* autos, void* census) {
    NCensus* realCensus = (NCensus*)census;
    if (pairing) {
        // We've found another face pairing.
        if (realCensus->progress)
            realCensus->progress->setMessage(pairing->toString());

        // Select the individual gluing permutations.
        realCensus->selectGluingPermsInternal(pairing, autos);
    } else {
        // Census generation has finished.
        if (realCensus->progress) {
            realCensus->progress->setMessage("Finished.");
            realCensus->progress->setFinished();
            delete realCensus;
        }
    }
}

void NCensus::selectGluingPermsInternal(const NFacePairing* pairing,
        const NFacePairingIsoList* autos) {
    NTetFace face(0, 0);
    if (pairing->dest(face).isBoundary(nTetrahedra)) {
        // There are no permutations to choose!
        trySolution(pairing, autos);
        return;
    }

    orientation[0] = 1;

    NPerm gluing;
    while (! face.isBeforeStart()) {
        // Have we been cancelled?
        if (progress && progress->isCancelled())
            return;

        // Play nicely with the other children.
        NThread::yield();

        // When moving to the next permutation, be sure to preserve the
        // orientation of the permutation if necessary.
        if (orientability.hasFalse() || pairing->dest(face).face == 0)
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
                    (pairing->dest(face).isBoundary(nTetrahedra) ||
                    pairing->dest(face) < face))
                face--;
            continue;
        }

        // We are sitting on a new permutation to try.
        gluing = NPerm(pairing->dest(face).face, 3) *
            allPermsS3[joinPermIndex(face)] * NPerm(face.face, 3);
        if (tet[face.tet]->getAdjacentTetrahedron(face.face))
            tet[face.tet]->unjoin(face.face);
        tet[face.tet]->joinTo(face.face, tet[pairing->dest(face).tet], gluing);

        // Fix the orientation if appropriate.
        if (pairing->dest(face).face == 0) {
            // It's the first time we've hit this tetrahedron.
            if ((joinPermIndex(face) + (face.face == 3 ? 0 : 1) +
                    (pairing->dest(face).face == 3 ? 0 : 1)) % 2 == 0)
                orientation[pairing->dest(face).tet] = -orientation[face.tet];
            else
                orientation[pairing->dest(face).tet] = orientation[face.tet];
        }

        // Move on to the next face.
        face++;
        while (face.tet < (int)nTetrahedra &&
                (pairing->dest(face).isBoundary(nTetrahedra) ||
                pairing->dest(face) < face))
            face++;

        // If we're at the end, try the solution and step back.
        if (face.tet == (int)nTetrahedra) {
            trySolution(pairing, autos);

            // Back to the previous face.
            face--;
            while ((! face.isBeforeStart()) &&
                    (pairing->dest(face).isBoundary(nTetrahedra) ||
                    pairing->dest(face) < face))
                face--;
        } else if ((! orientability.hasFalse()) &&
                pairing->dest(face).face > 0) {
            // Be sure to get the orientation right.
            if (orientation[face.tet] == orientation[pairing->dest(face).tet])
                joinPermIndex(face) = 1;
            else
                joinPermIndex(face) = 0;
            
            if ((face.face == 3 ? 0 : 1) +
                    (pairing->dest(face).face == 3 ? 0 : 1) == 1)
                joinPermIndex(face) = (joinPermIndex(face) + 1) % 2;

            joinPermIndex(face) -= 2;
        }
    }
}

void NCensus::trySolution(const NFacePairing* pairing,
        const NFacePairingIsoList* autos) {
    // Run through the automorphisms and check we are in canonical form
    // for the permutations.
    // Skip the first automorphism because this will always be the identity.
    std::list<NIsomorphismDirect*>::const_iterator it = autos->begin();
    for (it++; it != autos->end(); it++) {
        // Have we been cancelled?
        if (progress && progress->isCancelled())
            return;

        // Play nicely with the other children.
        NThread::yield();

        if (cmpPermsWithPreImage(pairing, **it) > 0)
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
    std::ostringstream out;
    out << "Item " << whichSoln;
    item->setPacketLabel(parent->makeUniqueLabel(out.str()));

    parent->insertChildLast(item);
    whichSoln++;
}

int NCensus::cmpPermsWithPreImage(const NFacePairing* pairing,
        const NIsomorphism& automorph) {
    NTetFace faceDest, faceImage;
    NPerm myPerm, yourPerm;
    int order;
    for (NTetFace face(0, 0); face.tet < (int)nTetrahedra; face++) {
        faceDest = pairing->dest(face);
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

} // namespace regina

