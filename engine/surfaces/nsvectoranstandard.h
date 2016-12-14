
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

/*! \file surfaces/nsvectoranstandard.h
 *  \brief Implements almost normal surface vectors using standard
 *  triangle-quad-oct coordinates.
 */

#ifndef __NSVECTORANSTANDARD_H
#ifndef __DOXYGEN
#define __NSVECTORANSTANDARD_H
#endif

#include "regina-core.h"
#include "surfaces/normalsurface.h"

namespace regina {

class NSVectorANStandard;

/**
 * \weakgroup surfaces
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct NormalInfo<NS_AN_STANDARD> {
    typedef NSVectorANStandard Class;
    typedef NormalInfo<NS_AN_STANDARD> Standard;
    typedef NormalInfo<NS_AN_QUAD_OCT> Reduced;
    inline static const char* name() {
        return "Standard almost normal (tri-quad-oct)";
    }
    enum {
        almostNormal = 1,
        spun = 0,
        oriented = 0
    };
};
#endif

/**
 * An almost normal surface vector using standard triangle-quad-oct
 * coordinates.
 *
 * If there are \a t tetrahedra in the underlying
 * triangulation, there must be precisely 10<i>t</i> coordinates.
 * The first ten coordinates will be for the first tetrahedron, the
 * next ten for the second tetrahedron and so on.  For each
 * tetrahedron, the first four represent the number of
 * triangular discs about vertex 0, 1, 2 and 3, the next
 * three represent the number of quadrilateral discs of type 0,
 * 1 and 2 (see NormalSurface::quads()) and the final three
 * represent the number of octagonal discs of type 0, 1 and 2 (see
 * NormalSurface::octs()).
 *
 * \ifacespython Not present.
 */
class REGINA_API NSVectorANStandard : public NormalSurfaceVector {
    REGINA_NORMAL_SURFACE_FLAVOUR(NSVectorANStandard, NS_AN_STANDARD,
        NormalSurfaceVector);

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NSVectorANStandard(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NSVectorANStandard(const Vector<LargeInteger>& cloneMe);

        virtual LargeInteger triangles(size_t tetIndex,
            int vertex, const Triangulation<3>* triang) const;
        virtual LargeInteger quads(size_t tetIndex,
            int quadType, const Triangulation<3>* triang) const;
        virtual LargeInteger octs(size_t tetIndex,
            int octType, const Triangulation<3>* triang) const;
        virtual LargeInteger edgeWeight(size_t edgeIndex,
            const Triangulation<3>* triang) const;
        virtual LargeInteger arcs(size_t triIndex,
            int triVertex, const Triangulation<3>* triang) const;

        static NormalSurfaceVector* makeZeroVector(
            const Triangulation<3>* triangulation);
        static MatrixInt* makeMatchingEquations(
            const Triangulation<3>* triangulation);
        static EnumConstraints* makeEmbeddedConstraints(
            const Triangulation<3>* triangulation);
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NNormalSurfaceVectorStandard has now been
 * renamed to NSVectorStandard.
 */
REGINA_DEPRECATED typedef NSVectorANStandard NNormalSurfaceVectorANStandard;

/*@}*/

// Inline functions for NSVectorANStandard

inline NSVectorANStandard::NSVectorANStandard(
        size_t length) : NormalSurfaceVector(length) {
}
inline NSVectorANStandard::NSVectorANStandard(
        const Vector<LargeInteger>& cloneMe) :
        NormalSurfaceVector(cloneMe) {
}

inline LargeInteger NSVectorANStandard::triangles(
        size_t tetIndex, int vertex, const Triangulation<3>*) const {
    return coords_[10 * tetIndex + vertex];
}
inline LargeInteger NSVectorANStandard::quads(
        size_t tetIndex, int quadType, const Triangulation<3>*) const {
    return coords_[10 * tetIndex + 4 + quadType];
}
inline LargeInteger NSVectorANStandard::octs(
        size_t tetIndex, int octType, const Triangulation<3>*) const {
    return coords_[10 * tetIndex + 7 + octType];
}

} // namespace regina

#endif

