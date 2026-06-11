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

/*! \file subcomplex/doublesnappedball.h
 *  \brief Deals with double snapped 4-balls in a triangulation.
 */

#ifndef __REGINA_DOUBLESNAPPEDBALL_H
#ifndef __DOXYGEN
#define __REGINA_DOUBLESNAPPEDBALL_H
#endif

#include "regina-core.h"
#include "subcomplex/standardsubcomplex.h"
#include "triangulation/dim4.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * Represents a double snapped 4-ball in a triangulation.
 *
 * A double snapped 4-ball is a single pentachoron with two disjoint pairs of
 * tetrahedral facets glued together, leaving one tetrahedral facet free as
 * boundary.  One of the two gluings is always a fold, as in a snapped 4-ball.
 * The second gluing determines the subtype:
 *
 * - Type I uses another fold gluing;
 * - Type II uses a twisted gluing, whose permutation is a 4-cycle through the
 *   two identified facets and two vertices of their common triangle, fixing
 *   the remaining vertex of that common triangle.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the other standard triangulation
 * classes.  Note that the only way to create these objects (aside from copying
 * or moving) is via the static member function recognise().
 *
 * \ingroup subcomplex
 */
class DoubleSnappedBall : public StandardSubcomplex<4> {
    public:
        /**
         * Identifies which type of double snapped ball this is.
         */
        enum class Type {
            /**
             * Both internal facet pairings are folds.
             */
            TypeI,
            /**
             * One internal facet pairing is a fold, and the other is twisted.
             */
            TypeII
        };

    private:
        Pentachoron<4>* pent_;
            /**< The pentachoron that forms the double snapped ball. */
        int facet_[2][2];
            /**< The two pairs of identified tetrahedral facets. */
        int boundary_;
            /**< The tetrahedral facet left free as boundary. */
        Type type_;
            /**< The subtype of this double snapped ball. */

    public:
        /**
         * Creates a new copy of the given structure.
         */
        DoubleSnappedBall(const DoubleSnappedBall&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * \return a reference to this structure.
         */
        DoubleSnappedBall& operator = (const DoubleSnappedBall&) = default;

        /**
         * Swaps the contents of this and the given structure.
         *
         * \param other the structure whose contents should be swapped
         * with this.
         */
        void swap(DoubleSnappedBall& other) noexcept;

        /**
         * Returns the pentachoron that forms this double snapped ball.
         *
         * \return the pentachoron.
         */
        Pentachoron<4>* pentachoron() const;

        /**
         * Returns the subtype of this double snapped ball.
         *
         * \return the subtype.
         */
        Type type() const;
        /**
         * Returns the name of the subtype of this double snapped ball.
         *
         * \return either \c "Type I" or \c "Type II".
         */
        const char* typeName() const;

        /**
         * Returns the pentachoron tetrahedron that forms the boundary of this
         * double snapped ball.
         *
         * \return the corresponding tetrahedron number in the pentachoron.
         */
        int boundaryTetrahedron() const;
        /**
         * Returns one of the two pentachoron tetrahedra in one of the internal
         * facet pairings.
         *
         * Each internal pair is stored with its two tetrahedron numbers in
         * increasing order.  For Type II, pair 0 is the fold and pair 1 is the
         * twisted gluing.  For Type I, the two fold pairs appear in
         * lexicographic order.
         *
         * \param pair specifies which internal pair to query; this must be
         * either 0 or 1.
         * \param index specifies which tetrahedron in the pair to return; this
         * must be either 0 or 1.
         * \return the corresponding tetrahedron number in the pentachoron.
         */
        int internalTetrahedron(int pair, int index) const;
        /**
         * Returns the triangle common to one of the two internal facet
         * pairings.
         *
         * \param pair specifies which internal pair to query; this must be
         * either 0 or 1.
         * \return the corresponding triangle number in the pentachoron.
         */
        int internalTriangle(int pair) const;

        /**
         * Determines whether this and the given object represent the same
         * specific presentation of a double snapped 4-ball.
         *
         * Specifically, two double snapped 4-balls will compare as equal if
         * and only if each uses the same numbered pentachoron and the same two
         * internal facet pairings.  In particular, it is still meaningful to
         * compare double snapped 4-balls within different triangulations.
         *
         * \param other the double snapped 4-ball to compare with this.
         * \return \c true if and only if this and the given object represent
         * the same specific presentation of a double snapped 4-ball.
         */
        bool operator == (const DoubleSnappedBall& other) const;

        /**
         * Determines if the given pentachoron forms a double snapped 4-ball
         * within a triangulation.  The ball need not be the entire
         * triangulation; the boundary tetrahedron may be glued to something
         * else.
         *
         * \param pent the pentachoron to examine as a potential double snapped
         * 4-ball.
         * \return a structure containing details of the double snapped 4-ball,
         * or \c null if the given pentachoron is not a double snapped 4-ball.
         */
        static std::unique_ptr<DoubleSnappedBall> recognise(
            Pentachoron<4>* pent);

        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextShort(std::ostream& out) const override;

    private:
        /**
         * Creates a new structure containing the given internal data.
         */
        DoubleSnappedBall(Pentachoron<4>* pent, int first0, int first1,
            int second0, int second1, int boundary, Type type);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls DoubleSnappedBall::swap(); it is provided
 * so that DoubleSnappedBall meets the C++ Swappable requirements.
 *
 * \param a the first structure whose contents should be swapped.
 * \param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(DoubleSnappedBall& a, DoubleSnappedBall& b) noexcept;

// Inline functions for DoubleSnappedBall

inline DoubleSnappedBall::DoubleSnappedBall(Pentachoron<4>* pent,
        int first0, int first1, int second0, int second1, int boundary,
        Type type) : pent_(pent), facet_{{first0, first1}, {second0, second1}},
        boundary_(boundary), type_(type) {
}

inline void DoubleSnappedBall::swap(DoubleSnappedBall& other) noexcept {
    std::swap(pent_, other.pent_);
    std::swap(facet_, other.facet_);
    std::swap(boundary_, other.boundary_);
    std::swap(type_, other.type_);
}

inline Pentachoron<4>* DoubleSnappedBall::pentachoron() const {
    return pent_;
}

inline DoubleSnappedBall::Type DoubleSnappedBall::type() const {
    return type_;
}

inline const char* DoubleSnappedBall::typeName() const {
    return type_ == Type::TypeI ? "Type I" : "Type II";
}

inline int DoubleSnappedBall::boundaryTetrahedron() const {
    return boundary_;
}

inline int DoubleSnappedBall::internalTetrahedron(int pair, int index) const {
    return facet_[pair][index];
}

inline int DoubleSnappedBall::internalTriangle(int pair) const {
    return Edge<4>::edgeNumber[facet_[pair][0]][facet_[pair][1]];
}

inline bool DoubleSnappedBall::operator == (
        const DoubleSnappedBall& other) const {
    return pent_->index() == other.pent_->index() &&
        facet_[0][0] == other.facet_[0][0] &&
        facet_[0][1] == other.facet_[0][1] &&
        facet_[1][0] == other.facet_[1][0] &&
        facet_[1][1] == other.facet_[1][1] &&
        type_ == other.type_;
}

inline std::ostream& DoubleSnappedBall::writeName(std::ostream& out) const {
    return out << "Double snapped 4-ball (" << typeName() << ')';
}

inline std::ostream& DoubleSnappedBall::writeTeXName(
        std::ostream& out) const {
    return out << "\\mathit{DSB}_{"
        << (type_ == Type::TypeI ? "I" : "II") << '}';
}

inline void DoubleSnappedBall::writeTextShort(std::ostream& out) const {
    out << "Double snapped 4-ball (" << typeName() << "), pentachoron "
        << pent_->index();
}

inline void swap(DoubleSnappedBall& a, DoubleSnappedBall& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif
