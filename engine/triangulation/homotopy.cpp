
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  As an exception, when this program is distributed through (i) the     *
 *  App Store by Apple Inc.; (ii) the Mac App Store by Apple Inc.; or     *
 *  (iii) Google Play by Google Inc., then that store may impose any      *
 *  digital rights management, device limits and/or redistribution        *
 *  restrictions that are required by its terms of service.               *
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

#include "triangulation/ntriangulation.h"

namespace regina {

const NGroupPresentation& NTriangulation::fundamentalGroup() const {
    if (fundamentalGroup_.known())
        return *fundamentalGroup_.value();

    NGroupPresentation* ans = new NGroupPresentation();

    if (isEmpty())
        return *(fundamentalGroup_ = ans);

    // Calculate a maximal forest in the dual 1-skeleton.
    ensureSkeleton();

    // Each non-boundary not-in-forest triangle is a generator.
    // Each non-boundary edge is a relation.
    long nGens = countTriangles() - countBoundaryFacets()
        + countComponents() - size();

    // Insert the generators.
    ans->addGenerator(nGens);

    // Find out which triangle corresponds to which generator.
    long *genIndex = new long[countTriangles()];
    long i = 0;
    for (NTriangle* f : triangles())
        if (f->isBoundary() || f->inMaximalForest())
            genIndex[f->index()] = -1;
        else
            genIndex[f->index()] = i++;

    // Run through each edge and put the relations in the matrix.
    NTetrahedron* currTet;
    NTriangle* triangle;
    int currTetFace;
    long triGenIndex;
    NGroupExpression* rel;
    for (NEdge* e : edges())
        if (! e->isBoundary()) {
            // Put in the relation corresponding to this edge.
            rel = new NGroupExpression();
            for (auto& emb : *e) {
                currTet = emb.tetrahedron();
                currTetFace = emb.vertices()[2];
                triangle = currTet->triangle(currTetFace);
                triGenIndex = genIndex[triangleIndex(triangle)];
                if (triGenIndex >= 0) {
                    if ((triangle->front().tetrahedron() == currTet) &&
                            (triangle->front().triangle() == currTetFace))
                        rel->addTermLast(triGenIndex, 1);
                    else
                        rel->addTermLast(triGenIndex, -1);
                }
            }
            ans->addRelation(rel);
        }

    // Tidy up.
    delete[] genIndex;
    ans->intelligentSimplify();

    return *(fundamentalGroup_ = ans);
}

} // namespace regina

