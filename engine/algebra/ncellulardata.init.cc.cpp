
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

// only used in the NCellularData constructor
void setupIndices(const Dim4Triangulation* tri,   
 std::vector< std::vector<unsigned long> > &nicIx,  std::vector< std::vector<unsigned long> > &icIx, 
 std::vector< std::vector<unsigned long> > &dcIx,   std::vector< std::vector<unsigned long> > &bcIx,
 std::vector< std::vector<unsigned long> > &rIx,  
 unsigned long numStandardCells[5],    unsigned long numDualCells[5], 
 unsigned long numMixCells[5],         unsigned long numStandardBdryCells[4], 
 unsigned long numNonIdealCells[5],    unsigned long numIdealCells[4], 
 unsigned long numNonIdealBdryCells[4],unsigned long numRelativeCells[5], 
 unsigned long numDualRelCells[5],     unsigned long numMixRelCells[5],   
 unsigned long numMixBdryCells[4],     unsigned long numDualBdryCells[4])
{
    // nicIx[0]  interior or boundary,  bcIx[0] boundary,  dcIx[4] interior vertices. 
    for (Dim4Triangulation::VertexIterator vit = tri->getVertices().begin();
            vit != tri->getVertices().end(); vit++) if ( !(*vit)->isIdeal() ) {
        nicIx[0].push_back( tri->vertexIndex(*vit) );
        if ((*vit)->isBoundary()) bcIx[0].push_back( tri->vertexIndex(*vit) ); else
	 { dcIx[4].push_back( tri->vertexIndex(*vit) );
           rIx[0].push_back( tri->vertexIndex(*vit) ); }
    } 
    // nicIx[1] all,  icIx[0] ideal ends, dcIx[3] nonboundary, bcIx[1] boundary
    for (Dim4Triangulation::EdgeIterator eit = tri->getEdges().begin();
            eit != tri->getEdges().end(); eit++) {
        nicIx[1].push_back( tri->edgeIndex(*eit) );
        if ((*eit)->isBoundary()) bcIx[1].push_back( tri->edgeIndex(*eit) );
	 else 
	  { dcIx[3].push_back( tri->edgeIndex(*eit) );
            rIx[1].push_back( tri->edgeIndex(*eit) );
            for (unsigned i=0;i<2;i++) if ((*eit)->getVertex(i)->isIdeal()) 
             icIx[0].push_back(2*tri->edgeIndex(*eit)+i); }
    }
    // nicIx[2] all, icIx[1] ideal ends, dcIx[2] nonboundary, bcIx[2] boundary
    for (Dim4Triangulation::FaceIterator fit = tri->getFaces().begin();
            fit != tri->getFaces().end(); fit++) {
	nicIx[2].push_back( tri->faceIndex(*fit) );
        if ((*fit)->isBoundary()) { bcIx[2].push_back( tri->faceIndex(*fit) ); }
	 else
	  { dcIx[2].push_back( tri->faceIndex(*fit) );
            rIx[2].push_back( tri->faceIndex(*fit) );
            for (unsigned i=0;i<3;i++) if ((*fit)->getVertex(i)->isIdeal()) 
             icIx[1].push_back(3*tri->faceIndex(*fit)+i); }
    }
   for (Dim4Triangulation::TetrahedronIterator tit = tri->getTetrahedra().begin();
            tit != tri->getTetrahedra().end(); tit++) {
        nicIx[3].push_back( tri->tetrahedronIndex(*tit) );
        if ((*tit)->isBoundary()) bcIx[3].push_back( tri->tetrahedronIndex(*tit) );
	 else
 	  { dcIx[1].push_back( tri->tetrahedronIndex(*tit) );
            rIx[3].push_back( tri->tetrahedronIndex(*tit) );
            for (unsigned i=0;i<4;i++) if ((*tit)->getVertex(i)->isIdeal()) 
             icIx[2].push_back(4*tri->tetrahedronIndex(*tit)+i); }
    }
    for (Dim4Triangulation::PentachoronIterator pit = tri->getPentachora().begin();
            pit != tri->getPentachora().end(); pit++) {
	nicIx[4].push_back( tri->pentachoronIndex(*pit) );
	dcIx[0].push_back( tri->pentachoronIndex(*pit) );
        rIx[4].push_back( tri->pentachoronIndex(*pit) );
        for (unsigned i=0;i<5;i++) if ((*pit)->getVertex(i)->isIdeal()) 
            icIx[3].push_back(5*tri->pentachoronIndex(*pit)+i);
    }

    // standard CW-decomposition (0..4)-cells / triangulation + ideal cells.
    for (unsigned i=0; i<4; i++) numStandardCells[i] = nicIx[i].size() + icIx[i].size();
    numStandardCells[4] = nicIx[4].size();
    // dual (0..4)-cells : a dual k-cell for every n-k-cell in triangulation
    for (unsigned i=0; i<5; i++) numDualCells[i] = dcIx[i].size();
    // relative (0..4)-cells : non-boundary cells from triangulation
    for (unsigned i=0; i<5; i++) numRelativeCells[i] = rIx[i].size();
    // boundary (0..3)-cells: boundary triangulation + ideal cells
    for (unsigned i=0; i<4; i++) numStandardBdryCells[i] = bcIx[i].size() + icIx[i].size();
    // ideal and non-ideal cells: cells from std cw-decomposition that aren't ideal
    for (unsigned i=0; i<5; i++) numNonIdealCells[i] = nicIx[i].size();
    // cells from std cw-decomposition that are ideal
    for (unsigned i=0; i<4; i++) numIdealCells[i] = icIx[i].size();
    // boundary cells from std cw-decomposition that are not ideal
    for (unsigned i=0; i<4; i++) numNonIdealBdryCells[i] = bcIx[i].size();

    // this mixed decomposition is the proper cell decomposition induced by the barycentric
    // subdivision, so all previous internal/boundary standard cells contribute barycentres
    numMixCells[0] = numNonIdealCells[0] + numNonIdealCells[1] + numNonIdealCells[2] + numNonIdealCells[3] +
	             numNonIdealCells[4] + numIdealCells[0]; 
    numMixCells[1] = 2*numNonIdealCells[1] + 3*numNonIdealCells[2] + 4*numNonIdealCells[3] + 
		     5*numNonIdealCells[4] + numIdealCells[1];
    numMixCells[2] = 3*numNonIdealCells[2] + 6*numNonIdealCells[3] + 10*numNonIdealCells[4] +
		     numIdealCells[2];
    numMixCells[3] = 4*numNonIdealCells[3] + 10*numNonIdealCells[4] + numIdealCells[3];
    numMixCells[4] = 5*numNonIdealCells[4];

    // number of relative dual cells, these are dual to the standard CW-decomposition
    for (unsigned i=0; i<5; i++) numDualRelCells[i] = numStandardCells[4-i];

    // number of mixed relative cells -- each non-boundary (relative) cell gets multiplied appropriately..
    numMixRelCells[0] = numRelativeCells[0] + numRelativeCells[1] + numRelativeCells[2] + numRelativeCells[3] +
	                numRelativeCells[4]; 
    numMixRelCells[1] = 2*numRelativeCells[1] + 3*numRelativeCells[2] + 4*numRelativeCells[3] + 
		        5*numRelativeCells[4];
    numMixRelCells[2] = 3*numRelativeCells[2] + 6*numRelativeCells[3] + 10*numRelativeCells[4];
    numMixRelCells[3] = 4*numRelativeCells[3] + 10*numRelativeCells[4];
    numMixRelCells[4] = 5*numRelativeCells[4];

    // number of mixed boundary cells
    numMixBdryCells[0] = numStandardBdryCells[0] + numStandardBdryCells[1] + numStandardBdryCells[2] + numStandardBdryCells[3];
    numMixBdryCells[1] = 2*numStandardBdryCells[1] + 3*numStandardBdryCells[2] + 4*numStandardBdryCells[3];
    numMixBdryCells[2] = 3*numStandardBdryCells[2] + 4*numStandardBdryCells[3];
    numMixBdryCells[3] = 4*numStandardBdryCells[3];

    // number of boundary dual cells -- dual to std boundary cells
    for (unsigned i=0; i<4; i++) numDualBdryCells[i] = numStandardBdryCells[3-i];
}  


// only used in the NCellularData constructor
void setupIndices(const NTriangulation* tri,   
 std::vector< std::vector<unsigned long> > &nicIx,  std::vector< std::vector<unsigned long> > &icIx, 
 std::vector< std::vector<unsigned long> > &dcIx,   std::vector< std::vector<unsigned long> > &bcIx, 
 std::vector< std::vector<unsigned long> > &rIx, 
 unsigned long numStandardCells[5],    unsigned long numDualCells[5], 
 unsigned long numMixCells[5],         unsigned long numStandardBdryCells[4], 
 unsigned long numNonIdealCells[5],    unsigned long numIdealCells[4], 
 unsigned long numNonIdealBdryCells[4],unsigned long numRelativeCells[5], 
 unsigned long numDualRelCells[5],     unsigned long numMixRelCells[5],  
 unsigned long numMixBdryCells[4],     unsigned long numDualBdryCells[4])
{
    // nicIx[0]  interior or boundary,  bcIx[0] boundary,  dcIx[3] interior vertices. 
    for (NTriangulation::VertexIterator vit = tri->getVertices().begin();
            vit != tri->getVertices().end(); vit++) if ( !(*vit)->isIdeal() ) {
        nicIx[0].push_back( tri->vertexIndex(*vit) );
        if ((*vit)->isBoundary()) bcIx[0].push_back( tri->vertexIndex(*vit) ); else
	 { dcIx[3].push_back( tri->vertexIndex(*vit) );
            rIx[0].push_back( tri->vertexIndex(*vit) ); }
    } 
    // nicIx[1] all,  icIx[0] ideal ends, dcIx[2] nonboundary, bcIx[1] boundary
    for (NTriangulation::EdgeIterator eit = tri->getEdges().begin();
            eit != tri->getEdges().end(); eit++) {
        nicIx[1].push_back( tri->edgeIndex(*eit) );
        if ((*eit)->isBoundary()) bcIx[1].push_back( tri->edgeIndex(*eit) );
	 else 
	  { dcIx[2].push_back( tri->edgeIndex(*eit) );
	     rIx[1].push_back( tri->edgeIndex(*eit) );
            for (unsigned i=0;i<2;i++) if ((*eit)->getVertex(i)->isIdeal()) 
             icIx[0].push_back(2*tri->edgeIndex(*eit)+i); }
    }
    // nicIx[2] all, icIx[1] ideal ends, dcIx[1] nonboundary, bcIx[2] boundary
    for (NTriangulation::FaceIterator fit = tri->getFaces().begin();
            fit != tri->getFaces().end(); fit++) {
	nicIx[2].push_back( tri->faceIndex(*fit) );
        if ((*fit)->isBoundary()) bcIx[2].push_back( tri->faceIndex(*fit) );
	 else
	  { dcIx[1].push_back( tri->faceIndex(*fit) );
             rIx[2].push_back( tri->faceIndex(*fit) );
            for (unsigned i=0;i<3;i++) if ((*fit)->getVertex(i)->isIdeal()) 
             icIx[1].push_back(3*tri->faceIndex(*fit)+i); }
    }
   // nicIx[3], icIx[2] ideal ends, dcIx[0] all
   for (NTriangulation::TetrahedronIterator tit = tri->getTetrahedra().begin();
            tit != tri->getTetrahedra().end(); tit++) {
        nicIx[3].push_back( tri->tetrahedronIndex(*tit) );
        dcIx[0].push_back( tri->tetrahedronIndex(*tit) );
         rIx[3].push_back( tri->tetrahedronIndex(*tit) );
        for (unsigned i=0;i<4;i++) if ((*tit)->getVertex(i)->isIdeal()) 
             icIx[2].push_back(4*tri->tetrahedronIndex(*tit)+i); 
    }

    // standard (0..3)-cells:
    for (unsigned i=0; i<3; i++) numStandardCells[i] = nicIx[i].size() + icIx[i].size();
    numStandardCells[3] = nicIx[3].size();
    numStandardCells[4] = 0;
    // dual (0..3)-cells:
    for (unsigned i=0; i<4; i++) numDualCells[i] = dcIx[i].size();
    for (unsigned i=0; i<4; i++) numRelativeCells[i] = rIx[i].size();
    numDualCells[4] = 0; numRelativeCells[4] = 0;
    // boundary (0..3)-cells:
    for (unsigned i=0; i<3; i++) numStandardBdryCells[i] = bcIx[i].size() + icIx[i].size();
    numStandardBdryCells[3] = 0;
    // ideal and non-ideal cells:
    for (unsigned i=0; i<4; i++) numNonIdealCells[i] = nicIx[i].size();
    numNonIdealCells[4] = 0;
    for (unsigned i=0; i<3; i++) numIdealCells[i] = icIx[i].size();
    numIdealCells[3] = 0;
    for (unsigned long i=0; i<3; i++) numNonIdealBdryCells[i] = bcIx[i].size();
    numNonIdealBdryCells[3] = 0;

    // this mixed decomposition is the proper cell decomposition induced by the barycentric
    // subdivision, so all previous internal/boundary standard cells contribute barycentres
    numMixCells[0] = numNonIdealCells[0] + numNonIdealCells[1] + numNonIdealCells[2] + numNonIdealCells[3] +
		     numIdealCells[0]; 
    numMixCells[1] = 2*numNonIdealCells[1] + 3*numNonIdealCells[2] + 4*numNonIdealCells[3] + 
		     numIdealCells[1];
    numMixCells[2] = 3*numNonIdealCells[2] + 6*numNonIdealCells[3] + numIdealCells[2];
    numMixCells[3] = 4*numNonIdealCells[3];
    numMixCells[4] = 0;

    // number of relative dual cells, these are dual to the standard CW-decomposition
    for (unsigned i=0; i<4; i++) numDualRelCells[i] = numStandardCells[3-i];

    // number of mixed relative cells -- each non-boundary (relative) cell gets multiplied appropriately..
    numMixRelCells[0] = numRelativeCells[0] + numRelativeCells[1] + numRelativeCells[2] + numRelativeCells[3]; 
    numMixRelCells[1] = 2*numRelativeCells[1] + 3*numRelativeCells[2] + 4*numRelativeCells[3];
    numMixRelCells[2] = 3*numRelativeCells[2] + 6*numRelativeCells[3];
    numMixRelCells[3] = 4*numRelativeCells[3];

    // number of mixed boundary cells
    numMixBdryCells[0] = numStandardBdryCells[0] + numStandardBdryCells[1] + numStandardBdryCells[2];
    numMixBdryCells[1] = 2*numStandardBdryCells[1] + 3*numStandardBdryCells[2];
    numMixBdryCells[2] = 3*numStandardBdryCells[2];

    // number of boundary dual cells -- dual to std boundary cells
    for (unsigned i=0; i<3; i++) numDualBdryCells[i] = numStandardBdryCells[2-i];
}  

void fillStandardHomologyCC(const Dim4Triangulation* tri, 
	const unsigned long numStandardCells[5],  const unsigned long numNonIdealCells[5], 
	const unsigned long numIdealCells[5], 	  
	const std::vector< std::vector< unsigned long > > &nicIx, const std::vector< std::vector< unsigned long > > &icIx, 
	std::vector< NMatrixInt* > &sCC)
{
    // initialize chain complex matrices.
    for (unsigned i=1; i<5; i++) // sCC[i] defined for i == 0, ..., 5
	sCC[i] = new NMatrixInt(numStandardCells[i-1], numStandardCells[i]);
    sCC[0] = new NMatrixInt(1, numStandardCells[0]);
    sCC[5] = new NMatrixInt(numStandardCells[4], 1);

    // various useful pointers, index holders.
    const Dim4Edge* edg(NULL);  const Dim4Face* fac(NULL); const Dim4Tetrahedron* tet(NULL); const Dim4Pentachoron* pen(NULL);
    unsigned long I;
    
    // now we fill them out, first sCC.  sCC[0] is zero, 
    unsigned long D=1; // sCC[D]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{ // endpts getEdge(nicIx[D][j]) ideal?
	edg = tri->getEdge(nicIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->getVertex(i)->isIdeal())
	 {   // endpt i is ideal, find index
          I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	  sCC[D]->entry(numNonIdealCells[D-1] + I, j) += 1;
	 } 
	else // endpt i is not ideal
	 {
          I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->vertexIndex( edg->getVertex(i) ) )
	      - nicIx[D-1].begin();
	  sCC[D]->entry(I, j) += ( i == 0 ? -1 : 1 );
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // scc[D]->entry( *, numNonIdealCells[D] + j )
        { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
	fac = tri->getFace(icIx[D][j]/(D+2));
	for (unsigned long i=1; i<D+2; i++)
	 {
          NPerm5 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  unsigned long iX( (D+1)*tri->edgeIndex( fac->getEdge( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  sCC[D]->entry(numNonIdealCells[D-1] + I,  numNonIdealCells[D] + j) -= P.sign();
	 }
	}

    D = 2; // sCC[2]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{
	fac = tri->getFace(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (fac->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	    sCC[D]->entry(numNonIdealCells[D-1] + I, j) += 1;
	   } // standard face boundaries
	  NPerm5 P( fac->getEdgeMapping(i) );
          I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->edgeIndex( fac->getEdge(i) )) 
		- nicIx[D-1].begin();
	  sCC[D]->entry(I, j) += P.sign();
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) // scc[D]->entry( *, j+numNonIdealCells[D-1] )
        { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
	tet = tri->getTetrahedron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm5 P( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	  unsigned long iX( (D+1)*tri->faceIndex( tet->getFace( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  sCC[D]->entry(numNonIdealCells[D-1] + I,  numNonIdealCells[D] + j) -= P.sign(); 
	 }
	}

    D = 3; // sCC[3]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{
	tet = tri->getTetrahedron(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (tet->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	    sCC[D]->entry(numNonIdealCells[D-1] + I, j) += 1;
	   } // standard face boundaries
	  NPerm5 P( tet->getFaceMapping(i) );
          I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->faceIndex( tet->getFace(i) )) 
		- nicIx[D-1].begin();
	  sCC[D]->entry(I, j) += P.sign();
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // scc[D]->entry( *, j+numNonIdealCells[D-1] )
        { // icIx[D][j]/(D+2) pentachoron icIx[1][j] % (D+2) vertex
	pen = tri->getPentachoron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm5 P( pen->getTetrahedronMapping( (icIx[D][j] + i) % (D+2)) );
	  unsigned long iX( (D+1)*tri->tetrahedronIndex( pen->getTetrahedron( (icIx[D][j] + i) % (D+2) ) ) 
			            + ( P.preImageOf(icIx[D][j] % (D+2)) )     );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  sCC[D]->entry(numNonIdealCells[D-1] + I,  numNonIdealCells[D] + j) -= P.sign(); 
	 }
	}
    D = 4; // sCC[4]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{
	pen = tri->getPentachoron(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (pen->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	    sCC[D]->entry(numNonIdealCells[D-1] + I, j) += 1;
	   } // standard face boundaries
	  NPerm5 P( pen->getTetrahedronMapping(i) );
          I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->tetrahedronIndex( pen->getTetrahedron(i) )) 
		- nicIx[D-1].begin();
	  sCC[D]->entry(I, j) += P.sign();
	 }
	}
}

void fillStandardHomologyCC(const NTriangulation* tri, 
	const unsigned long numStandardCells[5],  const unsigned long numNonIdealCells[5], 
	const unsigned long numIdealCells[5], 	  
	const std::vector< std::vector< unsigned long > > &nicIx, const std::vector< std::vector< unsigned long > > &icIx, 
	std::vector< NMatrixInt* > &sCC)
{
    // initialize chain complex matrices.
    for (unsigned i=1; i<4; i++) // sCC[i] defined for i == 0, ..., 5
	sCC[i] = new NMatrixInt(numStandardCells[i-1], numStandardCells[i]);
    sCC[0] = new NMatrixInt(1, numStandardCells[0]);
    sCC[4] = new NMatrixInt(numStandardCells[3], 1);

    // various useful pointers, index holders.
    const NEdge* edg(NULL);  const NFace* fac(NULL); const NTetrahedron* tet(NULL); 
    unsigned long I;
    
    // now we fill them out, first sCC.  sCC[0] is zero, 
    unsigned long D=1; // sCC[D]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{ // endpts getEdge(nicIx[D][j]) ideal?
	edg = tri->getEdge(nicIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->getVertex(i)->isIdeal())
	 {   // endpt i is ideal, find index
          I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	  sCC[D]->entry(numNonIdealCells[D-1] + I, j) += 1;
	 } 
	else // endpt i is not ideal
	 {
          I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->vertexIndex( edg->getVertex(i) ) )
	      - nicIx[D-1].begin();
	  sCC[D]->entry(I, j) += ( i == 0 ? -1 : 1 );
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // scc[D]->entry( *, numNonIdealCells[D] + j )
        { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
	fac = tri->getFace(icIx[D][j]/(D+2));
	for (unsigned long i=1; i<D+2; i++)
	 {
          NPerm4 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  unsigned long iX( (D+1)*tri->edgeIndex( fac->getEdge( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  sCC[D]->entry(numNonIdealCells[D-1] + I,  numNonIdealCells[D] + j) -= P.sign();
	 }
	}

    D = 2; // sCC[2]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{
	fac = tri->getFace(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (fac->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	    sCC[D]->entry(numNonIdealCells[D-1] + I, j) += 1;
	   } // standard face boundaries
	  NPerm4 P( fac->getEdgeMapping(i) );
          I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->edgeIndex( fac->getEdge(i) )) 
		- nicIx[D-1].begin();
	  sCC[D]->entry(I, j) += P.sign();
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) // scc[D]->entry( *, j+numNonIdealCells[D-1] )
        { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
	tet = tri->getTetrahedron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm4 P( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	  unsigned long iX( (D+1)*tri->faceIndex( tet->getFace( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  sCC[D]->entry(numNonIdealCells[D-1] + I,  numNonIdealCells[D] + j) -= P.sign(); 
	 }
	}

    D = 3; // sCC[3]
    for (unsigned long j=0; j<numNonIdealCells[D]; j++) // scc[D]->entry( *,j )
	{
	tet = tri->getTetrahedron(nicIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (tet->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	    sCC[D]->entry(numNonIdealCells[D-1] + I, j) += 1;
	   } // standard face boundaries
	  NPerm4 P( tet->getFaceMapping(i) );
          I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->faceIndex( tet->getFace(i) )) 
		- nicIx[D-1].begin();
	  sCC[D]->entry(I, j) += P.sign();
	 }
	}
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
void fillDualHomologyCC(const Dim4Triangulation* tri, const unsigned long numDualCells[5], 
	const std::vector< std::vector< unsigned long > > &dcIx, std::vector< NMatrixInt* > &dCC)
{
    for (unsigned i=1; i<5; i++) // dCC[i]
        dCC[i] = new NMatrixInt(numDualCells[i-1], numDualCells[i]);
    dCC[0] = new NMatrixInt(1, numDualCells[0]);
    dCC[5] = new NMatrixInt(numDualCells[4], 1);

    // various useful pointers, index holders.
    const Dim4Vertex* vrt(NULL);  const Dim4Edge* edg(NULL);  const Dim4Face* fac(NULL); 
	const Dim4Tetrahedron* tet(NULL); const Dim4Pentachoron* pen(NULL);
    unsigned long J;

    unsigned long D = 1; // outer loop the row parameter. We start with dCC[1]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ pen = tri->getPentachoron( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 5; j++) {
	    tet = pen->getTetrahedron(j);  if (!tet->isBoundary())
	     {
	      J = lower_bound( dcIx[D].begin(), dcIx[D].end(), tri->tetrahedronIndex( tet ) ) - dcIx[D].begin();
	      dCC[D]->entry( i, J ) += ( ( (tet->getEmbedding(1).getPentachoron() == pen) && 
				           (tet->getEmbedding(1).getTetrahedron() == j) ) ? +1 : -1 );
	     } }
	}

    D = 2; // dCC[2]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ tet = tri->getTetrahedron( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 4; j++) {
	    fac = tet->getFace(j); if (!fac->isBoundary())
	     {
	      J = lower_bound( dcIx[D].begin(), dcIx[D].end(), tri->faceIndex( fac ) ) - dcIx[D].begin();
	      pen = tet->getEmbedding(1).getPentachoron(); // our ambient pentachoron
	      // the natural inclusions of our tetrahedron and face into the ambient pentachoron
	      NPerm5 tetinc( tet->getEmbedding(1).getVertices() );
	      NPerm5 facinc( pen->getFaceMapping( 
		Dim4Face::faceNumber[tetinc[(j<=0) ? 1 : 0]][tetinc[(j<=1)? 2 : 1]][tetinc[(j<=2)? 3 : 2]] 
						) );
	      dCC[D]->entry( i, J ) += ( tetinc[4]==facinc[4] ? 1 : -1 );
	     } }
	}

    D = 3; // dCC[3]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ fac = tri->getFace( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 3; j++) {
	    edg = fac->getEdge(j); if (!edg->isBoundary())
	     {
	      J = lower_bound( dcIx[D].begin(), dcIx[D].end(), tri->edgeIndex( edg ) ) - dcIx[D].begin();
	      pen = fac->getEmbedding(0).getPentachoron(); // our ambient pentachoron
	      // the natural inclusions of our face and edge into the ambient pentachoron
	      NPerm5 facinc( fac->getEmbedding(0).getVertices() );
	      NPerm5 edginc( pen->getEdgeMapping( 
		Dim4Edge::edgeNumber[facinc[(j<=0) ? 1 : 0]][facinc[(j<=1)? 2 : 1]] 
						) );
	      NPerm5 delta( edginc.inverse()*facinc*NPerm5(2, j) ); // we consider this as a permutation of {2,3,4}
              delta = delta * NPerm5( 0, delta[0] ); // kill permutation of {0,1} part of delta
	      dCC[D]->entry( i, J ) += delta.sign();
	     } }
	}

    D = 4; // dCC[4]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ edg = tri->getEdge( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 2; j++) {
	    vrt = edg->getVertex(j); if (!vrt->isBoundary() && !vrt->isIdeal())
	     {
	      J = lower_bound( dcIx[D].begin(), dcIx[D].end(), tri->vertexIndex( vrt ) ) - dcIx[D].begin();
	      pen = edg->getEmbedding(0).getPentachoron(); // our ambient pentachoron
	      // sign...
	      NPerm5 edginc( edg->getEmbedding(0).getVertices() );
	      NPerm5 vrtinc( pen->getVertexMapping( edginc[j] ) );
	      NPerm5 delta( vrtinc.inverse()*edginc*NPerm5(1, j) );
	      dCC[D]->entry( i, J ) += delta.sign();
	     } }
	}
}

void fillDualHomologyCC(const NTriangulation* tri, const unsigned long numDualCells[5], 
	const std::vector< std::vector< unsigned long > > &dcIx, std::vector< NMatrixInt* > &dCC)
{
    for (unsigned i=1; i<4; i++) // dCC[i]
        dCC[i] = new NMatrixInt(numDualCells[i-1], numDualCells[i]);
    dCC[0] = new NMatrixInt(1, numDualCells[0]);
    dCC[4] = new NMatrixInt(numDualCells[3], 1);

    // various useful pointers, index holders.
    const NVertex* vrt(NULL);  const NEdge* edg(NULL);  const NFace* fac(NULL); const NTetrahedron* tet(NULL); 
    unsigned long J;

    unsigned long D = 1; // outer loop the row parameter. We start with dCC[1]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ tet = tri->getTetrahedron( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 4; j++) {
	    fac = tet->getFace(j);  if (!fac->isBoundary())
	     {
	      J = lower_bound( dcIx[D].begin(), dcIx[D].end(), tri->faceIndex( fac ) ) - dcIx[D].begin();
	      dCC[D]->entry( i, J ) += ( ( (fac->getEmbedding(1).getTetrahedron() == tet) && 
				       (fac->getEmbedding(1).getFace() == j) ) ? +1 : -1 );
	     } }
	}

    D = 2; // dCC[2]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ fac = tri->getFace( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 3; j++) {
	    edg = fac->getEdge(j); if (!edg->isBoundary())
	     {
	      J = lower_bound( dcIx[D].begin(), dcIx[D].end(), tri->edgeIndex( edg ) ) - dcIx[D].begin();
	      tet = fac->getEmbedding(1).getTetrahedron(); // our ambient tetrahedron
	      // the natural inclusions of our tetrahedron and face into the ambient tetrahedron
	      NPerm4 facinc( fac->getEmbedding(1).getVertices() );
	      NPerm4 edginc( tet->getEdgeMapping( 
		NEdge::edgeNumber[facinc[(j<=0) ? 1 : 0]][facinc[(j<=1)? 2 : 1]] 
						) );
	      dCC[D]->entry( i, J ) += ( facinc[3]==edginc[3] ? 1 : -1 );
	     } }
	}
 
    D = 3; // dCC[3]
    for (unsigned long i=0; i<numDualCells[D-1]; i++) // dCC[D]->entry( i, * )
	{ edg = tri->getEdge( dcIx[D-1][i] );
	  for (unsigned long j=0; j < 2; j++) {
	    vrt = edg->getVertex(j); if (!vrt->isBoundary() && !vrt->isIdeal())
	     {
	      J = lower_bound( dcIx[D].begin(), dcIx[D].end(), tri->vertexIndex( vrt ) ) - dcIx[D].begin();
	      tet = edg->getEmbedding(0).getTetrahedron(); // our ambient tetrahedron
	      // sign...
	      NPerm4 edginc( edg->getEmbedding(0).getVertices() );
	      NPerm4 vrtinc( tet->getVertexMapping( edginc[j] ) );
	      NPerm4 delta( vrtinc.inverse()*edginc*NPerm4(1, j) );
	      dCC[D]->entry( i, J ) += delta.sign();
	     } }
	}
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
void fillMixedHomologyCC(const Dim4Triangulation* tri, 
     const unsigned long numMixCells[5], const unsigned long numNonIdealCells[5], 
     const unsigned long numIdealCells[4], 
     const std::vector< std::vector< unsigned long > > &icIx, 
     const std::vector< std::vector< unsigned long > > &nicIx, 
     std::vector< NMatrixInt* > &mCC)
{    
 for (unsigned i=1; i<5; i++) // mCC[i]
        mCC[i] = new NMatrixInt(numMixCells[i-1], numMixCells[i]);
    mCC[0] = new NMatrixInt(1, numMixCells[0]);
    mCC[5] = new NMatrixInt(numMixCells[4], 1);

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

   for (unsigned long j=0; j<2*numNonIdealCells[1]; j++)
	{ // j % 2  mCC[D]->entry( *, j )
	 edg = tri->getEdge(nicIx[1][j/2]); vrt = edg->getVertex( j%2 );
	 if (vrt->isIdeal())
	  { I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - icIx[D-1].begin();
            mCC[D]->entry( ri5 + I, j ) += 1; }
         else
          { I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->vertexIndex( vrt ) ) - nicIx[D-1].begin();
	    mCC[D]->entry( I, j ) += ( (j%2)==0 ? -1 : 1 ); }  
         mCC[D]->entry( ri1 + (j/2), j ) += ( (j%2)==0 ? 1 : -1); // vertex on the edge
	}

   for (unsigned long j=0; j<3*numNonIdealCells[2]; j++)
	{ // j % 3  mCC[D]->entry( *, ci1+j )
	 fac = tri->getFace(nicIx[2][j/3]); edg = fac->getEdge( j%3 );
	 I = lower_bound( nicIx[D].begin(), nicIx[D].end(), tri->edgeIndex( edg ) ) - nicIx[D].begin();
	 mCC[D]->entry( ri1 + I, ci1 + j ) += 1;
	 mCC[D]->entry( ri2 + (j/3), ci1 + j ) -= 1; 
	}

   for (unsigned long j=0; j<4*numNonIdealCells[3]; j++)
	{ // j % 4  mCC[D]->entry( *, ci2+j )
	 tet = tri->getTetrahedron(nicIx[3][j/4]); fac = tet->getFace( j%4 );
	 I = lower_bound( nicIx[D+1].begin(), nicIx[D+1].end(), tri->faceIndex( fac ) ) - nicIx[D+1].begin();
	 mCC[D]->entry( ri2 + I, ci2 + j ) += 1;
	 mCC[D]->entry( ri3 + (j/4), ci2 + j ) -= 1;
	}

   for (unsigned long j=0; j<5*numNonIdealCells[4]; j++) 
	{ // j % 5  mCC[D]->entry( *, ci3+j )            
	 pen = tri->getPentachoron(nicIx[4][j/5]); tet = pen->getTetrahedron( j%5 );
	 int sig( (tet->getEmbedding(0).getPentachoron() == pen) &&
		  (tet->getEmbedding(0).getTetrahedron() == (j%5)) ? 1 : -1 ); 
	 I = lower_bound( nicIx[D+2].begin(), nicIx[D+2].end(), tri->tetrahedronIndex( tet ) ) - nicIx[D+2].begin();
	 mCC[D]->entry( ri3 + I, ci3 + j ) += sig;
	 mCC[D]->entry( ri4 + (j/5), ci3 + j ) -= sig;	
	}

   for (unsigned long j=0; j<numIdealCells[D]; j++)
	{ // j%3    mCC[D]->entry( *, ci4+j )
	 fac = tri->getFace( icIx[D][j]/(D+2) );
	 for (unsigned long i=1; i<(D+2); i++)
	  {          
	  NPerm5 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  unsigned long iX( (D+1)*tri->edgeIndex( fac->getEdge( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  mCC[D]->entry( ri5 + I, ci4 + j ) -= P.sign();
	  }
	}

   ri1 = ci1; ri2 = ci2; ri3 = ci3; ri4 = ci4;
   ci1 = 3*numNonIdealCells[2]; ci2 = ci1 + 6*numNonIdealCells[3]; ci3 = ci2 + 10*numNonIdealCells[4];
   D = 2; // mCC[2]
   for (unsigned long j=0; j<3*numNonIdealCells[2]; j++) // 4 boundary facets, 5 if vrt ideal
	{ // j%3,  mCC[D]->entry( *, j )
	 fac = tri->getFace(nicIx[2][j/3]); vrt = fac->getVertex( j%3 ); 
         for (unsigned i=1; i<3; i++)
	  {
	   edg = fac->getEdge( (j+i)%3 ); NPerm5 edginc = fac->getEdgeMapping( (j+i)%3 );
	   I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->edgeIndex( edg ) ) - nicIx[D-1].begin();
	   mCC[D]->entry( 2*I + ( edginc.sign()==1 ? 2-i : i-1 ), j ) += edginc.sign();
           mCC[D]->entry( ri1 + 3*(j/3)+( (j+i)%3 ), j ) += ( i==1 ? 1 : -1 );
	  }
	 if (vrt->isIdeal()) 
	  {
	   I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - icIx[D-1].begin();
	   mCC[D]->entry( ri4 + I, j ) += 1 ;
	  }
	}

   for (unsigned long j=0; j<6*numNonIdealCells[3]; j++)  // 6 facets in a tetrahedron
	{ // j%6,  mCC[D]->entry( *, ci1+j )
	 tet = tri->getTetrahedron(nicIx[3][j/6]);   
	 NPerm5 edginc = tet->getEdgeMapping( j%6 ); 

	for (unsigned long i=0; i<2; i++)
	 {
          fac = tet->getFace( edginc[i+2] ); NPerm5 facinc = tet->getFaceMapping( edginc[i+2] );
	  I = lower_bound( nicIx[D].begin(), nicIx[D].end(), tri->faceIndex( fac ) ) - nicIx[D].begin();
          mCC[D]->entry( ri1 + 3*I + (facinc.preImageOf(edginc[3-i])), ci1 + j ) += 
		( i == 0 ? 1 : -1 ); // face part
	  mCC[D]->entry( ri2 + 4*(j/6)+edginc[i+2], ci1 + j ) += ( i == 0 ? 1 : -1 ); // tet part
	 }
	}

   for (unsigned long j=0; j<10*numNonIdealCells[4]; j++) // dual to faces of pentachoron
	{ // j%10, mCC[D]->entry( *, ci2+j )
	 pen = tri->getPentachoron(nicIx[4][j/10]); NPerm5 facinc = pen->getFaceMapping( j%10 );
	 for (unsigned long i=0; i<2; i++)
	  {
	   tet = pen->getTetrahedron( facinc[i+3] ); NPerm5 tetinc = pen->getTetrahedronMapping( facinc[i+3] );
	   I = lower_bound( nicIx[D+1].begin(), nicIx[D+1].end(), tri->tetrahedronIndex( tet ) ) - nicIx[D+1].begin();
	   mCC[D]->entry( ri2 + 4*I + tetinc.preImageOf(facinc[4-i]), ci2 + j ) += 
		(i == 0 ? 1 : -1 ); // tet part
	   int sig( (tet->getEmbedding(0).getPentachoron() == pen) &&
		    (tet->getEmbedding(0).getTetrahedron() == facinc[i+3]) ? 1 : -1);
	   mCC[D]->entry( ri3 + 5*(j/10) + facinc[i+3], ci2 + j ) += sig*(i == 0 ? 1 : -1); // pen part
	  }
	}

   for (unsigned long j=0; j<numIdealCells[2]; j++)
	{ // j$3,  mCC[D]->entry( *, ci3+j )
	 tet = tri->getTetrahedron( icIx[D][j]/(D+2) );
	 for (unsigned long i=1; i<D+2; i++)
	  {
           NPerm5 facinc( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	   unsigned long iX( (D+1)*tri->faceIndex( tet->getFace( (icIx[D][j] + i) % (D+2) ) ) 
			            + ( facinc.preImageOf(icIx[D][j] % (D+2)) ) );
	   I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	   mCC[D]->entry( ri4 + I, ci3 + j ) -= facinc.sign();
	  }
	}

   ri1 = ci1; ri2 = ci2; ri3 = ci3;
   ci1 = 4*numNonIdealCells[3]; ci2 = ci1 + 10*numNonIdealCells[4];
   D = 3; // mCC[3]
   for (unsigned long j=0; j<4*numNonIdealCells[3]; j++)
	{ // j%4, mCC[D]->entry( *, j )
	 tet = tri->getTetrahedron( nicIx[D][j/4] ); vrt = tet->getVertex( j%4 );
	 for (unsigned long i=1; i<4; i++) // boundary facets corresponding to face j+i&4 and edge j%4, (j+1)%4.
	  {
	   fac = tet->getFace( (j+i)%4 ); NPerm5 facinc = tet->getFaceMapping( (j+i)%4 );  // tet index wrong?
	   I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->faceIndex( fac ) ) - nicIx[D-1].begin();
	   mCC[D]->entry( 3*I + facinc.preImageOf(j%4), j ) += facinc.sign(); // face
           NPerm5 edginc = tet->getEdgeMapping( NEdge::edgeNumber[ j%4 ][ (j+i)%4 ] );
	   mCC[D]->entry( ri1 + 6*(j/4) + NEdge::edgeNumber[ j%4 ][ (j+i)%4 ], j) += 
	     (edginc[1] == (j%4) ? 1 : -1) * edginc.sign(); // edge
	  }
	 if (vrt->isIdeal())
	  {
	   I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - icIx[D-1].begin();
	   mCC[D]->entry( ri3 + I, j ) += 1 ;
	  }
	}

   for (unsigned long j=0; j<10*numNonIdealCells[4]; j++) 
	{ // j%10, mCC[D]->entry( *, ci1 + j )
	 pen = tri->getPentachoron( nicIx[D][j/10] ); NPerm5 edginc( pen->getEdgeMapping( j%10 ) );
	 for (unsigned long i=2; i<5; i++) // boundary facets have 3 parts dual to edges in tets, 3 dual to faces in pen
	  { 
	   tet = pen->getTetrahedron( edginc[i] ); NPerm5 tetinc( pen->getTetrahedronMapping( edginc[i] ) ); 
	   NPerm5 edgtetinc( tet->getEdgeMapping( // how edg sits in tet
		NEdge::edgeNumber[tetinc.preImageOf(edginc[0])][tetinc.preImageOf(edginc[1])] ) );
	   // part dual to an edge in tet.
	   I = lower_bound( nicIx[D].begin(), nicIx[D].end(), tri->tetrahedronIndex( tet ) ) - nicIx[D].begin();
	   mCC[D]->entry( ri1 + 6*I + NEdge::edgeNumber[tetinc.preImageOf(edginc[0])][tetinc.preImageOf(edginc[1])], 
		ci1 + j ) -= ( (tetinc*edgtetinc).inverse()*edginc).sign(); 
	   // part dual to a face in pen
	   NPerm5 facinc( pen->getFaceMapping( Dim4Face::faceNumber[edginc[0]][edginc[1]][edginc[i]] ) );
	   NPerm5 delta( edginc.inverse()*facinc*NPerm5( 2, facinc.preImageOf(edginc[i]) ) );
	   delta = delta * NPerm5(0, delta[0]);
	   mCC[D]->entry( ri2 + 10*(j/10) + Dim4Face::faceNumber[edginc[0]][edginc[1]][edginc[i]], ci1 + j ) 
		+= delta.sign(); // tetinc.inverse()*edginc sends 0,1 into 0,1,2 and
	  } 
	}

   for (unsigned long j=0; j<numIdealCells[3]; j++)
	{ // j%3, mCC[D]->entry( *, ci2 + j )
	pen = tri->getPentachoron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm5 P( pen->getTetrahedronMapping( (icIx[D][j] + i) % (D+2)) );
	  unsigned long iX( (D+1)*tri->tetrahedronIndex( pen->getTetrahedron( (icIx[D][j] + i) % (D+2) ) ) 
			            + ( P.preImageOf(icIx[D][j] % (D+2)) )     );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  mCC[D]->entry(ri3 + I,  ci2 + j) -= P.sign(); 
	 }
	}

   ri1 = ci1; ri2 = ci2;
   D = 4; // mCC[4]
   for (unsigned long j=0; j<5*numNonIdealCells[4]; j++)
	{ // j%5, mCC[D]->entry( *, j )
	  pen = tri->getPentachoron( nicIx[D][j/5] ); vrt = pen->getVertex( j%5 );
	  for (unsigned long i=1; i<5; i++) 
	   {
	    // standard boundary part opposite tet (j+i)%5 in facet j%5 of pen j/5
	    tet = pen->getTetrahedron( (j+i)%5 ); NPerm5 tetinc( pen->getTetrahedronMapping( (j+i)%5 ) );
	    I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->tetrahedronIndex( tet ) ) - nicIx[D-1].begin();
	    mCC[D]->entry( 4*I + tetinc.preImageOf( j%5 ), j ) += tetinc.sign();
	    // part dual to edges 0,i
	    edg = pen->getEdge( Dim4Edge::edgeNumber[j%5][(i+j)%5] );
	    NPerm5 edginc( pen->getEdgeMapping( Dim4Edge::edgeNumber[j%5][(i+j)%5] ) );
	    mCC[D]->entry( ri1 + 10*(j/5) + Dim4Edge::edgeNumber[j%5][(i+j)%5], j ) += 
		( (edginc[1] == (j%5)) ? 1 : -1)*edginc.sign(); 
	   }
	  // potentially ideal boundary part
	  if (vrt->isIdeal()) 
		{
		 I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - icIx[D-1].begin();
		 mCC[D]->entry( ri2 + I, j ) += 1;
	 	}
	}   
}


void fillMixedHomologyCC(const NTriangulation* tri, 
     const unsigned long numMixCells[5], const unsigned long numNonIdealCells[5], 
     const unsigned long numIdealCells[4], 
     const std::vector< std::vector< unsigned long > > &icIx, 
     const std::vector< std::vector< unsigned long > > &nicIx, 
     std::vector< NMatrixInt* > &mCC)
{    
 for (unsigned i=1; i<4; i++) // mCC[i]
        mCC[i] = new NMatrixInt(numMixCells[i-1], numMixCells[i]);
    mCC[0] = new NMatrixInt(1, numMixCells[0]);
    mCC[4] = new NMatrixInt(numMixCells[3], 1);

   // various useful pointers, index holders.
    const NVertex* vrt(NULL);  const NEdge* edg(NULL);  const NFace* fac(NULL); const NTetrahedron* tet(NULL); 
    unsigned long I;
   // we'll also need to remember some placeholder indices
   unsigned long ri1 = numNonIdealCells[0];        unsigned long ri2 = ri1 + numNonIdealCells[1];
   unsigned long ri3 = ri2 + numNonIdealCells[2];  unsigned long ri4 = ri3 + numNonIdealCells[3];
   unsigned long ci1 = 2*numNonIdealCells[1];      unsigned long ci2 = ci1 + 3*numNonIdealCells[2]; 
   unsigned long ci3 = ci2 + 4*numNonIdealCells[3];
   unsigned long D = 1; // outer loop the column parameter. We start with mCC[1]

   for (unsigned long j=0; j<2*numNonIdealCells[1]; j++)
	{ // j % 2  mCC[D]->entry( *, j )
	 edg = tri->getEdge(nicIx[1][j/2]); vrt = edg->getVertex( j%2 );
	 if (vrt->isIdeal())
	  { I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - icIx[D-1].begin();
            mCC[D]->entry( ri4 + I, j ) += 1; }
         else
          { I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->vertexIndex( vrt ) ) - nicIx[D-1].begin();
	    mCC[D]->entry( I, j ) += ( (j%2)==0 ? -1 : 1 ); }  
         mCC[D]->entry( ri1 + (j/2), j ) += ( (j%2)==0 ? 1 : -1); // vertex on the edge
	}

   for (unsigned long j=0; j<3*numNonIdealCells[2]; j++)
	{ // j % 3  mCC[D]->entry( *, ci1+j )
	 fac = tri->getFace(nicIx[2][j/3]); edg = fac->getEdge( j%3 );
	 I = lower_bound( nicIx[D].begin(), nicIx[D].end(), tri->edgeIndex( edg ) ) - nicIx[D].begin();

	 mCC[D]->entry( ri1 + I, ci1 + j ) += 1;
	 mCC[D]->entry( ri2 + (j/3), ci1 + j ) -= 1; 
	}

   for (unsigned long j=0; j<4*numNonIdealCells[3]; j++)
	{ // j % 4  mCC[D]->entry( *, ci2+j )
	 tet = tri->getTetrahedron(nicIx[3][j/4]); fac = tet->getFace( j%4 );
	 int sig( (fac->getEmbedding(0).getTetrahedron() == tet) &&
		  (fac->getEmbedding(0).getFace() == (j%4)) ? 1 : -1 ); 
	 I = lower_bound( nicIx[D+1].begin(), nicIx[D+1].end(), tri->faceIndex( fac ) ) - nicIx[D+1].begin();

	 mCC[D]->entry( ri2 + I, ci2 + j ) += sig;
	 mCC[D]->entry( ri3 + (j/4), ci2 + j ) -= sig;
	}

   for (unsigned long j=0; j<numIdealCells[D]; j++)
	{ // j%3    mCC[D]->entry( *, ci4+j )
	 fac = tri->getFace( icIx[D][j]/(D+2) );
	 for (unsigned long i=1; i<(D+2); i++)
	  {          
	  NPerm4 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  unsigned long iX( (D+1)*tri->edgeIndex( fac->getEdge( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  mCC[D]->entry( ri4 + I, ci3 + j ) -= P.sign();
	  }
	}

   ri1 = ci1; ri2 = ci2; ri3 = ci3; 
   ci1 = 3*numNonIdealCells[2]; ci2 = ci1 + 6*numNonIdealCells[3]; 
   D = 2; // mCC[2]
   for (unsigned long j=0; j<3*numNonIdealCells[2]; j++) // 4 boundary facets, 5 if vrt ideal
	{ // j%3,  mCC[D]->entry( *, j )
	 fac = tri->getFace(nicIx[2][j/3]); vrt = fac->getVertex( j%3 ); 
         for (unsigned i=1; i<3; i++)
	  {
	   edg = fac->getEdge( (j+i)%3 ); NPerm4 edginc = fac->getEdgeMapping( (j+i)%3 );
	   I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->edgeIndex( edg ) ) - nicIx[D-1].begin();

	   mCC[D]->entry( 2*I + ( edginc.sign()==1 ? 2-i : i-1 ), j ) += edginc.sign();
           mCC[D]->entry( ri1 + 3*(j/3)+( (j+i)%3 ), j ) += ( i==1 ? 1 : -1 );
	  }
	 if (vrt->isIdeal()) 
	  {
	   I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - icIx[D-1].begin();
	   mCC[D]->entry( ri3 + I, j ) += 1 ;
	  }
	}

   for (unsigned long j=0; j<6*numNonIdealCells[3]; j++)  // 6 facets in a tetrahedron
	{ // j%6,  mCC[D]->entry( *, ci1+j )
	 tet = tri->getTetrahedron(nicIx[3][j/6]); NPerm4 edginc = tet->getEdgeMapping( j%6 ); 
	for (unsigned long i=0; i<2; i++)
	 {
          fac = tet->getFace( edginc[i+2] ); NPerm4 facinc = tet->getFaceMapping( edginc[i+2] );
	  int sig( (fac->getEmbedding(0).getTetrahedron() == tet) &&
		   (fac->getEmbedding(0).getFace() == edginc[i+2]) ? 1 : -1 );
          I = lower_bound( nicIx[D].begin(), nicIx[D].end(), tri->faceIndex( fac ) ) - nicIx[D].begin(); 

          mCC[D]->entry( ri1 + 3*I + (facinc.preImageOf(edginc[3-i])), ci1 + j ) += 
	               ( i == 0 ? 1 : -1 ); // face part
	  mCC[D]->entry( ri2 + 4*(j/6)+edginc[i+2], ci1 + j ) += sig*( i == 0 ? 1 : -1 ); // tet part
	 }
	}

   for (unsigned long j=0; j<numIdealCells[2]; j++)
	{ // j$3,  mCC[D]->entry( *, ci3+j )
	 tet = tri->getTetrahedron( icIx[D][j]/(D+2) );
	 for (unsigned long i=1; i<D+2; i++)
	  {
           NPerm4 facinc( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	   unsigned long iX( (D+1)*tri->faceIndex( tet->getFace( (icIx[D][j] + i) % (D+2) ) ) 
			            + ( facinc.preImageOf(icIx[D][j] % (D+2)) ) );
	   I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	   mCC[D]->entry( ri3 + I, ci2 + j ) -= facinc.sign();
	  }
	}

   ri1 = ci1; ri2 = ci2; 
   D = 3; // mCC[3]
   for (unsigned long j=0; j<4*numNonIdealCells[3]; j++)
	{ // j%4, mCC[D]->entry( *, j )
	 tet = tri->getTetrahedron( nicIx[D][j/4] ); vrt = tet->getVertex( j%4 );
	 for (unsigned long i=1; i<4; i++) // boundary facets corresponding to face j+i&4 and edge j%4, (j+1)%4.
	  {
	   fac = tet->getFace( (j+i)%4 ); NPerm4 facinc = tet->getFaceMapping( (j+i)%4 );  
	   I = lower_bound( nicIx[D-1].begin(), nicIx[D-1].end(), tri->faceIndex( fac ) ) - nicIx[D-1].begin();
	   mCC[D]->entry( 3*I + facinc.preImageOf(j%4), j ) += facinc.sign(); // face

           NPerm4 edginc = tet->getEdgeMapping( NEdge::edgeNumber[ j%4 ][ (j+i)%4 ] );
	   mCC[D]->entry( ri1 + 6*(j/4) + NEdge::edgeNumber[ j%4 ][ (j+i)%4 ], j) += 
	     (edginc[1] == (j%4) ? 1 : -1) * edginc.sign(); // edge
	  }
	 if (vrt->isIdeal())
	  {
	   I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), j ) - icIx[D-1].begin();
	   mCC[D]->entry( ri2 + I, j ) += 1 ;
	  }
	}
}

void fillBoundaryHomologyCC(const Dim4Triangulation* tri, 
	const unsigned long numStandardBdryCells[4],  const unsigned long numIdealCells[4], 
	const unsigned long numNonIdealBdryCells[4],
	const std::vector< std::vector< unsigned long > > &bcIx, const std::vector< std::vector< unsigned long > > &icIx, 
	std::vector< NMatrixInt* > &sbCC)
{
    // initialize chain complex matrices.
    for (unsigned i=1; i<4; i++) // sbCC[i]
        sbCC[i] = new NMatrixInt(numStandardBdryCells[i-1], numStandardBdryCells[i]);
    sbCC[0] = new NMatrixInt(1, numStandardBdryCells[0]);
    sbCC[4] = new NMatrixInt(numStandardBdryCells[3], 1);

    // various useful pointers, index holders.
    const Dim4Edge* edg(NULL);  const Dim4Face* fac(NULL); const Dim4Tetrahedron* tet(NULL); const Dim4Pentachoron* pen(NULL);
    unsigned long I;

    // now we fill them out, first sbCC.  sbCC[0] is zero, 
    unsigned long D=1; // sbCC[D]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // sbCC[D]->entry( *,j )
	{ // endpts getEdge(nicIx[D][j]) ideal?
	edg = tri->getEdge(bcIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->getVertex(i)->isIdeal())
	 {   // endpt i is ideal, find index
          I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	  sbCC[D]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
	 } 
	else // endpt i is not ideal
	 {
          I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->vertexIndex( edg->getVertex(i) ) )
	      - bcIx[D-1].begin();
	  sbCC[D]->entry(I, j) += ( i == 0 ? -1 : 1 );
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // sbCC[D]->entry( *, numNonIdealCells[D] + j )
        { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
	fac = tri->getFace(icIx[D][j]/(D+2));
	for (unsigned long i=1; i<D+2; i++)
	 {
          NPerm5 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  unsigned long iX( (D+1)*tri->edgeIndex( fac->getEdge( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  sbCC[D]->entry(numNonIdealBdryCells[D-1] + I,  numNonIdealBdryCells[D] + j) -= P.sign();
	 }
	}

    D = 2; // sbCC[2]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // sbCC[D]->entry( *,j )
	{
	fac = tri->getFace(bcIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (fac->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), (D+1)*j+i ) - bcIx[D-1].begin();
	    sbCC[D]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
	   } // standard face boundaries
	  NPerm5 P( fac->getEdgeMapping(i) );
          I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->edgeIndex( fac->getEdge(i) )) 
		- bcIx[D-1].begin();
	  sbCC[D]->entry(I, j) += P.sign();
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) // sbCC[D]->entry( *, j+numNonIdealCells[D-1] )
        { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
	tet = tri->getTetrahedron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm5 P( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	  unsigned long iX( (D+1)*tri->faceIndex( tet->getFace( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  sbCC[D]->entry(numNonIdealBdryCells[D-1] + I,  numNonIdealBdryCells[D] + j) -= P.sign(); 
	 }
	}

    D = 3; // sbCC[3]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // sbCC[D]->entry( *,j )
	{
	tet = tri->getTetrahedron(bcIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (tet->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	    sbCC[D]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
	   } // standard face boundaries
	  NPerm5 P( tet->getFaceMapping(i) );
          I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->faceIndex( tet->getFace(i) )) 
		- bcIx[D-1].begin();
	  sbCC[D]->entry(I, j) += P.sign();
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // sbCC[D]->entry( *, j+numNonIdealCells[D-1] )
        { // icIx[D][j]/(D+2) pentachoron icIx[1][j] % (D+2) vertex
	pen = tri->getPentachoron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm5 P( pen->getTetrahedronMapping( (icIx[D][j] + i) % (D+2)) );
	  unsigned long iX( (D+1)*tri->tetrahedronIndex( pen->getTetrahedron( (icIx[D][j] + i) % (D+2) ) ) 
			            + ( P.preImageOf(icIx[D][j] % (D+2)) )     );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  sbCC[D]->entry(numNonIdealBdryCells[D-1] + I,  numNonIdealBdryCells[D] + j) -= P.sign(); 
	 }
	}
}

void fillBoundaryHomologyCC(const NTriangulation* tri, 
	const unsigned long numStandardBdryCells[4],  const unsigned long numIdealCells[4],
        const unsigned long numNonIdealBdryCells[4],  
	const std::vector< std::vector< unsigned long > > &bcIx, const std::vector< std::vector< unsigned long > > &icIx, 
	std::vector< NMatrixInt* > &sbCC)
{
    // initialize chain complex matrices.
    for (unsigned i=1; i<3; i++) // sbCC[i]
        sbCC[i] = new NMatrixInt(numStandardBdryCells[i-1], numStandardBdryCells[i]);
    sbCC[0] = new NMatrixInt(1, numStandardBdryCells[0]);
    sbCC[3] = new NMatrixInt(numStandardBdryCells[2], 1);

    // various useful pointers, index holders.
    const NEdge* edg(NULL);  const NFace* fac(NULL); const NTetrahedron* tet(NULL); 
    unsigned long I;
    
    // now we fill them out, first sbCC.  sbCC[0] is zero, 
    unsigned long D=1; // sbCC[D]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // sbCC[D]->entry( *,j )
	{ // endpts getEdge(bcIx[D][j]) ideal?
	edg = tri->getEdge(bcIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->getVertex(i)->isIdeal())
	 {   // endpt i is ideal, find index
          I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	  sbCC[D]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
	 } 
	else // endpt i is not ideal
	 {
          I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->vertexIndex( edg->getVertex(i) ) )
	      - bcIx[D-1].begin();
	  sbCC[D]->entry(I, j) += ( i == 0 ? -1 : 1 );
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // sbCC[D]->entry( *, numNonIdealBdryCells[D] + j )
        { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
	fac = tri->getFace(icIx[D][j]/(D+2));
	for (unsigned long i=1; i<D+2; i++)
	 {
          NPerm4 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  unsigned long iX( (D+1)*tri->edgeIndex( fac->getEdge( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  sbCC[D]->entry(numNonIdealBdryCells[D-1] + I,  numNonIdealBdryCells[D] + j) -= P.sign();
	 }
	}

    D = 2; // sbCC[2]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // sbCC[D]->entry( *,j )
	{
	fac = tri->getFace(bcIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (fac->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	    sbCC[D]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
	   } // standard face boundaries
	  NPerm4 P( fac->getEdgeMapping(i) );
          I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->edgeIndex( fac->getEdge(i) )) 
		- bcIx[D-1].begin();
	  sbCC[D]->entry(I, j) += P.sign();
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) // sbCC[D]->entry( *, j+numNonIdealBdryCells[D-1] )
        { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
	tet = tri->getTetrahedron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm4 P( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	  unsigned long iX( (D+1)*tri->faceIndex( tet->getFace( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  sbCC[D]->entry(numNonIdealBdryCells[D-1] + I,  numNonIdealBdryCells[D] + j) -= P.sign(); 
	 }
	}
}

// new

void fillDualBoundaryHomologyCC(const Dim4Triangulation* tri,  // fills dbCC
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
// also todo fillMixedBoundaryHomologyCC     mbCC
//           fillDualRelBoundaryHomologyCC   drCC
//           fillMixedRelBoundaryHomologyCC  mrCC
//	      J = lower_bound( dcIx[D].begin(), dcIx[D].end(), tri->faceIndex( fac ) ) - dcIx[D].begin();
//	      dCC[D]->entry( i, J ) += ( ( (fac->getEmbedding(1).getTetrahedron() == tet) && 
//				       (fac->getEmbedding(1).getFace() == j) ) ? +1 : -1 );
// end new

void fillRelativeHomologyCC(const Dim4Triangulation* tri, 
	const unsigned long numRelativeCells[5], const std::vector< std::vector< unsigned long > > &rIx, 
	std::vector< NMatrixInt* > &srCC)
{
    // initialize chain complex matrices.
    for (unsigned i=1; i<5; i++) // srCC[i] defined for i == 0, ..., 5
	srCC[i] = new NMatrixInt(numRelativeCells[i-1], numRelativeCells[i]);
    srCC[0] = new NMatrixInt(1, numRelativeCells[0]);
    srCC[5] = new NMatrixInt(numRelativeCells[4], 1);

    // various useful pointers, index holders.
    const Dim4Edge* edg(NULL);  const Dim4Face* fac(NULL); const Dim4Tetrahedron* tet(NULL); const Dim4Pentachoron* pen(NULL);
    unsigned long I;
    
    // now we fill them out, first srCC.  srCC[0] is zero, 
    unsigned long D=1; // srCC[D]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{ // endpts getEdge(rIx[D][j]) ideal?
	edg = tri->getEdge(rIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if ( (!edg->getVertex(i)->isIdeal()) && (!edg->getVertex(i)->isBoundary()) )
	 {
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->vertexIndex( edg->getVertex(i) ) )
	      - rIx[D-1].begin();
	  srCC[D]->entry(I, j) += ( i == 0 ? -1 : 1 );
	 }
	}

    D = 2; // srCC[2]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{
	fac = tri->getFace(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!fac->getEdge(i)->isBoundary())
	 { 
	  NPerm5 P( fac->getEdgeMapping(i) );
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->edgeIndex( fac->getEdge(i) )) 
		- rIx[D-1].begin();
	  srCC[D]->entry(I, j) += P.sign();
	 }
	}

    D = 3; // srCC[3]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{
	tet = tri->getTetrahedron(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!tet->getFace(i)->isBoundary())
	 { 
	  NPerm5 P( tet->getFaceMapping(i) );
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->faceIndex( tet->getFace(i) )) 
		- rIx[D-1].begin();
	  srCC[D]->entry(I, j) += P.sign();
	 }
	}

    D = 4; // srCC[4]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{
	pen = tri->getPentachoron(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!pen->getTetrahedron(i)->isBoundary())
	 { 
	  NPerm5 P( pen->getTetrahedronMapping(i) );
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->tetrahedronIndex( pen->getTetrahedron(i) )) 
		- rIx[D-1].begin();
	  srCC[D]->entry(I, j) += P.sign();
	 }
	}
}

void fillRelativeHomologyCC(const NTriangulation* tri, 
	const unsigned long numRelativeCells[5], const std::vector< std::vector< unsigned long > > &rIx, 
	std::vector< NMatrixInt* > &srCC)
{
    // initialize chain complex matrices.
    for (unsigned i=1; i<4; i++) // srCC[i] defined for i == 0, ..., 5
	srCC[i] = new NMatrixInt(numRelativeCells[i-1], numRelativeCells[i]);
    srCC[0] = new NMatrixInt(1, numRelativeCells[0]);
    srCC[4] = new NMatrixInt(numRelativeCells[3], 1);

    // various useful pointers, index holders.
    const NEdge* edg(NULL);  const NFace* fac(NULL); const NTetrahedron* tet(NULL);
    unsigned long I;
    
    // now we fill them out, first srCC.  srCC[0] is zero, 
    unsigned long D=1; // srCC[D]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{ // endpts getEdge(rIx[D][j]) ideal?
	edg = tri->getEdge(rIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if ( (!edg->getVertex(i)->isIdeal()) && (!edg->getVertex(i)->isBoundary()) )
	 {
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->vertexIndex( edg->getVertex(i) ) )
	      - rIx[D-1].begin();
	  srCC[D]->entry(I, j) += ( i == 0 ? -1 : 1 );
	 }
	}

    D = 2; // srCC[2]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{
	fac = tri->getFace(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!fac->getEdge(i)->isBoundary())
	 { 
	  NPerm4 P( fac->getEdgeMapping(i) );
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->edgeIndex( fac->getEdge(i) )) 
		- rIx[D-1].begin();
	  srCC[D]->entry(I, j) += P.sign();
	 }
	}

    D = 3; // srCC[3]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // srCC[D]->entry( *,j )
	{
	tet = tri->getTetrahedron(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!tet->getFace(i)->isBoundary())
	 { 
	  NPerm4 P( tet->getFaceMapping(i) );
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->faceIndex( tet->getFace(i) )) 
		- rIx[D-1].begin();
	  srCC[D]->entry(I, j) += P.sign();
	 }
	}
}

// required forward declaration
void fillChainMaps( NTriangulation* tri3, Dim4Triangulation* tri4, 
                    unsigned long numStandardCells[5],    unsigned long numDualCells[5],     unsigned long numMixCells[5], 
                    unsigned long numStandardBdryCells[4],unsigned long numNonIdealCells[5], unsigned long numIdealCells[4], 
                    unsigned long numNonIdealBdryCells[4],unsigned long numRelativeCells[5], unsigned long numDualRelCells[5], 
                    unsigned long numMixRelCells[5],      unsigned long numMixBdryCells[4],  unsigned long numDualBdryCells[4], 
                    std::vector< std::vector<unsigned long> > &nicIx, std::vector< std::vector<unsigned long> > &icIx, 
                    std::vector< std::vector<unsigned long> > &dcIx,  std::vector< std::vector<unsigned long> > &bcIx, 
                    std::vector< std::vector<unsigned long> > &rIx, 
                    std::vector< NMatrixInt* > &sbiCM, std::vector< NMatrixInt* > &smCM, 
                    std::vector< NMatrixInt* > &dmCM,  std::vector< NMatrixInt* > &strCM, 
                    std::vector< NMatrixInt* > &schCM );


// constructor for 4-manifold triangulations
NCellularData::NCellularData(const Dim4Triangulation& input): ShareableObject(),
        tri4(new Dim4Triangulation(input)), tri3(0), 
	nicIx(5), icIx(4), dcIx(5), bcIx(4), rIx(5), // indexing cells 
	sCC(6), sbCC(5), srCC(6),   dCC(6), dbCC(5), drCC(6),   mCC(6), mbCC(5), mrCC(6), // chain complexes 
	sbiCM(4), strCM(5), schCM(4),   dbiCM(4), dtrCM(5), dchCM(4),   mbiCM(4), mtrCM(5), mchCM(4), 
        smCM(5), dmCM(5), smbCM(4), dmbCM(4), srmCM(5), drmCM(5) // chain maps
//        normalsDim4BdryFaces(0),  normalsDim4BdryEdges(0), normalsDim3BdryEdges(0), normalsDim3BdryVertices(0) // orientations
{
   setupIndices( tri4, nicIx, icIx, dcIx, bcIx, rIx, numStandardCells, numDualCells, numMixCells, 
			numStandardBdryCells, numNonIdealCells, numIdealCells, numNonIdealBdryCells, 
			numRelativeCells, numDualRelCells, numMixRelCells, numMixBdryCells, numDualBdryCells );
        
   fillStandardHomologyCC( tri4, numStandardCells, numNonIdealCells, numIdealCells, 
			nicIx, icIx, sCC);

   fillDualHomologyCC( tri4, numDualCells, dcIx, dCC );

   fillMixedHomologyCC( tri4, numMixCells, numNonIdealCells, numIdealCells, icIx, nicIx, mCC );

   fillBoundaryHomologyCC( tri4, numStandardBdryCells, numIdealCells, numNonIdealBdryCells, bcIx, icIx, sbCC );

   fillRelativeHomologyCC( tri4, numRelativeCells, rIx, srCC );

   // todo fillMixedBdryHomology, fillDualBdryHomology, fillMixRelBdryHomology, fillDualRelBdryHomology

   fillChainMaps( tri3, tri4, numStandardCells, numDualCells, numMixCells, numStandardBdryCells, 
                                  numNonIdealCells, numIdealCells, numNonIdealBdryCells, 
	                          numRelativeCells, numDualRelCells, numMixRelCells, numMixBdryCells, 
		                  numDualBdryCells, 
                    nicIx, icIx, dcIx, bcIx, rIx, 
                    sbiCM, smCM, dmCM, strCM, schCM );

   buildExtraNormalData();
   buildMaximalTree(); 

}

// constructor for 3-manifold triangulations
NCellularData::NCellularData(const NTriangulation& input): ShareableObject(),
        tri4(0), tri3(new NTriangulation(input)), 
	nicIx(4), icIx(3), dcIx(4), bcIx(3), rIx(4), // indexing cells 
	sCC(5), sbCC(4), srCC(5),   dCC(5), dbCC(4), drCC(5),   mCC(5), mbCC(4), mrCC(5), // chain complexes 
	sbiCM(3), strCM(4), schCM(3),   dbiCM(3), dtrCM(4), dchCM(3),   mbiCM(3), mtrCM(4), mchCM(3), 
        smCM(4), dmCM(4), smbCM(3), dmbCM(3), srmCM(4), drmCM(4) // chain maps
//        normalsDim4BdryFaces(0),  normalsDim4BdryEdges(0), normalsDim3BdryEdges(0), normalsDim3BdryVertices(0) // orientations
{

   setupIndices( tri3, nicIx, icIx, dcIx, bcIx, rIx, numStandardCells, numDualCells, numMixCells, 
		 numStandardBdryCells, numNonIdealCells, numIdealCells, numNonIdealBdryCells,
		 numRelativeCells, numDualRelCells, numMixRelCells, numMixBdryCells, numDualBdryCells );

   fillStandardHomologyCC( tri3, numStandardCells, numNonIdealCells, numIdealCells, 
			nicIx, icIx, sCC);

   fillDualHomologyCC( tri3, numDualCells, dcIx, dCC );

   fillMixedHomologyCC( tri3, numMixCells, numNonIdealCells, numIdealCells, icIx, nicIx, mCC );

   fillBoundaryHomologyCC( tri3, numStandardBdryCells, numIdealCells, numNonIdealBdryCells, bcIx, icIx, sbCC );

   fillRelativeHomologyCC( tri3, numRelativeCells, rIx, srCC );
   // todo fillMixedBdryHomology, fillDualBdryHomology, fillMixRelBdryHomology, fillDualRelBdryHomology

   fillChainMaps( tri3, tri4, numStandardCells, numDualCells, numMixCells, numStandardBdryCells, 
                                  numNonIdealCells, numIdealCells, numNonIdealBdryCells, 
	                          numRelativeCells, numDualRelCells, numMixRelCells, numMixBdryCells, 
		                  numDualBdryCells, 
                    nicIx, icIx, dcIx, bcIx, rIx, 
                    sbiCM, smCM, dmCM, strCM, schCM );
   // standard face boundaries

   buildExtraNormalData();
   buildMaximalTree(); 
}



} // namespace regina



