
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file triangulation/nexampletriangulation.h
 *  \brief Offers several example triangulations as starting points for
 *  testing code or getting used to Regina.
 */

#ifndef __NEXAMPLETRIANGULATION_H
#ifndef __DOXYGEN
#define __NEXAMPLETRIANGULATION_H
#endif

namespace regina {

class NTriangulation;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * This class offers routines for constructing sample triangulations of
 * various types.  These triangulations may be useful for testing new
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
class NExampleTriangulation {
    public:
        /**
         * \name Closed Triangulations
         */
        /*@{*/

        /**
         * Returns a one-tetrahedron triangulation of the 3-sphere.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* threeSphere();

        /**
         * Returns the two-tetrahedron triangulation of the 3-sphere
         * that is dual to Bing's house with two rooms.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* bingsHouse();

        /**
         * Returns a two-tetrahedron triangulation of the product space
         * <tt>S^2 x S^1</tt>.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* s2xs1();

        /**
         * Returns a three-tetrahedron triangulation of the non-orientable
         * product space <tt>RP^2 x S^1</tt>.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* rp2xs1();

        /**
         * Returns a triangulation of the connected sum
         * <tt>RP^3 # RP^3</tt>.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* rp3rp3();

        /**
         * Returns the minimal triangulation of the lens space
         * <tt>L(8,3)</tt>.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* lens8_3();

        /**
         * Returns the five-tetrahedron triangulation of the
         * Poincare homology sphere.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* poincareHomologySphere();

        /**
         * Returns a one-vertex triangulation of the Weber-Seifert
         * dodecahedral space.
         *
         * This 3-manifold is described in "Die beiden Dodekaederraume",
         * C. Weber and H. Seifert, Math. Z. 37 (1933), no. 1, 237-253.
         * The triangulation returned by this routine (with 23 tetrahedra)
         * is given in "The Weber-Seifert dodecahedral space is non-Haken",
         * Benjamin A. Burton, J. Hyam Rubinstein and Stephan Tillmann,
         * preprint, arXiv:0909.4625.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* weberSeifert();

        /**
         * Returns a one-vertex triangulation of the Weber-Seifert
         * dodecahedral space.
         *
         * \deprecated This routine is now called weberSeifert(),
         * for consistency with Weber and Seifert's original paper.
         * The old name seifertWeber() has been kept for backward
         * compatibility, but will be removed in a future version of Regina.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* seifertWeber();

        /**
         * Returns the nine-tetrahedron closed orientable hyperbolic
         * 3-manifold with volume 0.94270736.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* smallClosedOrblHyperbolic();

        /**
         * Returns the eleven-tetrahedron closed non-orientable hyperbolic
         * 3-manifold with volume 2.02988321.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* smallClosedNonOrblHyperbolic();

        /*@}*/
        /**
         * (end: Closed Triangulations)
         */

        /**
         * \name Finite Bounded Triangulations
         */
        /*@{*/

        /**
         * Returns the three-tetrahedron layered solid torus
         * <tt>LST(3,4,7)</tt>.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* lst3_4_7();

        /**
         * Returns a triangulation of the solid Klein bottle.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* solidKleinBottle();

        /*@}*/
        /**
         * (end: Finite Bounded Triangulations)
         */

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
        static NTriangulation* figureEightKnotComplement();

        /**
         * Returns a four-tetrahedron ideal triangulation of the
         * Whitehead link complement.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* whiteheadLinkComplement();

        /**
         * Returns the one-tetrahedron ideal triangulation of the
         * non-orientable Gieseking manifold.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* gieseking();

        /**
         * Returns a triangulation of a solid genus two torus with a
         * cusped boundary.  This triangulation has one internal finite
         * vertex and one genus two ideal vertex.
         *
         * @return a newly constructed triangulation, which must be
         * destroyed by the caller of this routine.
         */
        static NTriangulation* cuspedGenusTwoTorus();

        /*@}*/
        /**
         * (end: Ideal Triangulations)
         */
};

/*@}*/

} // namespace regina

#endif

