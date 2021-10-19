
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

/*! \file subcomplex/l31pillow.h
 *  \brief Deals with triangular pillow L(3,1) components of a triangulation.
 */

#ifndef __REGINA_L31PILLOW_H
#ifndef __DOXYGEN
#define __REGINA_L31PILLOW_H
#endif

#include "regina-core.h"
#include "subcomplex/standardtri.h"

namespace regina {

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
 * All optional StandardTriangulation routines are implemented for this class.
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
class L31Pillow : public StandardTriangulation {
    private:
        Tetrahedron<3>* tet_[2];
            /**< The two tetrahedra in the triangular pillow. */
        unsigned interior_[2];
            /**< The vertex of each tetrahedron that corresponds to the
                 interior vertex of the triangular pillow. */

    public:
        /**
         * Creates a new copy of this structure.
         */
        L31Pillow(const L31Pillow&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * @return a reference to this structure.
         */
        L31Pillow& operator = (const L31Pillow&) = default;

        /**
         * Deprecated routine that returns a new copy of this structure.
         *
         * \deprecated Just use the copy constructor instead.
         *
         * @return a newly created clone.
         */
        [[deprecated]] L31Pillow* clone() const;

        /**
         * Swaps the contents of this and the given structure.
         *
         * @param other the structure whose contents should be swapped
         * with this.
         */
        void swap(L31Pillow& other) noexcept;

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
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * @param comp the triangulation component to examine.
         * @return a structure containing details of the triangular pillow
         * L(3,1), or \c null if the given component is not a
         * triangular pillow L(3,1).
         */
        static std::unique_ptr<L31Pillow> recognise(const Component<3>* comp);
        /**
         * A deprecated alias to recognise if a component forms a
         * triangular pillow L(3,1).
         *
         * \deprecated This function has been renamed to recognise().
         * See recognise() for details on the parameters and return value.
         */
        [[deprecated]] static std::unique_ptr<L31Pillow> isL31Pillow(
            const Component<3>* comp);

        std::unique_ptr<Manifold> manifold() const override;
        AbelianGroup homology() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

    private:
        /**
         * Creates a new uninitialised structure.
         */
        L31Pillow() = default;
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls L31Pillow::swap(); it is provided
 * so that L31Pillow meets the C++ Swappable requirements.
 *
 * @param a the first structure whose contents should be swapped.
 * @param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(L31Pillow& a, L31Pillow& b) noexcept;

// Inline functions for L31Pillow

inline L31Pillow* L31Pillow::clone() const {
    return new L31Pillow(*this);
}

inline void L31Pillow::swap(L31Pillow& other) noexcept {
    std::swap(tet_[0], other.tet_[0]);
    std::swap(tet_[1], other.tet_[1]);
    std::swap(interior_[0], other.interior_[0]);
    std::swap(interior_[1], other.interior_[1]);
}

inline Tetrahedron<3>* L31Pillow::tetrahedron(int whichTet) const {
    return tet_[whichTet];
}
inline unsigned L31Pillow::interiorVertex(int whichTet) const {
    return interior_[whichTet];
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

inline std::unique_ptr<L31Pillow> L31Pillow::isL31Pillow(
        const Component<3>* comp) {
    return recognise(comp);
}

inline void swap(L31Pillow& a, L31Pillow& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

