
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/*! \file dim4exampletriangulation.h
 *  \brief Offers some example 4-manifold triangulations as starting
 *  points for testing code or getting used to Regina.
 */

#ifndef __DIM4EXAMPLETRIANGULATION_H
#ifndef __DOXYGEN
#define __DIM4EXAMPLETRIANGULATION_H
#endif

namespace regina {

class Dim4Triangulation;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * This class offers routines for constructing a variety of sample 4-manifold
 * triangulations.  These triangulations may be useful for testing new
 * code, or for simply getting a feel for how Regina works.
 *
 * At the present time only a handful of triangulations are available
 * (in contrast to the 3-dimensional NExampleTriangulation, which offers
 * many interesting examples).  The authors hope to expand this list
 * of 4-dimensional examples as they become more familiar with 4-manifold
 * triangulations.
 *
 * The sample triangulations offered here may prove especially useful in
 * Regina's scripting interface, where working with pre-existing files
 * is more complicated than in the GUI.
 *
 * Note that each of these routines constructs a new triangulation from
 * scratch.  It is up to the caller of each routine to destroy the
 * triangulation that is returned.
 */
class Dim4ExampleTriangulation {
    public:
        /**
         * \name Closed Triangulations
         */
        /*@{*/

        /**
         * Returns a two-pentachoron triangulation of the 4-sphere.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim4Triangulation* fourSphere();

        /**
         * Returns a four-pentachoron triangulation of real projective
         * 4-space.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         * @author Ryan Budney
         */
        static Dim4Triangulation* rp4();

        /*@}*/
        /**
         * (end: Closed Triangulations)
         */
};

/*@}*/

} // namespace regina

#endif

