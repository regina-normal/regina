
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file triangulation/example4.h
 *  \brief Offers some example 4-dimensional triangulations as starting
 *  points for testing code or getting used to Regina.
 */

#ifndef __REGINA_EXAMPLE4_H
#ifndef __DOXYGEN
#define __REGINA_EXAMPLE4_H
#endif

#include "regina-core.h"
#include "triangulation/dim4.h"
#include "triangulation/detail/example.h"

namespace regina {

/**
 * Offers routines for constructing a variety of sample 4-dimensional
 * triangulations.
 *
 * This is a specialisation of the generic Example class template; see the
 * generic Example template documentation for a general overview of how the
 * example triangulation classes work.  In Python, you can read this generic
 * documentation by looking at a higher dimension: try `help(Example5)`.
 *
 * This 4-dimensional specialisation offers significant extra functionality,
 * by providing several more hard-coded and parameterised constructions.
 *
 * \ingroup triangulation
 */
template <>
class Example<4> : public detail::ExampleBase<4> {
    public:
        /**
         * \name Closed Triangulations
         */
        /*@{*/

        /**
         * Returns a two-pentachoron triangulation of the 4-sphere.
         * This is identical to calling the generic routine sphere().
         *
         * \return a two-pentachoron 4-sphere.
         */
        static Triangulation<4> fourSphere();

        /**
         * Returns the standard six-pentachoron triangulation of the 4-sphere
         * as the boundary of a 5-simplex.
         * This is identical to calling the generic routine simplicialSphere().
         *
         * \return the standard simplicial 4-sphere.
         */
        static Triangulation<4> simplicialFourSphere();

        /**
         * Returns a four-pentachoron triangulation of real projective
         * 4-space.
         *
         * \return real projective 4-space.
         */
        static Triangulation<4> rp4();

        /**
         * Returns a four-pentachoron triangulation of the standard
         * complex projective plane.  This triangulation is minimal.
         *
         * Under the orientation convention that we use for intersection
         * forms, this triangulation gives the "plain" `CP²` with
         * intersection form [1], not the reflected `CP²` with
         * intersection form [-1].
         *
         * \return the standard complex projective plane.
         */
        static Triangulation<4> cp2();

        /**
         * Returns a six-pentachoron triangulation of the standard product
         * `S² x S²`.  This triangulation is minimal.
         *
         * \return the standard product of two 2-spheres.
         */
        static Triangulation<4> s2xs2();

        /**
         * Returns a six-pentachoron triangulation of the twisted product
         * `S² x~ S²`.
         * This manifold is diffeomorphic to `CP² # -CP²`, where
         * `-CP²` denotes `CP²` with its orientation reversed.
         * This triangulation is minimal.
         *
         * \return the twisted product of two 2-spheres.
         */
        static Triangulation<4> s2xs2Twisted();

        /**
         * Returns a two-pentachoron triangulation of the product space
         * `S³ x S¹`.  This is identical to calling the
         * generic routine sphereBundle().
         *
         * \return the product `S³ x S¹`.
         */
        static Triangulation<4> s3xs1();

        /**
         * Returns a two-pentachoron triangulation of the twisted product
         * space `S³ x~ S¹`.  This is identical to calling the
         * generic routine twistedSphereBundle().
         *
         * \return the twisted product `S³ x~ S¹`.
         */
        static Triangulation<4> s3xs1Twisted();

        /**
         * Returns a triangulation of the standard 4-torus; that is, the
         * product space `T² x T²`.
         *
         * \return the product space `T² x T²`.
         */
        static Triangulation<4> fourTorus();

        /**
         * Returns a triangulation of the standard K3 surface.
         *
         * Be warned: this triangulation is fairly large.
         *
         * \return the K3 surface.
         */
        static Triangulation<4> k3();

        /*@}*/
        /**
         * (end: Closed Triangulations)
         */

        /**
         * \name Ideal Triangulations
         */
        /*@{*/

        /**
         * Returns a two-pentachoron triangulation of a Cappell-Shaneson 2-knot
         * complement in the 4-sphere.  This triangulation is described and
         * analysed in "Triangulating a Cappell-Shaneson knot complement",
         * Budney, Burton and Hillman, Mathematical Research Letters 19 (2012),
         * no. 5, 1117-1126.
         *
         * \return a Cappell-Shaneson 2-knot complement.
         */
        static Triangulation<4> cappellShaneson();

        /*@}*/
        /**
         * (end: Ideal Triangulations)
         */

        /**
         * \name Constructions from 3-Manifold Triangulations
         */
        /*@{*/

        /**
         * Returns a triangulation of the product `M x I`,
         * where \a M is the given 3-manifold triangulation.
         *
         * The boundary of this product will consist of two copies of \a M,
         * both combinatorially isomorphic to the original triangulation.
         * If \a n is the number of tetrahedra in \a M, then the first copy of
         * \a M on the boundary is obtained by mapping vertices 0,1,2,3 of
         * tetrahedron \a i of \a M to vertices 0,1,2,3 of pentachoron \a i,
         * and the second copy is obtained by mapping vertices 0,1,2,3 of
         * tetrahedron \a i of \a M to vertices 0,1,2,3 of pentachoron \a n+i.
         *
         * The product itself will contain 82 pentachora for each
         * original tetrahedron of \a M, and will contain many internal
         * vertices.  It is highly recommended that you call
         * Triangulation<4>::simplify() afterwards if you do
         * not need to preserve the combinatorial structure.
         *
         * \warning If the given 3-manifold triangulation has ideal boundary,
         * then you will obtain an invalid 4-manifold triangulation as a result.
         *
         * \param base the 3-manifold triangulation \a M, as described above.
         * \return the product `M x I`.
         */
        static Triangulation<4> iBundle(const Triangulation<3>& base);

        /**
         * Returns a triangulation of the product `M x S1`,
         * where \a M is the given 3-manifold triangulation.
         * This simply calls iBundle() and then glues together the
         * two copies of \a M on the boundary.
         *
         * The product will contain 82 pentachora for each
         * original tetrahedron of \a M, and will contain many internal
         * vertices.  It is highly recommended that you call
         * Triangulation<4>::simplify() afterwards if you do
         * not need to preserve the combinatorial structure.
         *
         * \warning If the given 3-manifold triangulation has ideal boundary,
         * then you will obtain an invalid 4-manifold triangulation as a result.
         *
         * \param base the 3-manifold triangulation \a M, as described above.
         * \return the product `M x S1`.
         */
        static Triangulation<4> s1Bundle(const Triangulation<3>& base);

        /**
         * Returns a bundle formed from a given 3-manifold and a given
         * monodromy.
         *
         * Specifically, let \a M be the given 3-manifold triangulation.
         * This routine builds the bundle `M x I`, and then
         * identifies the two copies of \a M on the boundary according
         * to the given homeomorphism from \a M to itself.
         * The homeomorphism must be expressed as a combinatorial
         * automorphism, which means that for a non-trivial monodromy you
         * may need to do some work to find a sufficiently symmetric
         * 3-manifold triangulation to begin with.
         *
         * The resulting manifold will contain 82 pentachora for each
         * original tetrahedron of \a M, and will contain many internal
         * vertices.  It is highly recommended that you call
         * Triangulation<4>::simplify() afterwards if you do
         * not need to preserve the combinatorial structure.
         *
         * \pre The given monodromy must be an isomorphism from \a M to
         * itself; that is, a combinatorial automorphism.
         *
         * \warning If the given 3-manifold triangulation has ideal boundary,
         * then you will obtain an invalid 4-manifold triangulation as a result.
         *
         * \param base the 3-manifold triangulation \a M, as described above.
         * \param monodromy the homeomorphism from \a M to itself, as
         * described above.
         * \return the requested bundle.
         */
        static Triangulation<4> bundleWithMonodromy(
            const Triangulation<3>& base,
            const Isomorphism<3>& monodromy);

        /*@}*/
        /**
         * (end: Constructions from 3-Manifold Triangulations)
         */
};

inline Triangulation<4> Example<4>::fourSphere() {
    return sphere();
}

inline Triangulation<4> Example<4>::simplicialFourSphere() {
    return simplicialSphere();
}

inline Triangulation<4> Example<4>::s3xs1() {
    return sphereBundle();
}

inline Triangulation<4> Example<4>::s3xs1Twisted() {
    return twistedSphereBundle();
}

} // namespace regina

#endif

