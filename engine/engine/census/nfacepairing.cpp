
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

#include <algorithm>
#include <sstream>
#include "census/nfacepairing.h"
#include "triangulation/npermit.h"
#include "utilities/memutils.h"

namespace regina {

namespace {
    /**
     * Holds the arguments passed to NFacePairing::findAllPairings().
     */
    struct NFacePairingArgs {
        NBoolSet boundary;
        int nBdryFaces;
        UseFacePairing use;
        void* useArgs;
    };
}

NFacePairing::NFacePairing(const NFacePairing& cloneMe) :
        nTetrahedra(cloneMe.nTetrahedra),
        pairs(new NTetFace[cloneMe.nTetrahedra]) {
    std::copy(cloneMe.pairs, cloneMe.pairs + (nTetrahedra * 4), pairs);
}

std::string NFacePairing::toString() const {
    std::ostringstream ans;

    for (NTetFace f(0, 0); ! f.isPastEnd(nTetrahedra, true); f++) {
        if (f.face == 0 && f.tet > 0)
            ans << " | ";
        else if (f.tet || f.face)
            ans << ' ';
        ans << dest(f).tet << ':' << dest(f).face;
    }
    return ans.str();
}

bool NFacePairing::findAllPairings(unsigned nTetrahedra,
        NBoolSet boundary, int nBdryFaces, UseFacePairing use,
        void* useArgs, bool newThread) {
    // Create a set of arguments.
    NFacePairingArgs* args = new NFacePairingArgs();
    args->boundary = boundary;
    args->nBdryFaces = nBdryFaces;
    args->use = use;
    args->useArgs = useArgs;

    // Start the face pairing generation.
    NFacePairing* pairing = new NFacePairing(nTetrahedra);
    if (newThread)
        return pairing->start(args, true);
    else {
        pairing->run(args);
        delete pairing;
        return true;
    }
}

void* NFacePairing::run(void* param) {
    NFacePairingArgs* args = (NFacePairingArgs*)param;

    // Bail if it's obvious that nothing will happen.
    if (args->boundary == NBoolSet::sNone || nTetrahedra == 0) {
        args->use(0, 0, args->useArgs);
        delete args;
        return 0;
    }
    if (args->boundary.hasTrue() && args->nBdryFaces >= 0 &&
            (args->nBdryFaces % 2 == 1 ||
            args->nBdryFaces > 2 * (int)nTetrahedra + 2
            || (args->nBdryFaces == 0 && ! args->boundary.hasFalse()))) {
        args->use(0, 0, args->useArgs);
        delete args;
        return 0;
    }

    // Initialise the pairings to unspecified (i.e., face -> itself).
    for (NTetFace f(0,0); f.tet < (int)nTetrahedra; f++)
        dest(f) = f;

    // Note that we have at least one tetrahedron.
    NTetFace trying(0, 0);
        /**< The face we're currently trying to match. */
    int boundaryFaces = 0;
        /**< How many (deliberately) unmatched faces do we currently have? */
    int usedFaces = 0;
        /**< How many faces have we already determined matchings for? */
    NFacePairingIsoList allAutomorphisms;
        /**< The set of all automorphisms of the current face pairing. */

    // Run through and find all possible matchings.
    NTetFace oldTrying, tmpFace;
    while (true) {
        // TODO: Check for cancellation,

        // Play nicely with the other children.
        yield();

        // INVARIANT: Face trying needs to be joined to something.
        // dest(trying) represents the last tried destination for the
        // join, and there is no reciprocal join from dest(trying) back
        // to trying.
        // The current value of dest(trying) is >= trying.

        // Move to the next destination.
        dest(trying)++;

        // If we're about to close off the current set of of tetrahedra
        // and it's not all the tetrahedra, we will have something
        // disconnected!
        // We will now avoid tying the last two faces in a set together,
        // and later we will avoid sending the last face of a set to the
        // boundary.
        if (usedFaces % 4 == 2 && usedFaces < 4 * (int)nTetrahedra - 2 &&
                noDest((usedFaces / 4) + 1, 0) &&
                dest(trying).tet <= (usedFaces / 4)) {
            // Move to the first unused tetrahedron.
            dest(trying).tet = (usedFaces / 4) + 1;
            dest(trying).face = 0;
        }

        // We'd better make sure we're not going to glue together so
        // many faces that there is no room for the required number of
        // boundary faces.
        if (args->boundary.hasTrue()) {
            // We're interested in triangulations with boundary.
            if (args->nBdryFaces < 0) {
                // We don't care how many boundary faces.
                if (! args->boundary.hasFalse()) {
                    // We must have some boundary though.
                    if (boundaryFaces == 0 &&
                            usedFaces == 4 * (int)nTetrahedra - 2 &&
                            dest(trying).tet < (int)nTetrahedra)
                        dest(trying).setBoundary(nTetrahedra);
                }
            } else {
                // We're specific about the number of boundary faces.
                if (usedFaces - boundaryFaces + args->nBdryFaces ==
                        4 * (int)nTetrahedra &&
                        dest(trying).tet < (int)nTetrahedra)
                    // We've used our entire quota of non-boundary faces.
                    dest(trying).setBoundary(nTetrahedra);
            }
        }

        // dest(trying) is now the first remaining candidate destination.
        // We still don't know whether this destination is valid however.
        while(true) {
            // Move onwards to the next free destination.
            while (dest(trying).tet < (int)nTetrahedra &&
                    ! noDest(dest(trying)))
                dest(trying)++;

            // If we are past face 0 of a tetrahedron and the previous face
            // was not used, we can't do anything with this tetrahedron.
            // Move to the next tetrahedron.
            if (dest(trying).tet < (int)nTetrahedra && dest(trying).face > 0 &&
                    noDest(dest(trying).tet, dest(trying).face - 1)) {
                dest(trying).tet++;
                dest(trying).face = 0;
                continue;
            }

            break;
        }

        // If we're still at an illegitimate destination, it must be
        // face 0 of a tetrahedron where the previous tetrahedron is
        // unused.  Note that face == 0 implies tet > 0.
        // In this case, we've passed the last sane choice; head
        // straight to the boundary.
        if (dest(trying).tet < (int)nTetrahedra && dest(trying).face == 0 &&
                noDest(dest(trying).tet - 1, 0))
            dest(trying).setBoundary(nTetrahedra);

        // Finally, return to the issue of prematurely closing off a
        // set of tetrahedra.  This time we will avoid sending the last
        // face of a set of tetrahedra to the boundary.
        if (usedFaces % 4 == 3 && usedFaces < 4 * (int)nTetrahedra - 1 &&
                noDest((usedFaces / 4) + 1, 0) && isUnmatched(dest(trying))) {
            // Can't use the boundary; all we can do is push past the
            // end.
            dest(trying)++;
        }

        // And so we're finally looking at the next real candidate for
        // dest(trying) that we know we're actually allowed to use.

        // Check if after all that we've been pushed past the end.
        if (dest(trying).isPastEnd(nTetrahedra,
                (! args->boundary.hasTrue()) ||
                boundaryFaces == args->nBdryFaces)) {
            // We can't join trying to anything else.  Step back.
            dest(trying) = trying;
            trying--;

            // Keep heading back until we find a face that joins
            // forwards or to the boundary.
            while (! trying.isBeforeStart()) {
                if (dest(trying) < trying)
                    trying--;
                else
                    break;
            }

            // Is the search over?
            if (trying.isBeforeStart())
                break;

            // Otherwise undo the previous gluing and prepare to loop
            // again trying the next option.
            if (isUnmatched(dest(trying))) {
                usedFaces--;
                boundaryFaces--;
            } else {
                usedFaces -= 2;
                dest(dest(trying)) = dest(trying);
            }

            continue;
        }

        // Let's match it up and head to the next free face!
        if (isUnmatched(dest(trying))) {
            usedFaces++;
            boundaryFaces++;
        } else {
            usedFaces += 2;
            dest(dest(trying)) = trying;
        }

        // Now we increment trying to move to the next unmatched face.
        oldTrying = trying;
        trying++;
        while (trying.tet < (int)nTetrahedra && ! noDest(trying))
            trying++;

        // Have we got a solution?
        if (trying.tet == (int)nTetrahedra) {
            // Deal with the solution!
            if (isCanonical(allAutomorphisms)) {
                args->use(this, &allAutomorphisms, args->useArgs);
                for_each(allAutomorphisms.begin(), allAutomorphisms.end(),
                    FuncDelete<NIsomorphismDirect>());
                allAutomorphisms.clear();
            }

            // Head back down to the previous gluing and undo it, ready
            // for the next loop.
            trying = oldTrying;
            if (isUnmatched(dest(trying))) {
                usedFaces--;
                boundaryFaces--;
            } else {
                usedFaces -= 2;
                dest(dest(trying)) = dest(trying);
            }
        } else {
            // We're about to start working on a new unmatched face.
            // Set dest(trying) to one step *before* the first feasible
            // destination.

            // Note that currently the destination is set to trying.

            // Ensure the destination is at least the
            // previous forward destination from an earlier face of this
            // tetrahedron.
            if (trying.face > 0) {
                tmpFace = trying;
                for (tmpFace--; tmpFace.tet == trying.tet; tmpFace--)
                    if (tmpFace < dest(tmpFace)) {
                        // Here is the previous forward destination in
                        // this tetrahedron.
                        if (dest(trying) < dest(tmpFace)) {
                            dest(trying) = dest(tmpFace);
                            
                            // Remember that dest(trying) will be
                            // incremented before it is used.  This
                            // should not happen if we're already on the
                            // boundary, so we need to move back one
                            // step so we will be pushed back onto the
                            // boundary.
                            if (isUnmatched(dest(trying)))
                                dest(trying)--;
                        }
                        break;
                    }
            }

            // If the first tetrahedron doesn't glue to itself and this
            // is not the first tetrahedron, it can't glue to itself either.
            // (Note that we already know there is at least 1 tetrahedron.)
            if (dest(trying).tet == trying.tet && dest(trying).face < 3 &&
                    trying.tet > 0)
                if (dest(0, 0).tet != 0)
                    dest(trying).face = 3;
        }
    }

    args->use(0, 0, args->useArgs);
    delete args;
    return 0;
}

bool NFacePairing::isCanonical(NFacePairingIsoList& list) const {
    // Create the automorphisms one tetrahedron at a time, selecting the
    // preimage of 0 first, then the preimage of 1 and so on.

    // ASSUMPTIONS:
    //
    // Throughout this routine we will rely on the fact that
    // findAllPairings() generates face pairings for which the images of
    // faces in an individual tetrahedron are strictly increasing.
    // We will also rely on the fact that the first appearances of a
    // forward link to each tetrahedron are in increasing order.

    // We want to cycle through all possible first face gluings, so we'll
    // special-case the situation in which there are no face gluings at all.
    if (isUnmatched(0, 0)) {
        // We must have just one tetrahedron with no face gluings at all.
        NIsomorphismDirect* ans;
        for (NPermItS4 it; ! it.done(); it++) {
            ans = new NIsomorphismDirect(1);
            ans->tetImage(0) = 0;
            ans->facePerm(0) = *it;
            list.push_back(ans);
        }
        return true;
    }

    // Now we know that face 0 of tetrahedron 0 is glued to something.

    NTetFace* image = new NTetFace[nTetrahedra * 4];
        /**< The automorphism currently under construction. */
    NTetFace* preImage = new NTetFace[nTetrahedra * 4];
        /**< The inverse of this automorphism. */

    unsigned i;
    for (i = 0; i < nTetrahedra * 4; i++) {
        image[i].setBeforeStart();
        preImage[i].setBeforeStart();
    }

    // Note that we know nTetrahedra >= 1.
    // For the preimage of face 0 of tetrahedron 0 we simply cycle
    // through all possibilities.
    const NTetFace firstFace(0, 0);
    const NTetFace firstFaceDest(dest(firstFace));
    NTetFace firstDestPre;
    NTetFace trying;
    NTetFace fImg, fPre;
    bool stepDown;
    int tet, face;
    for (preImage[0] = firstFace ; ! preImage[0].isPastEnd(nTetrahedra, true);
            preImage[0]++) {
        // Play nicely with the other children.
        yield();

        // Note that we know firstFace is not unmatched.
        if (isUnmatched(preImage[0]))
            continue;

        // If firstFace glues to the same tetrahedron and this face
        // doesn't, we can ignore this permutation.
        firstDestPre = dest(preImage[0]);
        if (firstFaceDest.tet == 0 && firstDestPre.tet != preImage[0].tet)
            continue;

        // If firstFace doesn't glue to the same tetrahedron but this
        // face does, we're not in canonical form.
        if (firstFaceDest.tet != 0 && firstDestPre.tet == preImage[0].tet) {
            for_each(list.begin(), list.end(),
                FuncDelete<NIsomorphismDirect>());
            list.clear();
            delete[] image;
            delete[] preImage;
            return false;
        }

        // We can use this face.  Set the corresponding reverse mapping
        // and off we go.
        image[preImage[0].tet * 4 + preImage[0].face] = firstFace;
        preImage[firstFaceDest.tet * 4 + firstFaceDest.face] = firstDestPre;
        image[firstDestPre.tet * 4 + firstDestPre.face] = firstFaceDest;

        // Step forwards to the next face whose preimage is undetermined.
        trying = firstFace;
        trying++;
        if (trying == firstFaceDest)
            trying++;
        while (! (trying == firstFace)) {
            // INV: We've successfully selected preimages for all faces
            // before trying.  We're currently looking at the last
            // attempted candidate for the preimage of trying.

            // Note that if preimage face A is glued to preimage face B
            // and the image of A is earlier than the image of B, then
            // the image of A will be selected whereas the image of B
            // will be automatically derived.

            // Play nicely with the other children.
            yield();

            stepDown = false;
            NTetFace& pre = preImage[trying.tet * 4 + trying.face];

            if (trying.isPastEnd(nTetrahedra, true)) {
                // We have a complete automorphism!
                list.push_back(new NIsomorphismDirect(nTetrahedra));
                // TODO: make a real automorphism.
                stepDown = true;
            } else {
                // Move to the next candidate.
                if (pre.tet >= 0 && pre.face == 3) {
                    // We're all out of candidates.
                    pre.setBeforeStart();
                    stepDown = true;
                } else {
                    if (pre.isBeforeStart()) {
                        // Which tetrahedron must we look in?
                        // Note that this tetrahedron will already have been
                        // determined.
                        pre.tet = preImage[trying.tet * 4].tet;
                        pre.face = 0;
                    } else
                        pre.face++;

                    // Step forwards until we have a preimage whose image
                    // has not already been set.
                    // If the preimage is unmatched and trying isn't,
                    // we'll also skip it.
                    // If trying is unmatched and the preimage isn't,
                    // we're not in canonical form.
                    for ( ; pre.face < 4; pre.face++) {
                        if (! image[pre.tet * 4 + pre.face].isBeforeStart())
                            continue;
                        if ((! isUnmatched(trying)) && isUnmatched(pre))
                            continue;
                        if (isUnmatched(trying) && (! isUnmatched(pre))) {
                            // We're not in canonical form.
                            for_each(list.begin(), list.end(),
                                FuncDelete<NIsomorphismDirect>());
                            list.clear();
                            delete[] image;
                            delete[] preImage;
                            return false;
                        }
                        break;
                    }
                    while (pre.face < 4 &&
                            ! image[pre.tet * 4 + pre.face].isBeforeStart())
                        pre.face++;
                    if (pre.face == 4) {
                        pre.setBeforeStart();
                        stepDown = true;
                    }
                }
            }

            if (! stepDown) {
                // We found a candidate.
                // We also know that trying is unmatched iff the preimage
                // is unmatched.
                image[pre.tet* 4 + pre.face] = trying;
                if (! isUnmatched(pre)) {
                    fPre = dest(pre);
                    if (image[fPre.tet * 4 + fPre.face].isBeforeStart()) {
                        // The image of fPre (the partner of the preimage
                        // face) can be determined at this point.
                        // Specifically, it should go into the next
                        // available slot.

                        // Do we already know which tetrahedron we should
                        // be looking into?
                        for (i = 0; i < 4; i++)
                            if (! image[fPre.tet * 4 + i].isBeforeStart()) {
                                // Here's the tetrahedron!
                                // Find the first available face.
                                tet = image[fPre.tet * 4 + i].tet;
                                for (face = 0; ! preImage[tet * 4 + face].
                                        isBeforeStart(); face++)
                                    ;
                                image[fPre.tet * 4 + fPre.face].tet = tet;
                                image[fPre.tet * 4 + fPre.face].face = face;
                                break;
                            }
                        if (i == 4) {
                            // We need to map to a new tetrahedron.
                            // Find the first available tetrahedron.
                            for (tet = trying.tet + 1;
                                    ! preImage[tet * 4].isBeforeStart(); tet++)
                                ;
                            image[fPre.tet * 4 + fPre.face].tet = tet;
                            image[fPre.tet * 4 + fPre.face].face = 0;
                        }

                        // Set the corresponding preimage.
                        fImg = image[fPre.tet * 4 + fPre.face];
                        preImage[fImg.tet * 4 + fImg.face] = fPre;
                    }
                }

                // Do a lexicographical comparison and shunt trying up
                // if need be.
                do {
                    fImg = dest(trying);
                    fPre = dest(preImage[trying.tet * 4 + trying.face]);
                    if (! fPre.isBoundary(nTetrahedra))
                        fPre = image[fPre.tet * 4 + fPre.face];

                    // Currently trying is glued to fImg.
                    // After applying our isomorphism, trying will be
                    // glued to fPre.

                    if (fImg < fPre) {
                        // This isomorphism will lead to a
                        // lexicographically greater representation.
                        // Ignore it.
                        stepDown = true;
                    } else if (fPre < fImg) {
                        // Whapow, we're not in canonical form.
                        for_each(list.begin(), list.end(),
                            FuncDelete<NIsomorphismDirect>());
                        list.clear();
                        delete[] image;
                        delete[] preImage;
                        return false;
                    }

                    // What we have so far is consistent with an automorphism.
                    trying++;
                } while (! (stepDown || trying.isPastEnd(nTetrahedra, true) ||
                        preImage[trying.tet * 4 + trying.face].isBeforeStart()));
            }

            if (stepDown) {
                // We're shunting trying back down.
                trying--;
                while (true) {
                    fPre = preImage[trying.tet * 4 + trying.face];
                    if (! isUnmatched(fPre)) {
                        fPre = dest(fPre);
                        if (image[fPre.tet * 4 + fPre.face] < trying) {
                            // This preimage/image was automatically
                            // derived.
                            trying--;
                            continue;
                        }
                    }
                    break;
                }

                // Note that this resetting of faces that follows will
                // also take place when trying makes it all the way back
                // down to firstFace.
                fPre = preImage[trying.tet * 4 + trying.face];
                image[fPre.tet * 4 + fPre.face].setBeforeStart();
                if (! isUnmatched(fPre)) {
                    fPre = dest(fPre);
                    fImg = image[fPre.tet * 4 + fPre.face];
                    preImage[fImg.tet * 4 + fImg.face].setBeforeStart();
                    image[fPre.tet * 4 + fPre.face].setBeforeStart();
                }
            }
        }
    }

    // The pairing is in canonical form and we have all our automorphisms.
    // Tidy up and return.
    delete[] image;
    delete[] preImage;
    return true;
}

} // namespace regina

