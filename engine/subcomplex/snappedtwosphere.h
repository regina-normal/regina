
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

/*! \file subcomplex/snappedtwosphere.h
 *  \brief Deals with 2-spheres made from two snapped 3-balls in a
 *  triangulation.
 */

#ifndef __REGINA_SNAPPEDTWOSPHERE_H
#ifndef __DOXYGEN
#define __REGINA_SNAPPEDTWOSPHERE_H
#endif

#include "regina-core.h"
#include "core/output.h"
#include "subcomplex/snappedball.h"

namespace regina {

class SnappedBall;

/**
 * Represents a 2-sphere made from two snapped 3-balls in a triangulation.
 * This occurs when two snapped 3-balls are glued together at their
 * equators (note that this gluing does not have to extend to triangular faces).
 * Each 3-ball has a central disc (bounded by the 3-ball's equator and bisecting
 * its internal degree one edge), and these two discs together form an embedded
 * 2-sphere in the triangulation.
 *
 * This 2-sphere can be cut along and the two resulting 2-sphere
 * boundaries filled in with 3-balls, and the resulting triangulation has
 * the same number of tetrahedra as the original.  If the snapped
 * 2-sphere was separating, the resulting triangulation will contain the
 * two terms of the corresponding connected sum.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 * However, the only way to create them (aside from copying or moving)
 * is via the static member function recognise().
 *
 * \ingroup subcomplex
 */
class SnappedTwoSphere : public ShortOutput<SnappedTwoSphere> {
    private:
        SnappedBall ball_[2];
            /**< The two snapped 3-balls whose equators are joined. */

    public:
        /**
         * Creates a new copy of the given structure.
         */
        SnappedTwoSphere(const SnappedTwoSphere&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * \return a reference to this structure.
         */
        SnappedTwoSphere& operator = (const SnappedTwoSphere&) = default;

        /**
         * Returns one of the two snapped 3-balls whose equators are
         * joined.
         *
         * \param index specifies which of the two 3-balls to return;
         * this must be either 0 or 1.
         * \return the corresponding snapped 3-ball.
         */
        const SnappedBall& snappedBall(int index) const;

        /**
         * Determines whether this and the given object represent the same
         * specific presentation of a snapped 2-sphere.
         *
         * Specifically, two snapped 2-spheres will compare as equal if and
         * only if they slice through the same pair of numbered tetrahedra,
         * presented in the same order, and bisecting the same numbered
         * degree one edges within corresponding tetrahedra.
         *
         * Since this test looks at tetrahedron numbers and not the specific
         * Tetrahedron objects, it is meaningful to compare snapped 2-spheres
         * within different triangulations.
         *
         * \param other the snapped 2-sphere to compare with this.
         * \return \c true if and only if this and the given object represent
         * the same specific presentation of a snapped 2-sphere.
         */
        bool operator == (const SnappedTwoSphere& other) const;

        /**
         * Determines if the two given tetrahedra together form a snapped
         * 2-sphere.
         *
         * Even though SnappedTwoSphere is a two-dimensional class and so does
         * not inherit from StandardTriangulation, this routine nevertheless
         * returns by (smart) pointer for consistency with the
         * StandardTriangulation recognition routines (which use pointers
         * because of the polymorphic StandardTriangulation class hierarchy).
         *
         * \pre The two given tetrahedra are distinct.
         *
         * \param tet1 the first tetrahedron to examine.
         * \param tet2 the second tetrahedron to examine.
         * \return a structure containing details of the snapped 2-sphere, or
         * \c null if the given tetrahedra do not form a snapped 2-sphere.
         */
        static std::unique_ptr<SnappedTwoSphere> recognise(Tetrahedron<3>* tet1,
            Tetrahedron<3>* tet2);
        /**
         * Determines if the two given snapped 3-balls together form a snapped
         * 2-sphere.
         *
         * If this is the case, the snapped 3-balls stored in
         * the structure returned will be clones of the
         * original 3-balls, not the original 3-balls themselves.
         *
         * Even though SnappedTwoSphere is a two-dimensional class and so does
         * not inherit from StandardTriangulation, this routine nevertheless
         * returns by (smart) pointer for consistency with the
         * StandardTriangulation recognition routines (which use pointers
         * because of the polymorphic StandardTriangulation class hierarchy).
         *
         * \pre The two given snapped 3-balls use distinct tetrahedra.
         *
         * \param ball1 the first snapped 3-ball to examine.
         * \param ball2 the second snapped 3-ball to examine.
         * \return a structure containing details of the snapped 2-sphere, or
         * \c null if the given snapped 3-balls do not form a snapped 2-sphere.
         */
        static std::unique_ptr<SnappedTwoSphere> recognise(
            const SnappedBall& ball1, const SnappedBall& ball2);

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new structure containing the given internal data.
         */
        SnappedTwoSphere(const SnappedBall& ball0, const SnappedBall& ball1);
};

// Inline functions for SnappedTwoSphere

inline SnappedTwoSphere::SnappedTwoSphere(
        const SnappedBall& ball0, const SnappedBall& ball1) :
        ball_ { ball0, ball1 } {
}
inline const SnappedBall& SnappedTwoSphere::snappedBall(int index) const {
    return ball_[index];
}

inline bool SnappedTwoSphere::operator == (const SnappedTwoSphere& other)
        const {
    return ball_[0] == other.ball_[0] && ball_[1] == other.ball_[1];
}

} // namespace regina

#endif

