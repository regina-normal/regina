
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

#include <set>
#include <vector>

#include "triangulation/ntriangulation.h"
#include "utilities/memutils.h"
#include "utilities/stlutils.h"

namespace regina {

void NTriangulation::barycentricSubdivision() {
    // Rewritten for Regina 4.94 to use a more sensible labelling scheme.

    // IMPORTANT: If this code is ever rewritten (and in particular, if
    // the labelling of new tetrahedra ever changes), then the
    // drillEdge() code must be rewritten as well (since it relies on
    // the specific labelling scheme that we use here).

    unsigned long nOldTet = tetrahedra_.size();
    if (nOldTet == 0)
        return;

    NTriangulation staging;
    ChangeEventSpan span1(&staging);

    NTetrahedron** newTet = new NTetrahedron*[nOldTet * 24];
    NTetrahedron* oldTet;

    // A tetrahedron in the subdivision is uniquely defined by the
    // permutation (face, edge, vtx, corner) of (0, 1, 2, 3).
    // This is the tetrahedron that:
    // - meets the boundary in the face opposite vertex "face";
    // - meets that face in the edge opposite vertex "edge";
    // - meets that edge in the vertex opposite vertex "vtx";
    // - directly touches vertex "corner".

    unsigned long tet;
    for (tet = 0; tet < 24 * nOldTet; ++tet)
        newTet[tet] = staging.newTetrahedron();

    // Do all of the internal gluings
    int permIdx;
    NPerm4 perm, glue;
    for (tet=0; tet < nOldTet; ++tet)
        for (permIdx = 0; permIdx < 24; ++permIdx) {
            perm = NPerm4::S4[permIdx];
            // (0, 1, 2, 3) -> (face, edge, vtx, corner)

            // Internal gluings within the old tetrahedron:
            newTet[24 * tet + permIdx]->joinTo(perm[3],
                newTet[24 * tet + (perm * NPerm4(3, 2)).S4Index()],
                NPerm4(perm[3], perm[2]));

            newTet[24 * tet + permIdx]->joinTo(perm[2],
                newTet[24 * tet + (perm * NPerm4(2, 1)).S4Index()],
                NPerm4(perm[2], perm[1]));

            newTet[24 * tet + permIdx]->joinTo(perm[1],
                newTet[24 * tet + (perm * NPerm4(1, 0)).S4Index()],
                NPerm4(perm[1], perm[0]));

            // Adjacent gluings to the adjacent tetrahedron:
            oldTet = getTetrahedron(tet);
            if (! oldTet->adjacentTetrahedron(perm[0]))
                continue; // This hits a boundary triangle.
            if (newTet[24 * tet + permIdx]->adjacentTetrahedron(perm[0]))
                continue; // We've already done this gluing from the other side.

            glue = oldTet->adjacentGluing(perm[0]);
            newTet[24 * tet + permIdx]->joinTo(perm[0],
                newTet[24 * tetrahedronIndex(
                    oldTet->adjacentTetrahedron(perm[0])) +
                    (glue * perm).S4Index()],
                glue);
        }


    // Delete the existing tetrahedra and put in the new ones.
    ChangeEventSpan span2(this);
    removeAllTetrahedra();
    swapContents(staging);
    delete[] newTet;
}

void NTriangulation::drillEdge(NEdge* e) {
    // Recall from the barycentric subdivision code above that
    // a tetrahedron in the subdivision is uniquely defined by the
    // permutation (face, edge, vtx, corner) of (0, 1, 2, 3).
    //
    // For an edge (i,j) opposite vertices (k,l), the tetrahedra that
    // meet it are:
    //
    // (k,l,i,j) and (l,k,i,j), both containing the half-edge touching vertex j;
    // (k,l,j,i) and (l,k,j,i), both containing the half-edge touching vertex i.
    //
    // In each case the corresponding edge number in the new tetrahedron
    // equals the edge number from the original tetrahedron.

    int edgeNum = e->getEmbedding(0).getEdge();
    long tetNum = tetrahedronIndex(e->getEmbedding(0).getTetrahedron());

    int oldToNew[2]; // Identifies two of the 24 tetrahedra in a subdivision
                     // that contain the two corresponding half-edges.
    oldToNew[0] = NPerm4(
        NEdge::edgeVertex[5 - edgeNum][0], NEdge::edgeVertex[5 - edgeNum][1],
        NEdge::edgeVertex[edgeNum][0], NEdge::edgeVertex[edgeNum][1]).
        S4Index();
    oldToNew[1] = NPerm4(
        NEdge::edgeVertex[5 - edgeNum][0], NEdge::edgeVertex[5 - edgeNum][1],
        NEdge::edgeVertex[edgeNum][1], NEdge::edgeVertex[edgeNum][0]).
        S4Index();

    ChangeEventSpan span(this);
    barycentricSubdivision();
    barycentricSubdivision();

    std::set<unsigned long> toRemove;

    int i, j, k;
    unsigned long finalTet;
    NVertex* finalVertex;
    std::vector<NVertexEmbedding>::const_iterator it;
    for (i = 0; i < 2; ++i)
        for (j = 0; j < 2; ++j) {
            finalTet = 24 * (24 * tetNum + oldToNew[i]) + oldToNew[j];

            // Remove all tetrahedra that touch each endpoint of the
            // resulting edge in the second barycentric subdivision.
            for (k = 0; k < 2; ++k) {
                finalVertex = tetrahedra_[finalTet]->getEdge(edgeNum)->
                    getVertex(k);
                for (it = finalVertex->getEmbeddings().begin();
                        it != finalVertex->getEmbeddings().end(); ++it)
                    toRemove.insert(tetrahedronIndex(it->getTetrahedron()));
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

bool NTriangulation::idealToFinite(bool forceDivision) {
    // The call to isValid() ensures the skeleton has been calculated.
    if (isValid() && ! isIdeal())
        if (! forceDivision)
            return false;

    int i,j,k,l;
    long numOldTet = tetrahedra_.size();
    if (! numOldTet)
        return false;

    NTriangulation staging;
    ChangeEventSpan span1(&staging);

    NTetrahedron **newTet = new NTetrahedron*[32*numOldTet];
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
            newTet[tip[j] + i * nDiv]->joinTo(j,
                newTet[interior[j] + i * nDiv], NPerm4());

        // Glue the interior tetrahedra to the others.
        for (j=0; j<4; j++) {
            for (k=0; k<4; k++)
                if (j != k) {
                    newTet[interior[j] + i * nDiv]->joinTo(k,
                        newTet[vertex[k][j] + i * nDiv], NPerm4());
                }
        }

        // Glue the edge tetrahedra to the others.
        for (j=0; j<4; j++)
            for (k=0; k<4; k++)
                if (j != k) {
                    newTet[edge[j][k] + i * nDiv]->joinTo(j,
                        newTet[edge[k][j] + i * nDiv], NPerm4(j,k));

                    for (l=0; l<4; l++)
                        if ( (l != j) && (l != k) )
                            newTet[edge[j][k] + i * nDiv]->joinTo(l,
                                newTet[vertex[j][l] + i * nDiv], NPerm4(k,l));
                }
    }

    // Now deal with the gluings between the pieces inside adjacent tetrahedra.
    NTetrahedron *ot;
    long oppTet;
    NPerm4 p;
    for (i=0; i<numOldTet; i++) {
        ot = getTetrahedron(i);
        for (j=0; j<4; j++)
            if (ot->adjacentTetrahedron(j)) {
                 oppTet = tetrahedronIndex(ot->adjacentTetrahedron(j));
                 p = ot->adjacentGluing(j);

                 // First deal with the tip tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                          newTet[tip[k] + i * nDiv]->joinTo(j,
                              newTet[tip[p[k]] + oppTet * nDiv], p);

                 // Next the edge tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                         newTet[edge[j][k] + i * nDiv]->joinTo(k,
                             newTet[edge[p[j]][p[k]] + oppTet * nDiv], p);

                 // Finally, the vertex tetrahedra.
                 for (k=0; k<4; k++)
                     if (j != k)
                         newTet[vertex[j][k] + i * nDiv]->joinTo(k,
                             newTet[vertex[p[j]][p[k]] + oppTet * nDiv], p);

            }
    }

    ChangeEventSpan span2(this);

    removeAllTetrahedra();
    swapContents(staging);
    calculateSkeleton();

    // Remove the tetrahedra that meet any of the non-standard or
    // ideal vertices.
    // First we make a list of the tetrahedra.
    std::vector<NTetrahedron*> tetList;
    std::vector<NVertexEmbedding>::const_iterator vembit;
    for (VertexIterator vIter = vertices_.begin();
            vIter != vertices_.end(); vIter++)
        if ((*vIter)->isIdeal() || ! (*vIter)->isStandard())
            for (vembit = (*vIter)->getEmbeddings().begin();
                    vembit != (*vIter)->getEmbeddings().end(); vembit++)
                tetList.push_back((*vembit).getTetrahedron());

    // Now remove the tetrahedra.
    // For each tetrahedron, remove it and delete it.
    for_each(tetList.begin(), tetList.end(),
        std::bind1st(std::mem_fun(&NTriangulation::removeTetrahedron), this));

    delete[] newTet;
    return true;
}

bool NTriangulation::finiteToIdeal() {
    if (! hasBoundaryTriangles())
        return false;

    // Make a list of all boundary triangles, indexed by triangle number,
    // and create the corresponding new tetrahedra.
    unsigned long nTriangles = getNumberOfTriangles();

    NTriangulation staging;
    NTetrahedron** bdry = new NTetrahedron*[nTriangles];
    NPerm4* bdryPerm = new NPerm4[nTriangles];
    NTetrahedron** newTet = new NTetrahedron*[nTriangles];

    ChangeEventSpan span1(&staging);

    TriangleIterator fit;
    unsigned i;
    for (i = 0, fit = triangles_.begin(); fit != triangles_.end(); ++i, ++fit) {
        if (! (*fit)->isBoundary()) {
            bdry[i] = newTet[i] = 0;
            continue;
        }

        bdry[i] = (*fit)->getEmbedding(0).getTetrahedron();
        bdryPerm[i] = (*fit)->getEmbedding(0).getVertices();
    }

    // Add the new tetrahedra one boundary component at a time, so that
    // the tetrahedron labels are compatible with previous versions of
    // regina (<= 4.6).
    BoundaryComponentIterator bit;
    for (bit = boundaryComponents_.begin();
            bit != boundaryComponents_.end(); bit++)
        for (i = 0; i < (*bit)->getNumberOfTriangles(); ++i)
            newTet[(*bit)->getTriangle(i)->markedIndex()] =
                staging.newTetrahedron();

    // Glue the new tetrahedra to each other.
    NEdge* edge;
    unsigned long tetTriangle1, tetTriangle2;
    NPerm4 t1Perm, t2Perm;
    for (bit = boundaryComponents_.begin();
            bit != boundaryComponents_.end(); bit++)
        for (i = 0; i < (*bit)->getNumberOfEdges(); i++) {
            edge = (*bit)->getEdge(i);

            // This must be a valid boundary edge.
            // Find the boundary triangles at either end.
            NEdgeEmbedding e1 = edge->getEmbeddings().front();
            NEdgeEmbedding e2 = edge->getEmbeddings().back();

            tetTriangle1 = e1.getTetrahedron()->getTriangle(
                e1.getVertices()[3])->markedIndex();
            tetTriangle2 = e2.getTetrahedron()->getTriangle(
                e2.getVertices()[2])->markedIndex();

            t1Perm = bdryPerm[tetTriangle1].inverse() * e1.getVertices();
            t2Perm = bdryPerm[tetTriangle2].inverse() * e2.getVertices() *
                NPerm4(2, 3);

            newTet[tetTriangle1]->joinTo(t1Perm[2], newTet[tetTriangle2],
                t2Perm * t1Perm.inverse());
        }

    // Now join the new tetrahedra to the boundary triangles of the original
    // triangulation.

    // Set up a change block, since here we start changing the original
    // triangulation.
    ChangeEventSpan span2(this);

    staging.moveContentsTo(*this);

    for (i = 0; i < nTriangles; ++i)
        if (newTet[i])
            newTet[i]->joinTo(3, bdry[i], bdryPerm[i]);

    // Clean up and return.
    delete[] newTet;
    delete[] bdryPerm;
    delete[] bdry;
    return true;
}

void NTriangulation::puncture(NTetrahedron* tet) {
    if (! tet) {
        // Preconditions disallow empty triangulations, but anyway:
        if (tetrahedra_.empty())
            return;

        tet = tetrahedra_.front();
    }

    ChangeEventSpan span(this);

    // We will attach a pair of triangular prisms to face 123 of tet.
    // We will join the rectangular walls of the prisms together, and
    // one triangular end from each will join to form the new S^2 boundary.
    NTetrahedron* prism[2][3];

    // Create the new tetrahedra in an order that ensures that the new
    // S^2 boundary will appear in the final two tetrahedra.
    int i, j;
    for (j = 0; j < 3; ++j)
        for (i = 0; i < 2; ++i)
            prism[i][j] = newTetrahedron();

    prism[0][0]->joinTo(0, prism[0][1], NPerm4(3,0,1,2));
    prism[0][1]->joinTo(0, prism[0][2], NPerm4(3,0,1,2));

    prism[1][0]->joinTo(1, prism[1][1], NPerm4(3,0,1,2));
    prism[1][1]->joinTo(1, prism[1][2], NPerm4(3,2,0,1));

    prism[0][0]->joinTo(1, prism[1][0], NPerm4(1,2,3,0));
    prism[0][0]->joinTo(2, prism[1][0], NPerm4(1,2,3,0));
    prism[0][1]->joinTo(1, prism[1][1], NPerm4(1,2,3,0));
    prism[0][1]->joinTo(2, prism[1][1], NPerm4(1,2,3,0));
    prism[0][2]->joinTo(1, prism[1][2], NPerm4(0,1,3,2));
    prism[0][2]->joinTo(2, prism[1][2], NPerm4(0,1,3,2));

    NTetrahedron* adj = tet->adjacentTetrahedron(0);
    if (adj) {
        NPerm4 gluing = tet->adjacentGluing(0);
        tet->unjoin(0);
        prism[1][0]->joinTo(0, adj, gluing);
    }

    tet->joinTo(0, prism[0][0], NPerm4(3,0,1,2));
}

void NTriangulation::connectedSumWith(const NTriangulation& other) {
    // Precondition check.
    if (tetrahedra_.empty() || ! isConnected())
        return;

    ChangeEventSpan span(this);

    NTetrahedron* toPuncture[2];

    // Insert the other triangulation *before* puncturing, so that
    // things work in the case where we sum a triangulation with itself.
    unsigned long n = tetrahedra_.size();
    insertTriangulation(other);
    toPuncture[0] = tetrahedra_[0];
    toPuncture[1] = tetrahedra_[n];

    NTetrahedron* bdry[2][2];

    puncture(toPuncture[0]);
    bdry[0][0] = tetrahedra_[tetrahedra_.size() - 2];
    bdry[0][1] = tetrahedra_[tetrahedra_.size() - 1];

    puncture(toPuncture[1]);
    bdry[1][0] = tetrahedra_[tetrahedra_.size() - 2];
    bdry[1][1] = tetrahedra_[tetrahedra_.size() - 1];

    bdry[0][0]->joinTo(0, bdry[1][0], NPerm4(2, 3));
    bdry[0][1]->joinTo(0, bdry[1][1], NPerm4(2, 3));
}

namespace { // unnamed namespace for private structs for divideEdges
 // I currently have a version of this in the dim4/subdivide.cpp as well
 // as the triangulation/subdivide.cpp file.  Should unify them at some point.

// this is intended as a bitmask version of the old simpFacet class. Describes
// a barycentre of a facet of a simplex. aDim is the hard-coded dimension limit
// aDim==5 means 4-simplices are currently the max.
struct bCtr {
 static const unsigned long aDim = 4; // we're at most dealing with 3-simplices.
 unsigned long code; // bitmask describing which vertices this is the
  // barycenter of. 
 bCtr() { code = 0; } // null constructor
 static bCtr byCode(const unsigned long &num) // define by code.
  { bCtr retval; retval.code = num; return retval; }
 bCtr(const bCtr &oth) { code = oth.code; } // copy constructor
 bCtr(const unsigned long &fac0)  { code = (1 << fac0); }
 bCtr(const unsigned long &fac0, const unsigned long &fac1)
  { code = (1 << fac0) | (1 << fac1); }
 bCtr(const unsigned long &fac0, const unsigned long &fac1, 
      const unsigned long &fac2)
  { code = (1 << fac0) | (1 << fac1) | (1 << fac2); }
 bCtr(const unsigned long &fac0, const unsigned long &fac1, 
      const unsigned long &fac2, const unsigned long &fac3)
  { code = (1 << fac0) | (1 << fac1) | (1 << fac2) | (1 << fac3); }
 bCtr pushForward(const NPerm4 per) const
  { // push forward 
   bCtr retval;
   for (unsigned long i=0; i<aDim; i++) if ( code & (1 << i) )
    retval.code = retval.code | (1 << per[i]);
   return retval;   
  }
 bCtr pullBack(const NPerm4 per) const
  { // pull back
   bCtr retval;
   for (unsigned long i=0; i<aDim; i++) if ( code & (1 << per[i]) )
    retval.code = retval.code | (1 << i);
   return retval;   
  }
 unsigned long dFac() const
  {
   unsigned long retval(0);
   for (unsigned long i=0; i<aDim; i++) if ( code & (1 << i) ) retval++;
   return retval; 
  }
 bool operator<(const bCtr &oth) const
  { // let's try a lexico sort on the dimension of the facets. 
   if (dFac() < oth.dFac()) return true;
   if (dFac() > oth.dFac()) return false;
   if (code < oth.code) return true;
   return false;
   // return code < oth.code; // this is the old sort. 
  }
}; 

// std output << operator
std::ostream& operator<<(std::ostream& out, const bCtr &c) {
 out << "<"; 
 for (unsigned long i=0; i<bCtr::aDim; i++) 
        out<<( ( (1 << i) & c.code) ? "1" : "0" );
 out << ">";
 return out;
}

// to be a replacement for the vtxSet struct.
struct simpSubFac { // a simplicial sub-facet of a simplex. This encodes any
 // sub-simplex of a simplex whose vertices consist of barycentres of the 
 // facets of the original simplex. 
 //
 // it is up to the user to decide what the ambient dimension of the simplex
 // is.  currently dim 3 is a hard-coded max, see aDim in the bCtr struct.
 std::list< bCtr > vtxset;
 // copy constructor
 simpSubFac(const simpSubFac &other) : vtxset( other.vtxset ) {}
 // empty constructor
 simpSubFac() {}
 // the not-subdivided n-dimensional simplex
 simpSubFac(const unsigned long aDim)
  { for (unsigned long i=0; i<=aDim; i++) vtxset.push_back( bCtr(i) ); }
 // check to see if facet fac is part of this simplex
 bool hasFacets( const std::list< bCtr > &fac ) const
  { // both vtxset and fac must be sorted.
   return std::includes( vtxset.begin(), vtxset.end(), 
                         fac.begin(), fac.end() );
  }
 // if facet inF appears, replace it by outF.  Return true if successful.
 bool replaceFacet( const bCtr &inF, const bCtr &outF )
  {
   for (std::list< bCtr >::iterator i=vtxset.begin(); i!=vtxset.end(); i++)
    if ( !((*i) < inF ) && !(inF < (*i)) )
     {
      vtxset.erase(i);
      vtxset.push_back( outF );
      vtxset.sort();
      return true;
     }
   return false;
  }
 // for sorting
 bool operator<(const simpSubFac &other) const
  {
    std::list< bCtr >::const_iterator i=vtxset.begin();
    std::list< bCtr >::const_iterator j=other.vtxset.begin();
    while ( (i!=vtxset.end()) && (j!=other.vtxset.end()) )
     {if ( (*i < *j) ) return true;  if ( (*j < *i) ) return false;
      i++; j++; }
    if ( (i == vtxset.end()) && (j != other.vtxset.end()) ) return true; 
    return false;
  }
 // warning, this routine does not sort -- up to user to do if neccessary.
 void coneOn( const bCtr &conePt )
  {   vtxset.push_back( conePt );  }
 simpSubFac pushForward( const NPerm4 &per ) const
  {
   simpSubFac retval;
   for (std::list< bCtr >::const_iterator i = vtxset.begin(); i!=vtxset.end(); 
        i++)
    retval.vtxset.push_back( i->pushForward(per) ); 
   retval.vtxset.sort();
   return retval;
  }
 simpSubFac pullBack( const NPerm4 &per ) const
  {
   simpSubFac retval;
   for (std::list< bCtr >::const_iterator i = vtxset.begin(); i!=vtxset.end(); 
        i++)
    retval.vtxset.push_back( i->pullBack(per) ); 
   retval.vtxset.sort();
   return retval;
  }
};

// std output << operator
std::ostream& operator<<(std::ostream& out, const simpSubFac &f) {
 out << "{"; 
 for (std::list< bCtr >::const_iterator i=f.vtxset.begin(); 
      i!=f.vtxset.end(); i++) 
  {
   if (i!=f.vtxset.begin()) out<<","; 
   out<<(*i);
  }
 out << "}";
 return out;
}

} // end unnamed namespace

void NTriangulation::divideEdges(const std::set< const NEdge* > &e)
{
 if (e.empty()) return; // nothing requested, nothing given. 

 //  Step 1: build a list of occurances of the edge e in triangles, then 
 //   subdivide the triangles appropriately. We never need a vertex in a
 //   triangle centre. This alone ensures we do not build the barycentric 
 //   subdivision.
 std::vector< std::list< unsigned long > > triInstVec( getNumberOfTriangles() );
 for (unsigned long i=0; i<getNumberOfTriangles(); i++)
  {
  for (unsigned long j=0; j<3; j++)
   if (e.find( getTriangle(i)->getEdge(j) ) != e.end() ) 
     triInstVec[i].push_back(j);
  triInstVec[i].sort();
  } 

 std::vector< std::list< simpSubFac > > triSubDiv( getNumberOfTriangles() );
 for (unsigned long i=0; i<getNumberOfTriangles(); i++)
  { 
   const NTriangle* tri( getTriangle(i) );
   simpSubFac genTri(2); 
   triSubDiv[i].push_back(genTri);

   for (std::list< unsigned long >::iterator j=triInstVec[i].begin(); 
        j!=triInstVec[i].end(); j++)
    {// run through buildDiv looking for the vertices of edge *j.  If found, 
     // subdivide and put into newDivision.  If not, move to newDivision. 
     for (std::list< simpSubFac >::iterator k=triSubDiv[i].begin(); 
          k!=triSubDiv[i].end(); k++) 
      {
       // if buildDiv has a simplex with those vertices, subdivide by modifying
       // the simplex pointed to by *k, and push_front the other subdivided 
       // simplex.  if not, we leave it be.
       
       unsigned long vCount(0);// check if *k has the vertices of edge *j. 
       std::list< bCtr > jEndPts;
       jEndPts.push_back( bCtr( tri->getEdgeMapping(*j)[0] ) );
       jEndPts.push_back( bCtr( tri->getEdgeMapping(*j)[1] ) );
       jEndPts.sort();
       if ( k->hasFacets(jEndPts) )
        { // subdivide!
         simpSubFac newFac( *k ); // new facet, for half of the subdivision
         newFac.replaceFacet( bCtr( tri->getEdgeMapping(*j)[0] ), 
          bCtr( tri->getEdgeMapping(*j)[0], tri->getEdgeMapping(*j)[1] ) );
         triSubDiv[i].push_front( newFac );

         // the modified old facet, to be part of the subdivision
         k->replaceFacet( bCtr( tri->getEdgeMapping(*j)[1] ), 
          bCtr( tri->getEdgeMapping(*j)[0], tri->getEdgeMapping(*j)[1] ) );
        } // end subdivision
      } // end k loop
    } // end j loop (elts of triInstVec[i])
   triSubDiv[i].sort();
  } // end i loop (triangles)

 //  Step 2: we have a subdivision of the triangles, now we extend to the
 //   tetrahedra. 
 std::vector< std::list< simpSubFac > > tetSubDiv( getNumberOfTetrahedra() );
 for (unsigned long i=0; i<getNumberOfTetrahedra(); i++)
  { 
   // look for efficient subdivisions, before resorting to coning.
   std::map< unsigned long, std::list< unsigned long > > divList;
   for (unsigned long j=0; j<4; j++)
    divList[ triSubDiv[ triangleIndex( getTetrahedron(i)->getTriangle(j) ) ].size() ].push_back(j);

   std::map< unsigned long, std::list< unsigned long > >::const_iterator oneIt( divList.find(1) );
   std::map< unsigned long, std::list< unsigned long > >::const_iterator twoIt( divList.find(2) );
   if ( ( oneIt != divList.end() ) && ( twoIt != divList.end() ) ) 
    if ( (oneIt->second.size()==2) && (twoIt->second.size()==2) )
    {
     // TODO: fill out the entire partition of the tetrahedron.  What we can do is
     //  take one of the divList[2] partitions and cone on the vertex opposite the triangle.
     NPerm4 triInc( getTetrahedron(i)->getTriangleMapping( divList[2].front() ) );
     const NTriangle* tri( getTetrahedron(i)->getTriangle( divList[2].front() ) );
     unsigned long triIdx( triangleIndex( tri ) );
     for (std::list< simpSubFac >::iterator k=triSubDiv[triIdx].begin();
                k!=triSubDiv[triIdx].end(); k++)
      {
       simpSubFac newFac( k->pushForward( triInc ) );
       newFac.coneOn( divList[2].front() );
       newFac.vtxset.sort();
       tetSubDiv[i].push_back( newFac );
      }
     continue;
    }
   // case where two opposite edges are divided.
   if ( twoIt != divList.end() ) if (twoIt->second.size()==4)
    { // we can subdivide using only 4 tets.  
      std::vector< unsigned long > edgeDiv;
      edgeDiv.reserve(2);
      for (unsigned long j=0; j<6; j++)
       if ( e.find( getTetrahedron(i)->getEdge(j) ) != e.end() )
        edgeDiv.push_back( j );
#ifdef DEBUG
      if (edgeDiv.size() != 2) 
        { std::cout<<"divideEdges() tet partition error."<<std::endl; exit(1); }
#endif
      // now check edges are opposite. 
      // I suppose we can compute all the vertices and check there's 4 of them. 
      std::set< unsigned long > edgeEndPts;
      edgeEndPts.insert( NEdge::edgeVertex[ edgeDiv[0] ][0] );
      edgeEndPts.insert( NEdge::edgeVertex[ edgeDiv[0] ][1] );
      edgeEndPts.insert( NEdge::edgeVertex[ edgeDiv[1] ][0] );
      edgeEndPts.insert( NEdge::edgeVertex[ edgeDiv[1] ][1] );
#ifdef DEBUG
      if (edgeEndPts.size() != 4) { 
         std::cout<<"divideEdges() tet partition error (2)."<<std::endl; 
         exit(1); 
       }
#endif
      bCtr e0bctr( NEdge::edgeVertex[ edgeDiv[0] ][0], 
                   NEdge::edgeVertex[ edgeDiv[0] ][1] );
      bCtr e1bctr( NEdge::edgeVertex[ edgeDiv[1] ][0], 
                   NEdge::edgeVertex[ edgeDiv[1] ][1] );
      for (unsigned long j=0; j<2; j++) for (unsigned long k=0; k<2; k++) 
       {
        simpSubFac newFac;
        newFac.vtxset.push_back( e0bctr );
        newFac.vtxset.push_back( e1bctr );
        newFac.vtxset.push_back( bCtr( NEdge::edgeVertex[ edgeDiv[0] ][j] ) );
        newFac.vtxset.push_back( bCtr( NEdge::edgeVertex[ edgeDiv[1] ][k] ) );
        tetSubDiv[i].push_back( newFac );
       }
      continue;
    }
   // TODO: consider other efficiencies?  The only remaining cases appear to be
   //  (a) 2 { 3 vertices } 4 { one vertex } and
   //  (b) 4 { all vertices }. 
   // Do any of these extend without new vertices? 
   /*
   for (std::map<unsigned long, std::list< unsigned long> >::iterator 
        j=divList.begin(); j!=divList.end(); j++)
    {
    std::cout<<j->first<<" { ";
    for (std::list< unsigned long >::iterator k=j->second.begin(); 
         k!=j->second.end(); k++)
      std::cout<<(*k)<<" ";
    std::cout<<"} ";
    } 
   std::cout<<std::endl;  */

   bCtr tetCtr(0,1,2,3);
   // at present we default to a coning construction.
   for (unsigned long j=0; j<4; j++)
    { 
     NPerm4 triInc( getSimplex(i)->getTriangleMapping(j) );
     const NTriangle* tri( getSimplex(i)->getTriangle(j) );
     unsigned long triIdx( triangleIndex( tri ) );
     // run through the subdivisions of this triangle. 
     for (std::list< simpSubFac >::iterator k=triSubDiv[triIdx].begin();
                k!=triSubDiv[triIdx].end(); k++)
        { 
         simpSubFac newFac( k->pushForward( triInc ) );
         newFac.coneOn(tetCtr);
         tetSubDiv[i].push_back( newFac );
        }
    }
   tetSubDiv[i].sort();
  } // end i loop (tetrahedra)

  // Step 3: indexing of new tetrahedra
  std::vector< std::map< simpSubFac, unsigned long > > 
        divPIdx( getNumberOfTetrahedra() );
  std::vector< std::map< std::pair< simpSubFac, bCtr >, unsigned long > > 
        divPvIdx( getNumberOfTetrahedra() );

  NTriangulation newTri; // we will use swapContents with *this to turn
   // our new triangulation into the ambient triangulation.
  unsigned long count(0); // for building divPIdx
  for (unsigned long i=0; i<getNumberOfTetrahedra(); i++)
   { // all the gluings inside getTetrahedron(i), tetSubdiv[i].
    const NTetrahedron* tet( getSimplex(i) );
    // index the elements of penSubDiv[] lexicographically.
    for (std::list< simpSubFac >::iterator j=tetSubDiv[i].begin(); 
         j!=tetSubDiv[i].end(); j++)
     {
      divPIdx[i].insert( std::pair< simpSubFac, unsigned long >( *j, count ) );
      count++; 
      newTri.newSimplex(); // divPIdx is the index of this pentachoron.
      // now index the vertices of this object.
      unsigned long subcount(0);
      for (std::list< bCtr >::iterator k=j->vtxset.begin(); k!=j->vtxset.end(); 
           k++)
       {
        divPvIdx[i].insert( 
         std::pair< std::pair< simpSubFac, bCtr >, unsigned long >
         (std::pair< simpSubFac, bCtr >( *j, *k ), subcount ) );
        subcount++;
       }
     } // end j loop
   } // end i loop -- indexing

  std::vector< std::map< simpSubFac, 
                         std::list< std::pair< simpSubFac, bCtr > > > > 
        incidTet(getNumberOfTetrahedra()); 
  // pents incident to the tetrahedra via the boundary relation, the 2nd 
  // element of the pair is the vertex we have to eliminate to get the vtxSet 
  // in the domain of the map.

  // preliminary step to build incidPen
  for (unsigned long i=0; i<getNumberOfTetrahedra(); i++)
   { 
    for (std::list< simpSubFac >::iterator j=tetSubDiv[i].begin(); 
         j!=tetSubDiv[i].end(); j++)
     for (unsigned long k=0; k<4; k++)
     {
      simpSubFac delK( *j );// delete k-th element of *j
      for (std::list< bCtr >::iterator l=delK.vtxset.begin(); 
           l!=delK.vtxset.end(); l++)
       if ( divPvIdx[i][std::pair< simpSubFac, bCtr >(*j, *l)] == k )
         {
         bCtr oldL( *l );
         delK.vtxset.erase( l );
         incidTet[i][ delK ].push_back( 
          std::pair< simpSubFac, bCtr >(*j, oldL) );
         break;
         }
#ifdef DEBUG
     if (delK.vtxset.size() != 3) { 
        std::cout<<"subdivide delK size error."<<std::endl; exit(1); 
     }
#endif
     } // end k loop
    // (a) internal gluings
    for (std::map< simpSubFac, std::list< 
         std::pair< simpSubFac, bCtr > > >::iterator j=incidTet[i].begin();
         j!=incidTet[i].end(); j++)
     if (j->second.size()==2) // we have a gluing.  Perform!
     { // pen vtx for first incident object
      simpSubFac tet0vtcs( j->second.front().first ); 
      bCtr tet0ofac( j->second.front().second );
      j->second.pop_front();
       // pen vtx for second incident object
      simpSubFac tet1vtcs( j->second.front().first ); 
      bCtr tet1ofac( j->second.front().second );
      j->second.pop_front();
      unsigned long tet0idx( divPIdx[i][tet0vtcs] );
      unsigned long tet1idx( divPIdx[i][tet1vtcs] );
      std::vector< unsigned long > vrtsIn0, vrtsIn1;
      vrtsIn0.reserve(4); vrtsIn1.reserve(4);
      vrtsIn0.push_back( divPvIdx[i][std::pair< simpSubFac, bCtr >
        (tet0vtcs, tet0ofac) ]);
      vrtsIn1.push_back( divPvIdx[i][std::pair< simpSubFac, bCtr >
        (tet1vtcs, tet1ofac) ]);
      for (std::list< bCtr >::const_iterator k=j->first.vtxset.begin(); 
           k!=j->first.vtxset.end(); k++)
       vrtsIn0.push_back( divPvIdx[i][std::pair< simpSubFac, bCtr >
        (tet0vtcs, *k) ]);
      for (std::list< bCtr >::const_iterator k=j->first.vtxset.begin(); 
           k!=j->first.vtxset.end(); k++)
       vrtsIn1.push_back( divPvIdx[i][std::pair< simpSubFac, bCtr >
        (tet1vtcs, *k) ]);
      NPerm4 glueMap( vrtsIn0[0], vrtsIn1[0], vrtsIn0[1], vrtsIn1[1], 
       vrtsIn0[2], vrtsIn1[2], vrtsIn0[3], vrtsIn1[3] );
      newTri.getSimplex( tet0idx )->joinTo( vrtsIn0[0], 
        newTri.getSimplex( tet1idx ), glueMap );
     }
    else
     {
#ifdef DEBUG
      // sanity check that j->second.size()==1
      if (j->second.size()!=1) { 
        std::cout<<"divideEdge() facet count error!"<<std::endl; exit(1); 
      }
#endif
      // check which tetrahedral facet this boundary facet is in...
      simpSubFac tet0vtcs( j->second.front().first ); 
      bCtr tet0ofac( j->second.front().second );
      j->second.pop_front();
#ifdef DEBUG
      if (tet0ofac.code != 15) { 
        std::cout<<"divideEdge() ambient pentachoral gluing across facet "<<
                   "that is not <1111>."<<std::endl; 
        exit(1); 
        }
#endif
      unsigned long tet0idx( divPIdx[i][tet0vtcs] );
      // step 1: if already glued, let's skip to the end.
      if (newTri.getSimplex( tet0idx )->adjacentSimplex(  
           divPvIdx[i][std::pair< simpSubFac, bCtr >
           (tet0vtcs, tet0ofac) ] )!=NULL )
       continue;
      // let's find the ambient tetrahedron. We are in pen i.  j->first is the 
      // simpSubFac of the subtet.  So we'll take the union of the codes and 
      // that should be the vertices of the ambient tet in pen i. 
      unsigned long code(0);
      for (std::list< bCtr >::const_iterator k=j->first.vtxset.begin(); 
           k!=j->first.vtxset.end(); k++)
       code = code | k->code;
      // code is a bitfield with 5 bits, only one should be zero.  Let's figure that out
      unsigned long vidx(0), zcount(0);
      for (unsigned long k=0; k<4; k++) if ( !((1 << k) & code) )
       { zcount++; vidx = k; }
#ifdef DEBUG
      if (zcount != 1) { std::cout<<"zcount error!"<<std::endl; 
         std::cout<<"in atet "<<i<<" tet idx "<<tet0idx<<" gluing across ";
         std::cout<<tet0ofac<<" and tet face consists of "<<j->first<<std::endl;
         exit(1); }
#endif
      // abort if this tetrahedron is a boundary tetrahedron. 
      if (getSimplex(i)->adjacentSimplex(vidx) == NULL) continue;

      // find out what its glued to, and push out subfacet forward
      //       quit if the pen index l>i. 
      unsigned long gPenIdx( simplexIndex( 
                getSimplex(i)->adjacentSimplex(vidx) ) );
      NPerm4 gPenGlue( getSimplex(i)->adjacentGluing(vidx) );
      if (gPenIdx > i) continue; // redundant gluing.
      simpSubFac pushedFac( tet0vtcs.pushForward( gPenGlue ) );
      unsigned long pushedIdx( divPIdx[gPenIdx][pushedFac] ); 
      // the subpentachoron index in newTri
#ifdef DEBUG
      if (newTri.getSimplex( pushedIdx )==NULL)
       { std::cout<<"simplex does not exist!"<<std::endl; exit(1); }
#endif      
      std::vector< unsigned long > vlist; 
      // this is to be the gluing permutation, set up in vector form...
      vlist.reserve(4);

      for (std::list< bCtr >::iterator X=tet0vtcs.vtxset.begin(); 
           X!=tet0vtcs.vtxset.end(); X++)
        vlist.push_back( divPvIdx[gPenIdx][std::pair< simpSubFac, bCtr >
         (pushedFac,X->pushForward(gPenGlue))] );
#ifdef DEBUG
      for (unsigned long x=0; x<4; x++) for (unsigned long y=x+1; y<4; y++)
       if (vlist[x]==vlist[y])
        { std::cout<<"Non permutation error."<<std::endl; exit(1); }
#endif

      newTri.getSimplex( tet0idx )->joinTo( 
        divPvIdx[i][std::pair< simpSubFac, bCtr >(tet0vtcs, tet0ofac)],  
        newTri.getSimplex( pushedIdx ),  
        NPerm4( vlist[0], vlist[1], vlist[2], vlist[3] ) ); 

     } // end facet only has one gluing fork.
   } // end i loop pentachora internal gluings

 swapContents( newTri );
 return;
} // end divideEdges


} // namespace regina
