
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

#include <map>
#include <list>
#include <cmath>

#include <iostream>
#include <sstream>

namespace regina {

/*
void dumpMat( NMatrixRing<NLargeInteger> mat )
{
for (unsigned long j=0; j<mat.columns(); j++)
 {
 std::cout<<"[";
 for (unsigned long i=0; i<mat.rows(); i++)
  std::cout<<mat.entry(i,j)<<" ";
 std::cout<<"]\n";
 }
}
*/

bool NCellularData::GroupLocator::operator<(const GroupLocator &rhs) const
{
if (var < rhs.var) return true; if (var > rhs.var) return false;
if (dim < rhs.dim) return true; if (dim > rhs.dim) return false;
if (cof < rhs.cof) return true; if (cof > rhs.cof) return false;
if (hcs < rhs.hcs) return true; if (hcs > rhs.hcs) return false;
return false;
}

bool NCellularData::GroupLocator::operator==(const GroupLocator &rhs) const
{ return ( (var==rhs.var) && (dim == rhs.dim) && (cof == rhs.cof) && (hcs == rhs.hcs) ); }

bool NCellularData::GroupLocator::operator!=(const GroupLocator &rhs) const
{ return ( (var!=rhs.var) || (dim != rhs.dim) || (cof != rhs.cof) || (hcs != rhs.hcs) ); }

bool NCellularData::HomLocator::operator<(const HomLocator &rhs) const
{
if ( domain < rhs.domain ) return true; if ( rhs.domain < domain ) return false; 
if ( range < rhs.range ) return true; if ( rhs.range < range ) return false;
return false;
}

bool NCellularData::HomLocator::operator==(const HomLocator &rhs) const
{ return ( (domain==rhs.domain) && (range == rhs.range) ); }

bool NCellularData::HomLocator::operator!=(const HomLocator &rhs) const
{ return ( (domain!=rhs.domain) || (range != rhs.range) ); }

bool NCellularData::FormLocator::operator<(const FormLocator &rhs) const
{
if ( ft < rhs.ft ) return true;           if ( rhs.ft < ft ) return false;
if ( ldomain < rhs.ldomain ) return true; if ( rhs.ldomain < ldomain ) return false; 
if ( rdomain < rhs.rdomain ) return true; if ( rhs.rdomain < rdomain ) return false;
return false;
}

bool NCellularData::FormLocator::operator==(const FormLocator &rhs) const
{ return ( (ft == rhs.ft) && (ldomain==rhs.ldomain) && (rdomain == rhs.rdomain) ); }

bool NCellularData::FormLocator::operator!=(const FormLocator &rhs) const
{ return ( (ft != rhs.ft) || (ldomain!=rhs.ldomain) || (rdomain != rhs.rdomain) ); }

// only used in the NCellularData constructor
void setupIndices(const Dim4Triangulation* tri,   
 std::vector< std::vector<unsigned long> > &nicIx,  std::vector< std::vector<unsigned long> > &icIx, 
 std::vector< std::vector<unsigned long> > &dcIx,   std::vector< std::vector<unsigned long> > &bcIx,
 std::vector< std::vector<unsigned long> > &rIx,  
 unsigned long numStandardCells[5],    unsigned long numDualCells[5], 
 unsigned long numMixCells[5],         unsigned long numStandardBdryCells[4], 
 unsigned long numNonIdealCells[5],    unsigned long numIdealCells[4], 
 unsigned long numNonIdealBdryCells[4],unsigned long numRelativeCells[5])
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
        if ((*fit)->isBoundary()) bcIx[2].push_back( tri->faceIndex(*fit) );
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

    // standard (0..4)-cells:
    for (unsigned i=0; i<4; i++) numStandardCells[i] = nicIx[i].size() + icIx[i].size();
    numStandardCells[4] = nicIx[4].size();
    // dual (0..4)-cells:
    for (unsigned i=0; i<5; i++) numDualCells[i] = dcIx[i].size();
    for (unsigned i=0; i<5; i++) numRelativeCells[i] = rIx[i].size();
    // boundary (0..3)-cells:
    for (unsigned i=0; i<4; i++) numStandardBdryCells[i] = bcIx[i].size() + icIx[i].size();
    // ideal and non-ideal cells:
    for (unsigned i=0; i<5; i++) numNonIdealCells[i] = nicIx[i].size();
    for (unsigned i=0; i<4; i++) numIdealCells[i] = icIx[i].size();
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
}  

// only used in the NCellularData constructor
void setupIndices(const NTriangulation* tri,   
 std::vector< std::vector<unsigned long> > &nicIx,  std::vector< std::vector<unsigned long> > &icIx, 
 std::vector< std::vector<unsigned long> > &dcIx,   std::vector< std::vector<unsigned long> > &bcIx, 
 std::vector< std::vector<unsigned long> > &rIx, 
 unsigned long numStandardCells[5],    unsigned long numDualCells[5], 
 unsigned long numMixCells[5],         unsigned long numStandardBdryCells[4], 
 unsigned long numNonIdealCells[5],    unsigned long numIdealCells[4], 
 unsigned long numNonIdealBdryCells[4],unsigned long numRelativeCells[5])
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
    const Dim4Edge* edg;  const Dim4Face* fac; const Dim4Tetrahedron* tet; const Dim4Pentachoron* pen;
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
    const NEdge* edg;  const NFace* fac; const NTetrahedron* tet; 
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
    const Dim4Vertex* vrt;  const Dim4Edge* edg;  const Dim4Face* fac; 
	const Dim4Tetrahedron* tet; const Dim4Pentachoron* pen;
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
    const NVertex* vrt;  const NEdge* edg;  const NFace* fac; const NTetrahedron* tet; 
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
    const Dim4Vertex* vrt;  const Dim4Edge* edg;  const Dim4Face* fac; 
	const Dim4Tetrahedron* tet; const Dim4Pentachoron* pen;
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
	   tet = pen->getTetrahedron( edginc[i] ); NPerm5 tetinc( pen->getTetrahedronMapping( edginc[i] ) ); // tet index wrong?
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
    const NVertex* vrt;  const NEdge* edg;  const NFace* fac; const NTetrahedron* tet; 
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
	std::vector< NMatrixInt* > &bsCC)
{
    // initialize chain complex matrices.
    for (unsigned i=1; i<4; i++) // bsCC[i]
        bsCC[i] = new NMatrixInt(numStandardBdryCells[i-1], numStandardBdryCells[i]);
    bsCC[0] = new NMatrixInt(1, numStandardBdryCells[0]);
    bsCC[4] = new NMatrixInt(numStandardBdryCells[3], 1);

    // various useful pointers, index holders.
    const Dim4Edge* edg;  const Dim4Face* fac; const Dim4Tetrahedron* tet; const Dim4Pentachoron* pen;
    unsigned long I;

    // now we fill them out, first bsCC.  bsCC[0] is zero, 
    unsigned long D=1; // bsCC[D]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // bsCC[D]->entry( *,j )
	{ // endpts getEdge(nicIx[D][j]) ideal?
	edg = tri->getEdge(bcIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->getVertex(i)->isIdeal())
	 {   // endpt i is ideal, find index
          I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	  bsCC[D]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
	 } 
	else // endpt i is not ideal
	 {
          I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->vertexIndex( edg->getVertex(i) ) )
	      - bcIx[D-1].begin();
	  bsCC[D]->entry(I, j) += ( i == 0 ? -1 : 1 );
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // bsCC[D]->entry( *, numNonIdealCells[D] + j )
        { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
	fac = tri->getFace(icIx[D][j]/(D+2));
	for (unsigned long i=1; i<D+2; i++)
	 {
          NPerm5 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  unsigned long iX( (D+1)*tri->edgeIndex( fac->getEdge( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  bsCC[D]->entry(numNonIdealBdryCells[D-1] + I,  numNonIdealBdryCells[D] + j) -= P.sign();
	 }
	}

    D = 2; // bsCC[2]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // bsCC[D]->entry( *,j )
	{
	fac = tri->getFace(bcIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (fac->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), (D+1)*j+i ) - bcIx[D-1].begin();
	    bsCC[D]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
	   } // standard face boundaries
	  NPerm5 P( fac->getEdgeMapping(i) );
          I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->edgeIndex( fac->getEdge(i) )) 
		- bcIx[D-1].begin();
	  bsCC[D]->entry(I, j) += P.sign();
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) // bsCC[D]->entry( *, j+numNonIdealCells[D-1] )
        { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
	tet = tri->getTetrahedron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm5 P( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	  unsigned long iX( (D+1)*tri->faceIndex( tet->getFace( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  bsCC[D]->entry(numNonIdealBdryCells[D-1] + I,  numNonIdealBdryCells[D] + j) -= P.sign(); 
	 }
	}

    D = 3; // bsCC[3]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // bsCC[D]->entry( *,j )
	{
	tet = tri->getTetrahedron(bcIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (tet->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	    bsCC[D]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
	   } // standard face boundaries
	  NPerm5 P( tet->getFaceMapping(i) );
          I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->faceIndex( tet->getFace(i) )) 
		- bcIx[D-1].begin();
	  bsCC[D]->entry(I, j) += P.sign();
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // bsCC[D]->entry( *, j+numNonIdealCells[D-1] )
        { // icIx[D][j]/(D+2) pentachoron icIx[1][j] % (D+2) vertex
	pen = tri->getPentachoron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm5 P( pen->getTetrahedronMapping( (icIx[D][j] + i) % (D+2)) );
	  unsigned long iX( (D+1)*tri->tetrahedronIndex( pen->getTetrahedron( (icIx[D][j] + i) % (D+2) ) ) 
			            + ( P.preImageOf(icIx[D][j] % (D+2)) )     );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  bsCC[D]->entry(numNonIdealBdryCells[D-1] + I,  numNonIdealBdryCells[D] + j) -= P.sign(); 
	 }
	}
}

void fillBoundaryHomologyCC(const NTriangulation* tri, 
	const unsigned long numStandardBdryCells[4],  const unsigned long numIdealCells[4],
        const unsigned long numNonIdealBdryCells[4],  
	const std::vector< std::vector< unsigned long > > &bcIx, const std::vector< std::vector< unsigned long > > &icIx, 
	std::vector< NMatrixInt* > &bsCC)
{
    // initialize chain complex matrices.
    for (unsigned i=1; i<3; i++) // bsCC[i]
        bsCC[i] = new NMatrixInt(numStandardBdryCells[i-1], numStandardBdryCells[i]);
    bsCC[0] = new NMatrixInt(1, numStandardBdryCells[0]);
    bsCC[3] = new NMatrixInt(numStandardBdryCells[2], 1);

    // various useful pointers, index holders.
    const NEdge* edg;  const NFace* fac; const NTetrahedron* tet; 
    unsigned long I;
    
    // now we fill them out, first bsCC.  bsCC[0] is zero, 
    unsigned long D=1; // bsCC[D]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // bsCC[D]->entry( *,j )
	{ // endpts getEdge(bcIx[D][j]) ideal?
	edg = tri->getEdge(bcIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if (edg->getVertex(i)->isIdeal())
	 {   // endpt i is ideal, find index
          I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	  bsCC[D]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
	 } 
	else // endpt i is not ideal
	 {
          I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->vertexIndex( edg->getVertex(i) ) )
	      - bcIx[D-1].begin();
	  bsCC[D]->entry(I, j) += ( i == 0 ? -1 : 1 );
	 }
	}
    for (unsigned long j=0; j<numIdealCells[D]; j++) // bsCC[D]->entry( *, numNonIdealBdryCells[D] + j )
        { // icIx[D][j]/(D+2) face icIx[D][j] % (D+2) vertex
	fac = tri->getFace(icIx[D][j]/(D+2));
	for (unsigned long i=1; i<D+2; i++)
	 {
          NPerm4 P( fac->getEdgeMapping( (icIx[D][j] + i) % (D+2) ) );
	  unsigned long iX( (D+1)*tri->edgeIndex( fac->getEdge( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  bsCC[D]->entry(numNonIdealBdryCells[D-1] + I,  numNonIdealBdryCells[D] + j) -= P.sign();
	 }
	}

    D = 2; // bsCC[2]
    for (unsigned long j=0; j<numNonIdealBdryCells[D]; j++) // bsCC[D]->entry( *,j )
	{
	fac = tri->getFace(bcIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) 
	 { 
	  if (fac->getVertex(i)->isIdeal())
	   { // ideal ends of faces	
            I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), (D+1)*j+i ) - icIx[D-1].begin();
	    bsCC[D]->entry(numNonIdealBdryCells[D-1] + I, j) += 1;
	   } // standard face boundaries
	  NPerm4 P( fac->getEdgeMapping(i) );
          I = lower_bound( bcIx[D-1].begin(), bcIx[D-1].end(), tri->edgeIndex( fac->getEdge(i) )) 
		- bcIx[D-1].begin();
	  bsCC[D]->entry(I, j) += P.sign();
	 }
	}
   for (unsigned long j=0; j<numIdealCells[D]; j++) // bsCC[D]->entry( *, j+numNonIdealBdryCells[D-1] )
        { // icIx[D][j]/(D+2) tetrahedron icIx[1][j] % (D+2) vertex
	tet = tri->getTetrahedron(icIx[D][j]/(D+2));
	for (unsigned long i=1; i < D+2; i++)
	 {
          NPerm4 P( tet->getFaceMapping( (icIx[D][j] + i) % (D+2)) );
	  unsigned long iX( (D+1)*tri->faceIndex( tet->getFace( (icIx[D][j] + i) % (D+2) ) ) // of corresp ideal 0-cell
			            + ( P.preImageOf(icIx[D][j] % (D+2)) ) );
	  I = lower_bound( icIx[D-1].begin(), icIx[D-1].end(), iX ) - icIx[D-1].begin();
	  bsCC[D]->entry(numNonIdealBdryCells[D-1] + I,  numNonIdealBdryCells[D] + j) -= P.sign(); 
	 }
	}
}

void fillRelativeHomologyCC(const Dim4Triangulation* tri, 
	const unsigned long numRelativeCells[5], const std::vector< std::vector< unsigned long > > &rIx, 
	std::vector< NMatrixInt* > &rCC)
{
    // initialize chain complex matrices.
    for (unsigned i=1; i<5; i++) // rCC[i] defined for i == 0, ..., 5
	rCC[i] = new NMatrixInt(numRelativeCells[i-1], numRelativeCells[i]);
    rCC[0] = new NMatrixInt(1, numRelativeCells[0]);
    rCC[5] = new NMatrixInt(numRelativeCells[4], 1);

    // various useful pointers, index holders.
    const Dim4Edge* edg;  const Dim4Face* fac; const Dim4Tetrahedron* tet; const Dim4Pentachoron* pen;
    unsigned long I;
    
    // now we fill them out, first rCC.  rCC[0] is zero, 
    unsigned long D=1; // rCC[D]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // rCC[D]->entry( *,j )
	{ // endpts getEdge(rIx[D][j]) ideal?
	edg = tri->getEdge(rIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if ( (!edg->getVertex(i)->isIdeal()) && (!edg->getVertex(i)->isBoundary()) )
	 {
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->vertexIndex( edg->getVertex(i) ) )
	      - rIx[D-1].begin();
	  rCC[D]->entry(I, j) += ( i == 0 ? -1 : 1 );
	 }
	}

    D = 2; // rCC[2]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // rCC[D]->entry( *,j )
	{
	fac = tri->getFace(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!fac->getEdge(i)->isBoundary())
	 { 
	  NPerm5 P( fac->getEdgeMapping(i) );
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->edgeIndex( fac->getEdge(i) )) 
		- rIx[D-1].begin();
	  rCC[D]->entry(I, j) += P.sign();
	 }
	}

    D = 3; // rCC[3]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // rCC[D]->entry( *,j )
	{
	tet = tri->getTetrahedron(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!tet->getFace(i)->isBoundary())
	 { 
	  NPerm5 P( tet->getFaceMapping(i) );
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->faceIndex( tet->getFace(i) )) 
		- rIx[D-1].begin();
	  rCC[D]->entry(I, j) += P.sign();
	 }
	}

    D = 4; // rCC[4]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // rCC[D]->entry( *,j )
	{
	pen = tri->getPentachoron(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!pen->getTetrahedron(i)->isBoundary())
	 { 
	  NPerm5 P( pen->getTetrahedronMapping(i) );
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->tetrahedronIndex( pen->getTetrahedron(i) )) 
		- rIx[D-1].begin();
	  rCC[D]->entry(I, j) += P.sign();
	 }
	}
}

void fillRelativeHomologyCC(const NTriangulation* tri, 
	const unsigned long numRelativeCells[5], const std::vector< std::vector< unsigned long > > &rIx, 
	std::vector< NMatrixInt* > &rCC)
{
    // initialize chain complex matrices.
    for (unsigned i=1; i<4; i++) // rCC[i] defined for i == 0, ..., 5
	rCC[i] = new NMatrixInt(numRelativeCells[i-1], numRelativeCells[i]);
    rCC[0] = new NMatrixInt(1, numRelativeCells[0]);
    rCC[4] = new NMatrixInt(numRelativeCells[3], 1);

    // various useful pointers, index holders.
    const NEdge* edg;  const NFace* fac; const NTetrahedron* tet;
    unsigned long I;
    
    // now we fill them out, first rCC.  rCC[0] is zero, 
    unsigned long D=1; // rCC[D]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // rCC[D]->entry( *,j )
	{ // endpts getEdge(rIx[D][j]) ideal?
	edg = tri->getEdge(rIx[D][j]);
	for (unsigned long i=0; i<D+1; i++) if ( (!edg->getVertex(i)->isIdeal()) && (!edg->getVertex(i)->isBoundary()) )
	 {
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->vertexIndex( edg->getVertex(i) ) )
	      - rIx[D-1].begin();
	  rCC[D]->entry(I, j) += ( i == 0 ? -1 : 1 );
	 }
	}

    D = 2; // rCC[2]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // rCC[D]->entry( *,j )
	{
	fac = tri->getFace(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!fac->getEdge(i)->isBoundary())
	 { 
	  NPerm4 P( fac->getEdgeMapping(i) );
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->edgeIndex( fac->getEdge(i) )) 
		- rIx[D-1].begin();
	  rCC[D]->entry(I, j) += P.sign();
	 }
	}

    D = 3; // rCC[3]
    for (unsigned long j=0; j<numRelativeCells[D]; j++) // rCC[D]->entry( *,j )
	{
	tet = tri->getTetrahedron(rIx[D][j]);
	for (unsigned long i=0; i < D+1; i++) if (!tet->getFace(i)->isBoundary())
	 { 
	  NPerm4 P( tet->getFaceMapping(i) );
          I = lower_bound( rIx[D-1].begin(), rIx[D-1].end(), tri->faceIndex( tet->getFace(i) )) 
		- rIx[D-1].begin();
	  rCC[D]->entry(I, j) += P.sign();
	 }
	}
}


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
 const Dim4Vertex* vrt;  const Dim4Edge* edg;  const Dim4Face* fac; 
   const Dim4Tetrahedron* tet; const Dim4Pentachoron* pen;
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
 const NVertex* vrt;  const NEdge* edg;  const NFace* fac; 
   const NTetrahedron* tet; 
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
	std::vector< std::vector<unsigned long> > &nicIx, 	 std::vector< std::vector<unsigned long> > &rIx, 
        std::vector< NMatrixInt* > &s_rCM)
{
 for (unsigned d=0; d<aDim+1; d++) s_rCM[d] = new NMatrixInt( numRelativeCells[d], numStandardCells[d] );

 unsigned long I;
 for (unsigned long d=0; d<aDim+1; d++) for (unsigned j=0; j<numNonIdealCells[d]; j++)
  {
   I = lower_bound( rIx[d].begin(), rIx[d].end(), nicIx[d][j] ) - rIx[d].begin();
   if ( I != rIx[d].size() ) s_rCM[d]->entry( I, j ) = 1;
  }
}

// H_{d+1}(M, \partial M) --> H_d(\partial M)
void fillDifferentialHomCM( const Dim4Triangulation* tri,  const unsigned long numRelativeCells[5], 
     const unsigned long numStandardBdryCells[4],      const unsigned long numNonIdealBdryCells[4],    
     std::vector< std::vector<unsigned long> > &bcIx,  std::vector< std::vector<unsigned long> > &icIx,
     std::vector< std::vector<unsigned long> > &rIx,   std::vector< NMatrixInt* > &rbCM)               
{

 for (unsigned d=0; d<4; d++) 
   rbCM[d] = new NMatrixInt( numStandardBdryCells[d], numRelativeCells[d+1] );

 unsigned long I;
 // various useful pointers, index holders.
 const Dim4Edge* edg;  const Dim4Face* fac; const Dim4Tetrahedron* tet; const Dim4Pentachoron* pen;
 // boundary relative 1-cells
 unsigned long D=1;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {	
   edg = tri->getEdge(rIx[D][j]);
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
   fac = tri->getFace(rIx[D][j]);
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
   tet = tri->getTetrahedron(rIx[D][j]);
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
   pen = tri->getPentachoron(rIx[D][j]);
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
     std::vector< std::vector<unsigned long> > &rIx,   std::vector< NMatrixInt* > &rbCM)               
{
 for (unsigned d=0; d<3; d++) rbCM[d] = new NMatrixInt( numStandardBdryCells[d], numRelativeCells[d+1] );

 unsigned long I;
 // various useful pointers, index holders.
 const NEdge* edg;  const NFace* fac; const NTetrahedron* tet; 

 // boundary relative 1-cells
 unsigned long D=1;
 for (unsigned long j=0; j<numRelativeCells[D]; j++)
  {	
   edg = tri->getEdge(rIx[D][j]);
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
   fac = tri->getFace(rIx[D][j]);
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
   tet = tri->getTetrahedron(rIx[D][j]);
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
	{ // rIx[aDim][i], dcIx[0][i]
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
	{ // rIx[aDim][i], dcIx[0][i]
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

// constructors

// constructor for 4-manifold triangulations
NCellularData::NCellularData(const Dim4Triangulation& input): ShareableObject(),
        tri4(new Dim4Triangulation(input)), tri3(0), 
	nicIx(5), icIx(4), dcIx(5), bcIx(4), rIx(5), // indexing cells 
	sCC(6), dCC(6), mCC(6), bsCC(5), rCC(6), 
	bs_sCM(4), s_mCM(5), d_mCM(5), s_rCM(5), rbCM(4) // chain complexes and maps
{
   setupIndices( tri4, nicIx, icIx, dcIx, bcIx, rIx, numStandardCells, numDualCells, numMixCells, 
			numStandardBdryCells, numNonIdealCells, numIdealCells, numNonIdealBdryCells, 
			numRelativeCells );
        
   fillStandardHomologyCC( tri4, numStandardCells, numNonIdealCells, numIdealCells, 
			nicIx, icIx, sCC);

   fillDualHomologyCC( tri4, numDualCells, dcIx, dCC );

   fillMixedHomologyCC( tri4, numMixCells, numNonIdealCells, numIdealCells, icIx, nicIx, mCC );
 
   fillBoundaryHomologyCC( tri4, numStandardBdryCells, numIdealCells, numNonIdealBdryCells, bcIx, icIx, bsCC );

   fillRelativeHomologyCC( tri4, numRelativeCells, rIx, rCC );

   fillStandardToMixedHomCM( 4, numStandardCells, numMixCells, numIdealCells, numNonIdealCells, s_mCM );

   fillDualToMixedHomCM( tri4, numDualCells, numMixCells, numNonIdealCells, dcIx, d_mCM );

   fillStandardToRelativeHomCM( 4, numStandardCells, numRelativeCells, numNonIdealCells, nicIx, rIx, s_rCM );

   fillBoundaryToStandardHomCM( 4, numStandardCells, numStandardBdryCells, numNonIdealBdryCells, 
        numIdealCells, numNonIdealCells, nicIx, bcIx, bs_sCM);

   fillDifferentialHomCM( tri4, numRelativeCells, numStandardBdryCells,  numNonIdealBdryCells,    
     bcIx,  icIx, rIx, rbCM );   

}

// constructor for 3-manifold triangulations
NCellularData::NCellularData(const NTriangulation& input): ShareableObject(),
        tri4(0), tri3(new NTriangulation(input)), 
	nicIx(4), icIx(3), dcIx(4), bcIx(3), rIx(4), // indexing cells 
	sCC(5), dCC(5), mCC(5), bsCC(4), rCC(5), 
        bs_sCM(3), s_mCM(4), d_mCM(4), s_rCM(4), rbCM(3) // chain complexes and maps
{
   setupIndices( tri3, nicIx, icIx, dcIx, bcIx, rIx, numStandardCells, numDualCells, numMixCells, 
		 numStandardBdryCells, numNonIdealCells, numIdealCells, numNonIdealBdryCells,
		 numRelativeCells );
        
   fillStandardHomologyCC( tri3, numStandardCells, numNonIdealCells, numIdealCells, 
			nicIx, icIx, sCC);

   fillDualHomologyCC( tri3, numDualCells, dcIx, dCC );

   fillMixedHomologyCC( tri3, numMixCells, numNonIdealCells, numIdealCells, icIx, nicIx, mCC );

   fillBoundaryHomologyCC( tri3, numStandardBdryCells, numIdealCells, numNonIdealBdryCells, bcIx, icIx, bsCC );

   fillRelativeHomologyCC( tri3, numRelativeCells, rIx, rCC );

   fillStandardToMixedHomCM( 3, numStandardCells, numMixCells, numIdealCells, numNonIdealCells, s_mCM );

   fillDualToMixedHomCM( tri3, numDualCells, numMixCells, numNonIdealCells, dcIx, d_mCM );

   fillStandardToRelativeHomCM( 3, numStandardCells, numRelativeCells, numNonIdealCells, nicIx, rIx, s_rCM );

   fillBoundaryToStandardHomCM( 3, numStandardCells, numStandardBdryCells, numNonIdealBdryCells, 
        numIdealCells, numNonIdealCells, nicIx, bcIx, bs_sCM);
 
   fillDifferentialHomCM( tri3, numRelativeCells, numStandardBdryCells,  numNonIdealBdryCells,    
     bcIx,  icIx, rIx, rbCM );   
}

bool NCellularData::chainComplexesVerified() const
{
for (unsigned long i=0; i<sCC.size()-1; i++) if (sCC[i] && sCC[i+1])
  {
   if ( sCC[i]->columns() != sCC[i+1]->rows() ) return false;
   std::auto_ptr< NMatrixRing<NLargeInteger> > prod = (*sCC[i])*(*sCC[i+1]);
   for (unsigned long j=0; j<prod->rows(); j++) for (unsigned long k=0; k<prod->columns(); k++)
	if (prod->entry(j,k) != 0) return false; 
  }
for (unsigned long i=0; i<dCC.size()-1; i++) if (dCC[i] && dCC[i+1])
  {
   if ( dCC[i]->columns() != dCC[i+1]->rows() ) return false;
   std::auto_ptr< NMatrixRing<NLargeInteger> > prod = (*dCC[i])*(*dCC[i+1]);
   for (unsigned long j=0; j<prod->rows(); j++) for (unsigned long k=0; k<prod->columns(); k++)
	if (prod->entry(j,k) != 0) return false; 
  }
for (unsigned long i=0; i<mCC.size()-1; i++) if (mCC[i] && mCC[i+1])
  {
   if ( mCC[i]->columns() != mCC[i+1]->rows() ) return false;
   std::auto_ptr< NMatrixRing<NLargeInteger> > prod = (*mCC[i])*(*mCC[i+1]);
   for (unsigned long j=0; j<prod->rows(); j++) for (unsigned long k=0; k<prod->columns(); k++)
       if (prod->entry(j,k) != 0) return false;
  }
for (unsigned long i=0; i<bsCC.size()-1; i++) if (bsCC[i] && bsCC[i+1])
  {
   if ( bsCC[i]->columns() != bsCC[i+1]->rows() ) return false;
   std::auto_ptr< NMatrixRing<NLargeInteger> > prod = (*bsCC[i])*(*bsCC[i+1]);
   for (unsigned long j=0; j<prod->rows(); j++) for (unsigned long k=0; k<prod->columns(); k++)
       if (prod->entry(j,k) != 0) return false;
  }
for (unsigned long i=0; i<rCC.size()-1; i++) if (rCC[i] && rCC[i+1])
  {
   if ( rCC[i]->columns() != rCC[i+1]->rows() ) return false;
   std::auto_ptr< NMatrixRing<NLargeInteger> > prod = (*rCC[i])*(*rCC[i+1]);
   for (unsigned long j=0; j<prod->rows(); j++) for (unsigned long k=0; k<prod->columns(); k++)
       if (prod->entry(j,k) != 0) return false;
  }
return true;
}

bool NCellularData::chainMapsVerified() const
{
// verify mCC[i]*s_mCM[i] == s_mCM[i-1]*sCC[i]
for (unsigned long i=1; i<s_mCM.size(); i++) if (s_mCM[i] && s_mCM[i-1] && mCC[i] && sCC[i])
 {
  if ( (mCC[i]->columns() != s_mCM[i]->rows()) || (s_mCM[i-1]->columns() != sCC[i]->rows()) ) return false;
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod1 = (*mCC[i])*(*s_mCM[i]);
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod2 = (*s_mCM[i-1])*(*sCC[i]);
  if ( (*prod1) != (*prod2) ) return false; 
 }
// verify mCC[i]*d_mCM[i] == d_mCM[i-1]*dCC[i]
for (unsigned long i=1; i<d_mCM.size(); i++) if (d_mCM[i] && d_mCM[i-1] && mCC[i] && dCC[i])
 {
  if ( (mCC[i]->columns() != d_mCM[i]->rows()) || (d_mCM[i-1]->columns() != dCC[i]->rows()) ) return false;
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod1 = (*mCC[i])*(*d_mCM[i]);
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod2 = (*d_mCM[i-1])*(*dCC[i]);
  if ( (*prod1) != (*prod2) ) return false; 
 }
// verify rCC[i]*s_rCM[i] == s_rCM[i-1]*sCC[i]
for (unsigned long i=1; i<s_rCM.size(); i++) if (s_rCM[i] && s_rCM[i-1] && sCC[i] && rCC[i])
 {
  if ( (rCC[i]->columns() != s_rCM[i]->rows()) || (s_rCM[i-1]->columns() != sCC[i]->rows()) ) return false;
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod1 = (*rCC[i])*(*s_rCM[i]);
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod2 = (*s_rCM[i-1])*(*sCC[i]);
  if ( (*prod1) != (*prod2) ) return false; 
 }
// verify sCC[i]*bs_sCM[i] == bs_sCM[i-1]*bsCC[i]
for (unsigned long i=1; i<bs_sCM.size(); i++) if (bs_sCM[i] && bs_sCM[i-1] && sCC[i] && bsCC[i])
 {
  if ( (sCC[i]->columns() != bs_sCM[i]->rows()) || (bs_sCM[i-1]->columns() != bsCC[i]->rows()) ) return false;
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod1 = (*sCC[i])*(*bs_sCM[i]);
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod2 = (*bs_sCM[i-1])*(*bsCC[i]);
  if ( (*prod1) != (*prod2) ) return false; 
 }
// verify bsCC[i]*rbCM[i] == (-1)*rbCM[i-1]*rCC[i+1]
for (unsigned long i=1; i<rbCM.size(); i++) if (rbCM[i] && rbCM[i-1] && rCC[i+1] && bsCC[i])
 { 
  if ( (bsCC[i]->columns() != rbCM[i]->rows()) || (rbCM[i-1]->columns() != rCC[i+1]->rows()) ) return false;
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod1 = (*bsCC[i])*(*rbCM[i]);
  std::auto_ptr< NMatrixRing<NLargeInteger> > prod2 = (*rbCM[i-1])*(*rCC[i+1]);
  for (unsigned long j=0; j<prod1->rows(); j++) for (unsigned long k=0; k<prod1->columns(); k++)
	if (prod1->entry(j,k) + prod2->entry(j,k) != 0) return false;
 }
return true;
}

bool NCellularData::coordinateIsomorphismsVerified(variance_type var, unsigned long coef) const
{
unsigned long aDim = ( tri3 ? 3 : 4 );
// dual to mixed, homology
for (unsigned long i=0; i<=aDim; i++)
 {
  GroupLocator dom(i, var, (var == coVariant ? STD_coord : MIX_coord), coef);
  GroupLocator ran(i, var, (var == coVariant ? MIX_coord : STD_coord), coef);
  if (!homGroup( HomLocator(dom, ran) )->isIsomorphism()) return false;
 }
// standard to mixed, homology
for (unsigned long i=0; i<=aDim; i++)
 {
  GroupLocator dom(i, var, (var == coVariant ? DUAL_coord : MIX_coord), coef);
  GroupLocator ran(i, var, (var == coVariant ? MIX_coord : DUAL_coord), coef);
  if (!homGroup( HomLocator(dom, ran) )->isIsomorphism()) return false;
 }
return true;
}

bool NCellularData::homologyLESVerified(variance_type var, unsigned long coef) const
{
unsigned long aDim = ( tri3 ? 3 : 4 );
// exactness at H_i M:                   H_i (\partial M) --> H_i M --> H_i(M,\partial M),       i == 0, ..., aDim-1
for (unsigned long i=0; i<aDim; i++) //  H^i (\partial M) <-- H^i M <-- H^i(M,\partial M)
 {
  GroupLocator middleG( i, var, STD_coord, coef );
  GroupLocator rightG( i, var, (var == coVariant ? STD_REL_BDRY_coord : STD_BDRY_coord), coef);
  GroupLocator leftG( i, var, (var == coVariant ? STD_BDRY_coord : STD_REL_BDRY_coord), coef);
  HomLocator secondMapLoc( middleG, rightG );
  HomLocator firstMapLoc( leftG, middleG );
  const NHomMarkedAbelianGroup secondMap(*homGroup(secondMapLoc));
  const NHomMarkedAbelianGroup firstMap(*homGroup(firstMapLoc));
  if (!(secondMap*firstMap).isZero()) return false;
  if (!(secondMap.getKernel().isIsomorphicTo( firstMap.getImage() ) ) ) return false;
  if ( (i==0) && (var==coVariant) ) if (!secondMap.isEpic()) return false;  // rightmost term in LES, covariant case
  if ( (i==0) && (var==contraVariant) ) if (!firstMap.isMonic()) return false; // leftmost in LES, contravariant case
 } 
// exactness at H_i(\partial M):          H_i(M,\partial M) --> H_{i-1} \partial M --> H_{i-1} M, i == 1, ..., aDim
for (unsigned long i=1; i<=aDim; i++) //  H^i(M,\partial M) <-- H^{i-1} \partial M <-- H^{i-1} M
 {
  GroupLocator middleG( i-1, var, STD_BDRY_coord, coef );
  GroupLocator rightG( (var == coVariant ? i-1 : i), var, (var == coVariant ? STD_coord : STD_REL_BDRY_coord), coef );
  GroupLocator leftG( (var == coVariant ? i : i-1), var, (var == coVariant ? STD_REL_BDRY_coord : STD_coord), coef );
  HomLocator secondMapLoc( middleG, rightG );
  HomLocator firstMapLoc( leftG, middleG );
  const NHomMarkedAbelianGroup secondMap(*homGroup(secondMapLoc));
  const NHomMarkedAbelianGroup firstMap(*homGroup(firstMapLoc));
  if (!(secondMap*firstMap).isZero()) return false;
  if (!(secondMap.getKernel().isIsomorphicTo( firstMap.getImage() ) ) ) return false;
 } 
// exactness at H_i(M, \partial M):       H_i M --> H_i(M, \partial M) --> H_{i-1} \partial M     i == 1, ..., aDim
for (unsigned long i=1; i<=aDim; i++) //  H^i M <-- H^i(M, \partial M) <-- H^{i-1} \partial M
 {
  GroupLocator middleG( i, var, STD_REL_BDRY_coord, coef );
  GroupLocator rightG( (var == coVariant ? i-1 : i), var, (var == coVariant ? STD_BDRY_coord : STD_coord), coef );
  GroupLocator leftG( (var == coVariant ? i : i-1), var, (var == coVariant ? STD_coord : STD_BDRY_coord), coef );
  HomLocator secondMapLoc( middleG, rightG );
  HomLocator firstMapLoc( leftG, middleG );
  const NHomMarkedAbelianGroup secondMap(*homGroup(secondMapLoc));
  const NHomMarkedAbelianGroup firstMap(*homGroup(firstMapLoc));
  if (!(secondMap*firstMap).isZero()) return false;
  if (!(secondMap.getKernel().isIsomorphicTo( firstMap.getImage() ) ) ) return false;
  if ( (i == aDim) && (var == coVariant) ) if (!firstMap.isMonic()) return false; // leftmost term in LES, coVariant case
  if ( (i == aDim) && (var == contraVariant) ) if (!secondMap.isEpic()) return false; // rightmost term, contravariant case
 } 

return true;
}

bool NCellularData::poincareDualityVerified() const
{ // at present this only checks to see if H_i(M) and H^{n-i}(M, boundary) are isomorphic.  Later
  // I'll add the appropriate map and this will check that that map is an isomorphism. 
unsigned long aDim = 3;
unsigned long coeff = 0;
if (tri4) { aDim = 4; if (!tri4->isOrientable()) coeff = 2; }
 else { if (!tri3->isOrientable()) coeff = 2; }

for (unsigned long i=0; i <= aDim; i++)
 {
  GroupLocator homoL( i, coVariant, DUAL_coord, coeff );
  GroupLocator cohomoL( aDim-i, contraVariant, STD_REL_BDRY_coord, coeff);
  const NHomMarkedAbelianGroup duality( *homGroup( HomLocator(homoL, cohomoL) ) );
  if (!duality.isIsomorphism()) return false;
 }

for (unsigned long i=0; i <= aDim; i++)
 {
  GroupLocator cohomoL( i, contraVariant, DUAL_coord, coeff );
  GroupLocator homoL( aDim-i, coVariant, STD_REL_BDRY_coord, coeff);
  const NHomMarkedAbelianGroup duality( *homGroup( HomLocator(cohomoL, homoL) ) );
  if (!duality.isIsomorphism()) return false;
 }

return true;
}



void NCellularData::writeTextShort(std::ostream& out) const 
{
bool written=false;
// run through list of everything computed, and print. 
 std::map< GroupLocator, NAbelianGroup* >::const_iterator abi;
 for (abi = abelianGroups.begin(); abi != abelianGroups.end(); abi++)
	{
	 if (written) out<<" ";
	 abi->first.writeTextShort(out);
	 out<<" is ";
	 abi->second->writeTextShort(out);
	 written = true;
        }
 std::map< GroupLocator, NMarkedAbelianGroup* >::const_iterator mabi;
 for (mabi = markedAbelianGroups.begin(); mabi != markedAbelianGroups.end(); mabi++)
	{
	 if (written) out<<" ";
	 mabi->first.writeTextShort(out);
	 out<<" is ";
	 mabi->second->writeTextShort(out);
	 written = true;
        }
 std::map< HomLocator, NHomMarkedAbelianGroup* >::const_iterator hmabi;
 for (hmabi = homMarkedAbelianGroups.begin(); hmabi != homMarkedAbelianGroups.end(); hmabi++)
	{
	 if (written) out<<" ";
	 hmabi->first.writeTextShort(out);
	 out<<" is ";
	 hmabi->second->writeTextShort(out);
	 written = true;
        }
}

void NCellularData::writeTextLong(std::ostream& out) const
{
// list basics of manifold -- dimension, boundary types, orientable, etc.
if (tri3) out<<"3"; else out<<"4";
out<<"-manifold triangulation";
if (tri3) { out<<" named \""; tri3->writeTextShort(out); out<<"\" is"; // use writeTextLong?
	    if (tri3->isOrientable()) out<<" orientable"; else out<<" non-orientable"; 
	    if (tri3->isIdeal() && tri3->hasBoundaryFaces()) out<<" has ideal and standard boundary";
            else if (tri3->isIdeal()) out<<" has ideal boundary";
            else if (tri3->hasBoundaryFaces()) out<<" has standard boundary";
            else out<<" no boundary";
          }
else {
 out<<" named \""; tri4->writeTextShort(out); out<<"\" is"; // use writeTextLong?
	    if (tri4->isOrientable()) out<<" orientable"; else out<<" non-orientable"; 
	    if (tri4->isIdeal() && tri4->hasBoundaryTetrahedra()) out<<" has ideal and standard boundary";
            else if (tri4->isIdeal()) out<<" has ideal boundary";
            else if (tri4->hasBoundaryTetrahedra()) out<<" has standard boundary";
            else out<<" no boundary";
     }
// run through list of everything computed, and print. 
 std::map< GroupLocator, NAbelianGroup* >::const_iterator abi;
 for (abi = abelianGroups.begin(); abi != abelianGroups.end(); abi++)
	{
	 out<<" ";
	 abi->first.writeTextShort(out);
         out<<" is ";
	 abi->second->writeTextShort(out);
         out<<", ";
        }
 std::map< GroupLocator, NMarkedAbelianGroup* >::const_iterator mabi;
 for (mabi = markedAbelianGroups.begin(); mabi != markedAbelianGroups.end(); mabi++)
	{
	 out<<" ";
	 mabi->first.writeTextShort(out);
	 out<<" is ";
	 mabi->second->writeTextShort(out);
         out<<", ";
        }
 std::map< HomLocator, NHomMarkedAbelianGroup* >::const_iterator hmabi;
 for (hmabi = homMarkedAbelianGroups.begin(); hmabi != homMarkedAbelianGroups.end(); hmabi++)
	{
	 out<<" ";
	 hmabi->first.writeTextShort(out);
	 out<<" is ";
	 hmabi->second->writeTextShort(out);
         out<<", ";
        }
 out<<" Euler Char == "<<eulerChar();
 out<<" Poincare Polynomial == "<<poincarePolynomial();
}


const NAbelianGroup* NCellularData::unmarkedGroup( const GroupLocator &g_desc) const
{
std::map< GroupLocator, NAbelianGroup* >::const_iterator p;
p = abelianGroups.find(g_desc);
if (p != abelianGroups.end()) return (p->second);
// okay, so now we know there's no group matching g_desc in markedAbelianGroups, so we make one.
NAbelianGroup* gptr;

std::vector< NMatrixInt* > CC; // choose the right chain complex
if (g_desc.hcs == DUAL_coord) CC = dCC; else if (g_desc.hcs == STD_coord) CC = sCC; else
if (g_desc.hcs == MIX_coord) CC = mCC; else if (g_desc.hcs == STD_BDRY_coord) CC = bsCC; else
if (g_desc.hcs == STD_REL_BDRY_coord) CC = rCC;

if (g_desc.var == coVariant) // homology requested
 {
  if (g_desc.cof == 0) gptr = new NAbelianGroup( *CC[g_desc.dim], *CC[g_desc.dim+1] );
   else gptr = new NAbelianGroup( *CC[g_desc.dim], *CC[g_desc.dim+1], NLargeInteger(g_desc.cof) );

  std::map< GroupLocator, NAbelianGroup* > *abgptr = 
	const_cast< std::map< GroupLocator, NAbelianGroup* > *> (&abelianGroups);
  abgptr->insert(std::pair<GroupLocator,NAbelianGroup*>( g_desc, gptr ) ); 
  return gptr;
 }
else // cohomology requested
 { 
  NMatrixInt dCCN( CC[g_desc.dim]->columns(),   CC[g_desc.dim]->rows() );
  for (unsigned long i=0; i<dCCN.rows(); i++) for (unsigned long j=0; j<dCCN.columns(); j++)
   dCCN.entry(i,j) = CC[g_desc.dim]->entry(j,i);
  NMatrixInt dCCM( CC[g_desc.dim+1]->columns(), CC[g_desc.dim+1]->rows() );
  for (unsigned long i=0; i<dCCM.rows(); i++) for (unsigned long j=0; j<dCCM.columns(); j++)
   dCCM.entry(i,j) = CC[g_desc.dim+1]->entry(j,i);
  if (g_desc.cof == 0) gptr = new NAbelianGroup( dCCM, dCCN );
   else gptr = new NAbelianGroup( dCCM, dCCN, NLargeInteger(g_desc.cof) );
  std::map< GroupLocator, NAbelianGroup* > *abgptr = 
	const_cast< std::map< GroupLocator, NAbelianGroup* > *> (&abelianGroups);
  abgptr->insert(std::pair<GroupLocator,NAbelianGroup*>(g_desc,gptr)); 
  return gptr;
 }

return NULL;
}

// todo add an aDim and ensure request is with dimension bounds
const NMarkedAbelianGroup* NCellularData::markedGroup( const GroupLocator &g_desc) const
{
std::map< GroupLocator, NMarkedAbelianGroup* >::const_iterator p;
p = markedAbelianGroups.find(g_desc);
if (p != markedAbelianGroups.end()) return (p->second);
// okay, so now we know there's no group matching g_desc in markedAbelianGroups, so we make one.
NMarkedAbelianGroup* mgptr;

std::vector< NMatrixInt* > CC; // choose the right chain complex
if (g_desc.hcs == DUAL_coord) CC = dCC; else if (g_desc.hcs == STD_coord) CC = sCC; else
if (g_desc.hcs == MIX_coord) CC = mCC; else if (g_desc.hcs == STD_BDRY_coord) CC = bsCC; else
if (g_desc.hcs == STD_REL_BDRY_coord) CC = rCC;

if (g_desc.var == coVariant) // homology requested
 {
  if (g_desc.cof == 0) mgptr = new NMarkedAbelianGroup( *CC[g_desc.dim], *CC[g_desc.dim+1] );
   else mgptr = new NMarkedAbelianGroup( *CC[g_desc.dim], *CC[g_desc.dim+1], NLargeInteger(g_desc.cof) );

  std::map< GroupLocator, NMarkedAbelianGroup* > *mabgptr = 
	const_cast< std::map< GroupLocator, NMarkedAbelianGroup* > *> (&markedAbelianGroups);
  mabgptr->insert(std::pair<GroupLocator,NMarkedAbelianGroup*>(g_desc,mgptr)); 
  return mgptr;
 }
else // cohomology requested
 { 
  NMatrixInt dCCN( CC[g_desc.dim]->columns(),   CC[g_desc.dim]->rows() );
  for (unsigned long i=0; i<dCCN.rows(); i++) for (unsigned long j=0; j<dCCN.columns(); j++)
   dCCN.entry(i,j) = CC[g_desc.dim]->entry(j,i);
  NMatrixInt dCCM( CC[g_desc.dim+1]->columns(), CC[g_desc.dim+1]->rows() );
  for (unsigned long i=0; i<dCCM.rows(); i++) for (unsigned long j=0; j<dCCM.columns(); j++)
   dCCM.entry(i,j) = CC[g_desc.dim+1]->entry(j,i);
  if (g_desc.cof == 0) mgptr = new NMarkedAbelianGroup( dCCM, dCCN );
   else mgptr = new NMarkedAbelianGroup( dCCM, dCCN, NLargeInteger(g_desc.cof) );
  std::map< GroupLocator, NMarkedAbelianGroup* > *mabgptr = 
	const_cast< std::map< GroupLocator, NMarkedAbelianGroup* > *> (&markedAbelianGroups);
  mabgptr->insert(std::pair<GroupLocator,NMarkedAbelianGroup*>(g_desc,mgptr)); 
  return mgptr;
 }

return NULL;
}

// todo: coefficient LES maps like Bockstein
const NHomMarkedAbelianGroup* NCellularData::homGroup( const HomLocator &h_desc) const
{
 std::map< HomLocator, NHomMarkedAbelianGroup* >::const_iterator p;
 p = homMarkedAbelianGroups.find(h_desc);
 if (p != homMarkedAbelianGroups.end()) return (p->second);
 // okay, so now we know there's no group matching g_desc in markedAbelianGroups, so we make one.
 unsigned long aDim = ( tri3 ? 3 : 4 ); // ambient dimension
 // out-of-bounds request
 if ( (h_desc.domain.dim > aDim) || (h_desc.range.dim > aDim) ) return NULL;
 // inappropriate change of coefficients request
 if ( h_desc.range.cof != 0 ) if ( h_desc.domain.cof % h_desc.range.cof != 0 ) return NULL;
 // choose the right chain map, or leave unallocated if we can't make sense of the request.
 NMatrixInt* CM(NULL); 

 if (h_desc.domain.var == h_desc.range.var) // variance-preserving map requested
  { // check if they want a pure change-of-coefficients map 
   if ( (h_desc.domain.dim == h_desc.range.dim) && (h_desc.domain.hcs == h_desc.range.hcs) )
    {
      unsigned long ccdim = markedGroup( h_desc.domain )->getRankCC();
      CM = new NMatrixInt(ccdim, ccdim); CM->makeIdentity(); 
    }

   // check if they want a subdivision-induced map, co-variant
   if ( ( (h_desc.domain.hcs == STD_coord) || (h_desc.domain.hcs == DUAL_coord) ) && 
          (h_desc.domain.var == coVariant) && (h_desc.range.hcs == MIX_coord) )
         CM = ( h_desc.domain.hcs == STD_coord ? clonePtr(s_mCM[h_desc.domain.dim]) :
					         clonePtr(d_mCM[h_desc.domain.dim]) );
   if ( ( (h_desc.range.hcs == STD_coord) || (h_desc.range.hcs == DUAL_coord) ) && // contravariant 
          (h_desc.domain.var == contraVariant)  && (h_desc.domain.hcs == MIX_coord) )
      {
	 const NMatrixInt* tCMp( h_desc.range.hcs == STD_coord ? s_mCM[h_desc.domain.dim] : 
			                                         d_mCM[h_desc.domain.dim] );
         CM = new NMatrixInt( tCMp->columns(), tCMp->rows() );
         for (unsigned long i=0; i<CM->rows(); i++) for (unsigned long j=0; j<CM->columns(); j++)
	  CM->entry( i, j ) = tCMp->entry( j, i );
      }

   // check if they want a map from the homology LES of the pair (M, \partial M) 
   if ( h_desc.domain.var == coVariant )
    { // \partial M --> M
      if ( (h_desc.domain.hcs == STD_BDRY_coord) && (h_desc.range.hcs == STD_coord) &&
           (h_desc.domain.dim == h_desc.range.dim) && (h_desc.domain.dim < aDim) )
	CM = clonePtr( bs_sCM[h_desc.domain.dim] ); 
      // M --> (M, \partial M)
      else if ( (h_desc.domain.hcs == STD_coord) && (h_desc.range.hcs == STD_REL_BDRY_coord) &&
           (h_desc.domain.dim == h_desc.range.dim) )
	CM = clonePtr( s_rCM[h_desc.domain.dim] );
      // (M, \partial M) --> \partial M
      else if ( (h_desc.domain.hcs == STD_REL_BDRY_coord) && (h_desc.range.hcs == STD_BDRY_coord) &&
           (h_desc.domain.dim == h_desc.range.dim+1) && (h_desc.range.dim < aDim) )
	CM = clonePtr( rbCM[h_desc.range.dim] );
   }
   else
    { // \partial M <-- M
      if ( (h_desc.domain.hcs == STD_coord) && (h_desc.range.hcs == STD_BDRY_coord) &&
           (h_desc.domain.dim == h_desc.range.dim) && (h_desc.range.dim < aDim) )
       {	 
         CM = new NMatrixInt( bs_sCM[h_desc.domain.dim]->columns(), bs_sCM[h_desc.domain.dim]->rows() );
         for (unsigned long i=0; i<CM->rows(); i++) for (unsigned long j=0; j<CM->columns(); j++)
	  CM->entry( i, j ) = bs_sCM[h_desc.domain.dim]->entry( j, i );
       }
      // M <-- (M, \partial M)
      else if ( (h_desc.domain.hcs == STD_REL_BDRY_coord) && (h_desc.range.hcs == STD_coord) &&
           (h_desc.domain.dim == h_desc.range.dim) )
       {	 
         CM = new NMatrixInt( s_rCM[h_desc.domain.dim]->columns(), s_rCM[h_desc.domain.dim]->rows() );
         for (unsigned long i=0; i<CM->rows(); i++) for (unsigned long j=0; j<CM->columns(); j++)
	  CM->entry( i, j ) = s_rCM[h_desc.domain.dim]->entry( j, i );
       }
      // (M, \partial M) <-- \partial M
      else if ( (h_desc.domain.hcs == STD_BDRY_coord) && (h_desc.range.hcs == STD_REL_BDRY_coord) &&
           (h_desc.domain.dim+1 == h_desc.range.dim) && (h_desc.domain.dim < aDim) )
       {	 
         CM = new NMatrixInt( rbCM[h_desc.domain.dim]->columns(), rbCM[h_desc.domain.dim]->rows() );
         for (unsigned long i=0; i<CM->rows(); i++) for (unsigned long j=0; j<CM->columns(); j++)
	  CM->entry( i, j ) = rbCM[h_desc.domain.dim]->entry( j, i );
       }
    } 
 }
 else
 { // variance-reversing map requested
   // record if manifold orientable or not
   bool orientable = false;
   if ( tri3 ) if ( tri3->isOrientable() ) orientable = true;
   if ( tri4 ) if ( tri4->isOrientable() ) orientable = true;
   if ( (!orientable) && (h_desc.domain.cof != 2) && (h_desc.range.cof != 2) ) return NULL;
 
   // check if they want Poincare Duality
   if ( (h_desc.domain.var == coVariant) && (h_desc.domain.hcs == DUAL_coord) &&
        (h_desc.range.hcs == STD_REL_BDRY_coord) && (h_desc.domain.dim + h_desc.range.dim == aDim) )
   { 
     CM = new NMatrixInt( numRelativeCells[ h_desc.range.dim ], numDualCells[ h_desc.domain.dim ] );
     CM->makeIdentity(); // good enough if not orientable, but we need to correct if orientable. 
     if (orientable) correctRelOrMat( *CM, h_desc.domain.dim, tri3, tri4, dcIx );
   } else 
   if ( (h_desc.domain.var == contraVariant) && (h_desc.domain.hcs == DUAL_coord) &&
        (h_desc.range.hcs == STD_REL_BDRY_coord) && (h_desc.domain.dim + h_desc.range.dim == aDim) )
   { 
     CM = new NMatrixInt( numRelativeCells[ h_desc.range.dim ], numDualCells[ h_desc.domain.dim ] );
     CM->makeIdentity(); // good enough if not orientable, but we need to correct if orientable. 
     if (orientable) correctRelOrMat( *CM, h_desc.domain.dim, tri3, tri4, dcIx );   
   }
 }

 NHomMarkedAbelianGroup* hmgptr(NULL);
 if ( CM ) // we found the requested map, now make sure we have the domain and range, then we're happy.
 {
   //  ensure we have domain and range
   const NMarkedAbelianGroup* dom = markedGroup( h_desc.domain );
   const NMarkedAbelianGroup* ran = markedGroup( h_desc.range );
   if ( dom && ran )
    {
     hmgptr = new NHomMarkedAbelianGroup( *dom, *ran, *CM );
     std::map< HomLocator, NHomMarkedAbelianGroup* > *hmabgptr = 
      const_cast< std::map< HomLocator, NHomMarkedAbelianGroup* > *> (&homMarkedAbelianGroups);
     hmabgptr->insert(std::pair<HomLocator,NHomMarkedAbelianGroup*>(h_desc,hmgptr)); 
     return hmgptr;
    }
 }
 if ( CM ) delete CM;

// didn't find what was requested
return NULL;
}

NSVPolynomialRing NCellularData::poincarePolynomial() const
{
NSVPolynomialRing retval;
unsigned long aDim( tri3 ? 3 : 4 );
for (unsigned long i=0; i<=aDim; i++) retval += 
 NSVPolynomialRing( NLargeInteger( unmarkedGroup( GroupLocator(i, coVariant, DUAL_coord, 0))->getRank() ), i );
return retval;
}


/**
 *  Computes various bilinear forms associated to the homology of the manifold:
 *
 *  1) Homology-Cohomology pairing <.,.>  ie: H_i(M;R) x H^i(M;R) --> R  where R is the coefficients
 *  2) Intersection product               ie: H_i(M;R) x H_j(M;R) --> H_{(i+j)-n}(M;R)
 *  3) Torsion linking form               ie: H_i(M;Z) x H_j(M;Z) --> H_{(i+j)-(n-1)}(M;Q/Z)
 *  4) cup products                       ie: H^i(M;R) x H^j(M;R) --> H^{i+j}(M;R)
 */
const NBilinearForm* NCellularData::bilinearForm( const FormLocator &f_desc ) const
{
 unsigned long aDim( tri3 ? 3 : 4 );

 std::map< FormLocator, NBilinearForm* >::const_iterator p;
 p = bilinearForms.find(f_desc);
 if (p != bilinearForms.end()) return (p->second);
 // okay, so now we know there's no form matching f_desc in bilinearForms, so we make one.

 // case 1: homology-cohomology pairing

 // case 2: intersection products i+j >= n == aDim
 //         (dual)H_i(M;R) x (std rel)H_j(M,P;R) --> (mix)H_{(i+j)-n}(M;R) (a)
 // also     (std)H_i(M;R) x (std rel)H_j(M,P;R) --> (std)H_{(i+j)-n}(M;R) 
 //          (std)H_i(M;R) x     (std)H_j(M,P;R) --> (std)H_{(i+j)-n}(M;R) 
 // we start with the most elementary intersection product... (a)
 if ( ( f_desc.ft == intersectionForm ) &&
      ( f_desc.ldomain.var == coVariant ) && (f_desc.rdomain.var == coVariant) &&
      ( f_desc.ldomain.dim + f_desc.rdomain.dim >= aDim ) &&
      ( (f_desc.ldomain.dim + f_desc.rdomain.dim) - aDim < aDim - 1 ) &&
      ( f_desc.ldomain.dim > 0) && ( f_desc.rdomain.dim > 0 ) &&
      ( f_desc.ldomain.cof == f_desc.rdomain.cof ) &&
      ( f_desc.ldomain.hcs == DUAL_coord ) && (f_desc.rdomain.hcs == STD_REL_BDRY_coord) )
  {
   // check its orientable if R != Z_2
   if ( (f_desc.ldomain.cof != 2) && ( tri3 ? !tri3->isOrientable() : !tri4->isOrientable() ) ) return NULL;
   const NMarkedAbelianGroup* lDom( markedGroup(f_desc.ldomain) );
   const NMarkedAbelianGroup* rDom( markedGroup(f_desc.rdomain) );
   const NMarkedAbelianGroup* rAng( markedGroup( GroupLocator( (f_desc.ldomain.dim + f_desc.rdomain.dim) - aDim,
					coVariant, MIX_coord, f_desc.ldomain.cof ) ) );
   NSparseGrid< NLargeInteger > intM(3); 

   NBilinearForm* bfptr(NULL);
   // aDim==3  1,2, 2,1  to H_0 // 2,2  to H_1
   // aDim==4  1,3, 2,2, 3,1 to H_0 // 2,3, 3,2  to H_1 // 3,3  to H_2   
   if ( (aDim==3) && (f_desc.ldomain.dim == 2) && (f_desc.rdomain.dim == 2) )
    {// aDim==3, (dual)H_2 x (std_rel)H_2 --> (mix)H_1
     // each STD_REL_BDRY cell has <= 3 boundary 1-cells, each one corresponds to a DUAL cell...
     for (unsigned long i=0; i<numRelativeCells[2]; i++)
      {
       const NFace* fac( tri3->getFace( rIx[2][i] ) ); const NEdge* edg(NULL);
       const NTetrahedron* tet( fac->getEmbedding(1).getTetrahedron() );
       for (unsigned long j=0; j<3; j++)
	{
	 edg = fac->getEdge(j); if (!edg->isBoundary())
	  { // intM[ J, i, 2*numNonIdealCells[2] + 3*i+j ] += whatever
	    // for orientation we need to compare normal orientation of these edges to product normal orientations
           unsigned long J( lower_bound( dcIx[2].begin(), dcIx[2].end(), tri3->edgeIndex( edg ) ) - dcIx[2].begin() );
	   NMultiIndex x(3); x[0] = J; x[1] = i; x[2] = 2*numNonIdealCells[1] + 3*rIx[2][i] + j;
// x[2] error? 
	   // fac->getEdgeMapping(j)[0] and [1] are the vertices of the edge in the face, so we apply
	   // facinc to that, then get the corresp edge number
	   NPerm4 facinc( fac->getEmbedding(1).getVertices() );
	   NPerm4 edginc( tet->getEdgeMapping( NEdge::edgeNumber[facinc[fac->getEdgeMapping(j)[0]]][facinc[fac->getEdgeMapping(j)[1]]] ) );
           // edginc[2,3] give orientation of part of dual 2-cell in this tet...
	   // normalize edginc to ambient orientation
	   if (tet->orientation() != edginc.sign()) edginc = edginc * NPerm4(0,1);
	   unsigned long oppfac ( edginc[2] == fac->getEmbedding(1).getFace() ? edginc[3] : edginc[2] );
           unsigned long facnum ( fac->getEmbedding(1).getFace() );

           NPerm4 relor( edginc[0], edginc[1], ( (tet->orientation()==1) ? facnum : oppfac), 
                                               ( (tet->orientation()==1) ? oppfac : facnum) );

           intM.incEntry( x, ( relor.sign() == tet->orientation() ? 1 : -1 ) );

	  }
	}
      }
    }

//intM.writeTextShort(std::cout);

   bfptr = new NBilinearForm( *lDom, *rDom, *rAng, intM );
   std::map< FormLocator, NBilinearForm* > *mbfptr = 
    const_cast< std::map< FormLocator, NBilinearForm* > *> (&bilinearForms);
   mbfptr->insert( std::pair<FormLocator, NBilinearForm*>(f_desc, bfptr) );
   return bfptr; 
  }

 // case 3: torsion linking forms

 // case 4: cup products

 return NULL;
}


} // namespace regina



