
/**************************************************************************
 *                                                                        *
 *  Regina - A normal surface theory calculator                           *
 *  Computational engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
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

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ntriangulation.h"
    #include "ngrouppresentation.h"
#else
    #include "engine/triangulation/ntriangulation.h"
    #include "engine/algebra/ngrouppresentation.h"
#endif

const NGroupPresentation& NTriangulation::getFundamentalGroup() {
    if (calculatedFundamentalGroup)
        return *fundamentalGroup;
    
    fundamentalGroup = new NGroupPresentation();

    if (getNumberOfTetrahedra() == 0) {
        calculatedFundamentalGroup = true;
        return *fundamentalGroup;
    }

    // Find a maximal forest in the dual 1-skeleton.
    // Note that this will ensure the skeleton has been calculated.
    NPointerSet<NFace> forest;
    maximalForestInDualSkeleton(forest);

    // Each non-boundary not-in-forest face is a generator.
    // Each non-boundary edge is a relation.
    unsigned long nBdryFaces = 0;
    for (BoundaryComponentIterator bit(boundaryComponents);
            ! bit.done(); bit++)
        nBdryFaces += (*bit)->getNumberOfFaces();
    long nGens = getNumberOfFaces() - nBdryFaces - forest.size();

    // Insert the generators.
    fundamentalGroup->addGenerator(nGens);

    // Find out which face corresponds to which generator.
    long *genIndex = new long[getNumberOfFaces()];
    long i = 0;
    for (FaceIterator fit(faces); ! fit.done(); fit++)
        if ((*fit)->isBoundary() || forest.contains(*fit))
            genIndex[fit.getArrayIndex()] = -1;
        else
            genIndex[fit.getArrayIndex()] = i++;
    
    // Run through each edge and put the relations in the matrix.
    NDynamicArrayIterator<NEdgeEmbedding> embit;
    NTetrahedron* currTet;
    NFace* face;
    int currTetFace;
    long faceGenIndex;
    NGroupExpression* rel;
    for (EdgeIterator eit(edges); ! eit.done(); eit++)
        if (! (*eit)->isBoundary()) {
            // Put in the relation corresponding to this edge.
            rel = new NGroupExpression();
            for (embit.init((*eit)->getEmbeddings());
                    ! embit.done(); embit++) {
                currTet = (*embit).getTetrahedron();
                currTetFace = (*embit).getVertices()[2];
                face = currTet->getFace(currTetFace);
                faceGenIndex = genIndex[faces.position(face)];
                if (faceGenIndex >= 0) {
                    if ((face->getEmbedding(0).getTetrahedron() == currTet) &&
                            (face->getEmbedding(0).getFace() == currTetFace))
                        rel->addTermLast(faceGenIndex, 1);
                    else
                        rel->addTermLast(faceGenIndex, -1);
                }
            }
            fundamentalGroup->addRelation(rel);
        }

    // Tidy up.
    delete[] genIndex;
    fundamentalGroup->intelligentSimplify();

    calculatedFundamentalGroup = true;
    return *fundamentalGroup;
}

