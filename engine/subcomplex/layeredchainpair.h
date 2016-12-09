
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

/*! \file subcomplex/layeredchainpair.h
 *  \brief Deals with layered chain pair components of a triangulation.
 */

#ifndef __LAYEREDCHAINPAIR_H
#ifndef __DOXYGEN
#define __LAYEREDCHAINPAIR_H
#endif

#include "regina-core.h"
#include "subcomplex/layeredchain.h"
#include "triangulation/forward.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

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
 * All optional StandardTriangulation routines are implemented for this
 * class.
 */
class REGINA_API LayeredChainPair : public StandardTriangulation {
    private:
        LayeredChain* chain_[2];
            /**< The two layered chains that make up this pair. */

    public:
        /**
         * Destroys this layered chain pair.
         */
        virtual ~LayeredChainPair();
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        LayeredChainPair* clone() const;

        /**
         * Returns the requested layered chain used to form this structure.
         * If the two chains have different lengths, the shorter chain
         * will be chain 0 and the longer chain will be chain 1.
         *
         * @param which specifies which chain to return; this must be 0
         * or 1.
         * @return the requested layered chain.
         */
        const LayeredChain* chain(int which) const;

        /**
         * Determines if the given triangulation component is a layered
         * chain pair.
         *
         * @param comp the triangulation component to examine.
         * @return a newly created structure containing details of the
         * layered chain pair, or \c null if the given component is
         * not a layered chain pair.
         */
        static LayeredChainPair* isLayeredChainPair(const Component<3>* comp);

        Manifold* manifold() const;
        AbelianGroup* homology() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new uninitialised structure.
         */
        LayeredChainPair();
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NLayeredChainPair has now been renamed to
 * LayeredChainPair.
 */
REGINA_DEPRECATED typedef LayeredChainPair NLayeredChainPair;

/*@}*/

// Inline functions for LayeredChainPair

inline LayeredChainPair::LayeredChainPair() {
    chain_[0] = chain_[1] = 0;
}
inline LayeredChainPair::~LayeredChainPair() {
    if (chain_[0]) delete chain_[0];
    if (chain_[1]) delete chain_[1];
}

inline const LayeredChain* LayeredChainPair::chain(int which) const {
    return chain_[which];
}
inline std::ostream& LayeredChainPair::writeName(std::ostream& out) const {
    return out << "C("
        << chain_[0]->index() << ',' << chain_[1]->index() << ')';
}
inline std::ostream& LayeredChainPair::writeTeXName(std::ostream& out) const {
    return out << "C_{"
        << chain_[0]->index() << ',' << chain_[1]->index() << '}';
}
inline void LayeredChainPair::writeTextLong(std::ostream& out) const {
    out << "Layered chain pair (chain lengths "
        << chain_[0]->index() << ", " << chain_[1]->index() << ')';
}

} // namespace regina

#endif

