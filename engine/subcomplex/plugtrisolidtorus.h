
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file subcomplex/plugtrisolidtorus.h
 *  \brief Deals with plugged triangular solid torus components of a
 *  triangulation.
 */

#ifndef __REGINA_PLUGTRISOLIDTORUS_H
#ifndef __DOXYGEN
#define __REGINA_PLUGTRISOLIDTORUS_H
#endif

#include <optional>
#include "regina-core.h"
#include "subcomplex/trisolidtorus.h"
#include "subcomplex/layeredchain.h"
#include "triangulation/forward.h"

namespace regina {

/**
 * Represents a plugged triangular solid torus component of a
 * triangulation.  Such a component is obtained as follows.
 *
 * Begin with a three-tetrahedron triangular solid torus (as described by
 * class TriSolidTorus).  Observe that the three axis edges divide the
 * boundary into three annuli.
 *
 * To each of these annuli a layered chain may be optionally attached.
 * If present, the chain should be attached so its hinge edges are
 * identified with the axis edges of the corresonding annulus and its bottom
 * tetrahedron is layered over either the major edge or minor edge of the
 * corresponding annulus.  The top two triangular faces of the layered chain
 * should remain free.
 *
 * Thus we now have three annuli on the boundary, each represented as a
 * square two of whose (opposite) edges are axis edges of the original
 * triangular solid torus (and possibly also hinge edges of a layered
 * chain).
 *
 * Create a _plug_ by gluing two tetrahedra together along a single
 * triangle.  The six edges that do not run along this common triangle split the
 * plug boundary into three squares.  These three squares must be glued
 * to the three boundary annuli previously described.  Each axis edge meets
 * two of the annuli; the two corresponding edges of the plug must be
 * non-adjacent (have no common vertex) on the plug.
 * In this way each of the six edges of the plug not running along its
 * interior triangle corresponds to precisely one of the two instances of
 * precisely one of the three axis edges.
 *
 * If the axis edges are directed so that they all point the
 * same way around the triangular solid torus, these axis edges when
 * drawn on the plug must all point from one common tip of the plug to
 * the other (where the _tips_ of the plug are the vertices not meeting the
 * interior triangle).  The gluings must also be made so that the resulting
 * triangulation component is orientable.
 *
 * Of the optional StandardTriangulation routines, manifold() is
 * implemented for most plugged triangular solid tori and
 * homology() is not implemented at all.
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
class PlugTriSolidTorus : public StandardTriangulation {
    public:
        static constexpr int CHAIN_NONE = 0;
            /**< Indicates an annulus on the triangular solid torus
                 boundary with no attached layered chain. */
        static constexpr int CHAIN_MAJOR = 1;
            /**< Indicates an annulus on the triangular solid torus
                 boundary with an attached layered chain layered over
                 the major edge of the annulus. */
        static constexpr int CHAIN_MINOR = 3;
            /**< Indicates an annulus on the triangular solid torus
                 boundary with an attached layered chain layered over
                 the minor edge of the annulus. */

        static constexpr int EQUATOR_MAJOR = 1;
            /**< Indicates that, if no layered chains were present, the
                 equator of the plug would consist of major edges of the
                 core triangular solid torus. */
        static constexpr int EQUATOR_MINOR = 3;
            /**< Indicates that, if no layered chains were present, the
                 equator of the plug would consist of minor edges of the
                 core triangular solid torus. */

    private:
        TriSolidTorus core_;
            /**< The triangular solid torus at the core of this
                 triangulation. */
        std::optional<LayeredChain> chain_[3];
            /**< The layered chains attached to the annuli on the
                 triangular solid torus, or \nullopt for those annuli without
                 attached layered chains. */
        int chainType_[3];
            /**< The way in which the layered chain is attached to each
                 annulus on the triangular solid torus, or \a CHAIN_NONE
                 for those annuli without attached layered chains. */
        int equatorType_;
            /**< Indicates which types of edges form the equator of the plug. */

    public:
        /**
         * Creates a new copy of the given structure.
         */
        PlugTriSolidTorus(const PlugTriSolidTorus&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * \return a reference to this structure.
         */
        PlugTriSolidTorus& operator = (const PlugTriSolidTorus&) = default;

        /**
         * Swaps the contents of this and the given structure.
         *
         * \param other the structure whose contents should be swapped
         * with this.
         */
        void swap(PlugTriSolidTorus& other) noexcept;

        /**
         * Returns the triangular solid torus at the core of this
         * triangulation.
         *
         * \return the core triangular solid torus.
         */
        const TriSolidTorus& core() const;

        /**
         * Returns the layered chain attached to the requested
         * annulus on the boundary of the core triangular solid torus.
         * If there is no attached layered chain, \nullopt will be returned.
         *
         * Note that the core triangular solid torus will be attached to
         * the bottom (as opposed to the top) of the layered chain.
         *
         * \param annulus specifies which annulus to examine; this must
         * be 0, 1 or 2.
         * \return the corresponding layered chain.
         */
        const std::optional<LayeredChain>& chain(int annulus) const;

        /**
         * Returns the way in which a layered chain is attached to the
         * requested annulus on the boundary of the core triangular solid
         * torus.  This will be one of the chain type constants defined
         * in this class.
         *
         * \param annulus specifies which annulus to examine; this must
         * be 0, 1 or 2.
         * \return the type of layered chain, or \a CHAIN_NONE
         * if there is no layered chain attached to the requested annulus.
         */
        int chainType(int annulus) const;

        /**
         * Returns which types of edges form the equator of the plug.
         * In the absence of layered chains these will either all be major
         * edges or all be minor edges.
         *
         * Layered chains complicate matters, but the roles that the major
         * and minor edges play on the boundary annuli of the triangular
         * solid torus can be carried up to the annuli at the top of each
         * layered chain; the edges filling the corresponding major or
         * minor roles will then form the equator of the plug.
         *
         * \return the types of edges that form the equator of the plug;
         * this will be one of the equator type constants defined in this
         * class.
         */
        int equatorType() const;

        /**
         * Determines whether this and the given structure represent
         * the same type of plugged triangular solid torus.
         *
         * Specifically, two structures will compare as equal if and only if
         * their equators are of the same (major/minor) type, and the same
         * numbered annuli either both have no chains attached or both have
         * chains of the same length attached in the same (major/minor) manner.
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass is more specific than combinatorial isomorphism, since
         * this test does not account for the many symmetries in a
         * plugged triangular solid torus).
         *
         * \param other the structure with which this will be compared.
         * \return \c true if and only if this and the given structure
         * represent the same type of plugged triangular solid torus.
         */
        bool operator == (const PlugTriSolidTorus& other) const;

        /**
         * Determines if the given triangulation component is a
         * plugged triangular solid torus.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * \param comp the triangulation component to examine.
         * \return a structure containing details of the plugged triangular
         * solid torus, or \c null if the given component is not a plugged
         * triangular solid torus.
         */
        static std::unique_ptr<PlugTriSolidTorus> recognise(Component<3>* comp);

        std::unique_ptr<Manifold> manifold() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

    private:
        /**
         * Creates a new structure with the given core.
         * All optional data members will be initialsed to \nullopt,
         * all chain types will be initialised to CHAIN_NONE, and
         * all remaining data members will be left uninitialised.
         */
        PlugTriSolidTorus(const TriSolidTorus& core);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls PlugTriSolidTorus::swap(); it is provided
 * so that PlugTriSolidTorus meets the C++ Swappable requirements.
 *
 * \param a the first structure whose contents should be swapped.
 * \param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(PlugTriSolidTorus& a, PlugTriSolidTorus& b) noexcept;

// Inline functions for PlugTriSolidTorus

inline PlugTriSolidTorus::PlugTriSolidTorus(const TriSolidTorus& core) :
        core_(core), chainType_ { CHAIN_NONE, CHAIN_NONE, CHAIN_NONE } {
}

inline void PlugTriSolidTorus::swap(PlugTriSolidTorus& other) noexcept {
    core_.swap(other.core_);
    chain_[0].swap(other.chain_[0]);
    chain_[1].swap(other.chain_[1]);
    chain_[2].swap(other.chain_[2]);
    std::swap_ranges(chainType_, chainType_ + 3, other.chainType_);
    std::swap(equatorType_, other.equatorType_);
}

inline const TriSolidTorus& PlugTriSolidTorus::core() const {
    return core_;
}
inline const std::optional<LayeredChain>& PlugTriSolidTorus::chain(int annulus)
        const {
    return chain_[annulus];
}
inline int PlugTriSolidTorus::chainType(int annulus) const {
    return chainType_[annulus];
}
inline int PlugTriSolidTorus::equatorType() const {
    return equatorType_;
}

inline bool PlugTriSolidTorus::operator == (
        const PlugTriSolidTorus& other) const {
    return equatorType_ == other.equatorType_ &&
        std::equal(chainType_, chainType_ + 3, other.chainType_) &&
        std::equal(chain_, chain_ + 3, other.chain_);
}

inline void swap(PlugTriSolidTorus& a, PlugTriSolidTorus& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

