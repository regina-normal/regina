
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file manifold/nlensspace.h
 *  \brief Deals with general Lens spaces.
 */

#ifndef __NLENSSPACE_H
#ifndef __DOXYGEN
#define __NLENSSPACE_H
#endif

#include "nmanifold.h"

namespace regina {

/**
 * \weakgroup manifold
 * @{
 */

/**
 * Represents a general Lens space.
 *
 * All optional NManifold routines are implemented for this class.
 *
 * \testpart
 */
class NLensSpace : public NManifold {
    private:
        unsigned long p;
            /**< The first parameter of the Lens space. */
        unsigned long q;
            /**< The second parameter of the Lens space.  This will
                 always be stored in reduced form. */

    public:
        /**
         * Creates a new Lens space with the given parameters. The lens space
         * L(p,q) is the 3-manifold you get by p/q Dehn surgery on the unknot.
         * ie: L(1,0) and L(1,1) are the 3-sphere, L(0,1) is the product S^1 x S^2, 
         * L(p,1) is the circle bundle over S^2 with Euler class p.  In L(p,q) if
         * you take a generator g of H_1 and evaluate the torsion linking form on
         * it, <g,g> = [\pm r^2 q/p] in Q/Z where r is an integer. 
         *
         * \pre The two given parameters are coprime (have a gcd of 1).
         *
         * @param newP the first parameter \a p of the Lens space L(p,q).
         * @param newQ the second parameter \a q of the Lens space L(p,q).
         * Note that there are no range restrictions whatsoever on this
         * parameter.
         */
        NLensSpace(unsigned long newP, unsigned long newQ);
        /**
         * Creates a clone of the given Lens space.
         *
         * @param cloneMe the Lens space to clone.
         */
        NLensSpace(const NLensSpace& cloneMe);
        /**
         * Destroys this Lens space.
         */
        virtual ~NLensSpace();
        /**
         * Returns the first parameter \a p of this Lens space L(p,q).
         *
         * @return the first parameter.
         */
        unsigned long getP() const;
        /**
         * Returns the second parameter \a q of this Lens space L(p,q).
         * The value of \a q returned will be the smallest \a q between
         * 0 and <i>p</i>-1 inclusive that produces the same 3-manifold
         * as this Lens space.  Thus it might not be the value of \a q
         * that was used to initialise this Lens space.
         */
        unsigned long getQ() const;
        /**
         * Determines whether this and the given Lens space represent
         * the same 3-manifold.  Note that this may be true even if they
         * were initialised with different parameters.
         *
         * @param compare the Lens space with which this will be compared.
         * @return \c true if and only if this and the given Lens space
         * are homeomorphic.
         */
        bool operator == (const NLensSpace& compare) const;

        NTriangulation* construct() const;
        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;

    private:
        /**
         * Reduces the second parameter \a q to the smallest equivalent value.
         */
        void reduce();
};

/*@}*/

// Inline functions for NLensSpace

inline NLensSpace::NLensSpace(unsigned long newP, unsigned long newQ) :
        p(newP), q(newQ) {
    reduce();
}
inline NLensSpace::NLensSpace(const NLensSpace& cloneMe) :
        NManifold(), p(cloneMe.p), q(cloneMe.q) {
}
inline NLensSpace::~NLensSpace() {
}
inline unsigned long NLensSpace::getP() const {
    return p;
}
inline unsigned long NLensSpace::getQ() const {
    return q;
}
inline bool NLensSpace::operator == (const NLensSpace& compare) const {
    return (p == compare.p && q == compare.q);
}

} // namespace regina

#endif

