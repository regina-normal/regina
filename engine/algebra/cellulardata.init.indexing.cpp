
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

namespace regina {

// only used in the CellularData constructor
void setupIndices(const Triangulation<4>* tri,   
 std::vector< std::vector<unsigned long> > &nicIx,  
 std::vector< std::vector<unsigned long> > &icIx, 
 std::vector< std::vector<unsigned long> > &dcIx,   
 std::vector< std::vector<unsigned long> > &bcIx,
 std::vector< std::vector<unsigned long> > &rIx,  
 unsigned long numStandardCells[5],    unsigned long numDualCells[5], 
 unsigned long numMixCells[5],         unsigned long numStandardBdryCells[4], 
 unsigned long numNonIdealCells[5],    unsigned long numIdealCells[4], 
 unsigned long numNonIdealBdryCells[4],unsigned long numRelativeCells[5], 
 unsigned long numDualRelCells[5],     unsigned long numMixRelCells[5],   
 unsigned long numMixBdryCells[4],     unsigned long numDualBdryCells[4])
{
 // nicIx[0]  interior or boundary,  bcIx[0] boundary,  dcIx[4] interior vertices. 
 for (Dim4Triangulation::VertexIterator vit = tri->vertices().begin();
      vit != tri->vertices().end(); vit++) if ( !(*vit)->isIdeal() ) {
  nicIx[0].push_back( (*vit)->index() );
  if ((*vit)->isBoundary()) bcIx[0].push_back( (*vit)->index() ); else
   { dcIx[4].push_back( (*vit)->index() );
    rIx[0].push_back( (*vit)->index() ); }
  } 
  // nicIx[1] all,  icIx[0] ideal ends, dcIx[3] nonboundary, bcIx[1] boundary
  for (Dim4Triangulation::EdgeIterator eit = tri->edges().begin();
       eit != tri->edges().end(); eit++) 
  {
   nicIx[1].push_back( (*eit)->index() );
   if ((*eit)->isBoundary()) bcIx[1].push_back( (*eit)->index() );
   else 
   { dcIx[3].push_back( (*eit)->index() );
     rIx[1].push_back( (*eit)->index() );
     for (unsigned i=0;i<2;i++) if ((*eit)->vertex(i)->isIdeal()) 
     icIx[0].push_back(2*(*eit)->index()+i); 
   }
  }
  // nicIx[2] all, icIx[1] ideal ends, dcIx[2] nonboundary, bcIx[2] boundary
  for (Dim4Triangulation::TriangleIterator fit = tri->triangles().begin();
       fit != tri->triangles().end(); fit++) 
  {
   nicIx[2].push_back( (*fit)->index() );
   if ((*fit)->isBoundary()) { bcIx[2].push_back( (*fit)->index() ); }
   else
   { dcIx[2].push_back( (*fit)->index() );
     rIx[2].push_back( (*fit)->index() );
     for (unsigned i=0;i<3;i++) if ((*fit)->vertex(i)->isIdeal()) 
     icIx[1].push_back(3*(*fit)->index()+i); }
   }
   for (Triangulation<4>::TetrahedronIterator tit = tri->tetrahedra().begin();
        tit != tri->tetrahedra().end(); tit++) 
   {
    nicIx[3].push_back( (*tit)->index() );
    if ((*tit)->isBoundary()) bcIx[3].push_back( (*tit)->index() );
	 else
     { dcIx[1].push_back( (*tit)->index() );
       rIx[3].push_back( (*tit)->index() );
       for (unsigned i=0;i<4;i++) if ((*tit)->vertex(i)->isIdeal()) 
        icIx[2].push_back(4*(*tit)->index()+i); 
     }
   }
  for (Triangulation<4>::PentachoronIterator pit = tri->pentachora().begin();
       pit != tri->pentachora().end(); pit++) 
   {
    nicIx[4].push_back( (*pit)->index() );
    dcIx[0].push_back( (*pit)->index() );
    rIx[4].push_back( (*pit)->index() );
    for (unsigned i=0;i<5;i++) if ((*pit)->vertex(i)->isIdeal()) 
     icIx[3].push_back(5*(*pit)->index()+i);
   }

  // standard CW-decomposition (0..4)-cells / triangulation + ideal cells.
  for (unsigned i=0; i<4; i++) 
   numStandardCells[i] = nicIx[i].size() + icIx[i].size();
  numStandardCells[4] = nicIx[4].size();
  // dual (0..4)-cells : a dual k-cell for every n-k-cell in triangulation
  for (unsigned i=0; i<5; i++) numDualCells[i] = dcIx[i].size();
  // relative (0..4)-cells : non-boundary cells from triangulation
  for (unsigned i=0; i<5; i++) numRelativeCells[i] = rIx[i].size();
  // boundary (0..3)-cells: boundary triangulation + ideal cells
  for (unsigned i=0; i<4; i++) 
    numStandardBdryCells[i] = bcIx[i].size() + icIx[i].size();
  // ideal and non-ideal cells: cells from std cw-decomposition w/ aren't ideal
  for (unsigned i=0; i<5; i++) numNonIdealCells[i] = nicIx[i].size();
  // cells from std cw-decomposition that are ideal
  for (unsigned i=0; i<4; i++) numIdealCells[i] = icIx[i].size();
  // boundary cells from std cw-decomposition that are not ideal
  for (unsigned i=0; i<4; i++) numNonIdealBdryCells[i] = bcIx[i].size();

  // this mixed decomposition is the proper cell decomposition induced by the 
  // barycentric subdivision, so all previous internal/boundary standard 
  // cells contribute barycentres
  numMixCells[0] = numNonIdealCells[0] + numNonIdealCells[1] + 
   numNonIdealCells[2] + numNonIdealCells[3] + numNonIdealCells[4] + 
   numIdealCells[0]; 
  numMixCells[1] = 2*numNonIdealCells[1] + 3*numNonIdealCells[2] + 
    4*numNonIdealCells[3] + 5*numNonIdealCells[4] + numIdealCells[1];
  numMixCells[2] = 3*numNonIdealCells[2] + 6*numNonIdealCells[3] + 
    10*numNonIdealCells[4] + numIdealCells[2];
  numMixCells[3] = 4*numNonIdealCells[3] + 10*numNonIdealCells[4] + 
    numIdealCells[3];
  numMixCells[4] = 5*numNonIdealCells[4];

  // number of relative dual cells, are dual to the standard CW-decomposition
  for (unsigned i=0; i<5; i++) numDualRelCells[i] = numStandardCells[4-i];

  // number of mixed relative cells -- each non-boundary (relative) cell
  //  gets multiplied appropriately..
  numMixRelCells[0] = numRelativeCells[0] + numRelativeCells[1] + 
   numRelativeCells[2] + numRelativeCells[3] + numRelativeCells[4]; 
  numMixRelCells[1] = 2*numRelativeCells[1] + 3*numRelativeCells[2] + 
    4*numRelativeCells[3] + 5*numRelativeCells[4];
  numMixRelCells[2] = 3*numRelativeCells[2] + 6*numRelativeCells[3] + 
    10*numRelativeCells[4];
  numMixRelCells[3] = 4*numRelativeCells[3] + 10*numRelativeCells[4];
  numMixRelCells[4] = 5*numRelativeCells[4];

  // number of mixed boundary cells
  numMixBdryCells[0] = numStandardBdryCells[0] + numStandardBdryCells[1] + 
    numStandardBdryCells[2] + numStandardBdryCells[3];
  numMixBdryCells[1] = 2*numStandardBdryCells[1] + 3*numStandardBdryCells[2] + 
    4*numStandardBdryCells[3];
  numMixBdryCells[2] = 3*numStandardBdryCells[2] + 4*numStandardBdryCells[3];
  numMixBdryCells[3] = 4*numStandardBdryCells[3];

  // number of boundary dual cells -- dual to std boundary cells
  for (unsigned i=0; i<4; i++) numDualBdryCells[i] = numStandardBdryCells[3-i];
}  


// only used in the CellularData constructor
void setupIndices(const Triangulation<3>* tri,   
 std::vector< std::vector<unsigned long> > &nicIx,  
 std::vector< std::vector<unsigned long> > &icIx, 
 std::vector< std::vector<unsigned long> > &dcIx,   
 std::vector< std::vector<unsigned long> > &bcIx, 
 std::vector< std::vector<unsigned long> > &rIx, 
 unsigned long numStandardCells[5],    unsigned long numDualCells[5], 
 unsigned long numMixCells[5],         unsigned long numStandardBdryCells[4], 
 unsigned long numNonIdealCells[5],    unsigned long numIdealCells[4], 
 unsigned long numNonIdealBdryCells[4],unsigned long numRelativeCells[5], 
 unsigned long numDualRelCells[5],     unsigned long numMixRelCells[5],  
 unsigned long numMixBdryCells[4],     unsigned long numDualBdryCells[4])
{
 // nicIx[0] interior or boundary, bcIx[0] boundary, dcIx[3] interior vertices. 
 for (Triangulation<3>::VertexIterator vit = tri->vertices().begin();
      vit != tri->vertices().end(); vit++) if ( !(*vit)->isIdeal() ) 
  {
   nicIx[0].push_back( (*vit)->index() );
   if ((*vit)->isBoundary()) bcIx[0].push_back( (*vit)->index() ); else
    { dcIx[3].push_back( (*vit)->index() );
      rIx[0].push_back( (*vit)->index() ); }
  } 
 // nicIx[1] all,  icIx[0] ideal ends, dcIx[2] nonboundary, bcIx[1] boundary
 for (Triangulation<3>::EdgeIterator eit = tri->edges().begin();
      eit != tri->edges().end(); eit++) 
  {
   nicIx[1].push_back( (*eit)->index() );
   if ((*eit)->isBoundary()) bcIx[1].push_back( (*eit)->index() );
   else 
   { dcIx[2].push_back( (*eit)->index() );
     rIx[1].push_back( (*eit)->index() );
     for (unsigned i=0;i<2;i++) if ((*eit)->vertex(i)->isIdeal()) 
     icIx[0].push_back(2*(*eit)->index()+i); 
   }
  }
 // nicIx[2] all, icIx[1] ideal ends, dcIx[1] nonboundary, bcIx[2] boundary
 for (Triangulation<3>::TriangleIterator fit = tri->triangles().begin();
      fit != tri->triangles().end(); fit++) 
 {
  nicIx[2].push_back( (*fit)->index() );
  if ((*fit)->isBoundary()) bcIx[2].push_back( (*fit)->index() );
  else
  { dcIx[1].push_back( (*fit)->index() );
    rIx[2].push_back( (*fit)->index() );
    for (unsigned i=0;i<3;i++) if ((*fit)->vertex(i)->isIdeal()) 
      icIx[1].push_back(3*(*fit)->index()+i); 
  }
 }
 // nicIx[3], icIx[2] ideal ends, dcIx[0] all
 for (Triangulation<3>::TetrahedronIterator tit = tri->tetrahedra().begin();
      tit != tri->tetrahedra().end(); tit++) 
 {
  nicIx[3].push_back( (*tit)->index() );
  dcIx[0].push_back( (*tit)->index() );
  rIx[3].push_back( (*tit)->index() );
  for (unsigned i=0;i<4;i++) if ((*tit)->vertex(i)->isIdeal()) 
  icIx[2].push_back(4*(*tit)->index()+i); 
 }

 // standard (0..3)-cells:
 for (unsigned i=0; i<3; i++) numStandardCells[i] = 
    nicIx[i].size() + icIx[i].size();
 numStandardCells[3] = nicIx[3].size();
 numStandardCells[4] = 0;
 // dual (0..3)-cells:
 for (unsigned i=0; i<4; i++) numDualCells[i] = dcIx[i].size();
 for (unsigned i=0; i<4; i++) numRelativeCells[i] = rIx[i].size();
 numDualCells[4] = 0; numRelativeCells[4] = 0;
 // boundary (0..3)-cells:
 for (unsigned i=0; i<3; i++) numStandardBdryCells[i] = 
    bcIx[i].size() + icIx[i].size();
 numStandardBdryCells[3] = 0;
 // ideal and non-ideal cells:
 for (unsigned i=0; i<4; i++) numNonIdealCells[i] = nicIx[i].size();
 numNonIdealCells[4] = 0;
 for (unsigned i=0; i<3; i++) numIdealCells[i] = icIx[i].size();
 numIdealCells[3] = 0;
 for (unsigned long i=0; i<3; i++) numNonIdealBdryCells[i] = bcIx[i].size();
 numNonIdealBdryCells[3] = 0;

 // mixed decomposition is proper cell decomposition induced by the barycentric
 // subdivision, so all previous internal/boundary standard cells contribute 
 // barycentres
 numMixCells[0] = numNonIdealCells[0] + numNonIdealCells[1] + 
    numNonIdealCells[2] + numNonIdealCells[3] + numIdealCells[0]; 
 numMixCells[1] = 2*numNonIdealCells[1] + 3*numNonIdealCells[2] + 
    4*numNonIdealCells[3] + numIdealCells[1];
 numMixCells[2] = 3*numNonIdealCells[2] + 6*numNonIdealCells[3] + 
    numIdealCells[2];
 numMixCells[3] = 4*numNonIdealCells[3];
 numMixCells[4] = 0;

 // number relative dual cells, these are dual to the standard CW-decomposition
 for (unsigned i=0; i<4; i++) numDualRelCells[i] = numStandardCells[3-i];

 // number of mixed relative cells -- each non-boundary (relative) cell 
 //  gets multiplied appropriately..
 numMixRelCells[0] = numRelativeCells[0] + numRelativeCells[1] + 
    numRelativeCells[2] + numRelativeCells[3]; 
 numMixRelCells[1] = 2*numRelativeCells[1] + 3*numRelativeCells[2] + 
    4*numRelativeCells[3];
 numMixRelCells[2] = 3*numRelativeCells[2] + 6*numRelativeCells[3];
 numMixRelCells[3] = 4*numRelativeCells[3];

 // number of mixed boundary cells
 numMixBdryCells[0] = numStandardBdryCells[0] + numStandardBdryCells[1] + 
    numStandardBdryCells[2];
 numMixBdryCells[1] = 2*numStandardBdryCells[1] + 3*numStandardBdryCells[2];
 numMixBdryCells[2] = 3*numStandardBdryCells[2];

 // number of boundary dual cells -- dual to std boundary cells
 for (unsigned i=0; i<3; i++) numDualBdryCells[i] = numStandardBdryCells[2-i];
}  


// required forward declaration
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
 std::vector< MatrixInt* > &schCM );


// constructor for 4-manifold triangulations
CellularData::CellularData(const Triangulation<4>& input): 
    tri4(new Triangulation<4>(input)), tri3(0), 
	nicIx(5), icIx(4), dcIx(5), bcIx(4), rIx(5), // indexing cells 
	sbiCM(4), strCM(5), schCM(4),   dbiCM(4), dtrCM(5), dchCM(4),   mbiCM(4), 
    mtrCM(5), mchCM(4), smCM(5), dmCM(5), smbCM(4), dmbCM(4), srmCM(5), drmCM(5) 
    // chain maps
//  normalsDim4BdryFaces(0),  normalsDim4BdryEdges(0), 
// normalsDim3BdryEdges(0), normalsDim3BdryVertices(0) // orientations
{
 setupIndices( tri4, nicIx, icIx, dcIx, bcIx, rIx, numStandardCells, 
    numDualCells, numMixCells, numStandardBdryCells, numNonIdealCells, 
    numIdealCells, numNonIdealBdryCells, numRelativeCells, numDualRelCells, 
    numMixRelCells, numMixBdryCells, numDualBdryCells );
 buildExtraNormalData();     
 buildMaximalTree(); 

 // the 9 core chain complexes for genCC
 fillStandardHomologyCC();   fillDualHomologyCC();      fillMixedHomologyCC();
 fillBoundaryHomologyCC();   fillRelativeHomologyCC();
 // maps of chain complexes.
 fillStandardToMixedHomCM(); // more to do...

 // todo fillMixedBdryHomology, fillDualBdryHomology, 
 //  fillMixRelBdryHomology, fillDualRelBdryHomology
 fillChainMaps( tri3, tri4, numStandardCells, numDualCells, numMixCells, 
  numStandardBdryCells, numNonIdealCells, numIdealCells, numNonIdealBdryCells,   
  numRelativeCells, numDualRelCells, numMixRelCells, numMixBdryCells, 
  numDualBdryCells, nicIx, icIx, dcIx, bcIx,  rIx, 
  sbiCM, smCM, dmCM, strCM, schCM );
}

// constructor for 3-manifold triangulations
CellularData::CellularData(const Triangulation<3>& input): 
 tri4(0), tri3(new Triangulation<3>(input)), nicIx(4), icIx(3), dcIx(4), bcIx(3), 
 rIx(4), sbiCM(3), strCM(4), schCM(3),   dbiCM(3), dtrCM(4), dchCM(3),   
 mbiCM(3), mtrCM(4), mchCM(3), smCM(4), dmCM(4), smbCM(3), dmbCM(3), 
 srmCM(4), drmCM(4) // chain maps
//  normalsDim4BdryFaces(0),  normalsDim4BdryEdges(0), normalsDim3BdryEdges(0), 
// normalsDim3BdryVertices(0) // orientations
{
 setupIndices( tri3, nicIx, icIx, dcIx, bcIx, rIx, numStandardCells, 
    numDualCells, numMixCells, numStandardBdryCells, numNonIdealCells, 
    numIdealCells, numNonIdealBdryCells, numRelativeCells, numDualRelCells, 
    numMixRelCells, numMixBdryCells, numDualBdryCells );
 buildExtraNormalData();     buildMaximalTree(); 

 // the 9 core chain complexes for genCC
 fillStandardHomologyCC();   fillDualHomologyCC();      fillMixedHomologyCC();
 fillBoundaryHomologyCC();   fillRelativeHomologyCC();
 // todo fillMixedBdryHomology, fillDualBdryHomology, fillMixRelBdryHomology, 
 //  fillDualRelBdryHomology
 // chain maps
 fillStandardToMixedHomCM(); // more to do...

 fillChainMaps( tri3, tri4, numStandardCells, numDualCells, numMixCells, 
    numStandardBdryCells, numNonIdealCells, numIdealCells, numNonIdealBdryCells, 
	numRelativeCells, numDualRelCells, numMixRelCells, numMixBdryCells, 
    numDualBdryCells, nicIx, icIx, dcIx, bcIx,  rIx, 
    sbiCM, smCM, dmCM, strCM, schCM );
}


} // namespace regina

