
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
class NTriangulation;

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

        /*@}*/
        /**
         * (end: Constructions from 3-Manifold Triangulations)
         */
};

/*@}*/

} // namespace regina

#endif

