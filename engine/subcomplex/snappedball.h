
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

/*! \file subcomplex/snappedball.h
 *  \brief Deals with snapped 3-balls in a triangulation.
 */

#ifndef __SNAPPEDBALL_H
#ifndef __DOXYGEN
#define __SNAPPEDBALL_H
#endif

#include "regina-core.h"
#include "subcomplex/standardtri.h"
#include "triangulation/dim3.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a snapped 3-ball in a triangulation.
 * A snapped 3-ball is a single tetrahedron with two faces glued to each
 * other to form a 3-ball with a two triangle boundary.
 *
 * All optional StandardTriangulation routines are implemented for this
 * class.
 */
class REGINA_API SnappedBall : public StandardTriangulation {
    private:
        Tetrahedron<3>* tet;
            /**< The tetrahedron that forms the snapped ball. */
        int equator;
            /**< The edge that forms the equator on the ball boundary. */

    public:
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        SnappedBall* clone() const;

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
         * @param tet the tetrahedron to examine as a potential 3-ball.
         * @return a newly created structure containing details of the
         * snapped 3-ball, or \c null if the given tetrahedron is
         * not a snapped 3-ball.
         */
        static SnappedBall* formsSnappedBall(Tetrahedron<3>* tet);

        Manifold* manifold() const;
        AbelianGroup* homology() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new uninitialised structure.
         */
        SnappedBall();
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NSnappedBall has now been renamed to
 * SnappedBall.
 */
REGINA_DEPRECATED typedef SnappedBall NSnappedBall;

/*@}*/

// Inline functions for SnappedBall

inline SnappedBall::SnappedBall() {
}
inline Tetrahedron<3>* SnappedBall::tetrahedron() const {
    return tet;
}
inline int SnappedBall::boundaryFace(int index) const {
    return index == 0 ?
        Edge<3>::edgeVertex[5 - equator][0] :
        Edge<3>::edgeVertex[5 - equator][1];
}
inline int SnappedBall::internalFace(int index) const {
    return index == 0 ?
        Edge<3>::edgeVertex[equator][0] :
        Edge<3>::edgeVertex[equator][1];
}
inline int SnappedBall::equatorEdge() const {
    return equator;
}
inline int SnappedBall::internalEdge() const {
    return 5 - equator;
}
inline std::ostream& SnappedBall::writeName(std::ostream& out) const {
    return out << "Snap";
}
inline std::ostream& SnappedBall::writeTeXName(std::ostream& out) const {
    return out << "\\mathit{Snap}";
}
inline void SnappedBall::writeTextLong(std::ostream& out) const {
    out << "Snapped 3-ball";
}

} // namespace regina

#endif

