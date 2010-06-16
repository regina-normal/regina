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

/**
 *  This routine runs through the dual 1-skeleton to the triangulation, building a maximal tree in the dual 1-skeleton
 * to the triangulation which restricts to a maximal tree in any boundary component, ideal or "standard" boundary.
 *
 * Assumes triangulation is connected.
 */
void NCellularData::buildMaximalTree() const
{
// store the tree forest as std::set< unsigned long > with reference to
//  nicIx, icIx, dcIx, bcIx, rIx. Store this in the domain of these functions.

// Iterate: walk through dual 1-skeleton until we hit a boundary component we haven't hit before.  During the iteration we
//  keep track of several things. 
//  the 0-cell list will consist of: Indexing of the barycentres of the pentachora, boundary tetrahedra and ideal boundary 
//   tetrahedra, indexed as the ideal ends of 1-cells 5*pen index + vertex index.  So we'll build it using a while loop that
//   goes back through the list of what's been done, and tries to expand it.  Priority will be to the boundary and ideal
//   boundary so that those trees are fully-developed as soon as the tree hits the boundary, as we want the tree to restrict
//   to trees in the boundary.   We also need to keep track of the tree itself.  This will be kept track of by the indexing
//   of (extended) dual 1-cells [index set the tetrahedra] and ideal dual 1-cells [index set

std::set< unsigned long > visitedPen, visitedBdTet, visitedIdTet; // 0-cells in maximal tree
 // indexed by            nicIx[4],   
 //                                   bcIx[3], 
 //                                                 icIx[3] respectively.

std::set< unsigned long > visitedInTet, visitedBdFac, visitedIdFac; // 1-cells in maximal tree
 // indexed by            nicIx[3] 
 //                                     bcIx[2] 
 //                                                   icIx[2]

// let's seed the process and start in pentachoron 0.
visitedPen.insert(0);

while ( (visitedPen.size() < numStandardCells[4] ) || (visitedBdTet.size() < numNonIdealBdryCells[3]) ||
        (visitedIdTet.size() < numIdealCells[3]) ) // or use a "while (somethingtodo)" style loop ?
 {
  // first we look to expand the forest in the standard boundary
  std::set< unsigned long >::iterator btet_it = visitedBdTet.begin();
  std::set< unsigned long >::iterator new_it = visitedBdTet.end();
  while (btet_it != visitedBdTet.end())
   {
    // look for adjacent tetrahedra to btet_it whose addition wouldn't introduce any loops
    const Dim4Tetrahedron* btet(NULL); const Dim4Pentachoron* pen(NULL); unsigned long tetnum;

    btet = tri4 -> getTetrahedron( *btet_it );     //btet
    pen = btet -> getEmbedding(0).getPentachoron();//ambient pen
    tetnum = btet -> getEmbedding(0).getTetrahedron(); // btet's number in ambient tet

    for (unsigned long i=1; i<5; i++)
      {
       const Dim4Tetrahedron* adjtet(NULL); const Dim4Pentachoron* adjpen(NULL); const Dim4Tetrahedron* septet(NULL);
       septet = pen -> getTetrahedron( (tetnum + i) % 5 );
       const NPerm5 septetmap( pen -> getTetrahedronMapping( (tetnum + i) % 5 ) );
       const Dim4Face* bfac(NULL);
       bfac = septet->getFace( septetmap.preImageOf( tetnum ) );
       const unsigned long bfacnum( tri4->faceIndex(bfac) );

       adjpen = pen -> adjacentPentachoron( (tetnum + i) % 5 );
       adjtet = adjpen -> getTetrahedron( pen -> adjacentGluing( (tetnum + i) % 5)[tetnum] );
       // suitably update visitedBdTet and visitedBdFac indexed by bcIx[3] and bcIx[2] resp.
       // find adjtet's index in bcIx[3]
       unsigned long I,J; 
       I = lower_bound( bcIx[3].begin(), bcIx[3].end(), tri4->tetrahedronIndex(adjtet) ) - bcIx[3].begin();
       J = lower_bound( bcIx[2].begin(), bcIx[2].end(), bfacnum ) - bcIx[2].begin();
       // check to see if I is in visitedBdTet
       if (!binary_search( visitedBdTet.begin(), visitedBdTet.end(), I) ) // not found
        {
         std::set< unsigned long >::iterator temp_it = visitedBdTet.insert(I).first;
         visitedBdFac.insert(J);
         if ( (*temp_it) < (*btet_it) ) if ( (*temp_it) < (*new_it) ) new_it = temp_it;
        }
      }
     if ( (*new_it) < (*btet_it) ) btet_it = new_it; else btet_it++;
   }

  // then we look to expand the forest in the ideal boundary

  // finally we look to expand the forest in the manifold's interior (or towards a boundary)

 }


}

void buildFundamentalGroups(Dim4Triangulation* tri4 )
{
}

void buildFundamentalGroupInclusions(Dim4Triangulation* tri4 )
{
}


} // namespace regina


