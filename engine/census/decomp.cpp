
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 2012-2015, William Pettersson                           *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                  or William Pettersson (william.pettersson@gmail.com). *
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

#include <assert.h>
#include <sstream>
#include <stdlib.h>

#include "census/ncensus.h"
#include "census/ngluingpermsearcher.h"
#include "triangulation/nedge.h"
#include "triangulation/nfacepair.h"
#include "triangulation/ntriangulation.h"
//#include "utilities/boostutils.h"
#include "utilities/memutils.h"

#include "decomp.h"

using namespace regina;

const unsigned int DecompSearcher::faceEdges[4][3] = {
    {3,4,5}, {1,2,5}, {0,2,4}, {0,1,3}
};

const unsigned int DecompSearcher::otherFace[4][3] = {
    {3,2,1}, {3,2,0}, {3,1,0}, {2,1,0}
};

const signed int DecompSearcher::edgeParity[6][6] = {
    {-1,2,1,4,3,-1}, {2,-1,0,5,-1,3}, {1,0,-1,-1,5,4},
    {4,5,-1,-1,0,1}, {3,-1,5,0,-1,2}, {-1,3,4,1,2,-1}
};

const signed int DecompSearcher::otherVert[4][6] = {
    {-1,-1,-1,3,2,1},
    {-1,3,2,-1,-1,0},
    {3,-1,1,-1,0,-1},
    {2,1,-1,0,-1,-1}
};

const char DecompSearcher::dataTag_ = 'd';



DecompSearcher::DecompSearcher(const NFacePairing* pairing,
        const NFacePairing::IsoList* autos, bool orientableOnly,
        UseCycles use, void* useArgs, bool minimal) : use_(use),
        useArgs_(useArgs), pairing_(pairing),
        orientable(orientableOnly), minimal_(minimal) {
    // Begin by testing for face pairings that can never lead to such a
    // triangulation.
    if (minimal_ && (pairing_->hasTripleEdge() ||
            pairing_->hasBrokenDoubleEndedChain() ||
            pairing_->hasOneEndedChainWithDoubleHandle() ||
            pairing_->hasOneEndedChainWithStrayBigon() ||
            pairing_->hasWedgedDoubleEndedChain() ||
            pairing_->hasTripleOneEndedChain())) {
        empty = true;
        return;
    }
    empty = false;

    nTets = pairing->size();
    nEdges = 2*nTets;
    nEnds = 2*nEdges;

    edgesLeft = 3*nEdges;

    tets = new Tetrahedron[nTets];
    edges = new Edge[nEdges];
    ends = new EdgeEnd[nEnds];

    links = new VertexLink[4*nTets];
    // If we force the number of edges in the triangulation to be n+1 we get
    // \chi = 0 (Euler's formula) for each vertex link.
    nCycles = nTets+1;

    for( unsigned int i=0; i < nTets; i++ ) {
        tets[i].index = i;
    }

    nextColour = 0;

    // Note that we don't use index 0 to denote a cycle so we need space for
    // (nCycles+1) cycle descriptors.
    cycleLengths = new unsigned int[nCycles+1];
    cycles = new signed int*[nCycles+1];
    parityArray = new unsigned int[nCycles+1];
    parityArrayCount = new unsigned int[nCycles+1];
    for( unsigned int i=0; i < nCycles+1; i++ ) {
        cycleLengths[i] = 0;
        parityArray[i] = 0;
        parityArrayCount[i] = 0;
        cycles[i] = new signed int[3*nEdges];
    }


    bool* orderAssigned = new bool[nTets * 4];
        /* Have we placed a tetrahedron face or its partner in the
           order[] array yet? */

    NTetFace face, adj;
    unsigned int edgesDone = 0;
    std::fill(orderAssigned, orderAssigned + 4 * nTets, false);


    // Assign a (somewhat arbitrary) ordering onto the edges of the face pairing
    // graph.
    for (face.setFirst(); ! face.isPastEnd(nTets, true); face++) {
        if (orderAssigned[face.simp * 4 + face.facet])
            continue;

        adj = (*pairing)[face];


        ends[2*edgesDone].tet = &tets[face.simp];
        ends[2*edgesDone+1].tet = &tets[adj.simp];
        ends[2*edgesDone].face = face.facet;
        ends[2*edgesDone+1].face = adj.facet;
        ends[2*edgesDone].edge = &edges[edgesDone];
        ends[2*edgesDone+1].edge = &edges[edgesDone];

        edges[edgesDone].ends[0]=&ends[2*edgesDone];
        edges[edgesDone].ends[1]=&ends[2*edgesDone+1];
        edges[edgesDone].index = edgesDone+1;

        tets[face.simp].externalEdgeEnd[face.facet] =
            edges[edgesDone].ends[0];

        tets[adj.simp].externalEdgeEnd[adj.facet] =
            edges[edgesDone].ends[1];

        orderAssigned[face.simp * 4 + face.facet] = true;
        orderAssigned[adj.simp * 4 + adj.facet] = true;
        edgesDone++;
    }
    // All done for the orderAssigned[] array.  Tidy up.
    delete[] orderAssigned;

    // Generate the list of face pairing automorphisms if necessary.
    if (autos == 0) {
        autos = new NFacePairing::IsoList();
        pairing->findAutomorphisms(const_cast<NFacePairing::IsoList&>(*autos));
    }
    nAutos = 0;
    automorphisms = new Automorphism*[autos->size()];
    for( NFacePairing::IsoList::const_iterator it = autos->begin();
            it != autos->end(); it++) {
        automorphisms[nAutos] = new Automorphism((*it), edges, nEdges, nCycles);
        nAutos++;
    }
}

DecompSearcher::~DecompSearcher() {
    // If nothing here, we did nothing
    if (empty)
        return;

    for( unsigned int i=0; i < nCycles+1; i++ ) {
        delete[] cycles[i];
    }
    if (automorphisms != 0) {
        for( unsigned int i=0; i < nAutos; i++ ) {
            delete automorphisms[i];
        }
        delete[] automorphisms;
    }
    delete[] cycles;
    delete[] parityArray;
    delete[] parityArrayCount;
    delete[] cycleLengths;
    delete[] tets;
    delete[] edges;
    delete[] ends;
}

void DecompSearcher::colourLowestEdge() {
    unsigned int edge;
    // Note empty for loop to find first edge with space.
    for (edge=0; edges[edge].used == 3; edge++)
        assert(edge<=nEdges);
    Edge *nextEdge = &edges[edge];
    Tetrahedron * tet = nextEdge->ends[0]->tet;
    tet->used++;

    nextColour++;
    nextEdge->colour(nextColour);
    // Try each unused internal edge to start this cycle.
    for(unsigned int i=0; i < 3; i++) {
        unsigned int iEdge = faceEdges[nextEdge->ends[0]->face][i];
        if (tet->internalEdges[iEdge] != 0)
            continue;

        assert(cycleLengths[nextColour] == 0);

        tet->internalEdges[iEdge] = nextColour;

        // Note that edgeVertex[a] gives the two vertices on edge a
        // We want the two corresponding faces, so do edgeVertex[5-a]
        unsigned int startFace = NEdge::edgeVertex[5-iEdge][0];
        EdgeEnd *start = tet->externalEdgeEnd[startFace];

        if ( start == nextEdge->ends[0] ) {
            startFace = NEdge::edgeVertex[5-iEdge][1];
            start = tet->externalEdgeEnd[startFace];
        }

        nextEdge->ends[0]->map[iEdge] = nextEdge->used;
        edgesLeft--;
        EdgeEnd *nextEdgeEnd;
        nextEdgeEnd = nextEdge->ends[1];

        signed int dir = nextEdge->index;
        cycles[nextColour][cycleLengths[nextColour]]=dir;
        cycleLengths[nextColour]++;

        bool goodGluing = finishTet(tet);
        if (goodGluing) {
            // Try to find more paths.
            nextPath(start, iEdge, nextEdgeEnd);
        }

        unFinishTet(tet);
        cycles[nextColour][cycleLengths[nextColour]]=0;
        cycleLengths[nextColour]--;
        edgesLeft++;
        nextEdge->ends[0]->map[iEdge] = 0;
        tet->internalEdges[iEdge] = 0;
    }
    tet->used--;
    nextEdge->unColour(); 
    nextColour--;
}

bool DecompSearcher::checkColourOk() {
    // No short cycles
    if ( cycleLengths[nextColour] < 4)
        return false;
    return true;
}

void DecompSearcher::nextPath(EdgeEnd *start, unsigned int firstEdge,
        EdgeEnd *now) {
    unsigned int nextInternal;
    EdgeEnd *nextEnd, *outEnd;
    Edge *nextEdge;
    Tetrahedron *nextTet = now->tet;
    // Count the number of edges left. We are working on cycle number
    // nextColour. We need 4 edges for each of the (nCycles - nextColour)
    // cycles after this one.
    if (edgesLeft < 4*(nCycles - nextColour))
        return;

    for (unsigned int i=0; i<3 ; i++) {
        nextInternal = faceEdges[now->face][i];
        if (nextTet->internalEdges[nextInternal] != 0) 
            continue;
        outEnd = nextTet->externalEdgeEnd[otherFace[now->face][i]];
        nextEdge = outEnd->edge;
        nextEnd = nextEdge->otherEnd(outEnd);

        // An edge has two ends, ends[0] and ends[1]
        // A "forward" direction along an edge is from ends[0] to ends[1]
        // and "backwards" is the reverse. Forwards is denoted by a positive
        // edge number and backwards by a negative edge number.
        signed dir = nextEdge->index;
        if (nextEnd == (nextEnd->edge->ends[0]) ) 
            dir = -1* nextEdge->index;
        // In the following loop, dir=0 means the edge has already been used in
        // the opposite direction.
        if (orientable) {
            for(unsigned j=0; j< cycleLengths[nextColour];j++) {
                if (cycles[nextColour][j] == -dir) {
                    dir=0;
                    break;  
                }
            }
            if (dir == 0)  {
                continue;
            }
        }
        assert(cycleLengths[nextColour] < 3*nEdges);
        cycles[nextColour][cycleLengths[nextColour]]=dir;
        cycleLengths[nextColour]++;
        nextTet->internalEdges[nextInternal] = nextColour;
        nextTet->used++;
        now->map[nextInternal] = now->edge->used;
        nextEdge->colour(nextColour);
        outEnd->map[nextInternal] = nextEdge->used;
        edgesLeft--;


        // Check for vertex links being closed.
        // First get the required faces/edges/etc
        EdgeEnd* lastEnd = now->edge->otherEnd(now);
        int vl_tet = lastEnd->tet->index;
        int vl_internalEdge = 0;
        // Short loop to work out which edge was used in the last gluing.
        while(lastEnd->map[vl_internalEdge] != now->edge->used) {
            vl_internalEdge++;
        }
        int vl_face = lastEnd->face;
        // Get the vertex left on this face, but not on this edge.
        int vl_vert = otherVert[vl_face][vl_internalEdge];

        // First vertex link.
        VertexLink * vl_1 = links[vl_tet*4 + vl_vert].getHead();

        vl_tet = now->tet->index;
        vl_internalEdge = nextInternal;
        vl_face = now->face;
        vl_vert = otherVert[vl_face][vl_internalEdge];

        // Second vertex link.
        VertexLink * vl_2 = links[vl_tet*4 + vl_vert].getHead();

        // Swap pointers so that vl_1 has "not lower" depth.
        if ( vl_2->depth > vl_1->depth) {
            VertexLink *temp = vl_1;
            vl_1 = vl_2;
            vl_2 = temp;
        }

        // If we're gluing two faces of the same vertex link, we're just
        // subtracting 2 unglued faces. Otherwise we need to also "join"
        // the two vertex links.
        int vl_val;
        if (vl_1 != vl_2) {
            vl_val = vl_2->get();
        } else {
            vl_val = 0;
        }
        // The number of unglued faces left on the new combined link is the sum
        // of the unglued faces from the two earlier links, minus the 2 glued
        // faces.
        int complete = vl_1->add(vl_val-2);
        // Make sure that either the vertex link has not been closed off, or
        // possibly we have finished the decomposition.
        if ((complete > 0) || (edgesLeft == 0)) {
            // Ok, when looking for the number of unglued faces on vl_2, just
            // check vl_1.
            // Don't forget to store old ptr
            VertexLink *old;
            bool equal_depths = false;
            if (vl_val != 0) {
                old = vl_2->getPtr();
                vl_2->setPtr(vl_1);
                if (vl_1->depth == vl_2->depth) {
                    vl_1->depth++;
                    equal_depths = true;
                }
            }
            // Try to complete the cycle
            if (nextEnd == start) {
                start->map[firstEdge] = start->edge->used;
                bool goodGluing = finishTet(nextTet);
                if (goodGluing && checkColourOk() && isCanonical()) {
                    if ( (edgesLeft == 0) && checkComplete()) {
                        use_(this, useArgs_);
                    } else {
                        // At most nCycles cycles.  Remember nextColour starts at
                        // 1, and colourOnLowestEdge() increments it by one.
                        if (nextColour < nCycles)
                            colourLowestEdge();
                    }
                }
                unFinishTet(nextTet);
                start->map[firstEdge] = 0;
            }
            // Try to find more paths.
            bool goodGluing = finishTet(nextTet);
            if (goodGluing) {
                nextPath(start, firstEdge, nextEnd);
            }
            unFinishTet(nextTet);
            // Break the link between vl_1 and vl_2
            if (vl_val != 0) {
                vl_2->setPtr(old); // Unjoin the two vertex links.
                if (equal_depths) {
                    vl_1->depth--;
                }
            }
        }
        // And reset the number of unglued faces on vl_1
        vl_1->add(2-vl_val);

        cycleLengths[nextColour]--;
        cycles[nextColour][cycleLengths[nextColour]]=0;

        edgesLeft++;
        outEnd->map[nextInternal] = 0;
        now->map[nextInternal] = 0;
        nextEdge->unColour();
        nextTet->internalEdges[nextInternal] = 0;
        nextTet->used--;
    }
}


// Return true if there is nothing wrong.
bool DecompSearcher::finishTet(Tetrahedron *tet) {
    if ( tet->used < 6 ) {
        return true;
    }
    bool goodGluing = true;
    for (unsigned int i=0; i< 6;i++) {
        // Find the ends for this actual internal edge.
        // Note that edgeVertex[a] gives the two vertices on edge a
        // We want the two corresponding faces, so do edgeVertex[5-a]
        unsigned int endA = NEdge::edgeVertex[5-i][0];
        unsigned int endB = NEdge::edgeVertex[5-i][1];
        // Convert the objects into a numerical value
        EdgeEnd *aa = tet->externalEdgeEnd[endA];
        EdgeEnd *bb = tet->externalEdgeEnd[endB];
        // Get the colour
        unsigned int col = tet->internalEdges[i];
        assert(col>0);

        // Assign a,b,c,d such that (a,c) maps to (b,d) when we follow edge i.
        unsigned int a=0,b=0,c=0,d=0;
        for (unsigned int j=0; j < 3; j++) {
            unsigned int e = faceEdges[endA][j];
            // Don't map the current cycle anywhere, only the two edges
            // around it.
            if (e == i) {
                continue;
            }
            // Find the resultant edge.
            signed int eb = edgeParity[i][e];
            assert(eb>=0);

            // If we have started a new cycle on this tet, then we may not have
            // set up the map[] array on the very last edge.  If not, since
            // it's the last edge to be added, we know it must be the "third"
            // one used.
            unsigned int aAdjust = aa->map[e];
            if (aAdjust == 0) {
                aAdjust = 3;
            }
            unsigned int bAdjust = bb->map[eb];
            if (bAdjust == 0) {
                bAdjust = 3;
            }

            unsigned int valA = 3*(aa->edge->index) + aAdjust;
            unsigned int valB = 3*(bb->edge->index) + bAdjust;

            if (a==0) {
                a=valA;
                b=valB;
            } else {
                c=valA;
                d=valB;
            }
        }
        assert(a!=0);
        assert(b!=0);
        assert(c!=0);
        assert(d!=0);
        parityArrayCount[col]+=1;

        // a,b,c,d have been assigned such that (a,c) <-> (b,d)
        // Check whether parity has changed.
        if ( ((a<c) && (d<b)) || ( (c<a) && (b<d)))
            parityArray[col]+=1;

        // Now check to see if this results in a 2-sided projective
        // plane.  In other words, see if the "positive" and "negative" edges
        // swap an odd number of times. Note that we can only do this
        // if we have completed the current cycle, and also found all
        // possible "flips".
        if (( col < nextColour ) && (parityArrayCount[col] == cycleLengths[col]) && ( parityArray[col]%2 == 1))
            goodGluing=false;

        // Note that we keep running through all checks even if we know it will
        // fail. Otherwise we need to track which changes need to be "undone"
        // which is more complicated.
    }
    return goodGluing;
}

void DecompSearcher::unFinishTet(Tetrahedron *tet) {
    if ( tet->used < 6) {
        return;
    }
    // tet has just been fully completed.  Iterate over all
    // internal edges and remove parity checks.
    for (unsigned int i=0; i< 6;i++) {
        // Find the ends for this actual internal edge.

        // Note that edgeVertex[a] gives the two vertices on edge a
        // We want the two corresponding faces, so do edgeVertex[5-a]
        unsigned int endA = NEdge::edgeVertex[5-i][0];
        unsigned int endB = NEdge::edgeVertex[5-i][1];
        // Get the colour
        unsigned int col = tet->internalEdges[i];
        assert(col>0);
        EdgeEnd *aa = tet->externalEdgeEnd[endA];
        EdgeEnd *bb = tet->externalEdgeEnd[endB];
        unsigned int a=0,b=0,c=0,d=0;
        for (unsigned int j=0; j < 3; j++) {
            unsigned int e = faceEdges[endA][j];
            // Don't map the current cycle anywhere, only the two edges
            // around it.
            if (e == i) {
                continue;
            }
            // Find the resultant edge.
            signed int eb = edgeParity[i][e];
            assert(eb>=0);

            // If we have started a new cycle on this tet, then we may not have
            // set up the map[] array on the very last edge.  If not, since
            // it's the last edge to be added, we know it must be the "third"
            // one used.
            unsigned int aAdjust = aa->map[e];
            if (aAdjust == 0) {
                aAdjust = 3;
            }
            unsigned int bAdjust = bb->map[eb];
            if (bAdjust == 0) {
                bAdjust = 3;
            }

            // Convert the objects into a numerical value
            unsigned int valA = 3*(aa->edge->index) + aAdjust; 
            unsigned int valB = 3*(bb->edge->index) + bAdjust; 
            if (a==0) {
                a=valA;
                b=valB;
            } else {
                c=valA;
                d=valB;
            }
        }
        parityArrayCount[col]-=1;
        // a,b,c,d have been assigned such that (a,c) <-> (b,d)
        // Check whether parity has changed, and undo the changes
        if ( ((a<c) && (b>d)) || ( (a>c) && (b<d))) {
            parityArray[col]-=1;
        }
    }
}

bool DecompSearcher::checkComplete() {
    for(unsigned int i=0; i< nEdges; i++) {
        if (edges[i].used != 3) {
            return false;
        }
    }
    return true;
}

unsigned int DecompSearcher::findTetWithMostInternalEdgesUsed() {
    unsigned int mostUsed=0;
    // If nothing is used, start on tetrahedra 0.
    unsigned int tet=0;
    for(unsigned int i=0; i< nTets; i++) {
        if ((tets[i].used < 6) && (tets[i].used > mostUsed)) {
            mostUsed = tets[i].used;
            tet = i;
        }
    }
    assert(tets[tet].used < 6);
    return tet;
}

void DecompSearcher::runSearch(long maxDepth) {
    // Preconditions:
    //     Only closed prime minimal P2-irreducible triangulations are needed.
    //     The given face pairing is closed with order >= 3.
    //     The given face pairing does not contain any chains of length 1 or
    //     more.

    // Begin by testing for face pairings that can never lead to such a
    // triangulation.
    if (empty) {
        use_(0, useArgs_);
        return;
    }
    colourLowestEdge();
    use_(0, useArgs_);
}

NTriangulation* DecompSearcher::triangulate() const {
    NTriangulation* ans = new NTriangulation;
    NTetrahedron** simp = new NTetrahedron*[nTets];
    unsigned int t,a,b;
    signed int k;
    for (t = 0; t < nTets; ++t)
        simp[t] = ans->newSimplex();
    int perms[4];
    Edge *e;
    for (t = 0; t < nEdges; ++t) {
        e = &(edges[t]);
        // The edge e has 3 colours in it. These are denoted by k,
        // and can be 1, 2 or 3.
        for (k = 1; k <= 3; k++) {
            // The edge e represents 3 distinct edges.
            // The following loops work out, for each distinct edge,
            // which internal edge on either end is connected to
            // this edge.
            for (a = 0; (e->ends[0]->map[a] != k); a++) 
                assert(a<6);
            for (b = 0; (e->ends[1]->map[b] != k); b++) 
                assert(b<6);

            // Use otherVert to determine the "other" vertex on this face
            // of the tetrahedron, the vertex that is not on this edge.
            // Note that we don't know how edges a and b are glued together 
            // (in which direction that is) but we do know that the third
            // vertex on each face must be identified.
            int vertA = otherVert[e->ends[0]->face][a];
            assert(vertA >= 0);
            int vertB = otherVert[e->ends[1]->face][b];
            assert(vertB >= 0);

            perms[vertA] = vertB;
        }
        perms[e->ends[0]->face] = e->ends[1]->face;
        int thisTet = e->ends[0]->tet->index;
        int otherTet = e->ends[1]->tet->index;
        int thisFace = e->ends[0]->face;
        NPerm4 gluing(perms[0],perms[1],perms[2],perms[3]);
        simp[thisTet]->joinTo(thisFace, simp[otherTet], gluing);
    }
    delete[] simp;
    return ans;
}

void DecompSearcher::dumpData(std::ostream& out, bool cycle, bool colours,
        bool internal, bool maps) const {
    //NCompactSearcher::dumpData(out);

    // The cycles, and which edges they contain.
    if (cycle) {
        for (unsigned int i = 1; i <= nCycles && cycleLengths[i] > 0; i++) {
            out << i << ": ";
            for(unsigned int j = 0; j < cycleLengths[i]; j++) {
                out << cycles[i][j];
                if (j != cycleLengths[i]-1) 
                    out << ", ";
            }
            out << std::endl;
        }
    }
    // The colours on each edge
    if (colours) {
        for (unsigned int i = 0; i < nEdges; i++) {
            out << i << ": " << edges[i].colours[0]
                << ", " << edges[i].colours[1]
                << ", " << edges[i].colours[2] << std::endl;
        }
    }

    // Internal edges
    if (internal) {
        for (unsigned int i = 0; i < nTets; i++) {
            for(unsigned int j = 0; j < 6; j++) {
                out << tets[i].internalEdges[j] << " ";
            }
            if (i != (nTets-1))
                out << ": ";
        }
        out << std::endl;
    }
    // Maps of each edge end.
    if (maps) {
        for (unsigned int i = 0; i < nEdges; i++) {
            out << "Edge " << i << ": [";
            for(unsigned int j = 0; j < 6; j++) {
                unsigned int c = edges[i].ends[0]->map[j];
                assert(c < 3);
                out << std::endl << i << " " << j << " " << c << std::endl;
                if (c > 0) {
                    out << c << "(" << edges[i].colours[c] << ")";
                } else {
                    out << "0";
                }
                if (j != 5)
                    out << ", ";
            }
            out << "] -> [";
            for(unsigned int j = 0; j < 6; j++) {
                int c = edges[i].ends[1]->map[j];
                if (c > 0) {
                    out << c << "(" << edges[i].colours[c] << ")";
                } else {
                    out << "0";
                }
                if (j != 5)
                    out << ", ";
            }
            out << "]" << std::endl;
        }
    }
}

void DecompSearcher::dumpTaggedData(std::ostream& out) const {
    out << dataTag() << std::endl;
    dumpData(out);
}

bool DecompSearcher::isCanonical() {
    if ( automorphisms == 0 ) {
        return true;
    }
    unsigned int autoNo;
    bool thisAuto;
    unsigned int *cycleList[nextColour+1];
    unsigned int cycleListLengths[nextColour+1];
    unsigned int offset[nCycles];

    for(autoNo=0; autoNo < nAutos; autoNo++) {
        Automorphism *theAuto = automorphisms[autoNo];
        // As long as thisAuto is true, we keep looking at this automorphism.
        // If it becomes false, it means that this automorphism results in a
        // less canonical representation.
        thisAuto=true;
        // Generate new cycle lists
        for(unsigned int i=1; i<nextColour; i++) {
            cycleList[i] = theAuto->cycles[i];
            cycleListLengths[i] = theAuto->cycleLength[i];
            offset[i] = theAuto->offset[i];
        }
        offset[nextColour]=0;
        cycleList[nextColour] = theAuto->cycles[nextColour];
        // offset[i] is going to hold the current "starting" place for the
        // cycle.

        unsigned int nextEdge = 255;
        // nextEdge will store the "second" edge in the cycle, if offset[i]
        // is the first edge.  Note that the "second" edge may be the one
        // before the first edge, if the first edge is negative (since
        // swapping signs also reverses direction).
        // A value of 255 means something is broken.

        unsigned int newEdge = (*automorphisms[autoNo])[cycles[nextColour][0]];
        theAuto->cycles[nextColour][0] = newEdge;
        unsigned int min = newEdge;

        bool checkNextPair=false;
        bool setNextEdge=false;
        if ( newEdge %2 == 0) {
            // first edge +ve
            setNextEdge = true;
        } else {
            nextEdge =(*automorphisms[autoNo])[cycles[nextColour][cycleLengths[nextColour]-1]];
        }
        for(unsigned int j=1; j < cycleLengths[nextColour]; j++) {
            newEdge = (*automorphisms[autoNo])[cycles[nextColour][j]];

            // If checkNextPair is true, that means the last edge we
            // checked was equal-smallest.  We check this new edge against
            // the "next" edge from the current smallest, and if we have a
            // smaller edge, update the offset.
            if (checkNextPair) {
                if (newEdge < nextEdge) {
                    offset[nextColour]=j-1;
                    nextEdge = newEdge;
                }
                checkNextPair=false;
            }
            // If setNextEdge is true, that means we reset the offset in
            // the last iteration through, so we need to update what the
            // "next" edge after the offset is.
            if (setNextEdge) {
                nextEdge = newEdge;
                setNextEdge = false;
            }
            // New lowest edge used in this cycle.
            if ( newEdge < min ) { 
                if ((min%2 == 1) && ( newEdge + 1 == min )) {
                    // Current lowest is negative, new one is positive.
                    // Need to check "next".
                    if ( j == cycleLengths[nextColour]-1) {
                        // "next" edge is first
                        unsigned int newNext = cycleList[nextColour][0];
                        if  ( newNext < nextEdge) {
                            offset[nextColour]=j;
                            // no need to change nextEdge
                        }

                    } else {
                        // We need to check the next edge against the
                        // "next edge" from the current minimum, so
                        // make this note.
                        checkNextPair = true;
                    }
                } else {
                    min = newEdge;
                    offset[nextColour] = j;
                    if (min%2 ==1) {
                        // Negative
                        // If i%2 == 0, then 1 - 2*(i%2) == 1
                        // If i%2 == 1, then 1 - 2*(i%2) == -1
                        // So the line below adds one (if i%2 ==0) or
                        // subtracts one (if i%2 == 1), which flips the
                        // sign.
                        nextEdge = cycleList[nextColour][j-1] + (1- 2*(cycleList[nextColour][j-1]%2));
                    } else {
                        // Positive
                        setNextEdge = true;
                    }
                }
            } else {
                // Check to see if we have a tie.
                if ( newEdge == min ) {
                    // Have to check whether the next edge is smaller or
                    // not.  Don't forget that "next" might be previous if
                    // the lowest edge is a -ve.

                    if ( newEdge %2 == 1) { // lowest edge is a -ve
                        // If the current lowest is negative, we will be
                        // flipping the signs of all edges, so don't forget
                        // that in this comparison.

                        // If i%2 == 0, then 1 - 2*(i%2) == 1
                        // If i%2 == 1, then 1 - 2*(i%2) == -1
                        // So the line below adds one (if i%2 ==0) or
                        // subtracts one (if i%2 == 1), which flips the
                        // sign.
                        unsigned int newNext = cycleList[nextColour][j-1] + (1 - 2*(cycleList[nextColour][j-1]%2));

                        if (newNext < nextEdge) {
                            offset[nextColour]=j;
                            nextEdge = newNext;
                            // min stays the same.
                        }
                    } else {
                        // min and current edges are positive.
                        if ( j == cycleLengths[nextColour]-1) {
                            // "next" edge is first
                            unsigned int newNext = cycleList[nextColour][0];
                            if  ( newNext < nextEdge) {
                                offset[nextColour]=j;
                                // no need to change nextEdge
                            }

                        } else {
                            // We need to check the next edge against the
                            // "next edge" from the current minimum, so
                            // make this note.
                            checkNextPair = true;
                        }
                    }
                } else {
                    // This check here is for when the current "lowest edge in
                    // the cycle" is positive, but the edge we're checking now
                    // is negative but has same absolute value.
                    if ( (newEdge == (min+1)) && (min%2 == 0)) {
                        // If i%2 == 0, then 1 - 2*(i%2) == 1
                        // If i%2 == 1, then 1 - 2*(i%2) == -1
                        // So the line below adds one (if i%2 ==0) or
                        // subtracts one (if i%2 == 1), which flips the
                        // sign.
                        unsigned int newNext = cycleList[nextColour][j-1] + (1 - 2*(cycleList[nextColour][j-1]%2));
                        if ( newNext < nextEdge) {
                            min = newEdge;
                            nextEdge = newNext;
                            offset[nextColour]=j;
                        }
                    }
                }
            }
            theAuto->cycles[nextColour][j] = newEdge;
        }
        theAuto->cycleLength[nextColour] = cycleLengths[nextColour];
        theAuto->offset[nextColour] = offset[nextColour];
        cycleList[nextColour] = theAuto->cycles[nextColour];;
        cycleListLengths[nextColour] = cycleLengths[nextColour];

        // Sort cycleList based on values of cycleList[i][offset[i]]
        unsigned int i=1;
        for(; thisAuto && i <= nextColour ; i++ ) {
            assert(cycleListLengths[i] > 3);

            // Reverse bubble sort order so position[0] is definitely lowest
            // after one run through.  We can then check to see if this is more or
            // less canonical after 1 run through.
            for(unsigned int j=nextColour; j > i; j--) {
                if ( compareCycles(cycleList[j],        cycleList[j-1],
                                   cycleListLengths[j], cycleListLengths[j-1],
                                   offset[j],           offset[j-1]) == -1) {
                    unsigned int *temp;
                    temp = cycleList[j];
                    cycleList[j] = cycleList[j-1];
                    cycleList[j-1] = temp;
                    unsigned int temp2 = offset[j];
                    offset[j] = offset[j-1];
                    offset[j-1] = temp2;
                    temp2 = cycleListLengths[j];
                    cycleListLengths[j] = cycleListLengths[j-1];
                    cycleListLengths[j-1] = temp2;
                }
            }
            unsigned int counterA=offset[i];
            unsigned int maxLength = cycleLengths[i] < cycleListLengths[i] ? cycleLengths[i] : cycleListLengths[i];
            for(unsigned int j=0; j < maxLength; j++) {
                signed int t = 2*cycles[i][j];
                if (t < 0) {
                    t = (-t)+1;
                }
                unsigned int e = t;
                unsigned newE = cycleList[i][counterA];
                if (cycleList[i][offset[i]] %2 == 1) {
                    // If i%2 == 0, then 1 - 2*(i%2) == 1
                    // If i%2 == 1, then 1 - 2*(i%2) == -1
                    // So the line below adds one (if i%2 ==0) or
                    // subtracts one (if i%2 == 1), which flips the
                    // sign.
                    newE += (1 - 2*(newE%2));
                }

                // the automorphism gives a more canonical representation
                if (e > newE) {
                    return false;
                }
                // this automorphism is definitely worse, so go to next.
                if (e < newE) {
                    thisAuto=false;
                    break;
                }

                // Increment counter in appropriate direction.
                if (cycleList[i][offset[i]] %2 == 1) { // Negative
                    if ( counterA == 0) {
                        counterA = cycleListLengths[i]-1;
                    } else {
                        counterA--;
                    }
                } else {
                    counterA++;
                    if ( counterA == cycleListLengths[i] ) {
                        counterA = 0;
                    }
                }
            }
            // Check cycle lengths, shorter cycles are more canonical

            if ( cycleLengths[i] < cycleListLengths[i] ) {
                thisAuto=false;
            }
            if ( thisAuto && ( cycleListLengths[i] < cycleLengths[i] )) {
                return false;
            }
        }
    }
    return true;
}

// Check to see which of two cycles is more canonical.  Return values are
//  -1 if A < B
//  1 if B > A
//  0 if A == B
unsigned int DecompSearcher::compareCycles(unsigned int *cycleListA, 
        unsigned int *cycleListB, unsigned int lengthA, unsigned int lengthB,
        unsigned int offsetA, unsigned int offsetB) {
    unsigned int maxLength = lengthA < lengthB ? lengthA : lengthB;
    unsigned int counterA=offsetA;
    unsigned int counterB=offsetB;
    for(unsigned int i=0 ; i < maxLength; i++) {
        // Find next edges
        unsigned int edgeA = cycleListA[counterA];
        unsigned int edgeB = cycleListB[counterB];

        if (cycleListA[offsetA] %2 == 1) {
            // If i%2 == 0, then 1 - 2*(i%2) == 1
            // If i%2 == 1, then 1 - 2*(i%2) == -1
            // So the line below adds one (if i%2 ==0) or
            // subtracts one (if i%2 == 1), which flips the
            // sign.
            edgeA += (1 - 2*(edgeA%2));
        }

        if (cycleListB[offsetB] %2 == 1) {
            // If i%2 == 0, then 1 - 2*(i%2) == 1
            // If i%2 == 1, then 1 - 2*(i%2) == -1
            // So the line below adds one (if i%2 ==0) or
            // subtracts one (if i%2 == 1), which flips the
            // sign.
            edgeB += (1 - 2*(edgeB%2));
        }
        // Compare edges
        if (edgeA < edgeB)
            return -1;
        if (edgeB < edgeA)
            return 1;

        // Move counters along.  Note that we have to check whether the first
        // edge is positive or negative.
        if (cycleListA[offsetA] %2 == 1) { // Negative
            if ( counterA == 0) {
                counterA = lengthA-1;
            } else {
                counterA--;
            }
        } else {
            counterA++;
            if ( counterA == lengthA ) {
                counterA = 0;
            }
        }
        if (cycleListB[offsetB] %2 == 1) { // Negative
            if ( counterB == 0) {
                counterB = lengthB-1;
            } else {
                counterB--;
            }
        } else {
            counterB++;
            if ( counterB == lengthB ) {
                counterB = 0;
            }
        }
    }
    // So far all edges are the same.  shorter cycles are "more" canonical.
    if (lengthA < lengthB)
        return -1;
    if (lengthB < lengthA)
        return 1;
    // All edges the same, lengths the same.  Same cycles.
    return 0;
}


inline void DecompSearcher::Edge::colour(unsigned newColour) {
    assert(0 <= used);
    assert(used < 3);
    colours[used++] = newColour;
}

inline void DecompSearcher::Edge::unColour() {
    assert(0 < used);
    assert(used <= 3);
    colours[--used] = 0;
}

inline DecompSearcher::EdgeEnd* DecompSearcher::Edge::otherEnd(EdgeEnd *one) {
    if ( ends[0] == one )
        return ends[1];
    else {
        assert(one == ends[1]);
        return ends[0];
    }
}

DecompSearcher::Automorphism::Automorphism(const NIsomorphism * iso,
        const Edge *edges, const unsigned int _nEdges, 
        const unsigned int _nCycles) : nEdges(_nEdges), nCycles(_nCycles)  {
    edgeMap = new unsigned int[2*(nEdges+1)];
    realEdgeMap = edgeMap+nEdges;
    cycles = new unsigned int*[nCycles+1];
    cycleLength = new unsigned int[nCycles+1];
    offset = new unsigned int[nCycles+1];
    for(unsigned int i=1;i< nCycles+1;i++) {
        cycles[i] = new unsigned int[3*(nCycles-1)-2];
    }
    for (unsigned int i=0; i < nEdges;i++) {
        unsigned int startFace = edges[i].ends[0]->face;
        unsigned int startTet = edges[i].ends[0]->tet->index;
        unsigned int endFace = edges[i].ends[1]->face;
        unsigned int endTet = edges[i].ends[1]->tet->index;

        unsigned int newStartFace = iso->facePerm(startTet)[startFace];
        unsigned int newStartTet = iso->tetImage(startTet);
        unsigned int newEndFace = iso->facePerm(endTet)[endFace];
        unsigned int newEndTet = iso->tetImage(endTet);
        signed int iIndex = edges[i].index; 
        for(unsigned int j=0; j< nEdges;j++) {
            if (( newStartTet == edges[j].ends[0]->tet->index) &&
                ( newStartFace == edges[j].ends[0]->face) &&
                ( newEndTet == edges[j].ends[1]->tet->index) &&
                ( newEndFace == edges[j].ends[1]->face)) {
                // Edge parity stays the same.
                unsigned int jIndex = 2*edges[j].index;
                realEdgeMap[iIndex] = jIndex;
                realEdgeMap[std::ptrdiff_t(-iIndex)] = jIndex+1;
                break;
            }
            if (( newEndTet == edges[j].ends[0]->tet->index) &&
                ( newEndFace == edges[j].ends[0]->face) &&
                ( newStartTet == edges[j].ends[1]->tet->index) &&
                ( newStartFace == edges[j].ends[1]->face)) {
                unsigned int jIndex = 2*edges[j].index;
                realEdgeMap[std::ptrdiff_t(-iIndex)] = jIndex;
                realEdgeMap[iIndex] = jIndex+1;
                break;
            }
        }
    }
}

DecompSearcher::Automorphism::~Automorphism() {
    for(unsigned int i=1;i< nCycles+1;i++) {
        delete[] cycles[i];
    }
    delete[] cycles;
    delete[] cycleLength;
    delete[] offset;
    delete[] edgeMap;
}

unsigned int inline DecompSearcher::Automorphism::operator [] (const signed int in) {
  return realEdgeMap[(std::ptrdiff_t)(in)];
}

/* vim: set ts=4 sw=4 expandtab: */
