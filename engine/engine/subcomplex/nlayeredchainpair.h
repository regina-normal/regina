
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nlayeredchainpair.h
 *  \brief Deals with layered chain pair components of a triangulation.
 */

#ifndef __NLAYEREDCHAINPAIR_H
#ifndef __DOXYGEN
#define __NLAYEREDCHAINPAIR_H
#endif

#include "shareableobject.h"
#include "subcomplex/nlayeredchain.h"
#include "subcomplex/nsfs.h"

namespace regina {

class NComponent;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a layered chain pair component of a triangulation.
 *
 * A layered chain pair consists of two layered chains (as described by
 * class NLayeredChain) glued together in a particular way.
 *
 * Orient the hinge edges and diagonals of each chain so they all point
 * in the same direction around the solid tori formed by each layered
 * chain (a \e diagonal is an edge between the two top faces or an edge
 * between the two bottom faces of a layered chain).
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
 * (see class NLayeredLoop).
 */
class NLayeredChainPair : public ShareableObject {
    private:
        NLayeredChain* chain[2];
            /**< The two layered chains that make up this pair. */
        NSFS seifertStructure;
            /**< The structure of the corresponding Seifert fibred space. */

    public:
        /**
         * Destroys this layered chain pair.
         */
        virtual ~NLayeredChainPair();
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        NLayeredChainPair* clone() const;

        /**
         * Returns the requested layered chain used to form this structure.
         * If the two chains have different lengths, the shorter chain
         * will be chain 0 and the longer chain will be chain 1.
         *
         * @param which specifies which chain to return; this must be 0
         * or 1.
         * @return the requested layered chain.
         */
        const NLayeredChain* getChain(int which) const;

        /**
         * Returns the structure of the Seifert fibred space formed by
         * this layered chain pair.
         *
         * @return the structure of the corresponding Seifert fibred space.
         */
        const NSFS& getSeifertStructure() const;

        /**
         * Determines if the given triangulation component is a layered
         * chain pair.
         *
         * @param comp the triangulation component to examine.
         * @return a newly created structure containing details of the
         * layered chain pair, or \c null if the given component is
         * not a layered chain pair.
         */
        static NLayeredChainPair* isLayeredChainPair(const NComponent* comp);

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new uninitialised structure.
         */
        NLayeredChainPair();

        /**
         * Calculate the Seifert structure according to the other
         * information already stored in this structure.
         */
        void findExceptionalFibres();
};

/*@}*/

// Inline functions for NLayeredChainPair

inline NLayeredChainPair::NLayeredChainPair() {
    chain[0] = chain[1] = 0;
}
inline NLayeredChainPair::~NLayeredChainPair() {
    if (chain[0]) delete chain[0];
    if (chain[1]) delete chain[1];
}

inline const NLayeredChain* NLayeredChainPair::getChain(int which) const {
    return chain[which];
}
inline const NSFS& NLayeredChainPair::getSeifertStructure() const {
    return seifertStructure;
}
inline void NLayeredChainPair::writeTextShort(std::ostream& out) const {
    out << "Layered chain pair (chain lengths "
        << (chain[0] ? chain[0]->getIndex() : '?') << ", "
        << (chain[1] ? chain[1]->getIndex() : '?') << ')';
}

} // namespace regina

#endif

