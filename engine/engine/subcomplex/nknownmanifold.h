
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nknownmanifold.h
 *  \brief Provides global 3-manifold recognition routines.
 */

#ifndef __NKNOWNMANIFOLD_H
#ifndef __DOXYGEN
#define __NKNOWNMANIFOLD_H
#endif

namespace regina {

class NSFS;
class NTriangulation;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Determines whether the given triangulation is recognised as a Seifert
 * fibred space.  Either a Seifert fibred space will be recognised or the
 * results will be inconclusive.
 *
 * This routine operates by simply searching for standard triangulations
 * of Seifert fibred spaces.  Thus most triangulations of Seifert fibred
 * spaces will \e not be recognised.
 *
 * However, this routine is more likely to succeed if the given triangulation
 * is minimal.  In particular, it will successfully identify every minimal
 * one-vertex triangulation of a Seifert fibred space with less than
 * six tetrahedra.
 *
 * @param tri the triangulation to identify
 * @return a newly created Seifert structure if the given triangulation
 * was recognised, or \c null if the results were inconclusive (i.e.,
 * it might or might not be a Seifert fibred space).
 */
NSFS* isKnownSFS(NTriangulation* tri);

/*@}*/

} // namespace regina

#endif

