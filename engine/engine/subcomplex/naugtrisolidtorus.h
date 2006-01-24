
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file naugtrisolidtorus.h
 *  \brief Deals with augmented triangular solid torus components of a
 *  triangulation.
 */

#ifndef __NAUGTRISOLIDTORUS_H
#ifndef __DOXYGEN
#define __NAUGTRISOLIDTORUS_H
#endif

#include "subcomplex/ntrisolidtorus.h"
#include "subcomplex/nlayeredsolidtorus.h"

namespace regina {

class NComponent;

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents an augmented triangular solid torus component of a
 * triangulation.  Such a component is obtained as follows.  Begin with
 * a three-tetrahedron triangular solid torus (as described by
 * NTriSolidTorus).  Observe that the three axis edges divide the
 * boundary into three annuli.  Then take one of the following actions.
 *
 *   - To each of these annuli, glue a layered solid torus.
 *     Note that the degenerate (2,1,1) layered solid torus
 *     (i.e., a one-triangle mobius strip) is allowed and corresponds to
 *     simply gluing the two faces of the annulus together.
 *
 *   - To one of these annuli, glue a layered solid torus as described
 *     above.  Join the other two annuli with a layered chain
 *     in either the manner described by
 *     NTriSolidTorus::areAnnuliLinkedMajor() or the manner described by
 *     NTriSolidTorus::areAnnuliLinkedAxis().
 *
 * It will be assumed that all layered solid tori other than the
 * degenerate (2,1,1) will have (3,2,1) layered solid tori at their
 * bases.  That is, layered solid tori that begin with the degenerate
 * (2,1,1) and layer over the boundary of the mobius strip are \b not
 * considered in this class.
 *
 * Note that (unless a (1,1,0) layered solid torus is used with the 0
 * edge glued to an axis edge) the resulting space will be a Seifert
 * fibred space over the 2-sphere with at most three exceptional fibres.
 *
 * Of the optional NStandardTriangulation routines, getManifold() is
 * implemented for most augmented triangular solid tori and
 * getHomologyH1() is not implemented at all.
 */
class NAugTriSolidTorus : public NStandardTriangulation {
    public:
        static const int CHAIN_NONE;
            /**< Indicates that this augmented triangular solid torus
                 contains no layered chain. */
        static const int CHAIN_MAJOR;
            /**< Indicates that this augmented triangular solid torus
                 contains a layered chain attached as described by
                 NTriSolidTorus::areAnnuliLinkedMajor(). */
        static const int CHAIN_AXIS;
            /**< Indicates that this augmented triangular solid torus
                 contains a layered chain attached as described by
                 NTriSolidTorus::areAnnuliLinkedAxis(). */

    private:
        NTriSolidTorus* core;
            /**< The triangular solid torus at the core of this
                 triangulation. */
        NLayeredSolidTorus* augTorus[3];
            /**< The layered solid tori attached to the boundary annuli.
                 If one of the layered solid tori is a degenerate (2,1,1)
                 triangle, the corresponding pointer will be 0.
                 Note that <tt>augTorus[i]</tt> will be attached to
                 annulus \c i of the triangular solid torus. */
        NPerm edgeGroupRoles[3];
            /**< Permutation <tt>edgeGroupRoles[i]</tt> describes the role
                 played by each top level edge group of layered solid torus
                 <tt>i</tt>.  For permutation <tt>p</tt>, group <tt>p[0]</tt>
                 is glued to an axis edge, group <tt>p[1]</tt> is glued to a
                 major edge and group <tt>p[2]</tt> is glued to a minor edge. */
        unsigned long chainIndex;
            /**< The number of tetrahedra in the layered chain if
                 present, or 0 if there is no layered chain. */
        int chainType;
            /**< The way in which the layered chain is attached, or
                 \a CHAIN_NONE if there is no layered chain. */
        int torusAnnulus;
            /**< The annulus to which the single layered solid torus is
                 attached (if there is a layered chain), or -1 if there is
                 no layered chain. */

    public:
        /**
         * Destroys this augmented solid torus; note that the corresponding
         * triangular and layered solid tori will also be destroyed.
         */
        virtual ~NAugTriSolidTorus();
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        NAugTriSolidTorus* clone() const;

        /**
         * Returns the triangular solid torus at the core of this
         * triangulation.
         *
         * @return the core triangular solid torus.
         */
        const NTriSolidTorus& getCore() const;
        /**
         * Returns the layered solid torus attached to the requested
         * annulus on the boundary of the core triangular solid torus.
         * If the layered solid torus is a degenerate (2,1,1) mobius
         * band (i.e., the two faces of the corresponding annulus have
         * simply been glued together), \c null will be returned.
         * 
         * @param annulus specifies which annulus to examine; this must
         * be 0, 1 or 2.
         * @return the corresponding layered solid torus.
         */
        const NLayeredSolidTorus* getAugTorus(int annulus) const;

        /**
         * Returns a permutation describing the role played by each top
         * level edge group of the layered solid torus glued to the
         * requested annulus of the core triangular solid torus.  See
         * NLayeredSolidTorus::getTopEdge() for details regarding edge groups.
         *
         * If the permutation returned is <tt>p</tt>, edge group <tt>p[0]</tt>
         * will be glued to an axis edge, group <tt>p[1]</tt> will be
         * glued to a major edge and group <tt>p[2]</tt> will be glued
         * to a minor edge.  <tt>p[3]</tt> will always be 3.
         *
         * Even if the corresponding layered solid torus is a degenerate
         * (2,1,1) mobius band (i.e., getAugTorus() returns \c null),
         * the concept of edge groups is still
         * meaningful and this routine will return correct results.
         * 
         * @param annulus specifies which annulus to examine; this must
         * be 0, 1 or 2.  It is the layered solid torus glued to this
         * annulus whose edge groups will be described.
         * @return a permutation describing the roles of the
         * corresponding top level edge groups.
         */
        NPerm getEdgeGroupRoles(int annulus) const;

        /**
         * Returns the number of tetrahedra in the layered chain linking
         * two of the boundary annuli of the core triangular solid torus.
         * Note that this count does not include any of the tetrahedra
         * actually belonging to the triangular solid torus.
         *
         * @return the number of tetrahedra in the layered chain, or 0
         * if there is no layered chain linking two boundary annuli.
         */
        unsigned long getChainLength() const;

        /**
         * Returns the way in which a layered chain links
         * two of the boundary annuli of the core triangular solid torus.
         * This will be one of the chain type constants defined in this
         * class.
         *
         * @return the type of layered chain, or \a CHAIN_NONE
         * if there is no layered chain linking two boundary annuli.
         */
        int getChainType() const;

        /**
         * Returns the single boundary annulus of the core triangular
         * solid torus to which a layered solid torus is attached.  This
         * routine is only meaningful if the other two annuli are linked
         * by a layered chain.
         *
         * The integer returned will be 0, 1 or 2; see the
         * NTriSolidTorus class notes for how the boundary annuli are
         * numbered.
         *
         * @return the single annulus to which the layered solid torus
         * is attached, or -1 if there is no layered chain (and thus all
         * three annuli have layered solid tori attached).
         */
        int getTorusAnnulus() const;

        /**
         * Determines whether the core triangular solid torus has two of
         * its boundary annuli linked by a layered chain as described in
         * the general class notes.
         *
         * @return \c true if and only if the layered chain described in
         * the class notes is present.
         */
        bool hasLayeredChain() const;

        /**
         * Determines if the given triangulation component is an
         * augmented triangular solid torus.
         *
         * @param comp the triangulation component to examine.
         * @return a newly created structure containing details of the
         * augmented triangular solid torus, or \c null if the given
         * component is not an augmented triangular solid torus.
         */
        static NAugTriSolidTorus* isAugTriSolidTorus(const NComponent* comp);

        NManifold* getManifold() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new structure with all subcomponent pointers
         * initialised to \c null.
         */
        NAugTriSolidTorus();

        /**
         * Contains code common to both writeName() and writeTeXName().
         *
         * @param out the output stream to which to write.
         * @param tex \c true if this routine is called from
         * writeTeXName() or \c false if it is called from writeName().
         * @return a reference to \a out.
         */
        std::ostream& writeCommonName(std::ostream& out, bool tex) const;
};

/*@}*/

// Inline functions for NAugTriSolidTorus

inline NAugTriSolidTorus::NAugTriSolidTorus() : core(0),
        chainType(CHAIN_NONE) {
    augTorus[0] = augTorus[1] = augTorus[2] = 0;
}

inline const NTriSolidTorus& NAugTriSolidTorus::getCore() const {
    return *core;
}
inline const NLayeredSolidTorus* NAugTriSolidTorus::getAugTorus(
        int annulus) const {
    return augTorus[annulus];
}
inline NPerm NAugTriSolidTorus::getEdgeGroupRoles(int annulus) const {
    return edgeGroupRoles[annulus];
}
inline unsigned long NAugTriSolidTorus::getChainLength() const {
    return chainIndex;
}
inline int NAugTriSolidTorus::getChainType() const {
    return chainType;
}
inline int NAugTriSolidTorus::getTorusAnnulus() const {
    return torusAnnulus;
}
inline bool NAugTriSolidTorus::hasLayeredChain() const {
    return (chainIndex != 0);
}

} // namespace regina

#endif

