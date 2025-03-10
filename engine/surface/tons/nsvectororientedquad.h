
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/*! \file surfaces/nsvectororientedquad.h
 *  \brief Implements normal surface vectors using
 *  transversely oriented quad coordinates.
 */

#ifndef __REGINA_NSVECTORORIENTEDQUAD_H
#ifndef __DOXYGEN
#define __REGINA_NSVECTORORIENTEDQUAD_H
#endif
#include "regina-core.h"
#include "enumerate/enumconstraints.h"
#include "surfaces/nsvectormirrored.h"

namespace regina {

class NSVectorOrientedQuad;

/**
 * \weakgroup surfaces
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct NormalInfo<NS_ORIENTED_QUAD> {
    typedef NSVectorOrientedQuad Class;
    typedef NormalInfo<NS_ORIENTED> Standard;
    typedef NormalInfo<NS_ORIENTED_QUAD> Reduced;
    static constexpr const char* name = "Transversely oriented quad normal";
    static constexpr bool almostNormal = false;
    static constexpr bool spun = true;
    static constexpr bool oriented = true;
    static constexpr size_t dimension(size_t n) { return 6 * n; }
};
#endif

/**
 * A normal surface vector using transversely oriented quadrilateral
 * coordinates.
 *
 * If there are \a t tetrahedra in the underlying
 * triangulation, there must be precisely 6<i>t</i> coordinates.
 * For each \a i, coordinates 2<i>i</i> and 2<i>i</i>+1 represent
 * the \c true and \c false orientations for coordinate \a i in the
 * 3<i>t</i>-dimensional quadrilateral coordinate system.  See
 * NSVectorQuad for further details.
 *
 * \warning Support for transversely oriented normal surfaces is still
 * experimental, and some features \b will break (e.g., testing
 * connectedness, disjointness or embeddedness).
 *
 * \ifacespython Not present.
 */
class NSVectorOrientedQuad : public NSVectorMirrored {
    REGINA_NORMAL_SURFACE_FLAVOUR(NSVectorOrientedQuad, NS_ORIENTED_QUAD,
        NSVectorMirrored);

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NSVectorOrientedQuad(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        template <typename T>
        NSVectorOrientedQuad(const Vector<T>& cloneMe);

        virtual NormalSurfaceVector* makeMirror(const Triangulation<3>& triang)
            const override;

        virtual const Vertex<3>* isVertexLink(const Triangulation<3>& triang)
            const override;

        virtual LargeInteger quads(size_t tetIndex,
            int quadType, const Triangulation<3>& triang) const override;
        virtual LargeInteger orientedQuads(size_t tetIndex,
            int quadType, const Triangulation<3>& triang, bool orientation)
            const override;
        virtual LargeInteger octs(size_t tetIndex,
            int octType, const Triangulation<3>& triang) const override;

        static std::optional<MatrixInt> makeMatchingEquations(
            const Triangulation<3>& triangulation);
        static EnumConstraints makeEmbeddedConstraints(
            const Triangulation<3>& triangulation);
};

/*@}*/

// Inline functions for NSVectorOrientedQuad

inline NSVectorOrientedQuad::NSVectorOrientedQuad(
        size_t length) : NSVectorMirrored(length) {
}
template <typename T>
inline NSVectorOrientedQuad::NSVectorOrientedQuad(const Vector<T>& cloneMe) :
        NSVectorMirrored(cloneMe) {
}

inline LargeInteger NSVectorOrientedQuad::quads(
        size_t tetIndex, int quadType, const Triangulation<3>& tri) const {
    return orientedQuads(tetIndex, quadType, tri, true)
           + orientedQuads(tetIndex, quadType, tri, false);
}

inline LargeInteger NSVectorOrientedQuad::orientedQuads(
        size_t tetIndex, int quadType, const Triangulation<3>&,
        bool orientation) const {
    return coords_[6 * tetIndex + 2 * quadType +
        (orientation ? 0 : 1)];
}

inline const Vertex<3>* NSVectorOrientedQuad::isVertexLink(
        const Triangulation<3>&) const {
    // Quad space does not contain vertex links at all.
    return 0;
}

inline LargeInteger NSVectorOrientedQuad::octs(
        size_t, int, const Triangulation<3>&) const {
    return LargeInteger::zero;
}

} // namespace regina

#endif

