
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

/*! \file circlepack/circlePack.h
 *  \brief Generates postscript of circle packings of triangulated spheres. 
 */

#ifndef __CIRCLEPACK_H
#ifndef __DOXYGEN
#define __CIRCLEPACK_H
#endif

namespace regina{

/**
 *  Given a Dim4Triangulation pointer and a Dim4Edge pointer, 
 * this routine generates a postscript file that has a visual
 * image of a circle packed link of the edge. 
 */
REGINA_API std::string CPpackSpherePS(Dim4Triangulation* tri, Dim4Edge* edge);

/**
 *  Given an NTriangulation pointer and an NVertex pointer, 
 * this routine generates a postscript file that has a visual
 * image of a circle packed link of the vertex. 
 */
REGINA_API std::string CPpackSpherePS(NTriangulation* tri, NVertex* vertex);

}//namespace regina
#endif

