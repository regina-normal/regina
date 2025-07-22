
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file manifold/graphloop.h
 *  \brief Deals with graph manifolds formed from self-identified Seifert
 *  fibred spaces.
 */

#ifndef __REGINA_GRAPHLOOP_H
#ifndef __DOXYGEN
#define __REGINA_GRAPHLOOP_H
#endif

#include "regina-core.h"
#include "manifold/manifold.h"
#include "manifold/sfs.h"
#include "maths/matrix2.h"

namespace regina {

/**
 * Represents a closed graph manifold formed by joining a
 * single bounded Seifert fibred space to itself along a torus.
 *
 * The Seifert fibred space must have two boundary components, each a
 * torus corresponding to a puncture in the base orbifold (with no
 * fibre-reversing twist as one travels around the boundary).
 *
 * The way in which the two torus boundaries are joined together is specified
 * by a 2-by-2 matrix \a M.  This matrix relates the locations of the fibres
 * and base orbifold on the two boundary tori.
 *
 * More specifically, suppose that \a f0 and \a o0 are generators of the
 * first boundary torus, where \a f0 represents a directed fibre in the
 * Seifert fibred space and \a o0 represents the oriented boundary of
 * the base orbifold.  Likewise, let \a f1 and \a o1 be generators of the
 * second boundary torus representing a directed fibre and the oriented
 * boundary of the base orbifold.  Then the tori are joined together so
 * that the curves \a f0, \a o0, \a f1 and \a o1 become related as follows:
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
 * GraphLoop still requires a non-trivial (but constant sized) amount of data
 * to be copied even in a move operation.
 *
 * \todo \opt Speed up homology calculations involving orientable base
 * spaces by adding rank afterwards, instead of adding generators for
 * genus into the presentation matrix.
 *
 * \ingroup manifold
 */
class GraphLoop : public Manifold {
    private:
        SFSpace sfs_;
            /**< The bounded Seifert fibred space that is joined to itself. */
        Matrix2 matchingReln_;
            /**< The matrix describing how the two boundary tori are joined;
                 see the class notes for details. */

    public:
        /**
         * Creates a new graph manifold as a self-identified Seifert fibred
         * space.  The bounded Seifert fibred space and the four elements of
         * the 2-by-2 matching matrix are all passed separately.  The elements
         * of the matching matrix combine to give the full matrix \a M as
         * follows:
         *
         * <pre>
         *           [ mat00  mat01 ]
         *     M  =  [              ]
         *           [ mat10  mat11 ]
         * </pre>
         *
         * \pre The given matching matrix has determinant +1 or -1.
         *
         * \exception InvalidArgument The given Seifert fibred space
         * does not have precisely two torus boundaries, corresponding to
         * two untwisted punctures in its base orbifold.
         *
         * \param sfs the bounded Seifert fibred space.
         * \param mat00 the (0,0) element of the matching matrix.
         * \param mat01 the (0,1) element of the matching matrix.
         * \param mat10 the (1,0) element of the matching matrix.
         * \param mat11 the (1,1) element of the matching matrix.
         */
        GraphLoop(const SFSpace& sfs, long mat00, long mat01,
            long mat10, long mat11);
        /**
         * Creates a new graph manifold as a self-identified Seifert fibred
         * space, which is moved instead of copied.
         *
         * Other than its use of move semantics, this behaves identically
         * to the other constructor that takes the Seifert fibred space
         * by const reference.  See that constructor for further details.
         *
         * \pre The given matching matrix has determinant +1 or -1.
         *
         * \exception InvalidArgument The given Seifert fibred space
         * does not have precisely two torus boundaries, corresponding to
         * two untwisted punctures in its base orbifold.
         *
         * \param sfs the bounded Seifert fibred space.
         * \param mat00 the (0,0) element of the matching matrix.
         * \param mat01 the (0,1) element of the matching matrix.
         * \param mat10 the (1,0) element of the matching matrix.
         * \param mat11 the (1,1) element of the matching matrix.
         */
        GraphLoop(SFSpace&& sfs, long mat00, long mat01,
            long mat10, long mat11);
        /**
         * Creates a new graph manifold as a self-identified Seifert fibred
         * space.  The bounded Seifert fibred space and the entire 2-by-2
         * matching matrix are each passed separately.
         *
         * \pre The given matching matrix has determinant +1 or -1.
         *
         * \exception InvalidArgument The given Seifert fibred space
         * does not have precisely two torus boundaries, corresponding to
         * two untwisted punctures in its base orbifold.
         *
         * \param sfs the bounded Seifert fibred space.
         * \param matchingReln the 2-by-2 matching matrix.
         */
        GraphLoop(const SFSpace& sfs, const Matrix2& matchingReln);
        /**
         * Creates a new graph manifold as a self-identified Seifert fibred
         * space, which is moved instead of copied.
         *
         * Other than its use of move semantics, this behaves identically
         * to the other constructor that takes the Seifert fibred space
         * by const reference.  See that constructor for further details.
         *
         * \pre The given matching matrix has determinant +1 or -1.
         *
         * \exception InvalidArgument The given Seifert fibred space
         * does not have precisely two torus boundaries, corresponding to
         * two untwisted punctures in its base orbifold.
         *
         * \param sfs the bounded Seifert fibred space.
         * \param matchingReln the 2-by-2 matching matrix.
         */
        GraphLoop(SFSpace&& sfs, const Matrix2& matchingReln);
        /**
         * Creates a clone of the given graph manifold.
         */
        GraphLoop(const GraphLoop&) = default;
        /**
         * Moves the contents of the given graph manifold into this
         * new graph manifold.  This is a constant time operation.
         *
         * The graph manifold that was passed will no longer be usable.
         */
        GraphLoop(GraphLoop&&) noexcept = default;

        /**
         * Returns a reference to the bounded Seifert fibred space that
         * is joined to itself.
         *
         * \return a reference to the bounded Seifert fibred space.
         */
        const SFSpace& sfs() const;
        /**
         * Returns a reference to the 2-by-2 matrix describing how the
         * two boundary tori of the Seifert fibred space are joined together.
         * See the class notes for details on precisely how this matrix is
         * represented.
         *
         * \return a reference to the matching matrix.
         */
        const Matrix2& matchingReln() const;

        /**
         * Compares representations of two graph manifolds according to an
         * aesthetic ordering.
         *
         * The only purpose of this routine is to implement a consistent
         * ordering of graph manifold representations.  The specific ordering
         * used is purely aesthetic on the part of the author, and is subject
         * to change in future versions of Regina.
         *
         * It does not matter whether the two manifolds are homeomorphic; this
         * routine compares the specific _representations_ of these manifolds
         * (and so in particular, different representations of the same
         * graph manifold will be ordered differently).
         *
         * This operator generates all of the usual comparison operators,
         * including `<`, `<=`, `>`, and `>=`.
         *
         * \python This spaceship operator `x <=> y` is not available, but the
         * other comparison operators that it generates _are_ available.
         *
         * \param rhs the other representation to compare this with.
         * \return A result that indicates how this and the given graph
         * manifold representation should be ordered with respect to each other.
         */
        std::strong_ordering operator <=> (const GraphLoop& rhs) const;

        /**
         * Sets this to be a clone of the given graph manifold.
         *
         * \return a reference to this graph manifold.
         */
        GraphLoop& operator = (const GraphLoop&) = default;
        /**
         * Moves the contents of the given graph manifold into this
         * graph manifold.  This is a constant time operation.
         *
         * The graph manifold that was passed will no longer be usable.
         *
         * \return a reference to this graph manifold.
         */
        GraphLoop& operator = (GraphLoop&&) noexcept = default;

        /**
         * Swaps the contents of this and the given graph manifold.
         *
         * \param other the graph manifold whose contents should be swapped
         * with this.
         */
        void swap(GraphLoop& other) noexcept;

        /**
         * Determines whether this and the given object contain precisely
         * the same presentations of the same graph manifold.
         *
         * This routine does _not_ test for homeomorphism.  Instead it
         * compares the exact presentations, including the matching matrix
         * and the specific presentation of the bounded Seifert fibred space,
         * and determines whether or not these _presentations_ are identical.
         * If you have two different presentations of the same graph manifold,
         * they will be treated as not equal by this routine.
         *
         * \param compare the presentation with which this will be compared.
         * \return \c true if and only if this and the given object contain
         * identical presentations of the same graph manifold.
         */
        bool operator == (const GraphLoop& compare) const;

        AbelianGroup homology() const override;
        bool isHyperbolic() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;

    private:
        /**
         * Ensures that the preconditions on the internal Seifert fibred
         * space are satisfied.
         *
         * This should be called from every class constructor.
         *
         * \exception InvalidArgument The preconditions were not met.
         */
        void verifySFS();

        /**
         * Uses (1,1) twists, inversion and/or reflection to make the
         * presentation of this space more aesthetically pleasing.
         */
        void reduce();

        /**
         * Uses (1,1) twists and/or inversion to make the given matching
         * matrix more aesthetically pleasing.
         *
         * This routine is for internal use by reduce().
         *
         * \param reln the matching matrix to simplify.
         */
        static void reduce(Matrix2& reln);

        /**
         * Uses (1,1) twists to make the given matching matrix more
         * aesthetically pleasing.
         *
         * This routine is for internal use by reduce().
         *
         * \param reln the matching matrix to simplify.
         */
        static void reduceBasis(Matrix2& reln);
};

/**
 * Swaps the contents of the two given graph manifolds.
 *
 * This global routine simply calls GraphLoop::swap(); it is provided so
 * that GraphLoop meets the C++ Swappable requirements.
 *
 * \param a the first graph manifold whose contents should be swapped.
 * \param b the second graph manifold whose contents should be swapped.
 *
 * \ingroup manifold
 */
void swap(GraphLoop& a, GraphLoop& b) noexcept;

// Inline functions for GraphLoop

// NOLINTNEXTLINE(modernize-pass-by-value)
inline GraphLoop::GraphLoop(const SFSpace& sfs,
        long mat00, long mat01, long mat10, long mat11) :
        sfs_(sfs), matchingReln_(mat00, mat01, mat10, mat11) {
    verifySFS();
    reduce();
}

// NOLINTNEXTLINE(modernize-pass-by-value)
inline GraphLoop::GraphLoop(const SFSpace& sfs, const Matrix2& matchingReln) :
        sfs_(sfs), matchingReln_(matchingReln) {
    verifySFS();
    reduce();
}

inline GraphLoop::GraphLoop(SFSpace&& sfs,
        long mat00, long mat01, long mat10, long mat11) :
        sfs_(std::move(sfs)), matchingReln_(mat00, mat01, mat10, mat11) {
    verifySFS();
    reduce();
}

inline GraphLoop::GraphLoop(SFSpace&& sfs, const Matrix2& matchingReln) :
        sfs_(std::move(sfs)), matchingReln_(matchingReln) {
    verifySFS();
    reduce();
}

inline void GraphLoop::swap(GraphLoop& other) noexcept {
    sfs_.swap(other.sfs_);
    matchingReln_.swap(other.matchingReln_);
}

inline const SFSpace& GraphLoop::sfs() const {
    return sfs_;
}

inline const Matrix2& GraphLoop::matchingReln() const {
    return matchingReln_;
}

inline bool GraphLoop::isHyperbolic() const {
    return false;
}

inline bool GraphLoop::operator == (const GraphLoop& compare) const {
    return sfs_ == compare.sfs_ && matchingReln_ == compare.matchingReln_;
}

inline void swap(GraphLoop& a, GraphLoop& b) noexcept {
    a.swap(b);
}

inline void GraphLoop::verifySFS() {
    if (sfs_.punctures(false) != 2 || sfs_.punctures(true) != 0)
        throw InvalidArgument("GraphLoop requires its internal Seifert "
            "fibred space to have a base orbifold with precisely "
            "two punctures, both untwisted");
}

} // namespace regina

#endif

