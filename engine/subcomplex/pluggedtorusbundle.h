
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

/*! \file subcomplex/pluggedtorusbundle.h
 *  \brief Supports self-identified Seifert fibred spaces that are
 *  triangulated using a combination of thin I-bundles and saturated blocks.
 */

#ifndef __REGINA_PLUGGEDTORUSBUNDLE_H
#ifndef __DOXYGEN
#define __REGINA_PLUGGEDTORUSBUNDLE_H
#endif

#include "regina-core.h"
#include "maths/matrix2.h"
#include "subcomplex/layering.h"
#include "subcomplex/satregion.h"
#include "subcomplex/standardtri.h"
#include "subcomplex/txicore.h"

namespace regina {

/**
 * Describes a triangulation of a graph manifold formed by joining a
 * bounded saturated region with a thin I-bundle over the torus,
 * possibly with layerings in between.
 *
 * The thin I-bundle must be untwisted, so that it forms the product
 * `T x I` with two boundary tori.  Moreover, it must be isomorphic
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
 * Note that the routines writeName() and writeTeXName() do _not_ offer
 * enough information to uniquely identify the triangulation, since this
 * essentially requires 2-dimensional assemblings of saturated blocks.
 * For more detail, writeTextLong() may be used instead.
 *
 * The optional StandardTriangulation routine manifold() is
 * implemented for this class, but homology() is not.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  Note, however, that
 * the only way to create objects of this class (aside from copying or moving)
 * is via the static member function recognise().
 *
 * \ingroup subcomplex
 */
class PluggedTorusBundle : public StandardTriangulation {
    private:
        const TxICore* bundle_;
            /**< The thin I-bundle that appears within this triangulation.
                 This thin I-bundle is referenced from elsewhere (i.e., it
                 is not owned by this object), and its tetrahedra do not
                 belong to this triangulation (instead see the data member
                 \a bundleIso_). */
        Isomorphism<3> bundleIso_;
            /**< A mapping from the thin I-bundle \a bundle_ to this
                 triangulation.  This is required since the thin I-bundle
                 \a bundle_ is external, and does not refer directly to this
                 triangulation. */
        Layering layer_[2];
            /**< The layerings applied to the upper and lower boundaries of the
                 thin I-bundle (at indices 0 and 1 respectively). */
        SatRegion region_;
            /**< The saturated region that appears within this triangulation. */
        int upperConnection_;
            /**< One of the values 0, 1 or 2, indicating the rotation used to
                 connect the layering on the upper boundary of the thin
                 I-bundle to the first boundary annulus of the saturated region.
                 See the \a regionPos variable in the implementation of hunt()
                 for details. */

        Matrix2 matchingReln_;
            /**< Describes how the two torus boundaries of the saturated
                 region are joined, as discussed in the class notes above. */

    public:
        /**
         * Creates a new copy of the given structure.
         * This will induce a deep copy of \a src.
         *
         * \param src the structure to copy.
         */
        PluggedTorusBundle(const PluggedTorusBundle& src) = default;
        /**
         * Moves the contents of the given structure into this new structure.
         * This is a fast (constant time) operation.
         *
         * The structure that was passed (\a src) will no longer be usable.
         *
         * \param src the structure to move from.
         */
        PluggedTorusBundle(PluggedTorusBundle&& src) noexcept = default;
        /**
         * Sets this to be a copy of the given structure.
         * This will induce a deep copy of \a src.
         *
         * \param src the structure to copy.
         * \return a reference to this structure.
         */
        PluggedTorusBundle& operator = (const PluggedTorusBundle& src) =
            default;
        /**
         * Moves the contents of the given structure into this structure.
         * This is a fast (constant time) operation.
         *
         * The structure that was passed (\a src) will no longer be usable.
         *
         * \param src the structure to move from.
         * \return a reference to this structure.
         */
        PluggedTorusBundle& operator = (PluggedTorusBundle&& src) noexcept =
            default;
        /**
         * Swaps the contents of this and the given structure.
         *
         * \param other the structure whose contents should be swapped
         * with this.
         */
        void swap(PluggedTorusBundle& other) noexcept;

        /**
         * Returns an isomorphic copy of the thin I-bundle that forms part
         * of this triangulation.  Like all objects of class TxICore, the
         * thin I-bundle that is returned is an external object with its own
         * separate triangulation of the product `T x I`.  For
         * information on how the thin I-bundle is embedded within this
         * triangulation, see the routine bundleIso().
         *
         * \return the an isomorphic copy of the thin I-bundle within
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
         * \return an isomorphism from the thin I-bundle described
         * by bundle() to the tetrahedra of this triangulation.
         */
        const Isomorphism<3>& bundleIso() const;
        /**
         * Returns the saturated region that forms part of this triangulation.
         * The region refers directly to tetrahedra within this triangulation
         * (as opposed to the thin I-bundle, which refers to a separate
         * external triangulation).
         *
         * \return the saturated region.
         */
        const SatRegion& region() const;
        /**
         * Returns the matrix describing how the two torus boundaries of
         * the saturated region are joined by the thin I-bundle and
         * layerings.  See the class notes above for details.
         *
         * \return the matching relation between the two region boundaries.
         */
        const Matrix2& matchingReln() const;

        /**
         * Determines whether this and the given structure represent
         * the same type of plugged torus bundle.
         *
         * Specifically, two structures will compare as equal if and only if:
         *
         * - both structures use the same type of thin I-bundle with the
         *   same parameters (as tested by the TxICore comparison operators);
         *
         * - both structures use saturated regions with the same combinatorial
         *   presentation (as tested by the SatRegion comparison operators);
         *
         * - the layerings that connect the thin I-bundle and saturated region
         *   in each structure are the same (as tested by the Layering
         *   comparison operators), and use the same attaching matrices.
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass is more specific than combinatorial isomorphism, since
         * this test does not account for the many possible symmetries in a
         * plugged torus bundle).
         *
         * \param other the structure with which this will be compared.
         * \return \c true if and only if this and the given structure
         * represent the same type of plugged torus bundle.
         */
        bool operator == (const PluggedTorusBundle& other) const;

        std::unique_ptr<Manifold> manifold() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

        /**
         * Determines if the given triangulation is a saturated region
         * joined to a thin I-bundle via optional layerings, as described
         * in the class notes above.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * \param tri the triangulation to examine.
         * \return an object containing details of the structure that was
         * found, or \c null if the given triangulation is not of the form
         * described by this class.
         */
        static std::unique_ptr<PluggedTorusBundle> recognise(
            const Triangulation<3>& tri);

    private:
        /**
         * Creates a new structure of the form described in the class notes
         * above, based on the given constituent components.
         *
         * Note that the new object must refer to an existing triangulation.
         *
         * \warning Only a pointer to the thin I-bundle \a bundle is stored.
         * This class does not manage the life span of the bundle; it is
         * assumed that \a bundle will remain in existence for at least
         * as long as this object (and any objects copied or moved from it).
         * Typically \a bundle would be a static or global variable that is
         * not destroyed until the program exits.
         *
         * \param bundle the thin I-bundle whose isomorphic copy is used
         * within the triangulation described by the new object.
         * \param bundleIso the corresponding isomorphism from the given
         * thin I-bundle to the triangulation described by the new object.
         * \param layerUpper the layering applied to the upper boundary
         * of the thin I-bundle.
         * \param layerLower the layering applied to the lower boundary
         * of the thin I-bundle.
         * \param region the saturated region used within the new object.
         * \param upperConnection 0, 1 or 2, indicating the rotation used to
         * connect the layering on the upper boundary of the thin I-bundle to
         * the first boundary annulus of the saturated region.  See the
         * \a regionPos variable in the implementation of hunt() for details.
         * \param matchingReln the full matching relation describing how the
         * two saturated region boundaries are joined by the thin
         * I-bundle and layerings, as described in the class notes above.
         */
        PluggedTorusBundle(const TxICore& bundle,
            const Isomorphism<3>& bundleIso,
            Layering&& layerUpper, Layering&& layerLower, SatRegion&& region,
            int upperConnection, const Matrix2& matchingReln);

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
         * \warning If this routine is successful and a value is returned,
         * this returned object (and any objects copied or moved from it)
         * must not outlive the given thin I-bundle (since the returned object
         * will in fact contain a direct reference to this thin I-bundle).
         *
         * \param tri the triangulation to examine.
         * \param bundle the thin I-bundle whose isomorphic copy must be
         * used in the given triangulation.
         * \return an object containing details of the structure that was
         * found, or \c null if the given triangulation is not of the form
         * described by this class using an isomorphic copy of the given
         * thin I-bundle.
         */
        static std::unique_ptr<PluggedTorusBundle> hunt(
            const Triangulation<3>& tri, const TxICore& bundle);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls PluggedTorusBundle::swap(); it is provided
 * so that PluggedTorusBundle meets the C++ Swappable requirements.
 *
 * \param a the first structure whose contents should be swapped.
 * \param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(PluggedTorusBundle& a, PluggedTorusBundle& b) noexcept;

// Inline functions for PluggedTorusBundle

inline PluggedTorusBundle::PluggedTorusBundle(const TxICore& bundle,
        const Isomorphism<3>& bundleIso,
        Layering&& layerUpper, Layering&& layerLower, SatRegion&& region,
        int upperConnection, const Matrix2& matchingReln) :
        bundle_(&bundle), bundleIso_(bundleIso),
        layer_{ std::move(layerUpper), std::move(layerLower) },
        region_(std::move(region)), upperConnection_(upperConnection),
        matchingReln_(matchingReln) {
}

inline void PluggedTorusBundle::swap(PluggedTorusBundle& other) noexcept {
    std::swap(bundle_, other.bundle_);
    bundleIso_.swap(other.bundleIso_);
    std::swap(layer_[0], other.layer_[0]);
    std::swap(layer_[1], other.layer_[1]);
    region_.swap(other.region_);
    std::swap(upperConnection_, other.upperConnection_);
    matchingReln_.swap(other.matchingReln_);
}

inline const TxICore& PluggedTorusBundle::bundle() const {
    return *bundle_;
}

inline const Isomorphism<3>& PluggedTorusBundle::bundleIso() const {
    return bundleIso_;
}

inline const SatRegion& PluggedTorusBundle::region() const {
    return region_;
}

inline const Matrix2& PluggedTorusBundle::matchingReln() const {
    return matchingReln_;
}

inline bool PluggedTorusBundle::operator == (const PluggedTorusBundle& other)
        const {
    if ((*bundle_) != (*other.bundle_))
        return false;
    if (layer_[0] != other.layer_[0] || layer_[1] != other.layer_[1])
        return false;
    if (upperConnection_ != other.upperConnection_)
        return false;
    // Comparing the matching relations is equivalent to comparing the
    // connections between the lower layering and the saturated region,
    // since we are separately confirming that all other components of the
    // matching are equivalent.
    if (matchingReln_ != other.matchingReln_)
        return false;
    if (region_ != other.region_)
        return false;
    return true;
}

inline void swap(PluggedTorusBundle& a, PluggedTorusBundle& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

