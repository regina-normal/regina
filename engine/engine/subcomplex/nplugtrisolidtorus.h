
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

/*! \file nplugtrisolidtorus.h
 *  \brief Deals with plugged triangular solid torus components of a
 *  triangulation.
 */

#ifndef __NPLUGTRISOLIDTORUS_H
#ifndef __DOXYGEN
#define __NPLUGTRISOLIDTORUS_H
#endif

#include "subcomplex/ntrisolidtorus.h"
#include "subcomplex/nlayeredchain.h"

namespace regina {

class NComponent;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a plugged triangular solid torus component of a
 * triangulation.  Such a component is obtained as follows.
 *
 * Begin with a three-tetrahedron triangular solid torus (as described by
 * class NTriSolidTorus).  Observe that the three axis edges divide the
 * boundary into three annuli.
 *
 * To each of these annuli a layered chain may be optionally attached.
 * If present, the chain should be attached so its hinge edges are
 * identified with the axis edges of the corresonding annulus and its bottom
 * tetrahedron is layered over either the major edge or minor edge of the
 * corresponding annulus.  The top two faces of the layered chain should
 * remain free.
 *
 * Thus we now have three annuli on the boundary, each represented as a
 * square two of whose (opposite) edges are axis edges of the original
 * triangular solid torus (and possibly also hinge edges of a layered
 * chain).
 *
 * Create a \e plug by gluing two tetrahedra together along a single
 * face.  The six edges that do not run along this common face split the
 * plug boundary into three squares.  These three squares must be glued
 * to the three boundary annuli previously described.  Each axis edge meets
 * two of the annuli; the two corresponding edges of the plug must be
 * non-adjacent (have no common vertex) on the plug.
 * In this way each of the six edges of the plug not running along its
 * interior face corresponds to precisely one of the two instances of
 * precisely one of the three axis edges.
 *
 * If the axis edges are directed so that they all point the
 * same way around the triangular solid torus, these axis edges when
 * drawn on the plug must all point from one common tip of the plug to
 * the other (where the \e tips of the plug are the vertices not meeting the
 * interior face).  The gluings must also be made so that the resulting
 * triangulation component is orientable.
 *
 * Of the optional NStandardTriangulation routines, getManifold() is
 * implemented for most plugged triangular solid tori and
 * getHomologyH1() is not implemented at all.
 */
class NPlugTriSolidTorus : public NStandardTriangulation {
    public:
        static const int CHAIN_NONE;
            /**< Indicates an annulus on the triangular solid torus
                 boundary with no attached layered chain. */
        static const int CHAIN_MAJOR;
            /**< Indicates an annulus on the triangular solid torus
                 boundary with an attached layered chain layered over
                 the major edge of the annulus. */
        static const int CHAIN_MINOR;
            /**< Indicates an annulus on the triangular solid torus
                 boundary with an attached layered chain layered over
                 the minor edge of the annulus. */

        static const int EQUATOR_MAJOR;
            /**< Indicates that, if no layered chains were present, the
                 equator of the plug would consist of major edges of the
                 core triangular solid torus. */
        static const int EQUATOR_MINOR;
            /**< Indicates that, if no layered chains were present, the
                 equator of the plug would consist of minor edges of the
                 core triangular solid torus. */

    private:
        NTriSolidTorus* core;
            /**< The triangular solid torus at the core of this
                 triangulation. */
        NLayeredChain* chain[3];
            /**< The layered chains attached to the annuli on the
                 triangular solid torus, or 0 for those annuli without
                 attached layered chains. */
        int chainType[3];
            /**< The way in which the layered chain is attached to each
                 annulus on the triangular solid torus, or \a CHAIN_NONE
                 for those annuli without attached layered chains. */
        int equatorType;
            /**< Indicates which types of edges form the equator of the
                 plug. */
    
    public:
        /**
         * Destroys this plugged solid torus; note that the corresponding
         * triangular solid torus and layered chains will also be destroyed.
         */
        virtual ~NPlugTriSolidTorus();
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        NPlugTriSolidTorus* clone() const;

        /**
         * Returns the triangular solid torus at the core of this
         * triangulation.
         *
         * @return the core triangular solid torus.
         */
        const NTriSolidTorus& getCore() const;

        /**
         * Returns the layered chain attached to the requested
         * annulus on the boundary of the core triangular solid torus.
         * If there is no attached layered chain, \c null will be returned.
         *
         * Note that the core triangular solid torus will be attached to
         * the bottom (as opposed to the top) of the layered chain.
         * 
         * @param annulus specifies which annulus to examine; this must
         * be 0, 1 or 2.
         * @return the corresponding layered chain.
         */
        const NLayeredChain* getChain(int annulus) const;

        /**
         * Returns the way in which a layered chain is attached to the
         * requested annulus on the boundary of the core triangular solid
         * torus.  This will be one of the chain type constants defined
         * in this class.
         * 
         * @param annulus specifies which annulus to examine; this must
         * be 0, 1 or 2.
         * @return the type of layered chain, or \a CHAIN_NONE
         * if there is no layered chain attached to the requested annulus.
         */
        int getChainType(int annulus) const;

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
         * @return the types of edges that form the equator of the plug;
         * this will be one of the equator type constants defined in this
         * class.
         */
        int getEquatorType() const;

        /**
         * Determines if the given triangulation component is a
         * plugged triangular solid torus.
         *
         * @param comp the triangulation component to examine.
         * @return a newly created structure containing details of the
         * plugged triangular solid torus, or \c null if the given
         * component is not a plugged triangular solid torus.
         */
        static NPlugTriSolidTorus* isPlugTriSolidTorus(NComponent* comp);

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new structure with all subcomponent pointers
         * initialised to \c null.
         */
        NPlugTriSolidTorus();
};

/*@}*/

// Inline functions for NPlugTriSolidTorus

inline NPlugTriSolidTorus::NPlugTriSolidTorus() : core(0) {
    chain[0] = chain[1] = chain[2] = 0;
    chainType[0] = chainType[1] = chainType[2] = CHAIN_NONE;
}

inline const NTriSolidTorus& NPlugTriSolidTorus::getCore() const {
    return *core;
}
inline const NLayeredChain* NPlugTriSolidTorus::getChain(int annulus) const {
    return chain[annulus];
}
inline int NPlugTriSolidTorus::getChainType(int annulus) const {
    return chainType[annulus];
}
inline int NPlugTriSolidTorus::getEquatorType() const {
    return equatorType;
}

} // namespace regina

#endif

