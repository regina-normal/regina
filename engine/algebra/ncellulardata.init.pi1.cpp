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
void buildMaximalTree(Dim4Triangulation* tri4 )
{
// store the tree forest as std::vector< unsigned long > with reference to
//  nicIx, icIx, dcIx, bcIx, rIx. Store this in the domain of these functions.

// step 1: walk through dual 1-skeleton until we hit a boundary component we haven't
//         hit before.  We keep track of three things:
//         a) pentachora visited. 
//         b) ideal boundary tetrahedra visited
//         c) standard boundary tetrahedra visited

// (a)(b)(c) visited will be stored as a map ? 
std::map< unsigned long, unsigned long > visitedPen, visitedTet, visitedIdTet;

// step 2: boundary_comp::iterate through dual 1-skeleton until exhausted
//         b) if in ideal boundary tet, walk to all adjacent tets until can't find
//            an unvisited one.
//         c) same with standard boundary tetrahedra
}

void buildFundamentalGroups(Dim4Triangulation* tri4 )
{
}

void buildFundamentalGroupInclusions(Dim4Triangulation* tri4 )
{
}


} // namespace regina


