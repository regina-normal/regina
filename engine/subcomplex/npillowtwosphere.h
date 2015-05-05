
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file subcomplex/npillowtwosphere.h
 *  \brief Deals with 2-spheres made from two triangles glued along their
 *  three edges.
 */

#ifndef __NPILLOWTWOSPHERE_H
#ifndef __DOXYGEN
#define __NPILLOWTWOSPHERE_H
#endif

#include "regina-core.h"
#include "shareableobject.h"
#include "maths/nperm4.h"

namespace regina {

class NTriangle;

template <int> class Triangulation;
typedef Triangulation<3> NTriangulation;

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
class REGINA_API NPillowTwoSphere : public ShareableObject {
    private:
        NTriangle* triangle[2];
            /**< The two triangles whose edges are joined. */
        NPerm4 triMapping;
            /**< A mapping from vertices (0,1,2) of the first triangle to
                 vertices (0,1,2) of the second triangle describing how the
                 triangle boundaries are joined. */

    public:
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        NPillowTwoSphere* clone() const;

        /**
         * Returns one of the two triangles whose boundaries are joined.
         *
         * @param index specifies which of the two triangles to return;
         * this must be either 0 or 1.
         * @return the corresponding triangle.
         */
        NTriangle* getTriangle(int index) const;
        /**
         * A deprecated alias for getTriangle().
         *
         * This routine returns one of the two triangles whose boundaries
         * are joined.  See getTriangle() for further details.
         *
         * \deprecated This routine will be removed in a future version
         * of Regina.  Please use getTriangle() instead.
         *
         * @param index specifies which of the two triangles to return;
         * this must be either 0 or 1.
         * @return the corresponding triangle.
         */
        NTriangle* getFace(int index) const;
        /**
         * Returns a permutation describing how the boundaries of the two
         * triangles are joined.
         *
         * The permutation will map vertices (0,1,2) of
         * <tt>getTriangle(0)</tt> to vertices (0,1,2) of
         * <tt>getTriangle(1)</tt>.  The map will represent how the vertices
         * of the triangles are identified by the three edge gluings.
         *
         * @return a permutation describing how the triangle boundaries are
         * joined.
         */
        NPerm4 getTriangleMapping() const;
        /**
         * A deprecated alias for getTriangleMapping().
         *
         * This routine returns a permutation describing how the boundaries
         * of the two triangles are joined.  See getTriangleMapping()
         * for further details.
         *
         * \deprecated This routine will be removed in a future version
         * of Regina.  Please use getTriangleMapping() instead.
         *
         * @return a permutation describing how the triangle boundaries are
         * joined.
         */
        NPerm4 getFaceMapping() const;

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
        static NPillowTwoSphere* formsPillowTwoSphere(NTriangle* tri1,
            NTriangle* tri2);

        void writeTextShort(std::ostream& out) const;

    private:
        /**
         * Creates a new uninitialised structure.
         */
        NPillowTwoSphere();
};

/*@}*/

// Inline functions for NPillowTwoSphere

inline NPillowTwoSphere::NPillowTwoSphere() {
}
inline NTriangle* NPillowTwoSphere::getTriangle(int index) const {
    return triangle[index];
}
inline NTriangle* NPillowTwoSphere::getFace(int index) const {
    return triangle[index];
}
inline NPerm4 NPillowTwoSphere::getTriangleMapping() const {
    return triMapping;
}
inline NPerm4 NPillowTwoSphere::getFaceMapping() const {
    return triMapping;
}
inline void NPillowTwoSphere::writeTextShort(std::ostream& out) const {
    out << "Pillow 2-sphere";
}

} // namespace regina

#endif

