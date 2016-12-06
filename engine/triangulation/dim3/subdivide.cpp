
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <set>
#include <vector>

#include "triangulation/dim3.h"
#include "utilities/memutils.h"

namespace regina {

void Triangulation<3>::drillEdge(Edge<3>* e) {
    // Recall from the barycentric subdivision code that a
    // subsimplex of the barycentric subdivision of an original simplex
    // correspond to a permutation p.
    // Namely, vertex 0 of the subsimplex is at vertex p[0] of the
    // original simplex, vertex 1 at the center of the edge joining vertex p[0]
    // and p[1] of the original simplex, etc.
    // In particular, an edge embedding of the given edge e provides us a
    // permutation that corresponds to a subsimplex adjacent to one half of e.
    // By applying the transposition 0-1 to the permutation, we get the
    // neighboring subsimplex adjacent to the other half of e.

    const Perm<4> flip(0, 1);
    const int flipIndex = flip.index();

    const Perm<4>& edgeEmbed        = e->front().vertices();
    const Perm<4>& flippedEdgeEmbed = edgeEmbed * flip;
    const int subsimplexIndices[2] = {
        edgeEmbed.index(), flippedEdgeEmbed.index()
    };

    const long originalTetNum      = e->front().tetrahedron()->index();

    ChangeEventSpan span(this);
    barycentricSubdivision();
    barycentricSubdivision();

    std::set<unsigned long> toRemove;

    for (int i = 0; i < 2; ++i) {
        // Subsimplex adjacent to one of the half edges coming from
        // subdividing given edge
        const long intermediateTetNum =
            24 * originalTetNum + subsimplexIndices[i];
        for (int j = 0; j < 2; ++j) {
            // The half edges join vertex 0 and 1 of the subsimplex
            // The next subdivision produces quarter edges
            const long finalTet =
                24 * intermediateTetNum + j * flipIndex;

            // Remove all subsubsimplices that touch an endpoint of the
            // resulting quarter edge from the second barycentric subdivision.
            for (int k = 0; k < 2; ++k) {
                const Vertex<3>* finalVertex = simplices_[finalTet]->vertex(k);
                for (auto& emb : *finalVertex)
                    toRemove.insert(emb.tetrahedron()->index());
            }
        }
    }

    // Make sure we remove tetrahedra in reverse order, so the numbering
    // doesn't change.
    for (std::set<unsigned long>::reverse_iterator rit = toRemove.rbegin();
            rit != toRemove.rend(); ++rit)
        removeTetrahedronAt(*rit);

    // We have lots of tetrahedra now.  Simplify.
    intelligentSimplify();
}

bool Triangulation<3>::idealToFinite() {
    // The call to isValid() ensures the skeleton has been calculated.
    if (isValid() && ! isIdeal())
        return false;

    int i,j,k,l;
    long numOldTet = simplices_.size();
    if (! numOldTet)
        return false;

    Triangulation<3> staging;
    ChangeEventSpan span1(&staging);

    Tetrahedron<3> **newTet = new Tetrahedron<3>*[32*numOldTet];
    for (i=0; i<32*numOldTet; i++)
        newTet[i] = staging.newTetrahedron();

    int tip[4];
    int interior[4];
    int edge[4][4];
    int vertex[4][4];

    int nDiv = 0;
    for (j=0; j<4; j++) {
        tip[j] = nDiv++;
        interior[j] = nDiv++;

        for (k=0; k<4; k++)
            if (j != k) {
                edge[j][k] = nDiv++;
                vertex[j][k] = nDiv++;
            }
    }

    // First glue all of the tetrahedra inside the same
    // old tetrahedron together.
    for (i=0; i<numOldTet; i++) {
        // Glue the tip tetrahedra to the others.
        for (j=0; j<4; j++)
            newTet[tip[j] + i * nDiv]->join(j,
                newTet[interior[j] + i * nDiv], Perm<4>());

        // Glue the interior tetrahedra to the others.
        for (j=0; j<4; j++) {
            for (k=0; k<4; k++)
                if (j != k) {
                    newTet[interior[j] + i * nDiv]->join(k,
                        newTet[vertex[k][j] + i * nDiv], Perm<4>());
                }
        }

        // Glue the edge tetrahedra to the others.
        for (j=0; j<4; j++)
            for (k=0; k<4; k++)
                if (j != k) {
                    newTet[edge[j][k] + i * nDiv]->join(j,
                        newTet[edge[k][j] + i * nDiv], Perm<4>(j,k));

                    for (l=0; l<4; l++)
                        if ( (l != j) && (l != k) )
                            newTet[edge[j][k] + i * nDiv]->join(l,
                                newTet[vertex[j][l] + i * nDiv], Perm<4>(k,l));
                }
    }

    // Now deal with the gluings between the pieces inside adjacent tetrahedra.
    Tetrahedron<3> *ot;
    long oppTet;
    Perm<4> p;
    for (i=0; i<numOldTet; i++) {
        ot = tetrahedron(i);
        for (j=0; j<4; j++)
            if (ot->adjacentTetrahedron(j)) {
                 oppTet = ot->adjacentTetrahedron(j)->index();
                 p = ot->adjacentGluing(j);

                 // First deal with the tip tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                          newTet[tip[k] + i * nDiv]->join(j,
                              newTet[tip[p[k]] + oppTet * nDiv], p);

                 // Next the edge tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                         newTet[edge[j][k] + i * nDiv]->join(k,
                             newTet[edge[p[j]][p[k]] + oppTet * nDiv], p);

                 // Finally, the vertex tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                         newTet[vertex[j][k] + i * nDiv]->join(k,
                             newTet[vertex[p[j]][p[k]] + oppTet * nDiv], p);

            }
    }

    ChangeEventSpan span2(this);

    swapContents(staging);
    ensureSkeleton();

    // Remove the tetrahedra that meet any of the ideal or invalid vertices.
    // First we make a list of the tetrahedra.
    std::vector<Tetrahedron<3>*> tetList;
    for (Vertex<3>* v : vertices())
        if (v->isIdeal() || ! v->isValid())
            for (auto& emb : *v)
                tetList.push_back(emb.tetrahedron());

    // Now remove the tetrahedra.
    // For each tetrahedron, remove it and delete it.
    for_each(tetList.begin(), tetList.end(),
        std::bind1st(std::mem_fun(&Triangulation<3>::removeTetrahedron), this));

    delete[] newTet;
    return true;
}

void Triangulation<3>::puncture(Tetrahedron<3>* tet) {
    if (! tet) {
        // Preconditions disallow empty triangulations, but anyway:
        if (simplices_.empty())
            return;

        tet = simplices_.front();
    }

    ChangeEventSpan span(this);

    // We will attach a pair of triangular prisms to face 123 of tet.
    // We will join the rectangular walls of the prisms together, and
    // one triangular end from each will join to form the new S^2 boundary.
    Tetrahedron<3>* prism[2][3];

    // Create the new tetrahedra in an order that ensures that the new
    // S^2 boundary will appear in the final two tetrahedra.
    int i, j;
    for (j = 0; j < 3; ++j)
        for (i = 0; i < 2; ++i)
            prism[i][j] = newTetrahedron();

    prism[0][0]->join(0, prism[0][1], Perm<4>(3,0,1,2));
    prism[0][1]->join(0, prism[0][2], Perm<4>(3,0,1,2));

    prism[1][0]->join(1, prism[1][1], Perm<4>(3,0,1,2));
    prism[1][1]->join(1, prism[1][2], Perm<4>(3,2,0,1));

    prism[0][0]->join(1, prism[1][0], Perm<4>(1,2,3,0));
    prism[0][0]->join(2, prism[1][0], Perm<4>(1,2,3,0));
    prism[0][1]->join(1, prism[1][1], Perm<4>(1,2,3,0));
    prism[0][1]->join(2, prism[1][1], Perm<4>(1,2,3,0));
    prism[0][2]->join(1, prism[1][2], Perm<4>(0,1,3,2));
    prism[0][2]->join(2, prism[1][2], Perm<4>(0,1,3,2));

    Tetrahedron<3>* adj = tet->adjacentTetrahedron(0);
    if (adj) {
        Perm<4> gluing = tet->adjacentGluing(0);
        tet->unjoin(0);
        prism[1][0]->join(0, adj, gluing);
    }

    tet->join(0, prism[0][0], Perm<4>(3,0,1,2));
}

void Triangulation<3>::connectedSumWith(const Triangulation<3>& other) {
    // Precondition check.
    if (simplices_.empty() || ! isConnected())
        return;

    ChangeEventSpan span(this);

    Tetrahedron<3>* toPuncture[2];

    // Insert the other triangulation *before* puncturing, so that
    // things work in the case where we sum a triangulation with itself.
    unsigned long n = simplices_.size();
    insertTriangulation(other);
    toPuncture[0] = simplices_[0];
    toPuncture[1] = simplices_[n];

    Tetrahedron<3>* bdry[2][2];

    puncture(toPuncture[0]);
    bdry[0][0] = simplices_[simplices_.size() - 2];
    bdry[0][1] = simplices_[simplices_.size() - 1];

    puncture(toPuncture[1]);
    bdry[1][0] = simplices_[simplices_.size() - 2];
    bdry[1][1] = simplices_[simplices_.size() - 1];

    bdry[0][0]->join(0, bdry[1][0], Perm<4>(2, 3));
    bdry[0][1]->join(0, bdry[1][1], Perm<4>(2, 3));
}

} // namespace regina
