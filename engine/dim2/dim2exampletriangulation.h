
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/*! \file dim2/dim2exampletriangulation.h
 *  \brief Offers several example 2-manifold triangulations as starting
 *  points for testing code or getting used to Regina.
 */

#ifndef __NEXAMPLETRIANGULATION_H
#ifndef __DOXYGEN
#define __NEXAMPLETRIANGULATION_H
#endif

#include "regina-core.h"

namespace regina {

template <int> class Triangulation;
typedef Triangulation<2> Dim2Triangulation;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * This class offers routines for constructing sample 2-manifold triangulations
 * of various types.  These triangulations may be useful for testing new
 * code, or for simply getting a feel for how Regina works.
 *
 * The sample triangulations offered here may prove especially useful in
 * Regina's scripting interface, where working with pre-existing files
 * is more complicated than in the GUI.
 *
 * Note that each of these routines constructs a new triangulation from
 * scratch.  It is up to the caller of each routine to destroy the
 * triangulation that is returned.
 */
class REGINA_API Dim2ExampleTriangulation {
    public:
        /**
         * Returns a triangulation of the given orientable surface.
         *
         * @param genus the genus of the surface; this must be greater
         * than or equal to zero.
         * @param punctures the number of punctures in the surface;
         * this must be greater than or equal to zero.
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim2Triangulation* orientable(
            unsigned genus, unsigned punctures);

        /**
         * Returns a triangulation of the given non-orientable surface.
         *
         * @param genus the non-orientable genus of the surface, i.e.,
         * the number of crosscaps that it contains; this must be greater
         * than or equal to one.
         * @param punctures the number of punctures in the surface;
         * this must be greater than or equal to zero.
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim2Triangulation* nonOrientable(
            unsigned genus, unsigned punctures);

        /**
         * Returns a two-triangle 2-sphere.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim2Triangulation* sphere();

        /**
         * Returns the four-triangle 2-sphere formed from the boundary
         * of a tetrahedron.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim2Triangulation* sphereTetrahedron();

        /**
         * Returns the eight-triangle 2-sphere formed from the boundary
         * of an octahedron.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim2Triangulation* sphereOctahedron();

        /**
         * Returns a one-triangle disc.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim2Triangulation* disc();

        /**
         * Returns a two-triangle annulus.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim2Triangulation* annulus();

        /**
         * Returns a one-triangle Mobius band.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim2Triangulation* mobius();

        /**
         * Returns a two-triangle torus.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim2Triangulation* torus();

        /**
         * Returns a two-triangle projective plane.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim2Triangulation* rp2();

        /**
         * Returns a two-triangle Klein bottle.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim2Triangulation* kb();
};

/*@}*/

} // namespace regina

#endif

