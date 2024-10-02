
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file subcomplex/pillowtwosphere.h
 *  \brief Deals with 2-spheres made from two triangles glued along their
 *  three edges.
 */

#ifndef __REGINA_PILLOWTWOSPHERE_H
#ifndef __DOXYGEN
#define __REGINA_PILLOWTWOSPHERE_H
#endif

#include <memory>
#include "regina-core.h"
#include "core/output.h"
#include "maths/perm.h"
#include "triangulation/dim3.h"

namespace regina {

/**
 * Represents a 2-sphere made from two triangles glued together along their
 * three edges.  The two triangles must be distinct and the three edges of
 * each triangle must also be distinct.  Neither of the triangles may be
 * boundary triangles.
 * These two triangless together form an embedded 2-sphere in the triangulation
 * (with the exception that two or three points of the sphere corresponding
 * to the triangles vertices may be identified).
 *
 * This 2-sphere can be cut along and the two resulting 2-sphere
 * boundaries filled in with 3-balls, and the resulting triangulation has
 * the same number of tetrahedra as the original.  If the original
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
class PillowTwoSphere : public ShortOutput<PillowTwoSphere> {
    private:
        Triangle<3>* triangle_[2];
            /**< The two triangles whose edges are joined. */
        Perm<4> triMapping_;
            /**< A mapping from vertices (0,1,2) of the first triangle to
                 vertices (0,1,2) of the second triangle describing how the
                 triangle boundaries are joined. */

    public:
        /**
         * Creates a new copy of the given structure.
         */
        PillowTwoSphere(const PillowTwoSphere&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * \return a reference to this structure.
         */
        PillowTwoSphere& operator = (const PillowTwoSphere&) = default;

        /**
         * Returns one of the two triangles whose boundaries are joined.
         *
         * \param index specifies which of the two triangles to return;
         * this must be either 0 or 1.
         * \return the corresponding triangle.
         */
        Triangle<3>* triangle(int index) const;
        /**
         * Returns a permutation describing how the boundaries of the two
         * triangles are joined.
         *
         * The permutation will map vertices (0,1,2) of
         * `triangle(0)` to vertices (0,1,2) of
         * `triangle(1)`.  The map will represent how the vertices
         * of the triangles are identified by the three edge gluings.
         *
         * \return a permutation describing how the triangle boundaries are
         * joined.
         */
        Perm<4> triangleMapping() const;

        /**
         * Determines whether this and the given object represent the same
         * specific presentation of a pillow 2-sphere.
         *
         * Specifically, two pillow 2-spheres will compare as equal if and
         * only if they join the same two numbered triangles, presented
         * in the same order, using the same mapping of triangle vertices.
         *
         * Since this test looks at triangle numbers and not the specific
         * Triangle objects, it is meaningful to compare pillow 2-spheres
         * within different triangulations.
         *
         * \param other the pillow 2-sphere to compare with this.
         * \return \c true if and only if this and the given object represent
         * the same specific presentation of a pillow 2-sphere.
         */
        bool operator == (const PillowTwoSphere& other) const;

        /**
         * Determines if the two given triangles together form a pillow
         * 2-sphere.
         *
         * Even though PillowTwoSphere is a two-dimensional class and so does
         * not inherit from StandardTriangulation, this routine nevertheless
         * returns by (smart) pointer for consistency with the
         * StandardTriangulation recognition routines (which use pointers
         * because of the polymorphic StandardTriangulation class hierarchy).
         *
         * \pre The two given triangles are distinct.
         *
         * \param tri1 the first triangle to examine.
         * \param tri2 the second triangle to examine.
         * \return a structure containing details of the pillow 2-sphere, or
         * \c null if the given triangles do not form a pillow 2-sphere.
         */
        static std::unique_ptr<PillowTwoSphere> recognise(
            Triangle<3>* tri1, Triangle<3>* tri2);

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
        PillowTwoSphere(Triangle<3>* tri0, Triangle<3>* tri1,
            Perm<4> triMapping);
};

// Inline functions for PillowTwoSphere

inline PillowTwoSphere::PillowTwoSphere(Triangle<3>* tri0, Triangle<3>* tri1,
        Perm<4> triMapping) :
        triangle_ { tri0, tri1 }, triMapping_(triMapping) {
}
inline Triangle<3>* PillowTwoSphere::triangle(int index) const {
    return triangle_[index];
}
inline Perm<4> PillowTwoSphere::triangleMapping() const {
    return triMapping_;
}

inline bool PillowTwoSphere::operator == (const PillowTwoSphere& other) const {
    return
        triangle_[0]->index() == other.triangle_[0]->index() &&
        triangle_[1]->index() == other.triangle_[1]->index() &&
        triMapping_ == other.triMapping_;
}

} // namespace regina

#endif

