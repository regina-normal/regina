
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

/*! \file manifold/torusbundle.h
 *  \brief Deals with torus bundles over the circle.
 */

#ifndef __TORUSBUNDLE_H
#ifndef __DOXYGEN
#define __TORUSBUNDLE_H
#endif

#include "regina-core.h"
#include "manifold/manifold.h"
#include "maths/matrix2.h"

namespace regina {

/**
 * \weakgroup manifold
 * @{
 */

/**
 * Represents a torus bundle over the circle.  This is expressed as the
 * product of the torus and the interval, with the two torus boundaries
 * identified according to some specified monodromy.
 *
 * The monodromy is described by a 2-by-2 matrix \a M as follows.
 * Let \a a and \a b be generating curves of the upper torus boundary,
 * and let \a p and \a q be the corresponding curves on the lower torus
 * boundary (so that \a a and \a p are parallel and \a b and \a q are
 * parallel).  Then we identify the torus boundaries so that, in
 * additive terms:
 *
 * <pre>
 *     [a]       [p]
 *     [ ] = M * [ ]
 *     [b]       [q]
 * </pre>
 *
 * All optional Manifold routines except for construct() are implemented
 * for this class.
 *
 * \todo \feature Implement the == operator for finding conjugate and
 * inverse matrices.
 */
class REGINA_API TorusBundle : public Manifold {
    private:
        Matrix2 monodromy_;
            /**< The monodromy describing how the two torus boundaries
                 are identified.  See the class notes for details. */

    public:
        /**
         * Creates a new trivial torus bundle over the circle.
         * In other words, this routine creates a torus bundle with the
         * identity monodromy.
         */
        TorusBundle();
        /**
         * Creates a new torus bundle over the circle using the given
         * monodromy.
         *
         * \pre The given matrix has determinant +1 or -1.
         *
         * @param newMonodromy describes precisely how the upper and lower
         * torus boundaries are identified.  See the class notes for details.
         */
        TorusBundle(const Matrix2& newMonodromy);
        /**
         * Creates a new torus bundle over the circle using the given
         * monodromy.  The four elements of the monodromy matrix are
         * passed separately.  They combine to give the full monodromy
         * matrix \a M as follows:
         *
         * <pre>
         *           [ mon00  mon01 ]
         *     M  =  [              ]
         *           [ mon10  mon11 ]
         * </pre>
         *
         * \pre The monodromy matrix formed from the given parameters
         * has determinant +1 or -1.
         *
         * @param mon00 the (0,0) element of the monodromy matrix.
         * @param mon01 the (0,1) element of the monodromy matrix.
         * @param mon10 the (1,0) element of the monodromy matrix.
         * @param mon11 the (1,1) element of the monodromy matrix.
         */
        TorusBundle(long mon00, long mon01, long mon10, long mon11);
        /**
         * Creates a clone of the given torus bundle.
         *
         * @param cloneMe the torus bundle to clone.
         */
        TorusBundle(const TorusBundle& cloneMe);
        /**
         * Returns the monodromy describing how the upper and lower
         * torus boundaries are identified.  See the class notes for
         * details.
         *
         * @return the monodromy for this torus bundle.
         */
        const Matrix2& monodromy() const;

        AbelianGroup* homology() const;
        bool isHyperbolic() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;

    private:
        /**
         * Uses change of basis and/or inversion to reduces the monodromy
         * representation to something more aesthetically pleasing.
         */
        void reduce();

        /**
         * Rotate the monodromy matrix by 180 degrees (i.e., swap the
         * main diagonal and also swap the off-diagonal).
         *
         * This gives an alternate monodromy matrix for the same 3-manifold;
         * the transformation merely represents a change of basis.
         */
        void rotate();

        /**
         * Add the first row of the monodromy matrix to the second,
         * and then subtract the second column from the first.
         *
         * This gives an alternate monodromy matrix for the same 3-manifold;
         * the transformation merely represents a change of basis.
         */
        void addRCDown();

        /**
         * Subtract the first row of the monodromy matrix from the second,
         * and then add the second column to the first.
         *
         * This gives an alternate monodromy matrix for the same 3-manifold;
         * the transformation merely represents a change of basis.
         */
        void subtractRCDown();

        /**
         * Add the second row of the monodromy matrix to the first,
         * and then subtract the first column from the second.
         *
         * This gives an alternate monodromy matrix for the same 3-manifold;
         * the transformation merely represents a change of basis.
         */
        void addRCUp();

        /**
         * Subtract the second row of the monodromy matrix from the first,
         * and then add the first column to the second.
         *
         * This gives an alternate monodromy matrix for the same 3-manifold;
         * the transformation merely represents a change of basis.
         */
        void subtractRCUp();

        /**
         * Determines whether the first given monodromy matrix is more
         * aesthetically pleasing than the second.  The way in which
         * this judgement is made is purely aesthetic on the part of the
         * author, and is subject to change in future versions of Regina.
         *
         * Note that this routine is not equivalent to the global
         * simpler(const Matrix2&, const Matrix2&).  This routine is
         * tweaked specifically for use with torus bundle monodromies.
         *
         * \pre Both matrices consist entirely of non-negative elements.
         *
         * @param m1 the first monodromy matrix to examine.
         * @param m2 the second monodromy matrix to examine.
         * @return \c true if \a m1 is deemed to be more pleasing than \a m2,
         * or \c false if either the matrices are equal or \a m2 is more
         * pleasing than \a m1.
         */
        static bool simplerNonNeg(const Matrix2& m1, const Matrix2& m2);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NTorusBundle has now been renamed to
 * TorusBundle.
 */
REGINA_DEPRECATED typedef TorusBundle NTorusBundle;

/*@}*/

// Inline functions for TorusBundle

inline TorusBundle::TorusBundle() :
        monodromy_(1, 0, 0, 1) {
}

inline TorusBundle::TorusBundle(const Matrix2& newMonodromy) :
        monodromy_(newMonodromy) {
    reduce();
}

inline TorusBundle::TorusBundle(long mon00, long mon01, long mon10,
        long mon11) : monodromy_(mon00, mon01, mon10, mon11) {
    reduce();
}

inline TorusBundle::TorusBundle(const TorusBundle& cloneMe) :
        Manifold(), monodromy_(cloneMe.monodromy_) {
}

inline const Matrix2& TorusBundle::monodromy() const {
    return monodromy_;
}

inline void TorusBundle::rotate() {
    long x = monodromy_[0][0];
    monodromy_[0][0] = monodromy_[1][1];
    monodromy_[1][1] = x;

    x = monodromy_[0][1];
    monodromy_[0][1] = monodromy_[1][0];
    monodromy_[1][0] = x;
}

inline void TorusBundle::addRCDown() {
    monodromy_[1][0] += monodromy_[0][0];
    monodromy_[1][1] += monodromy_[0][1];
    monodromy_[0][0] -= monodromy_[0][1];
    monodromy_[1][0] -= monodromy_[1][1];
}

inline void TorusBundle::subtractRCDown() {
    monodromy_[1][0] -= monodromy_[0][0];
    monodromy_[1][1] -= monodromy_[0][1];
    monodromy_[0][0] += monodromy_[0][1];
    monodromy_[1][0] += monodromy_[1][1];
}

inline void TorusBundle::addRCUp() {
    monodromy_[0][0] += monodromy_[1][0];
    monodromy_[0][1] += monodromy_[1][1];
    monodromy_[0][1] -= monodromy_[0][0];
    monodromy_[1][1] -= monodromy_[1][0];
}

inline void TorusBundle::subtractRCUp() {
    monodromy_[0][0] -= monodromy_[1][0];
    monodromy_[0][1] -= monodromy_[1][1];
    monodromy_[0][1] += monodromy_[0][0];
    monodromy_[1][1] += monodromy_[1][0];
}

inline bool TorusBundle::isHyperbolic() const {
    return false;
}

} // namespace regina

#endif

