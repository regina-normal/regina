
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

/*! \file subcomplex/pillowtwosphere.h
 *  \brief Deals with 2-spheres made from two triangles glued along their
 *  three edges.
 */

#ifndef __PILLOWTWOSPHERE_H
#ifndef __DOXYGEN
#define __PILLOWTWOSPHERE_H
#endif

#include "regina-core.h"
#include "output.h"
#include "maths/perm.h"
#include "triangulation/forward.h"
#include <boost/noncopyable.hpp>

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

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
 */
class REGINA_API PillowTwoSphere :
        public ShortOutput<PillowTwoSphere>,
        public boost::noncopyable {
    private:
        Triangle<3>* triangle_[2];
            /**< The two triangles whose edges are joined. */
        Perm<4> triMapping_;
            /**< A mapping from vertices (0,1,2) of the first triangle to
                 vertices (0,1,2) of the second triangle describing how the
                 triangle boundaries are joined. */

    public:
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        PillowTwoSphere* clone() const;

        /**
         * Returns one of the two triangles whose boundaries are joined.
         *
         * @param index specifies which of the two triangles to return;
         * this must be either 0 or 1.
         * @return the corresponding triangle.
         */
        Triangle<3>* triangle(int index) const;
        /**
         * Returns a permutation describing how the boundaries of the two
         * triangles are joined.
         *
         * The permutation will map vertices (0,1,2) of
         * <tt>triangle(0)</tt> to vertices (0,1,2) of
         * <tt>triangle(1)</tt>.  The map will represent how the vertices
         * of the triangles are identified by the three edge gluings.
         *
         * @return a permutation describing how the triangle boundaries are
         * joined.
         */
        Perm<4> triangleMapping() const;

        /**
         * Determines if the two given triangles together form a pillow
         * 2-sphere.
         *
         * \pre The two given triangles are distinct.
         *
         * @param tri1 the first triangle to examine.
         * @param tri2 the second triangle to examine.
         * @return a newly created structure containing details of the
         * pillow 2-sphere, or \c null if the given triangles do not
         * form a pillow 2-sphere.
         */
        static PillowTwoSphere* formsPillowTwoSphere(Triangle<3>* tri1,
            Triangle<3>* tri2);

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new uninitialised structure.
         */
        PillowTwoSphere();
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NPillowTwoSphere has now been renamed to
 * PillowTwoSphere.
 */
REGINA_DEPRECATED typedef PillowTwoSphere NPillowTwoSphere;

/*@}*/

// Inline functions for PillowTwoSphere

inline PillowTwoSphere::PillowTwoSphere() {
}
inline Triangle<3>* PillowTwoSphere::triangle(int index) const {
    return triangle_[index];
}
inline Perm<4> PillowTwoSphere::triangleMapping() const {
    return triMapping_;
}
inline void PillowTwoSphere::writeTextShort(std::ostream& out) const {
    out << "Pillow 2-sphere";
}

} // namespace regina

#endif

