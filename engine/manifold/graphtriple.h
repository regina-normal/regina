
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file manifold/graphtriple.h
 *  \brief Deals with graph manifolds formed from sequences of three Seifert
 *  fibred spaces.
 */

#ifndef __REGINA_GRAPHTRIPLE_H
#ifndef __DOXYGEN
#define __REGINA_GRAPHTRIPLE_H
#endif

#include "regina-core.h"
#include "manifold/manifold.h"
#include "manifold/sfs.h"
#include "maths/matrix2.h"

namespace regina {

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
 * <b>Note that these are not the same matrices that appear in the
 * manifold name in the census data files!</b>  See the warning below.
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
 * The optional Manifold routine homology() is implemented, but
 * the optional routine construct() is not.
 *
 * This class implements C++ move semantics and adheres to the C++ Swappable
 * requirement.  It is designed to avoid deep copies wherever possible,
 * even when passing or returning objects by value.  Note, however, that
 * GraphTriple still requires a non-trivial (but constant sized) amount of data
 * to be copied even in a move operation.
 *
 * \warning The 2-by-2 matrices used in this class are \e not the same
 * matrices that appear in the manifold name returned by name()
 * and TeXName() and seen in the census data files.  The matrices
 * used in this class work from the inside out, describing the boundary torus
 * on each end space in terms of a boundary torus on the central space.
 * The matrices used in the manifold name work from left to right in the
 * diagram above, describing a boundary torus on the central space or
 * rightmost end space in terms of a boundary torus on the leftmost end
 * space or central space respectively.  The upshot of all this is that
 * <b>the first matrix becomes inverted</b> (and the second matrix
 * remains unchanged).  It is likely that future versions of Regina will
 * replace this class with a more general class that (amongst other
 * things) removes this inconsistency.
 *
 * \todo \opt Speed up homology calculations involving orientable base
 * spaces by adding rank afterwards, instead of adding generators for
 * genus into the presentation matrix.
 *
 * \ingroup manifold
 */
class GraphTriple : public Manifold {
    private:
        SFSpace end_[2];
            /**< The two end spaces, i.e., the Seifert fibred spaces
                 with just one boundary torus. */
        SFSpace centre_;
            /**< The central space, i.e., the Seifert fibred space with
                 two boundary tori that meets both end spaces. */
        Matrix2 matchingReln_[2];
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
         * \pre Each of the given matrices has determinant +1 or -1.
         *
         * \exception InvalidArgument one of the spaces \a end0 and \a end1
         * does not have precisely one torus boundary corresponding to a
         * single untwisted puncture in its base orbifold, and/or the space
         * \a centre does not have precisely two disjoint torus boundaries
         * corresponding to two untwisted punctures in its base orbifold.
         *
         * @param end0 the first end space, as described in the class notes.
         * @param centre the central space, as described in the class notes.
         * @param end1 the second end space, as described in the class notes.
         * @param matchingReln0 the 2-by-2 matching matrix that
         * specifies how spaces \a end0 and \a centre are joined.
         * @param matchingReln1 the 2-by-2 matching matrix that
         * specifies how spaces \a end1 and \a centre are joined.
         */
        GraphTriple(const SFSpace& end0, const SFSpace& centre,
            const SFSpace& end1, const Matrix2& matchingReln0,
            const Matrix2& matchingReln1);
        /**
         * Creates a new graph manifold from three bounded Seifert
         * fibred spaces, which are moved instead of copied.
         *
         * Other than its use of move semantics, this behaves identically
         * to the other constructor that takes the Seifert fibred spaces
         * by const reference.
         *
         * \pre Each of the given matrices has determinant +1 or -1.
         *
         * \exception InvalidArgument one of the spaces \a end0 and \a end1
         * does not have precisely one torus boundary corresponding to a
         * single untwisted puncture in its base orbifold, and/or the space
         * \a centre does not have precisely two disjoint torus boundaries
         * corresponding to two untwisted punctures in its base orbifold.
         *
         * @param end0 the first end space, as described in the class notes.
         * @param centre the central space, as described in the class notes.
         * @param end1 the second end space, as described in the class notes.
         * @param matchingReln0 the 2-by-2 matching matrix that
         * specifies how spaces \a end0 and \a centre are joined.
         * @param matchingReln1 the 2-by-2 matching matrix that
         * specifies how spaces \a end1 and \a centre are joined.
         */
        GraphTriple(SFSpace&& end0, SFSpace&& centre, SFSpace&& end1,
            const Matrix2& matchingReln0, const Matrix2& matchingReln1);
        /**
         * Creates a clone of the given graph manifold.
         */
        GraphTriple(const GraphTriple&) = default;
        /**
         * Moves the contents of the given graph manifold into this
         * new graph manifold.  This is a constant time operation.
         *
         * The graph manifold that was passed will no longer be usable.
         */
        GraphTriple(GraphTriple&&) noexcept = default;

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
        const SFSpace& end(unsigned which) const;
        /**
         * Returns a reference to the central space.
         * This is the Seifert fibred space with two boundary components,
         * to which the two end spaces are joined.  See the class notes
         * for further discussion.
         *
         * @return a reference to the requested Seifert fibred space.
         */
        const SFSpace& centre() const;
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
        const Matrix2& matchingReln(unsigned which) const;

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
         * @param compare the representation with which this will be compared.
         * @return \c true if and only if this is "smaller" than the
         * given graph manifold representation.
         */
        bool operator < (const GraphTriple& compare) const;

        /**
         * Sets this to be a clone of the given graph manifold.
         *
         * @return a reference to this graph manifold.
         */
        GraphTriple& operator = (const GraphTriple&) = default;
        /**
         * Moves the contents of the given graph manifold into this
         * graph manifold.  This is a constant time operation.
         *
         * The graph manifold that was passed will no longer be usable.
         *
         * @return a reference to this graph manifold.
         */
        GraphTriple& operator = (GraphTriple&&) noexcept = default;

        /**
         * Swaps the contents of this and the given graph manifold.
         *
         * @param other the graph manifold whose contents should be swapped
         * with this.
         */
        void swap(GraphTriple& other) noexcept;

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
         * \exception InvalidArgument the preconditions were not met.
         */
        void verifySFS();

        /**
         * Uses (1,1) twists and other techniques to make the presentation
         * of this manifold more aesthetically pleasing.
         */
        void reduce();

        /**
         * Uses 180 degree rotation and/or (1,1) twists to make the
         * given pair of matching matrices more aesthetically pleasing.
         *
         * This routine is for internal use by reduce().
         *
         * @param reln0 the first matching matrix in the pair to simplify.
         * @param reln1 the second matching matrix in the pair to simplify.
         */
        static void reduceBasis(Matrix2& reln0, Matrix2& reln1);

        /**
         * Uses 180 degree rotation to make the given matching matrix
         * more aesthetically pleasing.
         *
         * This routine is for internal use by reduce().
         *
         * @param reln the matching matrix to simplify.
         */
        static void reduceSign(Matrix2& reln);
};

/**
 * Swaps the contents of the two given graph manifolds.
 *
 * This global routine simply calls GraphTriple::swap(); it is provided so
 * that GraphTriple meets the C++ Swappable requirements.
 *
 * @param a the first graph manifold whose contents should be swapped.
 * @param b the second graph manifold whose contents should be swapped.
 *
 * \ingroup manifold
 */
void swap(GraphTriple& a, GraphTriple& b) noexcept;

// Inline functions for GraphTriple

// NOLINTNEXTLINE(modernize-pass-by-value)
inline GraphTriple::GraphTriple(const SFSpace& end0, const SFSpace& centre,
        const SFSpace& end1, const Matrix2& matchingReln0,
        const Matrix2& matchingReln1) :
        end_ { end0, end1 }, centre_(centre),
        matchingReln_ { matchingReln0, matchingReln1 } {
    verifySFS();
    reduce();
}

inline GraphTriple::GraphTriple(SFSpace&& end0, SFSpace&& centre,
        SFSpace&& end1, const Matrix2& matchingReln0,
        const Matrix2& matchingReln1) :
        end_ { std::move(end0), std::move(end1) }, centre_(std::move(centre)),
        matchingReln_ { matchingReln0, matchingReln1 } {
    verifySFS();
    reduce();
}

inline void GraphTriple::swap(GraphTriple& other) noexcept {
    end_[0].swap(other.end_[0]);
    end_[1].swap(other.end_[1]);
    centre_.swap(other.centre_);
    matchingReln_[0].swap(other.matchingReln_[0]);
    matchingReln_[1].swap(other.matchingReln_[1]);
}

inline const SFSpace& GraphTriple::end(unsigned which) const {
    return end_[which];
}

inline const SFSpace& GraphTriple::centre() const {
    return centre_;
}

inline const Matrix2& GraphTriple::matchingReln(unsigned which) const {
    return matchingReln_[which];
}

inline bool GraphTriple::isHyperbolic() const {
    return false;
}

inline void swap(GraphTriple& a, GraphTriple& b) noexcept {
    a.swap(b);
}

inline void GraphTriple::verifySFS() {
    if (end_[0].punctures(false) != 1 || end_[0].punctures(true) != 0 ||
            end_[1].punctures(false) != 1 || end_[1].punctures(true) != 0)
        throw InvalidArgument("GraphTriple requires its end spaces "
            "to each have a base orbifold with precisely one puncture, "
            "which must be untwisted");
    if (centre_.punctures(false) != 2 || centre_.punctures(true) != 0)
        throw InvalidArgument("GraphTriple requires its central space "
            "to have a base orbifold with precisely two punctures, "
            "both untwisted");
}

} // namespace regina

#endif

