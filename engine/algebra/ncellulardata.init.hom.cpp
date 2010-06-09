
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

namespace regina {


void fillStandardToMixedHomCM( unsigned aDim, // dimension of the triangulation
        const unsigned long numStandardCells[5],                 const unsigned long numMixCells[5], 
	const unsigned long numIdealCells[4],                    const unsigned long numNonIdealCells[5],
        std::vector< NMatrixInt* > &s_mCM)
{
 for (unsigned d=0; d<aDim+1; d++) s_mCM[d] = new NMatrixInt( numMixCells[d], numStandardCells[d] );
 long int delta[aDim]; for (unsigned long d=0; d<aDim; d++) delta[d] = numMixCells[d] - numIdealCells[d] - numNonIdealCells[d];

 for (unsigned long d=0; d<aDim+1; d++) for (unsigned long j=0; j<s_mCM[d]->columns(); j++)
    { // each standard d-dimensional simplex divided into d+1 bits.
     if (j < numNonIdealCells[d]) for (unsigned long i=0; i<d+1; i++) s_mCM[d]->entry( (d+1)*j + i, j ) = 1; 
     else s_mCM[d]->entry( delta[d] + j, j ) = 1;
    }
}

void fillDualToMixedHomCM( const Dim4Triangulation* tri, const unsigned long numDualCells[5], 
	const unsigned long numMixCells[5], const unsigned long numNonIdealCells[5],
        std::vector< std::vector< unsigned long > > &dcIx, std::vector< NMatrixInt* > &d_mCM)
{
 for (unsigned d=0; d<5; d++) d_mCM[d] = new NMatrixInt( numMixCells[d], numDualCells[d] );
 long int delta[5]; 
 delta[0] = numNonIdealCells[0] +     numNonIdealCells[1] +   numNonIdealCells[2] + numNonIdealCells[3];
 delta[1] = 2*numNonIdealCells[1] + 3*numNonIdealCells[2] + 4*numNonIdealCells[3];
 delta[2] = 3*numNonIdealCells[2] + 6*numNonIdealCells[3];
 delta[3] = 4*numNonIdealCells[3];
 delta[4] = 0;

 // various useful pointers, index holders.
 const Dim4Vertex* vrt(NULL);  const Dim4Edge* edg(NULL);  const Dim4Face* fac(NULL); 
   const Dim4Tetrahedron* tet(NULL); const Dim4Pentachoron* pen(NULL);
 unsigned long J;

 for (unsigned long j=0; j<numNonIdealCells[4]; j++)
  {
    pen = tri->getPentachoron(j);
    // d_mCM[0]
    d_mCM[0]->entry( delta[0] + j, j ) += 1;

    // d_mCM[1]
    for (unsigned long i=0; i<5; i++)
	{
	 tet = pen->getTetrahedron(i); if (!tet->isBoundary())
	  {
	   J = lower_bound( dcIx[1].begin(), dcIx[1].end(), tri->tetrahedronIndex( tet ) ) - dcIx[1].begin();
	   d_mCM[1]->entry( delta[1] + 5*j + i, J ) += 1; 
	  }
	}

    // d_mCM[2]
    for (unsigned long i=0; i<10; i++)
	{
	 fac = pen->getFace(i); if (!fac->isBoundary())
	  {
	   J = lower_bound( dcIx[2].begin(), dcIx[2].end(), tri->faceIndex( fac ) ) - dcIx[2].begin();
	   d_mCM[2]->entry( delta[2] + 10*j + i, J ) += 1; 
	  }
	}

    // d_mCM[3]
    for (unsigned long i=0; i<10; i++)
	{
	 edg = pen->getEdge(i); if (!edg->isBoundary())
	  {
	   J = lower_bound( dcIx[3].begin(), dcIx[3].end(), tri->edgeIndex( edg ) ) - dcIx[3].begin();
	   d_mCM[3]->entry( delta[3] + 10*j + i, J ) += 1; 
	  }
	}

    // d_mCM[4]
    for (unsigned long i=0; i<5; i++)
	{
	 vrt = pen->getVertex(i); if ( (!vrt->isBoundary()) && (!vrt->isIdeal()) )
	  {
	   J = lower_bound( dcIx[4].begin(), dcIx[4].end(), tri->vertexIndex( vrt ) ) - dcIx[4].begin();
	   d_mCM[4]->entry( delta[4] + 5*j + i, J ) += pen->getVertexMapping(i).sign(); 
	  }
	}
  }
}

void fillDualToMixedHomCM( const NTriangulation* tri, const unsigned long numDualCells[5], 
	const unsigned long numMixCells[5], const unsigned long numNonIdealCells[5],
        std::vector< std::vector< unsigned long > > &dcIx, std::vector< NMatrixInt* > &d_mCM)
{
 for (unsigned d=0; d<4; d++) d_mCM[d] = new NMatrixInt( numMixCells[d], numDualCells[d] );
 long int delta[4]; 
 delta[0] = numNonIdealCells[0] +     numNonIdealCells[1] +   numNonIdealCells[2];
 delta[1] = 2*numNonIdealCells[1] + 3*numNonIdealCells[2];
 delta[2] = 3*numNonIdealCells[2];
 delta[3] = 0;

 // various useful pointers, index holders.
 const NVertex* vrt(NULL);  const NEdge* edg(NULL);  const NFace* fac(NULL); 
   const NTetrahedron* tet(NULL); 
 unsigned long J;

 for (unsigned long j=0; j<numNonIdealCells[3]; j++)
  {
    tet = tri->getTetrahedron(j);
    // d_mCM[0]
    d_mCM[0]->entry( delta[0] + j, j ) += 1;

    // d_mCM[1]
    for (unsigned long i=0; i<4; i++)
	{
	 fac = tet->getFace(i); if (!fac->isBoundary())
	  {
	   J = lower_bound( dcIx[1].begin(), dcIx[1].end(), tri->faceIndex( fac ) ) - dcIx[1].begin();
	   d_mCM[1]->entry( delta[1] + 4*j + i, J ) += 1; 
	  }
	}

    // d_mCM[2]
    for (unsigned long i=0; i<6; i++)
	{
	 edg = tet->getEdge(i); if (!edg->isBoundary())
	  {
	   J = lower_bound( dcIx[2].begin(), dcIx[2].end(), tri->edgeIndex( edg ) ) - dcIx[2].begin();
	   d_mCM[2]->entry( delta[2] + 6*j + i, J ) += 1; 
	  }
	}

    // d_mCM[3]
    for (unsigned long i=0; i<4; i++)
	{
	 vrt = tet->getVertex(i); if ( (!vrt->isBoundary()) && (!vrt->isIdeal()) )
	  {
	   J = lower_bound( dcIx[3].begin(), dcIx[3].end(), tri->vertexIndex( vrt ) ) - dcIx[3].begin();
	   d_mCM[3]->entry( delta[3] + 4*j + i, J ) += tet->getVertexMapping(i).sign(); 
	  }
	}
  }
}

void fillBoundaryToStandardHomCM( unsigned aDim, // dimension of the triangulation
        const unsigned long numStandardCells[5],         const unsigned long numStandardBdryCells[4], 
        const unsigned long numNonIdealBdryCells[4],     const unsigned long numIdealCells[4],
	const unsigned long numNonIdealCells[5],         std::vector< std::vector<unsigned long> > &nicIx, 	 
	std::vector< std::vector<unsigned long> > &bcIx, std::vector< NMatrixInt* > &bs_sCM)
{
 for (unsigned d=0; d<aDim; d++) bs_sCM[d] = new NMatrixInt( numStandardCells[d], numStandardBdryCells[d] );
 unsigned long I;
 for (unsigned long d=0; d<aDim; d++) 
  {// standard part of boundary
   for (unsigned j=0; j<numNonIdealBdryCells[d]; j++)
    {
     I = lower_bound( nicIx[d].begin(), nicIx[d].end(), bcIx[d][j] ) - nicIx[d].begin();
     bs_sCM[d]->entry( I, j ) = 1;
    }
   // ideal part of boundary
   for (unsigned j=0; j<numIdealCells[d]; j++)
     bs_sCM[d]->entry( numNonIdealCells[d] + j, numNonIdealBdryCells[d] + j ) = 1;
  }
}

void fillStandardToRelativeHomCM( unsigned aDim, // dimension of the triangulation
        const unsigned long numStandardCells[5],                 const unsigned long numRelativeCells[5], 
	const unsigned long numNonIdealCells[5],
	std::vector< std::vector<unsigned long> > &nicIx, 	 std::vector< std::vector<unsigned long> > &srCC, 
        std::vector< NMatrixInt* > &s_rCM)
{
 for (unsigned d=0; d<aDim+1; d++) s_rCM[d] = new NMatrixInt( numRelativeCells[d], numStandardCells[d] );

 unsigned long I;
 for (unsigned long d=0; d<aDim+1; d++) for (unsigned j=0; j<numNonIdealCells[d]; j++)
  {
   I = lower_bound( srCC[d].begin(), srCC[d].end(), nicIx[d][j] ) - srCC[d].begin();
   if ( I != srCC[d].size() ) s_rCM[d]->entry( I, j ) = 1;
  }
}

// H_{d+1}(M, \partial M) --> H_d(\partial M)
void fillDifferentialHomCM( const Dim4Triangulation* tri,  const unsigned long numRelativeCells[5], 
     const unsigned long numStandardBdryCells[4],      const unsigned long numNonIdealBdryCells[4],    
     std::vector< std::vector<unsigned long> > &bcIx,  std::vector< std::vector<unsigned long> > &icIx,
     std::vector< std::vector<unsigned long> > &srCC,   std::vector< NMatrixInt* > &rbCM)               
{
 for (unsigned d=0; d<4; d++) 
   rbCM[d] = new NMatrixInt( numStandardBdryCells[d], numRelativeCells[d+1] );
 unsigned long I;
 // various useful pointers, index holders.
 const Dim4Edge* edg(NULL);  const Dim4Face* fac(NULL); const Dim4Tetrahedron* tet(NULL); const Dim4Pentachoron* pen(NULL);
 // boundary relative 1-cells
 unsigned long D=1;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {	
   edg = tri->getEdge(srCC[D][j]);
   for (unsigned long i=0; i<D+1; i++) if (edg->getVertex(i)->isIdeal())
    {   // endpt i is ideal, find index
     I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
     rbCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
    } 
   else if (edg->getVertex(i)->isBoundary())
    {
     I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->vertexIndex( edg->getVertex(i) ) )
      - bcIx[D-1].begin();
     rbCM[D-1]->entry(I, j) += ( i == 0 ? -1 : 1 );
    }
  }

 // boundary relative 2-cells
 D = 2;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {
   fac = tri->getFace(srCC[D][j]);
   for (unsigned long i=0; i < D+1; i++) 
    { 
     if (fac->getVertex(i)->isIdeal())
      { // ideal ends of faces	
       I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
       rbCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
      } // standard face boundaries
     if (fac->getEdge(i)->isBoundary())
      {
       NPerm5 P( fac->getEdgeMapping(i) );
       I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->edgeIndex( fac->getEdge(i) )) 
	- bcIx[D-1].begin();
       rbCM[D-1]->entry(I, j) += P.sign();
      }
    }
 }

 // boundary relative 3-cells
 D = 3;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {
   tet = tri->getTetrahedron(srCC[D][j]);
   for (unsigned long i=0; i < D+1; i++) 
    { 
     if (tet->getVertex(i)->isIdeal())
      { // ideal ends of faces	
       I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
       rbCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
      } // standard face boundaries
     if (tet->getFace(i)->isBoundary())
      {
       NPerm5 P( tet->getFaceMapping(i) );
       I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->faceIndex( tet->getFace(i) )) 
	- bcIx[D-1].begin();
       rbCM[D]->entry(I, j) += P.sign();
      }
    }
  }

 // boundary relative 4-cells
 D = 4;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {
   pen = tri->getPentachoron(srCC[D][j]);
   for (unsigned long i=0; i < D+1; i++) 
    { 
     if (pen->getVertex(i)->isIdeal())
      { // ideal ends of faces	
       I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
       rbCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
      } // standard face boundaries
     if (pen->getTetrahedron(i)->isBoundary())
      {
       NPerm5 P( pen->getTetrahedronMapping(i) );
       I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->tetrahedronIndex( pen->getTetrahedron(i) )) 
		- bcIx[D-1].begin();
       rbCM[D-1]->entry(I, j) += P.sign();
      }
    }
  }
}

void fillDifferentialHomCM( const NTriangulation* tri,     const unsigned long numRelativeCells[5], 
     const unsigned long numStandardBdryCells[4],  const unsigned long numNonIdealBdryCells[4],    
     std::vector< std::vector<unsigned long> > &bcIx,  std::vector< std::vector<unsigned long> > &icIx,
     std::vector< std::vector<unsigned long> > &srCC,   std::vector< NMatrixInt* > &rbCM)               
{
 for (unsigned d=0; d<3; d++) rbCM[d] = new NMatrixInt( numStandardBdryCells[d], numRelativeCells[d+1] );

 unsigned long I;
 // various useful pointers, index holders.
 const NEdge* edg(NULL);  const NFace* fac(NULL); const NTetrahedron* tet(NULL); 

 // boundary relative 1-cells
 unsigned long D=1;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {	
   edg = tri->getEdge(srCC[D][j]);
   for (unsigned long i=0; i<D+1; i++) if (edg->getVertex(i)->isIdeal())
    {   // endpt i is ideal, find index
     I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
     rbCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
    } 
   else if (edg->getVertex(i)->isBoundary())
    {
     I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->vertexIndex( edg->getVertex(i) ) )
      - bcIx[D-1].begin();
     rbCM[D-1]->entry(I, j) += ( i == 0 ? -1 : 1 );
    }
  }

 // boundary relative 2-cells
 D = 2;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {
   fac = tri->getFace(srCC[D][j]);
   for (unsigned long i=0; i < D+1; i++) 
    { 
     if (fac->getVertex(i)->isIdeal())
      { // ideal ends of faces	
       I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
       rbCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
      } // standard face boundaries
     if (fac->getEdge(i)->isBoundary())
      {
       NPerm4 P( fac->getEdgeMapping(i) );
       I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->edgeIndex( fac->getEdge(i) )) 
	- bcIx[D-1].begin();
       rbCM[D-1]->entry(I, j) += P.sign();
      }
    }
 }

 // boundary relative 3-cells
 D = 3;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {
   tet = tri->getTetrahedron(srCC[D][j]);
   for (unsigned long i=0; i < D+1; i++) 
    { 
     if (tet->getVertex(i)->isIdeal())
      { // ideal ends of faces	
       I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
       rbCM[D-1]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
      } // standard face boundaries
     if ( tet->getFace(i)->isBoundary() )
      {
       NPerm4 P( tet->getFaceMapping(i) );
       I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->faceIndex( tet->getFace(i) )) 
	- bcIx[D-1].begin();
       rbCM[D]->entry(I, j) += P.sign();
      }
    }
  }
}

// CM is the (appropriate size) identity matrix and wants to be the chain map inducing Poincare duality
// so we need to correct the signs down the diagonal -- we assume CM is an identity matrix to begin with.
void correctRelOrMat( NMatrixInt &CM, unsigned long domdim, const NTriangulation* tri3, const Dim4Triangulation* tri4, 
 const std::vector< std::vector<unsigned long> > &dcIx )
{
// CM is from dual to std_rel_bdry coord
unsigned long aDim = ( tri3 ? 3 : 4 );
//   if ( (h_desc.domain.var == coVariant) && (h_desc.domain.hcs == DUAL_coord) &&
//        (h_desc.range.hcs == STD_REL_BDRY_coord) && (h_desc.domain.dim + h_desc.range.dim == aDim) )
if ( aDim == 3 )
 { // domdim \in \{ 0, 1, 2, 3 \}
  if (domdim == 0) for (unsigned long i=0; i<CM.rows(); i++)
	{ // srCC[aDim][i], dcIx[0][i]
         const NTetrahedron* tet( tri3->getTetrahedron( dcIx[domdim][i] ) );
	 CM.entry( i, i ) = tet->orientation();
	}
  else if (domdim == 1) for (unsigned long i=0; i<CM.rows(); i++)
	{
	 const NFace* fac( tri3->getFace( dcIx[domdim][i] ) );
         const NTetrahedron* tet( fac->getEmbedding(0).getTetrahedron() );
         NPerm4 emb( fac->getEmbedding(0).getVertices() );
	 CM.entry( i, i ) = emb.sign()*tet->orientation();  
	}   
  else if (domdim == 2) for (unsigned long i=0; i<CM.rows(); i++)
	{	 
	 const NEdge* edg( tri3->getEdge( dcIx[domdim][i] ) );
         const NTetrahedron* tet( edg->getEmbedding(0).getTetrahedron() );
         NPerm4 emb( edg->getEmbedding(0).getVertices() );
 	 CM.entry( i, i ) = emb.sign()*tet->orientation();  
       }
  else if (domdim == 3) for (unsigned long i=0; i<CM.rows(); i++)
	{	 
	 const NVertex* vrt( tri3->getVertex( dcIx[domdim][i] ) );
         const NTetrahedron* tet( vrt->getEmbedding(0).getTetrahedron() );
         NPerm4 emb( vrt->getEmbedding(0).getVertices() );
	 CM.entry( i, i ) = emb.sign()*tet->orientation();  
        }
 }
else
 { // aDim == 4,  domdim \in \{ 0, 1, 2, 3, 4 \}
  if (domdim == 0) for (unsigned long i=0; i<CM.rows(); i++)
	{ // srCC[aDim][i], dcIx[0][i]
         const Dim4Pentachoron* pen( tri4->getPentachoron( dcIx[domdim][i] ) );
	 CM.entry( i, i ) = pen->orientation();
	}
  else if (domdim == 1) for (unsigned long i=0; i<CM.rows(); i++)
	{
	 const Dim4Tetrahedron* tet( tri4->getTetrahedron( dcIx[domdim][i] ) );
         const Dim4Pentachoron* pen( tet->getEmbedding(0).getPentachoron() );
         NPerm5 emb( tet->getEmbedding(0).getVertices() );
	 CM.entry( i, i ) = emb.sign()*pen->orientation();  
	}   
  else if (domdim == 2) for (unsigned long i=0; i<CM.rows(); i++)
	{	 
	 const Dim4Face* fac( tri4->getFace( dcIx[domdim][i] ) );
         const Dim4Pentachoron* pen( fac->getEmbedding(0).getPentachoron() );
         NPerm5 emb( fac->getEmbedding(0).getVertices() );
 	 CM.entry( i, i ) = emb.sign()*pen->orientation();  
       }
  else if (domdim == 3) for (unsigned long i=0; i<CM.rows(); i++)
	{	 
	 const Dim4Edge* edg( tri4->getEdge( dcIx[domdim][i] ) );
         const Dim4Pentachoron* pen( edg->getEmbedding(0).getPentachoron() );
         NPerm5 emb( edg->getEmbedding(0).getVertices() );
	 CM.entry( i, i ) = emb.sign()*pen->orientation();  
        }
  else if (domdim == 4) for (unsigned long i=0; i<CM.rows(); i++)
	{
	 const Dim4Vertex* vrt( tri4->getVertex( dcIx[domdim][i] ) );
         const Dim4Pentachoron* pen( vrt->getEmbedding(0).getPentachoron() );
         NPerm5 emb( vrt->getEmbedding(0).getVertices() );
	 CM.entry( i, i ) = emb.sign()*pen->orientation();  
        }
 }
}
// end of chain map constructions

void fillChainMaps( NTriangulation* tri3, Dim4Triangulation* tri4, 
                    unsigned long numStandardCells[5],    unsigned long numDualCells[5],     unsigned long numMixCells[5], 
                    unsigned long numStandardBdryCells[4],unsigned long numNonIdealCells[5], unsigned long numIdealCells[4], 
                    unsigned long numNonIdealBdryCells[4],unsigned long numRelativeCells[5], unsigned long numDualRelCells[5], 
                    unsigned long numMixRelCells[5],      unsigned long numMixBdryCells[4],  unsigned long numDualBdryCells[4], 
                    std::vector< std::vector<unsigned long> > &nicIx, std::vector< std::vector<unsigned long> > &icIx, 
                    std::vector< std::vector<unsigned long> > &dcIx,  std::vector< std::vector<unsigned long> > &bcIx, 
                    std::vector< std::vector<unsigned long> > &srCC, 
                    std::vector< NMatrixInt* > &bs_sCM, std::vector< NMatrixInt* > &s_mCM, 
                    std::vector< NMatrixInt* > &d_mCM,  std::vector< NMatrixInt* > &s_rCM, 
                    std::vector< NMatrixInt* > &rbCM )
{
 if (tri4)
  {
   fillStandardToMixedHomCM( 4, numStandardCells, numMixCells, numIdealCells, numNonIdealCells, s_mCM );

   fillDualToMixedHomCM( tri4, numDualCells, numMixCells, numNonIdealCells, dcIx, d_mCM );

   fillStandardToRelativeHomCM( 4, numStandardCells, numRelativeCells, numNonIdealCells, nicIx, srCC, s_rCM );

   fillBoundaryToStandardHomCM( 4, numStandardCells, numStandardBdryCells, numNonIdealBdryCells, 
        numIdealCells, numNonIdealCells, nicIx, bcIx, bs_sCM);

   fillDifferentialHomCM( tri4, numRelativeCells, numStandardBdryCells,  numNonIdealBdryCells,    
     bcIx,  icIx, srCC, rbCM );   
  }
 if (tri3)
  {
   fillStandardToMixedHomCM( 3, numStandardCells, numMixCells, numIdealCells, numNonIdealCells, s_mCM );

   fillDualToMixedHomCM( tri3, numDualCells, numMixCells, numNonIdealCells, dcIx, d_mCM );

   fillStandardToRelativeHomCM( 3, numStandardCells, numRelativeCells, numNonIdealCells, nicIx, srCC, s_rCM );

   fillBoundaryToStandardHomCM( 3, numStandardCells, numStandardBdryCells, numNonIdealBdryCells, 
        numIdealCells, numNonIdealCells, nicIx, bcIx, bs_sCM);
 
   fillDifferentialHomCM( tri3, numRelativeCells, numStandardBdryCells,  numNonIdealBdryCells,    
     bcIx,  icIx, srCC, rbCM );   
  }
}



} // namespace regina


