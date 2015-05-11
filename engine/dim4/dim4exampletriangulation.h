
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file dim4/dim4exampletriangulation.h
 *  \brief Offers some example 4-manifold triangulations as starting
 *  points for testing code or getting used to Regina.
 */

#ifndef __DIM4EXAMPLETRIANGULATION_H
#ifndef __DOXYGEN
#define __DIM4EXAMPLETRIANGULATION_H
#endif

#include "regina-core.h"

namespace regina {

template <int> class Isomorphism;
template <int> class Triangulation;
typedef Isomorphism<3> NIsomorphism;
typedef Triangulation<3> NTriangulation;
typedef Triangulation<4> Dim4Triangulation;

/**
 * \weakgroup dim4
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
class REGINA_API Dim4ExampleTriangulation {
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
         * Returns the standard six-pentachoron triangulation of the 4-sphere
         * as the boundary of a 5-simplex. 
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine. 
         */
        static Dim4Triangulation* simplicialFourSphere();

        /**
         * Returns a four-pentachoron triangulation of real projective
         * 4-space.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         * @author Ryan Budney
         */
        static Dim4Triangulation* rp4();

        /**
         * Returns a two-pentachoron triangulation of the product space
         * <tt>S^3 x S^1</tt>.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim4Triangulation* s3xs1();

        /**
         * Returns a two-pentachoron triangulation of the twisted product
         * space <tt>S^3 x~ S^1</tt>.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim4Triangulation* s3xs1Twisted();

        /*@}*/
        /**
         * (end: Closed Triangulations)
         */

        /**
         * \name Ideal Triangulations
         */
        /*@{*/

        /**
         * Returns a two-pentachoron triangulation of a Cappell-Shaneson
         * 2-knot complement in the 4-sphere.  This triangulation is
         * described and analysed in "Triangulating a Cappell-Shaneson
         * knot complement", Budney, Burton and Hillman, Preprint,
         * arXiv:1109.3899, Sep 2011.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim4Triangulation* cappellShaneson();

        /*@}*/
        /**
         * (end: Ideal Triangulations)
         */

        /**
         * \name Constructions from 3-Manifold Triangulations
         */
        /*@{*/

        /**
         * Returns a double cone over the given 3-manifold triangulation.
         *
         * If the given 3-manifold is <tt>M</tt>, this returns an ideal
         * triangulation of the product <tt>M x I</tt> (with two ideal
         * boundary components).  A copy of the original 3-manifold
         * triangulation \a base can be found at the centre of this
         * construction, formed from the tetrahedra that sit between the
         * two ideal vertices.
         *
         * Note that, as a special case, if <tt>M</tt> is either a
         * 3-sphere or a 3-ball, then this routine returns a 4-sphere or
         * a 4-ball (since "ideal spheres" and "ideal balls" just become
         * regular internal and boundary vertices respectively).
         *
         * This construction is essentially the suspension of the
         * triangulation \a base.  We do not call it this however, since
         * from a topological point of view, to form the ideal triangulation
         * of <tt>M x I</tt> we "remove" the vertices at the apex of each cone.
         *
         * \warning If the given 3-manifold triangulation has any boundary
         * whatsoever (either real or ideal), then unless this 3-manifold
         * triangulation is a 3-ball you will obtain an invalid 4-manifold
         * triangulation as a result.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim4Triangulation* doubleCone(const NTriangulation& base);

        /**
         * Returns a single cone over the given 3-manifold triangulation.
         *
         * If the given 3-manifold is <tt>M</tt>, this returns a triangulation
         * of the product <tt>M x I</tt> that has one real boundary component
         * and one ideal boundary component.  The triangulation of the real
         * boundary component will be identical to the original 3-manifold
         * triangulation \a base.
         *
         * \warning If the given 3-manifold triangulation has any boundary
         * whatsoever (either real or ideal), then unless this 3-manifold
         * triangulation is a 3-ball you will obtain an invalid 4-manifold
         * triangulation as a result.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim4Triangulation* singleCone(const NTriangulation& base);

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
         * Dim4Triangulation::intelligentSimplify() afterwards if you do
         * not need to preserve the combinatorial structure.
         *
         * \warning If the given 3-manifold triangulation has ideal boundary,
         * then you will obtain an invalid 4-manifold triangulation as a result.
         *
         * @param base the 3-manifold triangulation \a M, as described above.
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim4Triangulation* iBundle(const NTriangulation& base);

        /**
         * Returns a triangulation of the product <tt>M x S1</tt>,
         * where \a M is the given 3-manifold triangulation.
         * This simply calls iBundle() and then glues together the
         * two copies of \a M on the boundary.
         *
         * The product will contain 82 pentachora for each
         * original tetrahedron of \a M, and will contain many internal
         * vertices.  It is highly recommended that you call
         * Dim4Triangulation::intelligentSimplify() afterwards if you do
         * not need to preserve the combinatorial structure.
         *
         * \warning If the given 3-manifold triangulation has ideal boundary,
         * then you will obtain an invalid 4-manifold triangulation as a result.
         *
         * @param base the 3-manifold triangulation \a M, as described above.
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Dim4Triangulation* s1Bundle(const NTriangulation& base);

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
         * Dim4Triangulation::intelligentSimplify() afterwards if you do
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
        static Dim4Triangulation* bundleWithMonodromy(
            const NTriangulation& base,
            const NIsomorphism& monodromy);

        /*@}*/
        /**
         * (end: Constructions from 3-Manifold Triangulations)
         */
};

/*@}*/

} // namespace regina

#endif

