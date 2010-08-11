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


// counts number of elements in thelist less than obj
unsigned long num_less_than(const std::set<unsigned long> &thelist, const unsigned long &obj)
{
 unsigned long retval = 0;
 std::set<unsigned long>::const_iterator i;
 for (i=thelist.begin(); i!=thelist.end(); i++) {  if ( (*i) < obj ) retval++; else return retval; }
 return retval; 
}

// dim4
bool NCellularData::inMaximalTree(const Dim4Tetrahedron* tet) const
{
if (!binary_search( nicIx[3].begin(), nicIx[3].end(), tri4->tetrahedronIndex( tet ) ) ) return false; // not in list of tetrahedra!
unsigned long I = lower_bound( nicIx[3].begin(), nicIx[3].end(), tri4->tetrahedronIndex( tet ) ) - nicIx[3].begin();
if (!binary_search( maxTreeStd.begin(), maxTreeStd.end(), I ) ) return false;
return true;
}

bool NCellularData::inMaximalTree(const Dim4Face* fac) const
{
if (!binary_search(bcIx[2].begin(), bcIx[2].end(), tri4->faceIndex( fac ) ) ) return false; // not in list of faces!
unsigned long I = lower_bound( bcIx[2].begin(), bcIx[2].end(), tri4->faceIndex( fac ) ) - bcIx[2].begin();
if (!binary_search( maxTreeStB.begin(), maxTreeStB.end(), I ) ) return false;
return true; 
}

bool NCellularData::inMaximalTree(const Dim4Tetrahedron* tet, unsigned long num) const
{
if (!binary_search(icIx[2].begin(), icIx[2].end(), 4*tri4->tetrahedronIndex( tet ) + num ) ) return false; // not in list of ideal ends of tetrahedra
unsigned long I = lower_bound( icIx[2].begin(), icIx[2].end(), 4*tri4->tetrahedronIndex( tet ) + num ) - icIx[2].begin();
if (!binary_search( maxTreeIdB.begin(), maxTreeIdB.end(), I ) ) return false; 
return true;
}

bool NCellularData::inMaximalTree(const Dim4Pentachoron* pen, unsigned long num) const
{
if (!binary_search(icIx[3].begin(), icIx[3].end(), 5*tri4->pentachoronIndex( pen ) + num ) ) return false; 
unsigned long I = lower_bound( icIx[3].begin(), icIx[3].end(), 5*tri4->pentachoronIndex( pen ) + num ) - icIx[3].begin();
if (!binary_search( maxTreeSttIdB.begin(), maxTreeSttIdB.end(), I ) ) return false;
return true;
}


// dim3 
bool NCellularData::inMaximalTree(const NFace* fac) const
{
if (!binary_search( nicIx[2].begin(), nicIx[2].end(), tri3->faceIndex( fac ) ) ) return false; // not in list of faces!
unsigned long I = lower_bound( nicIx[2].begin(), nicIx[2].end(), tri3->faceIndex( fac ) ) - nicIx[2].begin();
if (!binary_search( maxTreeStd.begin(), maxTreeStd.end(), I ) ) return false;
return true;
}

bool NCellularData::inMaximalTree(const NEdge* edg) const
{
if (!binary_search(bcIx[1].begin(), bcIx[1].end(), tri3->edgeIndex( edg ) ) ) return false; // not in list of edges!
unsigned long I = lower_bound( bcIx[1].begin(), bcIx[1].end(), tri3->edgeIndex( edg ) ) - bcIx[1].begin();
if (!binary_search( maxTreeStB.begin(), maxTreeStB.end(), I ) ) return false;
return true; 
}

bool NCellularData::inMaximalTree(const NFace* fac, unsigned long num) const
{
if (!binary_search(icIx[1].begin(), icIx[1].end(), 3*tri3->faceIndex( fac ) + num ) ) return false; // not in list of ideal ends of faces
unsigned long I = lower_bound( icIx[1].begin(), icIx[1].end(), 3*tri3->faceIndex( fac ) + num ) - icIx[1].begin();
if (!binary_search( maxTreeIdB.begin(), maxTreeIdB.end(), I ) ) return false; 
return true;
}

bool NCellularData::inMaximalTree(const NTetrahedron* tet, unsigned long num) const
{
if (!binary_search(icIx[2].begin(), icIx[2].end(), 4*tri3->tetrahedronIndex( tet ) + num ) ) return false; 
unsigned long I = lower_bound( icIx[2].begin(), icIx[2].end(), 4*tri3->tetrahedronIndex( tet ) + num ) - icIx[2].begin();
if (!binary_search( maxTreeSttIdB.begin(), maxTreeSttIdB.end(), I ) ) return false;
return true;
}

   /**
     * Normal orientations for cells Regina does not naturally give normal orientations to.  This routine
     *  also sets up an indexing so that one can determine from an (ideal) skeletal object which boundary
     *  component it lies in. 
     *
     * normalsDim4BdryFaces is a vector that assigns to the i-th boundary face [tri4->getFace(bcIx[2][i])]
     *  the two boundary tetrahedra that contain it and the face number of the face in the tetrahedron. 
     *
     * normalsDim4BdryEdges is a vector that assigns to the i-th boundary edge [tri4->getFace(bcIx[1][i])]
     *  the circle of tetrahedra incident to that edge, with edginc[2] and edginc[3] forming the normal orientation
     *  in agreement with the indexing of tet. 
     *
     * normalsDim4BdryVertices is a vector that assigns to the i-th boundary vertex [tri4->getVertex(bcIx[0][i])]
     *  the sphere of tetrahedra incident to that vertex, with vrtinc[1], vrtinc[2], vrtinc[3] forming a normal
     *  orientation.
     *
     * normalsDim3BdryEdges is a vector that assigns to the i-th boundary face [tri3->getEdge(bcIx[1][i])]
     *  the two boundary faces that contain it and the edge number of the edge in the NFace.  
     *
     * normalsDim3BdryVertices is a vector that assigns to the i-th boundary vertex [tri3->getVertex(bcIx[0][i])]
     *  the circle of faces incident to that vertex, with vrtinc[1] and vrtinc[2] forming the normal orientation
     *  in agreement with the indexing of face. TODO
     *
     * Warning, not yet NTriangulation safe.  TODO
     * The data this creates is perhaps not copy-constructor safe, either.  
     */
void NCellularData::buildExtraNormalData()
{
 if (tri4!=NULL)
 { 
  normalsDim4BdryFaces.resize( bcIx[2].size() );
  normalsDim4BdryEdges.resize( bcIx[1].size() );
  normalsDim4BdryVertices.resize( bcIx[0].size() );

  // iterate through boundary components
  const std::vector<Dim4BoundaryComponent*> bComps(tri4->getBoundaryComponents());
  for (Dim4Triangulation::BoundaryComponentIterator bcit=tri4->getBoundaryComponents().begin(); 
       bcit!=tri4->getBoundaryComponents().end(); bcit++) if ( !(*bcit)->isIdeal() )
   {
    const NTriangulation* bTriComp( (*bcit)->getTriangulation() );
     
    // run through vertices, edges, faces in bTriComp, find corresponding Dim4 object, get its bcIx, call it j
    //  for the same objects get incidence data, compute corresponding bcIx indices, this allows us to fill out
    //  normalsDim4Bdry***[j]

    // iterate through bTriComp vertices
    for (NTriangulation::VertexIterator vit=bTriComp->getVertices().begin(); 
         vit!=bTriComp->getVertices().end(); vit++)
     {
      unsigned long I = lower_bound( bcIx[0].begin(), bcIx[0].end(), 
       tri4->vertexIndex( (*bcit)->getVertex( bTriComp->vertexIndex(*vit) ) ) ) - bcIx[0].begin(); // bcIx[0] index

      // find vit's embeddings
      const std::vector<NVertexEmbedding> tri3vrtEmb( (*vit)->getEmbeddings() ); // vertex embeddings in bdry triangulation
 
      dim4BoundaryVertexInclusion dim4bvrtInc;
      dim4bvrtInc.tet.resize(tri3vrtEmb.size()); 
      dim4bvrtInc.vrtnum.resize(tri3vrtEmb.size());
      dim4bvrtInc.vrtinc.resize(tri3vrtEmb.size());
      // find corresponding tri4 data
      for (unsigned long i=0; i<tri3vrtEmb.size(); i++)
       {
         dim4bvrtInc.tet[i] = (*bcit)->getTetrahedron( bTriComp->tetrahedronIndex( tri3vrtEmb[i].getTetrahedron() ) );
         dim4bvrtInc.vrtnum[i] = tri3vrtEmb[i].getVertex();
         dim4bvrtInc.vrtinc[i] = tri3vrtEmb[i].getVertices();
       }
      normalsDim4BdryVertices[I] = dim4bvrtInc; 
     }    

    // iterate through bTriComp edges
    for (NTriangulation::EdgeIterator eit=bTriComp->getEdges().begin(); 
         eit!=bTriComp->getEdges().end(); eit++)
     {
      unsigned long I = lower_bound( bcIx[1].begin(), bcIx[1].end(), 
       tri4->edgeIndex( (*bcit)->getEdge( bTriComp->edgeIndex(*eit) ) ) ) - bcIx[1].begin(); // bcIx[1] index
  
      // find vit's embeddings
      const std::deque<NEdgeEmbedding> tri3edgEmb( (*eit)->getEmbeddings() ); // vertex embeddings in bdry triangulation
 
      dim4BoundaryEdgeInclusion dim4bedgInc;
      dim4bedgInc.tet.resize(tri3edgEmb.size()); 
      dim4bedgInc.edgenum.resize(tri3edgEmb.size());
      dim4bedgInc.edginc.resize(tri3edgEmb.size());
      // find corresponding tri4 data
      for (unsigned long i=0; i<tri3edgEmb.size(); i++)
       {
         dim4bedgInc.tet[i] = (*bcit)->getTetrahedron( bTriComp->tetrahedronIndex( tri3edgEmb[i].getTetrahedron() ) );
         dim4bedgInc.edgenum[i] = tri3edgEmb[i].getEdge();
         dim4bedgInc.edginc[i] = tri3edgEmb[i].getVertices();
       }
      normalsDim4BdryEdges[I] = dim4bedgInc; // is this array resized properly? need to ensure that...
     }    

    // iterate through bTriComp faces
    for (NTriangulation::FaceIterator fit=bTriComp->getFaces().begin(); 
         fit!=bTriComp->getFaces().end(); fit++)
     {
      unsigned long I = lower_bound( bcIx[2].begin(), bcIx[2].end(), 
       tri4->faceIndex( (*bcit)->getFace( bTriComp->faceIndex(*fit) ) ) ) - bcIx[2].begin(); // bcIx[2] index
   
      dim4BoundaryFaceInclusion dim4bfacInc;
      dim4bfacInc.firsttet = (*bcit)->getTetrahedron( bTriComp->tetrahedronIndex( (*fit)->getEmbedding(0).getTetrahedron() ) );
      dim4bfacInc.secondtet = (*bcit)->getTetrahedron( bTriComp->tetrahedronIndex( (*fit)->getEmbedding(1).getTetrahedron() ) );
      dim4bfacInc.firstfacnum = (*fit)->getEmbedding(0).getFace();
      dim4bfacInc.secondfacnum = (*fit)->getEmbedding(1).getFace();

      normalsDim4BdryFaces[I] = dim4bfacInc; // is this array resized properly? need to ensure that...
     }    
   }
 } // end tri4 
 else
 { // tri3 construct normalsDim3BdryEdges and normalsDim3BdryVertices TODO
  normalsDim3BdryEdges.resize( bcIx[1].size() );
  // okay, so for every bcIx[1] we find the corresponding boundary edge, and the boundary NFaces that include it
  // struct dim3BoundaryEdgeInclusion
  //  { NFace *firstfac, *secondfac;
  //    unsigned long firstedgnum, secondedgnum; };
  // so we request bcIx[1]'s embeddings, find the first and last face in that list
  for (unsigned long i=0; i<bcIx[1].size(); i++)
   {
    const NEdge* edg( tri3->getEdge( bcIx[1][i] ) );    const NEdgeEmbedding emb1( edg->getEmbedding(0) );
    const NTetrahedron* tet1( emb1.getTetrahedron() );  NFace* fac1 ( tet1->getFace( emb1.getVertices()[3] ) );
    normalsDim3BdryEdges[i].firstfac = fac1;
    normalsDim3BdryEdges[i].firstedgnum = fac1->getEmbedding(0).getVertices().preImageOf( emb1.getVertices()[2] );

    const NEdgeEmbedding emb2( edg->getEmbedding( edg->getNumberOfEmbeddings() - 1 ) );
    const NTetrahedron* tet2( emb1.getTetrahedron() );  NFace* fac2 ( tet2->getFace( emb2.getVertices()[2] ) );
    normalsDim3BdryEdges[i].secondfac = fac2;
    normalsDim3BdryEdges[i].secondedgnum = fac2->getEmbedding(0).getVertices().preImageOf( emb2.getVertices()[3] );
   }
  // now for the boundary vertices.
  normalsDim4BdryVertices.resize( bcIx[0].size() );
  // similarly, for every bcIx[0] find the boundary NFace's that include it and their data, as described in the structure below.
  // struct dim3BoundaryVertexInclusion
  //  { std::vector< NFace* > face;
  //    std::vector< unsigned long > vrtnum; 
  //    std::vector< NPerm3 > vrtinc; };
  for (unsigned long i=0; i<bcIx[0].size(); i++)
   {
    const NVertex* vrt( tri3->getVertex(bcIx[0][i]) );
    const std::vector< NVertexEmbedding > vrtEmbs(vrt->getEmbeddings());
    // TODO 
   }

 } // end tri3

 // figure out number of standard vs. ideal boundary components, also compute a vector which describes
 //  the map (boundary faces) --> (boundary components they belong to)
 numStdBdryComps=0; numIdealBdryComps=0; 
  if (tri4!=NULL)
  {
  stdBdryCompIndexCD1.resize( bcIx[2].size() ); 
  idBdryCompIndexCD1.resize( icIx[2].size() );
  for (unsigned long i=0; i<tri4->getNumberOfBoundaryComponents(); i++) 
   { // now we can run through all the faces in this boundary component, and fill out the appropriate array stdBdryCompIndex
    const Dim4BoundaryComponent* bcomp( tri4->getBoundaryComponent(i) );
    if (!bcomp->isIdeal())
     {
      for (unsigned long j=0; j<bcomp->getNumberOfFaces(); j++)
       {
        unsigned long I=lower_bound( bcIx[2].begin(), bcIx[2].end(), tri4->faceIndex( bcomp->getFace(j) ) ) - bcIx[2].begin();
        stdBdryCompIndexCD1[I] = numStdBdryComps; 
       }
       numStdBdryComps++; 
      }
    else // bcomp *is* ideal
     {
      // run through icIx[2] check to see if the corresponding ideal vertex is in bcomp
      for (unsigned long j=0; j<icIx[2].size(); j++)
       {
        const Dim4Tetrahedron* tet( tri4->getTetrahedron( icIx[2][j] / 4 ) ); 
        const Dim4Vertex* vrt( tet->getVertex(icIx[2][j] % 4 ) );
        if (vrt->isIdeal()) if (tri4->boundaryComponentIndex( vrt->getBoundaryComponent() )==i)
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
  for (unsigned long i=0; i<tri3->getNumberOfBoundaryComponents(); i++) 
   { // now we can run through all the faces in this boundary component, and fill out the appropriate array stdBdryCompIndex
    const NBoundaryComponent* bcomp( tri3->getBoundaryComponent(i) );
    if (!bcomp->isIdeal())
     {
      for (unsigned long j=0; j<bcomp->getNumberOfEdges(); j++)
       {
        unsigned long I=lower_bound( bcIx[1].begin(), bcIx[1].end(), tri3->edgeIndex( bcomp->getEdge(j) ) ) - bcIx[1].begin();
        stdBdryCompIndexCD1[I] = numStdBdryComps; 
       }
       numStdBdryComps++; 
      }
    else // bcomp *is* ideal
     {
      // run through icIx[1] check to see if the corresponding ideal vertex is in bcomp
      for (unsigned long j=0; j<icIx[1].size(); j++)
       {
        const NFace* fac( tri3->getFace( icIx[1][j] / 3 ) ); 
        const NVertex* vrt( fac->getVertex(icIx[1][j] % 3 ) );
        if (vrt->isIdeal()) if (tri3->boundaryComponentIndex( vrt->getBoundaryComponent() )==i)
         idBdryCompIndexCD1[j] = numIdealBdryComps;
       }
      numIdealBdryComps++;
     }
   }
  }
} // end buildExtraNormalData()

/**
 * This routine runs through the dual 1-skeleton to the triangulation, building a maximal tree in the dual 1-skeleton
 *  to the triangulation which restricts to a maximal tree in any boundary component, ideal or "standard" boundary.
 *
 * This algorithm also sets up the indexing of cells on the boundary components. At present these are the internal 
 *  vectors stdBdryCompIndexCD1 and idBdryCompIndexCD1
 *
 * Assumes triangulation is connected.
 */
void NCellularData::buildMaximalTree()
{
 if ( maxTreeStd.size() != 0 ) return; // don't bother calling the routine twice
 // Iterate: walk through dual 1-skeleton until we hit an (ideal) boundary component we haven't hit before.  During the iteration we
 //  keep track of several things. Priority to building (ideal) boundary max tree ahead of interior max tree.

 std::set< unsigned long > visitedZ, visitedBd, visitedId; // 0-cells in maximal tree
 // indexed by             nicIx[n], bcIx[n-1], icIx[n-1] respectively.
 std::set< unsigned long > newVertexListS, newVertexListB, newVertexListI; // 0-cell indices yet unexplored...
 // indexed by             nicIx[n],       bcIx[n-1],      icIx[n-1]

 if (tri4 != NULL)
 {  // let's seed the process and start in pentachoron 0.
 visitedZ.insert(0); newVertexListS.insert(0);

 while ( (newVertexListS.size() > 0) || (newVertexListB.size() > 0) || (newVertexListI.size() > 0) ) // unexplored vertices...
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
    newVertexListI.erase(unexploredV); // increment unexploredV but return the unincremented iterator for erase arg...
   }

  standard_boundary_loop4:
  
  // then check to see if there are standard boundary vertices unexplored
  while (newVertexListB.size() > 0)
   {
    unexploredV = newVertexListB.begin();

    // *unexploredV is the bcIx[3]-index of the standard boundary dual 0-cell
    const Dim4Tetrahedron* btet(NULL); 
    btet = tri4 -> getTetrahedron( bcIx[3][*unexploredV] ); // btet

    for (unsigned long i=0; i<4; i++) // cross all faces
      {
       const Dim4Face* fac( btet->getFace(i) ); // look this up in normalsDim4BdryFaces, for that we need the bcIx[2] index.
       unsigned long facidx( lower_bound( bcIx[2].begin(), bcIx[2].end(), tri4->faceIndex( fac ) ) - bcIx[2].begin() ); 
       // one of normalsDim4BdryFaces[facidx] first/second represents this tetrahedron and face, lets walk to the other. 
       if ( (normalsDim4BdryFaces[facidx].firsttet == btet) && (normalsDim4BdryFaces[facidx].firstfacnum == i) ) 
        { // ensure second tet hasn't been touched, if so record the fac.  
          unsigned long newbtetidx ( lower_bound( bcIx[3].begin(), bcIx[3].end(), 
                tri4->tetrahedronIndex( normalsDim4BdryFaces[facidx].secondtet ) ) - bcIx[3].begin() );
          if (!binary_search( visitedBd.begin(), visitedBd.end(), newbtetidx ) )
          {
            visitedBd.insert(newbtetidx); newVertexListB.insert(newbtetidx); maxTreeStB.insert( facidx );             
          }
        }
       else // this btet is the secondtet.
        {
          unsigned long newbtetidx ( lower_bound( bcIx[3].begin(), bcIx[3].end(), 
                tri4->tetrahedronIndex( normalsDim4BdryFaces[facidx].firsttet ) ) - bcIx[3].begin() );
          if (!binary_search( visitedBd.begin(), visitedBd.end(), newbtetidx ) )
          {
            visitedBd.insert(newbtetidx); newVertexListB.insert(newbtetidx); maxTreeStB.insert( facidx );             
          }
        }
      }
     newVertexListB.erase(unexploredV); // increment unexploredV but return the unincremented iterator...
   }


  // then check to see if there are standard vertices unexplored
  while (newVertexListS.size() > 0)
    {
    unexploredV = newVertexListS.begin();

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
       { 
         visitedId.insert(I); newVertexListI.insert(I); maxTreeSttIdB.insert(I);  
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

    newVertexListS.erase(unexploredV); // increment unexploredV but return the unincremented iterator for erase arg...
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
  std::set< unsigned long >::iterator unexploredV;
  // first check to see if there are ideal vertices unexplored
  while (newVertexListI.size() > 0)
   {
    unexploredV = newVertexListI.begin();

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
  // then check to see if there are standard boundary vertices unexplored
  while (newVertexListB.size() > 0)
   {
    unexploredV = newVertexListB.begin();

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

  // then check to see if there are standard vertices unexplored
  while (newVertexListS.size() > 0)
    {
    unexploredV = newVertexListS.begin();

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

 // other data required for presentations of fundamental groups of the boundary components
 stdBdryPi1Gen.resize( numStdBdryComps );
 idBdryPi1Gen.resize( numIdealBdryComps );
 // run through all dual boundary 1-cells, if not in max tree append to stdBdryPi1Gen, etc. 
 if (tri4 != NULL) 
  { for (unsigned long i=0; i<bcIx[2].size(); i++) if (!inMaximalTree( tri4->getFace(bcIx[2][i]) ) )
       stdBdryPi1Gen[ stdBdryCompIndexCD1[i] ].push_back(i); 
    for (unsigned long i=0; i<icIx[2].size(); i++) if (!inMaximalTree( tri4->getTetrahedron(icIx[2][i]/4), icIx[2][i]%4) )
       idBdryPi1Gen[ idBdryCompIndexCD1[i] ].push_back(i);
  }
 else // tri3 != NULL
  { for (unsigned long i=0; i<bcIx[1].size(); i++) if (!inMaximalTree( tri3->getEdge(bcIx[1][i]) ) )
       stdBdryPi1Gen[ stdBdryCompIndexCD1[i] ].push_back(i); 
    for (unsigned long i=0; i<icIx[1].size(); i++) if (!inMaximalTree( tri3->getFace(icIx[1][i]/3), icIx[1][i]%3) )
       idBdryPi1Gen[ idBdryCompIndexCD1[i] ].push_back(i);
  }
}// end buildMaximalTree()

void NCellularData::buildFundGrpPres() const
{
 NGroupPresentation pres;
 std::vector< NGroupPresentation > stdBdryPi1( numStdBdryComps );
 std::vector< NGroupPresentation > idBdryPi1( numIdealBdryComps );

 for (unsigned long i=0; i<stdBdryPi1.size(); i++)
  stdBdryPi1[i].addGenerator( stdBdryPi1Gen[i].size() );
 for (unsigned long i=0; i<idBdryPi1.size(); i++)
  idBdryPi1[i].addGenerator( idBdryPi1Gen[i].size() ); 

 // for now the group presentation will consist of: a list of numbers for the generators, 
 //  a list of words in the generators corresponding to the relators. 
 // the generators consist of edges of the dual 1-skeleton not in the maximal tree

 if (tri4)
  {
   // we sort the 1-cells by: standard boundary, ideal boundary, standard interior cells, edges to ideal boundary
   // so wedge indexed by:     - maxTreeStB.size,  - maxTreeIdB.size(), - maxTreeStd.size(),  maxTreeSttIdB.size()
   unsigned long delta0( numNonIdealBdryCells[2] - maxTreeStB.size() ); 
   unsigned long delta1( delta0 + numIdealCells[2] - maxTreeIdB.size() );
   unsigned long delta2( delta1 + numNonIdealCells[3] - maxTreeStd.size() );
   unsigned long delta3( delta2 + numIdealCells[3] - maxTreeSttIdB.size() ); // to know what is out of range
   pres.addGenerator( delta3 );// we've set the generators for the presentation.

   Dim4Tetrahedron* currTet (NULL); Dim4Pentachoron* currPen (NULL); Dim4Tetrahedron* tet (NULL); unsigned long currPenFace;

   // okay, lets start finding relators. Relators dual faces. There are two types. 
   //  1) Non-boundary. 
   //  2) Boundary. 

   for (Dim4Triangulation::FaceIterator fac = tri4->getFaces().begin(); fac!=tri4->getFaces().end(); fac++)
    {
     NGroupExpression relator; 

     if ( !(*fac)->isBoundary() ) // non-boundary -- interior 2-cell
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
          if ( (tet -> getEmbedding(1).getPentachoron() == currPen) && (tet -> getEmbedding(1).getTetrahedron() == currPenFace) )
           relator.addTermFirst( tetind, 1 ); else relator.addTermFirst( tetind, -1 ); // oriented from emb 0 to emb 1
          }
        } 
       NGroupExpression* relate ( new NGroupExpression(relator) );
       pres.addRelation(relate);
      }
     else // boundary face -- cell half on std boundary, half in interior
      {
       unsigned long tetind;
       const Dim4FaceEmbedding facemb = (*fac)->getEmbedding(0);
       currPen = facemb.getPentachoron();
       currPenFace = facemb.getVertices()[4]; 
       tet = currPen->getTetrahedron(currPenFace); // boundary tet we start with
       unsigned long tetfacnum = tet->getEmbedding(0).getVertices().preImageOf( facemb.getVertices()[3] );
       if (!tet->isBoundary()) std::cout<<"ERROR (unexpected tetrahedron) "<<std::endl; 

       if (!inMaximalTree(*fac))
        {
         // orientation? 
         unsigned long I = lower_bound( bcIx[2].begin(), bcIx[2].end(), tri4->faceIndex( *fac ) ) - bcIx[2].begin();
         if ( (normalsDim4BdryFaces[I].secondtet == tet) && (normalsDim4BdryFaces[I].secondfacnum == tetfacnum) )
          relator.addTermFirst( I - num_less_than( maxTreeStB, I ),  1 );
         else
          relator.addTermFirst( I - num_less_than( maxTreeStB, I ), -1 );
        }
   
       // main loop
       for (std::deque<Dim4FaceEmbedding>::const_iterator embit=(*fac)->getEmbeddings().begin();
            embit != (*fac)->getEmbeddings().end(); embit++)
        { // now we have to determine whether or not the embedding coincides with the normal or of the tet.
         currPen = (*embit).getPentachoron();
         currPenFace = (*embit).getVertices()[4]; 
         tet = currPen->getTetrahedron(currPenFace);  
         // and is the tet in the maximal tree?    
         if (!inMaximalTree(tet)) 
          {
          // get index. 
          tetind = delta1 + tri4->tetrahedronIndex( tet ) - num_less_than( maxTreeStd, tri4->tetrahedronIndex( tet ) );
          int sign;
          if (embit == (*fac)->getEmbeddings().begin() )
           { sign = -1; }
          else
           {
            if ( (tet -> getEmbedding(0).getPentachoron() == currPen) && (tet -> getEmbedding(0).getTetrahedron() == currPenFace) )
             sign = -1; else sign = 1; 
           }
           relator.addTermFirst( tetind, sign ); 
          }
        } 

       // end pad
       currPenFace= (*fac)->getEmbedding((*fac)->getNumberOfEmbeddings()-1).getVertices()[3];
       tet = currPen->getTetrahedron(currPenFace);
       if (!tet->isBoundary()) std::cout<<"ERROR (unexpected tetrahedron) "<<std::endl;
       if (!inMaximalTree(tet))
        {
         tetind = delta1 + tri4->tetrahedronIndex( tet ) - num_less_than( maxTreeStd, tri4->tetrahedronIndex( tet ) );
         relator.addTermFirst( tetind, 1 ); // all 1-cells dual to boundary tets assumed oriented outwards
        }

       // finish
       NGroupExpression* relate( new NGroupExpression(relator) );
       pres.addRelation(relate);
      }
    }// that finishes interior cells dual to faces. 

   // now for boundary dual 2-cells -- pure boundary relator.
   // run through bcIx[1], for each edge call getEmbeddings(), this describes a disc and we need to crawl around the boundary.
   Dim4Edge* edg(NULL);
   for (unsigned long i=0; i<bcIx[1].size(); i++)
    {
     edg = tri4->getEdge( bcIx[1][i] );
     NGroupExpression relator; 
     NGroupExpression brelator; unsigned long bcompidx(0);
     // call normalsDim4BdryEdges[i] for bcIx[1][i] normal data.
     for (unsigned long j=0; j<normalsDim4BdryEdges[i].tet.size(); j++)
      {
       // tetrahedron normalsDim4BdryEdges[i].tet[j] edge normalsDim4BdryEdges[i].edgenum[j] and normalsDim4BdryEdges[i].edginc[j]
       const Dim4Tetrahedron* tet( normalsDim4BdryEdges[i].tet[j] );
        // unsigned long edgnum ( normalsDim4BdryEdges[i].edgenum[j] );
       NPerm4 edginc ( normalsDim4BdryEdges[i].edginc[j] );
       // find the face that edginc[2] [3] comes out of
       const Dim4Face* bfac (tet -> getFace( edginc[3] ) ); 
       if (!inMaximalTree(bfac))
        {
        // find this bfac's bcIx[2] index
        unsigned long bfacidx ( lower_bound( bcIx[2].begin(), bcIx[2].end(), tri4->faceIndex(bfac) ) - bcIx[2].begin() ); 
        // what boundary component are we in? 
        bcompidx = stdBdryCompIndexCD1[ bfacidx ] ;
        unsigned long bgen = lower_bound( stdBdryPi1Gen[bcompidx].begin(), stdBdryPi1Gen[bcompidx].end(), 
                              bfacidx ) - stdBdryPi1Gen[bcompidx].begin();

        if ( ( normalsDim4BdryFaces[bfacidx].secondtet == tet ) && ( normalsDim4BdryFaces[bfacidx].secondfacnum == edginc[3] ) )
	   {
           relator.addTermFirst( bfacidx - num_less_than( maxTreeStB, bfacidx ), 1 );  // + or 
           brelator.addTermFirst( bgen, 1 );	   
	   }
        else
	   {
           relator.addTermFirst( bfacidx - num_less_than( maxTreeStB, bfacidx ), -1);  // - or
           brelator.addTermFirst( bgen, -1 );	   
 	   }
        }
      }
      // finish
      NGroupExpression* relate ( new NGroupExpression(relator) );
      pres.addRelation(relate);   

      NGroupExpression* brelate ( new NGroupExpression(brelator) );
      stdBdryPi1[bcompidx].addRelation(brelate);
    } // end boundary dual 2-cells

   //  now for ideal dual 2-cells, dual to ideal 1-cells in ideal boundary, one for every icIx[1]
   for (unsigned long i=0; i<icIx[1].size(); i++)
    {
     NGroupExpression relator, brelator; 
     unsigned long bcompidx(0);

     const Dim4Face* fac ( tri4->getFace( icIx[1][i]/3 ) );
     unsigned long idEdg ( icIx[1][i] % 3 );  // ideal edge number of fac
     // lets acquire all the Dim4Pentachora incident to fac. 
     for (unsigned long j=0; j<fac->getNumberOfEmbeddings(); j++)
      {
       const Dim4Pentachoron* pen ( fac->getEmbedding(j).getPentachoron() );
       NPerm5 facemb( fac->getEmbedding(j).getVertices() );
       // idEdg of fac represents an ideal edge.  We want to find all the ideal incident tets and mark them appropriately. 
       //  so we find fac's embeddings in pentachoral and look up the relevant ideal tets.  
       // so we are going across the tetrahedron in pen whose vertices are marked by facemb[0][1][2] and [3] ie tetrahedron
       //  labelled by facemb[4]. 
       const Dim4Tetrahedron* tet ( pen->getTetrahedron( facemb[4] ) );
       NPerm5 tetemb( pen->getTetrahedronMapping( facemb[4] ) );
       // vertex idEdg of fac corresponds to tetemb^{-1} facemb[idEdg] of tet.  So let's see if it is in the maximal tree 
       // icIx[2] stored as 4*tetindx + num
       unsigned long I ( lower_bound( icIx[2].begin(), icIx[2].end(), 4*tri4->tetrahedronIndex(tet) + tetemb.preImageOf( facemb[idEdg] ) )  
                                     -icIx[2].begin() );
       bcompidx = idBdryCompIndexCD1[ I ];
       unsigned long J ( lower_bound( idBdryPi1Gen[bcompidx].begin(), idBdryPi1Gen[bcompidx].end(), I ) -
 				      idBdryPi1Gen[bcompidx].begin() );
       if (!inMaximalTree( tet, tetemb.preImageOf( facemb[idEdg] ) ) ) 
        {
         // what's the sign? check to see if this tet embeds into the pentachoron with same normal orientation, or not.  
         int sign (-1);
         if ( ( tet->getEmbedding(1).getPentachoron() == pen ) && ( tet->getEmbedding(1).getTetrahedron() == facemb[4] ) )
          sign = 1;
         // what's the generator?  
         unsigned long gennum ( delta0 + I - num_less_than( maxTreeIdB, I ) ); // index of generator.
         // assign to relator  
         relator.addTermFirst( gennum, sign );
         brelator.addTermFirst( J , sign );
        }
      }
     NGroupExpression* relate ( new NGroupExpression(relator) );
     pres.addRelation(relate);

     NGroupExpression* brelate ( new NGroupExpression(brelator) );
     idBdryPi1[bcompidx].addRelation(brelate);
    }

   //  now for ideal dual 2-cells, into the interior of the manifold, one for every icIx[2]
   for (unsigned long i=0; i<icIx[2].size(); i++)
    {
     NGroupExpression relator; 
     const Dim4Tetrahedron* tet ( tri4->getTetrahedron( icIx[2][i]/4 ) );
     unsigned long idFac ( icIx[2][i] % 4 );  // ideal end number of tet

     // these relators have at most 4 terms depending on how many of the relevant edges are in the maximal
     //  tree. A boundary term, two connect-to-boundary terms, and an interior tetrahedron term. 
     //  orientations set by tet->getEmbedding()  
     const Dim4Pentachoron* penL( tet->getEmbedding(0).getPentachoron() );
     NPerm5 tetLinc( tet->getEmbedding(0).getVertices() );
     unsigned long penLnum( tet->getEmbedding(0).getTetrahedron() );

     const Dim4Pentachoron* penR( tet->getEmbedding(1).getPentachoron() );
     NPerm5 tetRinc( tet->getEmbedding(1).getVertices() );
     unsigned long penRnum( tet->getEmbedding(1).getTetrahedron() );

     // 1-cells in order.
     // 1st boundary connector in maximal tree?
     if (!inMaximalTree( penL, tetLinc[idFac] ) )
      { 
       unsigned long I ( lower_bound( icIx[3].begin(), icIx[3].end(), 5*tri4->pentachoronIndex( penL ) + tetLinc[idFac] ) - icIx[3].begin() );
       unsigned long indx( delta2 + I - num_less_than(maxTreeSttIdB, I ) );
       relator.addTermFirst( indx, -1 );
      }
     // tet in maximal tree?
     if (!inMaximalTree( tet ) )
      {
       unsigned long I ( lower_bound( nicIx[3].begin(), nicIx[3].end(), tri4->tetrahedronIndex( tet ) ) - nicIx[3].begin() );
       unsigned long indx( delta1 + I - num_less_than(maxTreeStd, I ) );
       relator.addTermFirst( indx, 1 );
      }
     // 2nd boundary connector in maximal tree?  
     if (!inMaximalTree( penR, tetRinc[idFac] ) )
      {
       unsigned long I ( lower_bound( icIx[3].begin(), icIx[3].end(), 5*tri4->pentachoronIndex( penR ) + tetRinc[idFac] ) - icIx[3].begin() );
       unsigned long indx( delta2 + I - num_less_than(maxTreeSttIdB, I ) );
      relator.addTermFirst( indx, 1 );
      }
     // boundary fac in maximal tree? 
     if (!inMaximalTree( tet, idFac ) )
      {
       unsigned long indx( delta0 + i - num_less_than(maxTreeIdB, i) );
       relator.addTermFirst( indx, -1 );
      }

     NGroupExpression* relate ( new NGroupExpression(relator) );
     pres.addRelation(relate);
    }

  } // end tri4
 else
  { // tri3 TODO

  } // end tri3

  GroupPresLocator g_desc(whole_manifold, 0);
  NGroupPresentation* GPptr( new NGroupPresentation( pres ) );
  std::map< GroupPresLocator, NGroupPresentation* > *mGPptr = 
    const_cast< std::map< GroupPresLocator, NGroupPresentation* > *> (&groupPresentations);
  mGPptr->insert( std::pair<GroupPresLocator, NGroupPresentation*>(g_desc, GPptr) );

  for (unsigned long i=0; i<numStdBdryComps; i++) // push stdBdryPi1 onto stack
   {
    g_desc.sub_man = standard_boundary; 
    g_desc.component_index = i; 
    GPptr = new NGroupPresentation( stdBdryPi1[i] );
    std::map< GroupPresLocator, NGroupPresentation* > *XX =  
     const_cast< std::map< GroupPresLocator, NGroupPresentation* > *> (&groupPresentations);
    XX->insert( std::pair<GroupPresLocator, NGroupPresentation*>(g_desc, GPptr) );
   }

  for (unsigned long i=0; i<numIdealBdryComps; i++) // push idBdryPi1 onto stack
   {
    g_desc.sub_man = ideal_boundary; 
    g_desc.component_index = i; 
    GPptr = new NGroupPresentation( idBdryPi1[i] );
    std::map< GroupPresLocator, NGroupPresentation* > *XX =  
     const_cast< std::map< GroupPresLocator, NGroupPresentation* > *> (&groupPresentations);
    XX->insert( std::pair<GroupPresLocator, NGroupPresentation*>(g_desc, GPptr) );
   }

 // TODO, the stuff below needs adaptation to 3-dimensional triangulations
 // vector that gives the generator *number* in the ambient manifold's pi1 presentation from generator of boundary component. 
 std::vector< std::vector< unsigned long > > stdBdryGenIncl( numStdBdryComps );
 std::vector< std::vector< unsigned long > > idlBdryGenIncl( numIdealBdryComps );
 for (unsigned long i=0; i<stdBdryGenIncl.size(); i++)
  {
   stdBdryGenIncl[i].resize( stdBdryPi1Gen[i].size() );
   for (unsigned long j=0; j<stdBdryPi1Gen[i].size(); j++)
     stdBdryGenIncl[i][j] = stdBdryPi1Gen[i][j] - num_less_than( maxTreeStB, stdBdryPi1Gen[i][j] ) ;
  }
 for (unsigned long i=0; i<idlBdryGenIncl.size(); i++)
  {
   idlBdryGenIncl[i].resize( idBdryPi1Gen[i].size() );
   for (unsigned long j=0; j<idBdryPi1Gen[i].size(); j++)
     idlBdryGenIncl[i][j] =  numNonIdealBdryCells[2] - maxTreeStB.size()  + idBdryPi1Gen[i][j] - num_less_than( maxTreeIdB, idBdryPi1Gen[i][j] );
  }
 
 // push the inclusion maps onto the stack
 NHomGroupPresentation* presPtr;
 for (unsigned long i=0; i<stdBdryGenIncl.size(); i++)
  {
   std::vector< NGroupExpression > ithInclMap( stdBdryGenIncl[i].size() ); // one for every generator of pi1 i-th boundary component
   for (unsigned long j=0; j<ithInclMap.size(); j++)
      ithInclMap[j].addTermFirst( NGroupExpressionTerm( stdBdryGenIncl[i][j], 1 ) );
   presPtr = new NHomGroupPresentation( stdBdryPi1[i], pres, ithInclMap );
   HomGroupPresLocator h_desc( standard_boundary, i );
   std::map< HomGroupPresLocator, NHomGroupPresentation* > *XX = 
    const_cast< std::map< HomGroupPresLocator, NHomGroupPresentation* > *> (&homGroupPresentations);
   XX -> insert( std::pair< HomGroupPresLocator, NHomGroupPresentation*>(h_desc, presPtr) );
  }

 for (unsigned long i=0; i<idlBdryGenIncl.size(); i++)
  {
   std::vector< NGroupExpression > ithInclMap( idlBdryGenIncl[i].size() ); // one for every generator of pi1 i-th boundary component
   for (unsigned long j=0; j<ithInclMap.size(); j++)
      ithInclMap[j].addTermFirst( NGroupExpressionTerm( idlBdryGenIncl[i][j], 1 ) );
   presPtr = new NHomGroupPresentation( idBdryPi1[i], pres, ithInclMap );
   HomGroupPresLocator h_desc( ideal_boundary, i );
   std::map< HomGroupPresLocator, NHomGroupPresentation* > *XX = 
    const_cast< std::map< HomGroupPresLocator, NHomGroupPresentation* > *> (&homGroupPresentations);
   XX -> insert( std::pair< HomGroupPresLocator, NHomGroupPresentation*>(h_desc, presPtr) );
  }

 } // end pi1 code




} // namespace regina


