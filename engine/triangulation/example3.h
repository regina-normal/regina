
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

/*! \file triangulation/example3.h
 *  \brief Offers some example 3-dimensional triangulations as starting
 *  points for testing code or getting used to Regina.
 */

#ifndef __EXAMPLE3_H
#ifndef __DOXYGEN
#define __EXAMPLE3_H
#endif

#include "regina-core.h"
#include "triangulation/forward.h"
#include "triangulation/detail/example.h"

namespace regina {

/**
 * \addtogroup triangulation Triangulations
 * Triangulations of manifolds in all supported dimensions.
 * @{
 */

/**
 * Offers routines for constructing a variety of sample 3-dimensional
 * triangulations.
 *
 * This is a specialisation of the generic Example class template; see
 * the Example template documentation for a general overview of how the example
 * triangulation classes work.
 *
 * This 3-dimensional specialisation offers significant extra functionality,
 * by providing several more hard-coded and parameterised constructions.
 */
template <>
class REGINA_API Example<3> : public detail::ExampleBase<3> {
    public:
        /**
         * \name Closed Triangulations
         */
        /*@{*/

        /**
         * Returns a one-tetrahedron triangulation of the 3-sphere.
         * This is \e different from the generic routine sphere(),
         * which uses two tetrahedra instead.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* threeSphere();

        /**
         * Returns the two-tetrahedron triangulation of the 3-sphere
         * that is dual to Bing's house with two rooms.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* bingsHouse();

        /**
         * Returns a two-tetrahedron triangulation of the product space
         * <tt>S^2 x S^1</tt>.
         * This is identical to calling the generic routine sphereBundle().
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* s2xs1();

        /**
         * Returns a three-tetrahedron triangulation of the non-orientable
         * product space <tt>RP^2 x S^1</tt>.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* rp2xs1();

        /**
         * Returns a triangulation of the connected sum
         * <tt>RP^3 # RP^3</tt>.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* rp3rp3();

        /**
         * Returns a triangulation of the lens space <tt>L(p,q)</tt>.
         *
         * The triangulation uses a layered lens space, which is
         * conjectured (but not proven in all cases) to be the
         * triangulation requiring the fewest tetrahedra.
         *
         * \pre \a p \> \a q \>= 0 unless (<i>p</i>,<i>q</i>) = (0,1).
         * \pre gcd(\a p, \a q) = 1.
         *
         * @param p a parameter of the desired lens space.
         * @param q a parameter of the desired lens space.
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* lens(size_t p, size_t q);

        /**
         * Returns the five-tetrahedron triangulation of the
         * Poincare homology sphere.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* poincareHomologySphere();

        /**
         * Returns a nine-tetrahedron minimal triangulation of the Weeks
         * manifold.  The Weeks manifold is the smallest-volume closed
         * hyperbolic 3-manifold, with a volume of roughly 0.9427.
         * Note that there are nine minimal triangulations of the Weeks
         * manifold (of course this routine returns just one).
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* weeks();

        /**
         * Returns a one-vertex triangulation of the Weber-Seifert
         * dodecahedral space.
         *
         * This 3-manifold is described in "Die beiden Dodekaederraume",
         * C. Weber and H. Seifert, Math. Z. 37 (1933), no. 1, 237-253.
         * The triangulation returned by this routine (with 23 tetrahedra)
         * is given in "The Weber-Seifert dodecahedral space is non-Haken",
         * Benjamin A. Burton, J. Hyam Rubinstein and Stephan Tillmann,
         * Trans. Amer. Math. Soc. 364:2 (2012), pp. 911-932.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* weberSeifert();

        /**
         * Returns the nine-tetrahedron closed orientable hyperbolic
         * 3-manifold with volume 0.94270736.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* smallClosedOrblHyperbolic();

        /**
         * Returns the eleven-tetrahedron closed non-orientable hyperbolic
         * 3-manifold with volume 2.02988321.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* smallClosedNonOrblHyperbolic();

        /**
         * Returns the boundary 3-sphere of the regular 600-cell.
         * This is a triangulation of the 3-sphere that is a simplicial
         * complex, and in which every edge has degree five.
         *
         * The triangulation was extracted from the Benedetti-Lutz
         * library of triangulations.  See:
         * http://page.math.tu-berlin.de/~lutz/stellar/library_of_triangulations.html
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* sphere600();

        /*@}*/
        /**
         * \name Finite Bounded Triangulations
         */
        /*@{*/

        /**
         * Returns the layered solid torus <tt>LST(a,b,c)</tt>.
         *
         * This is a parameterised triangulation of the solid torus.
         * It has two boundary triangles and three boundary edges,
         * and the meridional disc of the solid torus cuts these
         * boundary edges \a a, \a b and \a c times respectively.
         *
         * Only the parameters \a a and \a b are passed as arguments to
         * this routine.  The third parameter \a c will be deduced
         * automatically as \a c = (\a a + \a b).
         *
         * \pre gcd(\a a, \a b) = 1.
         *
         * @param a the first parameter of the layered solid torus.
         * @param b the second parameter of the layered solid torus.
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* lst(size_t a, size_t b);

        /**
         * Returns a triangulation of the solid Klein bottle.
         * This is isomorphic to the triangulation returned by the
         * generic routine twistedBallBundle(), though it will have a
         * different packet label.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* solidKleinBottle();

        /*@}*/
        /**
         * \name Ideal Triangulations
         */
        /*@{*/

        /**
         * Returns a two-tetrahedron ideal triangulation of the figure
         * eight knot complement.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* figureEight();

        /**
         * Returns a two-tetrahedron ideal triangulation of the trefoil
         * knot complement.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* trefoil();

        /**
         * Returns a four-tetrahedron ideal triangulation of the
         * Whitehead link complement.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* whiteheadLink();

        /**
         * Returns the one-tetrahedron ideal triangulation of the
         * non-orientable Gieseking manifold.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* gieseking();

        /**
         * Returns a triangulation of a solid genus two torus with a
         * cusped boundary.  This triangulation has one internal finite
         * vertex and one genus two ideal vertex.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static Triangulation<3>* cuspedGenusTwoTorus();

        /*@}*/
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef NExampleTriangulation, you should
 * use the full class name Example<3>.
 */
REGINA_DEPRECATED typedef Example<3> NExampleTriangulation;

inline Triangulation<3>* Example<3>::s2xs1() {
    return sphereBundle();
}

/*@}*/

} // namespace regina

#endif

