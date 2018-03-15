/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

#include "algebra/cellulardata.h"
#include "maths/perm.h"
#include "algebra/grouppresentation.h"
#include "algebra/homgrouppresentation.h"
#include "triangulation/dim2.h"

#include <map>
#include <list>
#include <cmath>

#include <iostream>
#include <sstream>

/**
 *  This file sets up the basic data required for fundamental group computations
 *  in a given triangulation.  Fundamental groups are computed by first finding 
 *  maximal forests in the dual skeleton to the triangulation.  Since induced 
 *  maps from the boundary components to the manifold are to be computed, this 
 *  skeleton is constructed inductively as a maximal forest in the dual boundary
 *  skeleton (both for the ideal boundary and standard boundary), then it is 
 *  extended to the ambient manifold.  From this all the pi1 data and maps 
 *  between the various pi1's can be computed. 
 */

namespace regina {


// counts number of elements in thelist less than obj
unsigned long num_less_than(const std::set<unsigned long> &thelist, 
    const unsigned long &obj)
{
 unsigned long retval = 0;
 std::set<unsigned long>::const_iterator i;
 for (i=thelist.begin(); i!=thelist.end(); i++) 
    {  if ( (*i) < obj ) retval++; else return retval; }
 return retval; 
}

// dim4
bool CellularData::inMaximalTree(const Face<4,3>* tet) const
{
 if (!binary_search( nicIx[3].begin(), nicIx[3].end(), 
  tet->index() ) ) return false; // not in list of tetrahedra!
 unsigned long I = lower_bound( nicIx[3].begin(), nicIx[3].end(), 
    tet->index() ) - nicIx[3].begin();
 if (!binary_search( maxTreeStd.begin(), maxTreeStd.end(), I ) ) return false;
 return true;
}

bool CellularData::inMaximalTree(const Face<4,2>* fac) const
{
 if (!binary_search(bcIx[2].begin(), bcIx[2].end(), 
    fac->index() ) ) return false; // not in list of faces!
 unsigned long I = lower_bound( bcIx[2].begin(), bcIx[2].end(), 
    fac->index() ) - bcIx[2].begin();
 if (!binary_search( maxTreeStB.begin(), maxTreeStB.end(), I ) ) return false;
 return true; 
}

bool CellularData::inMaximalTree(
    const Face<4,3>* tet, unsigned long num) const
{
 if (!binary_search(icIx[2].begin(), icIx[2].end(), 
    4*tet->index() + num ) ) return false; 
 // not in list of ideal ends of tetrahedra
 unsigned long I = lower_bound( icIx[2].begin(), icIx[2].end(), 
    4*tet->index() + num ) - icIx[2].begin();
 if (!binary_search( maxTreeIdB.begin(), maxTreeIdB.end(), I ) ) return false; 
 return true;
}

bool CellularData::inMaximalTree(
    const Simplex<4>* pen, unsigned long num) const
{
 if (!binary_search(icIx[3].begin(), icIx[3].end(), 
    5*pen->index() + num ) ) return false; 
 unsigned long I = lower_bound( icIx[3].begin(), icIx[3].end(), 
    5*pen->index() + num ) - icIx[3].begin();
 if (!binary_search( maxTreeSttIdB.begin(), maxTreeSttIdB.end(), I ) ) 
    return false;
 return true;
}


// dim3 
bool CellularData::inMaximalTree(const Face<3,2>* fac) const
{
 if (!binary_search( nicIx[2].begin(), nicIx[2].end(), 
    fac->index() ) ) return false; // not in list of faces!
 unsigned long I = lower_bound( nicIx[2].begin(), nicIx[2].end(), 
    fac->index() ) - nicIx[2].begin();
 if (!binary_search( maxTreeStd.begin(), maxTreeStd.end(), I ) ) return false;
 return true;
}

bool CellularData::inMaximalTree(const Face<3,1>* edg) const
{
 if (!binary_search(bcIx[1].begin(), bcIx[1].end(), 
    edg->index() ) ) return false; // not in list of edges!
 unsigned long I = lower_bound( bcIx[1].begin(), bcIx[1].end(), 
    edg->index() ) - bcIx[1].begin();
 if (!binary_search( maxTreeStB.begin(), maxTreeStB.end(), I ) ) return false;
 return true; 
}

bool CellularData::inMaximalTree(const Face<3,2>* fac, unsigned long num) const
{
 if (!binary_search(icIx[1].begin(), icIx[1].end(), 
    3*fac->index() + num ) ) return false; 
    // not in list of ideal ends of faces
 unsigned long I = lower_bound( icIx[1].begin(), icIx[1].end(), 
    3*fac->index() + num ) - icIx[1].begin();
 if (!binary_search( maxTreeIdB.begin(), maxTreeIdB.end(), I ) ) return false; 
 return true;
}

bool CellularData::inMaximalTree(const Simplex<3>* tet, 
                                  unsigned long num) const
{
 if (!binary_search(icIx[2].begin(), icIx[2].end(), 
    4*tet->index() + num ) ) return false; 
 unsigned long I = lower_bound( icIx[2].begin(), icIx[2].end(), 
    4*tet->index() + num ) - icIx[2].begin();
 if (!binary_search( maxTreeSttIdB.begin(), maxTreeSttIdB.end(), I ) ) 
    return false;
 return true;
}

/**
 * Normal orientations for cells Regina does not naturally give normal 
 * orientations to.  This routine also sets up an indexing so that one can 
 * determine from an (ideal) skeletal object which boundary component it lies 
 * in. 
 *
 * normalsDim4BdryFaces is a vector that assigns to the i-th boundary face 
 * [tri4->triangle(bcIx[2][i])] the two boundary tetrahedra that contain 
 * it and the face number of the face in the tetrahedron. 
 *
 * normalsDim4BdryEdges is a vector that assigns to the i-th boundary edge 
 * [tri4->triangle(bcIx[1][i])] the circle of tetrahedra incident to that 
 * edge, with edginc[2] and edginc[3] forming the normal orientation in 
 * agreement with the indexing of tet. 
 *
 * normalsDim4BdryVertices is a vector that assigns to the i-th boundary vertex 
 * [tri4->vertex(bcIx[0][i])] the sphere of tetrahedra incident to that 
 * vertex, with vrtinc[1], vrtinc[2], vrtinc[3] forming a normal orientation.
 *
 * normalsDim3BdryEdges is a vector that assigns to the i-th boundary face 
 * [tri3->edge(bcIx[1][i])] the two boundary faces that contain it and 
 * the edge number of the edge in the Face<3,2>.  
 *
 * normalsDim3BdryVertices is a vector that assigns to the i-th boundary vertex 
 * [tri3->vertex(bcIx[0][i])] the circle of faces incident to that vertex, 
 * with vrtinc[1] and vrtinc[2] forming the normal orientation in agreement 
 * with the indexing of face. 
 */
void CellularData::buildExtraNormalData()
{
 if (tri4!=NULL)
 { 
  normalsDim4BdryFaces.resize( bcIx[2].size() );
  normalsDim4BdryEdges.resize( bcIx[1].size() );
  normalsDim4BdryVertices.resize( bcIx[0].size() );

  // iterate through boundary components
  const std::vector<BoundaryComponent<4>*> 
    bComps(tri4->boundaryComponents());
  for (Dim4Triangulation::BoundaryComponentIterator 
       bcit=tri4->boundaryComponents().begin(); 
       bcit!=tri4->boundaryComponents().end(); bcit++) 
  if ( !(*bcit)->isIdeal() )
   {
    const Triangulation<3>* bTriComp( (*bcit)->build() );
     
    // run through vertices, edges, faces in bTriComp, find corresponding Dim4 
    // object, get its bcIx, call it j for the same objects get incidence data, 
    // compute corresponding bcIx indices, this allows us to fill out
    //  normalsDim4Bdry***[j]

    // iterate through bTriComp vertices
    for (Triangulation<3>::VertexIterator vit=bTriComp->vertices().begin(); 
         vit!=bTriComp->vertices().end(); vit++)
     {
      unsigned long I = bcIxLookup( (*bcit)->vertex( (*vit)->index() ) );
      // find vit's embeddings

      // vertex embeddings in bdry triangulation
      dim4BoundaryVertexInclusion dim4bvrtInc;
      for (auto& emb : *(*vit))
        {
         dim4bvrtInc.tet.push_back( (*bcit)->tetrahedron( 
          emb.tetrahedron()->index() ) );
         dim4bvrtInc.vrtnum.push_back( emb.vertex() );
         dim4bvrtInc.vrtinc.push_back( emb.vertices() );
        }       
      normalsDim4BdryVertices[I] = dim4bvrtInc; 
     }    

    // iterate through bTriComp edges
    for (Triangulation<3>::EdgeIterator eit=bTriComp->edges().begin(); 
         eit!=bTriComp->edges().end(); eit++)
     {
      unsigned long I = bcIxLookup( (*bcit)->edge( (*eit)->index() ) );
      // find eit's embeddings
 
      dim4BoundaryEdgeInclusion dim4bedgInc;
      // find corresponding tri4 data
      for (auto& emb : *(*eit))
        {
         dim4bedgInc.tet.push_back( (*bcit)->tetrahedron( 
          emb.tetrahedron()->index() ) );
         dim4bedgInc.edgenum.push_back( emb.edge() );
         dim4bedgInc.edginc.push_back( emb.vertices() );
        }

      normalsDim4BdryEdges[I] = dim4bedgInc; 
     }    

    // iterate through bTriComp faces
    for (Triangulation<3>::TriangleIterator fit=bTriComp->triangles().begin(); 
         fit!=bTriComp->triangles().end(); fit++)
     {
      unsigned long I = bcIxLookup( (*bcit)->triangle( 
        (*fit)->index() ) );

      dim4BoundaryFaceInclusion dim4bfacInc;
      dim4bfacInc.firsttet = (*bcit)->tetrahedron(
                (*fit)->embedding(0).tetrahedron()->index() );
      dim4bfacInc.secondtet = (*bcit)->tetrahedron(
                (*fit)->embedding(1).tetrahedron()->index() );
      dim4bfacInc.firstfacnum = (*fit)->embedding(0).face();
      dim4bfacInc.secondfacnum = (*fit)->embedding(1).face();

      normalsDim4BdryFaces[I] = dim4bfacInc; 
        // is this array resized properly? need to ensure that...
     }    
   }
 } // end tri4 
 else
 { // tri3 construct normalsDim3BdryEdges
  normalsDim3BdryEdges.resize( bcIx[1].size() );
  // std::vector< dim3BoundaryEdgeInclusion >   normalsDim3BdryEdges;
  //
  // struct dim3BoundaryEdgeInclusion
  //  { Face<3,2> *firstfac, *secondfac;
  //    unsigned long firstedgnum, secondedgnum; };
  //
  // construct normalsDim3BdryEdges
  // 
  // The strategy here is to get every incident tetrahedron using the 
  // edg->embedding call, and since Regina has them cyclically ordered 
  // with a normal orientation already, take the first and last and the 
  // appropriate face of each.  Ok DONE. 
  
  // TODO experiment ?MIGHT WORK?
  const std::vector<BoundaryComponent<3>*> 
    bComps(tri3->boundaryComponents());
  for (Triangulation<3>::BoundaryComponentIterator 
       bcit=tri3->boundaryComponents().begin(); 
       bcit!=tri3->boundaryComponents().end(); bcit++) 
  if ( !(*bcit)->isIdeal() )
  {
   const Triangulation<2>* bTriComp( (*bcit)->build() );
   // TODO: test iterate through vertices of bTriComp
   for (auto& vit : bTriComp->vertices())
    {
      unsigned long I = bcIxLookup( (*bcit)->vertex( vit->index() ) );
      // find vit's embeddings

      // vertex embeddings in bdry triangulation
      dim3BoundaryVertexInclusion dim3bvrtInc;
      // auto is cleaner!
      //for (FaceList<2,0>::Iterator vit=bTriComp->vertices().begin(); 
      //    vit!=bTriComp->vertices().end(); vit++)
      for (auto& emb : *vit)
        {
         dim3bvrtInc.face.push_back( (*bcit)->triangle( 
          emb.tetrahedron()->index() ) );
         dim3bvrtInc.vrtnum.push_back( emb.vertex() );
         dim3bvrtInc.vrtinc.push_back( emb.vertices() );
        }       
      normalsDim3BdryVertices[I] = dim3bvrtInc; 
    }
  
  // edges second TODO
  
 } // end tri3
 
 // figure out number of standard vs. ideal boundary components, also compute 
 // a vector which describes the map (boundary faces) --> (boundary components 
 // they belong to)
 numStdBdryComps=0; numIdealBdryComps=0; 
 if (tri4!=NULL)
  {
  stdBdryCompIndexCD1.resize( bcIx[2].size() ); 
  idBdryCompIndexCD1.resize( icIx[2].size() );
  for (unsigned long i=0; i<tri4->countBoundaryComponents(); i++) 
   { // now we can run through all the faces in this boundary component, 
     //  and fill out the appropriate array stdBdryCompIndex
    const BoundaryComponent<4>* bcomp( tri4->boundaryComponent(i) );
    if (!bcomp->isIdeal())
     {
      for (unsigned long j=0; j<bcomp->getNumberOfTriangles(); j++)
        stdBdryCompIndexCD1[bcIxLookup( 
            bcomp->triangle(j) )] = numStdBdryComps; 
       numStdBdryComps++; 
     }
    else // bcomp *is* ideal
     {
      // run through icIx[2] check to see if the corresponding 
      //  ideal vertex is in bcomp
      for (unsigned long j=0; j<icIx[2].size(); j++)
       {
        const Face<4,3>* tet( tri4->tetrahedron( icIx[2][j] / 4 ) ); 
        const Face<4,0>* vrt( tet->vertex(icIx[2][j] % 4 ) );
        if (vrt->isIdeal()) 
         if (tri4->boundaryComponentIndex( vrt->boundaryComponent() )==i)
         idBdryCompIndexCD1[j] = numIdealBdryComps;
       }
      numIdealBdryComps++;
     }
   }
  }
 else // tri3!=NULL
  {
  stdBdryCompIndexCD1.resize( bcIx[1].size() ); 
  idBdryCompIndexCD1.resize( icIx[1].size() );
  for (unsigned long i=0; i<tri3->countBoundaryComponents(); i++) 
   { // now we can run through all the faces in this boundary component, 
     //  and fill out the appropriate array stdBdryCompIndex
    const NBoundaryComponent* bcomp( tri3->boundaryComponent(i) );
    if (!bcomp->isIdeal())
     {
      for (unsigned long j=0; j<bcomp->getNumberOfEdges(); j++)
        stdBdryCompIndexCD1[bcIxLookup( bcomp->edge(j) )] = numStdBdryComps; 
       numStdBdryComps++; 
      }
    else // bcomp *is* ideal
     {
      // run through icIx[1] check to see if the corresponding 
      //  ideal vertex is in bcomp
      for (unsigned long j=0; j<icIx[1].size(); j++)
       {
        const Face<3,2>* fac( tri3->face( icIx[1][j] / 3 ) ); 
        const Face<3,0>* vrt( fac->vertex(icIx[1][j] % 3 ) );
        if (vrt->isIdeal()) 
         if (tri3->boundaryComponentIndex( vrt->boundaryComponent() )==i)
          idBdryCompIndexCD1[j] = numIdealBdryComps;
       }
      numIdealBdryComps++;
     }
   }
  }
} // end buildExtraNormalData()

/**
 * This routine runs through the dual 1-skeleton to the triangulation, building 
 * a maximal tree in the dual 1-skeleton to the triangulation which restricts 
 * to a maximal tree in any boundary component, ideal or "standard" boundary.
 *
 * This algorithm also sets up the indexing of cells on the boundary components. 
 * At present these are the internal vectors stdBdryCompIndexCD1 and 
 * idBdryCompIndexCD1
 *
 * Assumes triangulation is connected.
 */
void CellularData::buildMaximalTree()
{
 if ( maxTreeStd.size() != 0 ) return; // don't bother calling the routine twice
 // Iterate: walk through dual 1-skeleton until we hit an (ideal) boundary 
 // component we haven't hit before.  During the iteration we keep track of 
 // several things. Priority to building (ideal) boundary max tree ahead of 
 // interior max tree.

 // 0-cells in maximal tree
 std::set< unsigned long > visitedZ, visitedBd, visitedId; 
 // indexed by             nicIx[n], bcIx[n-1], icIx[n-1] respectively.
 std::set< unsigned long > newVertexListS, newVertexListB, newVertexListI; 
 // indexed by             nicIx[n],       bcIx[n-1],      icIx[n-1]
 visitedZ.insert(0); newVertexListS.insert(0); 
 // seed process starting at pentachoron / tetrahedron 0

 if (tri4 != NULL)
 {  // let's seed the process and start in pentachoron 0.
 while ( (newVertexListS.size() > 0) || (newVertexListB.size() > 0) || 
         (newVertexListI.size() > 0) ) // unexplored vertices...
  {
  ideal_boundary_loop4:
  // iterator to go through unexplored vertices
  std::set< unsigned long >::iterator unexploredV;
  // first check to see if there are ideal vertices unexplored
  // perhaps we should exiting when newVertexListI.size() == 0 ?? 
  while (newVertexListI.size() > 0)
   {
    unexploredV = newVertexListI.begin();

    // *unexploredV is the icIx[3]-index of the ideal dual 0-cell
    const Simplex<4>* pen(NULL); unsigned long idvnum; 
     const Face<4,3>* septet(NULL); const Simplex<4>* adjpen(NULL);
    Perm<5> adjglue, tetmap;

    pen = tri4 -> pentachoron( icIx[3][*unexploredV]/5 );
    idvnum = icIx[3][*unexploredV] % 5;
    for (unsigned long i=1; i<5; i++) // look at adjacent pentachora
     {
      septet = pen -> tetrahedron( int( (idvnum + i) % 5) );
      adjpen = pen -> adjacentPentachoron( int( (idvnum + i) % 5) );
      adjglue = pen -> adjacentGluing( int( (idvnum + i) % 5) );
      tetmap = pen -> tetrahedronMapping( int( (idvnum + i) % 5) );
      // let's determine the number of adjpen
      unsigned long I = icIxLookup( adjpen, adjglue[idvnum] ); 
      // make J into the index of the ideal boundary face...
      unsigned long J = icIxLookup( septet, tetmap.preImageOf( idvnum ) );
      // check to see if I is in visitedId
      if (!binary_search( visitedId.begin(), visitedId.end(), I) ) // not found
       { visitedId.insert(I); newVertexListI.insert(I); maxTreeIdB.insert(J); }
     }
    newVertexListI.erase(unexploredV); 
    // increment unexploredV. return the unincremented iterator for erase arg...
   }

  standard_boundary_loop4:
  // then check to see if there are standard boundary vertices unexplored
  while (newVertexListB.size() > 0)
   {
    unexploredV = newVertexListB.begin();

    // *unexploredV is the bcIx[3]-index of the standard boundary dual 0-cell
    const Face<4,3>* btet(NULL); 
    btet = tri4 -> tetrahedron( bcIx[3][*unexploredV] ); // btet

    for (unsigned long i=0; i<4; i++) // cross all faces
      {
       const Face<4,2>* fac( btet->triangle(i) ); 
       // lookup in normalsDim4BdryFaces, for that we need the bcIx[2] index.
       unsigned long facidx( bcIxLookup( fac ) );
       // one of normalsDim4BdryFaces[facidx] first/second represents this
       // tetrahedron and face, lets walk to the other. 
       if ( (normalsDim4BdryFaces[facidx].firsttet == btet) && 
            (normalsDim4BdryFaces[facidx].firstfacnum == i) ) 
        { // ensure second tet hasn't been touched, if so record the fac.  
          unsigned long newbtetidx ( bcIxLookup( 
            normalsDim4BdryFaces[facidx].secondtet ) );
          if (!binary_search( visitedBd.begin(), visitedBd.end(), newbtetidx ) )
          {
            visitedBd.insert(newbtetidx); 
            newVertexListB.insert(newbtetidx); 
            maxTreeStB.insert( facidx );             
          }
        }
       else // this btet is the secondtet.
        {
          unsigned long newbtetidx ( bcIxLookup( 
            normalsDim4BdryFaces[facidx].firsttet ) );
          if (!binary_search( visitedBd.begin(), visitedBd.end(), newbtetidx ) )
           { 
            visitedBd.insert(newbtetidx); 
            newVertexListB.insert(newbtetidx); 
            maxTreeStB.insert( facidx ); 
           }
        }
      }
     newVertexListB.erase(unexploredV); 
     // increment unexploredV but return the unincremented iterator...
   }


  // then check to see if there are standard vertices unexplored
  while (newVertexListS.size() > 0)
    {
    unexploredV = newVertexListS.begin();

    // *unexploredV is the nicIx[4]-index of the dual 0-cell
    const Simplex<4>* pen(NULL); 
    Perm<5> adjglue, tetmap;
    pen = tri4 -> pentachoron( *unexploredV );

    // step 1 look for ideal connectors.  If one found, record and exit loop 
    for (unsigned long i=0; i<5; i++) 
        if ( pen -> vertex(int(i)) -> isIdeal() )
     {
      unsigned long I = icIxLookup( pen, i );
      // check to see if I in visitedId
      if (!binary_search( visitedId.begin(), visitedId.end(), I) ) // not found
       { 
         visitedId.insert(I); 
         newVertexListI.insert(I); 
         maxTreeSttIdB.insert(I);  
         goto ideal_boundary_loop4; } // found new ideal boundary, loop back!
     }

    // step 2 look for standard boundary connectors. found, record and exit loop
    for (unsigned long i=0; i<5; i++) 
        if ( pen -> tetrahedron(int(i)) -> isBoundary() )
     {
      const Face<4,3>* btet( pen -> tetrahedron(i) );
      unsigned long I = bcIxLookup( btet );
      unsigned long J = nicIxLookup( btet );
      if (!binary_search( visitedBd.begin(), visitedBd.end(), I ) ) // not found
       { 
         visitedBd.insert(I);  
         newVertexListB.insert(I); 
         maxTreeStd.insert(J);
         goto standard_boundary_loop4; 
       }
     }

    // step 3 look for internal connectors. end of the loop
    for (unsigned long i=0; i<5; i++) 
        if ( !pen -> tetrahedron(i) -> isBoundary() )
     {
      unsigned long I = tri4->pentachoronIndex( 
        pen -> adjacentPentachoron( i ) );
      unsigned long J = nicIxLookup( pen -> tetrahedron( i ) );
      if (!binary_search( visitedZ.begin(), visitedZ.end(), I ) ) // not found
       { 
         visitedZ.insert(I); 
         maxTreeStd.insert(J); 
         newVertexListS.insert(I); 
       }
     }
    newVertexListS.erase(unexploredV); 
    }
   } // end big while loop
 } // end tri4 
 else
 { // tri3
 while ( (newVertexListS.size() > 0) || (newVertexListB.size() > 0) || 
         (newVertexListI.size() > 0) ) 
  // unexplored vertices. Contrast with visitedZ, visitedBd, visitedId
  {
  ideal_boundary_loop3:
  // iterator to go through unexplored vertices
  std::set< unsigned long >::iterator unexploredV;
  // first check to see if there are ideal vertices unexplored
  while (newVertexListI.size() > 0)
   {
    unexploredV = newVertexListI.begin();

    // *unexploredV is the icIx[2]-index of the ideal dual 0-cell
    const Simplex<3>* tet(NULL); unsigned long idvnum; 
    const Face<3,2>* sepfac(NULL); const Simplex<3>* adjtet(NULL);
    Perm<4> adjglue, facmap;

    tet = tri3 -> tetrahedron( icIx[2][*unexploredV]/4 );
    idvnum = icIx[2][*unexploredV] % 4;
    for (unsigned long i=1; i<4; i++) // look at adjacent pentachora
     {
      sepfac = tet -> face( int( (idvnum + i) % 4) );
      adjtet = tet -> adjacentTetrahedron( int( (idvnum + i) % 4) );
      adjglue = tet -> adjacentGluing( int( (idvnum + i) % 4) );
      facmap = tet -> faceMapping( int( (idvnum + i) % 4) );
        // dual ideal 0-cell value, should be indexed by icIx[3]
      unsigned long I = icIxLookup( adjtet, adjglue[idvnum] );
        // make J into the index of the ideal boundary edge...
      unsigned long J = icIxLookup( sepfac, facmap.preImageOf( idvnum ) );
      // check to see if I is in visitedId
      if (!binary_search( visitedId.begin(), visitedId.end(), I) ) // not found
       { 
        visitedId.insert(I); 
        newVertexListI.insert(I); 
        maxTreeIdB.insert(J); 
       }
     }
    newVertexListI.erase(unexploredV); 
   }

  standard_boundary_loop3:
  // then check to see if there are standard boundary vertices unexplored
  while (newVertexListB.size() > 0)
   {
    unexploredV = newVertexListB.begin();

    // *unexploredV is the bcIx[3]-index of the standard boundary dual 0-cell
    const Face<3,2>* bfac(NULL); 
    bfac = tri3 -> face( bcIx[2][*unexploredV] ); // bfac

    for (unsigned long i=0; i<3; i++) // cross all faces
      {
       const Face<3,1>* edg( bfac->edge(i) ); 
       // look this up in normalsDim4BdryFaces, 
       //  for that we need the bcIx[1] index.
       unsigned long edgidx( bcIxLookup( edg ) );
       // one of normalsDim4BdryFaces[edgidx] first/second represents this 
       // tetrahedron and face, lets walk to the other. 
       if ( (normalsDim3BdryEdges[edgidx].firstfac == bfac) && 
            (normalsDim3BdryEdges[edgidx].firstedgnum == i) ) 
        { // ensure second tet hasn't been touched, if so record the fac.  
          unsigned long newbfacidx ( bcIxLookup( 
            normalsDim3BdryEdges[edgidx].secondfac ) );
          if (!binary_search( visitedBd.begin(), visitedBd.end(), newbfacidx ) )
          { 
            visitedBd.insert(newbfacidx); 
            newVertexListB.insert(newbfacidx); 
            maxTreeStB.insert( edgidx ); 
          }
        }
       else // this bfac is the secondfac.
        {
          unsigned long newbfacidx ( bcIxLookup( 
                normalsDim3BdryEdges[edgidx].firstfac ) );
          if (!binary_search( visitedBd.begin(), visitedBd.end(), newbfacidx ) )
           { 
             visitedBd.insert(newbfacidx); 
             newVertexListB.insert(newbfacidx); 
             maxTreeStB.insert( edgidx ); 
           }
        }
      }
     newVertexListB.erase(unexploredV); 
   }

  // then check to see if there are standard vertices unexplored
  while (newVertexListS.size() > 0)
    {
    unexploredV = newVertexListS.begin();

    // *unexploredV is the nicIx[4]-index of the dual 0-cell
    const Simplex<3>* tet(NULL); 
    Perm<5> adjglue, facmap;
    tet = tri3 -> tetrahedron( *unexploredV );

    // step 1 look for ideal connectors.  If one found, record and exit loop 
    for (unsigned long i=0; i<4; i++) 
        if ( tet -> vertex(int(i)) -> isIdeal() )
     {
      unsigned long I( icIxLookup( tet, i ) ); 
      // check to see if I in visitedId
      if (!binary_search( visitedId.begin(), visitedId.end(), I) ) // not found
       { 
         visitedId.insert(I); 
         newVertexListI.insert(I); 
         maxTreeSttIdB.insert(I);  
         goto ideal_boundary_loop3; 
       } // found new ideal boundary, loop back!
     }

    // step 2 look for standard boundary connectors.  exit loop
    for (unsigned long i=0; i<4; i++) 
        if ( tet -> face(int(i)) -> isBoundary() )
     {
      const Face<3,2>* bfac( tet -> face(int(i)) );
      unsigned long I ( bcIxLookup( bfac ) );
      unsigned long J (nicIxLookup( bfac ) );
      if (!binary_search( visitedBd.begin(), visitedBd.end(), I ) ) // not found
       { 
         visitedBd.insert(I);  
         newVertexListB.insert(I); 
         maxTreeStd.insert(J);
         goto standard_boundary_loop3; 
       }
     }

    // step 3 look for internal connectors. way to end of the loop
    for (unsigned long i=0; i<4; i++) 
        if ( !tet -> face(int(i)) -> isBoundary() )
     {
      unsigned long I = tri3->tetrahedronIndex( 
        tet -> adjacentTetrahedron( int(i) ) );
      unsigned long J ( nicIxLookup( tet -> face( int(i) ) ) );
      if (!binary_search( visitedZ.begin(), visitedZ.end(), I ) ) // not found
       { 
        visitedZ.insert(I); 
        maxTreeStd.insert(J); 
        newVertexListS.insert(I); 
       }
     }

    newVertexListS.erase(unexploredV); 
    // increment unexploredV. return the unincremented iterator for erase arg...
    }
   } // end big while loop
  } // end tri3

 // other data required for presentations of fundamental groups of the 
 //  boundary components
 stdBdryPi1Gen.resize( numStdBdryComps );
 idBdryPi1Gen.resize( numIdealBdryComps );
 // run through all dual boundary 1-cells, 
 // if not in max tree append to stdBdryPi1Gen, etc. 
 if (tri4 != NULL) 
  { for (unsigned long i=0; i<bcIx[2].size(); i++) 
        if (!inMaximalTree( tri4->triangle(bcIx[2][i]) ) )
       stdBdryPi1Gen[ stdBdryCompIndexCD1[i] ].push_back(i); 
    for (unsigned long i=0; i<icIx[2].size(); i++) 
        if (!inMaximalTree( tri4->tetrahedron(icIx[2][i]/4), icIx[2][i]%4) )
       idBdryPi1Gen[ idBdryCompIndexCD1[i] ].push_back(i);
  }
 else // tri3 != NULL
  { for (unsigned long i=0; i<bcIx[1].size(); i++) 
        if (!inMaximalTree( tri3->edge(bcIx[1][i]) ) )
       stdBdryPi1Gen[ stdBdryCompIndexCD1[i] ].push_back(i); 
    for (unsigned long i=0; i<icIx[1].size(); i++) 
        if (!inMaximalTree( tri3->face(icIx[1][i]/3), icIx[1][i]%3) )
       idBdryPi1Gen[ idBdryCompIndexCD1[i] ].push_back(i);
  }
}// end buildMaximalTree()



void CellularData::buildFundGrpPres() const
{
 GroupPresentation pres;
 std::vector< GroupPresentation > stdBdryPi1( numStdBdryComps );
 std::vector< GroupPresentation > idBdryPi1( numIdealBdryComps );

 for (unsigned long i=0; i<stdBdryPi1.size(); i++)
  stdBdryPi1[i].addGenerator( stdBdryPi1Gen[i].size() );
 for (unsigned long i=0; i<idBdryPi1.size(); i++)
  idBdryPi1[i].addGenerator( idBdryPi1Gen[i].size() ); 

 unsigned long delta0( numNonIdealBdryCells[tri3 ? 1 : 2] - 
                        maxTreeStB.size() ); 
 unsigned long delta1( delta0 + numIdealCells[tri3 ? 1 : 2] - 
                       maxTreeIdB.size() );
 unsigned long delta2( delta1 + numNonIdealCells[tri3 ? 2 : 3] - 
                       maxTreeStd.size() );
 unsigned long delta3( delta2 + numIdealCells[tri3 ? 2 : 3] - 
                       maxTreeSttIdB.size() ); 
    // to know what is out of range
 pres.addGenerator( delta3 );// we've set the generators for the presentation.
 // for now the group presentation will consist of: a list of numbers for 
 // the generators, a list of words in the generators corresponding to the 
 // relators. the generators consist of edges of the dual 1-skeleton not in 
 // the maximal tree

 if (tri4)
  {
   Face<4,3>* currTet (NULL); Simplex<4>* currPen (NULL); 
        Face<4,3>* tet (NULL); unsigned long currPenFace;

   // start finding relators. Relators dual faces. There are two types. 
   //  1) Non-boundary. 
   //  2) Boundary. 

   for (Dim4Triangulation::TriangleIterator fac = tri4->triangles().begin(); 
        fac!=tri4->triangles().end(); fac++)
    {
     GroupExpression relator; 

     if ( !(*fac)->isBoundary() ) // non-boundary -- interior 2-cell
      {
       std::deque<Dim4TriangleEmbedding>::const_iterator embit;
       for (embit = (*fac)->embeddings().begin();
                    embit != (*fac)->embeddings().end(); embit++)
        { // determine whether or not the embedding coincides with the 
          // normal or of the tet.
         currPen = (*embit).pentachoron();
         currPenFace = (*embit).vertices()[4]; 
         tet = currPen->tetrahedron(currPenFace);  
         // and is the tet in the maximal tree?    
         if (!inMaximalTree(tet)) 
          {
          // get index. 
          unsigned long tetind = delta1 + tri4->tetrahedronIndex( tet ) - 
                   num_less_than( maxTreeStd, tri4->tetrahedronIndex( tet ) );
          if ( (tet -> embedding(1).pentachoron() == currPen) && 
               (tet -> embedding(1).tetrahedron() == currPenFace) )
           relator.addTermFirst( tetind, 1 ); 
          else relator.addTermFirst( tetind, -1 );//oriented from emb 0 to emb 1
          }
        } 
       GroupExpression* relate ( new GroupExpression(relator) );
       pres.addRelation(relate);
      }
     else // boundary face -- cell half on std boundary, half in interior
      {
       unsigned long tetind;
       const Dim4TriangleEmbedding facemb = (*fac)->embedding(0);
       currPen = facemb.pentachoron();
       currPenFace = facemb.vertices()[4]; 
       tet = currPen->tetrahedron(currPenFace); // boundary tet we start with
       unsigned long tetfacnum = tet->embedding(0).vertices().preImageOf( 
        facemb.vertices()[3] );
       if (!tet->isBoundary()) 
        std::cout<<"ERROR (unexpected tetrahedron) "<<std::endl; 

       if (!inMaximalTree(*fac))
        {
         // orientation? 
         unsigned long I ( bcIxLookup ( *fac ) );
         if ( (normalsDim4BdryFaces[I].secondtet == tet) && 
              (normalsDim4BdryFaces[I].secondfacnum == tetfacnum) )
          relator.addTermFirst( I - num_less_than( maxTreeStB, I ),  1 );
         else
          relator.addTermFirst( I - num_less_than( maxTreeStB, I ), -1 );
        }
   
       // main loop
       for (std::deque<Dim4TriangleEmbedding>::const_iterator 
            embit=(*fac)->embeddings().begin();
            embit != (*fac)->embeddings().end(); embit++)
        { // now we have to determine whether or not the embedding 
          //  coincides with the normal or of the tet.
         currPen = (*embit).pentachoron();
         currPenFace = (*embit).vertices()[4]; 
         tet = currPen->tetrahedron(currPenFace);  
         // and is the tet in the maximal tree?    
         if (!inMaximalTree(tet)) 
          {
          // get index. 
          tetind = delta1 + tri4->tetrahedronIndex( tet ) -
             num_less_than( maxTreeStd, tri4->tetrahedronIndex( tet ) );
          int sign;
          if (embit == (*fac)->embeddings().begin() )
           { sign = -1; }
          else
           {
            if ( (tet -> embedding(0).pentachoron() == currPen) && 
                 (tet -> embedding(0).tetrahedron() == currPenFace) )
             sign = -1; else sign = 1; 
           }
           relator.addTermFirst( tetind, sign ); 
          }
        } 

       // end pad
       currPenFace= (*fac)->embeddings().back().vertices()[3];
       tet = currPen->tetrahedron(currPenFace);
       if (!tet->isBoundary()) 
        std::cout<<"ERROR (unexpected tetrahedron) "<<std::endl;
       if (!inMaximalTree(tet))
        {
         tetind = delta1 + tri4->tetrahedronIndex( tet ) - 
            num_less_than( maxTreeStd, tri4->tetrahedronIndex( tet ) );
         relator.addTermFirst( tetind, 1 ); 
         // all 1-cells dual to boundary tets assumed oriented outwards
        }

       // finish
       GroupExpression* relate( new GroupExpression(relator) );
       pres.addRelation(relate);
      }
    }// that finishes interior cells dual to faces. 

   // now for boundary dual 2-cells -- pure boundary relator. run through 
   // bcIx[1], for each edge call embeddings(), this describes a disc 
   //  and we need to crawl around the boundary.
   Face<4,1>* edg(NULL);
   for (unsigned long i=0; i<bcIx[1].size(); i++)
    {
     edg = tri4->edge( bcIx[1][i] );
     GroupExpression relator; 
     GroupExpression brelator; unsigned long bcompidx(0);
     // call normalsDim4BdryEdges[i] for bcIx[1][i] normal data.
     for (unsigned long j=0; j<normalsDim4BdryEdges[i].tet.size(); j++)
      {
       const Face<4,3>* tet( normalsDim4BdryEdges[i].tet[j] );
        // unsigned long edgnum ( normalsDim4BdryEdges[i].edgenum[j] );
       Perm<4> edginc ( normalsDim4BdryEdges[i].edginc[j] );
       // find the face that edginc[2] [3] comes out of
       const Face<4,2>* bfac (tet -> triangle( edginc[3] ) ); 
       if (!inMaximalTree(bfac))
        {
         // find this bfac's bcIx[2] index
        unsigned long bfacidx ( bcIxLookup( bfac ) );
         // what boundary component are we in? 
        bcompidx = stdBdryCompIndexCD1[ bfacidx ] ;
        unsigned long bgen = lower_bound( stdBdryPi1Gen[bcompidx].begin(), 
         stdBdryPi1Gen[bcompidx].end(), bfacidx ) - 
         stdBdryPi1Gen[bcompidx].begin();

        if ( ( normalsDim4BdryFaces[bfacidx].secondtet == tet ) && 
             ( normalsDim4BdryFaces[bfacidx].secondfacnum == edginc[3] ) )
        {
         relator.addTermFirst( bfacidx - 
            num_less_than( maxTreeStB, bfacidx ), 1 );  // + or 
         brelator.addTermFirst( bgen, 1 );	   
        }
        else
        {
         relator.addTermFirst( bfacidx - 
            num_less_than( maxTreeStB, bfacidx ), -1);  // - or
         brelator.addTermFirst( bgen, -1 );	   
        }
       }
      }
      // finish
      GroupExpression* relate ( new GroupExpression(relator) );
      pres.addRelation(relate);   

      GroupExpression* brelate ( new GroupExpression(brelator) );
      stdBdryPi1[bcompidx].addRelation(brelate);
    } // end boundary dual 2-cells

   //  now for ideal dual 2-cells, dual to ideal 1-cells in ideal boundary, 
   //  one for every icIx[1]
   for (unsigned long i=0; i<icIx[1].size(); i++)
    {
     GroupExpression relator, brelator; 
     unsigned long bcompidx(0);

     const Face<4,2>* fac ( tri4->triangle( icIx[1][i]/3 ) );
     unsigned long idEdg ( icIx[1][i] % 3 );  // ideal edge number of fac
     // lets acquire all the Dim4Pentachora incident to fac. 
     for (unsigned long j=0; j<fac->getNumberOfEmbeddings(); j++)
      {
       const Simplex<4>* pen ( fac->embedding(j).pentachoron() );
       Perm<5> facemb( fac->embedding(j).vertices() );
       // idEdg of fac represents an ideal edge.  We want to find all the ideal
       // incident tets and mark them appropriately. so we find fac's embeddings
       // in pentachoral and look up the relevant ideal tets. So we are going 
       // across the tetrahedron in pen whose vertices are marked by 
       // facemb[0][1][2] and [3] ie tetrahedron labelled by facemb[4]. 
       const Face<4,3>* tet ( pen->tetrahedron( facemb[4] ) );
       Perm<5> tetemb( pen->tetrahedronMapping( facemb[4] ) );
       // vertex idEdg of fac corresponds to tetemb^{-1} facemb[idEdg] of tet.  
       // see if it is in the maximal tree icIx[2] stored as 4*tetindx + num
       unsigned long I ( icIxLookup( tet, tetemb.preImageOf( facemb[idEdg] )) );
       bcompidx = idBdryCompIndexCD1[ I ];
       unsigned long J ( lower_bound( idBdryPi1Gen[bcompidx].begin(), 
        idBdryPi1Gen[bcompidx].end(), I ) - idBdryPi1Gen[bcompidx].begin() );
       if (!inMaximalTree( tet, tetemb.preImageOf( facemb[idEdg] ) ) ) 
        {
         // what's the sign? check to see if this tet embeds into the 
         // pentachoron with same normal orientation, or not.  
         int sign (-1);
         if ( ( tet->embedding(1).pentachoron() == pen ) && 
              ( tet->embedding(1).tetrahedron() == facemb[4] ) )
          sign = 1;
         // what's the generator?  
         unsigned long gennum ( delta0 + I - num_less_than( maxTreeIdB, I ) ); 
         // index of generator. assign to relator  
         relator.addTermFirst( gennum, sign );
         brelator.addTermFirst( J , sign );
        }
      }
     GroupExpression* relate ( new GroupExpression(relator) );
     pres.addRelation(relate);

     GroupExpression* brelate ( new GroupExpression(brelator) );
     idBdryPi1[bcompidx].addRelation(brelate);
    }

   //  now for ideal dual 2-cells, into the interior of the manifold, 
   //  one for every icIx[2]
   for (unsigned long i=0; i<icIx[2].size(); i++)
    {
     GroupExpression relator; 
     const Face<4,3>* tet ( tri4->tetrahedron( icIx[2][i]/4 ) );
     unsigned long idFac ( icIx[2][i] % 4 );  // ideal end number of tet

     // these relators have at most 4 terms depending on how many of the 
     // relevant edges are in the maximal tree. A boundary term, two 
     // connect-to-boundary terms, and an interior tetrahedron term. 
     //  orientations set by tet->embedding()  
     const Simplex<4>* penL( tet->embedding(0).pentachoron() );
     Perm<5> tetLinc( tet->embedding(0).vertices() );

     const Simplex<4>* penR( tet->embedding(1).pentachoron() );
     Perm<5> tetRinc( tet->embedding(1).vertices() );

     // 1-cells in order.
     // 1st boundary connector in maximal tree?
     if (!inMaximalTree( penL, tetLinc[idFac] ) )
      { 
       unsigned long I ( icIxLookup( penL, tetLinc[idFac]) );
       unsigned long indx( delta2 + I - num_less_than(maxTreeSttIdB, I ) );
       relator.addTermFirst( indx, -1 );
      }
     // tet in maximal tree?
     if (!inMaximalTree( tet ) )
      {
       unsigned long I ( nicIxLookup( tet ) );
       unsigned long indx( delta1 + I - num_less_than(maxTreeStd, I ) );
       relator.addTermFirst( indx, 1 );
      }
     // 2nd boundary connector in maximal tree?  
     if (!inMaximalTree( penR, tetRinc[idFac] ) )
      {
       unsigned long I ( icIxLookup( penR, tetRinc[idFac]) );
       unsigned long indx( delta2 + I - num_less_than(maxTreeSttIdB, I ) );
      relator.addTermFirst( indx, 1 );
      }
     // boundary fac in maximal tree? 
     if (!inMaximalTree( tet, idFac ) )
      {
       unsigned long indx( delta0 + i - num_less_than(maxTreeIdB, i) );
       relator.addTermFirst( indx, -1 );
      }

     GroupExpression* relate ( new GroupExpression(relator) );
     pres.addRelation(relate);
    }

  } // end tri4
 else
  { // tri3 
   Face<3,2>* currFac (NULL); Simplex<3>* currTet (NULL); 
   Face<3,2>* fac (NULL); unsigned long currTetFace;

   // find relators. Relators dual to edges. There are two types. 
   //  1) Non-boundary. 
   //  2) Boundary. 
   for (Triangulation<3>::EdgeIterator edg = tri3->edges().begin(); 
        edg!=tri3->edges().end(); edg++)
    {
     GroupExpression relator; 
     if ( !(*edg)->isBoundary() ) // non-boundary -- interior edge
      { 
       std::deque<Face<3,1>Embedding>::const_iterator embit;
       for (embit = (*edg)->embeddings().begin();
                    embit != (*edg)->embeddings().end(); embit++)
        { // now we have to determine whether or not the embedding coincides 
          // with the normal or of the tet.
         currTet = (*embit).tetrahedron();
         currTetFace = (*embit).vertices()[3]; 
         fac = currTet->face(currTetFace);  
         // and is the tet in the maximal tree?    
         if (!inMaximalTree(fac)) 
          {
          // get index. 
          // this index appears to be wrong! 
          unsigned long facind = delta1 + tri3->faceIndex( fac ) - 
                num_less_than( maxTreeStd, tri3->faceIndex( fac ) );
          if ( (fac -> embedding(1).tetrahedron() == currTet) && 
               (fac -> embedding(1).face() == currTetFace) )
           relator.addTermFirst( facind, 1 ); 
          else relator.addTermFirst( facind, -1 ); 
          // oriented from emb 0 to emb 1
          }
        } 
       GroupExpression* relate ( new GroupExpression(relator) );
       pres.addRelation(relate);
      }
     else // boundary edge -- cell half on std boundary, half in interior
      {
       unsigned long facind;
       const Face<3,1>Embedding edgemb = (*edg)->embedding(0);
       currTet = edgemb.tetrahedron();
       currTetFace = edgemb.vertices()[3]; 
       fac = currTet->face(currTetFace); // boundary tet we start with
       unsigned long facedgnum = fac->embedding(0).vertices().preImageOf( 
        edgemb.vertices()[2] );
       if (!fac->isBoundary()) std::cout<<"CellularData::buildFundGrpPres()"
        " ERROR unexpected face (1)."<<std::endl; 

       if (!inMaximalTree(*edg))
        { // the part of the map in the manifold's boundary
         unsigned long I = bcIxLookup(*edg); 
         if ( (normalsDim3BdryEdges[I].secondfac == fac) && 
              (normalsDim3BdryEdges[I].secondedgnum == facedgnum) )
          relator.addTermFirst( I - num_less_than( maxTreeStB, I ),  1 );
         else
          relator.addTermFirst( I - num_less_than( maxTreeStB, I ), -1 );
        }
   
       // interior part of cell boundary
       for (std::deque<Face<3,1>Embedding>::const_iterator 
            embit=(*edg)->embeddings().begin();
            embit != (*edg)->embeddings().end(); embit++)
        { // now we have to determine whether or not the embedding coincides 
          // with the normal or of the tet.
         currTet = (*embit).tetrahedron();
         currTetFace = (*embit).vertices()[3]; 
         fac = currTet->face(currTetFace);  
         // and is the tet in the maximal tree?    
         if (!inMaximalTree(fac)) 
          { // get index. 
          facind = delta1 + tri3->faceIndex( fac ) - 
                   num_less_than( maxTreeStd, tri3->faceIndex( fac ) );
          int sign;
          if (embit == (*edg)->embeddings().begin() ) { sign = -1; }
          else
           {
            if ( (fac -> embedding(0).tetrahedron() == currTet) && 
                 (fac -> embedding(0).face() == currTetFace) )
             sign = -1; else sign = 1; 
           }
           relator.addTermFirst( facind, sign ); 
          }
        } 

       // end pad

       // TODO: fix this embeddings() call, original commented out
       //currTetFace= (*edg)->embeddings().back().vertices()[2];
       currTetFace= (*edg)->back().vertices()[2];

       fac = currTet->triangle(currTetFace);
       if (!fac->isBoundary()) std::cout<<"CellularData::buildFundGrpPres()"
            " ERROR unexpected face (2)."<<std::endl;
       if (!inMaximalTree(fac))
        
         facind = delta1 + fac->index()  -
                  num_less_than( maxTreeStd, fac->index() );
         relator.addTermFirst( facind, 1 ); 
         // all 1-cells dual to boundary tets assumed oriented outwards
        }

       // finish
       GroupExpression* relate( new GroupExpression(relator) );
       pres.addRelation(relate);
      }// end boundary edge loop
    }// that finishes interior cells dual to edges. 

   // now for boundary dual 2-cells -- pure boundary relator.
   //  run through bcIx[0], for each edge call normalsDim3BdryVertices[] 
   Face<3,0>* vrt(NULL);
   for (unsigned long i=0; i<bcIx[0].size(); i++)
    {
     vrt = tri3->vertex( bcIx[0][i] );
     GroupExpression relator; 
     GroupExpression brelator; unsigned long bcompidx(0);
     // call normalsDim3BdryEdges[i] for bcIx[0][i] normal data.
     for (unsigned long j=0; j<normalsDim3BdryVertices[i].face.size(); j++)
      {
       const Face<3,2>* fac( normalsDim3BdryVertices[i].face[j] );
        // unsigned long edgnum ( normalsDim4BdryEdges[i].edgenum[j] );
       Perm<3> vrtinc ( normalsDim3BdryVertices[i].vrtinc[j] );
       // find the face that edginc[2] [3] comes out of
       const Face<3,1>* bedg (fac -> edge( vrtinc[2] ) ); 
       if (!inMaximalTree(bedg))
        {
        unsigned long bedgidx (bcIxLookup(bedg));// bedg's bcIx[1] index
        // what boundary component are we in? 
        bcompidx = stdBdryCompIndexCD1[ bedgidx ] ;
        unsigned long bgen = lower_bound( stdBdryPi1Gen[bcompidx].begin(), 
         stdBdryPi1Gen[bcompidx].end(), bedgidx ) - 
         stdBdryPi1Gen[bcompidx].begin();

        if ( ( normalsDim3BdryEdges[bedgidx].secondfac == fac ) && 
             ( normalsDim3BdryEdges[bedgidx].secondedgnum == vrtinc[2] ) )
         {
          relator.addTermFirst( bedgidx - 
            num_less_than( maxTreeStB, bedgidx ), 1 );  // + or 
          brelator.addTermFirst( bgen, 1 );	   
         }
        else
         {
          relator.addTermFirst( bedgidx - 
            num_less_than( maxTreeStB, bedgidx ), -1);  // - or
          brelator.addTermFirst( bgen, -1 );	   
         }
        }
      }
      // finish
      GroupExpression* relate ( new GroupExpression(relator) );
      pres.addRelation(relate);   

      GroupExpression* brelate ( new GroupExpression(brelator) );
      stdBdryPi1[bcompidx].addRelation(brelate);
    } // end boundary dual 2-cells

   //  now for ideal dual 2-cells, dual to ideal 0-cells in ideal boundary, 
   // one for every icIx[0]
   for (unsigned long i=0; i<icIx[0].size(); i++)
    {
     GroupExpression relator, brelator; 
     unsigned long bcompidx(0);
     const Face<3,1>* edg ( tri3->edge( icIx[0][i]/2 ) );
     unsigned long idEdg ( icIx[0][i] % 2 );  // ideal edge number of fac
     // lets acquire all the Dim4Pentachora incident to fac. 
     for (unsigned long j=0; j<edg->degree(); j++)
      {
       const Simplex<3>* tet ( edg->embedding(j).tetrahedron() );
       Perm<4> edgemb( edg->embedding(j).vertices() );
       // idEdg of fac represents an ideal edge.  We want to find all the 
       // ideal incident tets and mark them appropriately so we find fac's 
       // embeddings in pentachoral and look up the relevant ideal tets.  
       // so we are going across the tetrahedron in pen whose vertices are 
       // marked by facemb[0][1][2] and [3] tetrahedron  labelled by facemb[4]. 
       const Face<3,2>* fac ( tet->triangle( edgemb[3] ) );
       Perm<4> facemb( tet->triangleMapping( edgemb[3] ) );
       // vertex idEdg of fac corresponds to tetemb^{-1} facemb[idEdg] of tet. 
       //  So let's see if it is in the 
       // maximal tree icIx[2] stored as 4*tetindx + num
       unsigned long I ( icIxLookup( fac, 
                         facemb.preImageOf( edgemb[idEdg] ) ) );
       bcompidx = idBdryCompIndexCD1[ I ];
       unsigned long J ( lower_bound( idBdryPi1Gen[bcompidx].begin(),
                         idBdryPi1Gen[bcompidx].end(), I ) -
 				         idBdryPi1Gen[bcompidx].begin() );
       if (!inMaximalTree( fac, facemb.preImageOf( edgemb[idEdg] ) ) ) 
        {
         // what's the sign? check to see if this tet embeds into the 
         //  pentachoron with same normal orientation, or not.  
         int sign (-1);
         if ( ( fac->embedding(1).tetrahedron() == tet ) && 
              ( fac->embedding(1).face() == facemb[3] ) ) sign = 1;
         // what's the generator?  
         unsigned long gennum ( delta0 + I - num_less_than( maxTreeIdB, I ) ); 
         // index of generator assign to relator  
         relator.addTermFirst( gennum, sign );
         brelator.addTermFirst( J , sign );
        }
      }
     GroupExpression* relate ( new GroupExpression(relator) );
     pres.addRelation(relate);

     GroupExpression* brelate ( new GroupExpression(brelator) );
     idBdryPi1[bcompidx].addRelation(brelate);
    }

   //  now for ideal dual 2-cells, into the interior of the manifold, 
   //  one for every icIx[1]
   for (unsigned long i=0; i<icIx[1].size(); i++)
    {
     GroupExpression relator; 
     const Face<3,2>* fac ( tri3->triangle( icIx[1][i]/3 ) );
     unsigned long idEdg ( icIx[1][i] % 3 );  // ideal end number of tet

     // these relators have at most 4 terms depending on how many of the 
     // relevant edges are in the maximal tree. A boundary term, two 
     // connect-to-boundary terms, and an interior tetrahedron term. 
     //  orientations set by tet->embedding()  
     const Simplex<3>* tetL( fac->embedding(0).tetrahedron() );
     Perm<4> facLinc( fac->embedding(0).vertices() );

     const Simplex<3>* tetR( fac->embedding(1).tetrahedron() );
     Perm<4> facRinc( fac->embedding(1).vertices() );

     // 1-cells in order.
     // 1st boundary connector in maximal tree?
     if (!inMaximalTree( tetL, facLinc[idEdg] ) )
      { 
       unsigned long I ( icIxLookup( tetL, facLinc[idEdg] ) );
       unsigned long indx( delta2 + I - num_less_than(maxTreeSttIdB, I ) );
       relator.addTermFirst( indx, -1 );
      }
     // tet in maximal tree?
     if (!inMaximalTree( fac ) )
      {
       unsigned long I ( nicIxLookup( fac ) );
       unsigned long indx( delta1 + I - num_less_than(maxTreeStd, I ) );
       relator.addTermFirst( indx, 1 );
      }
     // 2nd boundary connector in maximal tree?  
     if (!inMaximalTree( tetR, facRinc[idEdg] ) )
      {
       unsigned long I ( icIxLookup( tetR, facRinc[idEdg] ) );
       unsigned long indx( delta2 + I - num_less_than(maxTreeSttIdB, I ) );
       relator.addTermFirst( indx, 1 );
      }
     // boundary fac in maximal tree? 
     if (!inMaximalTree( fac, idEdg ) )
      {
       unsigned long indx( delta0 + i - num_less_than(maxTreeIdB, i) );
       relator.addTermFirst( indx, -1 );
      }

     GroupExpression* relate ( new GroupExpression(relator) );
     pres.addRelation(relate);
    } // end the 2-cells that connect ideal boundary to interior.
  } // end tri3

  // Generate the pi1 presentation for the whole manifold
  GroupPresLocator g_desc(whole_manifold, 0);
  GroupPresentation* GPptr( new GroupPresentation( pres ) );
  std::map< GroupPresLocator, GroupPresentation* > *mGPptr = 
    const_cast< std::map< GroupPresLocator, GroupPresentation* > *> 
    (&groupPresentations);
  mGPptr->insert( std::pair<GroupPresLocator, GroupPresentation*>
    (g_desc, GPptr) );

  // Generate the pi1 presentation for the standard boundary components  
  for (unsigned long i=0; i<numStdBdryComps; i++) // push stdBdryPi1 onto stack
   {
    g_desc.sub_man = standard_boundary; 
    g_desc.component_index = i; 
    GPptr = new GroupPresentation( stdBdryPi1[i] );
    std::map< GroupPresLocator, GroupPresentation* > *XX =  
     const_cast< std::map< GroupPresLocator, GroupPresentation* > *> 
     (&groupPresentations);
    XX->insert( std::pair<GroupPresLocator, 
                GroupPresentation*>(g_desc, GPptr) );
   }

  // Generate the pi1 presentation for the ideal/cusp boundary components
  for (unsigned long i=0; i<numIdealBdryComps; i++) // push idBdryPi1 onto stack
   {
    g_desc.sub_man = ideal_boundary; 
    g_desc.component_index = i; 
    GPptr = new GroupPresentation( idBdryPi1[i] );
    std::map< GroupPresLocator, GroupPresentation* > *XX =  
     const_cast< std::map< GroupPresLocator, GroupPresentation* > *> 
      (&groupPresentations);
    XX->insert( std::pair<GroupPresLocator, GroupPresentation*>
      (g_desc, GPptr) );
   }

 // Note: The material below is to construct maps of pi1 presentations.

 // vector that gives the generator *number* in the ambient manifold's pi1 
 // presentation from generator of boundary component. 
 std::vector< std::vector< unsigned long > > stdBdryGenIncl( numStdBdryComps );
 std::vector< std::vector< unsigned long > > 
    idlBdryGenIncl( numIdealBdryComps );
 for (unsigned long i=0; i<stdBdryGenIncl.size(); i++)
  {
   stdBdryGenIncl[i].resize( stdBdryPi1Gen[i].size() );
   for (unsigned long j=0; j<stdBdryPi1Gen[i].size(); j++)
     stdBdryGenIncl[i][j] = stdBdryPi1Gen[i][j] - 
       num_less_than( maxTreeStB, stdBdryPi1Gen[i][j] ) ;
  }
 for (unsigned long i=0; i<idlBdryGenIncl.size(); i++)
  {
   idlBdryGenIncl[i].resize( idBdryPi1Gen[i].size() );
   for (unsigned long j=0; j<idBdryPi1Gen[i].size(); j++)
     idlBdryGenIncl[i][j] =  numNonIdealBdryCells[tri4 ? 2 : 1] - 
      maxTreeStB.size()  + idBdryPi1Gen[i][j] - 
      num_less_than( maxTreeIdB, idBdryPi1Gen[i][j] );
  }
 
 // Generate the homomorphisms of group presentations
 // First for standard boundary to whole manifold
 HomGroupPresentation* presPtr;
 for (unsigned long i=0; i<stdBdryGenIncl.size(); i++)
  {
   std::vector< GroupExpression > ithInclMap( stdBdryGenIncl[i].size() ); 
        // one for every generator of pi1 i-th boundary component
   for (unsigned long j=0; j<ithInclMap.size(); j++)
     ithInclMap[j].addTermFirst( GroupExpressionTerm( 
        stdBdryGenIncl[i][j], 1 ) );
   presPtr = new HomGroupPresentation( stdBdryPi1[i], pres, ithInclMap );
   HomGroupPresLocator h_desc( standard_boundary, i );
   std::map< HomGroupPresLocator, HomGroupPresentation* > *XX = 
    const_cast< std::map< HomGroupPresLocator, HomGroupPresentation* > *> 
        (&homGroupPresentations);
   XX -> insert( std::pair< HomGroupPresLocator, HomGroupPresentation*>
        (h_desc, presPtr) );
  }

 // And for ideal boundary to whole manifold
 for (unsigned long i=0; i<idlBdryGenIncl.size(); i++)
  {
   std::vector< GroupExpression > ithInclMap( idlBdryGenIncl[i].size() ); 
        // one for every generator of pi1 i-th boundary component
   for (unsigned long j=0; j<ithInclMap.size(); j++)
      ithInclMap[j].addTermFirst( GroupExpressionTerm( 
        idlBdryGenIncl[i][j], 1 ) );
   presPtr = new HomGroupPresentation( idBdryPi1[i], pres, ithInclMap );
   HomGroupPresLocator h_desc( ideal_boundary, i );
   std::map< HomGroupPresLocator, HomGroupPresentation* > *XX = 
    const_cast< std::map< HomGroupPresLocator, HomGroupPresentation* > *> 
        (&homGroupPresentations);
   XX -> insert( std::pair< HomGroupPresLocator, HomGroupPresentation*>
        (h_desc, presPtr) );
  }
 } // end pi1 code

} // namespace regina


