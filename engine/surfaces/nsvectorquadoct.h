
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

/*! \file surfaces/nsvectorquadoct.h
 *  \brief Implements almost normal surface vectors using quad-oct coordinates.
 */

#ifndef __NSVECTORQUADOCT_H
#ifndef __DOXYGEN
#define __NSVECTORQUADOCT_H
#endif

#include "regina-core.h"
#include "surfaces/nsvectormirrored.h"

namespace regina {

class NSVectorQuadOct;

/**
 * \weakgroup surfaces
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct NormalInfo<NS_AN_QUAD_OCT> {
    typedef NSVectorQuadOct Class;
    typedef NormalInfo<NS_AN_STANDARD> Standard;
    typedef NormalInfo<NS_AN_QUAD_OCT> Reduced;
    inline static const char* name() {
        return "Quad-oct almost normal";
    }
    enum {
        almostNormal = 1,
        spun = 1,
        oriented = 0
    };
};
#endif

/**
 * An almost normal surface vector using quad-oct coordinates.
 *
 * If there are \a t tetrahedra in the underlying
 * triangulation, there must be precisely 6<i>t</i> coordinates.
 * The first six coordinates will be for the first tetrahedron, the
 * next six for the second tetrahedron and so on.  For each
 * tetrahedron, the first three coordinates represent the number of
 * quadrilateral discs of type 0, 1 and 2 (see NormalSurface::quads()),
 * and the final three represent the number of octagonal discs of type
 * 0, 1 and 2 (see NormalSurface::octs()).
 *
 * \ifacespython Not present.
 */
class REGINA_API NSVectorQuadOct :
        public NSVectorMirrored {
    REGINA_NORMAL_SURFACE_FLAVOUR(NSVectorQuadOct, NS_AN_QUAD_OCT,
        NSVectorMirrored);

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NSVectorQuadOct(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NSVectorQuadOct(const Vector<LargeInteger>& cloneMe);

        static NormalSurfaceVector* makeMirror(const Ray& original,
            const Triangulation<3>* triang);
        virtual NormalSurfaceVector* makeMirror(const Triangulation<3>* triang)
            const;

        virtual const Vertex<3>* isVertexLink(const Triangulation<3>* triang) const;

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
 * \deprecated The class NNormalSurfaceVectorQuadOct has now been
 * renamed to NSVectorQuadOct.
 */
REGINA_DEPRECATED typedef NSVectorQuadOct NNormalSurfaceVectorQuadOct;

/*@}*/

// Inline functions for NSVectorQuadOct

inline NSVectorQuadOct::NSVectorQuadOct(
        size_t length) : NSVectorMirrored(length) {
}
inline NSVectorQuadOct::NSVectorQuadOct(
        const Vector<LargeInteger>& cloneMe) :
        NSVectorMirrored(cloneMe) {
}

inline NormalSurfaceVector* NSVectorQuadOct::makeMirror(
        const Triangulation<3>* triang) const {
    return makeMirror(coords(), triang);
}

inline const Vertex<3>* NSVectorQuadOct::isVertexLink(
        const Triangulation<3>*) const {
    // Quad-oct space does not contain vertex links at all.
    return 0;
}

} // namespace regina

#endif

