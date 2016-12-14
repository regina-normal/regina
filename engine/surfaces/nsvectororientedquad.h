
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

/*! \file surfaces/nsvectororientedquad.h
 *  \brief Implements normal surface vectors using
 *  transversely oriented quad coordinates.
 */

#ifndef __NSVECTORORIENTEDQUAD_H
#ifndef __DOXYGEN
#define __NSVECTORORIENTEDQUAD_H
#endif
#include "regina-core.h"
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
    inline static const char* name() {
        return "Transversely oriented quad normal";
    }
    enum {
        almostNormal = 0,
        spun = 1,
        oriented = 1
    };
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
class REGINA_API NSVectorOrientedQuad :
        public NSVectorMirrored {
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
        NSVectorOrientedQuad(const Vector<LargeInteger>& cloneMe);

        static NormalSurfaceVector* makeMirror(const Ray& original,
            const Triangulation<3>* triang);
        virtual NormalSurfaceVector* makeMirror(const Triangulation<3>* triang)
            const;

        virtual const Vertex<3>* isVertexLink(const Triangulation<3>* triang) const;

        virtual LargeInteger quads(size_t tetIndex,
            int quadType, const Triangulation<3>* triang) const;
        virtual LargeInteger orientedQuads(size_t tetIndex,
            int quadType, const Triangulation<3>* triang, bool orientation) const;
        virtual LargeInteger octs(size_t tetIndex,
            int octType, const Triangulation<3>* triang) const;

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
 * \deprecated The class NNormalSurfaceVectorOrientedQuad has now been
 * renamed to NSVectorOrientedQuad.
 */
REGINA_DEPRECATED typedef NSVectorOrientedQuad NNormalSurfaceVectorOrientedQuad;

/*@}*/

// Inline functions for NSVectorOrientedQuad

inline NSVectorOrientedQuad::NSVectorOrientedQuad(
        size_t length) : NSVectorMirrored(length) {
}
inline NSVectorOrientedQuad::NSVectorOrientedQuad(
        const Vector<LargeInteger>& cloneMe) :
        NSVectorMirrored(cloneMe) {
}

inline NormalSurfaceVector* NSVectorOrientedQuad::makeMirror(
        const Triangulation<3>* triang) const {
    return makeMirror(coords(), triang);
}

inline LargeInteger NSVectorOrientedQuad::quads(
        size_t tetIndex, int quadType, const Triangulation<3>* tri) const {
    return orientedQuads(tetIndex, quadType, tri, true)
           + orientedQuads(tetIndex, quadType, tri, false);
}

inline LargeInteger NSVectorOrientedQuad::orientedQuads(
        size_t tetIndex, int quadType, const Triangulation<3>*, 
        bool orientation) const {
    return coords_[6 * tetIndex + 2 * quadType +
        (orientation ? 0 : 1)];
}

inline const Vertex<3>* NSVectorOrientedQuad::isVertexLink(
        const Triangulation<3>*) const {
    // Quad space does not contain vertex links at all.
    return 0;
}

inline LargeInteger NSVectorOrientedQuad::octs(
        size_t, int, const Triangulation<3>*) const {
    return Ray::zero;
}

} // namespace regina

#endif

