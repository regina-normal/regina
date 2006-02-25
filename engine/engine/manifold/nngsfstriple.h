
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

/*! \file nngsfstriple.h
 *  \brief Deals with graph manifolds formed from sequences of three Seifert
 *  fibred spaces.
 */

#ifndef __NNGSFSTRIPLE_H
#ifndef __DOXYGEN
#define __NNGSFSTRIPLE_H
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
 * Represents a closed graph manifold formed by joining
 * three bounded Seifert fibred spaces along their torus boundaries.
 *
 * There must be one Seifert fibred space at either end, each with a
 * single torus boundary (corresponding to a single puncture in the
 * base orbifold, with no fibre-reversing twist around this puncture).
 * Each of these end spaces is joined to the space in the centre, which
 * has two disjoint torus boundaries (corresponding to two punctures
 * in the base orbifold, again with no fibre-reversing twists around
 * these punctures).
 *
 * This configuration is illustrated in the diagram below.  The large
 * boxes represent the bounded Seifert fibred spaces, and the small
 * tunnels show how their boundaries are joined.
 *
 * <pre>
 *     /---------------\   /-----------------\   /---------------\
 *     |               |   |                 |   |               |
 *     |  End space 0   ---   Central space   ---   End space 1  |
 *     |                ---                   ---                |
 *     |               |   |                 |   |               |
 *     \---------------/   \-----------------/   \---------------/
 * </pre>
 *
 * The way in which each pair of spaces is joined is specified by a
 * 2-by-2 matrix.  This matrix expresses the locations of the fibres and
 * base orbifold of the corresponding end space in terms of the central
 * space.
 *
 * More specifically, consider the matrix \a M that describes the
 * joining of the central space and the first end space (marked above as
 * end space 0).  Suppose that \a f and \a o are generators of the common
 * boundary torus, where \a f represents a directed fibre in the central
 * space and \a o represents the oriented boundary of the corresponding
 * base orbifold.  Likewise, let \a f0 and \a o0 be generators of the
 * common boundary torus representing a directed fibre and the base
 * orbifold of the first end space.  Then the curves \a f, \a o,
 * \a f0 and \a o0 are related as follows:
 *
 * <pre>
 *     [f0]       [f ]
 *     [  ] = M * [  ]
 *     [o0]       [o ]
 * </pre>
 *
 * Likewise, let matrix \a M' describe the joining of the central
 * space and the second end space (marked in the diagram above as end
 * space 1).  Let \a f' and \a o' be curves on the common boundary torus
 * representing the fibres and the base orbifold of the central space,
 * and let \a f1 and \a o1 be curves on this same torus representing the
 * fibres and the base orbifold of the second end space.  Then the curves
 * \a f', \a o', \a f1 and \a o1 are related as follows:
 *
 * <pre>
 *     [f1]        [f']
 *     [  ] = M' * [  ]
 *     [o1]        [o']
 * </pre>
 *
 * See the page on \ref sfsnotation for details on some of the
 * terminology used above.
 *
 * The optional NManifold routine getHomologyH1() is implemented, but
 * the optional routine construct() is not.
 *
 * \todo \opt Speed up homology calculations involving orientable base
 * spaces by adding rank afterwards, instead of adding generators for
 * genus into the presentation matrix.
 */
class NNGSFSTriple : public NManifold {
    private:
        NSFSpace* end_[2];
            /**< The two end spaces, i.e., the Seifert fibred spaces
                 with just one boundary torus. */
        NSFSpace* centre_;
            /**< The central space, i.e., the Seifert fibred space with
                 two boundary tori that meets both end spaces. */
        NMatrix2 matchingReln_[2];
            /**< The matrices describing how the various spaces are joined.
                 In particular, matrix \a matchingReln_[i] describes how
                 the central space is joined to end space \a i.  See the
                 class notes for further details. */

    public:
        /**
         * Creates a new graph manifold from three bounded
         * Seifert fibred spaces, as described in the class notes.
         * The three Seifert fibred spaces and both 2-by-2 matching
         * matrices are passed separately.
         *
         * Note that the new object will take ownership of the three given
         * Seifert fibred spaces, and when this object is destroyed the
         * Seifert fibred spaces will be destroyed also.
         *
         * \pre Spaces \a end0 and \a end1 each have a single torus
         * boundary, corresponding to a single untwisted puncture in the
         * base orbifold.
         * \pre Space \a centre has two disjoint torus boundaries,
         * corresponding to two untwisted punctures in the base orbifold.
         * \pre Each of the given matrices has determinant +1 or -1.
         *
         * @param end0 the first end space, as described in the class notes.
         * @param centre the central space, as described in the class notes.
         * @param end1 the second end space, as described in the class notes.
         * @param matchingReln0 the 2-by-2 matching matrix that
         * specifies how spaces \a end0 and \a centre are joined.
         * @param matchingReln1 the 2-by-2 matching matrix that
         * specifies how spaces \a end1 and \a centre are joined.
         */
        NNGSFSTriple(NSFSpace* end0, NSFSpace* centre, NSFSpace* end1,
            const NMatrix2& matchingReln0, const NMatrix2& matchingReln1);
        /**
         * Destroys this structure along with the component Seifert
         * fibred spaces and matching matrices.
         */
        ~NNGSFSTriple();

        /**
         * Returns a reference to one of the two end spaces.
         * These are the Seifert fibred spaces with just one boundary
         * component, to be joined to the central space.  See the class
         * notes for further discussion.
         *
         * @param which 0 if the first end space is to be returned, or
         * 1 if the second end space is to be returned.
         * @return a reference to the requested Seifert fibred space.
         */
        const NSFSpace& end(unsigned which) const;
        /**
         * Returns a reference to the central space.
         * This is the Seifert fibred space with two boundary components,
         * to which the two end spaces are joined.  See the class notes
         * for further discussion.
         *
         * @return a reference to the requested Seifert fibred space.
         */
        const NSFSpace& centre() const;
        /**
         * Returns a reference to the 2-by-2 matrix describing how the
         * two requested bounded Seifert fibred spaces are joined together.
         * See the class notes for details on precisely how these matrices
         * are represented.
         *
         * The argument \a which indicates which particular join should be
         * examined.  A value of 0 denotes the join between the central
         * space and the first end space (corresponding to matrix \a M
         * in the class notes), whereas a value of 1 denotes the join
         * between the central space and the second end space
         * (corresponding to matrix \a M' in the class notes).
         *
         * @param which indicates which particular join should be
         * examined; this should be 0 or 1 as described above.
         * @return a reference to the requested matching matrix.
         */
        const NMatrix2& matchingReln(unsigned which) const;

        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;

    private:
        /**
         * Uses (1,1) twists and other techniques to make the presentation
         * of this manifold more aesthetically pleasing.
         */
        void reduce();

        /**
         * Uses 180 degree rotation and/or reflection of an end space
         * to make the given matching matrix more aesthetically pleasing.
         *
         * This routine is for internal use by reduce().
         *
         * @param reln the matching matrix to simplify.
         * @param fibres the number of exceptional fibres in the
         * corresponding end space.
         * @param ref used to return \c true if the end space was
         * reflected, or \c false if not.
         */
        static void reduceReflectEnd(NMatrix2& reln, unsigned long fibres,
            bool& ref);

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

// Inline functions for NNGSFSTriple

inline NNGSFSTriple::NNGSFSTriple(NSFSpace* end0, NSFSpace* centre,
        NSFSpace* end1, const NMatrix2& matchingReln0,
        const NMatrix2& matchingReln1) {
    end_[0] = end0;
    centre_ = centre;
    end_[1] = end1;

    matchingReln_[0] = matchingReln0;
    matchingReln_[1] = matchingReln1;

    reduce();
}

inline const NSFSpace& NNGSFSTriple::end(unsigned which) const {
    return *end_[which];
}

inline const NSFSpace& NNGSFSTriple::centre() const {
    return *centre_;
}

inline const NMatrix2& NNGSFSTriple::matchingReln(unsigned which) const {
    return matchingReln_[which];
}

} // namespace regina

#endif

