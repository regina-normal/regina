
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

/*! \file circlepack/facetData.h
 *  \brief Stored information about facets in 2-manifold triangulation. 
 */

#ifndef __FACETDATA_H
#ifndef __DOXYGEN
#define __FACETDATA_H
#endif

//have another data structure edgeData indexed by d2edge* edg.  for each edge it contains
//.. doubles x_i,y_i, i in[0,1] co-ords of vertices of the edge
//.. int eCol that somehow tells us where this edge is from (face or tet)
//.. bool bdry that tells us whether or not edge is a boundary edge
//.. int oCol that tells us which (tet or pent) the triangle edge was formerly attached to is from 
struct edgeData{
	edgeData(){};

	int eCol;
	int col_0;
	int col_1;
	bool bdry;
	double x_0;
	double y_0;
	double x_1;
	double y_1;
};

//begin with a data structure triData. The structure is indexed by Dim2Triangle* tri. for each tri it contains:
//.. int tCol, corresponding to the tet or pent that tri originates from in buildLink()
//.. doubles x_i,y_i, i in[0,2] co-ords of vertices of the triangle
struct triData{
	triData(){};

	int tCol;
	double x_0;
	double y_0;
	double x_1;
	double y_1;
	double x_2;
	double y_2;
	int adj[3];
};


//one last data struct indexed by d2vert:
//.. doubles x,y cords of vert
//.. int vCol, corresponding to the edge or face that this vert originates from in buildLink()
struct vertData{
	vertData(){};

	int vCol;
	double x;
	double y;
};

#endif
