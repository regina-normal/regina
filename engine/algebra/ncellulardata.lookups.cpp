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

#include <map>

#include <iostream>
#include <sstream>

namespace regina {

unsigned long NCellularData::nicIxLookup(const NVertex* vrt) const
{ return lower_bound( nicIx[0].begin(), nicIx[0].end(), tri3->vertexIndex(vrt) ) - nicIx[0].begin(); }
unsigned long NCellularData::nicIxLookup(const NEdge* edg) const
{ return lower_bound( nicIx[1].begin(), nicIx[1].end(), tri3->edgeIndex(edg) ) - nicIx[1].begin(); }
unsigned long NCellularData::nicIxLookup(const NFace* fac) const
{ return lower_bound( nicIx[2].begin(), nicIx[2].end(), tri3->faceIndex(fac) ) - nicIx[2].begin(); }
unsigned long NCellularData::nicIxLookup(const NTetrahedron* tet) const
{ return lower_bound( nicIx[3].begin(), nicIx[3].end(), tri3->tetrahedronIndex(tet) ) - nicIx[3].begin(); }

unsigned long NCellularData::nicIxLookup(const Dim4Vertex* vrt) const
{ return lower_bound( nicIx[0].begin(), nicIx[0].end(), tri4->vertexIndex(vrt) ) - nicIx[0].begin(); }
unsigned long NCellularData::nicIxLookup(const Dim4Edge* edg) const
{ return lower_bound( nicIx[1].begin(), nicIx[1].end(), tri4->edgeIndex(edg) ) - nicIx[1].begin(); }
unsigned long NCellularData::nicIxLookup(const Dim4Face* fac) const
{ return lower_bound( nicIx[2].begin(), nicIx[2].end(), tri4->faceIndex(fac) ) - nicIx[2].begin(); }
unsigned long NCellularData::nicIxLookup(const Dim4Tetrahedron* tet) const
{ return lower_bound( nicIx[3].begin(), nicIx[3].end(), tri4->tetrahedronIndex(tet) ) - nicIx[3].begin(); }
unsigned long NCellularData::nicIxLookup(const Dim4Pentachoron* pen) const
{ return lower_bound( nicIx[4].begin(), nicIx[4].end(), tri4->pentachoronIndex(pen) ) - nicIx[4].begin(); }

unsigned long NCellularData::icIxLookup(const NEdge* edg, unsigned long i) const
{ return lower_bound( icIx[0].begin(), icIx[0].end(), 2*tri3->edgeIndex(edg)+i ) - icIx[0].begin(); }
unsigned long NCellularData::icIxLookup(const NFace* fac, unsigned long i) const
{ return lower_bound( icIx[1].begin(), icIx[1].end(), 3*tri3->faceIndex(fac)+i ) - icIx[1].begin(); }
unsigned long NCellularData::icIxLookup(const NTetrahedron* tet, unsigned long i) const
{ return lower_bound( icIx[2].begin(), icIx[2].end(), 4*tri3->tetrahedronIndex(tet)+i ) - icIx[2].begin(); }

unsigned long NCellularData::icIxLookup(const Dim4Edge* edg, unsigned long i) const
{ return lower_bound( icIx[0].begin(), icIx[0].end(), 2*tri4->edgeIndex(edg) + i) - icIx[0].begin(); }
unsigned long NCellularData::icIxLookup(const Dim4Face* fac, unsigned long i) const
{ return lower_bound( icIx[1].begin(), icIx[1].end(), 3*tri4->faceIndex(fac) + i) - icIx[1].begin(); }
unsigned long NCellularData::icIxLookup(const Dim4Tetrahedron* tet, unsigned long i) const
{ return lower_bound( icIx[2].begin(), icIx[2].end(), 4*tri4->tetrahedronIndex(tet) + i) - icIx[2].begin(); }
unsigned long NCellularData::icIxLookup(const Dim4Pentachoron* pen, unsigned long i) const
{ return lower_bound( icIx[3].begin(), icIx[3].end(), 5*tri4->pentachoronIndex(pen) + i ) - icIx[3].begin(); }

unsigned long NCellularData::dcIxLookup(const NTetrahedron* tet) const 
{ return lower_bound( dcIx[0].begin(), dcIx[0].end(), tri3->tetrahedronIndex(tet) ) - dcIx[0].begin(); }
unsigned long NCellularData::dcIxLookup(const NFace* fac) const 
{ return lower_bound( dcIx[1].begin(), dcIx[1].end(), tri3->faceIndex(fac) ) - dcIx[1].begin(); }
unsigned long NCellularData::dcIxLookup(const NEdge* edg) const 
{ return lower_bound( dcIx[2].begin(), dcIx[2].end(), tri3->edgeIndex(edg) ) - dcIx[2].begin(); }
unsigned long NCellularData::dcIxLookup(const NVertex* vrt) const 
{ return lower_bound( dcIx[3].begin(), dcIx[3].end(), tri3->vertexIndex(vrt) ) - dcIx[3].begin(); }

unsigned long NCellularData::dcIxLookup(const Dim4Pentachoron* pen) const 
{ return lower_bound( dcIx[0].begin(), dcIx[0].end(), tri4->pentachoronIndex(pen) ) - dcIx[0].begin(); }
unsigned long NCellularData::dcIxLookup(const Dim4Tetrahedron* tet) const 
{ return lower_bound( dcIx[1].begin(), dcIx[1].end(), tri4->tetrahedronIndex(tet) ) - dcIx[1].begin(); }
unsigned long NCellularData::dcIxLookup(const Dim4Face* fac) const 
{ return lower_bound( dcIx[2].begin(), dcIx[2].end(), tri4->faceIndex(fac) ) - dcIx[2].begin(); }
unsigned long NCellularData::dcIxLookup(const Dim4Edge* edg) const 
{ return lower_bound( dcIx[3].begin(), dcIx[3].end(), tri4->edgeIndex(edg) ) - dcIx[3].begin(); }
unsigned long NCellularData::dcIxLookup(const Dim4Vertex* vrt) const 
{ return lower_bound( dcIx[4].begin(), dcIx[4].end(), tri4->vertexIndex(vrt) ) - dcIx[4].begin(); }

unsigned long NCellularData::bcIxLookup(const NVertex* vrt) const 
{ return lower_bound( bcIx[0].begin(), bcIx[0].end(), tri3->vertexIndex(vrt) ) - bcIx[0].begin(); }
unsigned long NCellularData::bcIxLookup(const NEdge* edg) const 
{ return lower_bound( bcIx[1].begin(), bcIx[1].end(), tri3->edgeIndex(edg) ) - bcIx[1].begin(); }
unsigned long NCellularData::bcIxLookup(const NFace* fac) const 
{ return lower_bound( bcIx[2].begin(), bcIx[2].end(), tri3->faceIndex(fac) ) - bcIx[2].begin(); }

unsigned long NCellularData::bcIxLookup(const Dim4Vertex* vrt) const
{ return lower_bound( bcIx[0].begin(), bcIx[0].end(), tri4->vertexIndex(vrt) ) - bcIx[0].begin(); }
unsigned long NCellularData::bcIxLookup(const Dim4Edge* edg) const
{ return lower_bound( bcIx[1].begin(), bcIx[1].end(), tri4->edgeIndex(edg) ) - bcIx[1].begin(); }
unsigned long NCellularData::bcIxLookup(const Dim4Face* fac) const
{ return lower_bound( bcIx[2].begin(), bcIx[2].end(), tri4->faceIndex(fac) ) - bcIx[2].begin(); }
unsigned long NCellularData::bcIxLookup(const Dim4Tetrahedron* tet) const
{ return lower_bound( bcIx[3].begin(), bcIx[3].end(), tri4->tetrahedronIndex(tet) ) - bcIx[3].begin(); }

unsigned long NCellularData::rIxLookup(const NVertex* vrt) const
{ return lower_bound( rIx[0].begin(), rIx[0].end(), tri3->vertexIndex(vrt) ) - rIx[0].begin(); }
unsigned long NCellularData::rIxLookup(const NEdge* edg) const
{ return lower_bound( rIx[1].begin(), rIx[1].end(), tri3->edgeIndex(edg) ) - rIx[1].begin(); }
unsigned long NCellularData::rIxLookup(const NFace* fac) const
{ return lower_bound( rIx[2].begin(), rIx[2].end(), tri3->faceIndex(fac) ) - rIx[2].begin(); }
unsigned long NCellularData::rIxLookup(const NTetrahedron* tet) const
{ return lower_bound( rIx[3].begin(), rIx[3].end(), tri3->tetrahedronIndex(tet) ) - rIx[3].begin(); }

unsigned long NCellularData::rIxLookup(const Dim4Vertex* vrt) const
{ return lower_bound( rIx[0].begin(), rIx[0].end(), tri4->vertexIndex(vrt) ) - rIx[0].begin(); }
unsigned long NCellularData::rIxLookup(const Dim4Edge* edg) const
{ return lower_bound( rIx[1].begin(), rIx[1].end(), tri4->edgeIndex(edg) ) - rIx[1].begin(); }
unsigned long NCellularData::rIxLookup(const Dim4Face* fac) const
{ return lower_bound( rIx[2].begin(), rIx[2].end(), tri4->faceIndex(fac) ) - rIx[2].begin(); }
unsigned long NCellularData::rIxLookup(const Dim4Tetrahedron* tet) const
{ return lower_bound( rIx[3].begin(), rIx[3].end(), tri4->tetrahedronIndex(tet) ) - rIx[3].begin(); }
unsigned long NCellularData::rIxLookup(const Dim4Pentachoron* pen) const
{ return lower_bound( rIx[4].begin(), rIx[4].end(), tri4->pentachoronIndex(pen) ) - rIx[4].begin(); }

} // regina namespace
