
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file subcomplex/layeredchainpair.h
 *  \brief Deals with layered chain pair components of a triangulation.
 */

#ifndef __REGINA_LAYEREDCHAINPAIR_H
#ifndef __DOXYGEN
#define __REGINA_LAYEREDCHAINPAIR_H
#endif

#include "regina-core.h"
#include "subcomplex/layeredchain.h"
#include "triangulation/forward.h"

namespace regina {

/**
 * Represents a layered chain pair component of a triangulation.
 *
 * A layered chain pair consists of two layered chains (as described by
 * class LayeredChain) glued together in a particular way.
 *
 * Orient the hinge edges and diagonals of each chain so they all point
 * in the same direction around the solid tori formed by each layered
 * chain (a \e diagonal is an edge between the two top triangular faces or an
 * edge between the two bottom triangular faces of a layered chain).
 *
 * The two top faces of the first chain are glued to a top and bottom
 * face of the second chain, and the two bottom faces of the first chain
 * are glued to a top and bottom face of the second chain.
 *
 * The four oriented diagonals are all identified as a single edge.  Of the
 * remaining unglued edges (two hinge edges and two non-hinge edges per chain),
 * each hinge edge of one chain must be identified to a non-hinge edge of
 * the other chain and vice versa.  From here the face identifications are
 * uniquely determined.
 *
 * Note that a layered chain pair in which one of the chains contains
 * only one tetrahedron is in fact a layered loop with a twist
 * (see class LayeredLoop).
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
class LayeredChainPair : public StandardTriangulation {
    private:
        LayeredChain chain_[2];
            /**< The two layered chains that make up this pair. */

    public:
        /**
         * Creates a new copy of the given structure.
         */
        LayeredChainPair(const LayeredChainPair&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * @return a reference to this structure.
         */
        LayeredChainPair& operator = (const LayeredChainPair&) = default;

        /**
         * Swaps the contents of this and the given structure.
         *
         * \param other the structure whose contents should be swapped
         * with this.
         */
        void swap(LayeredChainPair& other) noexcept;

        /**
         * Returns the requested layered chain used to form this structure.
         * If the two chains have different lengths, the shorter chain
         * will be chain 0 and the longer chain will be chain 1.
         *
         * \param which specifies which chain to return; this must be 0
         * or 1.
         * @return the requested layered chain.
         */
        const LayeredChain& chain(int which) const;

        /**
         * Determines whether this and the given structure represent
         * the same type of layered chain pair.
         *
         * Specifically, two layered chain pairs will compare as equal if and
         * only if they their constituent layered chains each have the same
         * respective lengths, \e and appear in the same order.
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass is more specific than combinatorial isomorphism, due
         * to the ordering requirement).
         *
         * \param other the structure with which this will be compared.
         * @return \c true if and only if this and the given structure
         * represent the same type of layered chain pair.
         */
        bool operator == (const LayeredChainPair& other) const;

        /**
         * Determines whether this and the given structure represent
         * different types of layered chain pair.
         *
         * Specifically, two layered chain pairs will compare as equal if and
         * only if they their constituent layered chains each have the same
         * respective lengths, \e and appear in the same order.
         *
         * This test follows the general rule for most subclasses of
         * StandardTriangulation (excluding fixed structures such as
         * SnappedBall and TriSolidTorus): two objects compare as equal if and
         * only if they have the same combinatorial parameters (which for this
         * subclass is more specific than combinatorial isomorphism, due
         * to the ordering requirement).
         *
         * \param other the structure with which this will be compared.
         * @return \c true if and only if this and the given structure
         * represent different types of layered chain pair.
         */
        bool operator != (const LayeredChainPair& other) const;

        /**
         * Determines if the given triangulation component is a layered
         * chain pair.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * \param comp the triangulation component to examine.
         * @return a structure containing details of the layered chain pair,
         * or \c null if the given component is not a layered chain pair.
         */
        static std::unique_ptr<LayeredChainPair> recognise(
            const Component<3>* comp);
        std::unique_ptr<Manifold> manifold() const override;
        AbelianGroup homology() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

    private:
        /**
         * Creates a new structure containing the given data.
         */
        LayeredChainPair(const LayeredChain& c0, const LayeredChain& c1);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls LayeredChainPair::swap(); it is provided
 * so that LayeredChainPair meets the C++ Swappable requirements.
 *
 * \param a the first structure whose contents should be swapped.
 * \param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(LayeredChainPair& a, LayeredChainPair& b) noexcept;

// Inline functions for LayeredChainPair

inline LayeredChainPair::LayeredChainPair(const LayeredChain& c0,
        const LayeredChain& c1) : chain_ { c0, c1 } {
}

inline void LayeredChainPair::swap(LayeredChainPair& other) noexcept {
    chain_[0].swap(other.chain_[0]);
    chain_[1].swap(other.chain_[1]);
}

inline const LayeredChain& LayeredChainPair::chain(int which) const {
    return chain_[which];
}
inline bool LayeredChainPair::operator == (const LayeredChainPair& other)
        const {
    return chain_[0] == other.chain_[0] && chain_[1] == other.chain_[1];
}
inline bool LayeredChainPair::operator != (const LayeredChainPair& other)
        const {
    return chain_[0] != other.chain_[0] || chain_[1] != other.chain_[1];
}
inline std::ostream& LayeredChainPair::writeName(std::ostream& out) const {
    return out << "C("
        << chain_[0].index() << ',' << chain_[1].index() << ')';
}
inline std::ostream& LayeredChainPair::writeTeXName(std::ostream& out) const {
    return out << "C_{"
        << chain_[0].index() << ',' << chain_[1].index() << '}';
}
inline void LayeredChainPair::writeTextLong(std::ostream& out) const {
    out << "Layered chain pair (chain lengths "
        << chain_[0].index() << ", " << chain_[1].index() << ')';
}

inline void swap(LayeredChainPair& a, LayeredChainPair& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

