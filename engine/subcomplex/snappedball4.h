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

/*! \file subcomplex/snappedball4.h
 *  \brief Deals with snapped 4-balls in a triangulation.
 */

#ifndef __REGINA_SNAPPEDBALL4_H
#ifndef __DOXYGEN
#define __REGINA_SNAPPEDBALL4_H
#endif

#include "regina-core.h"
#include "subcomplex/standardtri4.h"
#include "triangulation/dim4.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * Represents a snapped 4-ball in a triangulation.
 *
 * A snapped 4-ball is a single pentachoron with two tetrahedral facets
 * glued to each other.  These two facets share a common triangle, and the
 * gluing folds the facets across this triangle to form a 4-ball with a
 * three-tetrahedron boundary.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the other StandardTriangulation4
 * subclasses.  Note that the only way to create these objects (aside from
 * copying or moving) is via the static member function recognise().
 *
 * \ingroup subcomplex
 */
class SnappedBall4 : public StandardTriangulation4 {
    private:
        Pentachoron<4>* pent_;
            /**< The pentachoron that forms the snapped ball. */
        int equator_;
            /**< The edge common to all boundary tetrahedra. */

    public:
        /**
         * Creates a new copy of the given structure.
         */
        SnappedBall4(const SnappedBall4&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * \return a reference to this structure.
         */
        SnappedBall4& operator = (const SnappedBall4&) = default;

        /**
         * Swaps the contents of this and the given structure.
         *
         * \param other the structure whose contents should be swapped
         * with this.
         */
        void swap(SnappedBall4& other) noexcept;

        /**
         * Returns the pentachoron that forms this snapped ball.
         *
         * \return the pentachoron.
         */
        Pentachoron<4>* pentachoron() const;

        /**
         * Returns one of the three pentachoron tetrahedra that forms the
         * boundary of this snapped ball.
         *
         * You are guaranteed that indices 0, 1 and 2 will return tetrahedron
         * numbers in increasing order.
         *
         * \param index specifies which of the three boundary tetrahedra to
         * return; this must be 0, 1 or 2.
         * \return the corresponding tetrahedron number in the pentachoron.
         */
        int boundaryTetrahedron(int index) const;
        /**
         * Returns one of the two pentachoron tetrahedra internal to this
         * snapped ball.
         *
         * These are the two tetrahedral facets that are folded together.
         * You are guaranteed that index 0 will return a smaller tetrahedron
         * number than index 1.
         *
         * \param index specifies which of the two internal tetrahedra to
         * return; this must be either 0 or 1.
         * \return the corresponding tetrahedron number in the pentachoron.
         */
        int internalTetrahedron(int index) const;
        /**
         * Returns the edge that forms the equator of the boundary sphere
         * of this ball.
         *
         * This edge is common to all three boundary tetrahedra.
         *
         * \return the corresponding edge number in the pentachoron.
         */
        int equatorEdge() const;
        /**
         * Returns the triangle internal to this snapped ball.
         *
         * This triangle is common to the two folded tetrahedra.
         *
         * \return the corresponding triangle number in the pentachoron.
         */
        int internalTriangle() const;

        /**
         * Determines whether this and the given object represent the same
         * specific presentation of a snapped 4-ball.
         *
         * Specifically, two snapped 4-balls will compare as equal if and only
         * if each uses the same numbered pentachoron, and has the same
         * numbered equator edge within that pentachoron.  That is, the
         * pentachora returned by pentachoron() must have equal indices within
         * the triangulation, and the edge numbers returned by equatorEdge()
         * must be equal.  In particular, it is still meaningful to compare
         * snapped 4-balls within different triangulations.
         *
         * \param other the snapped 4-ball to compare with this.
         * \return \c true if and only if this and the given object represent
         * the same specific presentation of a snapped 4-ball.
         */
        bool operator == (const SnappedBall4& other) const;

        /**
         * Determines if the given pentachoron forms a snapped 4-ball
         * within a triangulation.  The ball need not be the entire
         * triangulation; the boundary tetrahedra may be glued to something
         * else.
         *
         * Note that the three boundary tetrahedra of the snapped 4-ball
         * need not be boundary tetrahedra within the overall triangulation.
         *
         * \param pent the pentachoron to examine as a potential 4-ball.
         * \return a structure containing details of the snapped 4-ball, or
         * \c null if the given pentachoron is not a snapped 4-ball.
         */
        static std::unique_ptr<SnappedBall4> recognise(Pentachoron<4>* pent);

        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextShort(std::ostream& out) const override;

    private:
        /**
         * Creates a new structure containing the given internal data.
         */
        SnappedBall4(Pentachoron<4>* pent, int equator);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls SnappedBall4::swap(); it is provided
 * so that SnappedBall4 meets the C++ Swappable requirements.
 *
 * \param a the first structure whose contents should be swapped.
 * \param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(SnappedBall4& a, SnappedBall4& b) noexcept;

// Inline functions for SnappedBall4

inline SnappedBall4::SnappedBall4(Pentachoron<4>* pent, int equator) :
        pent_(pent), equator_(equator) {
}

inline void SnappedBall4::swap(SnappedBall4& other) noexcept {
    std::swap(pent_, other.pent_);
    std::swap(equator_, other.equator_);
}

inline Pentachoron<4>* SnappedBall4::pentachoron() const {
    return pent_;
}
inline int SnappedBall4::boundaryTetrahedron(int index) const {
    int seen = 0;
    for (int i = 0; i < 5; ++i)
        if (i != Edge<4>::edgeVertex[equator_][0] &&
                i != Edge<4>::edgeVertex[equator_][1])
            if (seen++ == index)
                return i;
    return -1;
}
inline int SnappedBall4::internalTetrahedron(int index) const {
    return Edge<4>::edgeVertex[equator_][index];
}
inline int SnappedBall4::equatorEdge() const {
    return equator_;
}
inline int SnappedBall4::internalTriangle() const {
    return equator_;
}

inline bool SnappedBall4::operator == (const SnappedBall4& other) const {
    return pent_->index() == other.pent_->index() &&
        equator_ == other.equator_;
}

inline std::ostream& SnappedBall4::writeName(std::ostream& out) const {
    return out << "OSB";
}
inline std::ostream& SnappedBall4::writeTeXName(std::ostream& out) const {
    return out << "\\mathrm{OSB}";
}
inline void SnappedBall4::writeTextShort(std::ostream& out) const {
    out << "Snapped 4-ball, internal triangle "
        << pent_->index() << " ("
        << Triangle<4>::triangleVertex[equator_][0]
        << Triangle<4>::triangleVertex[equator_][1]
        << Triangle<4>::triangleVertex[equator_][2] << ')';
}

inline void swap(SnappedBall4& a, SnappedBall4& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif
