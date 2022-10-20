
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

/*! \file manifold/graphpair.h
 *  \brief Deals with graph manifolds formed from pairs of Seifert fibred
 *  spaces.
 */

#ifndef __REGINA_GRAPHPAIR_H
#ifndef __DOXYGEN
#define __REGINA_GRAPHPAIR_H
#endif

#include "regina-core.h"
#include "manifold/manifold.h"
#include "manifold/sfs.h"
#include "maths/matrix2.h"

namespace regina {

/**
 * Represents a closed graph manifold formed by joining
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
 * The optional Manifold routine homology() is implemented, but
 * the optional routine construct() is not.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  Note, however, that
 * GraphPair still requires a non-trivial (but constant sized) amount of data
 * to be copied even in a move operation.
 *
 * \todo \opt Speed up homology calculations involving orientable base
 * spaces by adding rank afterwards, instead of adding generators for
 * genus into the presentation matrix.
 *
 * \ingroup manifold
 */
class GraphPair : public Manifold {
    private:
        SFSpace sfs_[2];
            /**< The two bounded Seifert fibred spaces that are joined
                 together. */
        Matrix2 matchingReln_;
            /**< The matrix describing how the two spaces are joined;
                 see the class notes for details. */

    public:
        /**
         * Creates a new graph manifold as a pair of joined Seifert fibred
         * spaces.  The two bounded Seifert fibred spaces and the four
         * elements of the 2-by-2 matching matrix are all passed separately.
         * The elements of the matching matrix combine to give the full
         * matrix \a M as follows:
         *
         * <pre>
         *           [ mat00  mat01 ]
         *     M  =  [              ]
         *           [ mat10  mat11 ]
         * </pre>
         *
         * \pre The given matching matrix has determinant +1 or -1.
         *
         * \exception InvalidArgument One of the given Seifert fibred spaces
         * does not have precisely one torus boundary, corresponding to a
         * single untwisted puncture in its base orbifold.
         *
         * \param sfs0 the first Seifert fibred space.
         * \param sfs1 the second Seifert fibred space.
         * \param mat00 the (0,0) element of the matching matrix.
         * \param mat01 the (0,1) element of the matching matrix.
         * \param mat10 the (1,0) element of the matching matrix.
         * \param mat11 the (1,1) element of the matching matrix.
         */
        GraphPair(const SFSpace& sfs0, const SFSpace& sfs1,
            long mat00, long mat01, long mat10, long mat11);
        /**
         * Creates a new graph manifold from a pair of bounded Seifert
         * fibred spaces, which are moved instead of copied.
         *
         * Other than its use of move semantics, this behaves identically
         * to the other constructor that takes the Seifert fibred spaces
         * by const reference.  See that constructor for further details.
         *
         * \pre The given matching matrix has determinant +1 or -1.
         *
         * \exception InvalidArgument One of the given Seifert fibred spaces
         * does not have precisely one torus boundary, corresponding to a
         * single untwisted puncture in its base orbifold.
         *
         * \param sfs0 the first Seifert fibred space.
         * \param sfs1 the second Seifert fibred space.
         * \param mat00 the (0,0) element of the matching matrix.
         * \param mat01 the (0,1) element of the matching matrix.
         * \param mat10 the (1,0) element of the matching matrix.
         * \param mat11 the (1,1) element of the matching matrix.
         */
        GraphPair(SFSpace&& sfs0, SFSpace&& sfs1,
            long mat00, long mat01, long mat10, long mat11);
        /**
         * Creates a new graph manifold as a pair of joined Seifert fibred
         * spaces.  The two bounded Seifert fibred spaces and the entire
         * 2-by-2 matching matrix are each passed separately.
         *
         * \pre The given matching matrix has determinant +1 or -1.
         *
         * \exception InvalidArgument One of the given Seifert fibred spaces
         * does not have precisely one torus boundary, corresponding to a
         * single untwisted puncture in its base orbifold.
         *
         * \param sfs0 the first Seifert fibred space.
         * \param sfs1 the second Seifert fibred space.
         * \param matchingReln the 2-by-2 matching matrix.
         */
        GraphPair(const SFSpace& sfs0, const SFSpace& sfs1,
            const Matrix2& matchingReln);
        /**
         * Creates a new graph manifold from a pair of bounded Seifert
         * fibred spaces, which are moved instead of copied.
         *
         * Other than its use of move semantics, this behaves identically
         * to the other constructor that takes the Seifert fibred spaces
         * by const reference.  See that constructor for further details.
         *
         * \pre The given matching matrix has determinant +1 or -1.
         *
         * \exception InvalidArgument One of the given Seifert fibred spaces
         * does not have precisely one torus boundary, corresponding to a
         * single untwisted puncture in its base orbifold.
         *
         * \param sfs0 the first Seifert fibred space.
         * \param sfs1 the second Seifert fibred space.
         * \param matchingReln the 2-by-2 matching matrix.
         */
        GraphPair(SFSpace&& sfs0, SFSpace&& sfs1, const Matrix2& matchingReln);
        /**
         * Creates a clone of the given graph manifold.
         */
        GraphPair(const GraphPair&) = default;
        /**
         * Moves the contents of the given graph manifold into this
         * new graph manifold.  This is a constant time operation.
         *
         * The graph manifold that was passed will no longer be usable.
         */
        GraphPair(GraphPair&&) noexcept = default;

        /**
         * Returns a reference to one of the two bounded Seifert fibred
         * spaces that are joined together.
         *
         * \param which 0 if the first Seifert fibred space is to be
         * returned, or 1 if the second space is to be returned.
         * @return a reference to the requested Seifert fibred space.
         */
        const SFSpace& sfs(unsigned which) const;
        /**
         * Returns a reference to the 2-by-2 matrix describing how the
         * two Seifert fibred spaces are joined together.  See the class
         * notes for details on precisely how this matrix is represented.
         *
         * @return a reference to the matching matrix.
         */
        const Matrix2& matchingReln() const;

        /**
         * Determines in a fairly ad-hoc fashion whether this
         * representation of this space is "smaller" than the given
         * representation of the given space.
         *
         * The ordering imposed on graph manifolds is purely aesthetic
         * on the part of the author, and is subject to change in future
         * versions of Regina.  It also depends upon the particular
         * representation, so that different representations of the same
         * space may be ordered differently.
         *
         * All that this routine really offers is a well-defined way of
         * ordering graph manifold representations.
         *
         * \param compare the representation with which this will be compared.
         * @return \c true if and only if this is "smaller" than the
         * given graph manifold representation.
         */
        bool operator < (const GraphPair& compare) const;

        /**
         * Sets this to be a clone of the given graph manifold.
         *
         * @return a reference to this graph manifold.
         */
        GraphPair& operator = (const GraphPair&) = default;
        /**
         * Moves the contents of the given graph manifold into this
         * graph manifold.  This is a constant time operation.
         *
         * The graph manifold that was passed will no longer be usable.
         *
         * @return a reference to this graph manifold.
         */
        GraphPair& operator = (GraphPair&&) noexcept = default;

        /**
         * Swaps the contents of this and the given graph manifold.
         *
         * \param other the graph manifold whose contents should be swapped
         * with this.
         */
        void swap(GraphPair& other) noexcept;

        /**
         * Determines whether this and the given object contain precisely
         * the same presentations of the same graph manifold.
         *
         * This routine does \e not test for homeomorphism.  Instead it
         * compares the exact presentations, including the matching matrix
         * and the specific presentations of the bounded Seifert fibred spaces,
         * and determines whether or not these \e presentations are identical.
         * If you have two different presentations of the same graph manifold,
         * they will be treated as not equal by this routine.
         *
         * \param compare the presentation with which this will be compared.
         * @return \c true if and only if this and the given object contain
         * identical presentations of the same graph manifold.
         */
        bool operator == (const GraphPair& compare) const;

        /**
         * Determines whether this and the given object do not contain
         * precisely the same presentations of the same graph manifold.
         *
         * This routine does \e not test for homeomorphism.  Instead it
         * compares the exact presentations, including the matching matrix
         * and the specific presentations of the bounded Seifert fibred spaces,
         * and determines whether or not these \e presentations are identical.
         * If you have two different presentations of the same graph manifold,
         * they will be treated as not equal by this routine.
         *
         * \param compare the presentation with which this will be compared.
         * @return \c true if and only if this and the given object do not
         * contain identical presentations of the same graph manifold.
         */
        bool operator != (const GraphPair& compare) const;

        AbelianGroup homology() const override;
        bool isHyperbolic() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;

    private:
        /**
         * Ensures that the preconditions on the internal Seifert fibred
         * spaces are satisfied.
         *
         * This should be called from every class constructor.
         *
         * \exception InvalidArgument The preconditions were not met.
         */
        void verifySFS();

        /**
         * Uses (1,1) twists, reflections and other techniques to make
         * the presentation of this space more aesthetically pleasing.
         */
        void reduce();

        /**
         * Uses 180 degree rotation to make the given matching matrix
         * more aesthetically pleasing.
         *
         * This routine is for internal use by reduce().
         *
         * \param reln the matching matrix to simplify.
         */
        static void reduceSign(Matrix2& reln);
};

/**
 * Swaps the contents of the two given graph manifolds.
 *
 * This global routine simply calls GraphPair::swap(); it is provided so
 * that GraphPair meets the C++ Swappable requirements.
 *
 * \param a the first graph manifold whose contents should be swapped.
 * \param b the second graph manifold whose contents should be swapped.
 *
 * \ingroup manifold
 */
void swap(GraphPair& a, GraphPair& b) noexcept;

// Inline functions for GraphPair

inline GraphPair::GraphPair(const SFSpace& sfs0, const SFSpace& sfs1,
        long mat00, long mat01, long mat10, long mat11) :
        sfs_ { sfs0, sfs1 }, matchingReln_(mat00, mat01, mat10, mat11) {
    verifySFS();
    reduce();
}

inline GraphPair::GraphPair(SFSpace&& sfs0, SFSpace&& sfs1,
        long mat00, long mat01, long mat10, long mat11) :
        sfs_ { std::move(sfs0), std::move(sfs1) },
        matchingReln_(mat00, mat01, mat10, mat11) {
    verifySFS();
    reduce();
}

inline GraphPair::GraphPair(const SFSpace& sfs0, const SFSpace& sfs1,
        const Matrix2& matchingReln) :
        sfs_ { sfs0, sfs1 }, matchingReln_(matchingReln) {
    verifySFS();
    reduce();
}

inline GraphPair::GraphPair(SFSpace&& sfs0, SFSpace&& sfs1,
        const Matrix2& matchingReln) :
        sfs_ { std::move(sfs0), std::move(sfs1) },
        matchingReln_(matchingReln) {
    verifySFS();
    reduce();
}

inline void GraphPair::swap(GraphPair& other) noexcept {
    sfs_[0].swap(other.sfs_[0]);
    sfs_[1].swap(other.sfs_[1]);
    matchingReln_.swap(other.matchingReln_);
}

inline const SFSpace& GraphPair::sfs(unsigned which) const {
    return sfs_[which];
}

inline const Matrix2& GraphPair::matchingReln() const {
    return matchingReln_;
}

inline bool GraphPair::isHyperbolic() const {
    return false;
}

inline bool GraphPair::operator == (const GraphPair& compare) const {
    return
        sfs_[0] == compare.sfs_[0] &&
        sfs_[1] == compare.sfs_[1] &&
        matchingReln_ == compare.matchingReln_;
}

inline bool GraphPair::operator != (const GraphPair& compare) const {
    return
        sfs_[0] != compare.sfs_[0] ||
        sfs_[1] != compare.sfs_[1] ||
        matchingReln_ != compare.matchingReln_;
}

inline void swap(GraphPair& a, GraphPair& b) noexcept {
    a.swap(b);
}

inline void GraphPair::verifySFS() {
    if (sfs_[0].punctures(false) != 1 || sfs_[0].punctures(true) != 0 ||
            sfs_[1].punctures(false) != 1 || sfs_[1].punctures(true) != 0)
        throw InvalidArgument("GraphPair requires its internal Seifert "
            "fibred spaces to each have a base orbifold with precisely "
            "one puncture, which must be untwisted");
}

} // namespace regina

#endif

