
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

/*! \file subcomplex/layeredtorusbundle.h
 *  \brief Deals with layered torus bundle triangulations.
 */

#ifndef __REGINA_LAYEREDTORUSBUNDLE_H
#ifndef __DOXYGEN
#define __REGINA_LAYEREDTORUSBUNDLE_H
#endif

#include "regina-core.h"
#include "maths/matrix2.h"
#include "subcomplex/standardtri.h"
#include "subcomplex/txicore.h"
#include "triangulation/dim3.h"

namespace regina {

/**
 * Describes a layered torus bundle.  This is a triangulation of a
 * torus bundle over the circle formed as follows.
 *
 * We begin with a thin I-bundle over the torus, i.e,. a triangulation
 * of the product `T x I` that is only one tetrahedron thick.
 * This is referred to as the \a core, and is described by an object
 * of type TxICore.
 *
 * We then identify the upper and lower torus boundaries of this core
 * according to some homeomorphism of the torus.  This may be impossible
 * due to incompatible boundary edges, and so we allow a layering of
 * tetrahedra over one of the boundari tori in order to adjust the
 * boundary edges accordingly.  Layerings are described in more detail
 * in the Layering class.
 *
 * Given the parameters of the core `T x I` and the specific layering,
 * the monodromy for this torus bundle over the circle can be calculated.
 * The manifold() routine returns details of the corresponding 3-manifold.
 *
 * All optional StandardTriangulation routines are implemented for this class.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the other StandardTriangulation
 * subclasses.  Note that the only way to create these objects (aside from
 * copying or moving) is via the static member function recognise().
 *
 * \ingroup subcomplex
 */
class LayeredTorusBundle : public StandardTriangulation {
    private:
        const TxICore* core_;
            /**< The core `T x I` triangulation whose boundaries
                 are joined (possibly via a layering of tetrahedra).
                 This is never \c null; we keep it as a pointer (not a
                 reference) mainly to support the assignment operator. */
        Isomorphism<3> coreIso_;
            /**< Describes how the tetrahedra and vertices of the core
                 `T x I` triangulation returned by TxICore::core()
                 map to the tetrahedra and vertices of the larger layered
                 torus bundle under consideration. */
        Matrix2 reln_;
            /**< Describes how the layering of tetrahedra maps the
                 lower boundary curves to the upper boundary curves.
                 See layeringReln() for details. */

    public:
        /**
         * Creates a new copy of the given structure.
         */
        LayeredTorusBundle(const LayeredTorusBundle&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * \return a reference to this structure.
         */
        LayeredTorusBundle& operator = (const LayeredTorusBundle&) = default;

        /**
         * Swaps the contents of this and the given structure.
         *
         * \param other the structure whose contents should be swapped
         * with this.
         */
        void swap(LayeredTorusBundle& other) noexcept;

        /**
         * Returns the `T x I` triangulation at the core of this
         * layered torus bundle.  This is the product `T x I`
         * whose boundaries are joined (possibly via some layering of
         * tetrahedra).
         *
         * Note that the triangulation returned by TxICore::core()
         * (that is, LayeredTorusBundle::core().core()) may
         * well use different tetrahedron and vertex numbers.  That is,
         * an isomorphic copy of it appears within this layered surface
         * bundle but the individual tetrahedra and vertices may have
         * been permuted.  For a precise mapping from the TxICore::core()
         * triangulation to this triangulation, see the routine coreIso().
         *
         * \return the core `T x I` triangulation.
         */
        const TxICore& core() const;

        /**
         * Returns the isomorphism describing how the core `T x I`
         * appears as a subcomplex of this layered torus bundle.
         *
         * As described in the core() notes, the core `T x I`
         * triangulation returned by TxICore::core() appears within this
         * layered torus bundle, but not necessarily with the same
         * tetrahedron or vertex numbers.
         *
         * This routine returns an isomorphism that maps the tetrahedra
         * and vertices of the core `T x I` triangulation (as
         * returned by LayeredTorusBundle::core().core()) to the
         * tetrahedra and vertices of this overall layered torus bundle.
         *
         * \return the isomorphism from the core `T x I` to this
         * layered torus bundle.
         */
        const Isomorphism<3>& coreIso() const;

        /**
         * Returns a 2-by-2 matrix describing how the layering of
         * tetrahedra relates curves on the two torus boundaries of the
         * core `T x I`.
         *
         * The TxICore class documentation describes generating α and β curves
         * on the two torus boundaries of the core `T x I` (which are
         * referred to as the _upper_ and _lower_ boundaries).  The two
         * boundary tori are parallel in two directions: through the core,
         * and through the layering.  It is desirable to know the parallel
         * relationship between the two sets of boundary curves in each
         * direction.
         *
         * The relationship through the core is already described by
         * TxICore::parallelReln().  This routine describes the
         * relationship through the layering.
         *
         * Let \a a_u and \a b_u be the α and β curves on the upper boundary
         * torus, and let \a a_l and \a b_l be the α and β curves on the lower
         * boundary torus.  Suppose that the upper α is parallel to
         * \a w.\a a_l + \a x.\a b_l, and that the upper β is parallel to
         * \a y.\a a_l + \a z.\a b_l.  Then the matrix returned will be
         *
         * <pre>
         *     [ w  x ]
         *     [      ] .
         *     [ y  z ]
         * </pre>
         *
         * In other words,
         *
         * <pre>
         *     [ a_u ]                      [ a_l ]
         *     [     ]  =  layeringReln() * [     ] .
         *     [ b_u ]                      [ b_l ]
         * </pre>
         *
         * It can be observed that this matrix expresses the upper
         * boundary curves in terms of the lower, whereas
         * TxICore::parallelReln() expresses the lower boundary curves
         * in terms of the upper.  This means that the monodromy
         * describing the overall torus bundle over the circle can be
         * calculated as
         * <pre>
         *     M  =  layeringReln() * core().parallelReln()
         * </pre>
         * or alternatively using the similar matrix
         * <pre>
         *     M'  =  core().parallelReln() * layeringReln() .
         * </pre>
         *
         * Note that in the degenerate case where there is no layering at
         * all, this matrix is still perfectly well defined; in this
         * case it describes a direct identification between the upper
         * and lower boundary tori.
         *
         * \return the relationship through the layering between the
         * upper and lower boundary curves of the core `T x I`.
         */
        const Matrix2& layeringReln() const;

        /**
         * Determines whether this and the given structure represent
         * the same type of layered torus bundle.
         *
         * Specifically, two layered torus bundles will compare as equal if
         * and only if their core `T x I` triangulations have the same
         * combinatorial parameters, and their layering relations are the same.
         *
         * In particular, if you invert a layered torus bundle (which means
         * the layering relation becomes its inverse matrix), the resulting
         * layered torus bundle will generally _not_ compare as equal.
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass is more specific than combinatorial isomorphism, since
         * this test does not recognise inversion and also does not recognise
         * symmetries within the `T x I` core).
         *
         * \param other the structure with which this will be compared.
         * \return \c true if and only if this and the given structure
         * represent the same type of layered torus bundle.
         */
        bool operator == (const LayeredTorusBundle& other) const;

        /**
         * Determines if the given triangulation is a layered torus bundle.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * \param tri the triangulation to examine.
         * \return a structure containing details of the layered torus bundle,
         * or \c null if the given triangulation is not a layered torus bundle.
         */
        static std::unique_ptr<LayeredTorusBundle> recognise(
            const Triangulation<3>& tri);

        std::unique_ptr<Manifold> manifold() const override;
        AbelianGroup homology() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

    private:
        /**
         * Creates a new structure based upon the given core `T x I`
         * triangulation, and initialised with the given additional data.
         *
         * \warning Only a pointer to the core `T x I` is stored.
         * This class does not manage the life span of the core; it is
         * assumed that the core will remain in existence for at least
         * as long as this object (and any objects copied or moved from it).
         * Typically the core would be a static or global variable that is
         * not destroyed until the program exits.
         *
         * \param whichCore a reference to the core `T x I`
         * triangulation upon which this layered torus bundle is based.
         */
        LayeredTorusBundle(const TxICore& whichCore,
            const Isomorphism<3>& coreIso, const Matrix2& reln);

        /**
         * Contains code common to both writeName() and writeTeXName().
         *
         * \param out the output stream to which to write.
         * \param tex \c true if this routine is called from
         * writeTeXName() or \c false if it is called from writeName().
         * \return a reference to \a out.
         */
        std::ostream& writeCommonName(std::ostream& out, bool tex) const;

        /**
         * Internal to recognise().  Determines if the given
         * triangulation is a layered torus bundle with the given core
         * `T x I` triangulation (up to isomorphism).
         *
         * \warning If this routine is successful and a value is returned,
         * this returned object (and any objects copied or moved from it)
         * must not outlive the given core (since the returned object
         * will in fact contain a direct reference to this core).
         *
         * \param tri the triangulation to examine.
         * \param core the core `T x I` to search for.
         * \return a structure containing details of the layered torus bundle,
         * or \c null if the given triangulation is
         * not a layered torus bundle with the given `T x I` core.
         */
        static std::unique_ptr<LayeredTorusBundle> hunt(
            const Triangulation<3>& tri, const TxICore& core);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls LayeredTorusBundle::swap(); it is provided
 * so that LayeredTorusBundle meets the C++ Swappable requirements.
 *
 * \param a the first structure whose contents should be swapped.
 * \param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(LayeredTorusBundle& a, LayeredTorusBundle& b) noexcept;

// Inline functions for LayeredTorusBundle

inline LayeredTorusBundle::LayeredTorusBundle(const TxICore& whichCore,
        const Isomorphism<3>& coreIso, const Matrix2& reln) :
        core_(&whichCore), coreIso_(coreIso), reln_(reln) {
    // Note above that we cannot move coreIso, since where it comes from
    // (via recognise()) is Triangulation<3>::findAllSubcomplexesIn(),
    // which only lets us access the isomorphism as a const reference.
    // Therefore we don't worry about move operations here.
}

inline void LayeredTorusBundle::swap(LayeredTorusBundle& other) noexcept {
    std::swap(core_, other.core_); // pointers
    coreIso_.swap(other.coreIso_);
    reln_.swap(other.reln_);
}

inline const TxICore& LayeredTorusBundle::core() const {
    return *core_;
}

inline const Isomorphism<3>& LayeredTorusBundle::coreIso() const {
    return coreIso_;
}

inline const Matrix2& LayeredTorusBundle::layeringReln() const {
    return reln_;
}

inline bool LayeredTorusBundle::operator == (const LayeredTorusBundle& other)
        const {
    return reln_ == other.reln_ && (*core_) == (*other.core_);
}

inline std::ostream& LayeredTorusBundle::writeName(std::ostream& out) const {
    return writeCommonName(out, false);
}

inline std::ostream& LayeredTorusBundle::writeTeXName(std::ostream& out)
        const {
    return writeCommonName(out, true);
}

inline void swap(LayeredTorusBundle& a, LayeredTorusBundle& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

