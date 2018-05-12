
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

#include "maths/matrixops.h"
#include "maths/primes.h"
#include "algebra/cellulardata.h"
#include "maths/perm.h"

#include <map>
#include <list>
#include <cmath>
/**
 *  General comment: in the code you'll see lots of "TODO: compute wordle!" 
 *   comments. The wordle is the key information needed when one lifts the 
 *   triangulation or CW structure to the universal cover.  It keeps track of 
 *   lifts.  At present this is only implemented for parts of the skeleton and 
 *   it is a slow on-going process to fully implement it. 
 */
namespace regina {
 
// routine fills out genCC for the ChainComplexLocator == STD_coord, 
//  all dimensions
void CellularData::fillStandardHomologyCC()
{
 if (tri4)
  {
    ccMapType* CC(NULL); // pointer to an NSparseGrid< coverFacetData > 
    GroupExpression wordle; // temp

    // various useful pointers, index holders.
    const Face<4,1>* edg(NULL);  const Face<4,2>* fac(NULL); 
    const Face<4,3>* tet(NULL); const Simplex<4>* pen(NULL);
    unsigned long I;

    // fill out CC
    CC = new ccMapType(2);
    // now we fill them out, first sCC.  sCC[0] is zero, 
    unsigned long D=1; // sCC[D]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{ // endpts edge(nicIx[D][j]) ideal?
	edg = tri4->edge(nicIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->vertex(i)->isIdeal())
	 { // endpt i is ideal, find index
       I = icIxLookup( edg, i ) + numNonIdealCells[D-1]; 
       CC->setEntry( NMultiIndex< unsigned long >( j, i ),  
                     coverFacetData( I, 1, wordle ) );
       // TODO compute wordle!
	 } 
	else // endpt i is not ideal
	 {
       I = nicIxLookup(edg->vertex(i));
       CC->setEntry( NMultiIndex< unsigned long >( j, i ),  
                     coverFacetData( I, (i==0? -1:1), wordle ) );
       // TODO compute wordle!
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) 
      // scc[D]->entry( *, numNonIdealCells[D] + j )
      { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
	fac = tri4->triangle(icIx[D][j]/(D+2));
	for (unsigned long i=1; i<D+2; i++)
	 {
      Perm<5> P( fac->edgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  I = icIxLookup( fac->edge( (icIx[D][j] + i) % (D+2) ), 
                      P.preImageOf(icIx[D][j] % (D+2)) );
      CC->setEntry( NMultiIndex< unsigned long >( numNonIdealCells[D] + j, i ),  
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
	fac = tri4->triangle(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (fac->vertex(i)->isIdeal())
	   { // ideal ends of faces	
        I = icIxLookup( fac, i ) + numNonIdealCells[D-1];
        CC->setEntry( NMultiIndex< unsigned long >(j, i+D+1), 
            coverFacetData( I,  1, wordle ) );
        // TODO: compute wordle!
	   } // standard face boundaries
	  Perm<5> P( fac->edgeMapping(i) );
      I = nicIxLookup( fac->edge(i) );
      CC->setEntry( NMultiIndex< unsigned long >(j, i),  
                    coverFacetData( I, P.sign(), wordle ) );
      // TODO: compute wordle!
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) 
   // scc[D]->entry( *, j+numNonIdealCells[D-1] )
    { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
	 tet = tri4->tetrahedron(icIx[D][j]/(D+2));
	 for (unsigned long i=1; i < D+2; i++)
	  {
       Perm<5> P( tet->triangleMapping( (icIx[D][j] + i) % (D+2)) );
	   I = icIxLookup( tet->triangle( (icIx[D][j] + i) % (D+2) ), 
            P.preImageOf(icIx[D][j] % (D+2)) );
       CC->setEntry( NMultiIndex< unsigned long >( numNonIdealCells[D] + j, i ),  
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
	tet = tri4->tetrahedron(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (tet->vertex(i)->isIdeal())
	   { // ideal ends of faces	
        I = icIxLookup( tet, i );
        CC->setEntry( NMultiIndex< unsigned long >( j, i + D + 1 ),  
                      coverFacetData( numNonIdealCells[D-1] + I, 1, wordle ) );
        // TODO: compute wordle!
	   } // standard face boundaries
	  Perm<5> P( tet->triangleMapping(i) );
      I = nicIxLookup( tet->triangle(i) );
      CC->setEntry( NMultiIndex< unsigned long >( j, i ),  
                    coverFacetData( I, P.sign(), wordle ) );
      // TODO: compute wordle!
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) 
    // scc[D]->entry( *, j+numNonIdealCells[D-1] )
     { // icIx[D][j]/(D+2) pentachoron icIx[1][j] % (D+2) vertex
      pen = tri4->pentachoron(icIx[D][j]/(D+2));
	  for (unsigned long i=1; i < D+2; i++)
	   {
        Perm<5> P( pen->tetrahedronMapping( (icIx[D][j] + i) % (D+2)) );
        I = icIxLookup( pen->tetrahedron( (icIx[D][j] + i) % (D+2) ), 
            P.preImageOf(icIx[D][j] % (D+2)) );
        CC->setEntry( NMultiIndex< unsigned long >(numNonIdealCells[D] + j, i ),  
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
	pen = tri4->pentachoron(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (pen->vertex(i)->isIdeal())
	   { // ideal ends of faces	
        I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - 
            icIx[D-1].begin();
        CC->setEntry( NMultiIndex< unsigned long >( j, i + D + 1 ),  
                      coverFacetData( numNonIdealCells[D-1] + I, 1, wordle ) );
        // TODO: compute wordle!
	   } // standard face boundaries
	  Perm<5> P( pen->tetrahedronMapping(i) );
      I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), 
       pen->tetrahedron(i)->index() ) - nicIx[D-1].begin();
      CC->setEntry( NMultiIndex< unsigned long >( j, i ),  
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
    GroupExpression wordle; // temp

    // various useful pointers, index holders.
    const Face<3,1>* edg(NULL);  const Face<3,2>* fac(NULL); 
    const Simplex<3>* tet(NULL); 
    unsigned long I;
    
    // fill out CC
    CC = new ccMapType(2);
    // now we fill them out, first sCC.  sCC[0] is zero, 
    unsigned long D=1; // sCC[D]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{ // endpts edge(nicIx[D][j]) ideal?
	edg = tri3->edge(nicIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->vertex(i)->isIdeal())
	 {   // endpt i is ideal, find index
      I = icIxLookup( edg, i );
      CC->setEntry( NMultiIndex< unsigned long >( j, i ),  
                    coverFacetData( numNonIdealCells[D-1] + I, 1, wordle ) );
      // TODO compute wordle!
	 } 
	else // endpt i is not ideal
	 {
      I = nicIxLookup(  edg->vertex(i) );
      CC->setEntry( NMultiIndex< unsigned long >( j, i ),  
                    coverFacetData( I, (i==0 ? -1:1), wordle ) );
      // TODO compute wordle!
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) 
    // scc[D]->entry( *, numNonIdealCells[D] + j )
     { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
      fac = tri3->triangle(icIx[D][j]/(D+2));
	  for (unsigned long i=1; i<D+2; i++)
	   {
        Perm<4> P( fac->edgeMapping( (icIx[D][j] + i) % (D+2) ) );
        I = icIxLookup( fac->edge( (icIx[D][j] + i) % (D+2) ), 
            P.preImageOf(icIx[D][j] % (D+2)) );
        CC->setEntry(NMultiIndex< unsigned long >( numNonIdealCells[D] + j, i ),  
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
	fac = tri3->triangle(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (fac->vertex(i)->isIdeal())
	   { // ideal ends of faces	
        I = icIxLookup( fac, i );
        CC->setEntry( NMultiIndex< unsigned long >( j, i + D + 1 ),  
                      coverFacetData( numNonIdealCells[D-1] + I, 1, wordle ) );
        // TODO compute wordle!
	   } // standard face boundaries
      Perm<4> P( fac->edgeMapping(i) );
      I = nicIxLookup( fac->edge(i) );
      CC->setEntry( NMultiIndex< unsigned long >( j, i ),  
                    coverFacetData( I, P.sign(), wordle ) );
      // TODO compute wordle!
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) 
   // scc[D]->entry( *, j+numNonIdealCells[D-1] )
    { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
	tet = tri3->tetrahedron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
      Perm<4> P( tet->triangleMapping( (icIx[D][j] + i) % (D+2)) );
	  I = icIxLookup( tet->triangle( (icIx[D][j] + i) % (D+2) ), 
          P.preImageOf(icIx[D][j] % (D+2)) );
      CC->setEntry( NMultiIndex< unsigned long >( numNonIdealCells[D] + j, i ),  
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
	tet = tri3->tetrahedron(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (tet->vertex(i)->isIdeal())
	   { // ideal ends of faces	
        I = icIxLookup( tet, i );
        CC->setEntry( NMultiIndex< unsigned long >( j, i + D + 1 ),  
                     coverFacetData( numNonIdealCells[D-1] + I, 1, wordle ) );
        // TODO compute wordle!
	   } // standard face boundaries
	  Perm<4> P( tet->triangleMapping(i) );
      I = nicIxLookup( tet->triangle(i) );
      CC->setEntry( NMultiIndex< unsigned long >( j, i ),  
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
 * 1) skeletalobjecttype -> embedding() and 
 *    skeletonobjecttype ->embedding().vertices() 
 * and
 * 2) Simplex<4>->get(skeletalobjecttype)mapping()
 * though (2) is not available for dual edges as tetrahedronMapping() 
 * (dimension 4) and triangleMapping() (dimension 3) do not give 
 * orientation data.
 *
 * We try to keep the orientation conventions as portable-through-dimensions 
 * as possible. Provided the dimension of the dual cell is 2 or larger, there 
 * is a simple formula for the orientation of an incident cellular bit.  Say 
 * we're interested in a dual j-cell and the sign of an incident dual 
 * (j-1)-cell.  In a particular ambient n-simplex Delta_n, the parts of such 
 * cells inside Delta_n correspond to their dual n-j-1 and n-j sub-simplices
 * of Delta_n by intersection, we'll denote them E and F respectively. Let e 
 * be the element of \Sigma_{n+1} given by the inclusion E-->\Delta_{n+1} 
 * coming from Regina's getXXXMapping() function and f be the corresponding 
 * one for F-->\Delta_{n+1}.  Then the sign is given by the parity of 
 * e^{-1}\circ f \circ (transposition n-j+1, face number of E in F) as a 
 * permutation of the set {n-j+1, n-j+2, ..., n}
 */
void CellularData::fillDualHomologyCC()
//const Dim4Triangulation* tri, const unsigned long numDualCells[5], 
//	const std::vector< std::vector< unsigned long > > &dcIx, 
// std::vector< MatrixInt* > &dCC)
{
 ccMapType* CC(NULL); // pointer to an NSparseGrid< coverFacetData > 
 GroupExpression wordle; // temp
 unsigned long delta( numNonIdealBdryCells[2] - maxTreeStB.size() + 
                      numIdealCells[2] - maxTreeIdB.size() );
 if (tri4 != NULL)
 {
  // various useful pointers, index holders.
  const Face<4,0>* vrt(NULL);  const Face<4,1>* edg(NULL);  
  const Face<4,2>* fac(NULL); 
  const Face<4,3>* tet(NULL); const Simplex<4>* pen(NULL);

  CC = new ccMapType(2);
  unsigned long D = 1; // outer loop the row parameter. We start with dCC[1]
  for (unsigned long j=0; j<numDualCells[D]; j++) // dCC[D]->entry( *, j )
   {
    tet = tri4->tetrahedron( dcIx[D][j] );
    for (unsigned long i=0; i<2; i++)
     {
      pen = tet->embedding(i).pentachoron();
      CC->setEntry( NMultiIndex< unsigned long >( j, i ), 
        coverFacetData( dcIxLookup(pen), (i==0) ? -1 : 1, wordle ) );
      if  (!inMaximalTree(tet))  
        wordle.addTermFirst( GroupExpressionTerm( pi1Lookup(tet), 1 ) );
     }
    wordle.erase();
   }
  // submit CC
  genCC.insert( std::pair< ChainComplexLocator, ccMapType* >(
    ChainComplexLocator(D, DUAL_coord), CC ) );

  CC = new ccMapType(2);
  D = 2; // dCC[2]
  for (unsigned long j=0; j<numDualCells[D]; j++) // dCC[D]->entry( *, j )
   {
    fac = tri4->triangle( dcIx[D][j] );
    for (unsigned long i=0; i<fac->degree(); i++)
     {
      pen = fac->embedding(i).pentachoron();
      Perm<5> facinc( fac->embedding(i).vertices() );
      tet = pen->tetrahedron( facinc[4] );
      signed long sig( ((tet->embedding(1).pentachoron() == pen) &&
        (tet->embedding(1).tetrahedron() == facinc[4] )) ? 1 : -1 );
      if ( (!inMaximalTree(tet)) && (sig==-1) ) 
       wordle.addTermFirst( GroupExpressionTerm( pi1Lookup(tet), -1 ) );
      CC->setEntry( NMultiIndex< unsigned long >( j, i ), 
       coverFacetData( dcIxLookup(tet), sig, wordle ) );
      if ( (!inMaximalTree(tet)) && (sig==1) ) 
       wordle.addTermFirst( GroupExpressionTerm( pi1Lookup(tet), 1 ) );
     }
    wordle.erase();
   }
  // submit CC
  genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
    (ChainComplexLocator(D, DUAL_coord), CC ) );

  CC = new ccMapType(2);
  D = 3; // dCC[3]
  for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
   { fac = tri4->triangle( dcIx[D-1][i] );
    for (unsigned long j=0; j < 3; j++) {
	 edg = fac->edge(j); if (!edg->isBoundary())
      {
       pen = fac->embedding(0).pentachoron(); // our ambient pentachoron
	   // natural inclusions of our face and edge into the ambient pentachoron
	   Perm<5> facinc( fac->embedding(0).vertices() );
	   Perm<5> edginc( pen->edgeMapping( 
        Face<4,1>::edgeNumber[facinc[(j<=0) ? 1 : 0]][facinc[(j<=1)? 2 : 1]] ) );
       Perm<5> delta( edginc.inverse()*facinc*Perm<5>(2, j) ); 
       // consider as permutation of {2,3,4}
       delta = delta * Perm<5>( 0, delta[0] ); 
       // kill permutation of {0,1} part of delta
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( dcIxLookup( edg ), 3*i+j ),  
                     coverFacetData( i, delta.sign(), wordle ) );
   } } }
  // submit CC
  genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, DUAL_coord), CC ) );

  CC = new ccMapType(2);
  D = 4; // dCC[4]
  for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
   { edg = tri4->edge( dcIx[D-1][i] );
    for (unsigned long j=0; j < 2; j++) {
     vrt = edg->vertex(j); if (!vrt->isBoundary() && !vrt->isIdeal())
      {
       pen = edg->embedding(0).pentachoron(); // our ambient pentachoron
       // sign...
       Perm<5> edginc( edg->embedding(0).vertices() );
       Perm<5> vrtinc( pen->vertexMapping( edginc[j] ) );
       Perm<5> delta( vrtinc.inverse()*edginc*Perm<5>(1, j) );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( dcIxLookup(vrt), 4*i+j ),  
                    coverFacetData( i, delta.sign(), wordle ) );
   } } }
   // submit CC
   genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, DUAL_coord), CC ) );
 }
 else // tri3 != NULL
 {
  // various useful pointers, index holders.
  const Face<3,0>* vrt(NULL);  const Face<3,1>* edg(NULL);  
  const Face<3,2>* fac(NULL); const Simplex<3>* tet(NULL); 

  CC = new ccMapType(2);
  unsigned long D = 1; // outer loop the row parameter. We start with dCC[1]
  for (unsigned long j=0; j<numDualCells[D]; j++) // dCC[D]->entry( *, j )
   {
    fac = tri3->triangle( dcIx[D][j] );
    for (unsigned long i=0; i<2; i++)
     {
      tet = fac->embedding(i).tetrahedron();
      CC->setEntry( NMultiIndex< unsigned long >( j, i ), 
        coverFacetData( dcIxLookup(tet), (i==0) ? -1 : 1, wordle ) );
      if  (!inMaximalTree(fac))  
       wordle.addTermFirst( GroupExpressionTerm( pi1Lookup(fac), 1 ) );
     }
    wordle.erase();
   }
  // submit CC
  genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
    (ChainComplexLocator(D, DUAL_coord), CC ) );

  CC = new ccMapType(2);
  D = 2; // dCC[2]
  for (unsigned long j=0; j<numDualCells[D]; j++) // dCC[D]->entry( *, j )
   {
    edg = tri3->edge( dcIx[D][j] );
    for (unsigned long i=0; i<edg->degree(); i++)
     {
      tet = edg->embedding(i).tetrahedron();
      Perm<4> edginc( edg->embedding(i).vertices() );
      fac = tet->triangle( edginc[3] );
      signed long sig( ((fac->embedding(1).tetrahedron() == tet) &&
                   (fac->embedding(1).face() == edginc[3] )) ? 1 : -1 );
      if ( (!inMaximalTree(fac)) && (sig==-1) ) 
       wordle.addTermFirst( GroupExpressionTerm( pi1Lookup(fac), -1 ) );
      CC->setEntry( NMultiIndex< unsigned long >( j, i ), 
       coverFacetData( dcIxLookup(fac), sig, wordle ) );
      if ( (!inMaximalTree(fac)) && (sig==1) ) 
       wordle.addTermFirst( GroupExpressionTerm( pi1Lookup(fac), 1 ) );
     }
    wordle.erase();
   }
  // submit CC
  genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
    (ChainComplexLocator(D, DUAL_coord), CC ) );

  CC = new ccMapType(2);
  D = 3; // dCC[3]
  for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
   { 
    edg = tri3->edge( dcIx[D-1][i] );
    for (unsigned long j=0; j < 2; j++) {
    vrt = edg->vertex(j); if (!vrt->isBoundary() && !vrt->isIdeal())
     {
      tet = edg->embedding(0).tetrahedron(); // our ambient tetrahedron
      // sign...
      Perm<4> edginc( edg->embedding(0).vertices() );
      Perm<4> vrtinc( tet->vertexMapping( edginc[j] ) );
      Perm<4> delta( vrtinc.inverse()*edginc*Perm<4>(1, j) );
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >( dcIxLookup( vrt ), 2*i+j ),  
                    coverFacetData( i, delta.sign(), wordle ) );
  } } }
 // submit CC
 genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, DUAL_coord), CC ) );
 } // end tri3!=NULL
}


/* A description of the cells in the mixed cellular decomposition and their 
 * orientation conventions.  We use the convention that nicIx[j] indexes the 
 * standard, non-ideal j-cells,  icIx[j] the standard ideal j-cells.
 *
 * 0-cells:  <nicIx[0]>, nicIx[1], nicIx[2], nicIx[3], [nicIx[4]], <icIx[0]>.
 *           +           +         +         +         [+]         boundary or. 
 * 1-cells:  <2*nicIx[1]>, 3*nicIx[2], 4*nicIx[3], [5*nicIx[4]], <icIx[1]>
 *           edge or.      outward or. outward or. [dual]          boundary or. 
 * 2-cells:  <3*nicIx[2]>, (4 choose 2 == 6)*nicIx[3], 
 *                         [(5 choose 3 == 10)*nicIx[4]], <icIx[2]>
 *           face or.      char map conv.              [dual]               
 * 3-cells:  <4*nicIx[3]>, [(5 choose 2 == 10)*nicIx[4]], <icIx[3]>
 *           tetra or.     [dual]                         boundary or.
 * 4-cells:  [<5*nicIx[4]>]
 *           Inherits orientation of pentachoron 
 *
 * [] brackets indicate these cells are bits of dual polyhedral cells and can
 * therefore inherit their orientations.  <> brackets indicate these cells are 
 * bits of the standard cellular decomposition and inherit their orientations.
 * Our convention will be to orient <> objects via their standard cellular 
 * orientations, and [] objects via their dual cellular orientations, with <> 
 * beating [] when they compete. if unlabelled by [] or <> we choose 
 * orientations using dim4Tetrahedron::edgeMapping
 */
void CellularData::fillMixedHomologyCC()
{    
 ccMapType* CC(NULL); // pointer to an NSparseGrid< coverFacetData > 
 GroupExpression wordle; // temp

 if (tri4!=NULL)
  {
   // various useful pointers, index holders.
   const Face<4,0>* vrt(NULL);  const Face<4,1>* edg(NULL);  
   const Face<4,2>* fac(NULL); const Face<4,3>* tet(NULL); 
   const Simplex<4>* pen(NULL);
   unsigned long I;
   // we'll also need to remember some placeholder indices
   unsigned long ri1 = numNonIdealCells[0];        
   unsigned long ri2 = ri1 + numNonIdealCells[1];
   unsigned long ri3 = ri2 + numNonIdealCells[2];  
   unsigned long ri4 = ri3 + numNonIdealCells[3];
   unsigned long ri5 = ri4 + numNonIdealCells[4];
   unsigned long ci1 = 2*numNonIdealCells[1];      
   unsigned long ci2 = ci1 + 3*numNonIdealCells[2]; 
   unsigned long ci3 = ci2 + 4*numNonIdealCells[3];
   unsigned long ci4 = ci3 + 5*numNonIdealCells[4];

   unsigned long D = 1; // outer loop the column parameter. We start with mCC[1]
   CC = new ccMapType(2);
   for (unsigned long j=0; j<2*numNonIdealCells[1]; j++)
	{ // j % 2  mCC[D]->entry( *, j )
	 edg = tri4->edge(nicIx[1][j/2]); vrt = edg->vertex( j%2 );
	 if (vrt->isIdeal())
	  { 
       I = icIxLookup( edg, j%2 );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, 0 ),  
                     coverFacetData( ri5 + I, 1, wordle ) );
      }
     else
      { 
       I = nicIxLookup( vrt );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, 0 ),  
                     coverFacetData( I, ( (j%2)==0 ? -1 : 1 ), wordle ) ); 
      }  
     // TODO fill wordle
     CC->setEntry( NMultiIndex< unsigned long >( j, 1 ),  
                coverFacetData( ri1 + (j/2), ( (j%2)==0 ? 1 : -1 ), wordle ) ); 
	}

   for (unsigned long j=0; j<3*numNonIdealCells[2]; j++)
	{ // j % 3  mCC[D]->entry( *, ci1+j )
	 fac = tri4->triangle(nicIx[2][j/3]); edg = fac->edge( j%3 );
	 I = nicIxLookup( edg );
     // TODO fill wordle
     CC->setEntry( NMultiIndex< unsigned long >( ci1 + j, 0 ),  
                   coverFacetData( ri1 + I, 1, wordle ) ); 
     // TODO fill wordle
     CC->setEntry( NMultiIndex< unsigned long >( ci1 + j, 1 ),  
                   coverFacetData( ri2 + (j/3), -1, wordle ) ); 
	}

   for (unsigned long j=0; j<4*numNonIdealCells[3]; j++)
	{ // j % 4  mCC[D]->entry( *, ci2+j )
	 tet = tri4->tetrahedron(nicIx[3][j/4]); fac = tet->triangle( j%4 );
	 I = nicIxLookup( fac );
     // TODO fill wordle
     CC->setEntry( NMultiIndex< unsigned long >( ci2 + j, 0 ),  
                   coverFacetData( ri2 + I, 1, wordle ) ); 
     // TODO fill wordle
     CC->setEntry( NMultiIndex< unsigned long >( ci2 + j, 1 ),  
                   coverFacetData( ri3 + (j/4), -1, wordle ) ); 
	}

   for (unsigned long j=0; j<5*numNonIdealCells[4]; j++) 
	{ // j % 5  mCC[D]->entry( *, ci3+j )            
	 pen = tri4->pentachoron(nicIx[4][j/5]); 
     tet = pen->tetrahedron( j%5 );
	 int sig( (tet->embedding(0).pentachoron() == pen) &&
		  (tet->embedding(0).tetrahedron() == (j%5)) ? 1 : -1 ); 
	 I = nicIxLookup( tet );
     // TODO fill wordle
     CC->setEntry( NMultiIndex< unsigned long >( ci3 + j, 0 ),  
                   coverFacetData( ri3 + I, sig, wordle ) ); 
     // TODO fill wordle
     CC->setEntry( NMultiIndex< unsigned long >( ci3 + j, 1 ),  
                   coverFacetData( ri4 + (j/5), -sig, wordle ) ); 
	}

   for (unsigned long j=0; j<numIdealCells[D]; j++)
	{ // j%3    mCC[D]->entry( *, ci4+j )
	 fac = tri4->triangle( icIx[D][j]/(D+2) );
	 for (unsigned long i=1; i<(D+2); i++)
	  {          
	  Perm<5> P( fac->edgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  I = icIxLookup( fac->edge( (icIx[D][j] + i) % (D+2) ), 
       P.preImageOf(icIx[D][j] % (D+2)) );
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >( ci4 + j, i ),  
                    coverFacetData( ri5 + I, -P.sign(), wordle ) ); 
	  }
	}
   // submit CC
   genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
       (ChainComplexLocator(D, MIX_coord), CC ) );

   ri1 = ci1; ri2 = ci2; ri3 = ci3; ri4 = ci4;
   ci1 = 3*numNonIdealCells[2]; ci2 = ci1 + 6*numNonIdealCells[3]; 
   ci3 = ci2 + 10*numNonIdealCells[4];
   CC = new ccMapType(2);
   D = 2; // mCC[2]
   for (unsigned long j=0; j<3*numNonIdealCells[2]; j++) 
   // 4 boundary facets, 5 if vrt ideal
	{ // j%3,  mCC[D]->entry( *, j )
	 fac = tri4->triangle(nicIx[2][j/3]); vrt = fac->vertex( j%3 ); 
         for (unsigned i=1; i<3; i++)
	  {
       edg = fac->edge( (j+i)%3 ); 
       Perm<5> edginc = fac->edgeMapping( (j+i)%3 );
	   I = nicIxLookup( edg );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, i%3 ),  
        coverFacetData( 2*I + ( edginc.sign()==1 ? 2-i : i-1 ), 
        edginc.sign(), wordle ) ); 

       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, 3+(i%3) ),  
        coverFacetData( ri1 + 3*(j/3)+( (j+i)%3 ), 
        ( i==1 ? 1 : -1 ), wordle ) ); 
	  }
	 if (vrt->isIdeal()) 
	  {
	   I = icIxLookup( fac, j%3 );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, 0 ),  
                     coverFacetData( ri4 + I, 1, wordle ) ); 
	  }
	}

   // 6 facets in a tetrahedron
   for (unsigned long j=0; j<6*numNonIdealCells[3]; j++)  
	{ // j%6,  mCC[D]->entry( *, ci1+j )
	 tet = tri4->tetrahedron(nicIx[3][j/6]);   
	 Perm<5> edginc = tet->edgeMapping( j%6 ); 

	for (unsigned long i=0; i<2; i++)
	 {
      fac = tet->triangle( edginc[i+2] ); 
      Perm<5> facinc = tet->triangleMapping( edginc[i+2] );
	  I = nicIxLookup( fac );
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >( ci1 + j, i%2 ),  
       coverFacetData( ri1 + 3*I + (facinc.preImageOf(edginc[3-i])), 
                        ( i == 0 ? 1 : -1 ), wordle ) ); 
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >( ci1 + j, 2+(i%2) ),  
       coverFacetData( ri2 + 4*(j/6)+edginc[i+2], 
       ( i == 0 ? 1 : -1 ), wordle ) ); 
	 }
	}

   // dual to faces of pentachoron
   for (unsigned long j=0; j<10*numNonIdealCells[4]; j++) 
	{ // j%10, mCC[D]->entry( *, ci2+j )
	 pen = tri4->pentachoron(nicIx[4][j/10]); 
     Perm<5> facinc = pen->triangleMapping( j%10 );
	 for (unsigned long i=0; i<2; i++)
	  {
	   tet = pen->tetrahedron( facinc[i+3] ); 
       Perm<5> tetinc = pen->tetrahedronMapping( facinc[i+3] );
	   I = nicIxLookup( tet );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( ci2 + j, i%2 ),  
        coverFacetData( ri2 + 4*I + tetinc.preImageOf(facinc[4-i]), 
                     ( i == 0 ? 1 : -1 ), wordle ) ); 
	   int sig( (tet->embedding(0).pentachoron() == pen) &&
        (tet->embedding(0).tetrahedron() == facinc[i+3]) ? 1 : -1);
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( ci2 + j, 2+(i%2) ),  
        coverFacetData( ri3 + 5*(j/10) + facinc[i+3], sig*(i == 0 ? 1 : -1), 
        wordle ) ); 
	  }
	}

   for (unsigned long j=0; j<numIdealCells[2]; j++)
	{ // j$3,  mCC[D]->entry( *, ci3+j )
	 tet = tri4->tetrahedron( icIx[D][j]/(D+2) );
	 for (unsigned long i=1; i<D+2; i++)
	  {
       Perm<5> facinc( tet->triangleMapping( (icIx[D][j] + i) % (D+2)) );
       I = icIxLookup( tet->triangle( (icIx[D][j] + i) % (D+2) ), 
           facinc.preImageOf(icIx[D][j] % (D+2)) );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( ci3 + j, i ),  
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
	 tet = tri4->tetrahedron( nicIx[D][j/4] ); vrt = tet->vertex( j%4 );
	 for (unsigned long i=1; i<4; i++) 
     // boundary facets corresponding to face j+i&4 and edge j%4, (j+1)%4.
	  {
       fac = tet->triangle( (j+i)%4 ); 
       Perm<5> facinc = tet->triangleMapping( (j+i)%4 );  // tet index wrong?
	   I = nicIxLookup( fac );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, i%4 ),  
        coverFacetData( 3*I + facinc.preImageOf(j%4), facinc.sign(), wordle ) ); 
       Perm<5> edginc = tet->edgeMapping( 
        Face<3,1>::edgeNumber[ j%4 ][ (j+i)%4 ] );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, 4+(i%4) ),  
        coverFacetData( ri1 + 6*(j/4) + Face<3,1>::edgeNumber[ j%4 ][ (j+i)%4 ], 
        (edginc[1] == (j%4) ? 1 : -1) * edginc.sign(), wordle ) ); 
	  }
	 if (vrt->isIdeal())
	  {
	   I = icIxLookup( tet, j%4 );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, 0 ),  
                     coverFacetData( ri3 + I, 1, wordle ) ); 
	  }
	}

   for (unsigned long j=0; j<10*numNonIdealCells[4]; j++) 
	{ // j%10, mCC[D]->entry( *, ci1 + j )
	 pen = tri4->pentachoron( nicIx[D][j/10] ); 
     Perm<5> edginc( pen->edgeMapping( j%10 ) );
	 for (unsigned long i=2; i<5; i++) 
     // bdry facets 3 pt dual to edges in tets, 3 dual to faces in pen
	  { 
	   tet = pen->tetrahedron( edginc[i] ); 
       Perm<5> tetinc( pen->tetrahedronMapping( edginc[i] ) ); 
	   Perm<5> edgtetinc( tet->edgeMapping( // how edg sits in tet
		Face<3,1>::edgeNumber[tetinc.preImageOf(edginc[0])]
         [tetinc.preImageOf(edginc[1])] ) );
	   // part dual to an edge in tet.
	   I = nicIxLookup( tet );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( ci1 + j, i%5 ),  
        coverFacetData( ri1 + 6*I + Face<3,1>::edgeNumber[
        tetinc.preImageOf(edginc[0])][tetinc.preImageOf(edginc[1])], 
        -( (tetinc*edgtetinc).inverse()*edginc).sign(), wordle ) ); 
	   // part dual to a face in pen
	   Perm<5> facinc( pen->triangleMapping( 
        Face<4,2>::triangleNumber[edginc[0]][edginc[1]][edginc[i]] ) );
	   Perm<5> delta( edginc.inverse()*facinc*Perm<5>( 2, 
        facinc.preImageOf(edginc[i]) ) );
	   delta = delta * Perm<5>(0, delta[0]);
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( ci1+j, 5+(i%5) ), 
        coverFacetData( ri2 + 10*(j/10) + Face<4,2>::triangleNumber[
         edginc[0]][edginc[1]][edginc[i]], delta.sign(), wordle ) ); 
	  } 
	}

   for (unsigned long j=0; j<numIdealCells[D]; j++)
	{ // j%3, mCC[D]->entry( *, ci2 + j )
	pen = tri4->pentachoron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
      Perm<5> P( pen->tetrahedronMapping( (icIx[D][j] + i) % (D+2)) );
	  I = icIxLookup( pen->tetrahedron( (icIx[D][j] + i) % (D+2) ), 
       P.preImageOf(icIx[D][j] % (D+2)) );
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >( ci2 + j, i), 
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
	  pen = tri4->pentachoron( nicIx[D][j/5] ); vrt = pen->vertex( j%5 );
	  for (unsigned long i=1; i<5; i++) 
	   {
	    // standard boundary part opposite tet (j+i)%5 in facet j%5 of pen j/5
	    tet = pen->tetrahedron( (j+i)%5 ); 
        Perm<5> tetinc( pen->tetrahedronMapping( (j+i)%5 ) );
	    I = nicIxLookup( tet );
        // TODO fill wordle
        CC->setEntry( NMultiIndex< unsigned long >( j, i%5 ), 
         coverFacetData( 4*I + tetinc.preImageOf( j%5 ), 
          tetinc.sign(), wordle ) ); 
	    // part dual to edges 0,i
	    edg = pen->edge( Face<4,1>::edgeNumber[j%5][(i+j)%5] );
	    Perm<5> edginc( pen->edgeMapping( 
                       Face<4,1>::edgeNumber[j%5][(i+j)%5] ) );
        // TODO fill wordle
        CC->setEntry( NMultiIndex< unsigned long >( j, 5+(i%5) ), 
         coverFacetData( ri1 + 10*(j/5) + Face<4,1>::edgeNumber[j%5][(i+j)%5], 
          ( (edginc[1] == (j%5)) ? 1 : -1)*edginc.sign(), wordle ) ); 
       }
      // potentially ideal boundary part
      if (vrt->isIdeal()) 
		{
         I = icIxLookup( pen, j%5 );
         // TODO fill wordle
                 CC->setEntry( NMultiIndex< unsigned long >( j, 0 ), 
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
  const Face<3,0>* vrt(NULL);  const Face<3,1>* edg(NULL);  
  const Face<3,2>* fac(NULL); const Simplex<3>* tet(NULL); 
  unsigned long I;
  // we'll also need to remember some placeholder indices
  unsigned long ri1 = numNonIdealCells[0];        
  unsigned long ri2 = ri1 + numNonIdealCells[1];
  unsigned long ri3 = ri2 + numNonIdealCells[2];  
  unsigned long ri4 = ri3 + numNonIdealCells[3];
  unsigned long ci1 = 2*numNonIdealCells[1];      
  unsigned long ci2 = ci1 + 3*numNonIdealCells[2]; 
  unsigned long ci3 = ci2 + 4*numNonIdealCells[3];

  CC = new ccMapType(2);
  unsigned long D = 1; // outer loop the column parameter. We start with mCC[1]
  for (unsigned long j=0; j<2*numNonIdealCells[1]; j++)
   { // j % 2  mCC[D]->entry( *, j )
    edg = tri3->edge(nicIx[1][j/2]); vrt = edg->vertex( j%2 );
    if (vrt->isIdeal())
     { 
      I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - 
          icIx[D-1].begin();
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >( j, 0 ),
                    coverFacetData( ri4 + I, 1, wordle ) );
     }
    else
     { 
      I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), vrt->index() ) - nicIx[D-1].begin();
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >( j, 0 ),
                    coverFacetData( I, ( (j%2)==0 ? -1 : 1 ), wordle ) );
     }
    // TODO fill wordle
    CC->setEntry( NMultiIndex< unsigned long >( j, 1 ),
               coverFacetData( ri1 + (j/2), ( (j%2)==0 ? 1 : -1 ), wordle ) );
  	}

   for (unsigned long j=0; j<3*numNonIdealCells[2]; j++)
    { // j % 3  mCC[D]->entry( *, ci1+j )
     fac = tri3->triangle(nicIx[2][j/3]); edg = fac->edge( j%3 );
     I = lower_bound( nicIx[D].begin(), nicIx[D].end(), edg->index() ) - nicIx[D].begin();

     // TODO fill wordle
     CC->setEntry( NMultiIndex< unsigned long >( ci1 + j, 0 ),
                    coverFacetData( ri1 + I, 1, wordle ) );
     // TODO fill wordle
     CC->setEntry( NMultiIndex< unsigned long >( ci1 + j, 1 ),
                  coverFacetData( ri2 + (j/3), -1, wordle ) );
	}

   for (unsigned long j=0; j<4*numNonIdealCells[3]; j++)
	{ // j % 4  mCC[D]->entry( *, ci2+j )
	 tet = tri3->tetrahedron(nicIx[3][j/4]); fac = tet->triangle( j%4 );
	 int sig( (fac->embedding(0).tetrahedron() == tet) &&
		  (fac->embedding(0).face() == (j%4)) ? 1 : -1 ); 
	 I = lower_bound( nicIx[D+1].begin(), nicIx[D+1].end(), fac->index() ) - nicIx[D+1].begin();

     // TODO fill wordle
     CC->setEntry( NMultiIndex< unsigned long >( ci2 + j, 0 ),
                   coverFacetData( ri2 + I, sig, wordle ) );
     // TODO fill wordle
     CC->setEntry( NMultiIndex< unsigned long >( ci2 + j, 1 ),
                   coverFacetData( ri3 + (j/4), -sig, wordle ) );
	}

   for (unsigned long j=0; j<numIdealCells[D]; j++)
    { // j%3    mCC[D]->entry( *, ci4+j )
     fac = tri3->triangle( icIx[D][j]/(D+2) );
     for (unsigned long i=1; i<(D+2); i++)
      {          
       Perm<4> P( fac->edgeMapping( (icIx[D][j] + i) % (D+2) ) );
       unsigned long iX( (D+1)*fac->edge( (icIx[D][j] + i) % (D+2) )->index() // of corresp ideal 0-cell
            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	   I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - 
            icIx[D-1].begin();
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( ci3 + j, i ),
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
   for (unsigned long j=0; j<3*numNonIdealCells[2]; j++) 
   // 4 boundary facets, 5 if vrt ideal
    { // j%3,  mCC[D]->entry( *, j )
     fac = tri3->triangle(nicIx[2][j/3]); vrt = fac->vertex( j%3 ); 
     for (unsigned i=1; i<3; i++)
      {
       edg = fac->edge( (j+i)%3 ); 
       Perm<4> edginc = fac->edgeMapping( (j+i)%3 );
       I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), edg->index() ) - nicIx[D-1].begin();
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, (i%3) ),
        coverFacetData( 2*I + ( edginc.sign()==1 ? 2-i : i-1 ), 
         edginc.sign(), wordle ) );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, 3+(i%3) ),
        coverFacetData( ri1 + 3*(j/3)+( (j+i)%3 ), 
         ( i==1 ? 1 : -1 ), wordle ) );
      }
    if (vrt->isIdeal()) 
     {
      I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - 
              icIx[D-1].begin();
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >( j, 0 ),
                 coverFacetData( ri3 + I, 1, wordle ) );
     }
    }
   for (unsigned long j=0; j<6*numNonIdealCells[3]; j++) 
      // 6 facets in a tetrahedron
    { // j%6,  mCC[D]->entry( *, ci1+j )
     tet = tri3->tetrahedron(nicIx[3][j/6]); 
     Perm<4> edginc = tet->edgeMapping( j%6 ); 
     for (unsigned long i=0; i<2; i++)
      {
       fac = tet->triangle( edginc[i+2] ); 
       Perm<4> facinc = tet->triangleMapping( edginc[i+2] );
	   int sig( (fac->embedding(0).tetrahedron() == tet) &&
		   (fac->embedding(0).face() == edginc[i+2]) ? 1 : -1 );
       I = lower_bound( nicIx[D].begin(), nicIx[D].end(), 
         fac->index() ) - nicIx[D].begin(); 

       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( ci1 + j, (i%2) ),
        coverFacetData( ri1 + 3*I + (facinc.preImageOf(edginc[3-i])), 
         ( i == 0 ? 1 : -1 ), wordle ) );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( ci1 + j, 2+(i%2) ),
        coverFacetData( ri2 + 4*(j/6)+edginc[i+2], sig*( i == 0 ? 1 : -1 ), 
         wordle ) );
      }
    }

   for (unsigned long j=0; j<numIdealCells[2]; j++)
    { // j$3,  mCC[D]->entry( *, ci3+j )
     tet = tri3->tetrahedron( icIx[D][j]/(D+2) );
     for (unsigned long i=1; i<D+2; i++)
      {
       Perm<4> facinc( tet->triangleMapping( (icIx[D][j] + i) % (D+2)) );
       unsigned long iX( (D+1)*tet->triangle( (icIx[D][j] + i) % (D+2) )->index() + 
         facinc.preImageOf(icIx[D][j] % (D+2)) );
       I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - 
                        icIx[D-1].begin();
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( ci2 + j, i ),
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
     tet = tri3->tetrahedron( nicIx[D][j/4] ); 
     vrt = tet->vertex( j%4 );
     for (unsigned long i=1; i<4; i++) 
     // boundary facets corresponding to face j+i&4 and edge j%4, (j+1)%4.
      {
       fac = tet->triangle( (j+i)%4 ); 
       Perm<4> facinc = tet->triangleMapping( (j+i)%4 );  
       I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), 
        fac->index() ) - nicIx[D-1].begin();
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, (i%4) ),
        coverFacetData( 3*I + facinc.preImageOf(j%4), facinc.sign(), wordle ) );

       Perm<4> edginc = tet->edgeMapping( 
        Face<3,1>::edgeNumber[ j%4 ][ (j+i)%4 ] );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, 4+(i%4) ),  
        coverFacetData( ri1 + 6*(j/4) + Face<3,1>::edgeNumber[ j%4 ][ (j+i)%4 ], 
        (edginc[1] == (j%4) ? 1 : -1) * edginc.sign(), wordle ) );
	  }
     if (vrt->isIdeal())
      {
       I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - 
           icIx[D-1].begin();
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, 0 ),
                     coverFacetData( ri2 + I, 1, wordle ) );
      }
    }
    // submit CC
    genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, MIX_coord), CC ) );
  } // tri3 != NULL
} // fillMixedHomologyCC()

void CellularData::fillBoundaryHomologyCC()
{
 ccMapType* CC(NULL); // pointer to an NSparseGrid< coverFacetData > 
 GroupExpression wordle; // temp

 if (tri4!=NULL)
  {
   // various useful pointers, index holders.
   const Face<4,1>* edg(NULL);  const Face<4,2>* fac(NULL); 
   const Face<4,3>* tet(NULL); const Simplex<4>* pen(NULL);
   unsigned long I;

   // now we fill them out, first sbCC.  sbCC[0] is zero, 
   CC = new ccMapType(2);
   unsigned long D=1; // sbCC[D]
   for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++)
    { // endpts edge(nicIx[D][j]) ideal?
	edg = tri4->edge(bcIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->vertex(i)->isIdeal())
     { // endpt i is ideal, find index
      I = icIxLookup( edg, i );
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >( j, i ),
                coverFacetData(numNonIdealBdryCells[D-1] + I , 1, wordle ) );
     } 
	else // endpt i is not ideal
     {
      I = bcIxLookup( edg->vertex(i) );
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >( j, i ),
                    coverFacetData( I, ( i == 0 ? -1 : 1 ), wordle ) );
     }
    }
   for (unsigned long j=0; j<numIdealCells[D]; j++) 
    // sbCC[D]->entry( *, numNonIdealCells[D] + j )
    { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
     fac = tri4->triangle(icIx[D][j]/(D+2));
     for (unsigned long i=1; i<D+2; i++)
      {
       Perm<5> P( fac->edgeMapping( (icIx[D][j] + i) % (D+2) ) );
       I = icIxLookup( fac->edge( (icIx[D][j] + i) % (D+2) ), 
        P.preImageOf(icIx[D][j] % (D+2)) );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( 
        numNonIdealBdryCells[D] + j, i ),
        coverFacetData( numNonIdealBdryCells[D-1] + I, -P.sign(), wordle ) );
      }
    }
   // submit CC
   genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
       (ChainComplexLocator(D, STD_BDRY_coord), CC ) );

   CC = new ccMapType(2);
   D = 2; // sbCC[2]
   for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) 
   // sbCC[D]->entry( *,j )
    {
     fac = tri4->triangle(bcIx[D][j]);
     for (unsigned long i=0; i < D+1; i++) 
      { 
       if (fac->vertex(i)->isIdeal()) 
       // technically for a valid triangulation this never happens
       { // ideal ends of faces	
        I = icIxLookup( fac, i );
        // TODO fill wordle
        CC->setEntry( NMultiIndex< unsigned long >( j, i ),
              coverFacetData( numNonIdealBdryCells[D-1] + I, 1, wordle ) );
	   } // standard face boundaries
	  Perm<5> P( fac->edgeMapping(i) );
      I = bcIxLookup( fac->edge(i) );
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >( j, i ),
                    coverFacetData( I, P.sign(), wordle ) );
      }
    }
   for (unsigned long j=0; j<numIdealCells[D]; j++) 
   // sbCC[D]->entry( *, j+numNonIdealCells[D-1] )
    { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
     tet = tri4->tetrahedron(icIx[D][j]/(D+2));
     for (unsigned long i=1; i < D+2; i++)
      {
       Perm<5> P( tet->triangleMapping( (icIx[D][j] + i) % (D+2)) );
       I = icIxLookup( tet->triangle( (icIx[D][j] + i) % (D+2) ), 
           P.preImageOf(icIx[D][j] % (D+2)) );
       // TODO fill wordle
       CC->setEntry(NMultiIndex< unsigned long >(numNonIdealBdryCells[D] + j,i),
           coverFacetData( numNonIdealBdryCells[D-1] + I, -P.sign(), wordle ) );
      }
    }
   // submit CC
   genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_BDRY_coord), CC ) );

   CC = new ccMapType(2);
   D = 3; // sbCC[3]
   for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) 
   // sbCC[D]->entry( *,j )
    {
     tet = tri4->tetrahedron(bcIx[D][j]);
     for (unsigned long i=0; i < D+1; i++) 
      { 
       if (tet->vertex(i)->isIdeal())
        { // ideal ends of faces	
         I = icIxLookup( tet, i );
         // TODO fill wordle
         CC->setEntry( NMultiIndex< unsigned long >( j, i ),
              coverFacetData( numNonIdealBdryCells[D-1] + I, 1, wordle ) );
        } // standard face boundaries
       Perm<5> P( tet->triangleMapping(i) );
       I = bcIxLookup( tet->triangle(i) );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, i ),
                 coverFacetData( I, P.sign(), wordle ) );
      }
    }
   for (unsigned long j=0; j<numIdealCells[D]; j++) 
   // sbCC[D]->entry( *, j+numNonIdealCells[D-1] )
    { // icIx[D][j]/(D+2) pentachoron icIx[1][j] % (D+2) vertex
	pen = tri4->pentachoron(icIx[D][j]/(D+2));
    for (unsigned long i=1; i < D+2; i++)
     {
      Perm<5> P( pen->tetrahedronMapping( (icIx[D][j] + i) % (D+2)) );
      I = icIxLookup( pen->tetrahedron( (icIx[D][j] + i) % (D+2) ), 
          P.preImageOf(icIx[D][j] % (D+2)) );
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >(numNonIdealBdryCells[D]+j,i),
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
   const Face<3,1>* edg(NULL);  const Face<3,2>* fac(NULL); 
   const Simplex<3>* tet(NULL); unsigned long I;
    
   // now we fill them out, first sbCC.  sbCC[0] is zero, 
   CC = new ccMapType(2);
   unsigned long D=1; // sbCC[D]
   for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) 
   // sbCC[D]->entry( *,j )	
    { // endpts edge(bcIx[D][j]) ideal
     edg = tri3->edge(bcIx[D][j]);
     for (unsigned long i=0; i<D+1; i++) if (edg->vertex(i)->isIdeal())
      {   // endpt i is ideal, find index
       I = icIxLookup( edg, i );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, i ),
        coverFacetData( numNonIdealBdryCells[D-1] + I, 1, wordle ) );
      } 
     else // endpt i is not ideal
      {
       I = bcIxLookup( edg->vertex(i) );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, i ),
        coverFacetData( I, ( i == 0 ? -1 : 1 ), wordle ) );
      }
    }
   for (unsigned long j=0; j<numIdealCells[D]; j++) 
   // sbCC[D]->entry( *, numNonIdealBdryCells[D] + j )
    { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
     fac = tri3->triangle(icIx[D][j]/(D+2));
     for (unsigned long i=1; i<D+2; i++)
      {
       Perm<4> P( fac->edgeMapping( (icIx[D][j] + i) % (D+2) ) );
       I = icIxLookup( fac->edge( (icIx[D][j] + i) % (D+2) ), 
            P.preImageOf(icIx[D][j] % (D+2)) );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >(numNonIdealBdryCells[D]+j,i),
         coverFacetData( numNonIdealBdryCells[D-1] + I, -P.sign(), wordle ) );
      }
    }
   // submit CC
   genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
       (ChainComplexLocator(D, STD_BDRY_coord), CC ) );
   CC = new ccMapType(2);
   D = 2; // sbCC[2]   // sbCC[D]->entry( *,j )
   for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) 
    {
     fac = tri3->triangle(bcIx[D][j]);
     for (unsigned long i=0; i < D+1; i++) 
      { 
       if (fac->vertex(i)->isIdeal())
        { // ideal ends of faces	
         I = icIxLookup( fac, i );
         // TODO fill wordle
         CC->setEntry( NMultiIndex< unsigned long >( j, i ),
               coverFacetData( numNonIdealBdryCells[D-1] + I, 1, wordle ) );
        } // standard face boundaries
       Perm<4> P( fac->edgeMapping(i) );
       I = bcIxLookup( fac->edge(i) );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, i ),
                    coverFacetData( I, P.sign(), wordle ) );
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) 
   // sbCC[D]->entry( *, j+numNonIdealBdryCells[D-1] )
    { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
     tet = tri3->tetrahedron(icIx[D][j]/(D+2));
     for (unsigned long i=1; i < D+2; i++)
      {
       Perm<4> P( tet->triangleMapping( (icIx[D][j] + i) % (D+2)) );
       I = icIxLookup( tet->triangle( (icIx[D][j] + i) % (D+2) ), 
        P.preImageOf(icIx[D][j] % (D+2)) );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( 
        numNonIdealBdryCells[D] + j, i ),
       coverFacetData( numNonIdealBdryCells[D-1] + I, -P.sign(), wordle ) );
      }
    }
   // submit CC
   genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
       (ChainComplexLocator(D, STD_BDRY_coord), CC ) );
 } // end tri3 != NULL
} // fillBoundaryHomologyCC()


void CellularData::fillRelativeHomologyCC()
{
 ccMapType* CC(NULL); // pointer to an NSparseGrid< coverFacetData > 
 GroupExpression wordle; // temp

 if (tri4 != NULL)
  {
   // various useful pointers, index holders.
   const Face<4,1>* edg(NULL);  const Face<4,2>* fac(NULL); 
   const Face<4,3>* tet(NULL); const Simplex<4>* pen(NULL);
   unsigned long I;
    
   // now we fill them out, first srCC.  srCC[0] is zero, 
   CC = new ccMapType(2);
   unsigned long D=1; // srCC[D]
   for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{ // endpts edge(rIx[D][j]) ideal?
	edg = tri4->edge(rIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) 
     if ((!edg->vertex(i)->isIdeal()) && (!edg->vertex(i)->isBoundary()))
	 {
      I = rIxLookup( edg->vertex(i) );
      // TODO fill wordle
      CC->setEntry( NMultiIndex< unsigned long >( j, i ),
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
     fac = tri4->triangle(rIx[D][j]);
     for (unsigned long i=0; i < D+1; i++) if (!fac->edge(i)->isBoundary())
      { 
       Perm<5> P( fac->edgeMapping(i) );
       I = rIxLookup( fac->edge(i) );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, i ),
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
     tet = tri4->tetrahedron(rIx[D][j]);
     for (unsigned long i=0; i < D+1; i++) 
      if (!tet->triangle(i)->isBoundary())
       { 
	    Perm<5> P( tet->triangleMapping(i) );
        I = rIxLookup( tet->triangle(i) );
        // TODO fill wordle
        CC->setEntry( NMultiIndex< unsigned long >( j, i ),
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
      pen = tri4->pentachoron(rIx[D][j]);
      for (unsigned long i=0; i < D+1; i++) 
       if (!pen->tetrahedron(i)->isBoundary())
        { 
         Perm<5> P( pen->tetrahedronMapping(i) );
         I = rIxLookup( pen->tetrahedron(i) );
         // TODO fill wordle
         CC->setEntry( NMultiIndex< unsigned long >( j, i ),
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
    const Face<3,1>* edg(NULL);  const Face<3,2>* fac(NULL); 
    const Simplex<3>* tet(NULL); unsigned long I;
    
    // now we fill them out, first srCC.  srCC[0] is zero, 
    CC = new ccMapType(2);
    unsigned long D=1; // srCC[D]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
     { // endpts edge(rIx[D][j]) ideal?
      edg = tri3->edge(rIx[D][j]);
      for (unsigned long i=0; i<D+1; i++) 
       if ((!edg->vertex(i)->isIdeal()) && 
           (!edg->vertex(i)->isBoundary()))
        {
         I = rIxLookup( edg->vertex(i) );
         // TODO fill wordle
         CC->setEntry( NMultiIndex< unsigned long >( j, i ),
                        coverFacetData( I, ( i == 0 ? -1 : 1 ), wordle ) );
        }
      }
     // submit CC
     genCC.insert( std::pair< ChainComplexLocator, ccMapType* >
        (ChainComplexLocator(D, STD_REL_BDRY_coord), CC ) );

     D = 2; // srCC[2]
     CC = new ccMapType(2);
     for (unsigned long j=0; j<numRelativeCells[D]; j++) 
     // srCC[D]->entry( *,j )
      {
       fac = tri3->triangle(rIx[D][j]);
       for (unsigned long i=0; i < D+1; i++) 
        if (!fac->edge(i)->isBoundary())
         { 
          Perm<4> P( fac->edgeMapping(i) );
          I = rIxLookup( fac->edge(i) );
          // TODO fill wordle
          CC->setEntry( NMultiIndex< unsigned long >( j, i ),
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
     tet = tri3->tetrahedron(rIx[D][j]);
     for (unsigned long i=0; i < D+1; i++) if (!tet->triangle(i)->isBoundary())
      { 
       Perm<4> P( tet->triangleMapping(i) );
       I = rIxLookup( tet->triangle(i) );
       // TODO fill wordle
       CC->setEntry( NMultiIndex< unsigned long >( j, i ),
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
	std::vector< MatrixInt* > &dbCC)
{
    // initialize chain complex matrices.
    for (unsigned i=1; i<4; i++) // sbCC[i]
        dbCC[i] = new MatrixInt(numDualBdryCells[i-1], numDualBdryCells[i]);
    dbCC[0] = new MatrixInt(1, numDualBdryCells[0]);
    dbCC[4] = new MatrixInt(numDualBdryCells[3], 1);

    // various useful pointers, index holders.
    const Face<4,0>* vrt(NULL);  const Face<4,1>* edg(NULL);  const Face<4,2>* fac(NULL); 
    const Face<4,3>* tet(NULL); const Simplex<4>* pen(NULL);
    unsigned long J;

    // now we fill them out, first dbCC.  dbCC[0] is zero, 
    unsigned long D=1; // fill dbCC[D]
    for (unsigned long i=0; i<numNonIdealBdryCells[4-D]; i++) // dbCC[D]->entry( i,* )
	{ // non-ideal boundary
          // bcIx[4-D] indexes the {boundary faces} = {dual 1-cells in boundary}, iterate through boundary faces of tets...
          tet = tri->tetrahedron( bcIx[4-D][i] );
	  pen = tet->embedding(0).pentachoron(); // pen tet lives in
	  for (unsigned long j=0; j<4; j++)
	   {
	    fac = tet->triangle(j); 
            const Face<4,3>* itet(NULL);// internal tet bounding fac
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
//	      dCC[D]->entry( i, J ) += ( ( (fac->embedding(1).tetrahedron() == tet) && 
//				       (fac->embedding(1).triangle() == j) ) ? +1 : -1 );
// end new



} // namespace regina



