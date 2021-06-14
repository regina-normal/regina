
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

#include <algorithm>
#include "dim2/dim2triangulation.h"
#include "maths/nmatrixint.h"
#ifndef EXCLUDE_SNAPPEA
#include "snappea/nsnappeatriangulation.h"
#endif
#include "surfaces/nnormalsurface.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"
#include "utilities/xmlutils.h"

namespace regina {

const int vertexSplit[4][4] = {
    { -1, 0, 1, 2 },
    {  0,-1, 2, 1 },
    {  1, 2,-1, 0 },
    {  2, 1, 0,-1 }
};

const int vertexSplitMeeting[4][4][2] = {
    { {-1,-1}, { 1, 2}, { 0, 2}, { 0, 1} },
    { { 1, 2}, {-1,-1}, { 0, 1}, { 0, 2} },
    { { 0, 2}, { 0, 1}, {-1,-1}, { 1, 2} },
    { { 0, 1}, { 0, 2}, { 1, 2}, {-1,-1} }
};

const int vertexSplitDefn[3][4] = {
    { 0, 1, 2, 3 },
    { 0, 2, 1, 3 },
    { 0, 3, 1, 2 }
};

const int vertexSplitPartner[3][4] = {
    { 1, 0, 3, 2},
    { 2, 3, 0, 1},
    { 3, 2, 1, 0}
};

/**
 * Indicates the two edges in the tetrahedron that the quad type is
 * disjoint from. 
 */
const char vertexSplitString[3][6] = { "01/23", "02/13", "03/12" };

// The following three arrays cannot be made 2-D because of a g++-2.95 bug.

const NPerm4 __triDiscArcs[12] = {
    NPerm4(0,1,2,3), NPerm4(0,2,3,1), NPerm4(0,3,1,2),
    NPerm4(1,0,3,2), NPerm4(1,3,2,0), NPerm4(1,2,0,3),
    NPerm4(2,3,0,1), NPerm4(2,0,1,3), NPerm4(2,1,3,0),
    NPerm4(3,2,1,0), NPerm4(3,1,0,2), NPerm4(3,0,2,1)
};

const NPerm4 __quadDiscArcs[12] = {
    NPerm4(0,2,3,1), NPerm4(3,0,1,2), NPerm4(1,3,2,0), NPerm4(2,1,0,3),
    NPerm4(0,3,1,2), NPerm4(1,0,2,3), NPerm4(2,1,3,0), NPerm4(3,2,0,1),
    NPerm4(0,1,2,3), NPerm4(2,0,3,1), NPerm4(3,2,1,0), NPerm4(1,3,0,2)
};

const NPerm4 __octDiscArcs[24] = {
    NPerm4(0,3,1,2), NPerm4(0,1,2,3), NPerm4(2,0,3,1), NPerm4(2,3,1,0),
    NPerm4(1,2,0,3), NPerm4(1,0,3,2), NPerm4(3,1,2,0), NPerm4(3,2,0,1),
    NPerm4(0,1,2,3), NPerm4(0,2,3,1), NPerm4(3,0,1,2), NPerm4(3,1,2,0),
    NPerm4(2,3,0,1), NPerm4(2,0,1,3), NPerm4(1,2,3,0), NPerm4(1,3,0,2),
    NPerm4(0,2,3,1), NPerm4(0,3,1,2), NPerm4(1,0,2,3), NPerm4(1,2,3,0),
    NPerm4(3,1,0,2), NPerm4(3,0,2,1), NPerm4(2,3,1,0), NPerm4(2,1,0,3)
};

NNormalSurface* NNormalSurface::clone() const {
    NNormalSurface* ans = new NNormalSurface(triangulation,
        dynamic_cast<NNormalSurfaceVector*>(vector->clone()));

    ans->eulerChar = eulerChar;
    ans->orientable = orientable;
    ans->twoSided = twoSided;
    ans->connected = connected;
    ans->realBoundary = realBoundary;
    ans->compact = compact;

    return ans;
}

NNormalSurface* NNormalSurface::doubleSurface() const {
    NNormalSurface* ans = new NNormalSurface(triangulation,
        dynamic_cast<NNormalSurfaceVector*>(vector->clone()));

    (*(ans->vector)) *= 2;

    // Some properties can be copied straight across.
    ans->realBoundary = realBoundary;
    ans->compact = compact;
    if (eulerChar.known())
        ans->eulerChar = eulerChar.value() * 2;

    // The following three properties can be used together to deduce how
    // they change in the clone.  However, until we sit down and check
    // through all possible cases we'll just leave them marked unknown.

    // TODO: ans->orientable, ans->twoSided, ans->connected

    // And some other properties are best left recalculated.

    return ans;
}

NNormalSurface::NNormalSurface(const NTriangulation* triang,
        NNormalSurfaceVector* newVector) :
        vector(newVector),
        triangulation(triang) {
}

void NNormalSurface::writeTextShort(std::ostream& out) const {
    unsigned long nTets = triangulation->getNumberOfTetrahedra();
    unsigned long tet;
    unsigned j;
    bool almostNormal = vector->allowsAlmostNormal();
    for (tet=0; tet<nTets; tet++) {
        if (tet > 0)
            out << " || ";
        for (j=0; j<4; j++)
            out << getTriangleCoord(tet, j) << ' ';
        out << ';';
        for (j=0; j<3; j++)
            out << ' ' << getQuadCoord(tet, j);
        if (almostNormal) {
            out << " ;";
            for (j=0; j<3; j++)
                out << ' ' << getOctCoord(tet, j);
        }
    }
}

bool NNormalSurfaceVector::hasMultipleOctDiscs(const NTriangulation* triang)
        const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    int oct;
    NLargeInteger coord;
    for (unsigned long tet=0; tet<nTets; tet++)
        for (oct=0; oct<3; oct++) {
            coord = getOctCoord(tet, oct, triang);
            if (coord == 0)
                continue;
            // We have found our one and only oct type!
            if (coord == 1)
                return false;
            return true;
        }
    // There are no octagonal types at all.
    return false;
}

bool NNormalSurfaceVector::isCompact(const NTriangulation* triang) const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    unsigned long tet;
    int type;
    for (tet = 0; tet < nTets; tet++) {
        for (type = 0; type < 4; type++)
            if (getTriangleCoord(tet, type, triang).isInfinite())
                return false;
        for (type = 0; type < 3; type++)
            if (getQuadCoord(tet, type, triang).isInfinite())
                return false;
    }
    if (allowsAlmostNormal())
        for (tet = 0; tet < nTets; tet++)
            for (type = 0; type < 3; type++)
                if (getOctCoord(tet, type, triang).isInfinite())
                    return false;
    return true;
}

bool NNormalSurfaceVector::isSplitting(const NTriangulation* triang) const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    unsigned long tet;
    int type;
    NLargeInteger tot;
    for (tet = 0; tet < nTets; tet++) {
        for (type = 0; type < 4; type++)
            if (getTriangleCoord(tet, type, triang) != 0)
                return false;
        tot = 0L;
        for (type = 0; type < 3; type++)
            tot += getQuadCoord(tet, type, triang);
        if (tot != 1)
            return false;
    }
    if (allowsAlmostNormal())
        for (tet = 0; tet < nTets; tet++)
            for (type = 0; type < 3; type++)
                if (getOctCoord(tet, type, triang) != 0)
                    return false;
    return true;
}

NLargeInteger NNormalSurfaceVector::isCentral(const NTriangulation* triang)
        const {
    unsigned long nTets = triang->getNumberOfTetrahedra();
    unsigned long tet;
    int type;
    NLargeInteger tot, tetTot;
    for (tet = 0; tet < nTets; tet++) {
        tetTot = 0L;
        for (type = 0; type < 4; type++)
            tetTot += getTriangleCoord(tet, type, triang);
        for (type = 0; type < 3; type++)
            tetTot += getQuadCoord(tet, type, triang);
        for (type = 0; type < 3; type++)
            tetTot += getOctCoord(tet, type, triang);
        if (tetTot > 1)
            return NLargeInteger::zero;
        tot += tetTot;
    }
    return tot;
}

bool NNormalSurface::isEmpty() const {
    unsigned long nTet = triangulation->getNumberOfTetrahedra();
    bool checkAlmostNormal = vector->allowsAlmostNormal();

    unsigned long t;
    int i;

    for (t = 0; t < nTet; ++t) {
        for (i = 0; i < 4; ++i)
            if (getTriangleCoord(t, i) != 0)
                return false;

        for (i = 0; i < 3; ++i)
            if (getQuadCoord(t, i) != 0)
                return false;

        if (checkAlmostNormal)
            for (i = 0; i < 3; ++i)
                if (getOctCoord(t, i) != 0)
                    return false;
    }

    return true;
}

bool NNormalSurface::sameSurface(const NNormalSurface& other) const {
    unsigned long nTet = triangulation->getNumberOfTetrahedra();
    bool checkAlmostNormal =
        (vector->allowsAlmostNormal() || other.vector->allowsAlmostNormal());

    unsigned long t;
    int i;

    for (t = 0; t < nTet; ++t) {
        for (i = 0; i < 4; ++i)
            if (getTriangleCoord(t, i) != other.getTriangleCoord(t, i))
                return false;

        for (i = 0; i < 3; ++i)
            if (getQuadCoord(t, i) != other.getQuadCoord(t, i))
                return false;

        if (checkAlmostNormal)
            for (i = 0; i < 3; ++i)
                if (getOctCoord(t, i) != other.getOctCoord(t, i))
                    return false;
    }

    return true;
}

bool NNormalSurface::locallyCompatible(const NNormalSurface& other) const {
    unsigned long nTets = triangulation->getNumberOfTetrahedra();

    int type;
    int found;
    for (unsigned long tet = 0; tet < nTets; ++tet) {
        found = 0;
        for (type = 0; type < 3; ++type)
            if (getQuadCoord(tet, type) > 0 ||
                    other.getQuadCoord(tet, type) > 0)
                ++found;
        for (type = 0; type < 3; ++type)
            if (getOctCoord(tet, type) > 0 ||
                    other.getOctCoord(tet, type) > 0)
                ++found;
        if (found > 1)
            return false;
    }

    return true;
}

void NNormalSurface::calculateOctPosition() const {
    if (! vector->allowsAlmostNormal()) {
        octPosition = NDiscType::NONE;
        return;
    }

    unsigned long tetIndex;
    int type;

    for (tetIndex = 0; tetIndex < triangulation->getNumberOfTetrahedra();
            ++tetIndex)
        for (type = 0; type < 3; ++type)
            if (getOctCoord(tetIndex, type) != 0) {
                octPosition = NDiscType(tetIndex, type);
                return;
            }

    octPosition = NDiscType::NONE;
    return;
}

void NNormalSurface::calculateEulerChar() const {
    unsigned long index, tot;
    int type;
    NLargeInteger ans = NLargeInteger::zero;

    // Add vertices.
    tot = triangulation->getNumberOfEdges();
    for (index = 0; index < tot; index++)
        ans += getEdgeWeight(index);

    // Subtract edges.
    tot = triangulation->getNumberOfTriangles();
    for (index = 0; index < tot; index++)
        for (type = 0; type < 3; type++)
            ans -= getTriangleArcs(index, type);

    // Add faces.
    tot = triangulation->getNumberOfTetrahedra();
    for (index = 0; index < tot; index++) {
        for (type=0; type<4; type++)
            ans += getTriangleCoord(index, type);
        for (type=0; type<3; type++)
            ans += getQuadCoord(index, type);
        for (type=0; type<3; type++)
            ans += getOctCoord(index, type);
    }

    // Done!
    eulerChar = ans;
}

void NNormalSurface::calculateRealBoundary() const {
    if (triangulation->isClosed()) {
        realBoundary = false;
        return;
    }

    unsigned long index;
    unsigned long tot = triangulation->getNumberOfTetrahedra();
    const NTetrahedron* tet;
    int type, face;

    for (index = 0; index < tot; index++) {
        tet = triangulation->getTetrahedron(index);
        if (tet->hasBoundary()) {
            // Check for disk types with boundary
            for (type=0; type<3; type++) {
                if (getQuadCoord(index, type) > 0) {
                    realBoundary = true;
                    return;
                }
            }
            for (type=0; type<3; type++) {
                if (getOctCoord(index, type) > 0) {
                    realBoundary = true;
                    return;
                }
            }
            for (type=0; type<4; type++)
                if (getTriangleCoord(index, type) > 0) {
                    // Make sure the triangle actually hits the
                    // boundary.
                    for (face=0; face<4; face++) {
                        if (face == type)
                            continue;
                        if (tet->adjacentTetrahedron(face) == 0) {
                            realBoundary = true;
                            return;
                        }
                    }
                }
        }
    }
    realBoundary = false;
}

namespace { // unnamed namespace for struct useful in triangulate() routine

struct normal_facet_data {
    unsigned long tetNum; // tetrahedron index this belongs to.
    bool TRI; // true if a triangle, false if part of a quad
    unsigned long TYPE; // for a triangle this is the tet vertex. 
      // for a quad, divide it by 2 to get an integer i. 
      // i indicates the quad separates edge [0,i+1] from
      // the complementary edge in the tetrahedron
      // The quad is cut in half to give two triangles and the parity 
      //  * const char vertexSplitString[3][6] = { "01/23", "02/13", "03/12" };
    unsigned long NUM; // which layer of this type are we.  We measure from 
      // the vertex for triangles, and from edge [i,3] for quads. 
      // this is perhaps where the data should be for which triangle of a 
      // subdivided quad we're dealing with. 
    normal_facet_data(const normal_facet_data &other); // copy constructor
    normal_facet_data(const unsigned long &TN, const bool &TI,
                      const unsigned long &TY, const unsigned long &NU);
    bool operator<(const normal_facet_data &other) const; // comparison
    normal_facet_data operator=(const normal_facet_data &other); // assignment
 };

normal_facet_data::normal_facet_data(const unsigned long &TN, const bool &TI,
                             const unsigned long &TY, const unsigned long &NU)
{ tetNum = TN; TRI=TI; TYPE=TY; NUM=NU; }

// copy constructor
normal_facet_data::normal_facet_data(const normal_facet_data &other) 
{
 tetNum = other.tetNum; TRI = other.TRI;
 TYPE = other.TYPE;     NUM = other.NUM;
}

// comparison for sorting
bool normal_facet_data::operator<(const normal_facet_data &other) const
{
 if (tetNum < other.tetNum) return true; if (tetNum > other.tetNum) return false;
 if (TRI && (!other.TRI)) return true;   if ((!TRI) && other.TRI) return false;
 if (TYPE < other.TYPE) return true;     if (TYPE > other.TYPE) return false;
 if (NUM < other.NUM) return true;       if (NUM > other.NUM) return false;
 return false;
}

// asst operator
normal_facet_data normal_facet_data::operator=(const normal_facet_data &other)
{
 tetNum = other.tetNum; TRI=other.TRI; TYPE=other.TYPE; NUM=other.NUM;
 return (*this);
}



} // end unnamed namespace

// NNormalSurface::triangulate() is a recent addtion by Ryan. Not extensively
//  tested, yet. 
Dim2Triangulation* NNormalSurface::triangulate() const
{
  Dim2Triangulation* retval( new Dim2Triangulation );
  std::map< Dim2Triangle*, normal_facet_data > triMap;
  std::map< normal_facet_data, Dim2Triangle* > triMapR; // reverse lookup

  // step 1: use getTriangleCoord and getQuadCoord to generate the triangles
  //  allocate the new Dim2Triangle* and set up triMap.  Set up a reverse
  //  lookup table so that we can quickly go from normal_facet_data to
  //  Dim2Triangle*. 
  for (unsigned long i=0; i<triangulation->getNumberOfTetrahedra(); i++)
   {
    // iterate through vertices, then create normal triangles
    for (unsigned long j=0; j<4; j++)
     for (unsigned long k=0; k<getTriangleCoord( i, j ).longValue(); k++)
      {
        Dim2Triangle* newTri( retval->newTriangle() );
        triMap.insert( std::pair< Dim2Triangle*, normal_facet_data >
         (newTri, normal_facet_data( i, true, j, k ) ) );
        triMapR.insert( std::pair< normal_facet_data, Dim2Triangle* >
         (normal_facet_data( i, true, j, k ), newTri ) );
      }
    // now iterate through the quads. 
    for (unsigned long j=0; j<3; j++)
     for (unsigned long k=0; k<getQuadCoord( i, j ).longValue(); k++)
      { // we divide the quad into two parts. 
        Dim2Triangle* newTri( retval->newTriangle() );
        triMap.insert( std::pair< Dim2Triangle*, normal_facet_data >
         ( newTri, normal_facet_data( i, false, 2*j, k ) ) );
        triMapR.insert( std::pair< normal_facet_data, Dim2Triangle* >
         (normal_facet_data( i, false, 2*j, k ), newTri ) );
        // "01/23", "02/13", "03/12" these are vertexSplitDefn[i][01/23]
        // we cut along vSD[i][0,2]--vSD[i][1,3] edge
        Dim2Triangle* newTri2 = retval->newTriangle();
        triMap.insert( std::pair< Dim2Triangle*, normal_facet_data >
         ( newTri2, normal_facet_data( i, false, 2*j+1, k ) ) );
        triMapR.insert( std::pair< normal_facet_data, Dim2Triangle* >
         (normal_facet_data( i, false, 2*j+1, k ), newTri2 ) );
        // and glue to make the subdivided quad.
        newTri->joinTo( 1, newTri2, NPerm3(2, 1, 0) ); 
      }
   }

  // step 2: run through triMap and perform gluings.  
  //         let's see if we can do it triangle by triangle. 
  for (unsigned long i=0; i<triangulation->getNumberOfTriangles(); i++)
   {
    const NTriangle* aTri( triangulation->getTriangle(i) );
    if (aTri->isBoundary()) continue;
    const NTetrahedron* tet0( aTri->getEmbedding(0).getTetrahedron() );
    const NTetrahedron* tet1( aTri->getEmbedding(1).getTetrahedron() );
    NPerm4 tetinc0( aTri->getEmbedding(0).getVertices() );
    NPerm4 tetinc1( aTri->getEmbedding(1).getVertices() );

    for (unsigned long j=0; j<3; j++)
     {
      unsigned long tricount0( getTriangleCoord( 
        triangulation->tetrahedronIndex( tet0 ), tetinc0[j] ).longValue() );
      unsigned long tricount1( getTriangleCoord( 
        triangulation->tetrahedronIndex( tet1 ), tetinc1[j] ).longValue() );
      // qtype0 and 1 are the quad types, if we happen to be dealing with quads.
      unsigned long qtype0 = vertexSplit[tetinc0[j]][tetinc0[3]];
      unsigned long qtype1 = vertexSplit[tetinc1[j]][tetinc1[3]];
      unsigned long quadcount0( getQuadCoord( 
        triangulation->tetrahedronIndex( tet0 ), qtype0 ).longValue() );
      unsigned long quadcount1( getQuadCoord( 
        triangulation->tetrahedronIndex( tet1 ), qtype1 ).longValue() );
      // sdt0 determines which facet of the two triangles that we've partitioned
      //  the quad into, we would be gluing to. 
      unsigned long sdt0 = ( (tetinc0[3]==vertexSplitDefn[qtype0][1]) || 
             (tetinc0[3]==vertexSplitDefn[qtype0][2]) ) ? 0 : 1;
      unsigned long sdt1 = ( (tetinc1[3]==vertexSplitDefn[qtype1][1]) || 
             (tetinc1[3]==vertexSplitDefn[qtype1][2]) ) ? 0 : 1;
      // a `matching equations' check. put in an ifdef DEBUG compiler flag.
      #ifdef DEBUG
      if (tricount0+quadcount0 != tricount1+quadcount1)
       { std::cout<<"NNormalSurface::triangulate() error 1."<<std::endl;
         exit(1); }
      #endif 

      for (unsigned long k=0; k<tricount0+quadcount0; k++)
       {
        normal_facet_data tri0( triangulation->tetrahedronIndex(tet0), 
         (k<tricount0) ? true : false, 
         (k<tricount0) ? tetinc0[j] : 2*qtype0 + sdt0, 
         (k<tricount0) ? k : k-tricount0 );
        normal_facet_data tri1( triangulation->tetrahedronIndex(tet1), 
         (k<tricount1) ? true : false, 
         (k<tricount1) ? tetinc1[j] : 2*qtype1 + sdt1, 
         (k<tricount1) ? k : k-tricount1 );

        // facet0 and facet1 represent the far vertex of the glued triangles, in
        //  tri0 and tri1 respectively.  This covers both the triangle and quad
        //  case.
        unsigned long facet0( (k<tricount0) ? // triangle case
            ( (tetinc0[j] < tetinc0[3]) ? tetinc0[3]-1 : tetinc0[3] ) : 
            (vertexSplitDefn[qtype0][0]==tetinc0[3] ||
             vertexSplitDefn[qtype0][1]==tetinc0[3]) ? 0 : 2 ); // quad case 
        unsigned long facet1( (k<tricount1) ? // triangle case
            ( (tetinc1[j] < tetinc1[3]) ? tetinc1[3]-1 : tetinc1[3] ) : 
            (vertexSplitDefn[qtype1][0]==tetinc1[3] ||
             vertexSplitDefn[qtype1][1]==tetinc1[3]) ? 0 : 2 ); // quad case

        unsigned long triEndpt0, triEndpt0S;
        if (k<tricount0)
         { // triangle case. Triangles are parametrized by the linear 
           // order of {0,1,2,3}, the vertices of the tetrahedra.
          triEndpt0 = (tetinc0[3] + 1) % 4;
          if (triEndpt0 == tetinc0[j]) triEndpt0 = (triEndpt0 + 1) % 4; 
          triEndpt0S = triEndpt0; 
          if (triEndpt0 > tetinc0[j]) triEndpt0--; // triangle coord
         }
        else
         { // quad case. Quads are parametrized by vertexSplitDefn. we need to
           // move in the positive orientation of the triangle from facet0. 
           // this can be determined by how vertexSplitDefn[qtype0] relates to 
           // tetinc0[3] and tetinc0[j]. facet0 is the opp index. 
           // (facet0+1) % 3 is the next in the triangle's coordinates.  We 
           // need the projection of this point to the one opposite tetinc0[j] 
           // in the tetrahedron, and we call it triEndpt0S 
           // 0 triangle vtx 0 -- vsd1 and vsd3
           //            vtx 1 -- vsd3 and vsd0
           //            vtx 2 -- vsd0 and vsd2
           // 1 triangle vtx 0 -- vsd0 and vsd2
           //            vtx 1 -- vsd1 and vsd2
           //            vtx 2 -- vsd1 and vsd3
           // compute (facet0+1)%3, this is the vtx number.  it corresponds to 
           // two vertices as above, 
           // and one of them must be tetinc0[j], then take the other one. 
           triEndpt0 = (facet0 + 1) % 3;
// TODO: rewrite using vertexSplitPartner
           triEndpt0S = (sdt0==0) ? // sdt0==0 
                ( (facet0==0) ? ( (tetinc0[j] == vertexSplitDefn[qtype0][3]) ?
                    vertexSplitDefn[qtype0][0] : vertexSplitDefn[qtype0][3] ) : 
                                ( (tetinc0[j] == vertexSplitDefn[qtype0][3]) ?
                    vertexSplitDefn[qtype0][1] : vertexSplitDefn[qtype0][3] ) ): 
                ( (facet0==0) ? ( (tetinc0[j] == vertexSplitDefn[qtype0][1]) ?
                    vertexSplitDefn[qtype0][2] : vertexSplitDefn[qtype0][1]) : 
                                ( (tetinc0[j] == vertexSplitDefn[qtype0][2]) ?
                    vertexSplitDefn[qtype0][0] : vertexSplitDefn[qtype0][2]) );
         } // end the triEndpt0(S) branch

        unsigned long triEndpt1, triEndpt1S;
        if (k<tricount1)
         { // triangle case. Triangles are parametrized by the linear order 
           // of {0,1,2,3}, the vertices of the tetrahedra.
          triEndpt1 = (tetinc1[3] + 1) % 4;
          if (triEndpt1 == tetinc1[j])  triEndpt1 = (triEndpt1 + 1) % 4; 
          triEndpt1S = triEndpt1; 
          if (triEndpt1 > tetinc1[j]) triEndpt1--; // triangle coord
         }
        else
         { // quad case. Quads are parametrized by vertexSplitDefn. 
           triEndpt1 = (facet1 + 1) % 3;

// TODO: rewrite using vertexSplitPartner
           triEndpt1S = (sdt1==0) ? // sdt1==0 
                ( (facet1==0) ? ( (tetinc1[j] == vertexSplitDefn[qtype1][3]) ?
                    vertexSplitDefn[qtype1][0] : vertexSplitDefn[qtype1][3] ) : 
                                ( (tetinc1[j] == vertexSplitDefn[qtype1][3]) ?
                    vertexSplitDefn[qtype1][1] : vertexSplitDefn[qtype1][3] ) ): 
                ( (facet1==0) ? ( (tetinc1[j] == vertexSplitDefn[qtype1][1]) ?
                    vertexSplitDefn[qtype1][2] : vertexSplitDefn[qtype1][1]) : 
                                ( (tetinc1[j] == vertexSplitDefn[qtype1][2]) ?
                    vertexSplitDefn[qtype1][0] : vertexSplitDefn[qtype1][2]) );
         }

        std::map<unsigned long, unsigned long> permGen;// used to define 
        std::set<unsigned long> domUnused, ranUnused; //gluing between triangles
        for (unsigned long X=0; X<3; X++) 
         { domUnused.insert(X); ranUnused.insert(X); }
        domUnused.erase(facet0);    ranUnused.erase(facet1);
        domUnused.erase(triEndpt0); ranUnused.erase(triEndpt1);

        bool switchEnds( tetinc0.preImageOf( triEndpt0S )!=
                         tetinc1.preImageOf( triEndpt1S ) );
        permGen.insert( std::pair< unsigned long, unsigned long >
                        (facet0, facet1) );
        permGen.insert( std::pair< unsigned long, unsigned long >(triEndpt0,
          (switchEnds) ? *ranUnused.begin() : triEndpt1 ) );
        permGen.insert( std::pair< unsigned long, unsigned long >
          (*domUnused.begin(),(switchEnds) ? triEndpt1 : *ranUnused.begin() ) );

        #ifdef DEBUG
        if ( (domUnused.size() != 1) || (ranUnused.size() != 1) )
         { std::cout<<"NNormalSurface::triangulate() Error! 2."<<std::endl;  
           std::cout<<"permGen == {";
           for (std::map<unsigned long, unsigned long>::iterator 
                A=permGen.begin(); 
                A!=permGen.end(); A++) std::cout<<"("<<A->first<<","<<
                A->second<<")";
           std::cout<<"}\n";
           std::cout<<"domUnused == {";
           for (std::set<unsigned long>::iterator A=domUnused.begin(); 
            A!=domUnused.end(); A++)
            std::cout<<(*A)<<" ";
           std::cout<<"} ";
           std::cout<<"ranUnused == {";
           for (std::set<unsigned long>::iterator A=ranUnused.begin(); 
            A!=ranUnused.end(); A++)
            std::cout<<(*A)<<" ";
           std::cout<<"} "<<std::endl;
           exit(1); }
        #endif

        NPerm3 glueMap( permGen[0], permGen[1], permGen[2] );
        #ifdef DEBUG
        if (triMapR.find(tri0)==triMapR.end()) {
          std::cout<<"NNormalSurface::triangulate() Error! triMapR[tri0] "<<
          "undefined!"<<std::endl; exit(1); }
        if (triMapR.find(tri1)==triMapR.end()) { 
          std::cout<<"NNormalSurface::triangulate() Error! triMapR[tri1] "<<
          "undefined!"<<std::endl; exit(1); }
        #endif
        triMapR[tri0]->joinTo( facet0, triMapR[tri1], glueMap );
       } // layers loop k.
     } // vertices of triangle loop j
   } // triangle loop i

 return retval;
}

#ifndef EXCLUDE_SNAPPEA
NMatrixInt* NNormalSurface::boundaryIntersections() const {
    // Make sure this is really a SnapPea triangulation.
    const NSnapPeaTriangulation* snapPea =
        dynamic_cast<const NSnapPeaTriangulation*>(getTriangulation());
    if (! snapPea)
        return 0;

    // Check the preconditions.
    if (! snapPea->isOriented())
        return 0;
    if (vector->allowsAlmostNormal())
        return 0;
    for (NTriangulation::VertexIterator it = snapPea->getVertices().begin();
            it != snapPea->getVertices().end(); ++it) {
        if (! (*it)->isIdeal())
            return 0;
        if (! (*it)->isLinkOrientable())
            return 0;
        if ((*it)->getLinkEulerChar() != 0)
            return 0;
    }

    NMatrixInt* equations = snapPea->slopeEquations();
    if (! equations)
        return 0;

    unsigned long cusps = equations->rows() / 2;
    unsigned long numTet = snapPea->getNumberOfTetrahedra();
    NMatrixInt* slopes = new NMatrixInt(cusps, 2);
    for(unsigned int i=0; i < cusps; i++) {
        NLargeInteger meridian; // constructor sets this to 0
        NLargeInteger longitude; // constructor sets this to 0
        for(unsigned int j=0; j < numTet; j++) {
            meridian += 
              equations->entry(2*i, 3*j)*getQuadCoord(j,vertexSplit[0][1]) +
              equations->entry(2*i, 3*j+1)*getQuadCoord(j,vertexSplit[0][2]) +
              equations->entry(2*i, 3*j+2)*getQuadCoord(j,vertexSplit[0][3]); 
            longitude += 
              equations->entry(2*i+1, 3*j)*getQuadCoord(j,vertexSplit[0][1]) +
              equations->entry(2*i+1, 3*j+1)*getQuadCoord(j,vertexSplit[0][2]) +
              equations->entry(2*i+1, 3*j+2)*getQuadCoord(j,vertexSplit[0][3]); 
        }
        slopes->entry(i,0) = meridian;
        slopes->entry(i,1) = longitude;
    }
    delete equations;
    return slopes;
}
#endif // EXCLUDE_SNAPPEA

void NNormalSurface::writeXMLData(std::ostream& out) const {
    using regina::xml::xmlEncodeSpecialChars;
    using regina::xml::xmlValueTag;

    // Write the opening tag including vector length.
    size_t vecLen = vector->size();
    out << "  <surface len=\"" << vecLen << "\" name=\""
        << xmlEncodeSpecialChars(name) << "\">";

    // Write all non-zero entries.
    NLargeInteger entry;
    for (size_t i = 0; i < vecLen; i++) {
        entry = (*vector)[i];
        if (entry != 0)
            out << ' ' << i << ' ' << entry;
    }

    // Write properties.
    if (eulerChar.known())
        out << "\n\t" << xmlValueTag("euler", eulerChar.value());
    if (orientable.known())
        out << "\n\t" << xmlValueTag("orbl", orientable.value());
    if (twoSided.known())
        out << "\n\t" << xmlValueTag("twosided", twoSided.value());
    if (connected.known())
        out << "\n\t" << xmlValueTag("connected", connected.value());
    if (realBoundary.known())
        out << "\n\t" << xmlValueTag("realbdry", realBoundary.value());
    if (compact.known())
        out << "\n\t" << xmlValueTag("compact", compact.value());

    // Write the closing tag.
    out << " </surface>\n";
}

// Default implementations for oriented surfaces. Returns zero as any
// coordinate system which supports orientation should override these.
NLargeInteger NNormalSurfaceVector::getOrientedTriangleCoord(
        unsigned long, int, const NTriangulation*, bool) const {
    return NLargeInteger::zero;
};

NLargeInteger NNormalSurfaceVector::getOrientedQuadCoord(
        unsigned long, int, const NTriangulation*, bool) const {
    return NLargeInteger::zero;
};

} // namespace regina

