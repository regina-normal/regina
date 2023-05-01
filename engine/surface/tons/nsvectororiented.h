
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

/*! \file surfaces/nsvectororiented.h
 *  \brief Implements normal surface vectors using transversly oriented 
 *  normal surface coordinates.*/

#ifndef __REGINA_NSVECTORORIENTED_H
#ifndef __DOXYGEN
#define __REGINA_NSVECTORORIENTED_H
#endif


#include "regina-core.h"
#include "enumerate/enumconstraints.h"
#include "surfaces/normalsurface.h"

namespace regina {
class NSVectorOriented;

/**
 * \weakgroup surfaces
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct NormalInfo<NS_ORIENTED> {
    typedef NSVectorOriented Class;
    typedef NormalInfo<NS_ORIENTED> Standard;
    typedef NormalInfo<NS_ORIENTED_QUAD> Reduced;
    static constexpr const char* name = "Transversely oriented standard normal";
    static constexpr bool almostNormal = false;
    static constexpr bool spun = false;
    static constexpr bool oriented = true;
    static constexpr size_t dimension(size_t n) { return 14 * n; }
};
#endif

/**
 * A normal surface vector using transversely oriented standard
 * (triangle-quad) coordinates.
 *
 * If there are \a t tetrahedra in the underlying
 * triangulation, there must be precisely 14<i>t</i> coordinates.
 * For each \a i, coordinates 2<i>i</i> and 2<i>i</i>+1 represent
 * the \c true and \c false orientations for coordinate \a i in the
 * 7<i>t</i>-dimensional standard coordinate system.  See
 * NSVectorStandard for further details.
 *
 * \warning Support for transversely oriented normal surfaces is still
 * experimental, and some features \b will break (e.g., testing
 * connectedness, disjointness or embeddedness).
 *
 * \ifacespython Not present.
 */
class NSVectorOriented : public NormalSurfaceVector {
    REGINA_NORMAL_SURFACE_FLAVOUR(NSVectorOriented, NS_ORIENTED,
        NormalSurfaceVector);

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NSVectorOriented(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        template <typename T>
        NSVectorOriented(const Vector<T>& cloneMe);

        virtual LargeInteger triangles(size_t tetIndex,
            int vertex, const Triangulation<3>& triang) const override;
        virtual LargeInteger quads(size_t tetIndex,
            int quadType, const Triangulation<3>& triang) const override;
        virtual LargeInteger orientedTriangles(size_t tetIndex,
            int vertex, const Triangulation<3>& triang, bool orientation) const
            override;
        virtual LargeInteger orientedQuads(size_t tetIndex,
            int quadType, const Triangulation<3>& triang, bool orientation)
            const override;
        virtual LargeInteger octs(size_t tetIndex,
            int octType, const Triangulation<3>& triang) const override;
        virtual LargeInteger edgeWeight(size_t edgeIndex,
            const Triangulation<3>& triang) const override;
        virtual LargeInteger arcs(size_t triIndex,
            int triVertex, const Triangulation<3>& triang) const override;

        static std::optional<MatrixInt> makeMatchingEquations(
            const Triangulation<3>& triangulation);
        static EnumConstraints makeEmbeddedConstraints(
            const Triangulation<3>& triangulation);
};

/*@}*/

// Inline functions for NSVectorOriented

inline NSVectorOriented::NSVectorOriented(
        size_t length) : NormalSurfaceVector(length) {
}
template <typename T>
inline NSVectorOriented::NSVectorOriented(const Vector<T>& cloneMe) :
        NormalSurfaceVector(cloneMe) {
}

inline LargeInteger NSVectorOriented::triangles(
        size_t tetIndex, int vertex, const Triangulation<3>& tri) const {
    return orientedTriangles(tetIndex,vertex,tri, true)
           + orientedTriangles(tetIndex,vertex,tri, false);
}
inline LargeInteger NSVectorOriented::quads(
        size_t tetIndex, int quadType, const Triangulation<3>& tri) const {
    return orientedQuads(tetIndex,quadType,tri, true)
           + orientedQuads(tetIndex,quadType,tri, false);
}

inline LargeInteger NSVectorOriented::orientedTriangles(
        size_t tetIndex, int vertex, const Triangulation<3>&,
        bool orientation) const {
    return coords_[14 * tetIndex + 2 * vertex +
        (orientation ? 0 : 1)];
}
inline LargeInteger NSVectorOriented::orientedQuads(
        size_t tetIndex, int quadType, const Triangulation<3>&,
        bool orientation) const {
    return coords_[14 * tetIndex + 8 + 2 * quadType +
        (orientation ? 0 : 1)];
}
inline LargeInteger NSVectorOriented::octs(
        size_t, int, const Triangulation<3>&) const {
    return LargeInteger::zero;
}

} // namespace regina

#endif

