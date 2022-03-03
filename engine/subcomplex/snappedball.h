
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

/*! \file subcomplex/snappedball.h
 *  \brief Deals with snapped 3-balls in a triangulation.
 */

#ifndef __REGINA_SNAPPEDBALL_H
#ifndef __DOXYGEN
#define __REGINA_SNAPPEDBALL_H
#endif

#include "regina-core.h"
#include "subcomplex/standardtri.h"
#include "triangulation/dim3.h"

namespace regina {

/**
 * Represents a snapped 3-ball in a triangulation.
 * A snapped 3-ball is a single tetrahedron with two faces glued to each
 * other to form a 3-ball with a two triangle boundary.
 *
 * All optional StandardTriangulation routines are implemented for this
 * class.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the other StandardTriangulation
 * subclasses.  Note that the only way to create these objects (aside from
 * copying or moving) is via the static member function recognise().
 *
 * \ingroup subcomplex
 */
class SnappedBall : public StandardTriangulation {
    private:
        Tetrahedron<3>* tet_;
            /**< The tetrahedron that forms the snapped ball. */
        int equator_;
            /**< The edge that forms the equator on the ball boundary. */

    public:
        /**
         * Creates a new copy of this structure.
         */
        SnappedBall(const SnappedBall&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * @return a reference to this structure.
         */
        SnappedBall& operator = (const SnappedBall&) = default;

        /**
         * Deprecated routine that returns a new copy of this structure.
         *
         * \deprecated Just use the copy constructor instead.
         *
         * @return a newly created clone.
         */
        [[deprecated]] SnappedBall* clone() const;

        /**
         * Swaps the contents of this and the given structure.
         *
         * @param other the structure whose contents should be swapped
         * with this.
         */
        void swap(SnappedBall& other) noexcept;

        /**
         * Returns the tetrahedron that forms this snapped ball.
         *
         * @return the tetrahedron.
         */
        Tetrahedron<3>* tetrahedron() const;

        /**
         * Returns one of the two tetrahedron faces that forms the boundary
         * of this snapped ball.
         *
         * You are guaranteed that index 0 will return a smaller face
         * number than index 1.
         *
         * @param index specifies which of the two boundary faces to return;
         * this must be either 0 or 1.
         * @return the corresponding face number in the tetrahedron.
         */
        int boundaryFace(int index) const;
        /**
         * Returns one of the two tetrahedron faces internal to this snapped
         * ball.
         *
         * You are guaranteed that index 0 will return a smaller face
         * number than index 1.
         *
         * @param index specifies which of the two internal faces to return;
         * this must be either 0 or 1.
         * @return the corresponding face number in the tetrahedron.
         */
        int internalFace(int index) const;
        /**
         * Returns the edge that forms the equator of the boundary sphere
         * of this ball.
         *
         * @return the corresponding edge number in the tetrahedron.
         */
        int equatorEdge() const;
        /**
         * Returns the edge internal to this snapped ball.
         *
         * @return the corresponding edge number in the tetrahedron.
         */
        int internalEdge() const;

        /**
         * Determines whether this and the given object represent the same
         * specific presentation of a snapped 3-ball.
         *
         * Unlike the parameterised subclasses of StandardTriangulation,
         * this SnappedBall subclass represents a fixed structure, and
         * so its comparisons test not for the \e structure but the precise
         * \e location of this structure within the enclosing triangulation.
         *
         * Specifically, two snapped 3-balls will compare as equal if and only
         * if each uses the same numbered tetrahedron, and has the same
         * numbered equator edge within that tetrahedron.  That is, the
         * tetrahedra returned by tetrahedron() must have equal indices within
         * the triangulation, and the edge numbers returned by equatorEdge()
         * must be equal.  In particular, it is still meaningful to compare
         * snapped 3-balls within different triangulations.
         *
         * @param other the snapped 3-ball to compare with this.
         * @return \c true if and only if this and the given object represent
         * the same specific presentation of a snapped 3-ball.
         */
        bool operator == (const SnappedBall& other) const;
        /**
         * Determines whether this and the given object represent different
         * specific presentations of a snapped 3-ball.
         *
         * Unlike the parameterised subclasses of StandardTriangulation,
         * this SnappedBall subclass represents a fixed structure, and
         * so its comparisons test not for the \e structure but the precise
         * \e location of this structure within the enclosing triangulation.
         *
         * Specifically, two snapped 3-balls will compare as equal if and only
         * if each uses the same numbered tetrahedron, and has the same
         * numbered equator edge within that tetrahedron.  That is, the
         * tetrahedra returned by tetrahedron() must have equal indices within
         * the triangulation, and the edge numbers returned by equatorEdge()
         * must be equal.  In particular, it is still meaningful to compare
         * snapped 3-balls within different triangulations.
         *
         * @param other the snapped 3-ball to compare with this.
         * @return \c true if and only if this and the given object represent
         * different specific presentations of a snapped 3-ball.
         */
        bool operator != (const SnappedBall& other) const;

        /**
         * Determines if the given tetrahedron forms a snapped 3-ball
         * within a triangulation.  The ball need not be the entire
         * triangulation; the boundary triangles may be glued to something
         * else (or to each other).
         *
         * Note that the two boundary triangles of the snapped 3-ball
         * need not be boundary triangles within the overall
         * triangulation, i.e., they may be identified with each other
         * or with triangles of other tetrahedra.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * @param tet the tetrahedron to examine as a potential 3-ball.
         * @return a structure containing details of the snapped 3-ball, or
         * \c null if the given tetrahedron is not a snapped 3-ball.
         */
        static std::unique_ptr<SnappedBall> recognise(Tetrahedron<3>* tet);

        /**
         * A deprecated alias to recognise if a tetrahedron forms a
         * snapped 3-ball.
         *
         * \deprecated This function has been renamed to recognise().
         * See recognise() for details on the parameters and return value.
         */
        [[deprecated]] static std::unique_ptr<SnappedBall> formsSnappedBall(
            Tetrahedron<3>* tet);

        std::unique_ptr<Manifold> manifold() const override;
        AbelianGroup homology() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextShort(std::ostream& out) const override;

    private:
        /**
         * Creates a new structure containing the given internal data.
         */
        SnappedBall(Tetrahedron<3>* tet, int equator);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls SnappedBall::swap(); it is provided
 * so that SnappedBall meets the C++ Swappable requirements.
 *
 * @param a the first structure whose contents should be swapped.
 * @param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(SnappedBall& a, SnappedBall& b) noexcept;

// Inline functions for SnappedBall

inline SnappedBall::SnappedBall(Tetrahedron<3>* tet, int equator) :
        tet_(tet), equator_(equator) {
}

inline SnappedBall* SnappedBall::clone() const {
    return new SnappedBall(*this);
}

inline void SnappedBall::swap(SnappedBall& other) noexcept {
    std::swap(tet_, other.tet_);
    std::swap(equator_, other.equator_);
}

inline Tetrahedron<3>* SnappedBall::tetrahedron() const {
    return tet_;
}
inline int SnappedBall::boundaryFace(int index) const {
    return index == 0 ?
        Edge<3>::edgeVertex[5 - equator_][0] :
        Edge<3>::edgeVertex[5 - equator_][1];
}
inline int SnappedBall::internalFace(int index) const {
    return index == 0 ?
        Edge<3>::edgeVertex[equator_][0] :
        Edge<3>::edgeVertex[equator_][1];
}
inline int SnappedBall::equatorEdge() const {
    return equator_;
}
inline int SnappedBall::internalEdge() const {
    return 5 - equator_;
}

inline bool SnappedBall::operator == (const SnappedBall& other) const {
    return tet_->index() == other.tet_->index() && equator_ == other.equator_;
}
inline bool SnappedBall::operator != (const SnappedBall& other) const {
    return tet_->index() != other.tet_->index() || equator_ != other.equator_;
}

inline std::ostream& SnappedBall::writeName(std::ostream& out) const {
    return out << "Snap";
}
inline std::ostream& SnappedBall::writeTeXName(std::ostream& out) const {
    return out << "\\mathit{Snap}";
}
inline void SnappedBall::writeTextShort(std::ostream& out) const {
    out << "Snapped 3-ball, internal edge "
        << tet_->index() << " ("
        << Edge<3>::edgeVertex[5 - equator_][0]
        << Edge<3>::edgeVertex[5 - equator_][1] << ')';
}

inline std::unique_ptr<SnappedBall> SnappedBall::formsSnappedBall(
        Tetrahedron<3>* tet) {
    return recognise(tet);
}

inline void swap(SnappedBall& a, SnappedBall& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

