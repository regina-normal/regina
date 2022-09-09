
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

/*! \file manifold/lensspace.h
 *  \brief Deals with general lens spaces.
 */

#ifndef __REGINA_LENSSPACE_H
#ifndef __DOXYGEN
#define __REGINA_LENSSPACE_H
#endif

#include "regina-core.h"
#include "manifold.h"

namespace regina {

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
 * All optional Manifold routines are implemented for this class.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the other manifold classes.
 *
 * \ingroup manifold
 */
class LensSpace : public Manifold {
    private:
        unsigned long p_;
            /**< The first parameter of the lens space. */
        unsigned long q_;
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
        LensSpace(unsigned long newP, unsigned long newQ);
        /**
         * Creates a new copy of the given lens space.
         */
        LensSpace(const LensSpace&) = default;
        /**
         * Returns the first parameter \a p of this lens space L(p,q).
         * See the class notes for details.
         *
         * @return the first parameter.
         */
        unsigned long p() const;
        /**
         * Returns the second parameter \a q of this lens space L(p,q).
         * See the class notes for details.
         *
         * The value of \a q returned will be the smallest \a q between
         * 0 and <i>p</i>-1 inclusive that produces the same 3-manifold
         * as this lens space.  This means it might not be the value of \a q
         * that was used to initialise this lens space.
         */
        unsigned long q() const;
        /**
         * Determines whether this and the given lens space have the
         * same presentation.
         *
         * Since the presentation is made canonical by the class constructor,
         * this routine also identifies whether this and the given lens space
         * are homeomorphic as 3-manifolds.  This is in contrast to the
         * comparison operators for other manifold classes (such as Seifert
         * fibred spaces and graph manifolds), where the same manifold
         * could have different presentations that compare as not equal.
         *
         * @param compare the lens space with which this should be compared.
         * @return \c true if and only if this and the given lens space
         * have the same presentation (i.e., are homeomorphic).
         */
        bool operator == (const LensSpace& compare) const;
        /**
         * Determines whether this and the given lens space have
         * different presentations.
         *
         * Since the presentation is made canonical by the class constructor,
         * this routine also identifies whether this and the given lens space
         * are homeomorphic as 3-manifolds.  This is in contrast to the
         * comparison operators for other manifold classes (such as Seifert
         * fibred spaces and graph manifolds), where the same manifold
         * could have different presentations that compare as not equal.
         *
         * @param compare the lens space with which this should be compared.
         * @return \c true if and only if this and the given lens space
         * have different presentations (i.e., are non-homeomorphic).
         */
        bool operator != (const LensSpace& compare) const;

        /**
         * Sets this to be a copy of the given lens space.
         *
         * @return a reference to this lens space.
         */
        LensSpace& operator = (const LensSpace&) = default;

        /**
         * Swaps the contents of this and the given lens space.
         *
         * @param other the lens space whose contents should be swapped
         * with this.
         */
        void swap(LensSpace& other) noexcept;

        Triangulation<3> construct() const override;
        AbelianGroup homology() const override;
        bool isHyperbolic() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;

    private:
        /**
         * Reduces the second parameter \a q to the smallest non-negative
         * value that gives the same (i.e., a homeomorphic) 3-manifold.
         */
        void reduce();
};

/**
 * Swaps the contents of the two given lens spaces.
 *
 * This global routine simply calls LensSpace::swap(); it is provided so
 * that LensSpace meets the C++ Swappable requirements.
 *
 * @param a the first lens space whose contents should be swapped.
 * @param b the second lens space whose contents should be swapped.
 *
 * \ingroup manifold
 */
void swap(LensSpace& a, LensSpace& b) noexcept;

// Inline functions for LensSpace

inline LensSpace::LensSpace(unsigned long newP, unsigned long newQ) :
        p_(newP), q_(newQ) {
    reduce();
}
inline unsigned long LensSpace::p() const {
    return p_;
}
inline unsigned long LensSpace::q() const {
    return q_;
}
inline bool LensSpace::operator == (const LensSpace& compare) const {
    return (p_ == compare.p_ && q_ == compare.q_);
}
inline bool LensSpace::operator != (const LensSpace& compare) const {
    return (p_ != compare.p_ || q_ != compare.q_);
}

inline bool LensSpace::isHyperbolic() const {
    return false;
}

inline void LensSpace::swap(LensSpace& other) noexcept {
    std::swap(p_, other.p_);
    std::swap(q_, other.q_);
}

inline void swap(LensSpace& a, LensSpace& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

