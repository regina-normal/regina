
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
 *  \brief Deals with general lens spaces.
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
 * Represents a general lens space.
 *
 * The lens space L(\a p,\a q) is the 3-manifold you get by
 * \a p/q Dehn surgery on the unknot.  For instance, L(1,0) and L(1,1)
 * are the 3-sphere, L(0,1) is the product S^1 x S^2, and L(\a p,1) is the
 * circle bundle over S^2 with Euler class \a p.  In L(\a p,\a q) if you
 * take a generator \a g of H_1 and evaluate the torsion linking form on it,
 * then &lt;\a g,\a g&gt; = [+/- \a r^2 \a q/\a p] in Q/Z where \a r is an
 * integer.
 *
 * All optional NManifold routines are implemented for this class.
 *
 * \testpart
 */
class NLensSpace : public NManifold {
    private:
        unsigned long p;
            /**< The first parameter of the lens space. */
        unsigned long q;
            /**< The second parameter of the lens space.  This will
                 always be stored in reduced form. */

    public:
        /**
         * Creates a new lens space with the given parameters.
         * See the class notes for details.
         *
         * \pre The two given parameters are coprime (have a gcd of 1).
         *
         * @param newP the first parameter \a p of the lens space L(p,q).
         * @param newQ the second parameter \a q of the lens space L(p,q).
         * Note that there are no range restrictions whatsoever on this
         * parameter.
         */
        NLensSpace(unsigned long newP, unsigned long newQ);
        /**
         * Creates a clone of the given lens space.
         *
         * @param cloneMe the lens space to clone.
         */
        NLensSpace(const NLensSpace& cloneMe);
        /**
         * Destroys this lens space.
         */
        virtual ~NLensSpace();
        /**
         * Returns the first parameter \a p of this lens space L(p,q).
         * See the class notes for details.
         *
         * @return the first parameter.
         */
        unsigned long getP() const;
        /**
         * Returns the second parameter \a q of this lens space L(p,q).
         * See the class notes for details.
         *
         * The value of \a q returned will be the smallest \a q between
         * 0 and <i>p</i>-1 inclusive that produces the same 3-manifold
         * as this lens space.  This means it might not be the value of \a q
         * that was used to initialise this lens space.
         */
        unsigned long getQ() const;
        /**
         * Determines whether this and the given lens space are
         * homeomorphic 3-manifolds.  Note that this may be true
         * even if they were initialised with different parameters.
         *
         * @param compare the lens space with which this will be compared.
         * @return \c true if and only if this and the given lens space
         * are homeomorphic.
         */
        bool operator == (const NLensSpace& compare) const;

        NTriangulation* construct() const;
        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;

    private:
        /**
         * Reduces the second parameter \a q to the smallest non-negative
         * value that gives the same (i.e., a homeomorphic) 3-manifold.
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

