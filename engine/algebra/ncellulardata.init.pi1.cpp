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

#include "algebra/ncellulardata.h"
#include "maths/nperm3.h"
#include "maths/nperm4.h"
#include "algebra/ngrouppresentation.h"
#include "algebra/nhomgrouppresentation.h"

#include <map>
#include <list>
#include <cmath>

#include <iostream>
#include <sstream>

/**
 *  This file sets up the basic data required for fundamental group computations in a given
 *  triangulation.  Fundamental groups are computed by first finding maximal forests in the
 *  dual skeleton to the triangulation.  Since induced maps from the boundary components to 
 *  the manifold are to be computed, this skeleton is constructed inductively as a maximal
 *  forest in the dual boundary skeleton (both for the ideal boundary and standard boundary),
 *  then it is extended to the ambient manifold.  From this all the pi1 data and maps between
 *  the various pi1's can be computed. 
 */

namespace regina {

/**
 *  This routine runs through the dual 1-skeleton to the triangulation, building a maximal tree in the dual 1-skeleton
 * to the triangulation which restricts to a maximal tree in any boundary component, ideal or "standard" boundary.
 *
 * Assumes triangulation is connected.
 */
void NCellularData::buildMaximalTree()
{
// Iterate: walk through dual 1-skeleton until we hit a boundary component we haven't hit before.  During the iteration we
//  keep track of several things. 
//  the 0-cell list will consist of: Indexing of the barycentres of the pentachora, boundary tetrahedra and ideal boundary 
//   tetrahedra, indexed as the ideal ends of 1-cells 5*pen index + vertex index.  So we'll build it using a while loop that
//   goes back through the list of what's been done, and tries to expand it.  Priority will be to the boundary and ideal
//   boundary so that those trees are fully-developed as soon as the tree hits the boundary, as we want the tree to restrict
//   to trees in the boundary.   We also need to keep track of the tree itself.  This will be kept track of by the indexing
//   of (extended) dual 1-cells [index set the tetrahedra] and ideal dual 1-cells [index set

std::set< unsigned long > visitedZ, visitedBd, visitedId; // 0-cells in maximal tree
 // indexed by            nicIx[n], bcIx[n-1], icIx[n-1] respectively.

std::set< unsigned long > newVertexListS, newVertexListB, newVertexListI; // 0-cell indices yet unexplored...
 // indexed by            nicIx[4],       bcIx[3],        icIx[3]

if (tri4 != NULL)
 {
 // let's seed the process and start in pentachoron 0.
 visitedZ.insert(0); newVertexListS.insert(0);

 while ( (newVertexListS.size() > 0) || (newVertexListB.size() > 0) || (newVertexListI.size() > 0) ) // unexplored vertices...
  {

  ideal_boundary_loop4:
  // iterator to go through unexplored vertices
  std::set< unsigned long >::iterator unexploredV( newVertexListI.begin() );
  // first check to see if there are ideal vertices unexplored
  while (unexploredV != newVertexListI.end())
   {
    // *unexploredV is the icIx[3]-index of the ideal dual 0-cell
    const Dim4Pentachoron* pen(NULL); unsigned long idvnum; const Dim4Tetrahedron* septet(NULL); const Dim4Pentachoron* adjpen(NULL);
    NPerm5 adjglue, tetmap;

    pen = tri4 -> getPentachoron( icIx[3][*unexploredV]/5 );
    idvnum = icIx[3][*unexploredV] % 5;
    for (unsigned long i=1; i<5; i++) // look at adjacent pentachora
     {
      septet = pen -> getTetrahedron( int( (idvnum + i) % 5) );
      adjpen = pen -> adjacentPentachoron( int( (idvnum + i) % 5) );
      adjglue = pen -> adjacentGluing( int( (idvnum + i) % 5) );
      tetmap = pen -> getTetrahedronMapping( int( (idvnum + i) % 5) );
      // let's determine the number of adjpen
      unsigned long adjpennum = tri4 -> pentachoronIndex( adjpen );
      unsigned long adj0cellnum = 5*adjpennum + adjglue[idvnum]; // dual ideal 0-cell value, should be indexed by icIx[3]
      unsigned long I = lower_bound( icIx[3].begin(), icIx[3].end(), adj0cellnum ) - icIx[3].begin();
      // make J into the index of the ideal boundary face...
      unsigned long tetnum = tri4 -> tetrahedronIndex( septet );
      unsigned long J = lower_bound( icIx[2].begin(), icIx[2].end(), 4*tetnum + tetmap.preImageOf( idvnum ) ) - icIx[2].begin();
      // check to see if I is in visitedId
      if (!binary_search( visitedId.begin(), visitedId.end(), I) ) // not found
       { visitedId.insert(I); newVertexListI.insert(I); maxTreeIdB.insert(J); }
     }
    newVertexListI.erase(unexploredV++); // increment unexploredV but return the unincremented iterator for erase arg...
   }

  standard_boundary_loop4:
  unexploredV = newVertexListB.begin();
  // then check to see if there are standard boundary vertices unexplored
  while (unexploredV != newVertexListB.end())
   {
    // *unexploredV is the bcIx[3]-index of the standard boundary dual 0-cell
    const Dim4Tetrahedron* btet(NULL); const Dim4Pentachoron* pen(NULL); unsigned long tetnum;
    btet = tri4 -> getTetrahedron( bcIx[3][*unexploredV] ); // btet
    pen = btet -> getEmbedding(0).getPentachoron(); // ambient pen
    tetnum = btet -> getEmbedding(0).getTetrahedron(); // btet's number in ambient tet
    for (unsigned long i=1; i<5; i++)
      {
       const Dim4Tetrahedron* adjtet(NULL); const Dim4Pentachoron* adjpen(NULL); const Dim4Tetrahedron* septet(NULL);
       septet = pen -> getTetrahedron( int((tetnum + i) % 5) );
       const NPerm5 septetmap( pen -> getTetrahedronMapping( int((tetnum + i) % 5 )) );
       const Dim4Face* bfac(NULL);
       bfac = septet->getFace( septetmap.preImageOf( tetnum ) );
       const unsigned long bfacnum( tri4->faceIndex(bfac) );

       if (septet->isBoundary())
        {
         unsigned long I,J; 
         I = lower_bound( bcIx[3].begin(), bcIx[3].end(), tri4->tetrahedronIndex(septet) ) - bcIx[3].begin();
         J = lower_bound( bcIx[2].begin(), bcIx[2].end(), bfacnum ) - bcIx[2].begin();
         // check to see if I is in visitedBd
         if (!binary_search( visitedBd.begin(), visitedBd.end(), I) ) // not found
           { visitedBd.insert(I); newVertexListB.insert(I); maxTreeSttIdB.insert(J); }    
        }
       else 
        {
         adjpen = pen -> adjacentPentachoron( int((tetnum + i) % 5) );
         adjtet = adjpen -> getTetrahedron( pen -> adjacentGluing( int((tetnum + i) % 5) )[int(tetnum)] );
         // suitably update visitedBd and maxTreeSttIdB indexed by bcIx[3] and bcIx[2] resp.
         // find adjtet's index in bcIx[3]
         unsigned long I,J; 
         I = lower_bound( bcIx[3].begin(), bcIx[3].end(), tri4->tetrahedronIndex(adjtet) ) - bcIx[3].begin();
         J = lower_bound( bcIx[2].begin(), bcIx[2].end(), bfacnum ) - bcIx[2].begin();
         // check to see if I is in visitedBd
         if (!binary_search( visitedBd.begin(), visitedBd.end(), I) ) // not found
          { visitedBd.insert(I); newVertexListB.insert(I); maxTreeSttIdB.insert(J); }
        }
      }
     newVertexListB.erase(unexploredV++); // increment unexploredV but return the unincremented iterator...
   }

  unexploredV = newVertexListS.begin();
  // then check to see if there are standard vertices unexplored
  while (unexploredV != newVertexListS.end())
    {
    // *unexploredV is the nicIx[4]-index of the dual 0-cell
    const Dim4Pentachoron* pen(NULL); 
    NPerm5 adjglue, tetmap;
    pen = tri4 -> getPentachoron( *unexploredV );

    // step 1 look for ideal connectors.  If one found, record and exit loop 
    for (unsigned long i=0; i<5; i++) if ( pen -> getVertex(int(i)) -> isIdeal() )
     {
      unsigned long idvind = 5*(*unexploredV) + i; 
      unsigned long I = lower_bound( icIx[3].begin(), icIx[3].end(), idvind ) - icIx[3].begin();  
      // check to see if I in visitedId
      if (!binary_search( visitedId.begin(), visitedId.end(), I) ) // not found
       { visitedId.insert(I); newVertexListI.insert(I); maxTreeSttIdB.insert(I);  
         goto ideal_boundary_loop4; } // found new ideal boundary, loop back!
     }

    // step 2 look for standard boundary connectors. If one found, record and exit loop
    for (unsigned long i=0; i<5; i++) if ( pen -> getTetrahedron(int(i)) -> isBoundary() )
     {
      const Dim4Tetrahedron* btet( pen -> getTetrahedron(i) );
      unsigned long I = lower_bound( bcIx[3].begin(), bcIx[3].end(), tri4->tetrahedronIndex( btet ) ) - bcIx[3].begin();
      unsigned long J = lower_bound( nicIx[3].begin(), nicIx[3].end(), tri4->tetrahedronIndex( btet ) ) - nicIx[3].begin();
      if (!binary_search( visitedBd.begin(), visitedBd.end(), I ) ) // not found
       { visitedBd.insert(I);  newVertexListB.insert(I); maxTreeStd.insert(J);
         goto standard_boundary_loop4; }
     }

    // step 3 look for internal connectors. Only way to make it to the end of the loop
    for (unsigned long i=0; i<5; i++) if ( !pen -> getTetrahedron(i) -> isBoundary() )
     {
      const Dim4Pentachoron* adjpen( pen -> adjacentPentachoron( i ) );
      unsigned long I = tri4->pentachoronIndex( adjpen );
      const Dim4Tetrahedron* adjtet( pen -> getTetrahedron( i ) );
      unsigned long J = lower_bound( nicIx[3].begin(), nicIx[3].end(), tri4->tetrahedronIndex( adjtet ) ) - nicIx[3].begin();
      if (!binary_search( visitedZ.begin(), visitedZ.end(), I ) ) // not found
       { visitedZ.insert(I); maxTreeStd.insert(J); newVertexListS.insert(I); }
     }

    newVertexListS.erase(unexploredV++); // increment unexploredV but return the unincremented iterator for erase arg...
    }
   } // end big while loop
 } // end tri4 
 else
 { // tri3
 // let's seed the process and start in pentachoron 0.
 visitedZ.insert(0); newVertexListS.insert(0);

 while ( (newVertexListS.size() > 0) || (newVertexListB.size() > 0) || (newVertexListI.size() > 0) ) // unexplored vertices...
  {

  ideal_boundary_loop3:
  // iterator to go through unexplored vertices
  std::set< unsigned long >::iterator unexploredV( newVertexListI.begin() );
  // first check to see if there are ideal vertices unexplored
  while (unexploredV != newVertexListI.end())
   {
    // *unexploredV is the icIx[2]-index of the ideal dual 0-cell

    const NTetrahedron* tet(NULL); unsigned long idvnum; const NFace* sepfac(NULL); const NTetrahedron* adjtet(NULL);
    NPerm4 adjglue, facmap;

    tet = tri3 -> getTetrahedron( icIx[2][*unexploredV]/4 );
    idvnum = icIx[2][*unexploredV] % 4;
    for (unsigned long i=1; i<4; i++) // look at adjacent pentachora
     {
      sepfac = tet -> getFace( (idvnum + i) % 4 );
      adjtet = tet -> adjacentTetrahedron( (idvnum + i) % 4 );
      adjglue = tet -> adjacentGluing( (idvnum + i) % 4 );
      facmap = tet -> getFaceMapping( (idvnum + i) % 4 );
      // let's determine the number of adjpen
      unsigned long adjtetnum = tri3 -> tetrahedronIndex( adjtet );
      unsigned long adj0cellnum = 4*adjtetnum + adjglue[idvnum]; // dual ideal 0-cell value, should be indexed by icIx[3]
      unsigned long I = lower_bound( icIx[2].begin(), icIx[2].end(), adj0cellnum ) - icIx[2].begin();
      // make J into the index of the ideal boundary edge...
      unsigned long facnum = tri3 -> faceIndex( sepfac );
      unsigned long J = lower_bound( icIx[1].begin(), icIx[1].end(), 3*facnum + facmap.preImageOf( idvnum ) ) - icIx[1].begin();
      // check to see if I is in visitedId
      if (!binary_search( visitedId.begin(), visitedId.end(), I) ) // not found
       { visitedId.insert(I); newVertexListI.insert(I); maxTreeIdB.insert(J); }
     }
    newVertexListI.erase(unexploredV++); // increment unexploredV but return the unincremented iterator for erase arg...
   }

  standard_boundary_loop3:
  unexploredV = newVertexListB.begin();
  // then check to see if there are standard boundary vertices unexplored
  while (unexploredV != newVertexListB.end())
   {
    // *unexploredV is the bcIx[2]-index of the standard boundary dual 0-cell
    const NFace* bfac(tri3 -> getFace( bcIx[2][*unexploredV] ));  // boundary face
    const NTetrahedron* tet(bfac -> getEmbedding(0).getTetrahedron()); // tet it lives in
    unsigned long facnum = bfac -> getEmbedding(0).getFace(); // face number in ambient tet.

    for (unsigned long i=1; i<4; i++)
      {
       const NFace* sepfac( tet -> getFace( (facnum + i) % 4 ) );
       const NPerm4 sepfacmap( tet -> getFaceMapping( (facnum + i) % 4 ) );
       const NEdge* bedg( sepfac->getEdge( sepfacmap.preImageOf( facnum ) ) );
       const unsigned long bedgnum( tri3->edgeIndex(bedg) );
       if (sepfac->isBoundary())
        {
         unsigned long I,J; 
         I = lower_bound( bcIx[2].begin(), bcIx[2].end(), tri3->faceIndex(sepfac) ) - bcIx[2].begin();
         J = lower_bound( bcIx[1].begin(), bcIx[1].end(), bedgnum ) - bcIx[1].begin();
         // check to see if I is in visitedBd
         if (!binary_search( visitedBd.begin(), visitedBd.end(), I) ) // not found
           { visitedBd.insert(I); newVertexListB.insert(I); maxTreeSttIdB.insert(J); }    
        }
       else
        {
        const NTetrahedron* adjtet( tet -> adjacentTetrahedron( (facnum + i) % 4 ) ); // oh, what if there is NO adjacent tet!!
        const NFace* adjfac( adjtet -> getFace( tet -> adjacentGluing( (facnum + i) % 4)[facnum] ) );
        // suitably update visitedBd and maxTreeSttIdB indexed by bcIx[2] and bcIx[1] resp.
        // find adjfac's index in bcIx[2]

        unsigned long I,J; 
        I = lower_bound( bcIx[2].begin(), bcIx[2].end(), tri3->faceIndex(adjfac) ) - bcIx[2].begin();
        J = lower_bound( bcIx[1].begin(), bcIx[1].end(), bedgnum ) - bcIx[1].begin();
        // check to see if I is in visitedBd
        if (!binary_search( visitedBd.begin(), visitedBd.end(), I) ) // not found
          { visitedBd.insert(I); newVertexListB.insert(I); maxTreeSttIdB.insert(J); }
        }
      }
     newVertexListB.erase(unexploredV++); // increment unexploredV but return the unincremented iterator...
   }

  unexploredV = newVertexListS.begin();
  // then check to see if there are standard vertices unexplored
  while (unexploredV != newVertexListS.end())
    {

    // *unexploredV is the nicIx[4]-index of the dual 0-cell
    const NTetrahedron* tet(NULL); 
    NPerm5 adjglue, facmap;
    tet = tri3 -> getTetrahedron( *unexploredV );

    // step 1 look for ideal connectors.  If one found, record and exit loop 
    for (unsigned long i=0; i<4; i++) if ( tet -> getVertex(int(i)) -> isIdeal() )
     {
      unsigned long idvind = 4*(*unexploredV) + i; 
      unsigned long I = lower_bound( icIx[2].begin(), icIx[2].end(), idvind ) - icIx[2].begin();  
      // check to see if I in visitedId
      if (!binary_search( visitedId.begin(), visitedId.end(), I) ) // not found
       { visitedId.insert(I); newVertexListI.insert(I); maxTreeSttIdB.insert(I);  
         goto ideal_boundary_loop3; } // found new ideal boundary, loop back!
     }

    // step 2 look for standard boundary connectors. If one found, record and exit loop
    for (unsigned long i=0; i<4; i++) if ( tet -> getFace(int(i)) -> isBoundary() )
     {
      const NFace* bfac( tet -> getFace(int(i)) );
      unsigned long I = lower_bound( bcIx[2].begin(), bcIx[2].end(), tri3->faceIndex( bfac ) ) - bcIx[2].begin();
      unsigned long J = lower_bound( nicIx[2].begin(), nicIx[2].end(), tri3->faceIndex( bfac ) ) - nicIx[2].begin();
      if (!binary_search( visitedBd.begin(), visitedBd.end(), I ) ) // not found
       { visitedBd.insert(I);  newVertexListB.insert(I); maxTreeStd.insert(J);
         goto standard_boundary_loop3; }
     }

    // step 3 look for internal connectors. Only way to make it to the end of the loop
    for (unsigned long i=0; i<4; i++) if ( !tet -> getFace(int(i)) -> isBoundary() )
     {
      const NTetrahedron* adjtet( tet -> adjacentTetrahedron( int(i) ) );
      unsigned long I = tri3->tetrahedronIndex( adjtet );
      const NFace* adjfac( tet -> getFace( int(i) ) );
      unsigned long J = lower_bound( nicIx[2].begin(), nicIx[2].end(), tri3->faceIndex( adjfac ) ) - nicIx[2].begin();
      if (!binary_search( visitedZ.begin(), visitedZ.end(), I ) ) // not found
       { visitedZ.insert(I); maxTreeStd.insert(J); newVertexListS.insert(I); }
     }

    newVertexListS.erase(unexploredV++); // increment unexploredV but return the unincremented iterator for erase arg...
   }

   } // end big while loop
  } // end tri3
}

// dim4
bool NCellularData::inMaximalTree(const Dim4Tetrahedron* tet)
{
//  tri4 -> tetrahedronIndex( tet )
//  binary_search( nicIx[3] )  // in nicIx[3] ?
if (!binary_search( nicIx[3].begin(), nicIx[3].end(), tri4->tetrahedronIndex( tet ) ) ) return false; // not in list of tetrahedra!
//  I = lower_bound( ) // if so, find it
unsigned long I = lower_bound( nicIx[3].begin(), nicIx[3].end(), tri4->tetrahedronIndex( tet ) ) - nicIx[3].begin();
//  binary_search( maxTreeStd, I ) // in maxTreeStd ?
if (!binary_search( maxTreeStd.begin(), maxTreeStd.end(), I ) ) return false;
return true;
}

bool NCellularData::inMaximalTree(const Dim4Face* fac)
{
//  tri4 -> faceIndex( fac )
//  binary_search( bcIx[2] )  // in bcIx[2] ?
if (!binary_search(bcIx[2].begin(), bcIx[2].end(), tri4->faceIndex( fac ) ) ) return false; // not in list of faces!
//  I = lower_bound( ) // if so, find it
unsigned long I = lower_bound( bcIx[2].begin(), bcIx[2].end(), tri4->faceIndex( fac ) ) - bcIx[2].begin();
//  binary_search( maxTreeStB, I ) // in maxTreeStB ?
if (!binary_search( maxTreeStB.begin(), maxTreeStB.end(), I ) ) return false;
return true; 
}

bool NCellularData::inMaximalTree(const Dim4Tetrahedron* tet, unsigned long num)
{
//  tri4 -> tetrahedronIndex( tet )
//  binary_search( icIx[2] )  // in icIx[2] ?
if (!binary_search(icIx[2].begin(), icIx[2].end(), 4*tri4->tetrahedronIndex( tet ) + num ) ) return false; // not in list of ideal ends of tetrahedra
//  I = lower_bound( ) // if so, find it
unsigned long I = lower_bound( icIx[2].begin(), icIx[2].end(), 4*tri4->tetrahedronIndex( tet ) + num ) - icIx[2].begin();
//  binary_search( maxTreeIdB, I ) // in maxTreeIdB ?
if (!binary_search( maxTreeIdB.begin(), maxTreeIdB.end(), I ) ) return false; 
return true;
}

bool NCellularData::inMaximalTree(const Dim4Pentachoron* pen, unsigned long num)
{
//  tri4 -> pentachoronIndex( pen )
//  binary_search( icIx[3] )  // in icIx[3] ?
if (!binary_search(icIx[3].begin(), icIx[3].end(), 5*tri4->pentachoronIndex( pen ) + num ) ) return false; 
//  I = lower_bound( ) // if so, find it
unsigned long I = lower_bound( icIx[3].begin(), icIx[3].end(), 5*tri4->pentachoronIndex( pen ) + num ) - icIx[3].begin();
//  binary_search( maxTreeSttIdB, I ) // in maxTreeSttIdB ?
if (!binary_search( maxTreeSttIdB.begin(), maxTreeSttIdB.end(), I ) ) return false;
return true;
}


// dim3
bool NCellularData::inMaximalTree(const NFace* fac)
{
if (!binary_search( nicIx[2].begin(), nicIx[2].end(), tri3->faceIndex( fac ) ) ) return false; // not in list of faces!
//  I = lower_bound( ) // if so, find it
unsigned long I = lower_bound( nicIx[2].begin(), nicIx[2].end(), tri3->faceIndex( fac ) ) - nicIx[2].begin();
//  binary_search( maxTreeStd, I ) // in maxTreeStd ?
if (!binary_search( maxTreeStd.begin(), maxTreeStd.end(), I ) ) return false;
return true;
}

bool NCellularData::inMaximalTree(const NEdge* edg)
{
if (!binary_search(bcIx[1].begin(), bcIx[1].end(), tri3->edgeIndex( edg ) ) ) return false; // not in list of edges!
//  I = lower_bound( ) // if so, find it
unsigned long I = lower_bound( bcIx[1].begin(), bcIx[1].end(), tri3->edgeIndex( edg ) ) - bcIx[1].begin();
//  binary_search( maxTreeStB, I ) // in maxTreeStB ?
if (!binary_search( maxTreeStB.begin(), maxTreeStB.end(), I ) ) return false;
return true; 
}

bool NCellularData::inMaximalTree(const NFace* fac, unsigned long num)
{
if (!binary_search(icIx[1].begin(), icIx[1].end(), 3*tri3->faceIndex( fac ) + num ) ) return false; // not in list of ideal ends of faces
//  I = lower_bound( ) // if so, find it
unsigned long I = lower_bound( icIx[1].begin(), icIx[1].end(), 3*tri3->faceIndex( fac ) + num ) - icIx[1].begin();
//  binary_search( maxTreeIdB, I ) // in maxTreeIdB ?
if (!binary_search( maxTreeIdB.begin(), maxTreeIdB.end(), I ) ) return false; 
return true;
}

// counts number of elements in thelist less than obj
unsigned long num_less_than(const std::set<unsigned long> &thelist, const unsigned long &obj)
{
unsigned long retval = 0;
std::set<unsigned long>::const_iterator i;
for (i=thelist.begin(); i!=thelist.end(); i++) {  if ( (*i) < obj ) retval++; else return retval; }
return retval; 
}

bool NCellularData::inMaximalTree(const NTetrahedron* tet, unsigned long num)
{
if (!binary_search(icIx[2].begin(), icIx[2].end(), 4*tri3->tetrahedronIndex( tet ) + num ) ) return false; 
//  I = lower_bound( ) // if so, find it
unsigned long I = lower_bound( icIx[2].begin(), icIx[2].end(), 4*tri3->tetrahedronIndex( tet ) + num ) - icIx[2].begin();
//  binary_search( maxTreeSttIdB, I ) // in maxTreeSttIdB ?
if (!binary_search( maxTreeSttIdB.begin(), maxTreeSttIdB.end(), I ) ) return false;
return true;
}


   /**
     * Normal orientations for cells Regina does not naturally give normal orientations to. 
     *
     * normalsDim4BdryFaces is a vector that assigns to the i-th boundary face [tri4->getFace(bcIx[2][i])]
     *  the two boundary tetrahedra that contain it and the face number of the face in the tetrahedron. 
     *
     * normalsDim4BdryEdges is a vector that assigns to the i-th boundary edge [tri4->getFace(bcIx[1][i])]
     *  the circle of tetrahedra incident to that edge, with edginc[2] and edginc[3] forming the normal orientation
     *  in agreement with the indexing of tet. 
     *
     * normalsDim3BdryEdges is a vector that assigns to the i-th boundary face [tri3->getEdge(bcIx[1][i])]
     *  the two boundary faces that contain it and the edge number of the edge in the NFace. 
     *
     * normalsDim3BdryVertices is a vector that assigns to the i-th boundary vertex [tri3->getVertex(bcIx[0][i])]
     *  the circle of faces incident to that vertex, with vrtinc[1] and vrtinc[2] forming the normal orientation
     *  in agreement with the indexing of face. 
     */
void NCellularData::buildExtraNormalData()
{

if (tri4!=NULL)
 { // construct normalsDim4BdryFaces and normalsDim4BdryEdges
  if (normalsDim4BdryFaces.size()==0) return; // this routine has already been called, no need to call twice.   

  // for normalsDim4BdryEdges, run through bcIx[1]...
  normalsDim4BdryEdges.reserve( bcIx[1].size() );
  for (unsigned long i=0; i<bcIx[1].size(); i++)
   {
    const Dim4Edge* edg( tri4->getEdge(bcIx[1][i]) );
    // look through its embeddings in pentachora, find the tetrahedra which are boundary. Then we have to
    // order them. How??

   }
  // for normalsDim4BdryFaces, run through bcIx[2]...
//  normalsDim4BdryFaces.reserve( bcIx[2].size() );
  for (unsigned long i=0; i<bcIx[2].size(); i++)
   {
    const Dim4Face* fac( tri4->getFace(bcIx[2][i]) );
    // look through its embeddings in pentachora, the first and last boundary tet should appear in the first and
    // last embedding, right??
   }

 }
else
 { // construct normalsDim3BdryEdges and normalsDim3BdryVertices
//  if (normalsDim3BdryEdges.size()==0) return; // this routine has already been called, no need to call twice.

 }
}

void NCellularData::buildFundGrpPres()
{
 NGroupPresentation pres;
 // for now the group presentation will consist of: a list of numbers for the generators, 
 //  a list of words in the generators corresponding to the relators. 
 // the generators consist of edges of the dual 1-skeleton not in the maximal tree
 buildMaximalTree();

 if (tri4)
  {
   // we sort the 1-cells by: standard boundary, ideal boundary, standard interior cells, edges to ideal boundary
   // so wedge indexed by:     - maxTreeStB.size,  - maxTreeIdB.size(), - maxTreeStd.size(),  maxTreeSttIdB.size()

   unsigned long delta0( numNonIdealBdryCells[2] - maxTreeStB.size() ); 
   unsigned long delta1( delta0 + numIdealCells[2] - maxTreeIdB.size() );
   unsigned long delta2( delta1 + numNonIdealCells[3] - maxTreeStd.size() );
   unsigned long delta3( delta2 ); // to know what is out of range
   for (Dim4Triangulation::VertexIterator i = tri4->getVertices().begin(); i!= tri4->getVertices().end(); i++)
    if ((*i)->isIdeal()) delta3++; 
   pres.addGenerator( delta3 );// we've set the generators for the presentation.

   Dim4Tetrahedron* currTet (NULL); Dim4Pentachoron* currPen (NULL); Dim4Tetrahedron* tet (NULL); unsigned long currPenFace;

   // okay, lets start finding relators. First, from the interior faces. There are two types. 
   //  1) Non-boundary. 
   //  2) Boundary. 
   for (Dim4Triangulation::FaceIterator fac = tri4->getFaces().begin(); fac!=tri4->getFaces().end(); fac++)
    {
     NGroupExpression relator; 

     if ( !(*fac)->isBoundary() ) // non-boundary
      {
       std::deque<Dim4FaceEmbedding>::const_iterator embit;
       for (embit = (*fac)->getEmbeddings().begin();
                    embit != (*fac)->getEmbeddings().end(); embit++)
        { // now we have to determine whether or not the embedding coincides with the normal or of the tet.
         currPen = (*embit).getPentachoron();
         currPenFace = (*embit).getVertices()[4]; 
         tet = currPen->getTetrahedron(currPenFace);  
         // and is the tet in the maximal tree?    
         if (!inMaximalTree(tet)) 
          {
          // get index. 
          unsigned long tetind = delta1 + tri4->tetrahedronIndex( tet ) - num_less_than( maxTreeStd, tri4->tetrahedronIndex( tet ) );
          if ( (tet -> getEmbedding(0).getPentachoron() == currPen) && (tet -> getEmbedding(0).getTetrahedron() == currPenFace) )
           relator.addTermFirst( tetind, 1 ); else relator.addTermFirst( tetind, -1 );
          }
        } 
       NGroupExpression* relate;
       relate = new NGroupExpression(relator);
       pres.addRelation(relate);
      }
     else // boundary face
      {// this is much like the non-boundary case, only we have to pad the word at the front and back and we don't count .begin()
       // initial pad
       unsigned long tetind;

       std::deque<Dim4FaceEmbedding>::const_iterator embit = (*fac)->getEmbeddings().begin();
       currPen = (*embit).getPentachoron();
       currPenFace = (*embit).getVertices()[4]; 
       tet = currPen->getTetrahedron(currPenFace);
       if (!tet->isBoundary()) std::cout<<"ERROR (1)!!"; std::cout.flush();
       if (!inMaximalTree(tet))
        {
         tetind = delta1 + tri4->tetrahedronIndex( tet ) - num_less_than( maxTreeStd, tri4->tetrahedronIndex( tet ) );
         relator.addTermFirst( tetind, -1 ); // all 1-cells dual to boundary tets assumed oriented outwards
        }
   
       // main loop
       embit++;
       for (;embit != (*fac)->getEmbeddings().end(); embit++)
        { // now we have to determine whether or not the embedding coincides with the normal or of the tet.
         currPen = (*embit).getPentachoron();
         currPenFace = (*embit).getVertices()[4]; 
         tet = currPen->getTetrahedron(currPenFace);  
         // and is the tet in the maximal tree?    
         if (!inMaximalTree(tet)) 
          {
          // get index. 
          tetind = delta1 + tri4->tetrahedronIndex( tet ) - num_less_than( maxTreeStd, tri4->tetrahedronIndex( tet ) );
          if ( (tet -> getEmbedding(0).getPentachoron() == currPen) && (tet -> getEmbedding(0).getTetrahedron() == currPenFace) )
           relator.addTermFirst( tetind, 1 ); else relator.addTermFirst( tetind, -1 );
          }
        } 
       // end pad
       embit--;
       currPenFace= (*embit).getVertices()[3];
       tet = currPen->getTetrahedron(currPenFace);
       if (!tet->isBoundary()) std::cout<<"ERROR (2)!!"; std::cout.flush();
       if (!inMaximalTree(tet))
        {
         tetind = delta1 + tri4->tetrahedronIndex( tet ) - num_less_than( maxTreeStd, tri4->tetrahedronIndex( tet ) );
         relator.addTermFirst( tetind, 1 ); // all 1-cells dual to boundary tets assumed oriented outwards
        }

       // finish
       NGroupExpression* relate;
       relate = new NGroupExpression(relator);
       pres.addRelation(relate);
      }
    }// that finishes interior cells dual to faces. 

   // now for boundary dual 2-cells TODO
   // run through bcIx[1], for each edge call getEmbeddings(), this describes a disc and we need to crawl around the boundary.
   Dim4Edge* edg(NULL);
   for (unsigned long i=0; i<bcIx[1].size(); i++)
    {
     edg = tri4->getEdge( bcIx[1][i] );
     // okay, now we should look through edg->getEmbeddings() until we find an embedding with getVertices[0,1,?,?] representing a boundary tetrahedron, 
     //  this will be our initial tetrahedron that we build off of
     

     // once we've found that, on pick a face, call getEmbeddings() and take the first and last, build a cyclic list...
     // we know we're at the end when we've found the first tetrahedron



    }
   // end boundary dual 2-cells

   // now for ideal dual 2-cells TODO
   // run through icIx[2]
   // end ideal dual 2-cells

   // lower_bound on a sorted list returns first object greater than or equal to. 
   // upper_bound                          first object strictly greater than
   // so an edge's index in the wedge of circles for the pi1-computation consists of its index
   // in the appropriate edge list, minus the corresponding upper_bound - first index search, appropriately
   // shifted. 

   // co-dim 2 simplices which are in the boundary, oriented according into getEmbedding(0)
   //     (and from getEmbedding(last)
   // co-dim 2 ideal simplices in ideal boundary -- oriented according to getEmbedding(0) from the 
   //     simplex of which it is the ideal end. 
   // edges from top-dim simplex to barycentre of ideal end. 

   // the relators correspond to the boundary of the 2-cells.  There are several varieties:
   //  2-cells dual to co-dimension 2 simplices in the triangulation
  }
 else
  {

  }


// TODO erase this line and do something else. debuggin purposes only.  We're so far okay for manifolds with no boundary.
pres.intelligentSimplify();
pres.writeTextLong(std::cout);
}

} // namespace regina


