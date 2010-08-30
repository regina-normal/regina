
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
	sbiCM(4), strCM(5), schCM(4),   dbiCM(4), dtrCM(5), dchCM(4),   mbiCM(4), mtrCM(5), mchCM(4), 
        smCM(5), dmCM(5), smbCM(4), dmbCM(4), srmCM(5), drmCM(5) // chain maps
//        normalsDim4BdryFaces(0),  normalsDim4BdryEdges(0), normalsDim3BdryEdges(0), normalsDim3BdryVertices(0) // orientations
{
   setupIndices( tri4, nicIx, icIx, dcIx, bcIx, rIx, numStandardCells, numDualCells, numMixCells, 
			numStandardBdryCells, numNonIdealCells, numIdealCells, numNonIdealBdryCells, 
			numRelativeCells, numDualRelCells, numMixRelCells, numMixBdryCells, numDualBdryCells );
   buildExtraNormalData();     buildMaximalTree(); 

   // the 9 core chain complexes for genCC
   fillStandardHomologyCC();   fillDualHomologyCC();      fillMixedHomologyCC();
   fillBoundaryHomologyCC();   fillRelativeHomologyCC();
   // maps of chain complexes.
   fillStandardToMixedHomCM(); // more to do...

   // todo fillMixedBdryHomology, fillDualBdryHomology, fillMixRelBdryHomology, fillDualRelBdryHomology
   fillChainMaps( tri3, tri4, numStandardCells, numDualCells, numMixCells, numStandardBdryCells, 
                                  numNonIdealCells, numIdealCells, numNonIdealBdryCells, 
	                          numRelativeCells, numDualRelCells, numMixRelCells, numMixBdryCells, 
		                  numDualBdryCells, 
                    nicIx, icIx, dcIx, bcIx,  rIx, 
                    sbiCM, smCM, dmCM, strCM, schCM );
}

// constructor for 3-manifold triangulations
NCellularData::NCellularData(const NTriangulation& input): ShareableObject(),
        tri4(0), tri3(new NTriangulation(input)), 
	nicIx(4), icIx(3), dcIx(4), bcIx(3), rIx(4), // indexing cells 
	sbiCM(3), strCM(4), schCM(3),   dbiCM(3), dtrCM(4), dchCM(3),   mbiCM(3), mtrCM(4), mchCM(3), 
        smCM(4), dmCM(4), smbCM(3), dmbCM(3), srmCM(4), drmCM(4) // chain maps
//        normalsDim4BdryFaces(0),  normalsDim4BdryEdges(0), normalsDim3BdryEdges(0), normalsDim3BdryVertices(0) // orientations
{

   setupIndices( tri3, nicIx, icIx, dcIx, bcIx, rIx, numStandardCells, numDualCells, numMixCells, 
		 numStandardBdryCells, numNonIdealCells, numIdealCells, numNonIdealBdryCells,
		 numRelativeCells, numDualRelCells, numMixRelCells, numMixBdryCells, numDualBdryCells );
   buildExtraNormalData();     buildMaximalTree(); 

   // the 9 core chain complexes for genCC
   fillStandardHomologyCC();   fillDualHomologyCC();      fillMixedHomologyCC();
   fillBoundaryHomologyCC();   fillRelativeHomologyCC();
   // todo fillMixedBdryHomology, fillDualBdryHomology, fillMixRelBdryHomology, fillDualRelBdryHomology
   // chain maps
   fillStandardToMixedHomCM(); // more to do...

   fillChainMaps( tri3, tri4, numStandardCells, numDualCells, numMixCells, numStandardBdryCells, 
                                  numNonIdealCells, numIdealCells, numNonIdealBdryCells, 
	                          numRelativeCells, numDualRelCells, numMixRelCells, numMixBdryCells, 
		                  numDualBdryCells, 
                    nicIx, icIx, dcIx, bcIx,  rIx, 
                    sbiCM, smCM, dmCM, strCM, schCM );
}


} // namespace regina

