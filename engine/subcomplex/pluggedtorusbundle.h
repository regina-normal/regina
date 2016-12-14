
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

/*! \file subcomplex/pluggedtorusbundle.h
 *  \brief Supports self-identified Seifert fibred spaces that are
 *  triangulated using a combination of thin I-bundles and saturated blocks.
 */

#ifndef __PLUGGEDTORUSBUNDLE_H
#ifndef __DOXYGEN
#define __PLUGGEDTORUSBUNDLE_H
#endif

#include "regina-core.h"
#include "maths/matrix2.h"
#include "subcomplex/standardtri.h"

namespace regina {

class SatRegion;
class TxICore;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Describes a triangulation of a graph manifold formed by joining a
 * bounded saturated region with a thin I-bundle over the torus,
 * possibly with layerings in between.
 *
 * The thin I-bundle must be untwisted, so that it forms the product
 * <tt>T x I</tt> with two boundary tori.  Moreover, it must be isomorphic
 * to some existing instance of the class TxICore.
 *
 * The saturated region is described by an object of the class SatRegion.
 * This region must have precisely two boundary annuli.  These may be
 * two separate torus boundaries (each formed from its own saturated annulus).
 * Alternatively, the saturated region may have a single boundary formed
 * from both saturated annuli, where this boundary is pinched together
 * so that each annulus becomes its own two-sided torus.
 *
 * Either way, the saturated region effectively has two torus boundaries,
 * each formed from two triangles of the triangulation.  These boundaries
 * are then joined to the two torus boundaries of the thin I-bundle,
 * possibly with layerings in between (for more detail on layerings, see
 * the Layering class).  This is illustrated in the following diagram,
 * where the small tunnels show where the torus boundaries are joined
 * (possibly via layerings).
 *
 * <pre>
 *    /--------------------\     /-----------------\
 *    |                     -----                  |
 *    |                     -----                  |
 *    |  Saturated region  |     |  Thin I-bundle  |
 *    |                     -----                  |
 *    |                     -----                  |
 *    \--------------------/     \-----------------/
 * </pre>
 *
 * The effect of the thin I-bundle and the two layerings is essentially
 * to join the two boundaries of the saturated region according to some
 * non-trivial homeomorphism of the torus.  This homeomorphism is
 * specified by a 2-by-2 matrix \a M as follows.
 *
 * Suppose that \a f0 and \a o0 are directed curves on the first
 * boundary torus and \a f1 and \a o1 are directed curves on the second
 * boundary torus, where \a f0 and \a f1 represent the fibres of the
 * saturated region and \a o0 and \a o1 represent the base orbifold (see
 * the page on \ref sfsnotation for terminology).
 * Then the torus boundaries of the saturated region are identified by
 * the thin I-bundle and layerings according to the following relation:
 *
 * <pre>
 *     [f1]       [f0]
 *     [  ] = M * [  ]
 *     [o1]       [o0]
 * </pre>
 *
 * Note that the routines writeName() and writeTeXName() do \e not offer
 * enough information to uniquely identify the triangulation, since this
 * essentially requires 2-dimensional assemblings of saturated blocks.
 * For more detail, writeTextLong() may be used instead.
 *
 * The optional StandardTriangulation routine manifold() is
 * implemented for this class, but homology() is not.
 */
class REGINA_API PluggedTorusBundle : public StandardTriangulation {
    private:
        const TxICore& bundle_;
            /**< The thin I-bundle that appears within this triangulation.
                 This thin I-bundle is referenced from elsewhere (i.e., it
                 is not owned by this object), and its tetrahedra do not
                 belong to this triangulation (instead see the data member
                 \a bundleIso_). */
        Isomorphism<3>* bundleIso_;
            /**< A mapping from the thin I-bundle \a bundle_ to this
                 triangulation.  This is required since the thin I-bundle
                 \a bundle_ is external, and does not refer directly to this
                 triangulation. */
        SatRegion* region_;
            /**< The saturated region that appears within this
                 triangulation.  This region is owned by this object, and
                 refers to tetrahedra within this triangulation. */

        Matrix2 matchingReln_;
            /**< Describes how the two torus boundaries of the saturated
                 region are joined, as discussed in the class notes above. */

    public:
        /**
         * Destroys this structure and its constituent components.
         *
         * As an exception, the thin I-bundle is not destroyed, since
         * it is assumed that this is referenced from elsewhere.
         */
        ~PluggedTorusBundle();

        /**
         * Returns an isomorphic copy of the thin I-bundle that forms part
         * of this triangulation.  Like all objects of class TxICore, the
         * thin I-bundle that is returned is an external object with its own
         * separate triangulation of the product <tt>T x I</tt>.  For
         * information on how the thin I-bundle is embedded within this
         * triangulation, see the routine bundleIso().
         *
         * @return the an isomorphic copy of the thin I-bundle within
         * this triangulation.
         */
        const TxICore& bundle() const;
        /**
         * Returns an isomorphism describing how the thin I-bundle forms
         * a subcomplex of this triangulation.
         *
         * The thin I-bundle returned by bundle() does not directly
         * refer to tetrahedra within this triangulation.  Instead it
         * contains its own isomorphic copy of the thin I-bundle
         * triangulation (as is usual for objects of class TxICore).
         *
         * The isomorphism returned by this routine is a mapping from
         * the triangulation bundle().core() to this triangulation,
         * showing how the thin I-bundle appears as a subcomplex of this
         * structure.
         *
         * @return an isomorphism from the thin I-bundle described
         * by bundle() to the tetrahedra of this triangulation.
         */
        const Isomorphism<3>& bundleIso() const;
        /**
         * Returns the saturated region that forms part of this triangulation.
         * The region refers directly to tetrahedra within this triangulation
         * (as opposed to the thin I-bundle, which refers to a separate
         * external triangulation).
         *
         * @return the saturated region.
         */
        const SatRegion& region() const;
        /**
         * Returns the matrix describing how the two torus boundaries of
         * the saturated region are joined by the thin I-bundle and
         * layerings.  See the class notes above for details.
         *
         * @return the matching relation between the two region boundaries.
         */
        const Matrix2& matchingReln() const;

        Manifold* manifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

        /**
         * Determines if the given triangulation is a saturated region
         * joined to a thin I-bundle via optional layerings, as described
         * in the class notes above.
         *
         * @param tri the triangulation to examine.
         * @return a newly created object containing details of the
         * structure that was found, or \c null if the given
         * triangulation is not of the form described by this class.
         */
        static PluggedTorusBundle* isPluggedTorusBundle
            (Triangulation<3>* tri);

    private:
        /**
         * Creates a new structure of the form described in the class notes
         * above, based on the given constituent components.  The new object
         * will take ownership of the given saturated region and isomorphism.
         * It will not take ownership of the given thin I-bundle.
         *
         * Note that the new object must refer to an existing triangulation.
         *
         * \warning The thin I-bundle \a bundle must have a lifetime at
         * least as long as the new object being created, since it will
         * be referenced directly by this new object.
         *
         * @param bundle the thin I-bundle whose isomorphic copy is used
         * within the triangulation described by the new object.
         * @param bundleIso the corresponding isomorphism from the given
         * thin I-bundle to the triangulation described by the new object.
         * @param region the saturated region used within the new object.
         * @param matchingReln the matching relation describing how the
         * two saturated region boundaries are joined by the thin
         * I-bundle and layerings, as described in the class notes above.
         */
        PluggedTorusBundle(const TxICore& bundle, Isomorphism<3>* bundleIso,
            SatRegion* region, const Matrix2& matchingReln);

        /**
         * Determines whether the given triangulation is of the form
         * described by this class, with the constraint that the
         * thin I-bundle used within the triangulation must be isomorphic
         * to the given thin I-bundle.
         *
         * This routine is internal to isPluggedTorusBundle().
         *
         * \pre The given triangulation is closed and connected.
         *
         * \warning If this routine is successful and a new object is
         * returned, this new object must not outlive the given thin
         * I-bundle (since the new object will in fact contain a direct
         * reference to this thin I-bundle).
         *
         * @param tri the triangulation to examine.
         * @param bundle the thin I-bundle whose isomorphic copy must be
         * used in the given triangulation.
         * @return a newly created object containing details of the
         * structure that was found, or \c null if the given triangulation
         * is not of the form described by this class using an isomorphic
         * copy of the given thin I-bundle.
         */
        static PluggedTorusBundle* hunt(Triangulation<3>* tri,
            const TxICore& bundle);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NPluggedTorusBundle has now been renamed to
 * PluggedTorusBundle.
 */
REGINA_DEPRECATED typedef PluggedTorusBundle NPluggedTorusBundle;

/*@}*/

// Inline functions for PluggedTorusBundle

inline PluggedTorusBundle::PluggedTorusBundle(const TxICore& bundle,
        Isomorphism<3>* bundleIso, SatRegion* region,
        const Matrix2& matchingReln) :
        bundle_(bundle), bundleIso_(bundleIso), region_(region),
        matchingReln_(matchingReln) {
}

inline const TxICore& PluggedTorusBundle::bundle() const {
    return bundle_;
}

inline const Isomorphism<3>& PluggedTorusBundle::bundleIso() const {
    return *bundleIso_;
}

inline const SatRegion& PluggedTorusBundle::region() const {
    return *region_;
}

inline const Matrix2& PluggedTorusBundle::matchingReln() const {
    return matchingReln_;
}

} // namespace regina

#endif

