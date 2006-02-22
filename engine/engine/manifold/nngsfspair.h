
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

/*! \file nngsfspair.h
 *  \brief Deals with non-geometric pairs of Seifert fibred spaces.
 */

#ifndef __NNGSFSPAIR_H
#ifndef __DOXYGEN
#define __NNGSFSPAIR_H
#endif

#include "manifold/nmanifold.h"
#include "utilities/nmatrix2.h"

namespace regina {

class NSFSpace;

/**
 * \weakgroup manifold
 * @{
 */

/**
 * Represents a closed non-geometric 3-manifold formed by joining
 * two bounded Seifert fibred spaces along a common torus.
 *
 * Each Seifert fibred space must have just one boundary component,
 * corresponding to a puncture in the base orbifold (with no
 * fibre-reversing twist as one travels around this boundary).
 *
 * The way in which the two spaces are joined is specified by a 2-by-2
 * matrix \a M.  This matrix expresses the locations of the fibres and
 * base orbifold of the second Seifert fibred space in terms of the first.
 *
 * More specifically, suppose that \a f0 and \a o0 are generators of the
 * common torus, where \a f0 represents a directed fibre in the first
 * Seifert fibred space and \a o0 represents the oriented boundary of
 * the corresponding base orbifold.  Likewise, let \a f1 and \a o1 be
 * generators of the common torus representing a directed fibre and
 * the base orbifold of the second Seifert fibred space.  Then the curves
 * \a f0, \a o0, \a f1 and \a o1 are related as follows:
 *
 * <pre>
 *     [f1]       [f0]
 *     [  ] = M * [  ]
 *     [o1]       [o0]
 * </pre>
 *
 * See the page on \ref sfsnotation for details on some of the
 * terminology used above.
 *
 * The optional NManifold routine getHomologyH1() is implemented for
 * cases where neither Seifert fibred space has any fibre-reversing
 * twists or reflector boundary components.  The optional routine
 * construct() is not implemented.
 */
class NNGSFSPair : public NManifold {
    private:
        NSFSpace* sfs_[2];
            /**< The two bounded Seifert fibred spaces that are joined
                 together. */
        NMatrix2 matchingReln_;
            /**< The matrix describing how the two spaces are joined;
                 see the class notes for details. */

    public:
        /**
         * Creates a new non-geometric pair of Seifert fibred spaces.
         * The two bounded Seifert fibred spaces and the four elements
         * of the 2-by-2 matching matrix are all passed separately.
         * The elements of the matching matrix combine to give the full
         * matrix \a M as follows:
         *
         * <pre>
         *           [ mat00  mat01 ]
         *     M  =  [              ]
         *           [ mat10  mat11 ]
         * </pre>
         *
         * Note that the new object will take ownership of the two given
         * Seifert fibred spaces, and when this object is destroyed the
         * Seifert fibred spaces will be destroyed also.
         *
         * \pre Each Seifert fibred space has a single torus boundary,
         * corresponding to a single puncture in the base orbifold.
         * \pre The given matching matrix has determinant +1 or -1.
         *
         * @param sfs0 the first Seifert fibred space.
         * @param sfs1 the second Seifert fibred space.
         * @param mat00 the (0,0) element of the matching matrix.
         * @param mat01 the (0,1) element of the matching matrix.
         * @param mat10 the (1,0) element of the matching matrix.
         * @param mat11 the (1,1) element of the matching matrix.
         */
        NNGSFSPair(NSFSpace* sfs0, NSFSpace* sfs1, long mat00, long mat01,
            long mat10, long mat11);
        /**
         * Creates a new non-geometric pair of Seifert fibred spaces.
         * The two bounded Seifert fibred spaces and the entire 2-by-2
         * matching matrix are each passed separately.
         *
         * Note that the new object will take ownership of the two given
         * Seifert fibred spaces, and when this object is destroyed the
         * Seifert fibred spaces will be destroyed also.
         *
         * \pre Each Seifert fibred space has a single torus boundary,
         * corresponding to a single puncture in the base orbifold.
         * \pre The given matching matrix has determinant +1 or -1.
         *
         * @param sfs0 the first Seifert fibred space.
         * @param sfs1 the second Seifert fibred space.
         * @param matchingReln the 2-by-2 matching matrix.
         */
        NNGSFSPair(NSFSpace* sfs0, NSFSpace* sfs1,
            const NMatrix2& matchingReln);
        /**
         * Destroys this structure along with the component Seifert
         * fibred spaces and the matching matrix.
         */
        ~NNGSFSPair();

        /**
         * Returns a reference to one of the two bounded Seifert fibred
         * spaces that are joined together.
         *
         * @param which 0 if the first Seifert fibred space is to be
         * returned, or 1 if the second space is to be returned.
         * @return a reference to the requested Seifert fibred space.
         */
        const NSFSpace& sfs(unsigned which) const;
        /**
         * Returns a reference to the 2-by-2 matrix describing how the
         * two Seifert fibred spaces are joined together.  See the class
         * notes for details on precisely how this matrix is represented.
         *
         * @return a reference to the matching matrix.
         */
        const NMatrix2& matchingReln() const;

        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;

    private:
        /**
         * Uses (1,1) twists, reflections and other techniques to make
         * the presentation of this space more aesthetically pleasing.
         */
        void reduce();

        /**
         * Uses 180 degree rotation and/or individual space reflections
         * to make the given matching matrix more aesthetically pleasing.
         *
         * This routine is for internal use by reduce().
         *
         * \pre Both Seifert fibred spaces must have obstruction
         * constant zero.
         *
         * @param reln the matching matrix to simplify.
         * @param fibres0 the number of exceptional fibres in the space
         * \a sfs_[0].
         * @param fibres1 the number of exceptional fibres in the space
         * \a sfs_[1].
         * @param ref0 used to return \c true if space \a sfs_[0] was
         * reflected, or \c false if not.
         * @param ref1 used to return \c true if space \a sfs_[1] was
         * reflected, or \c false if not.
         */
        static void reduceReflect(NMatrix2& reln, unsigned long fibres0,
            unsigned long fibres1, bool& ref0, bool& ref1);

        /**
         * Uses 180 degree rotation to make the given matching matrix
         * more aesthetically pleasing.
         *
         * This routine is for internal use by reduce().
         *
         * @param reln the matching matrix to simplify.
         */
        static void reduceSign(NMatrix2& reln);

        /**
         * Decides whether the first given matrix is more aesthetically
         * pleasing than the second.  This judgement is somewhat
         * arbitrary and is subject to change in future versions of Regina.
         *
         * This routine is for internal use by reduce().
         *
         * \warning This routine might behave differently from similar
         * routines in other classes (as a result of differing aesthetic
         * requirements).
         *
         * @param m1 the first matrix to examine.
         * @param m2 the second matrix to examine.
         * @return \c true if \a m1 is declared to be more pleasing than
         * \a m2, or \c false if \a m2 is more pleasing or a decision
         * could not be reached.
         */
        static bool simpler(const NMatrix2& m1, const NMatrix2& m2);
};

/*@}*/

// Inline functions for NNGSFSPair

inline NNGSFSPair::NNGSFSPair(NSFSpace* sfs0, NSFSpace* sfs1,
        long mat00, long mat01, long mat10, long mat11) :
        matchingReln_(mat00, mat01, mat10, mat11) {
    sfs_[0] = sfs0;
    sfs_[1] = sfs1;

    reduce();
}

inline NNGSFSPair::NNGSFSPair(NSFSpace* sfs0, NSFSpace* sfs1,
        const NMatrix2& matchingReln) : matchingReln_(matchingReln) {
    sfs_[0] = sfs0;
    sfs_[1] = sfs1;

    reduce();
}

inline const NSFSpace& NNGSFSPair::sfs(unsigned which) const {
    return *sfs_[which];
}

inline const NMatrix2& NNGSFSPair::matchingReln() const {
    return matchingReln_;
}

} // namespace regina

#endif

