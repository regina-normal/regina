
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

/*! \file triangulation/example3.h
 *  \brief Offers some example 3-dimensional triangulations as starting
 *  points for testing code or getting used to Regina.
 */

#ifndef __REGINA_EXAMPLE3_H
#ifndef __DOXYGEN
#define __REGINA_EXAMPLE3_H
#endif

#include "regina-core.h"
#include "triangulation/dim3.h"
#include "triangulation/detail/example.h"

namespace regina {

/**
 * \defgroup triangulation Triangulations
 * Triangulations of manifolds in all supported dimensions.
 */

/**
 * Offers routines for constructing a variety of sample 3-dimensional
 * triangulations.
 *
 * This is a specialisation of the generic Example class template; see the
 * generic Example template documentation for a general overview of how the
 * example triangulation classes work.  In Python, you can read this generic
 * documentation by looking at a higher dimension: try `help(Example5)`.
 *
 * This 3-dimensional specialisation offers significant extra functionality,
 * by providing several more hard-coded and parameterised constructions.
 *
 * \ingroup triangulation
 */
template <>
class Example<3> : public detail::ExampleBase<3> {
    public:
        /**
         * \name Closed Triangulations
         */
        /*@{*/

        /**
         * Returns a one-tetrahedron triangulation of the 3-sphere.
         * This is _different_ from the generic routine sphere(),
         * which uses two tetrahedra instead.
         *
         * \return a one-tetrahedron 3-sphere.
         */
        static Triangulation<3> threeSphere();

        /**
         * Returns the two-tetrahedron triangulation of the 3-sphere
         * that is dual to Bing's house with two rooms.
         *
         * \return a 3-sphere triangulation dual to Bing's house.
         */
        static Triangulation<3> bingsHouse();

        /**
         * Returns a two-tetrahedron triangulation of the product space
         * `S² x S¹`.
         * This is identical to calling the generic routine sphereBundle().
         *
         * \return the product space `S² x S¹`.
         */
        static Triangulation<3> s2xs1();

        /**
         * Returns a three-tetrahedron triangulation of the non-orientable
         * product space `RP² x S¹`.
         *
         * \return the product space `RP² x S¹`.
         */
        static Triangulation<3> rp2xs1();

        /**
         * Returns a triangulation of the connected sum
         * `RP³ # RP³`.
         *
         * \return the connected sum `RP³ # RP³`.
         */
        static Triangulation<3> rp3rp3();

        /**
         * Returns a triangulation of the lens space `L(p,q)`.
         *
         * The triangulation uses a layered lens space, which is conjectured
         * (but not proven in all cases) to be the triangulation requiring the
         * fewest tetrahedra.  A layered lens space is constructed by
         * building a layered solid torus and then joining together the
         * two boundary triangles.
         *
         * \pre \a p \> \a q ≥ 0 unless (\a p,\a q) = (0,1).
         * \pre gcd(\a p, \a q) = 1.
         *
         * \exception InvalidArgument The preconditions above do not hold;
         * that is, either \a q ≥ \a p and (\a p,\a q) ≠ (0,1), and/or
         * \a p and \a q are not coprime.
         *
         * \param p a parameter of the desired lens space.
         * \param q a parameter of the desired lens space.
         * \return the lens space `L(p,q)`.
         */
        static Triangulation<3> lens(size_t p, size_t q);

        /**
         * Returns a layered loop of the given length.
         * Layered loops are described in detail in the LayeredLoop class notes.
         *
         * \param length the length of the layered loop to construct;
         * this must be strictly positive.
         * \param twisted \c true if the layered loop should be twisted,
         * or \c false if it should be untwisted.
         * \return the resulting layered loop.
         */
        static Triangulation<3> layeredLoop(size_t length, bool twisted);

        /**
         * Returns the five-tetrahedron triangulation of the
         * Poincare homology sphere.
         *
         * \return the Poincare homology sphere.
         */
        static Triangulation<3> poincare();

        /**
         * Returns an augmented triangular solid torus with the given
         * parameters.  Almost all augmented triangular solid tori represent
         * Seifert fibred spaces with three or fewer exceptional fibres.
         * Augmented triangular solid tori are described in more detail in the
         * AugTriSolidTorus class notes.
         *
         * The resulting Seifert fibred space will be
         * SFS((\a a1, \a b1), (\a a2, \a b2), (\a a3, \a b3), (1, 1)),
         * where the parameters \a a1, ..., \a b3 are passed as arguments to
         * this routine.  The three layered solid tori that are attached to
         * the central triangular solid torus will be
         * LST(|<i>a1</i>|, |<i>b1</i>|, |-<i>a1</i>-<i>b1</i>|), ...,
         * LST(|<i>a3</i>|, |<i>b3</i>|, |-<i>a3</i>-<i>b3</i>|).
         *
         * There are no sign constraints on the parameters; in particular,
         * negative arguments are allowed.
         *
         * The new tetrahedra will be inserted at the end of the list of
         * tetrahedra in the triangulation.
         *
         * \pre gcd(\a a1, \a b1) = gcd(\a a2, \a b2) = gcd(\a a3, \a b3) = 1.
         *
         * \exception InvalidArgument The preconditions above do not hold;
         * that is, at least one of the pairs (\a a1, \a b1), (\a a2, \a b2)
         * or (\a a3, \a b3) is not coprime.
         *
         * \param a1 a parameter describing the first layered solid
         * torus in the augmented triangular solid torus.
         * \param b1 a parameter describing the first layered solid
         * torus in the augmented triangular solid torus.
         * \param a2 a parameter describing the second layered solid
         * torus in the augmented triangular solid torus.
         * \param b2 a parameter describing the second layered solid
         * torus in the augmented triangular solid torus.
         * \param a3 a parameter describing the third layered solid
         * torus in the augmented triangular solid torus.
         * \param b3 a parameter describing the third layered solid
         * torus in the augmented triangular solid torus.
         */
        static Triangulation<3> augTriSolidTorus(long a1, long b1,
            long a2, long b2, long a3, long b3);
        /**
         * Returns a triangulation of the given orientable Seifert fibred space
         * over the sphere with at most three exceptional fibres.
         *
         * The Seifert fibred space will be
         * SFS((\a a1, \a b1), (\a a2, \a b2), (\a a3, \a b3)), where the
         * parameters \a a1, ..., \a b3 are passed as arguments to this routine.
         *
         * The three pairs of parameters (\a a, \a b) do not need
         * to be normalised, i.e., the parameters can be positive or
         * negative and \a b may lie outside the range [0..\a a).
         * There is no separate twisting parameter; each additional
         * twist can be incorporated into the existing parameters by replacing
         * some pair (\a a>, \a b) with the pair (\a a, \a a + \a b).
         * For Seifert fibred spaces with less than three exceptional fibres,
         * some or all of the parameter pairs may be (1, \a k) or even (1, 0).
         *
         * If you wish to construct more complex Seifert fibred spaces
         * (e.g., with more exceptional fibres, or with a different base
         * orbifold), you can use the more sophisticated SFSpace::construct().
         *
         * \pre None of \a a1, \a a2 or \a a3 are 0.
         * \pre gcd(\a a1, \a b1) = gcd(\a a2, \a b2) = gcd(\a a3, \a b3) = 1.
         *
         * \exception InvalidArgument The preconditions above do not hold;
         * that is, at least one of \a a_1, \a a_2 or \a a_3 is zero, and/or
         * at least one of the pairs (\a a1, \a b1), (\a a2, \a b2) or
         * (\a a3, \a b3) is not coprime.
         *
         * \param a1 a parameter describing the first exceptional fibre.
         * \param b1 a parameter describing the first exceptional fibre.
         * \param a2 a parameter describing the second exceptional fibre.
         * \param b2 a parameter describing the second exceptional fibre.
         * \param a3 a parameter describing the third exceptional fibre.
         * \param b3 a parameter describing the third exceptional fibre.
         * \return the triangulated Seifert fibred space.
         */
        static Triangulation<3> sfsOverSphere(long a1 = 1, long b1 = 0,
            long a2 = 1, long b2 = 0, long a3 = 1, long b3 = 0);

        /**
         * Returns a nine-tetrahedron minimal triangulation of the Weeks
         * manifold.  The Weeks manifold is the smallest-volume closed
         * hyperbolic 3-manifold, with a volume of roughly 0.9427.
         * Note that there are nine minimal triangulations of the Weeks
         * manifold (of course this routine returns just one).
         *
         * \return the Weeks manifold.
         */
        static Triangulation<3> weeks();

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
         * \return the Weber-Seifert dodecahedral space.
         */
        static Triangulation<3> weberSeifert();

        /**
         * Returns the nine-tetrahedron closed orientable hyperbolic
         * 3-manifold with volume 0.94270736.
         *
         * \return the closed orientable hyperbolic manifold described above.
         */
        static Triangulation<3> smallClosedOrblHyperbolic();

        /**
         * Returns the eleven-tetrahedron closed non-orientable hyperbolic
         * 3-manifold with volume 2.02988321.
         *
         * \return the closed non-orientable hyperbolic manifold described
         * above.
         */
        static Triangulation<3> smallClosedNonOrblHyperbolic();

        /**
         * Returns the boundary 3-sphere of the regular 600-cell.
         * This is a triangulation of the 3-sphere that is a simplicial
         * complex, and in which every edge has degree five.
         *
         * The triangulation was extracted from the Benedetti-Lutz
         * library of triangulations.  See:
         * http://page.math.tu-berlin.de/~lutz/stellar/library_of_triangulations.html
         *
         * \return the boundary of the regular 600-cell.
         */
        static Triangulation<3> sphere600();

        /*@}*/
        /**
         * \name Finite Bounded Triangulations
         */
        /*@{*/

        /**
         * Returns the layered solid torus `LST(a,b,c)`.
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
         * \exception InvalidArgument The preconditions above do not hold;
         * that is, \a a and \a b are not coprime.
         *
         * \param a the first parameter of the layered solid torus.
         * \param b the second parameter of the layered solid torus.
         * \return the layered solid torus `LST(a,b,c)`.
         */
        static Triangulation<3> lst(size_t a, size_t b);

        /**
         * Returns a triangulation of the orientable handlebody with the
         * given genus.
         *
         * For positive genus, this routine uses a minimal layered
         * triangulation of the orientable handlebody. This is constructed by
         * starting with a one-vertex triangulation of a once-punctured
         * non-orientable surface with the given genus, and layering a
         * tetrahedron onto each internal edge of this surface, yielding a
         * (3*genus-2)-tetrahedron triangulation. For genus greater than one,
         * there are many choices for how to do this; this routine makes an
         * arbitrary choice.
         *
         * For genus 0, this routine uses the one-tetrahedron 3-ball.
         *
         * \param genus the genus of the handlebody.
         * \return the orientable handlebody with the given genus.
         *
         * \author Alex He
         */
        static Triangulation<3> handlebody(size_t genus);

        /**
         * Returns a triangulation of the solid Klein bottle.
         * This is isomorphic to the triangulation returned by the
         * generic routine twistedBallBundle().
         *
         * \return the solid Klein bottle.
         */
        static Triangulation<3> solidKleinBottle();

        /*@}*/
        /**
         * \name Ideal Triangulations
         */
        /*@{*/

        /**
         * Returns a two-tetrahedron ideal triangulation of the figure
         * eight knot complement.
         *
         * \return the figure eight knot complement.
         */
        static Triangulation<3> figureEight();

        /**
         * Returns a two-tetrahedron ideal triangulation of the trefoil
         * knot complement.
         *
         * \return the trefoil knot complement.
         */
        static Triangulation<3> trefoil();

        /**
         * Returns a four-tetrahedron ideal triangulation of the
         * Whitehead link complement.
         *
         * \return the Whitehead link complement.
         */
        static Triangulation<3> whitehead();

        /**
         * Deprecated alias for whitehead(), which returns a four-tetrahedron
         * ideal triangulation of the Whitehead link complement.
         *
         * \deprecated This routine has been renamed to whitehead().
         *
         * \return the Whitehead link complement.
         */
        [[deprecated]] static Triangulation<3> whiteheadLink();

        /**
         * Returns the one-tetrahedron ideal triangulation of the
         * non-orientable Gieseking manifold.
         *
         * \return the Gieseking manifold.
         */
        static Triangulation<3> gieseking();

        /**
         * Returns a triangulation of a solid genus two torus with a
         * cusped boundary.  This triangulation has one internal finite
         * vertex and one genus two ideal vertex.
         *
         * \return the solid genus two torus with cusped boundary.
         */
        static Triangulation<3> cuspedGenusTwoTorus();

        /*@}*/
};

inline Triangulation<3> Example<3>::threeSphere() {
    return lens(1, 0);
}

inline Triangulation<3> Example<3>::s2xs1() {
    return sphereBundle();
}

inline Triangulation<3> Example<3>::solidKleinBottle() {
    return twistedBallBundle();
}

inline Triangulation<3> Example<3>::whiteheadLink() {
    return whitehead();
}

} // namespace regina

#endif

