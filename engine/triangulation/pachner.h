
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file triangulation/pachner.h
 *  \brief Explains terminology used with Pachner moves on triangulations.
 */

/*! \page pachner Pachner moves on triangulations
 *
 * A Pachner move on a triangulated \a n-manifold \a N works as follows:
 *
 * - One finds a \a k-face \a F of \a N whose link is combinatorially
 *   isomorphic to the link of a \a k-face in the boundary of triangulated
 *   (\a n+1)-simplex.
 *
 * - One then replaces the star of \a F with the complement of the star of
 *   a \a k-face in the boundary of the (\a n+1)-simplex.  This complement
 *   is the star of an (\a n-\a k)-face.
 *
 * We call the \a k-face the \e attaching face; this is the input to
 * the Pachner move.  The corresponding (\a n-\a k)-face in the
 * new triangulation is called the \e belt face (in analogy with surgery).
 *
 * Since the star of a \a k-face in the boundary of an (\a n+1)-simplex has
 * (\a n+1-\a k) top-dimensional simplices, the star of the belt
 * (\a n-\a k)-face has (\a k+1) top-dimensional simplices.
 * The Pachner move therefore swaps (\a n+1-\a k) top-dimensional simplices
 * for (\a k+1).
 */

