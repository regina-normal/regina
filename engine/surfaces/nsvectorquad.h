
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

/*! \file surfaces/nsvectorquad.h
 *  \brief Implements normal surface vectors using quad coordinates.
 */

#ifndef __REGINA_NSVECTORQUAD_H
#ifndef __DOXYGEN
#define __REGINA_NSVECTORQUAD_H
#endif

#include "regina-core.h"
#include "enumerate/enumconstraints.h"
#include "surfaces/nsvectormirrored.h"

namespace regina {

class NSVectorQuad;

/**
 * \weakgroup surfaces
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct NormalInfo<NS_QUAD> {
    typedef NSVectorQuad Class;
    typedef NormalInfo<NS_STANDARD> Standard;
    typedef NormalInfo<NS_QUAD> Reduced;
    static constexpr const char* name = "Quad normal";
    static constexpr bool almostNormal = false;
    static constexpr bool spun = true;
    static constexpr bool oriented = false;
    static constexpr size_t dimension(size_t n) { return 3 * n; }
};
#endif

/**
 * A normal surface vector using quad coordinates.
 *
 * If there are \a t tetrahedra in the underlying
 * triangulation, there must be precisely 3<i>t</i> coordinates.
 * The first three coordinates will be for the first tetrahedron, the
 * next three for the second tetrahedron and so on.  For each
 * tetrahedron, the three individual coordinates represent the
 * number of quadrilateral discs of type 0, 1 and 2
 * (see NormalSurface::quads()).
 *
 * \ifacespython Not present.
 */
class NSVectorQuad : public NSVectorMirrored {
    REGINA_NORMAL_SURFACE_FLAVOUR(NSVectorQuad, NS_QUAD, NSVectorMirrored);

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NSVectorQuad(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        template <typename T>
        NSVectorQuad(const Vector<T>& cloneMe);

        virtual NormalSurfaceVector* makeMirror(const Triangulation<3>& triang)
            const override;

        virtual const Vertex<3>* isVertexLink(const Triangulation<3>& triang)
            const override;

        virtual LargeInteger octs(size_t tetIndex,
            int octType, const Triangulation<3>& triang) const override;

        static std::optional<MatrixInt> makeMatchingEquations(
            const Triangulation<3>& triangulation);
        static EnumConstraints makeEmbeddedConstraints(
            const Triangulation<3>& triangulation);
};

/*@}*/

// Inline functions for NSVectorQuad

inline NSVectorQuad::NSVectorQuad(
        size_t length) : NSVectorMirrored(length) {
}
template <typename T>
inline NSVectorQuad::NSVectorQuad(const Vector<T>& cloneMe) :
        NSVectorMirrored(cloneMe) {
}

inline const Vertex<3>* NSVectorQuad::isVertexLink(
        const Triangulation<3>&) const {
    // Quad space does not contain vertex links at all.
    return 0;
}

inline LargeInteger NSVectorQuad::octs(
        size_t, int, const Triangulation<3>&) const {
    return LargeInteger::zero;
}

} // namespace regina

#endif

