
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

#include "census/ncensus.h"
#include "progress/nprogresstypes.h"
#include "utilities/nmiscutils.h"

#include <strstream.h>

void NCensus::matchFacePairs() {
    // Generate a list of joins (without permutations).
    // Only the lexicographically smallest in an isomorphism class is
    // desired.
    NTetFace trying(0,0);
    NTetFace oldTrying;
    NTetFace tmpFace;
    int boundaryFaces = 0;
    int usedFaces = 0;
    while (true) {
        // Have we been cancelled?
        if (progress && progress->isCancelled()) {
            // Just exit with what we've found.
            progress->setMessage("Cancelled.");
            progress->setFinished();
            return;
        }

        // Play nicely with the other children.
        yield();

        // INVARIANT: Face trying needs to be joined to something.
        // dest(trying) represents the last tried destination for the
        // join, and there is no reciprocal join from dest(trying) back
        // to trying.

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
        if (boundary.hasTrue()) {
            // We're interested in triangulations with boundary.
            if (nBdryFaces < 0) {
                // We don't care how many boundary faces.
                if (! boundary.hasFalse()) {
                    // We must have some boundary though.
                    if (boundaryFaces == 0 &&
                            usedFaces == 4 * (int)nTetrahedra - 2 &&
                            dest(trying).tet < (int)nTetrahedra)
                        dest(trying).setBoundary(nTetrahedra);
                }
            } else {
                // We're specific about the number of boundary faces.
                if (usedFaces - boundaryFaces + nBdryFaces ==
                        4 * (int)nTetrahedra &&
                        dest(trying).tet < (int)nTetrahedra)
                    // We've used our entire quota of non-boundary faces.
                    dest(trying).setBoundary(nTetrahedra);
            }
        }

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
                noDest((usedFaces / 4) + 1, 0) &&
                dest(trying).isBoundary(nTetrahedra)) {
            // Can't use the boundary; all we can do is push past the
            // end.
            dest(trying)++;
        }

        // Now check if after all that we've been pushed past the end.
        if (dest(trying).isPastEnd(nTetrahedra,
                (! boundary.hasTrue()) || boundaryFaces == nBdryFaces)) {
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
            if (dest(trying).isBoundary(nTetrahedra)) {
                usedFaces--;
                boundaryFaces--;
            } else {
                usedFaces -= 2;
                dest(dest(trying)) = dest(trying);
            }

            continue;
        }

        // Let's glue it up and head to the next free face!
        if (dest(trying).isBoundary(nTetrahedra)) {
            usedFaces++;
            boundaryFaces++;
        } else {
            usedFaces += 2;
            dest(dest(trying)) = trying;
        }

        oldTrying = trying;
        trying++;
        while (trying.tet < (int)nTetrahedra && ! noDest(trying))
            trying++;

        // Have we got a solution?
        if (trying.tet == (int)nTetrahedra) {
            // Deal with the solution!
            if (isCanonical()) {
                if (progress) {
                    // Update the current state of progress.
                    ostrstream msg;
                    tmpFace.tet = 0;
                    tmpFace.face = 0;
                    for (; tmpFace.tet < (int)nTetrahedra; tmpFace++) {
                        if (tmpFace.tet || tmpFace.face)
                            msg << ' ';
                        msg << tmpFace.tet << ':' << tmpFace.face << '-'
                            << dest(tmpFace).tet << ':'
                            << dest(tmpFace).face;
                    }
                    msg << ends;
                    progress->setMessage(msg.str());
                    msg.freeze(0);
                }

                selectGluingPerms();
                for_each(allAutomorphisms.begin(), allAutomorphisms.end(),
                    FuncDelete<NIsomorphismIndexed>());
                allAutomorphisms.clear();
            }

            // Head back down to the previous gluing and undo it, ready
            // for the next loop.
            trying = oldTrying;
            if (dest(trying).isBoundary(nTetrahedra)) {
                usedFaces--;
                boundaryFaces--;
            } else {
                usedFaces -= 2;
                dest(dest(trying)) = dest(trying);
            }
        } else {
            // Make sure we head forwards to the first feasible
            // destination.
            // Currently the destination is set to trying.
            // However, the destination should also be at least the
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
                            if (dest(trying).isBoundary(nTetrahedra))
                                dest(trying)--;
                        }
                        break;
                    }
            }
        }
    }

    if (progress) {
        progress->setMessage("Finished.");
        progress->setFinished();
    }
}

