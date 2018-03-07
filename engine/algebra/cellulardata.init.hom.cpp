
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

namespace regina {


void CellularData::fillStandardToMixedHomCM()
{
 unsigned long aDim( (tri4!=NULL) ? 4 : 3 );
 ccMapType* CM(NULL); // pointer to an NSparseGrid< coverFacetData > 
 GroupExpression wordle; // temp
 for (unsigned d=0; d<=aDim; d++) smCM[d] = new 
   MatrixInt( numMixCells[d], numStandardCells[d] ); // TODO: erase
 long int delta[aDim]; 
 for (unsigned long d=0; d<aDim; d++) 
  delta[d] = numMixCells[d] - numIdealCells[d] - numNonIdealCells[d];

 for (unsigned long d=0; d<=aDim; d++) 
  {
   CM = new ccMapType(2);
   for (unsigned long j=0; j<numStandardCells[d]; j++)
    { // each standard d-dimensional simplex divided into d+1 bits.
     if (j < numNonIdealCells[d]) 
       for (unsigned long i=0; i<=d; i++)
        {
         smCM[d]->entry( (d+1)*j + i, j ) = 1; 
         // TODO fill wordle
         CM->setEntry( NMultiIndex< unsigned long >( j, i ),
                       coverFacetData( (d+1)*j + i, 1, wordle ) );
        }
     else 
        {
        smCM[d]->entry( delta[d] + j, j ) = 1;
        // TODO fill wordle
        CM->setEntry( NMultiIndex< unsigned long >( j, 0 ),
                      coverFacetData( delta[d] + j, 1, wordle ) );
        }
    } // j for loop

   // submit CC
   genCM.insert( std::pair< ChainMapLocator, ccMapType* >
    (ChainMapLocator(ChainComplexLocator(d,STD_coord), 
     ChainComplexLocator(d,MIX_coord)), CM ) );
  } // d for loop
}

void fillDualToMixedHomCM( const Triangulation<4>* tri, 
    const unsigned long numDualCells[5], const unsigned long numMixCells[5], 
    const unsigned long numNonIdealCells[5],
    std::vector< std::vector< unsigned long > > &dcIx, 
    std::vector< MatrixInt* > &dmCM)
{
 for (unsigned d=0; d<5; d++) dmCM[d] = new 
   MatrixInt( numMixCells[d], numDualCells[d] );
 long int delta[5]; 
 delta[0] = numNonIdealCells[0] +     numNonIdealCells[1] +   
            numNonIdealCells[2] + numNonIdealCells[3];
 delta[1] = 2*numNonIdealCells[1] + 3*numNonIdealCells[2] + 
            4*numNonIdealCells[3];
 delta[2] = 3*numNonIdealCells[2] + 6*numNonIdealCells[3];
 delta[3] = 4*numNonIdealCells[3];
 delta[4] = 0;

 // various useful pointers, index holders.
 const Face<4,0>* vrt(NULL);  const Face<4,1>* edg(NULL);  
   const Dim4Triangle* fac(NULL); const Face<4,3>* tet(NULL); 
   const Simplex<4>* pen(NULL);
 unsigned long J;

 for (unsigned long j=0; j<numNonIdealCells[4]; j++)
  {
    pen = tri->pentachoron(j); // dmCM[0]
    dmCM[0]->entry( delta[0] + j, j ) += 1;

    // dmCM[1]
    for (unsigned long i=0; i<5; i++)
	{
	 tet = pen->tetrahedron(i); if (!tet->isBoundary())
	  {
	   J = lower_bound( dcIx[1].begin(), dcIx[1].end(), 
                        tet->index() ) - dcIx[1].begin();
	   dmCM[1]->entry( delta[1] + 5*j + i, J ) += 1; 
	  }
	}

    // dmCM[2]
    for (unsigned long i=0; i<10; i++)
	{
	 fac = pen->triangle(i); if (!fac->isBoundary())
	  {
	   J = lower_bound( dcIx[2].begin(), dcIx[2].end(), 
                        fac->index() ) - dcIx[2].begin();
	   dmCM[2]->entry( delta[2] + 10*j + i, J ) += 1; 
	  }
	}

    // dmCM[3]
    for (unsigned long i=0; i<10; i++)
	{
	 edg = pen->edge(i); if (!edg->isBoundary())
	  {
	   J = lower_bound( dcIx[3].begin(), dcIx[3].end(), 
                        edg->index() ) - dcIx[3].begin();
	   dmCM[3]->entry( delta[3] + 10*j + i, J ) += 1; 
	  }
	}

    // dmCM[4]
    for (unsigned long i=0; i<5; i++)
	{
	 vrt = pen->vertex(i); if ( (!vrt->isBoundary()) && (!vrt->isIdeal()) )
	  {
	   J = lower_bound( dcIx[4].begin(), dcIx[4].end(), 
                        vrt->index() ) - dcIx[4].begin();
	   dmCM[4]->entry( delta[4] + 5*j + i, J ) += 
            pen->vertexMapping(i).sign(); 
	  }
	}
  }
}

void fillDualToMixedHomCM( const Triangulation<3>* tri, 
    const unsigned long numDualCells[5], const unsigned long numMixCells[5], 
    const unsigned long numNonIdealCells[5], 
    std::vector< std::vector< unsigned long > > &dcIx, 
    std::vector< MatrixInt* > &dmCM)
{
 for (unsigned d=0; d<4; d++) dmCM[d] = new 
   MatrixInt( numMixCells[d], numDualCells[d] );
 long int delta[4]; 
 delta[0] = numNonIdealCells[0] +     numNonIdealCells[1] +   
            numNonIdealCells[2];
 delta[1] = 2*numNonIdealCells[1] + 3*numNonIdealCells[2];
 delta[2] = 3*numNonIdealCells[2];
 delta[3] = 0;

 // various useful pointers, index holders.
 const Face<3,0>* vrt(NULL);  const Face<3,1>* edg(NULL);  const Face<3,2>* fac(NULL); 
   const Simplex<3>* tet(NULL); 
 unsigned long J;

 for (unsigned long j=0; j<numNonIdealCells[3]; j++)
  {
    tet = tri->tetrahedron(j);
    // dmCM[0]
    dmCM[0]->entry( delta[0] + j, j ) += 1;

    // dmCM[1]
    for (unsigned long i=0; i<4; i++)
	{
	 fac = tet->triangle(i); if (!fac->isBoundary())
	  {
	   J = lower_bound( dcIx[1].begin(), dcIx[1].end(), 
                        fac->index() ) - dcIx[1].begin();
	   dmCM[1]->entry( delta[1] + 4*j + i, J ) += 1; 
	  }
	}

    // dmCM[2]
    for (unsigned long i=0; i<6; i++)
	{
	 edg = tet->edge(i); if (!edg->isBoundary())
	  {
	   J = lower_bound( dcIx[2].begin(), dcIx[2].end(), 
                        edg->index() ) - dcIx[2].begin();
	   dmCM[2]->entry( delta[2] + 6*j + i, J ) += 1; 
	  }
	}

    // dmCM[3]
    for (unsigned long i=0; i<4; i++)
	{
	 vrt = tet->vertex(i); if ( (!vrt->isBoundary()) && (!vrt->isIdeal()) )
	  {
	   J = lower_bound( dcIx[3].begin(), dcIx[3].end(), 
                        vrt->index() ) - dcIx[3].begin();
	   dmCM[3]->entry( delta[3] + 4*j + i, J ) += 
         tet->vertexMapping(i).sign(); 
	  }
	}
  }
}

void fillBoundaryToStandardHomCM( unsigned aDim, // ambient dimension
   const unsigned long numStandardCells[5],         
   const unsigned long numStandardBdryCells[4], 
   const unsigned long numNonIdealBdryCells[4],     
   const unsigned long numIdealCells[4],
   const unsigned long numNonIdealCells[5],         
   std::vector< std::vector<unsigned long> > &nicIx, 	 
   std::vector< std::vector<unsigned long> > &bcIx, 
   std::vector< MatrixInt* > &sbiCM)
{
 for (unsigned d=0; d<aDim; d++) sbiCM[d] = new 
   MatrixInt( numStandardCells[d], numStandardBdryCells[d] );
 unsigned long I;
 for (unsigned long d=0; d<aDim; d++) 
  {// standard part of boundary
   for (unsigned j=0; j<numNonIdealBdryCells[d]; j++)
    {
     I = lower_bound( nicIx[d].begin(), nicIx[d].end(), bcIx[d][j] ) - 
                     nicIx[d].begin();
     sbiCM[d]->entry( I, j ) = 1;
    }
   // ideal part of boundary
   for (unsigned j=0; j<numIdealCells[d]; j++)
     sbiCM[d]->entry( numNonIdealCells[d] + j, 
        numNonIdealBdryCells[d] + j ) = 1;
  }
}

void fillStandardToRelativeHomCM( unsigned aDim, // ambient dimension
    const unsigned long numStandardCells[5],
    const unsigned long numRelativeCells[5], 
	const unsigned long numNonIdealCells[5],
	std::vector< std::vector<unsigned long> > &nicIx, 	 
    std::vector< std::vector<unsigned long> > &rIx, 
    std::vector< MatrixInt* > &strCM)
{
 for (unsigned d=0; d<aDim+1; d++) strCM[d] = new 
  MatrixInt( numRelativeCells[d], numStandardCells[d] );
 unsigned long I;
 for (unsigned long d=0; d<aDim+1; d++) 
  for (unsigned j=0; j<numNonIdealCells[d]; j++)
   if (binary_search( rIx[d].begin(), rIx[d].end(), nicIx[d][j] ) ) 
  {
   I = lower_bound( rIx[d].begin(), rIx[d].end(), nicIx[d][j] ) - rIx[d].begin();
   strCM[d]->entry( I, j ) = 1; 
  }
}

// H_{d+1}(M, \partial M) --> H_d(\partial M)
void fillDifferentialHomCM( const Triangulation<4>* tri,  
    const unsigned long numRelativeCells[5], 
    const unsigned long numStandardBdryCells[4],      
    const unsigned long numNonIdealBdryCells[4],    
    std::vector< std::vector<unsigned long> > &bcIx,  
    std::vector< std::vector<unsigned long> > &icIx,
    std::vector< std::vector<unsigned long> > &rIx,   
    std::vector< MatrixInt* > &schCM)               
{
 for (unsigned d=0; d<4; d++) 
   schCM[d] = new MatrixInt( numStandardBdryCells[d], numRelativeCells[d+1] );
 unsigned long I;
 // various useful pointers, index holders.
 const Face<4,1>* edg(NULL);  const Dim4Triangle* fac(NULL); 
 const Face<4,3>* tet(NULL); const Simplex<4>* pen(NULL);
 // boundary relative 1-cells
 unsigned long D=1;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {	
   edg = tri->edge(rIx[D][j]);
   for (unsigned long i=0; i<D+1; i++) if (edg->vertex(i)->isIdeal())
    {  
     I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), 
                      (D+1)*edg->index()+i ) - icIx[D-1].begin();
     schCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
    }  
   else if (edg->vertex(i)->isBoundary())
    {
     I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), 
                      edg->vertex(i)->index() )
      - bcIx[D-1].begin();
     schCM[D-1]->entry(I, j) += ( i == 0 ? -1 : 1 );
    } 
  }

 // boundary relative 2-cells
 D = 2;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {
   fac = tri->triangle(rIx[D][j]);
   for (unsigned long i=0; i < D+1; i++) 
    { 
     if (fac->vertex(i)->isIdeal())
      { // ideal ends of faces	
       I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), 
                        (D+1)*fac->index()+i ) - icIx[D-1].begin();
       schCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
      } // standard face boundaries
     if (fac->edge(i)->isBoundary())
      {
       Perm<5> P( fac->edgeMapping(i) );
       I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), 
                        fac->edge(i)->index() ) 
	- bcIx[D-1].begin();
       schCM[D-1]->entry(I, j) += P.sign();
      }
    }
 }

 // boundary relative 3-cells
 D = 3; // error in here
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {
   tet = tri->tetrahedron(rIx[D][j]);
   for (unsigned long i=0; i < D+1; i++) 
    { 
     if (tet->vertex(i)->isIdeal())
      { // ideal ends of faces	
       I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), 
                     (D+1)*tet->index()+i ) - icIx[D-1].begin();
       schCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
      } // standard face boundaries
     if (tet->triangle(i)->isBoundary())
      {
       Perm<5> P( tet->triangleMapping(i) );
       I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), 
                        tet->triangle(i)->index() )
	- bcIx[D-1].begin();
       schCM[D-1]->entry(I, j) += P.sign();
      }
    }
  }

 // boundary relative 4-cells
 D = 4;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {
   pen = tri->pentachoron(rIx[D][j]);
   for (unsigned long i=0; i < D+1; i++) 
    { 
     if (pen->vertex(i)->isIdeal())
      { // ideal ends of faces	
       I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), 
                (D+1)*pen->index()+i ) - icIx[D-1].begin();
       schCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
      } // standard face boundaries
     if (pen->tetrahedron(i)->isBoundary())
      {
       Perm<5> P( pen->tetrahedronMapping(i) );
       I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), 
        pen->tetrahedron(i)->index() ) - bcIx[D-1].begin();
       schCM[D-1]->entry(I, j) += P.sign();
      }
    }
  }

}

void fillDifferentialHomCM( const Triangulation<3>* tri,     
     const unsigned long numRelativeCells[5], 
     const unsigned long numStandardBdryCells[4],  
     const unsigned long numNonIdealBdryCells[4],    
     std::vector< std::vector<unsigned long> > &bcIx,  
     std::vector< std::vector<unsigned long> > &icIx,
     std::vector< std::vector<unsigned long> > &rIx,   
     std::vector< MatrixInt* > &schCM)               
{
 for (unsigned d=0; d<3; d++) schCM[d] = new 
    MatrixInt( numStandardBdryCells[d], numRelativeCells[d+1] );

 unsigned long I;
 // various useful pointers, index holders.
 const Face<3,1>* edg(NULL);  const Face<3,2>* fac(NULL); 
    const Simplex<3>* tet(NULL); 

 // boundary relative 1-cells
 unsigned long D=1;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {	
   edg = tri->edge(rIx[D][j]);
   for (unsigned long i=0; i<D+1; i++) if (edg->vertex(i)->isIdeal())
    {   // endpt i is ideal, find index
     I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), 
      (D+1)*edg->index()+i ) - icIx[D-1].begin();
     schCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
    } 
   else if (edg->vertex(i)->isBoundary())
    {
     I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), 
      edg->vertex(i)->index() ) - bcIx[D-1].begin();
     schCM[D-1]->entry(I, j) += ( i == 0 ? -1 : 1 );
    }
  }

 // boundary relative 2-cells
 D = 2;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {
   fac = tri->triangle(rIx[D][j]);
   for (unsigned long i=0; i < D+1; i++) 
    { 
     if (fac->vertex(i)->isIdeal())
      { // ideal ends of faces	
       I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), 
        (D+1)*fac->index()+i ) - icIx[D-1].begin();
       schCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
      } // standard face boundaries
     if (fac->edge(i)->isBoundary())
      {
       Perm<4> P( fac->edgeMapping(i) );
       I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), 
        fac->edge(i)->index() ) - bcIx[D-1].begin();
       schCM[D-1]->entry(I, j) += P.sign();
      }
    }
 }

 // boundary relative 3-cells
 D = 3;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {
   tet = tri->tetrahedron(rIx[D][j]);
   for (unsigned long i=0; i < D+1; i++) 
    { 
     if (tet->vertex(i)->isIdeal())
      { // ideal ends of tetrahedra
       I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), 
        (D+1)*tet->index()+i ) - icIx[D-1].begin();
       schCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
      } 
     if ( tet->triangle(i)->isBoundary() ) // standard face boundaries
      {
       Perm<4> P( tet->triangleMapping(i) );

       I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), 
        tet->triangle(i)->index() ) - bcIx[D-1].begin();

       schCM[D-1]->entry(I, j) += P.sign();
      }
    }
  }
}

// CM is the (appropriate size) identity matrix and wants to be the chain 
// map inducing Poincare duality so we need to correct the signs down the
//  diagonal -- we assume CM is an identity matrix to begin with.
void correctRelOrMat( MatrixInt &CM, unsigned long domdim, 
 const Triangulation<3>* tri3, const Triangulation<4>* tri4, 
 const std::vector< std::vector<unsigned long> > &dcIx )
{
// CM is from dual to std_rel_bdry coord
unsigned long aDim = ( tri3 ? 3 : 4 );
if ( aDim == 3 )
 { // domdim \in \{ 0, 1, 2, 3 \}
  if (domdim == 0) for (unsigned long i=0; i<CM.rows(); i++)
	{ // rIx[aDim][i], dcIx[0][i]
         const Simplex<3>* tet( tri3->tetrahedron( dcIx[domdim][i] ) );
	 CM.entry( i, i ) = tet->orientation();
	}
  else if (domdim == 1) for (unsigned long i=0; i<CM.rows(); i++)
	{
	 const Face<3,2>* fac( tri3->triangle( dcIx[domdim][i] ) );
         const Simplex<3>* tet( fac->embedding(0).tetrahedron() );
         Perm<4> emb( fac->embedding(0).vertices() );
	 CM.entry( i, i ) = emb.sign()*tet->orientation();  
	}   
  else if (domdim == 2) for (unsigned long i=0; i<CM.rows(); i++)
	{	 
	 const Face<3,1>* edg( tri3->edge( dcIx[domdim][i] ) );
         const Simplex<3>* tet( edg->embedding(0).tetrahedron() );
         Perm<4> emb( edg->embedding(0).vertices() );
 	 CM.entry( i, i ) = emb.sign()*tet->orientation();  
       }
  else if (domdim == 3) for (unsigned long i=0; i<CM.rows(); i++)
	{	 
	 const Face<3,0>* vrt( tri3->vertex( dcIx[domdim][i] ) );
         const Simplex<3>* tet( vrt->embedding(0).tetrahedron() );
         Perm<4> emb( vrt->embedding(0).vertices() );
	 CM.entry( i, i ) = emb.sign()*tet->orientation();  
        }
 }
else
 { // aDim == 4,  domdim \in \{ 0, 1, 2, 3, 4 \}
  if (domdim == 0) for (unsigned long i=0; i<CM.rows(); i++)
	{ // rIx[aDim][i], dcIx[0][i]
         const Simplex<4>* pen( tri4->pentachoron( dcIx[domdim][i] ) );
	 CM.entry( i, i ) = pen->orientation();
	}
  else if (domdim == 1) for (unsigned long i=0; i<CM.rows(); i++)
	{
	 const Face<4,3>* tet( tri4->tetrahedron( dcIx[domdim][i] ) );
         const Simplex<4>* pen( tet->embedding(0).pentachoron() );
         Perm<5> emb( tet->embedding(0).vertices() );
	 CM.entry( i, i ) = emb.sign()*pen->orientation();  
	}   
  else if (domdim == 2) for (unsigned long i=0; i<CM.rows(); i++)
	{	 
	 const Dim4Triangle* fac( tri4->triangle( dcIx[domdim][i] ) );
         const Simplex<4>* pen( fac->embedding(0).pentachoron() );
         Perm<5> emb( fac->embedding(0).vertices() );
 	 CM.entry( i, i ) = emb.sign()*pen->orientation();  
       }
  else if (domdim == 3) for (unsigned long i=0; i<CM.rows(); i++)
	{	 
	 const Face<4,1>* edg( tri4->edge( dcIx[domdim][i] ) );
         const Simplex<4>* pen( edg->embedding(0).pentachoron() );
         Perm<5> emb( edg->embedding(0).vertices() );
	 CM.entry( i, i ) = emb.sign()*pen->orientation();  
        }
  else if (domdim == 4) for (unsigned long i=0; i<CM.rows(); i++)
	{
	 const Face<4,0>* vrt( tri4->vertex( dcIx[domdim][i] ) );
         const Simplex<4>* pen( vrt->embedding(0).pentachoron() );
         Perm<5> emb( vrt->embedding(0).vertices() );
	 CM.entry( i, i ) = emb.sign()*pen->orientation();  
        }
 }
}
// end of chain map constructions

void fillChainMaps( Triangulation<3>* tri3, Triangulation<4>* tri4, 
    unsigned long numStandardCells[5],    unsigned long numDualCells[5],     
    unsigned long numMixCells[5],         unsigned long numStandardBdryCells[4],
    unsigned long numNonIdealCells[5],    unsigned long numIdealCells[4], 
    unsigned long numNonIdealBdryCells[4],unsigned long numRelativeCells[5], 
    unsigned long numDualRelCells[5],     unsigned long numMixRelCells[5],      
    unsigned long numMixBdryCells[4],     unsigned long numDualBdryCells[4], 
    std::vector< std::vector<unsigned long> > &nicIx, 
    std::vector< std::vector<unsigned long> > &icIx, 
    std::vector< std::vector<unsigned long> > &dcIx,  
    std::vector< std::vector<unsigned long> > &bcIx, 
    std::vector< std::vector<unsigned long> > &rIx, 
    std::vector< MatrixInt* > &sbiCM, std::vector< MatrixInt* > &smCM, 
    std::vector< MatrixInt* > &dmCM,  std::vector< MatrixInt* > &strCM, 
    std::vector< MatrixInt* > &schCM )
{
 if (tri4)
  {
   fillDualToMixedHomCM( tri4, numDualCells, numMixCells, 
                         numNonIdealCells, dcIx, dmCM );

   fillStandardToRelativeHomCM( 4, numStandardCells, numRelativeCells, 
                                numNonIdealCells, nicIx, rIx, strCM );

   fillBoundaryToStandardHomCM( 4, numStandardCells, numStandardBdryCells, 
    numNonIdealBdryCells, numIdealCells, numNonIdealCells, nicIx, bcIx, sbiCM);

   fillDifferentialHomCM( tri4, numRelativeCells, numStandardBdryCells,  
    numNonIdealBdryCells, bcIx,  icIx, rIx, schCM );   
  }
 if (tri3)
  {
   fillDualToMixedHomCM( tri3, numDualCells, numMixCells, 
                         numNonIdealCells, dcIx, dmCM );

   fillStandardToRelativeHomCM( 3, numStandardCells, numRelativeCells, 
                                numNonIdealCells, nicIx, rIx, strCM );

   fillBoundaryToStandardHomCM( 3, numStandardCells, numStandardBdryCells, 
    numNonIdealBdryCells, numIdealCells, numNonIdealCells, nicIx, bcIx, sbiCM);

   fillDifferentialHomCM( tri3, numRelativeCells, numStandardBdryCells,  
    numNonIdealBdryCells, bcIx,  icIx, rIx, schCM );  
  }
}



} // namespace regina


