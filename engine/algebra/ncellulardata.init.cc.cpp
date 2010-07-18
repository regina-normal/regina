
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

#include "maths/matrixops.h"
#include "maths/nprimes.h"
#include "algebra/ncellulardata.h"
#include "maths/nperm3.h"
#include "maths/nperm4.h"

#include <map>
#include <list>
#include <cmath>

#include <iostream>
#include <sstream>

namespace regina {
 
// routine fills out genCC for the ChainComplexLocator == STD_coord, all dimensions
void NCellularData::fillStandardHomologyCC()
{
 if (tri4)
  {
    ccMapType* CC(NULL); // pointer to an NSparseGrid< coverFacetData > 
    NGroupExpression wordle; // temp

    // various useful pointers, index holders.
    const Dim4Edge* edg(NULL);  const Dim4Face* fac(NULL); const Dim4Tetrahedron* tet(NULL); const Dim4Pentachoron* pen(NULL);
    unsigned long I;

    // fill out CC
    CC = new ccMapType(2);
    // now we fill them out, first sCC.  sCC[0] is zero, 
    unsigned long D=1; // sCC[D]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{ // endpts getEdge(nicIx[D][j]) ideal?
	edg = tri4->getEdge(nicIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->getVertex(i)->isIdeal())
	 {   // endpt i is ideal, find index
          I = icIxLookup( edg, i ) + numNonIdealCells[D-1]; 
          CC->setEntry( NMultiIndex( j, i ),  
                        coverFacetData( I, 1, wordle ) );
          // TODO compute wordle!
	 } 
	else // endpt i is not ideal
	 {
          I = nicIxLookup(edg->getVertex(i));
          CC->setEntry( NMultiIndex( j, i ),  
                        coverFacetData( I, (i==0? -1:1), wordle ) );
          // TODO compute wordle!
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // scc[D]->entry( *, numNonIdealCells[D] + j )
        { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
	fac = tri4->getFace(icIx[D][j]/(D+2));
	for (unsigned long i=1; i<D+2; i++)
	 {
          NPerm5 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  I = icIxLookup( fac->getEdge( (icIx[D][j] + i) % (D+2) ), P.preImageOf(icIx[D][j] % (D+2)) );
          CC->setEntry( NMultiIndex( numNonIdealCells[D] + j, i ),  
                        coverFacetData( numNonIdealCells[D-1] + I, -P.sign(), wordle ) );
          // TODO: compute wordle!
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_coord), CC ) );

    // fill out CC
    CC = new ccMapType(2);
    D = 2; // sCC[2]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{
	fac = tri4->getFace(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (fac->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = icIxLookup( fac, i ) + numNonIdealCells[D-1];
            CC->setEntry( NMultiIndex(j, i+D+1), coverFacetData( I,  1, wordle ) );
            // TODO: compute wordle!
	   } // standard face boundaries
	  NPerm5 P( fac->getEdgeMapping(i) );
          I = nicIxLookup( fac->getEdge(i) );
          CC->setEntry( NMultiIndex(j, i),  
                        coverFacetData( I, P.sign(), wordle ) );
          // TODO: compute wordle!
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) // scc[D]->entry( *, j+numNonIdealCells[D-1] )
        { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
	tet = tri4->getTetrahedron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm5 P( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	  I = icIxLookup( tet->getFace( (icIx[D][j] + i) % (D+2) ), P.preImageOf(icIx[D][j] % (D+2)) );
          CC->setEntry( NMultiIndex( numNonIdealCells[D] + j, i ),  
                        coverFacetData( numNonIdealCells[D-1] + I, -P.sign(), wordle ) );
          // TODO: compute wordle!
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_coord), CC ) );

    // fill out CC
    CC = new ccMapType(2);
    D = 3; // sCC[3]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{
	tet = tri4->getTetrahedron(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (tet->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = icIxLookup( tet, i );
            CC->setEntry( NMultiIndex( j, i + D + 1 ),  
                          coverFacetData( numNonIdealCells[D-1] + I, 1, wordle ) );
            // TODO: compute wordle!
	   } // standard face boundaries
	  NPerm5 P( tet->getFaceMapping(i) );
          I = nicIxLookup( tet->getFace(i) );
          CC->setEntry( NMultiIndex( j, i ),  
                        coverFacetData( I, P.sign(), wordle ) );
          // TODO: compute wordle!
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // scc[D]->entry( *, j+numNonIdealCells[D-1] )
        { // icIx[D][j]/(D+2) pentachoron icIx[1][j] % (D+2) vertex
	pen = tri4->getPentachoron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm5 P( pen->getTetrahedronMapping( (icIx[D][j] + i) % (D+2)) );
	  I = icIxLookup( pen->getTetrahedron( (icIx[D][j] + i) % (D+2) ), P.preImageOf(icIx[D][j] % (D+2)) );
          CC->setEntry( NMultiIndex( numNonIdealCells[D] + j, i ),  
                        coverFacetData( numNonIdealCells[D-1] + I, -P.sign(), wordle ) );
          // TODO: compute wordle!
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_coord), CC ) );

    // fill out CC
    CC = new ccMapType(2);
    D = 4; // sCC[4]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{
	pen = tri4->getPentachoron(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (pen->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
            CC->setEntry( NMultiIndex( j, i + D + 1 ),  
                          coverFacetData( numNonIdealCells[D-1] + I, 1, wordle ) );
            // TODO: compute wordle!
	   } // standard face boundaries
	  NPerm5 P( pen->getTetrahedronMapping(i) );
          I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri4->tetrahedronIndex( pen->getTetrahedron(i) )) 
		- nicIx[D-1].begin();
          CC->setEntry( NMultiIndex( j, i ),  
                        coverFacetData( I, P.sign(), wordle ) );
          // TODO: compute wordle!
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_coord), CC ) );
  }
 else
  {
    ccMapType* CC(NULL); // pointer to an NSparseGrid< coverFacetData > 
    NGroupExpression wordle; // temp

    // various useful pointers, index holders.
    const NEdge* edg(NULL);  const NFace* fac(NULL); const NTetrahedron* tet(NULL); 
    unsigned long I;
    
    // fill out CC
    CC = new ccMapType(2);
    // now we fill them out, first sCC.  sCC[0] is zero, 
    unsigned long D=1; // sCC[D]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{ // endpts getEdge(nicIx[D][j]) ideal?
	edg = tri3->getEdge(nicIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->getVertex(i)->isIdeal())
	 {   // endpt i is ideal, find index
          I = icIxLookup( edg, i );
          CC->setEntry( NMultiIndex( j, i ),  
                        coverFacetData( numNonIdealCells[D-1] + I, 1, wordle ) );
          // TODO compute wordle!
	 } 
	else // endpt i is not ideal
	 {
          I = nicIxLookup(  edg->getVertex(i) );
          CC->setEntry( NMultiIndex( j, i ),  
                        coverFacetData( I, (i==0 ? -1:1), wordle ) );
          // TODO compute wordle!
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // scc[D]->entry( *, numNonIdealCells[D] + j )
        { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
	fac = tri3->getFace(icIx[D][j]/(D+2));
	for (unsigned long i=1; i<D+2; i++)
	 {
          NPerm4 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  I = icIxLookup( fac->getEdge( (icIx[D][j] + i) % (D+2) ), P.preImageOf(icIx[D][j] % (D+2)) );
          CC->setEntry( NMultiIndex( numNonIdealCells[D] + j, i ),  
                        coverFacetData( numNonIdealCells[D-1] + I, -P.sign(), wordle ) );
          // TODO compute wordle!
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_coord), CC ) );

    // fill out CC
    CC = new ccMapType(2);
    D = 2; // sCC[2]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{
	fac = tri3->getFace(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (fac->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = icIxLookup( fac, i );
            CC->setEntry( NMultiIndex( j, i + D + 1 ),  
                          coverFacetData( numNonIdealCells[D-1] + I, 1, wordle ) );
            // TODO compute wordle!
	   } // standard face boundaries
	  NPerm4 P( fac->getEdgeMapping(i) );
          I = nicIxLookup( fac->getEdge(i) );
          CC->setEntry( NMultiIndex( j, i ),  
                        coverFacetData( I, P.sign(), wordle ) );
          // TODO compute wordle!
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) // scc[D]->entry( *, j+numNonIdealCells[D-1] )
        { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
	tet = tri3->getTetrahedron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm4 P( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	  I = icIxLookup( tet->getFace( (icIx[D][j] + i) % (D+2) ), P.preImageOf(icIx[D][j] % (D+2)) );
          CC->setEntry( NMultiIndex( numNonIdealCells[D] + j, i ),  
                        coverFacetData( numNonIdealCells[D-1] + I, -P.sign(), wordle ) );
          // TODO compute wordle!
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_coord), CC ) );

    // fill out CC
    CC = new ccMapType(2);
    D = 3; // sCC[3]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{
	tet = tri3->getTetrahedron(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (tet->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = icIxLookup( tet, i );
            CC->setEntry( NMultiIndex( j, i + D + 1 ),  
                          coverFacetData( numNonIdealCells[D-1] + I, 1, wordle ) );
            // TODO compute wordle!
	   } // standard face boundaries
	  NPerm4 P( tet->getFaceMapping(i) );
          I = nicIxLookup( tet->getFace(i) );
          CC->setEntry( NMultiIndex( j, i ),  
                        coverFacetData( I, P.sign(), wordle ) );
          // TODO compute wordle!
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_coord), CC ) );
  } // end if tri4/tri3 
}



/* The orientations of the dual cells in Regina are given (equivalently) by: 
 * 1) skeletalobjecttype -> getEmbedding() and skeletonobjecttype ->getEmbedding().getVertices() 
 * and
 * 2) Dim4Pentachoron->get(skeletalobjecttype)mapping()
 * though (2) is not available for dual edges as getTetrahedronMapping() (dimension 4)
 * and getFaceMapping() (dimension 3) do not give orientation data.
 *
 * We try to keep the orientation conventions as portable-through-dimensions as possible. 
 * Provided the dimension of the dual cell is 2 or larger, there is a simple formula
 * for the orientation of an incident cellular bit.  Say we're interested in a dual j-cell
 * and the sign of an incident dual (j-1)-cell.  In a particular ambient n-simplex Delta_n, 
 * the parts of such cells inside Delta_n correspond to their dual n-j-1 and n-j sub-simplices
 * of Delta_n by intersection, we'll denote them E and F respectively. Let e be the element
 * of \Sigma_{n+1} given by the inclusion E-->\Delta_{n+1} coming from Regina's getXXXMapping()
 * function and f be the corresponding one for F-->\Delta_{n+1}.  Then the sign is given by
 * the parity of e^{-1}\circ f \circ (transposition n-j+1, face number of E in F) as a permutation
 * of the set {n-j+1, n-j+2, ..., n}
 */
void NCellularData::fillDualHomologyCC()
//const Dim4Triangulation* tri, const unsigned long numDualCells[5], 
//	const std::vector< std::vector< unsigned long > > &dcIx, std::vector< NMatrixInt* > &dCC)
{
 ccMapType* CC(NULL); // pointer to an NSparseGrid< coverFacetData > 
 NGroupExpression wordle; // temp

 if (tri4 != NULL)
 {
    // various useful pointers, index holders.
    const Dim4Vertex* vrt(NULL);  const Dim4Edge* edg(NULL);  const Dim4Face* fac(NULL); 
	const Dim4Tetrahedron* tet(NULL); const Dim4Pentachoron* pen(NULL);
    unsigned long J;

    CC = new ccMapType(2);
    unsigned long D = 1; // outer loop the row parameter. We start with dCC[1]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ pen = tri4->getPentachoron( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 5; j++) {
	    tet = pen->getTetrahedron(j);  if (!tet->isBoundary())
	     {
	      J = dcIxLookup( tet );
              int sig( ( (tet->getEmbedding(1).getPentachoron() == pen) && 
 	                 (tet->getEmbedding(1).getTetrahedron() == j) ) ? +1 : -1 );
              // TODO fill wordle
              CC->setEntry( NMultiIndex( J, 5*i+j ),  
                            coverFacetData( i, sig, wordle ) );
	     } }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, DUAL_coord), CC ) );

    CC = new ccMapType(2);
    D = 2; // dCC[2]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ tet = tri4->getTetrahedron( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 4; j++) {
	    fac = tet->getFace(j); if (!fac->isBoundary())
	     {
	      J = dcIxLookup( fac );
	      pen = tet->getEmbedding(1).getPentachoron(); // our ambient pentachoron
	      // the natural inclusions of our tetrahedron and face into the ambient pentachoron
	      NPerm5 tetinc( tet->getEmbedding(1).getVertices() );
	      NPerm5 facinc( pen->getFaceMapping( 
		Dim4Face::faceNumber[tetinc[(j<=0) ? 1 : 0]][tetinc[(j<=1)? 2 : 1]][tetinc[(j<=2)? 3 : 2]] 
						) );
              // TODO fill wordle
              CC->setEntry( NMultiIndex( J, 4*i+j ),   
                            coverFacetData( i, ( tetinc[4]==facinc[4] ? 1 : -1 ), wordle ) );
	     } }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, DUAL_coord), CC ) );

    CC = new ccMapType(2);
    D = 3; // dCC[3]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ fac = tri4->getFace( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 3; j++) {
	    edg = fac->getEdge(j); if (!edg->isBoundary())
	     {
	      J = dcIxLookup( edg );
	      pen = fac->getEmbedding(0).getPentachoron(); // our ambient pentachoron
	      // the natural inclusions of our face and edge into the ambient pentachoron
	      NPerm5 facinc( fac->getEmbedding(0).getVertices() );
	      NPerm5 edginc( pen->getEdgeMapping( 
		Dim4Edge::edgeNumber[facinc[(j<=0) ? 1 : 0]][facinc[(j<=1)? 2 : 1]] 
						) );
	      NPerm5 delta( edginc.inverse()*facinc*NPerm5(2, j) ); // we consider this as a permutation of {2,3,4}
              delta = delta * NPerm5( 0, delta[0] ); // kill permutation of {0,1} part of delta
              // TODO fill wordle
              CC->setEntry( NMultiIndex( J, 3*i+j ),  
                            coverFacetData( i, delta.sign(), wordle ) );
	     } }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, DUAL_coord), CC ) );

    CC = new ccMapType(2);
    D = 4; // dCC[4]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ edg = tri4->getEdge( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 2; j++) {
	    vrt = edg->getVertex(j); if (!vrt->isBoundary() && !vrt->isIdeal())
	     {
	      J = dcIxLookup(vrt);
	      pen = edg->getEmbedding(0).getPentachoron(); // our ambient pentachoron
	      // sign...
	      NPerm5 edginc( edg->getEmbedding(0).getVertices() );
	      NPerm5 vrtinc( pen->getVertexMapping( edginc[j] ) );
	      NPerm5 delta( vrtinc.inverse()*edginc*NPerm5(1, j) );
              // TODO fill wordle
              CC->setEntry( NMultiIndex( J, 4*i+j ),  
                            coverFacetData( i, delta.sign(), wordle ) );
	     } }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, DUAL_coord), CC ) );
 }
 else // tri3 != NULL
 {
    // various useful pointers, index holders.
    const NVertex* vrt(NULL);  const NEdge* edg(NULL);  const NFace* fac(NULL); const NTetrahedron* tet(NULL); 
    unsigned long J;

    CC = new ccMapType(2);
    unsigned long D = 1; // outer loop the row parameter. We start with dCC[1]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ tet = tri3->getTetrahedron( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 4; j++) {
	    fac = tet->getFace(j);  if (!fac->isBoundary())
	     {
              signed long sig(( ( (fac->getEmbedding(1).getTetrahedron() == tet) && 
				       (fac->getEmbedding(1).getFace() == j) ) ? +1 : -1 ));
	      J = dcIxLookup( fac );
              // TODO fill wordle
              CC->setEntry( NMultiIndex( J, 4*i+j ),  
                            coverFacetData( i, sig, wordle ) );
	     } }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, DUAL_coord), CC ) );

    CC = new ccMapType(2);
    D = 2; // dCC[2]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ fac = tri3->getFace( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 3; j++) {
	    edg = fac->getEdge(j); if (!edg->isBoundary())
	     {
	      J = dcIxLookup(edg);
	      tet = fac->getEmbedding(1).getTetrahedron(); // our ambient tetrahedron
	      // the natural inclusions of our tetrahedron and face into the ambient tetrahedron
	      NPerm4 facinc( fac->getEmbedding(1).getVertices() );
	      NPerm4 edginc( tet->getEdgeMapping( 
		NEdge::edgeNumber[facinc[(j<=0) ? 1 : 0]][facinc[(j<=1)? 2 : 1]] 
						) );
              // TODO fill wordle
              CC->setEntry( NMultiIndex( J, 3*i+j ),  
                            coverFacetData( i, ( facinc[3]==edginc[3] ? 1 : -1 ), wordle ) );
	     } }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, DUAL_coord), CC ) );

    CC = new ccMapType(2);
    D = 3; // dCC[3]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ edg = tri3->getEdge( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 2; j++) {
	    vrt = edg->getVertex(j); if (!vrt->isBoundary() && !vrt->isIdeal())
	     {
	      J = dcIxLookup( vrt );
	      tet = edg->getEmbedding(0).getTetrahedron(); // our ambient tetrahedron
	      // sign...
	      NPerm4 edginc( edg->getEmbedding(0).getVertices() );
	      NPerm4 vrtinc( tet->getVertexMapping( edginc[j] ) );
	      NPerm4 delta( vrtinc.inverse()*edginc*NPerm4(1, j) );
              // TODO fill wordle
              CC->setEntry( NMultiIndex( J, 2*i+j ),  
                            coverFacetData( i, delta.sign(), wordle ) );
	     } }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, DUAL_coord), CC ) );
 } // end tri3!=NULL
}


/* A description of the cells in the mixed cellular decomposition and their orientation 
 * conventions.  We use the convention that nicIx[j] indexes the standard, non-ideal j-cells, 
 * icIx[j] the standard ideal j-cells.
 *
 * 0-cells:  <nicIx[0]>, nicIx[1], nicIx[2], nicIx[3], [nicIx[4]], <icIx[0]>.
 *           +           +         +         +         [+]         boundary or. 
 * 1-cells:  <2*nicIx[1]>, 3*nicIx[2], 4*nicIx[3], [5*nicIx[4]], <icIx[1]>
 *           edge or.      outward or. outward or. [dual]          boundary or. 
 * 2-cells:  <3*nicIx[2]>, (4 choose 2 == 6)*nicIx[3], [(5 choose 3 == 10)*nicIx[4]], <icIx[2]>
 *           face or.      char map conv.              [dual]               
 * 3-cells:  <4*nicIx[3]>, [(5 choose 2 == 10)*nicIx[4]], <icIx[3]>
 *           tetra or.     [dual]                         boundary or.
 * 4-cells:  [<5*nicIx[4]>]
 *           Inherits orientation of pentachoron 
 *
 * [] brackets indicate these cells are bits of dual polyhedral cells and can
 * therefore inherit their orientations. 
 * <> brackets indicate these cells are bits of the standard cellular decomposition
 * and inherit their orientations.
 * Our convention will be to orient <> objects via their standard cellular orientations, 
 * and [] objects via their dual cellular orientations, with <> beating [] when they compete. 
 * if unlabelled by [] or <> we choose orientations using dim4Tetrahedron::getEdgeMapping
 */
void NCellularData::fillMixedHomologyCC()
{    
 ccMapType* CC(NULL); // pointer to an NSparseGrid< coverFacetData > 
 NGroupExpression wordle; // temp

 if (tri4!=NULL)
  {
   // various useful pointers, index holders.
    const Dim4Vertex* vrt(NULL);  const Dim4Edge* edg(NULL);  const Dim4Face* fac(NULL); 
	const Dim4Tetrahedron* tet(NULL); const Dim4Pentachoron* pen(NULL);
    unsigned long I;
   // we'll also need to remember some placeholder indices
   unsigned long ri1 = numNonIdealCells[0];        unsigned long ri2 = ri1 + numNonIdealCells[1];
   unsigned long ri3 = ri2 + numNonIdealCells[2];  unsigned long ri4 = ri3 + numNonIdealCells[3];
   unsigned long ri5 = ri4 + numNonIdealCells[4];
   unsigned long ci1 = 2*numNonIdealCells[1];      unsigned long ci2 = ci1 + 3*numNonIdealCells[2]; 
   unsigned long ci3 = ci2 + 4*numNonIdealCells[3];unsigned long ci4 = ci3 + 5*numNonIdealCells[4];

   unsigned long D = 1; // outer loop the column parameter. We start with mCC[1]
   CC = new ccMapType(2);
   for (unsigned long j=0; j<2*numNonIdealCells[1]; j++)
	{ // j % 2  mCC[D]->entry( *, j )
	 edg = tri4->getEdge(nicIx[1][j/2]); vrt = edg->getVertex( j%2 );
	 if (vrt->isIdeal())
	  { 
            I = icIxLookup( edg, j%2 );
            // TODO fill wordle
            CC->setEntry( NMultiIndex( j, 0 ),  
                          coverFacetData( ri5 + I, 1, wordle ) );
          }
         else
          { 
            I = nicIxLookup( vrt );
            // TODO fill wordle
            CC->setEntry( NMultiIndex( j, 0 ),  
                          coverFacetData( I, ( (j%2)==0 ? -1 : 1 ), wordle ) ); 
          }  
         // TODO fill wordle
         CC->setEntry( NMultiIndex( j, 1 ),  
                       coverFacetData( ri1 + (j/2), ( (j%2)==0 ? 1 : -1 ), wordle ) ); 
	}

   for (unsigned long j=0; j<3*numNonIdealCells[2]; j++)
	{ // j % 3  mCC[D]->entry( *, ci1+j )
	 fac = tri4->getFace(nicIx[2][j/3]); edg = fac->getEdge( j%3 );
	 I = nicIxLookup( edg );
         // TODO fill wordle
         CC->setEntry( NMultiIndex( ci1 + j, 0 ),  
                       coverFacetData( ri1 + I, 1, wordle ) ); 
         // TODO fill wordle
         CC->setEntry( NMultiIndex( ci1 + j, 1 ),  
                       coverFacetData( ri2 + (j/3), -1, wordle ) ); 
	}

   for (unsigned long j=0; j<4*numNonIdealCells[3]; j++)
	{ // j % 4  mCC[D]->entry( *, ci2+j )
	 tet = tri4->getTetrahedron(nicIx[3][j/4]); fac = tet->getFace( j%4 );
	 I = nicIxLookup( fac );
         // TODO fill wordle
         CC->setEntry( NMultiIndex( ci2 + j, 0 ),  
                       coverFacetData( ri2 + I, 1, wordle ) ); 
         // TODO fill wordle
         CC->setEntry( NMultiIndex( ci2 + j, 1 ),  
                       coverFacetData( ri3 + (j/4), -1, wordle ) ); 
	}

   for (unsigned long j=0; j<5*numNonIdealCells[4]; j++) 
	{ // j % 5  mCC[D]->entry( *, ci3+j )            
	 pen = tri4->getPentachoron(nicIx[4][j/5]); tet = pen->getTetrahedron( j%5 );
	 int sig( (tet->getEmbedding(0).getPentachoron() == pen) &&
		  (tet->getEmbedding(0).getTetrahedron() == (j%5)) ? 1 : -1 ); 
	 I = nicIxLookup( tet );
         // TODO fill wordle
         CC->setEntry( NMultiIndex( ci3 + j, 0 ),  
                       coverFacetData( ri3 + I, sig, wordle ) ); 
         // TODO fill wordle
         CC->setEntry( NMultiIndex( ci3 + j, 1 ),  
                       coverFacetData( ri4 + (j/5), -sig, wordle ) ); 
	}

   for (unsigned long j=0; j<numIdealCells[D]; j++)
	{ // j%3    mCC[D]->entry( *, ci4+j )
	 fac = tri4->getFace( icIx[D][j]/(D+2) );
	 for (unsigned long i=1; i<(D+2); i++)
	  {          
	  NPerm5 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  I = icIxLookup( fac->getEdge( (icIx[D][j] + i) % (D+2) ), P.preImageOf(icIx[D][j] % (D+2)) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( ci4 + j, i ),  
                        coverFacetData( ri5 + I, -P.sign(), wordle ) ); 
	  }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, MIX_coord), CC ) );

   ri1 = ci1; ri2 = ci2; ri3 = ci3; ri4 = ci4;
   ci1 = 3*numNonIdealCells[2]; ci2 = ci1 + 6*numNonIdealCells[3]; ci3 = ci2 + 10*numNonIdealCells[4];
   CC = new ccMapType(2);
   D = 2; // mCC[2]
   for (unsigned long j=0; j<3*numNonIdealCells[2]; j++) // 4 boundary facets, 5 if vrt ideal
	{ // j%3,  mCC[D]->entry( *, j )
	 fac = tri4->getFace(nicIx[2][j/3]); vrt = fac->getVertex( j%3 ); 
         for (unsigned i=1; i<3; i++)
	  {
	   edg = fac->getEdge( (j+i)%3 ); NPerm5 edginc = fac->getEdgeMapping( (j+i)%3 );
	   I = nicIxLookup( edg );
           // TODO fill wordle
           CC->setEntry( NMultiIndex( j, i%3 ),  
                         coverFacetData( 2*I + ( edginc.sign()==1 ? 2-i : i-1 ), edginc.sign(), wordle ) ); 

           // TODO fill wordle
           CC->setEntry( NMultiIndex( j, 3+(i%3) ),  
                         coverFacetData( ri1 + 3*(j/3)+( (j+i)%3 ), ( i==1 ? 1 : -1 ), wordle ) ); 
	  }
	 if (vrt->isIdeal()) 
	  {
	   I = icIxLookup( fac, j%3 );
           // TODO fill wordle
           CC->setEntry( NMultiIndex( j, 0 ),  
                         coverFacetData( ri4 + I, 1, wordle ) ); 
	  }
	}

   for (unsigned long j=0; j<6*numNonIdealCells[3]; j++)  // 6 facets in a tetrahedron
	{ // j%6,  mCC[D]->entry( *, ci1+j )
	 tet = tri4->getTetrahedron(nicIx[3][j/6]);   
	 NPerm5 edginc = tet->getEdgeMapping( j%6 ); 

	for (unsigned long i=0; i<2; i++)
	 {
          fac = tet->getFace( edginc[i+2] ); NPerm5 facinc = tet->getFaceMapping( edginc[i+2] );
	  I = nicIxLookup( fac );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( ci1 + j, i%2 ),  
                        coverFacetData( ri1 + 3*I + (facinc.preImageOf(edginc[3-i])), ( i == 0 ? 1 : -1 ), wordle ) ); 
          // TODO fill wordle
          CC->setEntry( NMultiIndex( ci1 + j, 2+(i%2) ),  
                        coverFacetData( ri2 + 4*(j/6)+edginc[i+2], ( i == 0 ? 1 : -1 ), wordle ) ); 
	 }
	}

   for (unsigned long j=0; j<10*numNonIdealCells[4]; j++) // dual to faces of pentachoron
	{ // j%10, mCC[D]->entry( *, ci2+j )
	 pen = tri4->getPentachoron(nicIx[4][j/10]); NPerm5 facinc = pen->getFaceMapping( j%10 );
	 for (unsigned long i=0; i<2; i++)
	  {
	   tet = pen->getTetrahedron( facinc[i+3] ); NPerm5 tetinc = pen->getTetrahedronMapping( facinc[i+3] );
	   I = nicIxLookup( tet );
           // TODO fill wordle
           CC->setEntry( NMultiIndex( ci2 + j, i%2 ),  
                         coverFacetData( ri2 + 4*I + tetinc.preImageOf(facinc[4-i]), ( i == 0 ? 1 : -1 ), wordle ) ); 
	   int sig( (tet->getEmbedding(0).getPentachoron() == pen) &&
		    (tet->getEmbedding(0).getTetrahedron() == facinc[i+3]) ? 1 : -1);
           // TODO fill wordle
           CC->setEntry( NMultiIndex( ci2 + j, 2+(i%2) ),  
                         coverFacetData( ri3 + 5*(j/10) + facinc[i+3], sig*(i == 0 ? 1 : -1), wordle ) ); 
	  }
	}

   for (unsigned long j=0; j<numIdealCells[2]; j++)
	{ // j$3,  mCC[D]->entry( *, ci3+j )
	 tet = tri4->getTetrahedron( icIx[D][j]/(D+2) );
	 for (unsigned long i=1; i<D+2; i++)
	  {
           NPerm5 facinc( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	   I = icIxLookup( tet->getFace( (icIx[D][j] + i) % (D+2) ), facinc.preImageOf(icIx[D][j] % (D+2)) );
           // TODO fill wordle
           CC->setEntry( NMultiIndex( ci3 + j, i ),  
                         coverFacetData( ri4 + I, -facinc.sign(), wordle ) ); 
	  }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, MIX_coord), CC ) );

   ri1 = ci1; ri2 = ci2; ri3 = ci3;
   ci1 = 4*numNonIdealCells[3]; ci2 = ci1 + 10*numNonIdealCells[4];
   CC = new ccMapType(2);
   D = 3; // mCC[3]
   for (unsigned long j=0; j<4*numNonIdealCells[3]; j++)
	{ // j%4, mCC[D]->entry( *, j )
	 tet = tri4->getTetrahedron( nicIx[D][j/4] ); vrt = tet->getVertex( j%4 );
	 for (unsigned long i=1; i<4; i++) // boundary facets corresponding to face j+i&4 and edge j%4, (j+1)%4.
	  {
	   fac = tet->getFace( (j+i)%4 ); NPerm5 facinc = tet->getFaceMapping( (j+i)%4 );  // tet index wrong?
	   I = nicIxLookup( fac );
           // TODO fill wordle
           CC->setEntry( NMultiIndex( j, i%4 ),  
                         coverFacetData( 3*I + facinc.preImageOf(j%4), facinc.sign(), wordle ) ); 
           NPerm5 edginc = tet->getEdgeMapping( NEdge::edgeNumber[ j%4 ][ (j+i)%4 ] );
           // TODO fill wordle
           CC->setEntry( NMultiIndex( j, 4+(i%4) ),  coverFacetData( ri1 + 6*(j/4) + NEdge::edgeNumber[ j%4 ][ (j+i)%4 ], 
                         (edginc[1] == (j%4) ? 1 : -1) * edginc.sign(), wordle ) ); 
	  }
	 if (vrt->isIdeal())
	  {
	   I = icIxLookup( tet, j%4 );
           // TODO fill wordle
           CC->setEntry( NMultiIndex( j, 0 ),  
                         coverFacetData( ri3 + I, 1, wordle ) ); 
	  }
	}

   for (unsigned long j=0; j<10*numNonIdealCells[4]; j++) 
	{ // j%10, mCC[D]->entry( *, ci1 + j )
	 pen = tri4->getPentachoron( nicIx[D][j/10] ); NPerm5 edginc( pen->getEdgeMapping( j%10 ) );
	 for (unsigned long i=2; i<5; i++) // boundary facets have 3 parts dual to edges in tets, 3 dual to faces in pen
	  { 
	   tet = pen->getTetrahedron( edginc[i] ); NPerm5 tetinc( pen->getTetrahedronMapping( edginc[i] ) ); 
	   NPerm5 edgtetinc( tet->getEdgeMapping( // how edg sits in tet
		NEdge::edgeNumber[tetinc.preImageOf(edginc[0])][tetinc.preImageOf(edginc[1])] ) );
	   // part dual to an edge in tet.
	   I = nicIxLookup( tet );
           // TODO fill wordle
           CC->setEntry( NMultiIndex( ci1 + j, i%5 ),  coverFacetData( 
                ri1 + 6*I + NEdge::edgeNumber[tetinc.preImageOf(edginc[0])][tetinc.preImageOf(edginc[1])], 
                -( (tetinc*edgtetinc).inverse()*edginc).sign(), wordle ) ); 
	   // part dual to a face in pen
	   NPerm5 facinc( pen->getFaceMapping( Dim4Face::faceNumber[edginc[0]][edginc[1]][edginc[i]] ) );
	   NPerm5 delta( edginc.inverse()*facinc*NPerm5( 2, facinc.preImageOf(edginc[i]) ) );
	   delta = delta * NPerm5(0, delta[0]);
           // TODO fill wordle
           CC->setEntry( NMultiIndex( ci1+j, 5+(i%5) ), coverFacetData( ri2 + 10*(j/10) + 
                Dim4Face::faceNumber[edginc[0]][edginc[1]][edginc[i]], delta.sign(), wordle ) ); 
	  } 
	}

   for (unsigned long j=0; j<numIdealCells[D]; j++)
	{ // j%3, mCC[D]->entry( *, ci2 + j )
	pen = tri4->getPentachoron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm5 P( pen->getTetrahedronMapping( (icIx[D][j] + i) % (D+2)) );
	  I = icIxLookup( pen->getTetrahedron( (icIx[D][j] + i) % (D+2) ), P.preImageOf(icIx[D][j] % (D+2)) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( ci2 + j, i), 
                        coverFacetData( ri3 + I, -P.sign(), wordle ) ); 
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, MIX_coord), CC ) );

   ri1 = ci1; ri2 = ci2;
   CC = new ccMapType(2);
   D = 4; // mCC[4]
   for (unsigned long j=0; j<5*numNonIdealCells[4]; j++)
	{ // j%5, mCC[D]->entry( *, j )
	  pen = tri4->getPentachoron( nicIx[D][j/5] ); vrt = pen->getVertex( j%5 );
	  for (unsigned long i=1; i<5; i++) 
	   {
	    // standard boundary part opposite tet (j+i)%5 in facet j%5 of pen j/5
	    tet = pen->getTetrahedron( (j+i)%5 ); NPerm5 tetinc( pen->getTetrahedronMapping( (j+i)%5 ) );
	    I = nicIxLookup( tet );
            // TODO fill wordle
            CC->setEntry( NMultiIndex( j, i%5 ), 
                          coverFacetData( 4*I + tetinc.preImageOf( j%5 ), tetinc.sign(), wordle ) ); 
	    // part dual to edges 0,i
	    edg = pen->getEdge( Dim4Edge::edgeNumber[j%5][(i+j)%5] );
	    NPerm5 edginc( pen->getEdgeMapping( Dim4Edge::edgeNumber[j%5][(i+j)%5] ) );
            // TODO fill wordle
            CC->setEntry( NMultiIndex( j, 5+(i%5) ), 
                          coverFacetData( ri1 + 10*(j/5) + Dim4Edge::edgeNumber[j%5][(i+j)%5], 
                          ( (edginc[1] == (j%5)) ? 1 : -1)*edginc.sign(), wordle ) ); 
	   }
	  // potentially ideal boundary part
	  if (vrt->isIdeal()) 
		{
		 I = icIxLookup( pen, j%5 );
                 // TODO fill wordle
                 CC->setEntry( NMultiIndex( j, 0 ), 
                               coverFacetData( ri2 + I, 1, wordle ) ); 
	 	}
	}   
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, MIX_coord), CC ) );
 }
  else // tri3 != NULL
 {    
   // various useful pointers, index holders.
    const NVertex* vrt(NULL);  const NEdge* edg(NULL);  const NFace* fac(NULL); const NTetrahedron* tet(NULL); 
    unsigned long I;
   // we'll also need to remember some placeholder indices
   unsigned long ri1 = numNonIdealCells[0];        unsigned long ri2 = ri1 + numNonIdealCells[1];
   unsigned long ri3 = ri2 + numNonIdealCells[2];  unsigned long ri4 = ri3 + numNonIdealCells[3];
   unsigned long ci1 = 2*numNonIdealCells[1];      unsigned long ci2 = ci1 + 3*numNonIdealCells[2]; 
   unsigned long ci3 = ci2 + 4*numNonIdealCells[3];

   CC = new ccMapType(2);
   unsigned long D = 1; // outer loop the column parameter. We start with mCC[1]
   for (unsigned long j=0; j<2*numNonIdealCells[1]; j++)
	{ // j % 2  mCC[D]->entry( *, j )
	 edg = tri3->getEdge(nicIx[1][j/2]); vrt = edg->getVertex( j%2 );
	 if (vrt->isIdeal())
	  { 
            I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - icIx[D-1].begin();
            // TODO fill wordle
            CC->setEntry( NMultiIndex( j, 0 ),
                          coverFacetData( ri4 + I, 1, wordle ) );
          }
         else
          { 
            I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri3->vertexIndex( vrt ) ) - nicIx[D-1].begin();
            // TODO fill wordle
            CC->setEntry( NMultiIndex( j, 0 ),
                          coverFacetData( I, ( (j%2)==0 ? -1 : 1 ), wordle ) );
          }
            // TODO fill wordle
            CC->setEntry( NMultiIndex( j, 1 ),
                          coverFacetData( ri1 + (j/2), ( (j%2)==0 ? 1 : -1 ), wordle ) );
  	}

   for (unsigned long j=0; j<3*numNonIdealCells[2]; j++)
	{ // j % 3  mCC[D]->entry( *, ci1+j )
	 fac = tri3->getFace(nicIx[2][j/3]); edg = fac->getEdge( j%3 );
	 I = lower_bound( nicIx[D].begin(), nicIx[D].end(), tri3->edgeIndex( edg ) ) - nicIx[D].begin();

         // TODO fill wordle
         CC->setEntry( NMultiIndex( ci1 + j, 0 ),
                       coverFacetData( ri1 + I, 1, wordle ) );
         // TODO fill wordle
         CC->setEntry( NMultiIndex( ci1 + j, 1 ),
                       coverFacetData( ri2 + (j/3), -1, wordle ) );
	}

   for (unsigned long j=0; j<4*numNonIdealCells[3]; j++)
	{ // j % 4  mCC[D]->entry( *, ci2+j )
	 tet = tri3->getTetrahedron(nicIx[3][j/4]); fac = tet->getFace( j%4 );
	 int sig( (fac->getEmbedding(0).getTetrahedron() == tet) &&
		  (fac->getEmbedding(0).getFace() == (j%4)) ? 1 : -1 ); 
	 I = lower_bound( nicIx[D+1].begin(), nicIx[D+1].end(), tri3->faceIndex( fac ) ) - nicIx[D+1].begin();

         // TODO fill wordle
         CC->setEntry( NMultiIndex( ci2 + j, 0 ),
                       coverFacetData( ri2 + I, sig, wordle ) );
         // TODO fill wordle
         CC->setEntry( NMultiIndex( ci2 + j, 1 ),
                       coverFacetData( ri3 + (j/4), -sig, wordle ) );
	}

   for (unsigned long j=0; j<numIdealCells[D]; j++)
	{ // j%3    mCC[D]->entry( *, ci4+j )
	 fac = tri3->getFace( icIx[D][j]/(D+2) );
	 for (unsigned long i=1; i<(D+2); i++)
	  {          
	  NPerm4 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  unsigned long iX( (D+1)*tri3->edgeIndex( fac->getEdge( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
          // TODO fill wordle
          CC->setEntry( NMultiIndex( ci3 + j, i ),
                        coverFacetData( ri4 + I, -P.sign(), wordle ) );
	  }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, MIX_coord), CC ) );

   ri1 = ci1; ri2 = ci2; ri3 = ci3; 
   ci1 = 3*numNonIdealCells[2]; ci2 = ci1 + 6*numNonIdealCells[3]; 
   CC = new ccMapType(2);
   D = 2; // mCC[2]
   for (unsigned long j=0; j<3*numNonIdealCells[2]; j++) // 4 boundary facets, 5 if vrt ideal
	{ // j%3,  mCC[D]->entry( *, j )
	 fac = tri3->getFace(nicIx[2][j/3]); vrt = fac->getVertex( j%3 ); 
         for (unsigned i=1; i<3; i++)
	  {
	   edg = fac->getEdge( (j+i)%3 ); NPerm4 edginc = fac->getEdgeMapping( (j+i)%3 );
	   I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri3->edgeIndex( edg ) ) - nicIx[D-1].begin();
           // TODO fill wordle
           CC->setEntry( NMultiIndex( j, (i%3) ),
                         coverFacetData( 2*I + ( edginc.sign()==1 ? 2-i : i-1 ), edginc.sign(), wordle ) );

           // TODO fill wordle
           CC->setEntry( NMultiIndex( j, 3+(i%3) ),
                         coverFacetData( ri1 + 3*(j/3)+( (j+i)%3 ), ( i==1 ? 1 : -1 ), wordle ) );
	  }
	 if (vrt->isIdeal()) 
	  {
	   I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - icIx[D-1].begin();
           // TODO fill wordle
           CC->setEntry( NMultiIndex( j, 0 ),
                         coverFacetData( ri3 + I, 1, wordle ) );
	  }
	}

   for (unsigned long j=0; j<6*numNonIdealCells[3]; j++)  // 6 facets in a tetrahedron
	{ // j%6,  mCC[D]->entry( *, ci1+j )
	 tet = tri3->getTetrahedron(nicIx[3][j/6]); NPerm4 edginc = tet->getEdgeMapping( j%6 ); 
	for (unsigned long i=0; i<2; i++)
	 {
          fac = tet->getFace( edginc[i+2] ); NPerm4 facinc = tet->getFaceMapping( edginc[i+2] );
	  int sig( (fac->getEmbedding(0).getTetrahedron() == tet) &&
		   (fac->getEmbedding(0).getFace() == edginc[i+2]) ? 1 : -1 );
          I = lower_bound( nicIx[D].begin(), nicIx[D].end(), tri3->faceIndex( fac ) ) - nicIx[D].begin(); 

          // TODO fill wordle
          CC->setEntry( NMultiIndex( ci1 + j, (i%2) ),
                        coverFacetData( ri1 + 3*I + (facinc.preImageOf(edginc[3-i])), ( i == 0 ? 1 : -1 ), wordle ) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( ci1 + j, 2+(i%2) ),
                        coverFacetData( ri2 + 4*(j/6)+edginc[i+2], sig*( i == 0 ? 1 : -1 ), wordle ) );
	 }
	}

   for (unsigned long j=0; j<numIdealCells[2]; j++)
	{ // j$3,  mCC[D]->entry( *, ci3+j )
	 tet = tri3->getTetrahedron( icIx[D][j]/(D+2) );
	 for (unsigned long i=1; i<D+2; i++)
	  {
           NPerm4 facinc( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	   unsigned long iX( (D+1)*tri3->faceIndex( tet->getFace( (icIx[D][j] + i) % (D+2) ) ) 
			            + ( facinc.preImageOf(icIx[D][j] % (D+2)) ) );
	   I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
           // TODO fill wordle
           CC->setEntry( NMultiIndex( ci2 + j, i ),
                         coverFacetData( ri3 + I, -facinc.sign(), wordle ) );
	  }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, MIX_coord), CC ) );

   ri1 = ci1; ri2 = ci2; 
   CC = new ccMapType(2);
   D = 3; // mCC[3]
   for (unsigned long j=0; j<4*numNonIdealCells[3]; j++)
	{ // j%4, mCC[D]->entry( *, j )
	 tet = tri3->getTetrahedron( nicIx[D][j/4] ); vrt = tet->getVertex( j%4 );
	 for (unsigned long i=1; i<4; i++) // boundary facets corresponding to face j+i&4 and edge j%4, (j+1)%4.
	  {
	   fac = tet->getFace( (j+i)%4 ); NPerm4 facinc = tet->getFaceMapping( (j+i)%4 );  
	   I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri3->faceIndex( fac ) ) - nicIx[D-1].begin();
           // TODO fill wordle
           CC->setEntry( NMultiIndex( j, (i%4) ),
                         coverFacetData( 3*I + facinc.preImageOf(j%4), facinc.sign(), wordle ) );

           NPerm4 edginc = tet->getEdgeMapping( NEdge::edgeNumber[ j%4 ][ (j+i)%4 ] );
           // TODO fill wordle
           CC->setEntry( NMultiIndex( j, 4+(i%4) ),  
                         coverFacetData( ri1 + 6*(j/4) + NEdge::edgeNumber[ j%4 ][ (j+i)%4 ], 
                         (edginc[1] == (j%4) ? 1 : -1) * edginc.sign(), wordle ) );
	  }
	 if (vrt->isIdeal())
	  {
	   I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - icIx[D-1].begin();
           // TODO fill wordle
           CC->setEntry( NMultiIndex( j, 0 ),
                         coverFacetData( ri2 + I, 1, wordle ) );
	  }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, MIX_coord), CC ) );
  } // tri3 != NULL
} // fillMixedHomologyCC()

void NCellularData::fillBoundaryHomologyCC()
{
 ccMapType* CC(NULL); // pointer to an NSparseGrid< coverFacetData > 
 NGroupExpression wordle; // temp

 if (tri4!=NULL)
  {
    // various useful pointers, index holders.
    const Dim4Edge* edg(NULL);  const Dim4Face* fac(NULL); const Dim4Tetrahedron* tet(NULL); const Dim4Pentachoron* pen(NULL);
    unsigned long I;

    // now we fill them out, first sbCC.  sbCC[0] is zero, 
    CC = new ccMapType(2);
    unsigned long D=1; // sbCC[D]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // sbCC[D]->entry( *,j )
	{ // endpts getEdge(nicIx[D][j]) ideal?
	edg = tri4->getEdge(bcIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->getVertex(i)->isIdeal())
	 {   // endpt i is ideal, find index
          I = icIxLookup( edg, i );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData(numNonIdealBdryCells[D-1] + I , 1, wordle ) );
	 } 
	else // endpt i is not ideal
	 {
          I = bcIxLookup( edg->getVertex(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( I, ( i == 0 ? -1 : 1 ), wordle ) );
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // sbCC[D]->entry( *, numNonIdealCells[D] + j )
        { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
	fac = tri4->getFace(icIx[D][j]/(D+2));
	for (unsigned long i=1; i<D+2; i++)
	 {
          NPerm5 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  I = icIxLookup( fac->getEdge( (icIx[D][j] + i) % (D+2) ), P.preImageOf(icIx[D][j] % (D+2)) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( numNonIdealBdryCells[D] + j, i ),
                        coverFacetData( numNonIdealBdryCells[D-1] + I, -P.sign(), wordle ) );
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_BDRY_coord), CC ) );

    CC = new ccMapType(2);
    D = 2; // sbCC[2]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // sbCC[D]->entry( *,j )
	{
	fac = tri4->getFace(bcIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (fac->getVertex(i)->isIdeal()) // technically for a valid triangulation this never happens
	   { // ideal ends of faces	
            I = icIxLookup( fac, i );
            // TODO fill wordle
            CC->setEntry( NMultiIndex( j, i ),
                          coverFacetData( numNonIdealBdryCells[D-1] + I, 1, wordle ) );
	   } // standard face boundaries
	  NPerm5 P( fac->getEdgeMapping(i) );
          I = bcIxLookup( fac->getEdge(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( I, P.sign(), wordle ) );
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) // sbCC[D]->entry( *, j+numNonIdealCells[D-1] )
        { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
	tet = tri4->getTetrahedron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm5 P( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	  I = icIxLookup( tet->getFace( (icIx[D][j] + i) % (D+2) ), P.preImageOf(icIx[D][j] % (D+2)) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( numNonIdealBdryCells[D] + j, i ),
                        coverFacetData( numNonIdealBdryCells[D-1] + I, -P.sign(), wordle ) );
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_BDRY_coord), CC ) );

    CC = new ccMapType(2);
    D = 3; // sbCC[3]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // sbCC[D]->entry( *,j )
	{
	tet = tri4->getTetrahedron(bcIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (tet->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = icIxLookup( tet, i );
            // TODO fill wordle
            CC->setEntry( NMultiIndex( j, i ),
                          coverFacetData( numNonIdealBdryCells[D-1] + I, 1, wordle ) );
	   } // standard face boundaries
	  NPerm5 P( tet->getFaceMapping(i) );
          I = bcIxLookup( tet->getFace(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( I, P.sign(), wordle ) );
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // sbCC[D]->entry( *, j+numNonIdealCells[D-1] )
        { // icIx[D][j]/(D+2) pentachoron icIx[1][j] % (D+2) vertex
	pen = tri4->getPentachoron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm5 P( pen->getTetrahedronMapping( (icIx[D][j] + i) % (D+2)) );
	  I = icIxLookup( pen->getTetrahedron( (icIx[D][j] + i) % (D+2) ), P.preImageOf(icIx[D][j] % (D+2)) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( numNonIdealBdryCells[D] + j, i ),
                        coverFacetData( numNonIdealBdryCells[D-1] + I, -P.sign(), wordle ) );
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_BDRY_coord), CC ) );
  }
 else // tri3 != NULL
  {
    // various useful pointers, index holders.
    const NEdge* edg(NULL);  const NFace* fac(NULL); const NTetrahedron* tet(NULL); 
    unsigned long I;
    
    // now we fill them out, first sbCC.  sbCC[0] is zero, 
    CC = new ccMapType(2);
    unsigned long D=1; // sbCC[D]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // sbCC[D]->entry( *,j )
	{ // endpts getEdge(bcIx[D][j]) ideal?
	edg = tri3->getEdge(bcIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->getVertex(i)->isIdeal())
	 {   // endpt i is ideal, find index
          I = icIxLookup( edg, i );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( numNonIdealBdryCells[D-1] + I, 1, wordle ) );
	 } 
	else // endpt i is not ideal
	 {
          I = bcIxLookup( edg->getVertex(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( I, ( i == 0 ? -1 : 1 ), wordle ) );
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // sbCC[D]->entry( *, numNonIdealBdryCells[D] + j )
        { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
	fac = tri3->getFace(icIx[D][j]/(D+2));
	for (unsigned long i=1; i<D+2; i++)
	 {
          NPerm4 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  I = icIxLookup( fac->getEdge( (icIx[D][j] + i) % (D+2) ), P.preImageOf(icIx[D][j] % (D+2)) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( numNonIdealBdryCells[D] + j, i ),
                        coverFacetData( numNonIdealBdryCells[D-1] + I, -P.sign(), wordle ) );
         }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_BDRY_coord), CC ) );

    CC = new ccMapType(2);
    D = 2; // sbCC[2]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // sbCC[D]->entry( *,j )
	{
	fac = tri3->getFace(bcIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (fac->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = icIxLookup( fac, i );
            // TODO fill wordle
            CC->setEntry( NMultiIndex( j, i ),
                          coverFacetData( numNonIdealBdryCells[D-1] + I, 1, wordle ) );
	   } // standard face boundaries
	  NPerm4 P( fac->getEdgeMapping(i) );
          I = bcIxLookup( fac->getEdge(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( I, P.sign(), wordle ) );
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) // sbCC[D]->entry( *, j+numNonIdealBdryCells[D-1] )
        { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
	tet = tri3->getTetrahedron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm4 P( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	  I = icIxLookup( tet->getFace( (icIx[D][j] + i) % (D+2) ), P.preImageOf(icIx[D][j] % (D+2)) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( numNonIdealBdryCells[D] + j, i ),
                        coverFacetData( numNonIdealBdryCells[D-1] + I, -P.sign(), wordle ) );
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_BDRY_coord), CC ) );
  } // end tri3 != NULL
} // fillBoundaryHomologyCC()


void NCellularData::fillRelativeHomologyCC()
{
 ccMapType* CC(NULL); // pointer to an NSparseGrid< coverFacetData > 
 NGroupExpression wordle; // temp

  if (tri4 != NULL)
   {
    // various useful pointers, index holders.
    const Dim4Edge* edg(NULL);  const Dim4Face* fac(NULL); const Dim4Tetrahedron* tet(NULL); const Dim4Pentachoron* pen(NULL);
    unsigned long I;
    
    // now we fill them out, first srCC.  srCC[0] is zero, 
    CC = new ccMapType(2);
    unsigned long D=1; // srCC[D]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{ // endpts getEdge(rIx[D][j]) ideal?
	edg = tri4->getEdge(rIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if ( (!edg->getVertex(i)->isIdeal()) && (!edg->getVertex(i)->isBoundary()) )
	 {
          I = rIxLookup( edg->getVertex(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( I, ( i == 0 ? -1 : 1 ), wordle ) );
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_REL_BDRY_coord), CC ) );

    D = 2; // srCC[2]
    CC = new ccMapType(2);
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{
	fac = tri4->getFace(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!fac->getEdge(i)->isBoundary())
	 { 
	  NPerm5 P( fac->getEdgeMapping(i) );
          I = rIxLookup( fac->getEdge(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( I, P.sign(), wordle ) );
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_REL_BDRY_coord), CC ) );

    D = 3; // srCC[3]
    CC = new ccMapType(2);
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{
	tet = tri4->getTetrahedron(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!tet->getFace(i)->isBoundary())
	 { 
	  NPerm5 P( tet->getFaceMapping(i) );
          I = rIxLookup( tet->getFace(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( I, P.sign(), wordle ) );
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_REL_BDRY_coord), CC ) );

    D = 4; // srCC[4]
    CC = new ccMapType(2);
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{
	pen = tri4->getPentachoron(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!pen->getTetrahedron(i)->isBoundary())
	 { 
	  NPerm5 P( pen->getTetrahedronMapping(i) );
          I = rIxLookup( pen->getTetrahedron(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( I, P.sign(), wordle ) );
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_REL_BDRY_coord), CC ) );
  }
  else // tri3 != NULL
  {
    // various useful pointers, index holders.
    const NEdge* edg(NULL);  const NFace* fac(NULL); const NTetrahedron* tet(NULL);
    unsigned long I;
    
    // now we fill them out, first srCC.  srCC[0] is zero, 
    CC = new ccMapType(2);
    unsigned long D=1; // srCC[D]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{ // endpts getEdge(rIx[D][j]) ideal?
	edg = tri3->getEdge(rIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if ( (!edg->getVertex(i)->isIdeal()) && (!edg->getVertex(i)->isBoundary()) )
	 {
          I = rIxLookup( edg->getVertex(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( I, ( i == 0 ? -1 : 1 ), wordle ) );
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_REL_BDRY_coord), CC ) );

    D = 2; // srCC[2]
    CC = new ccMapType(2);
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{
	fac = tri3->getFace(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!fac->getEdge(i)->isBoundary())
	 { 
	  NPerm4 P( fac->getEdgeMapping(i) );
          I = rIxLookup( fac->getEdge(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( I, P.sign(), wordle ) );
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_REL_BDRY_coord), CC ) );

    D = 3; // srCC[3]
    CC = new ccMapType(2);
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{
	tet = tri3->getTetrahedron(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!tet->getFace(i)->isBoundary())
	 { 
	  NPerm4 P( tet->getFaceMapping(i) );
          I = rIxLookup( tet->getFace(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex( j, i ),
                        coverFacetData( I, P.sign(), wordle ) );
	 }
	}
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_REL_BDRY_coord), CC ) );
  } // tri3 != NULL
}


// TODO -- incomplete!
/*
void fillBoundaryDualHomologyCC(const Dim4Triangulation* tri,  // fills dbCC
	const unsigned long numDualBdryCells[4],  const unsigned long numIdealCells[4], 
	const unsigned long numNonIdealBdryCells[4],
	const std::vector< std::vector< unsigned long > > &bcIx, const std::vector< std::vector< unsigned long > > &icIx, 
	std::vector< NMatrixInt* > &dbCC)
{
    // initialize chain complex matrices.
    for (unsigned i=1; i<4; i++) // sbCC[i]
        dbCC[i] = new NMatrixInt(numDualBdryCells[i-1], numDualBdryCells[i]);
    dbCC[0] = new NMatrixInt(1, numDualBdryCells[0]);
    dbCC[4] = new NMatrixInt(numDualBdryCells[3], 1);

    // various useful pointers, index holders.
    const Dim4Vertex* vrt(NULL);  const Dim4Edge* edg(NULL);  const Dim4Face* fac(NULL); 
    const Dim4Tetrahedron* tet(NULL); const Dim4Pentachoron* pen(NULL);
    unsigned long J;

    // now we fill them out, first dbCC.  dbCC[0] is zero, 
    unsigned long D=1; // fill dbCC[D]
    for (unsigned long i=0; i<numNonIdealBdryCells[4-D]; i++) // dbCC[D]->entry( i,* )
	{ // non-ideal boundary
          // bcIx[4-D] indexes the {boundary faces} = {dual 1-cells in boundary}, iterate through boundary faces of tets...
          tet = tri->getTetrahedron( bcIx[4-D][i] );
	  pen = tet->getEmbedding(0).getPentachoron(); // pen tet lives in
	  for (unsigned long j=0; j<4; j++)
	   {
	    fac = tet->getFace(j); 
            const Dim4Tetrahedron* itet(NULL);// internal tet bounding fac
	    // now we have to look at the embeddings of fac into pen, the first and last have
            // boundary tets so that's how we'll order them.

            J = lower_bound( bcIx[3-D].begin(), bcIx[3-D].end(), tri->faceIndex( fac ) ) - bcIx[3-D].begin();
            dbCC[D]->entry( i, J ) += 0; // hmm todo
            // orientation of dual boundary 1-cell corresponds to dual or of corresp non-boundary 1-cell
	   } 
	}
    for (unsigned long j=0; j<numIdealCells[3-D]; j++) // dbCC[D]->entry( j+numNonIdealBdryCells[D], numNonIdealBdryCells[D+1]+*)
        { // ideal boundary
          // dbCC[D]->entry( j+numNonIdealBdryCells[D-1], numNonIdealBdryCells[D]+ ??)
	}

    D = 2; // dbCC[D]

    D = 3; // dbCC[D]

}
*/
// also todo fillMixedBoundaryHomologyCC     mbCC
//           fillDualRelBoundaryHomologyCC   drCC
//           fillMixedRelBoundaryHomologyCC  mrCC
//	      J = lower_bound( dcIx[D].begin(), dcIx[D].end(), tri->faceIndex( fac ) ) - dcIx[D].begin();
//	      dCC[D]->entry( i, J ) += ( ( (fac->getEmbedding(1).getTetrahedron() == tet) && 
//				       (fac->getEmbedding(1).getFace() == j) ) ? +1 : -1 );
// end new



} // namespace regina



