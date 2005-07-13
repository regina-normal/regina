
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
#include <sstream>
#include "census/ncensus.h"
#include "census/ngluingpermsearcher.h"
#include "triangulation/ntriangulation.h"
#include "utilities/memutils.h"

namespace regina {

NGluingPermSearcher::NGluingPermSearcher(
        const NFacePairing* pairing, const NFacePairingIsoList* autos,
        bool orientableOnly, bool finiteOnly, int whichPurge,
        UseGluingPerms use, void* useArgs) :
        NGluingPerms(pairing), autos_(autos), autosNew(autos == 0),
        orientableOnly_(orientableOnly), finiteOnly_(finiteOnly),
        whichPurge_(whichPurge), use_(use), useArgs_(useArgs),
        started(false),
        orientation(new int[pairing->getNumberOfTetrahedra()]),
        currFace(0, 0) {
    // Generate the list of face pairing automorphisms if necessary.
    // This will require us to remove the const for a wee moment.
    if (autos_ == 0) {
        const_cast<NFacePairingIsoList*>(autos_) = new NFacePairingIsoList();
        pairing->findAutomorphisms(const_cast<NFacePairingIsoList&>(*autos));
    }

    // Initialise arrays.
    unsigned nTetrahedra = getNumberOfTetrahedra();

    std::fill(orientation, orientation + nTetrahedra, 0);
    std::fill(permIndices, permIndices + nTetrahedra * 4, -1);
}

NGluingPermSearcher::~NGluingPermSearcher() {
    delete[] orientation;
    if (autosNew) {
        // We made them, so we'd better remove the const again and
        // delete them.
        NFacePairingIsoList* autos = const_cast<NFacePairingIsoList*>(autos_);
        std::for_each(autos->begin(), autos->end(),
            FuncDelete<NIsomorphismDirect>());
        delete autos;
    }
}

void NGluingPermSearcher::findAllPerms(const NFacePairing* pairing,
        const NFacePairingIsoList* autos, bool orientableOnly,
        bool finiteOnly, int whichPurge, UseGluingPerms use, void* useArgs) {

    // Call an optimised internal generation routine if possible.
    if (pairing->getNumberOfTetrahedra() >= 3) {
        if (finiteOnly && pairing->isClosed() &&
                (whichPurge & NCensus::PURGE_NON_MINIMAL) &&
                (whichPurge & NCensus::PURGE_NON_PRIME) &&
                (orientableOnly ||
                    (whichPurge & NCensus::PURGE_P2_REDUCIBLE))) {
            // Closed prime minimal P2-irreducible triangulations with >= 3
            // tetrahedra.
            NClosedPrimeMinSearcher(pairing, autos, orientableOnly,
                use, useArgs).runSearch();
            return;
        }
    }

    NGluingPermSearcher(pairing, autos, orientableOnly, finiteOnly,
        whichPurge, use, useArgs).runSearch();
}

void NGluingPermSearcher::runSearch(long maxDepth) {
    // In this generation algorithm, each orientation is simply +/-1.

    unsigned nTetrahedra = getNumberOfTetrahedra();
    if (maxDepth < 0) {
        // Larger than we will ever see (and in fact grossly so).
        maxDepth = nTetrahedra * 4 + 1;
    }

    if (! started) {
        // Search initialisation.
        started = true;

        // Do we in fact have no permutation at all to choose?
        if (maxDepth == 0 || pairing->dest(0, 0).isBoundary(nTetrahedra)) {
            use_(this, useArgs_);
            use_(0, useArgs_);
            return;
        }

        currFace.setFirst();
        orientation[0] = 1;
    }

    long depth = 0;
    while (depth >= 0) {
        // TODO: Check for cancellation.

        // When moving to the next permutation, be sure to preserve the
        // orientation of the permutation if necessary.
        if ((! orientableOnly_) || pairing->dest(currFace).face == 0)
            permIndex(currFace)++;
        else
            permIndex(currFace) += 2;

        if (permIndex(currFace) >= 6) {
            // Out of ideas for this face.
            // Head back down to the previous face.
            permIndex(currFace) = -1;
            currFace--;
            while ((! currFace.isBeforeStart()) &&
                    (pairing->isUnmatched(currFace) ||
                        pairing->dest(currFace) < currFace)) {
                permIndex(currFace) = -1;
                currFace--;
            }
            depth--;
            continue;
        }

        // We are sitting on a new permutation to try.

        // Is this going to lead to an unwanted triangulation?
        if (mayPurge(currFace))
            continue;
        if (! orientableOnly_)
            if (badEdgeLink(currFace))
                continue;

        // Fix the orientation if appropriate.
        if (pairing->dest(currFace).face == 0) {
            // It's the first time we've hit this tetrahedron.
            if ((permIndex(currFace) + (currFace.face == 3 ? 0 : 1) +
                    (pairing->dest(currFace).face == 3 ? 0 : 1)) % 2 == 0)
                orientation[pairing->dest(currFace).tet] =
                    -orientation[currFace.tet];
            else
                orientation[pairing->dest(currFace).tet] =
                    orientation[currFace.tet];
        }

        // Move on to the next face.
        for (currFace++; currFace.tet < static_cast<int>(nTetrahedra);
                currFace++) {
            if (pairing->isUnmatched(currFace))
                continue;
            if (currFace < pairing->dest(currFace))
                break;

            // We've already decided on this gluing permutation; don't
            // forget to store the corresponding inverse permutation.
            permIndex(currFace) =
                allPermsS3Inv[permIndex(pairing->dest(currFace))];
        }
        depth++;

        // If we're at the end, try the solution and step back.
        if (currFace.tet == static_cast<int>(nTetrahedra)) {
            // We in fact have an entire triangulation.
            // Run through the automorphisms and check whether our
            // permutations are in canonical form.
            if (isCanonical())
                use_(this, useArgs_);

            // Back to the previous face.
            currFace--;
            while ((! currFace.isBeforeStart()) &&
                    (pairing->isUnmatched(currFace) ||
                        pairing->dest(currFace) < currFace)) {
                permIndex(currFace) = -1;
                currFace--;
            }
            depth--;
        } else {
            // Not a full triangulation; just one level deeper.
            if (orientableOnly_ && pairing->dest(currFace).face > 0) {
                // Be sure to get the orientation right.
                if (orientation[currFace.tet] ==
                        orientation[pairing->dest(currFace).tet])
                    permIndex(currFace) = 1;
                else
                    permIndex(currFace) = 0;

                if ((currFace.face == 3 ? 0 : 1) +
                        (pairing->dest(currFace).face == 3 ? 0 : 1) == 1)
                    permIndex(currFace) = (permIndex(currFace) + 1) % 2;

                permIndex(currFace) -= 2;
            }

            if (depth == maxDepth) {
                // We haven't found an entire triangulation, but we've
                // gone as far as we need to.
                // Process it, then step back.
                use_(this, useArgs_);

                // Back to the previous face.
                permIndex(currFace) = -1;
                currFace--;
                while ((! currFace.isBeforeStart()) &&
                        (pairing->isUnmatched(currFace) ||
                            pairing->dest(currFace) < currFace)) {
                    permIndex(currFace) = -1;
                    currFace--;
                }
                depth--;
            }
        }
    }

    // And the search is over.
    use_(0, useArgs_);
}

void NGluingPermSearcher::dumpData(std::ostream& out) const {
    // Assuming nTets < 100, estimated worst case (35 * nTets + 12) bytes total.
    // Don't quote me on this.
    NGluingPerms::dumpData(out);

    out << (orientableOnly_ ? 'o' : '.');
    out << (finiteOnly_ ? 'f' : '.');
    out << (started ? 's' : '.');
    out << ' ' << whichPurge_ << std::endl;

    for (unsigned t = 0; t < pairing->getNumberOfTetrahedra(); t++) {
        if (t)
            out << ' ';
        out << orientation[t];
    }
    out << std::endl;

    out << currFace.tet << ' ' << currFace.face << std::endl;
}

NGluingPermSearcher::NGluingPermSearcher(std::istream& in,
        UseGluingPerms use, void* useArgs) :
        NGluingPerms(in), autos_(0), autosNew(false),
        use_(use), useArgs_(useArgs), orientation(0), currFace(0, 0) {
    if (inputError_)
        return;

    // Recontruct the face pairing automorphisms.
    const_cast<NFacePairingIsoList*>(autos_) = new NFacePairingIsoList();
    pairing->findAutomorphisms(const_cast<NFacePairingIsoList&>(*autos_));
    autosNew = true;

    // Keep reading.
    char c;

    in >> c;
    if (c == 'o')
        orientableOnly_ = true;
    else if (c == '.')
        orientableOnly_ = false;
    else {
        inputError_ = true; return;
    }

    in >> c;
    if (c == 'f')
        finiteOnly_ = true;
    else if (c == '.')
        finiteOnly_ = false;
    else {
        inputError_ = true; return;
    }

    in >> c;
    if (c == 's')
        started = true;
    else if (c == '.')
        started = false;
    else {
        inputError_ = true; return;
    }

    in >> whichPurge_;

    unsigned nTets = pairing->getNumberOfTetrahedra();
    orientation = new int[nTets];
    for (unsigned t = 0; t < nTets; t++)
        in >> orientation[t];

    in >> currFace.tet >> currFace.face;

    // Did we hit an unexpected EOF?
    if (in.eof())
        inputError_ = true;
}

bool NGluingPermSearcher::isCanonical() const {
    NTetFace face, faceDest, faceImage;
    int order;

    for (std::list<NIsomorphismDirect*>::const_iterator it = autos_->begin();
            it != autos_->end(); it++) {
        // Compare the current set of gluing permutations with its
        // preimage under each face pairing automorphism, to see whether
        // our current permutation set is closest to canonical form.
        for (face.setFirst(); face.tet <
                static_cast<int>(pairing->getNumberOfTetrahedra()); face++) {
            faceDest = pairing->dest(face);
            if (pairing->isUnmatched(face) || faceDest < face)
                continue;

            faceImage = (**it)[face];
            order = gluingPerm(face).compareWith(
                (*it)->facePerm(faceDest.tet).inverse() * gluingPerm(faceImage)
                * (*it)->facePerm(face.tet));
            if (order < 0) {
                // This permutation set is closer.
                break;
            } else if (order > 0) {
                // The transformed permutation set is closer.
                return false;
            }

            // So far it's an automorphism of gluing permutations also.
            // Keep running through faces.
        }
        // Nothing broke with this automorphism.  On to the next one.
    }

    // Nothing broke at all.
    return true;
}

bool NGluingPermSearcher::badEdgeLink(const NTetFace& face) const {
    // Run around all three edges bounding the face.
    NTetFace adj;
    unsigned tet;
    NPerm current;
    NPerm start(face.face, 3);
    bool started, incomplete;
    for (unsigned permIdx = 0; permIdx < 3; permIdx++) {
        start = start * NPerm(1, 2, 0, 3);

        // start maps (0,1,2) to the three vertices of face, with
        // (0,1) mapped to the edge that we wish to examine.

        // Continue to push through a tetrahedron and then across a
        // face, until either we hit a boundary or we return to the
        // original face.

        current = start;
        tet = face.tet;

        started = false;
        incomplete = false;

        while ((! started) || (static_cast<int>(tet) != face.tet) ||
                (start[2] != current[2]) || (start[3] != current[3])) {
            // Test for a return to the original tetrahedron with the
            // orientation reversed; this either means a bad edge link
            // or a bad vertex link.
            if (started && finiteOnly_ && static_cast<int>(tet) == face.tet)
                if (start[3] == current[3] && start.sign() != current.sign())
                    return true;

            // Push through the current tetrahedron.
            started = true;
            current = current * NPerm(2, 3);

            // Push across a face.
            if (pairing->isUnmatched(tet, current[3])) {
                incomplete = true;
                break;
            }
            adj = pairing->dest(tet, current[3]);

            if (permIndex(tet, current[3]) >= 0) {
                current = gluingPerm(tet, current[3]) * current;
            } else if (permIndex(adj) >= 0) {
                current = gluingPerm(adj).inverse() * current;
            } else {
                incomplete = true;
                break;
            }

            tet = adj.tet;
        }

        // Did we meet the original edge in reverse?
        if ((! incomplete) && (start != current))
            return true;
    }

    // No bad edge links were found.
    return false;
}

bool NGluingPermSearcher::lowDegreeEdge(const NTetFace& face,
        bool testDegree12, bool testDegree3) const {
    // Run around all three edges bounding the face.
    NTetFace adj;
    unsigned tet;
    NPerm current;
    NPerm start(face.face, 3);
    bool started, incomplete;
    unsigned size;
    for (unsigned permIdx = 0; permIdx < 3; permIdx++) {
        start = start * NPerm(1, 2, 0, 3);

        // start maps (0,1,2) to the three vertices of face, with
        // (0,1) mapped to the edge that we wish to examine.

        // Continue to push through a tetrahedron and then across a
        // face, until either we hit a boundary or we return to the
        // original face.

        current = start;
        tet = face.tet;

        started = false;
        incomplete = false;
        size = 0;

        while ((! started) || (static_cast<int>(tet) != face.tet) ||
                (start[2] != current[2]) || (start[3] != current[3])) {
            started = true;

            // We're about to push through the current tetrahedron; see
            // if we've already exceeded the size of edge links that we
            // care about.
            if (size >= 3) {
                incomplete = true;
                break;
            }

            // Push through the current tetrahedron.
            current = current * NPerm(2, 3);

            // Push across a face.
            if (pairing->isUnmatched(tet, current[3])) {
                incomplete = true;
                break;
            }
            adj = pairing->dest(tet, current[3]);

            if (permIndex(tet, current[3]) >= 0) {
                current = gluingPerm(tet, current[3]) * current;
            } else if (permIndex(adj) >= 0) {
                current = gluingPerm(adj).inverse() * current;
            } else {
                incomplete = true;
                break;
            }

            tet = adj.tet;
            size++;
        }

        if (! incomplete) {
            if (testDegree12 && size < 3)
                return true;
            if (testDegree3 && size == 3) {
                // Only throw away a degree three edge if it involves
                // three distinct tetrahedra.
                int tet1 = pairing->dest(face.tet, start[2]).tet;
                int tet2 = pairing->dest(face.tet, start[3]).tet;
                if (face.tet != tet1 && tet1 != tet2 && tet2 != face.tet)
                    return true;
            }
        }
    }

    // No bad low-degree edges were found.
    return false;
}

bool NGluingPermSearcher::mayPurge(const NTetFace& face) const {
    // Are we allowed to purge on edges of degree 3?
    bool mayPurgeDeg3 = (whichPurge_ & NCensus::PURGE_NON_MINIMAL);

    // Are we allowed to purge on edges of degree 1 or 2?
    //
    // A 2-0 edge move or a 2-1 edge move can result in one or more of
    // the following topological changes.
    //
    // Bigon squashing:
    //   - Disc reduction;
    //   - Sphere decomposition or reduction;
    //   - Crushing embedded RP2 to an invalid edge.
    //
    // Pillow squashing:
    //   - Loss of 3-ball;
    //   - Loss of 3-sphere;
    //   - Loss of L(3,1).
    //
    bool mayPurgeDeg12 = (whichPurge_ & NCensus::PURGE_NON_MINIMAL) &&
        (whichPurge_ & NCensus::PURGE_NON_PRIME) &&
        ((whichPurge_ & NCensus::PURGE_P2_REDUCIBLE) || orientableOnly_) &&
        finiteOnly_ && (getNumberOfTetrahedra() > 2);

    if (mayPurgeDeg12 || mayPurgeDeg3)
        return lowDegreeEdge(face, mayPurgeDeg12, mayPurgeDeg3);
    else
        return false;
}

} // namespace regina

