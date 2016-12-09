
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

/*! \file subcomplex/l31pillow.h
 *  \brief Deals with triangular pillow L(3,1) components of a triangulation.
 */

#ifndef __L31PILLOW_H
#ifndef __DOXYGEN
#define __L31PILLOW_H
#endif

#include "regina-core.h"
#include "subcomplex/standardtri.h"

namespace regina {

/**
 * \weakgroup subcomplex
 * @{
 */

/**
 * Represents a triangular pillow L(3,1) component of a triangulation.
 *
 * A triangular pillow L(3,1) is a two-tetrahedron two-vertex
 * triangulation of the lens space L(3,1) formed as follows.
 *
 * A triangular pillow is formed from two tetrahedra with a single
 * degree three vertex in the interior of the pillow.  The two boundary
 * triangles of this pillow are then identified with a one-third twist.
 *
 * All optional StandardTriangulation routines are implemented for this
 * class.
 */
class REGINA_API L31Pillow : public StandardTriangulation {
    private:
        Tetrahedron<3>* tet[2];
            /**< The two tetrahedra in the triangular pillow. */
        unsigned interior[2];
            /**< The vertex of each tetrahedron that corresponds to the
                 interior vertex of the triangular pillow. */

    public:
        /**
         * Destroys this structure.
         */
        virtual ~L31Pillow();
        /**
         * Returns a newly created clone of this structure.
         *
         * @return a newly created clone.
         */
        L31Pillow* clone() const;

        /**
         * Returns one of the two tetrahedra involved in this structure.
         *
         * @param whichTet specifies which tetrahedron to return; this
         * must be either 0 or 1.
         * @return the requested tetrahedron.
         */
        Tetrahedron<3>* tetrahedron(int whichTet) const;
        /**
         * Returns the vertex number of the given tetrahedron
         * corresponding to the degree three vertex in the interior of
         * the triangular pillow.  See the general class notes for
         * further details.
         *
         * The specific tetrahedron to examine is determined by the
         * argument \a whichTet; this will be the tetrahedron
         * <tt>tetrahedron(whichTet)</tt>.
         *
         * @param whichTet specifies which tetrahedron to examine;
         * this must be either 0 or 1.
         * @return the vertex of tetrahedron \a whichTet corresponding
         * to the vertex in the interior of the triangular pillow; this
         * will be between 0 and 3 inclusive.
         */
        unsigned interiorVertex(int whichTet) const;

        /**
         * Determines if the given triangulation component is a
         * triangular pillow L(3,1).
         *
         * @param comp the triangulation component to examine.
         * @return a newly created structure containing details of the
         * triangular pillow L(3,1), or \c null if the given component is
         * not a triangular pillow L(3,1).
         */
        static L31Pillow* isL31Pillow(const Component<3>* comp);

        Manifold* manifold() const;
        AbelianGroup* homology() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Creates a new uninitialised structure.
         */
        L31Pillow();
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NL31Pillow has now been renamed to
 * L31Pillow.
 */
REGINA_DEPRECATED typedef L31Pillow NL31Pillow;

/*@}*/

// Inline functions for L31Pillow

inline L31Pillow::L31Pillow() {
}
inline L31Pillow::~L31Pillow() {
}

inline Tetrahedron<3>* L31Pillow::tetrahedron(int whichTet) const {
    return tet[whichTet];
}
inline unsigned L31Pillow::interiorVertex(int whichTet) const {
    return interior[whichTet];
}
inline std::ostream& L31Pillow::writeName(std::ostream& out) const {
    return out << "L'(3,1)";
}
inline std::ostream& L31Pillow::writeTeXName(std::ostream& out) const {
    return out << "L'_{3,1}";
}
inline void L31Pillow::writeTextLong(std::ostream& out) const {
    out << "Triangular pillow lens space L(3,1)";
}

} // namespace regina

#endif

