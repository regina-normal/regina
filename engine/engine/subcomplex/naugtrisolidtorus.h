
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2001, Ben Burton                                   *
 *  For further details contact Ben Burton (benb@acm.org).                *
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

/*! \file naugtrisolidtorus.h
 *  \brief Deals with augmented triangular solid torus components of a
 *  triangulation.
 */

#ifndef __NAUGTRISOLIDTORUS_H
#ifndef __DOXYGEN
#define __NAUGTRISOLIDTORUS_H
#endif

#include "config.h"

#ifdef __NO_INCLUDE_PATHS
    #include "ntrisolidtorus.h"
    #include "nlayeredsolidtorus.h"
#else
    #include "engine/subcomplex/ntrisolidtorus.h"
    #include "engine/subcomplex/nlayeredsolidtorus.h"
#endif

class NComponent;

/**
 * Represents an augmented triangular solid torus component of a
 * triangulation.  Such a component is obtained as follows.  Begin with
 * a three-tetrahedron triangular solid torus (as described by
 * NTriSolidTorus).  Observe that the three axis edges divide the
 * boundary into three annuli.  To each of these annuli, glue a layered
 * solid torus.  Note that the degenerate (2,1,1) layered solid torus
 * (i.e., a one-triangle mobius strip) is allowed and corresponds to
 * simply gluing the two faces of the annulus together.
 *
 * It will be assumed that all layered solid tori other than the
 * degenerate (2,1,1) will have (3,2,1) layered solid tori at their
 * bases.  That is, layered solid tori that begin with the degenerate
 * (2,1,1) and layer over the boundary of the mobius strip are \b not
 * considered in this class.
 *
 * Note that (unless a (1,1,0) layered solid torus is used with the 0
 * edge glued to an axis edge) the resulting space will be a Seifert
 * fibred space with at most three exceptional fibres.  This will be
 * written using Matveev's notation as (S2, (a1,b1) (a2,b2) (a3,b3)).
 * Exceptional fibres will be listed with their indices (a1, a2, a3) in
 * non-decreasing order.  If there are fewer than three exceptional fibres,
 * additional (1,0) pairs will be added to the end of this list.
 * If several exceptional fibres have the same index, they will be
 * listed in non-decreasing order of the <i>b'</i> parameters, where
 * each <i>b'</i> is the corresponding <i>b</i> reduced modulo <i>a</i>
 * so it lies between 0 and <i>a</i>-1 inclusive.
 *
 * All of the pairs will be in standard form (\a a non-negative,
 * \a b between 0 and <i>a</i>-1 inclusive) except for the pair
 * corresponding to the final exceptional fibre.  For this final pair,
 * it is guaranteed that \a b is at least -<i>a</i>.
 *
 * In the exceptional case above with the (1,1,0) layered solid torus,
 * the first pair will have <i>a</i>=0 and there are no guarantees as to
 * what space this is.
 */
class NAugTriSolidTorus : public ShareableObject {
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
        long alpha[3];
            /**< The index \a a for each of the three (a,b) fibre pairs.
                 This is remaining unsigned so we don't have to cast
                 everywhere when doing modular arithmetic. */
        long beta[3];
            /**< The parameter \a b for each of the three (a,b) fibre pairs. */
    
    public:
        /**
         * Destroys this augmented solid torus; note that the corresponding
         * triangular and layered solid tori will also be destroyed.
         */
        virtual ~NAugTriSolidTorus();
        /**
         * Returns a newly created clone of this structure.
         *
         * \ifaces This routine is named \a cloneMe.
         *
         * @return a newly created clone.
         */
        NAugTriSolidTorus* clone() const;

        /**
         * Returns the first parameter \a a of the requested exceptional
         * fibre (a,b).  See the general class notes for further details.
         *
         * @param index specifies which exceptional fibre to examine;
         * this must be 0, 1 or 2.
         * @return the requested first parameter.
         */
        unsigned long getAlpha(int index) const;
        /**
         * Returns the second parameter \a b of the requested exceptional
         * fibre (a,b).  See the general class notes for further details.
         *
         * @param index specifies which exceptional fibre to examine;
         * this must be 0, 1 or 2.
         * @return the requested second parameter.
         */
        long getBeta(int index) const;
        /**
         * Returns the number of exceptional fibres.  See the general
         * class notes for further details.
         *
         * In the undefined case described in the general class notes with
         * the (1,1,0) layered solid torus, this routine will return -1.
         *
         * @return the number of exceptional fibres.
         */
        long getExceptionalFibres() const;

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
         * Determines if the given triangulation component is an
         * augmented triangular solid torus.
         *
         * \ifaces This routine is a member of class Engine.
         *
         * @param comp the triangulation component to examine.
         * @return a newly created structure containing details of the
         * augmented triangular solid torus, or \c null if the given
         * component is not an augmented triangular solid torus.
         */
        static NAugTriSolidTorus* isAugTriSolidTorus(const NComponent* comp);

        void writeTextShort(ostream& out) const;

    private:
        /**
         * Creates a new structure with all subcomponent pointers
         * initialised to \c null.
         */
        NAugTriSolidTorus();

        /**
         * Fill in the \a alpha and \a beta arrays according to the
         * other information already stored in this structure.
         */
        void findExceptionalFibres();
};

// Inline functions for NAugTriSolidTorus

inline NAugTriSolidTorus::NAugTriSolidTorus() : core(0) {
    augTorus[0] = augTorus[1] = augTorus[2] = 0;
}

inline unsigned long NAugTriSolidTorus::getAlpha(int index) const {
    return alpha[index];
}
inline long NAugTriSolidTorus::getBeta(int index) const {
    return beta[index];
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

#endif

