
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

/*! \file subcomplex/trivialtri.h
 *  \brief Deals with a few specific hard-coded trivial triangulations.
 */

#ifndef __REGINA_TRIVIALTRI_H
#ifndef __DOXYGEN
#define __REGINA_TRIVIALTRI_H
#endif

#include "regina-core.h"
#include "subcomplex/standardtri.h"

namespace regina {

/**
 * Represents one of a few particular hard-coded trivial triangulations
 * that do not belong to any of the other larger families.
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
class TrivialTri : public StandardTriangulation {
    public:
        /**
         * Represents the two-tetrahedron four-vertex triangulation of
         * the 3-sphere.
         */
        static constexpr int SPHERE_4_VERTEX = 5000;
        /**
         * Represents the one-tetrahedron three-vertex triangulation of
         * the ball.  This is a single tetrahedron with two faces as
         * boundary and the other two faces folded together.
         */
        static constexpr int BALL_3_VERTEX = 5100;
        /**
         * Represents the one-tetrahedron four-vertex triangulation of
         * the ball.  This is a single tetrahedron with all four faces
         * as boundary.
         */
        static constexpr int BALL_4_VERTEX = 5101;
        /**
         * Represents the two-tetrahedron triangulation N(2) of the
         * twisted 2-sphere bundle over the circle.
         */
        static constexpr int N2 = 200;
        /**
         * Represents the three-tetrahedron triangulation N(3,1) of the
         * projective plane bundle over the circle.  This particular
         * triangulation has no Mobius band triangles.
         */
        static constexpr int N3_1 = 301;
        /**
         * Represents the three-tetrahedron triangulation N(3,2) of the
         * projective plane bundle over the circle.  This particular
         * triangulation has two Mobius band triangles.
         */
        static constexpr int N3_2 = 302;

    private:
        int type_;
            /**< The specific triangulation being represented.
                 This must be one of the triangulation constants defined
                 in this class. */

    public:
        /**
         * Creates a new copy of this structure.
         */
        TrivialTri(const TrivialTri&) = default;

        /**
         * Sets this to be a copy of the given structure.
         *
         * @return a reference to this structure.
         */
        TrivialTri& operator = (const TrivialTri&) = default;

        /**
         * Deprecated routine that returns a new copy of this structure.
         *
         * \deprecated Just use the copy constructor instead.
         *
         * @return a newly created clone.
         */
        [[deprecated]] TrivialTri* clone() const;

        /**
         * Swaps the contents of this and the given structure.
         *
         * @param other the structure whose contents should be swapped
         * with this.
         */
        void swap(TrivialTri& other) noexcept;

        /**
         * Returns the specific trivial triangulation being represented.
         *
         * @return the specific triangulation.  This will be one of the
         * triangulation constants defined in this class.
         */
        int type() const;

        /**
         * Determines if the given triangulation component is one of the
         * trivial triangulations recognised by this class.
         *
         * This function returns by (smart) pointer for consistency with
         * StandardTriangulation::recognise(), which makes use of the
         * polymorphic nature of the StandardTriangulation class hierarchy.
         *
         * @param comp the triangulation component to examine.
         * @return a structure representing the trivial triangulation, or
         * \c null if the given component is not one of the triangulations
         * recognised by this class.
         */
        static std::unique_ptr<TrivialTri> recognise(const Component<3>* comp);
        /**
         * A deprecated alias to recognise if a component forms one of
         * the trivial triangulations recognised by this class.
         *
         * \deprecated This function has been renamed to recognise().
         * See recognise() for details on the parameters and return value.
         */
        [[deprecated]] static std::unique_ptr<TrivialTri>
            isTrivialTriangulation(const Component<3>* comp);

        std::unique_ptr<Manifold> manifold() const override;
        AbelianGroup homology() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
        void writeTextLong(std::ostream& out) const override;

    private:
        /**
         * Creates a new structure.
         */
        TrivialTri(int type);
};

/**
 * Swaps the contents of the two given structures.
 *
 * This global routine simply calls TrivialTri::swap(); it is provided
 * so that TrivialTri meets the C++ Swappable requirements.
 *
 * @param a the first structure whose contents should be swapped.
 * @param b the second structure whose contents should be swapped.
 *
 * \ingroup subcomplex
 */
void swap(TrivialTri& a, TrivialTri& b) noexcept;

// Inline functions for TrivialTri

inline TrivialTri::TrivialTri(int type) : type_(type) {
}

inline TrivialTri* TrivialTri::clone() const {
    return new TrivialTri(*this);
}

inline void TrivialTri::swap(TrivialTri& other) noexcept {
    std::swap(type_, other.type_);
}

inline int TrivialTri::type() const {
    return type_;
}

inline std::unique_ptr<TrivialTri> TrivialTri::isTrivialTriangulation(
        const Component<3>* comp) {
    return recognise(comp);
}

inline void swap(TrivialTri& a, TrivialTri& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

