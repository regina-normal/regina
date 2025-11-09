
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file triangulation/example2.h
 *  \brief Offers some example 2-dimensional triangulations as starting
 *  points for testing code or getting used to Regina.
 */

#ifndef __REGINA_EXAMPLE2_H
#ifndef __DOXYGEN
#define __REGINA_EXAMPLE2_H
#endif

#include "regina-core.h"
#include "triangulation/dim2.h"
#include "triangulation/detail/example.h"

namespace regina {

/**
 * Offers routines for constructing a variety of sample 2-dimensional
 * triangulations.
 *
 * This is a specialisation of the generic Example class template; see the
 * generic Example template documentation for a general overview of how the
 * example triangulation classes work.  In Python, you can read this generic
 * documentation by looking at a higher dimension: try `help(Example5)`.
 *
 * This 2-dimensional specialisation offers significant extra functionality,
 * by providing several more hard-coded constructions.
 *
 * \ingroup triangulation
 */
template <>
class Example<2> : public detail::ExampleBase<2> {
    public:

        //TODO The plan is for orientable() to always return minimal.

        /**
         * Returns a triangulation of the given orientable surface.
         *
         * If the number of punctures is 0, then the resulting triangulation
         * will be minimal (which, for positive genus, means there is exactly
         * one vertex).
         *
         * \param genus the genus of the surface; this must be greater
         * than or equal to zero.
         * \param punctures the number of punctures in the surface;
         * this must be greater than or equal to zero.
         * \return the requested orientable surface.
         */
        static Triangulation<2> orientable(
            unsigned genus, unsigned punctures);

        //TODO The plan is for nonOrientable() to always return minimal.

        /**
         * Returns a triangulation of the given non-orientable surface.
         *
         * If the number of punctures is 0 or 1, then the resulting
         * triangulation will be minimal (which, with the exception of
         * the projective plane, means there is exactly one vertex).
         *
         * \param genus the non-orientable genus of the surface, i.e.,
         * the number of crosscaps that it contains; this must be greater
         * than or equal to one.
         * \param punctures the number of punctures in the surface;
         * this must be greater than or equal to zero.
         * \return the requested non-orientable surface.
         *
         * \author Alex He, B.B.
         */
        static Triangulation<2> nonOrientable(
            unsigned genus, unsigned punctures);

        /**
         * Returns the four-triangle 2-sphere formed from the boundary
         * of a tetrahedron.  This is identical to the triangulation
         * returned by the generic routine simplicialSphere().
         *
         * \return the tetrahedral sphere.
         */
        static Triangulation<2> sphereTetrahedron();

        /**
         * Returns the eight-triangle 2-sphere formed from the boundary
         * of an octahedron.
         *
         * \return the octahedral sphere.
         */
        static Triangulation<2> sphereOctahedron();

        /**
         * Returns an <i>n</i>-triangle disc (<i>n</i> = 1 by default).
         *
         * The default 1-triangle disc is identical to the triangulation
         * returned by the generic routine ball().
         *
         * This routine always returns an oriented triangulation of an
         * (<i>n</i> + 2)-sided polygon. In particular, the returned
         * triangulation always uses the minimum number of edge
         * identifications for an <i>n</i>-triangle disc.
         *
         * \return the disc.
         */
        static Triangulation<2> disc(unsigned n = 1);

        /**
         * Returns a two-triangle annulus.
         * This is identical to the triangulation returned by the generic
         * routine ballBundle().
         *
         * \return the annulus.
         */
        static Triangulation<2> annulus();

        /**
         * Returns a one-triangle Mobius band.  This is identical to the
         * triangulation returned by the generic routine twistedBallBundle().
         *
         * \return the Mobius band.
         */
        static Triangulation<2> mobius();

        /**
         * Returns a two-triangle torus.
         * This is identical to the triangulation returned by the generic
         * routine sphereBundle().
         *
         * \return the torus.
         */
        static Triangulation<2> torus();

        /**
         * Returns a two-triangle projective plane.
         *
         * \return the projective plane.
         */
        static Triangulation<2> rp2();

        /**
         * Returns a two-triangle Klein bottle.  This is identical to the
         * triangulation returned by the generic routine twistedSphereBundle().
         *
         * \return the Klein bottle.
         */
        static Triangulation<2> kb();
};

inline Triangulation<2> Example<2>::sphereTetrahedron() {
    return simplicialSphere();
}

inline Triangulation<2> Example<2>::torus() {
    return sphereBundle();
}

inline Triangulation<2> Example<2>::kb() {
    return twistedSphereBundle();
}


inline Triangulation<2> Example<2>::disc(unsigned n) {
    //TODO Construct polygonal discs with any number of triangles.
    return ball();
}

inline Triangulation<2> Example<2>::annulus() {
    return ballBundle();
}

inline Triangulation<2> Example<2>::mobius() {
    return twistedBallBundle();
}

} // namespace regina

#endif

