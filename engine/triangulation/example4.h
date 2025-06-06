
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
#include "link/link.h"
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
         * `S² × S²`.  This triangulation is minimal.
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
         * `S³ × S¹`.  This is identical to calling the
         * generic routine sphereBundle().
         *
         * \return the product `S³ × S¹`.
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
         * product space `T² × T²`.
         *
         * \return the product space `T² × T²`.
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
         * \name Constructions from 3-Manifold Triangulations and Links
         */
        /*@{*/

        /**
         * Returns a triangulation of the product `M × I`,
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
         * The product is created as follows: for each original tetrahedron of
         * \a M we build a tetrahedral prism containing 82 pentachora, and we
         * then glue these prisms together in a manner that follows the
         * gluings of the original tetrahedra.  It is highly recommended that
         * you call Triangulation<4>::simplify() afterwards if you do not need
         * to preserve the combinatorial structure.
         *
         * For any simplex in \a base that is locked, all of the pentachora
         * and internal facets of the corresponding prism will also be locked.
         * For any triangular facet of \a base that is locked, all of the
         * tetrahedral facets on the corresponding prism wall(s) will likewise
         * be locked.
         *
         * Note that the current construction does _not_ give an oriented
         * triangulation (due to the specific choice of labelling); this may
         * change in a future version of Regina.
         *
         * \warning If the given 3-manifold triangulation has ideal boundary,
         * then you will obtain an invalid 4-manifold triangulation as a result.
         *
         * \param base the 3-manifold triangulation \a M, as described above.
         * \return the product `M × I`.
         */
        static Triangulation<4> iBundle(const Triangulation<3>& base);

        /**
         * Returns a triangulation of the product `M × S1`,
         * where \a M is the given 3-manifold triangulation.
         * This simply calls iBundle() and then glues together the
         * two copies of \a M on the boundary.
         *
         * The product is created as follows.  For each original tetrahedron of
         * \a M, we build a tetrahedral prism containing 82 pentachora.  We then
         * glue these prisms together in a manner that follows the gluings of
         * the original tetrahedra, and we also glue together the two
         * tetrahedra at each end of every prism.  It is highly recommended
         * that you call Triangulation<4>::simplify() afterwards if you do not
         * need to preserve the combinatorial structure.
         *
         * For any simplex in \a base that is locked, all of the pentachora
         * and internal facets of the corresponding prism will also be locked,
         * as well as the two tetrahedra at each end of the prism (which will
         * be glued together, as explained above).  For any triangular facet of
         * \a base that is locked, all of the tetrahedral facets on the
         * corresponding prism wall(s) will likewise be locked.
         *
         * Note that the current construction does _not_ give an oriented
         * triangulation (due to the specific choice of labelling); this may
         * change in a future version of Regina.
         *
         * \warning If the given 3-manifold triangulation has ideal boundary,
         * then you will obtain an invalid 4-manifold triangulation as a result.
         *
         * \param base the 3-manifold triangulation \a M, as described above.
         * \return the product `M × S1`.
         */
        static Triangulation<4> s1Bundle(const Triangulation<3>& base);

        /**
         * Returns a triangulation of the given 3-manifold spun around its
         * boundary.
         *
         * Let `M` be the given 3-manifold, with real boundary `∂M`.  This
         * constructs a 4-manifold from `M` as follows:
         *
         * - First we build the product `M × S1` in a similar way to s1Bundle().
         *   That is: we build a tetrahedral prism for each original tetrahedron
         *   of \a M, glue the top and bottom tetrahedra of each prism together,
         *   and glue the walls of the prisms together according to the gluings
         *   between the original tetrahedra of \a M.
         *
         * - Then, for each point `b` on the real boundary `∂M`, we collapse
         *   the fibre `b × S1` to a single point.  (Equivalently, we attach a
         *   copy of `∂M × D2` to the product `M × S1` so that, for each point
         *   `b` on the boundary `∂M`, the fibre `b × S1` becomes the boundary
         *   of the corresponding disc `b × D2`.)  We implement this as follows:
         *   for each boundary facet of \a M, we fold the corresponding prism
         *   wall in half, so that the top half folds onto the bottom.
         *
         * - If one or more of boundary facets of \a M are locked, then the
         *   corresponding prism walls will _not_ folded onto themselves;
         *   that is, the fibres over those parts of the boundary will _not_ be
         *   collapsed.  See the section below on locks for further details.
         *
         * Regarding real versus ideal boundary:
         *
         * - The second step (collapsing fibres) only acts on _real_ boundary;
         *   that is, points `b` that lie on boundary triangles of `M`.  It
         *   ignores ideal boundary, in the sense that ideal vertices will just
         *   be transformed as part of the product `M × S1` (the first step),
         *   without the subsequent collapse/filling operation.
         *
         * - As a result, any ideal vertices of `M` will produce invalid edges
         *   in the resulting 4-maifold triangulation (i.e., edges whose links
         *   are the same surfaces as the links of the original ideal vertices
         *   of `M`).
         *
         * Like s1Bundle(), the resulting triangulation will be very large:
         * it creates 82 pentachora for each original tetrahedron of \a M.
         * It is highly recommended that you call Triangulation<4>::simplify()
         * afterwards if you do not need to preserve the combinatorial
         * structure.
         *
         * This routine handles locks as follows:
         *
         * - For any simplex in \a base that is locked, all of the pentachora
         *   and internal facets of the corresponding prism will also be locked,
         *   as well as the two tetrahedra at each end of the prism (which will
         *   be glued together, as explained above).
         *
         * - For any internal triangular facet of \a base that is locked, all
         *   of the tetrahedral facets on the corresponding prism wall(s) will
         *   be locked.
         *
         * - For any boundary facet of \a base that is locked, the corresponding
         *   prism wall will _not_ be folded onto itself (i.e, the corresponding
         *   fibres will _not_ be collapsed).  Instead, all of the tetrahedral
         *   facets on that prism wall will be locked, and will remain as
         *   boundary facets of the final 4-dimensional triangulation.
         *
         * - If \a base has a mix of locked and unlocked boundary facets, you
         *   should aim to ensure that the locked and unlocked regions are
         *   separated by embedded curves in the boundary of \a M (i.e., there
         *   are no "pinch points" where the local picture has two or more
         *   locked regions meeting two or more unlocked regions).  Otherwise
         *   you may find that these pinch points create invalid edges in the
         *   resulting 4-manifold triangulation (specifically, edges whose
         *   links are 2-spheres with two or more punctures).
         *
         * Note that the current construction does _not_ give an oriented
         * triangulation (due to the specific choice of labelling); this may
         * change in a future version of Regina.
         *
         * \param base the 3-manifold triangulation \a M, as described above.
         * \return the 4-manifold obtained by spinning \a M around its boundary,
         * as described above.
         */
        static Triangulation<4> boundarySpin(const Triangulation<3>& base);

        /**
         * Returns a bundle formed from a given 3-manifold and a given
         * monodromy.
         *
         * Specifically, let \a M be the given 3-manifold triangulation.
         * This routine builds the bundle `M × I`, and then
         * identifies the two copies of \a M on the boundary according
         * to the given homeomorphism from \a M to itself.
         * The homeomorphism must be expressed as a combinatorial
         * automorphism, which means that for a non-trivial monodromy you
         * may need to do some work to find a sufficiently symmetric
         * 3-manifold triangulation to begin with.
         *
         * The product is created as follows.  For each original tetrahedron of
         * \a M, we build a tetrahedral prism containing 82 pentachora.  We then
         * glue these prisms together in a manner that follows the gluings of
         * the original tetrahedra.  Moreover, we take the two copies of \a M
         * that are formed from the tetrahedra at the two ends of these prisms,
         * and glue these together according to the given monodromy.  It is
         * highly recommended that you call Triangulation<4>::simplify()
         * afterwards if you do not need to preserve the combinatorial
         * structure.
         *
         * For any simplex in \a base that is locked, all of the pentachora
         * and internal facets of the corresponding prism will also be locked.
         * For any triangular facet of \a base that is locked, all of the
         * tetrahedral facets on the corresponding prism wall(s) will likewise
         * be locked.  The two tetrahedra at the ends of each prism will _not_
         * be locked (these are the tetrahedra along which the two copies of
         * \a M are identified using the given monodromy).
         *
         * Note that the current construction does _not_ give an oriented
         * triangulation (due to the specific choice of labelling); this may
         * change in a future version of Regina.
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

        /**
         * Returns an ideal triangulation of the complement of the 2-knot
         * obtained by spinning the given 1-knot (without twisting).  The knot
         * to be spun is passed as the first argument; the second (optional)
         * argument allows you to specify where the knot should be broken open
         * when carrying out the spinning construction.
         *
         * The spinning construction is described by Artin in "Zur Isotopie
         * zweidimensionaler Flächen im R_4", Abh. Math. Sem. Univ. Hamburg
         * 4 (1925), no. 1, 174-177.
         *
         * The final triangulation might (or might not) still contain internal
         * vertices, in addition to the one ideal vertex that represents the
         * 2-knot itself.
         *
         * \warning This routine could be slow, even when \a knot has very few
         * crossings.  This is because it typically goes via intermediate
         * triangulations with thousands or even tens of thousands of
         * pentachora, and simplifying such triangulations takes time.
         *
         * \pre The argument \a knot is a classical knot diagram.  That is, the
         * link diagram is not virtual, and has exactly one link component.
         *
         * \exception FailedPrecondition The given link diagram is empty, has
         * multiple components, and/or is virtual (as opposed to classical).
         *
         * \param knot the knot to be spun.
         * \param breakOpen indicates where to break open the given knot diagram
         * when performing the spinning construction.  See the StrandRef
         * documentation for the convention on how arcs are represented using
         * StrandRef objects.  This may be a null reference (the default), in
         * which case this routine will choose an arbitrary location to break
         * the knot open.
         * \return an ideal triangulation of the resulting 2-knot.
         */
        static Triangulation<4> spun(const Link& knot,
            StrandRef breakOpen = {});

        /*@}*/
        /**
         * (end: Constructions from 3-Manifold Triangulations and Links)
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

