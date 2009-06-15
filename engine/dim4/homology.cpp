
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

#include "algebra/nabeliangroup.h"
#include "dim4/dim4triangulation.h"
#include "maths/matrixops.h"
#include "maths/nmatrixint.h"

namespace regina {

const NAbelianGroup& Dim4Triangulation::getHomologyH1() const {
    if (H1_.known())
        return *H1_.value();

    if (getNumberOfPentachora() == 0)
        return *(H1_ = new NAbelianGroup());

    // Calculate the first homology.
    if (! calculatedSkeleton_)
        calculateSkeleton();

    // Build a presentation matrix.
    // Each non-boundary not-in-forest tetrahedron is a generator.
    // Each non-boundary face is a relation.
    long nBdryTets = 0;
    long nBdryFaces = 0;
    for (BoundaryComponentIterator bit = boundaryComponents_.begin();
            bit != boundaryComponents_.end(); ++bit) {
        nBdryTets += (*bit)->tetrahedra_.size();
        nBdryFaces += (*bit)->faces_.size();
    }

    // Cast away all unsignedness in case we run into problems subtracting.
    long nGens = static_cast<long>(tetrahedra_.size()) - nBdryTets
        - static_cast<long>(pentachora_.size())
        + static_cast<long>(components_.size());
    long nRels = static_cast<long>(faces_.size()) - nBdryFaces;

    NMatrixInt pres(nRels, nGens);

    // Find out which tetrahedron corresponds to which generator.
    long* genIndex = new long[tetrahedra_.size()];
    long i = 0;
    for (TetrahedronIterator tit = tetrahedra_.begin();
            tit != tetrahedra_.end(); ++tit)
        if (! ((*tit)->isBoundary() || (*tit)->inDualMaximalForest()))
            genIndex[tit - tetrahedra_.begin()] = i++;

    // Run through each face and put the corresponding relations into
    // the matrix.
    std::deque<Dim4FaceEmbedding>::const_iterator embit;
    Dim4Pentachoron* pent;
    int facet;
    Dim4Tetrahedron* tet;
    i = 0;
    for (FaceIterator fit = faces_.begin(); fit != faces_.end(); ++fit) {
        if ((*fit)->isBoundary())
            continue;

        // Put in the relation corresponding to this face.
        for (embit = (*fit)->emb_.begin();
                embit != (*fit)->emb_.end(); ++embit) {
            pent = (*embit).getPentachoron();
            facet = (*embit).getVertices()[3];

            tet = pent->tet_[facet];
            if (tet->inDualMaximalForest())
                continue;

            // We define the "direction" for this dual edge to point
            // from embedding tet->emb_[0] to embedding tet->emb_[1].
            //
            // Test whether we are traversing this dual edge forwards or
            // backwards as we walk around the face (*fit).
            if ((tet->emb_[0].getPentachoron() == pent) &&
                    (tet->emb_[0].getTetrahedron() == facet))
                pres.entry(i, genIndex[tet->markedIndex()]) += 1;
            else
                pres.entry(i, genIndex[tet->markedIndex()]) -= 1;
        }

        ++i;
    }

    delete[] genIndex;

    // Build the group from the presentation matrix and tidy up.
    NAbelianGroup* ans = new NAbelianGroup();
    ans->addGroup(pres);
    return *(H1_ = ans);
}

const NAbelianGroup& Dim4Triangulation::getHomologyH2() const {
    if (H2_.known())
        return *H2_.value();

    if (getNumberOfPentachora() == 0)
        return *(H1_ = new NAbelianGroup());

    if (! calculatedSkeleton_)
        calculateSkeleton();

    // A placeholder implementation until the heavy machinery is ready.
    // This is correct, but inefficient (uses the full chain maps)
    // and wasteful (since the chain maps will almost certainly end up
    // implemented elsewhere).

    unsigned long i, j;
    unsigned long row, col;
    Dim4Pentachoron* pent;
    Dim4Edge* edge;
    Dim4Face* face;
    Dim4Tetrahedron* tet;
    NPerm5 perm, tmpPerm;
    int pentEdge, pentFace;

    unsigned long nEdges = getNumberOfEdges();
    unsigned long nFaces = getNumberOfFaces();
    unsigned long nTetrahedra = getNumberOfTetrahedra();

    // Build a translation table from edge numbers -> "internal edge" indices.
    unsigned long nEdgesInternal = 0;
    unsigned long* edgeInternalIndex = new unsigned long[nEdges];
    for (i = 0; i < nEdges; ++i)
        if (! getEdge(i)->isBoundary())
            edgeInternalIndex[i] = nEdgesInternal++;

    // Build a translation table from face numbers -> "internal face" indices.
    unsigned long nFacesInternal = 0;
    unsigned long* faceInternalIndex = new unsigned long[nFaces];
    for (i = 0; i < nFaces; ++i)
        if (! getFace(i)->isBoundary())
            faceInternalIndex[i] = nFacesInternal++;

    // Count the number of internal tetrahedra.
    unsigned long nTetrahedraInternal = 0;
    for (i = 0; i < nTetrahedra; ++i)
        if (! getTetrahedron(i)->isBoundary())
            ++nTetrahedraInternal;

    // --------------------------------------
    // Boundary map from 3-cycles to 2-cycles
    // --------------------------------------
    //
    // Rows: Internal dual polyhedra
    // Cols: Internal dual faces
    NMatrixInt bdry32(nEdgesInternal, nFacesInternal);

    // Build the boundary map, one dual face at a time.
    col = 0;
    for (i = 0; i < nFaces; ++i) {
        face = getFace(i);
        if (face->isBoundary())
            continue;

        // The dual face surrounding this face bounds the dual polyhedron
        // surrounding each of its edges.
        pent = face->getEmbedding(0).getPentachoron();
        perm = face->getEmbedding(0).getVertices();

        for (j = 0; j < 3; ++j) {
            // Edge j of the face is opposite vertex j of the face.
            edge = face->getEdge(j);
            if (edge->isBoundary())
                continue;

            row = edgeInternalIndex[edgeIndex(edge)];
            pentEdge = Dim4Edge::edgeNumber[perm[(j+1) % 3]][perm[(j+2) % 3]];

            tmpPerm = NPerm5(2, j) * perm.inverse() *
                pent->getEdgeMapping(pentEdge);
            // tmpPerm maps (2,3,4) -> (2,3,4), and maps the dual edge into
            // the dual face with the correct orientation.
            // Force (0,1) to map to (0,1), and then read off the sign.
            if (tmpPerm[0] != 0)
                tmpPerm = tmpPerm * NPerm5(0, 1);
            bdry32.entry(row, col) += tmpPerm.sign();
        }

        ++col;
    }

    // --------------------------------------
    // Boundary map from 2-cycles to 1-cycles
    // --------------------------------------
    //
    // Rows: Internal dual faces
    // Cols: Internal dual edges
    NMatrixInt bdry21(nFacesInternal, nTetrahedraInternal);

    // Build the boundary map, one dual edge at a time.
    col = 0;
    for (i = 0; i < nTetrahedra; ++i) {
        tet = getTetrahedron(i);
        if (tet->isBoundary())
            continue;

        // The dual edge running through this tetrahedron bounds the dual face
        // surrounding each of its faces.
        pent = tet->getEmbedding(0).getPentachoron();
        perm = tet->getEmbedding(0).getVertices();

        for (j = 0; j < 4; ++j) {
            // Face j of the tetrahedron is opposite vertex j of the
            // tetrahedron.
            face = tet->getFace(j);
            if (face->isBoundary())
                continue;

            row = faceInternalIndex[faceIndex(face)];
            pentFace = Dim4Face::faceNumber
                [perm[(j+1) % 4]][perm[(j+2) % 4]][perm[(j+3) % 4]];
            bdry21.entry(row, col) +=
                (pent->getFaceMapping(pentFace)[4] == perm[4] ? 1 : -1);
        }

        ++col;
    }

    // -------------------------------
    // Finished building boundary maps
    // -------------------------------
    delete[] edgeInternalIndex;
    delete[] faceInternalIndex;

    // --------------------------------------
    // Pair of boundary maps to abelian group
    // --------------------------------------

    NAbelianGroup* ans = new NAbelianGroup();
    ans->addGroup(bdry32);

    smithNormalForm(bdry21);
    unsigned long lim = (bdry21.rows() < bdry21.columns() ?
        bdry21.rows() : bdry21.columns());

    unsigned long extra = lim;
    for (unsigned long i = 0; i < lim; ++i)
        if (bdry21.entry(i, i) == 0) {
            extra = i;
            break;
        }

    ans->addRank(-extra);
    return *(H2_ = ans);
}

} // namespace regina
