
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

/*! \file triangulation/example4.h
 *  \brief Offers some example 4-dimensional triangulations as starting
 *  points for testing code or getting used to Regina.
 */

#ifndef __EXAMPLE4_H
#ifndef __DOXYGEN
#define __EXAMPLE4_H
#endif

#include "regina-core.h"
#include "triangulation/forward.h"
#include "triangulation/detail/example.h"

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Offers routines for constructing a variety of sample 4-dimensional
 * triangulations.
 *
 * This is a specialisation of the generic Example class template; see
 * the Example template documentation for a general overview of how the example
 * triangulation classes work.
 *
 * This 4-dimensional specialisation offers significant extra functionality,
 * by providing several more hard-coded and parameterised constructions.
 */
template <>
class REGINA_API Example<4> : public detail::ExampleBase<4> {
    public:
        /**
         * \name Closed Triangulations
         */
        /*@{*/

        /**
         * Returns a two-pentachoron triangulation of the 4-sphere.
         * This is identical to calling the generic routine sphere().
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<4>* fourSphere();

        /**
         * Returns the standard six-pentachoron triangulation of the 4-sphere
         * as the boundary of a 5-simplex.
         * This is identical to calling the generic routine simplicialSphere().
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<4>* simplicialFourSphere();

        /**
         * Returns a four-pentachoron triangulation of real projective
         * 4-space.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         * @author Ryan Budney
         */
        static Triangulation<4>* rp4();

        /**
         * Returns a two-pentachoron triangulation of the product space
         * <tt>S^3 x S^1</tt>.  This is identical to calling the
         * generic routine sphereBundle().
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<4>* s3xs1();

        /**
         * Returns a two-pentachoron triangulation of the twisted product
         * space <tt>S^3 x~ S^1</tt>.  This is identical to calling the
         * generic routine twistedSphereBundle().
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<4>* s3xs1Twisted();

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
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<4>* cappellShaneson();

        /*@}*/
        /**
         * (end: Ideal Triangulations)
         */

        /**
         * \name Constructions from 3-Manifold Triangulations
         */
        /*@{*/

        /**
         * Returns a triangulation of the product <tt>M x I</tt>,
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
         * Triangulation<4>::intelligentSimplify() afterwards if you do
         * not need to preserve the combinatorial structure.
         *
         * \warning If the given 3-manifold triangulation has ideal boundary,
         * then you will obtain an invalid 4-manifold triangulation as a result.
         *
         * @param base the 3-manifold triangulation \a M, as described above.
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<4>* iBundle(const Triangulation<3>& base);

        /**
         * Returns a triangulation of the product <tt>M x S1</tt>,
         * where \a M is the given 3-manifold triangulation.
         * This simply calls iBundle() and then glues together the
         * two copies of \a M on the boundary.
         *
         * The product will contain 82 pentachora for each
         * original tetrahedron of \a M, and will contain many internal
         * vertices.  It is highly recommended that you call
         * Triangulation<4>::intelligentSimplify() afterwards if you do
         * not need to preserve the combinatorial structure.
         *
         * \warning If the given 3-manifold triangulation has ideal boundary,
         * then you will obtain an invalid 4-manifold triangulation as a result.
         *
         * @param base the 3-manifold triangulation \a M, as described above.
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<4>* s1Bundle(const Triangulation<3>& base);

        /**
         * Returns a bundle formed from a given 3-manifold and a given
         * monodromy.
         *
         * Specifically, let \a M be the given 3-manifold triangulation.
         * This routine builds the bundle <tt>M x I</tt>, and then
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
         * Triangulation<4>::intelligentSimplify() afterwards if you do
         * not need to preserve the combinatorial structure.
         *
         * \pre The given monodromy must be an isomorphism from \a M to
         * itself; that is, a combinatorial automorphism.
         *
         * \warning If the given 3-manifold triangulation has ideal boundary,
         * then you will obtain an invalid 4-manifold triangulation as a result.
         *
         * @param base the 3-manifold triangulation \a M, as described above.
         * @param monodromy the homeomorphism from \a M to itself, as
         * described above.
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<4>* bundleWithMonodromy(
            const Triangulation<3>& base,
            const Isomorphism<3>& monodromy);

        /*@}*/
        /**
         * (end: Constructions from 3-Manifold Triangulations)
         */
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4ExampleTriangulation, you should
 * use the full class name Example<4>.
 */
REGINA_DEPRECATED typedef Example<4> Dim4ExampleTriangulation;

/*@}*/

inline Triangulation<4>* Example<4>::fourSphere() {
    return sphere();
}

inline Triangulation<4>* Example<4>::simplicialFourSphere() {
    return simplicialSphere();
}

inline Triangulation<4>* Example<4>::s3xs1() {
    return sphereBundle();
}

inline Triangulation<4>* Example<4>::s3xs1Twisted() {
    return twistedSphereBundle();
}

} // namespace regina

#endif

