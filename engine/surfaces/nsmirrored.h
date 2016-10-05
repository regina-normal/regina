
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

/*! \file surfaces/nsmirrored.h
 *  \brief Provides a normal surface vector that is mirrored in another
 *  coordinate system to avoid frequent lengthy calculations.
 */

#ifndef __NSMIRRORED_H
#ifndef __DOXYGEN
#define __NSMIRRORED_H
#endif

#include "regina-core.h"
#include "surfaces/nnormalsurface.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

/**
 * A normal surface vector that is mirrored in another coordinate system
 * to avoid frequent lengthy calculations.  When it is difficult to
 * convert from the native coordinate system to standard tri-quad-oct
 * coordinates, use this as a base class.  The conversion of the entire
 * vector will be done once only, and future coordinate lookups will be
 * performed through the pre-converted mirror vector.
 *
 * Once the first coordinate lookup has taken place (via
 * triangles() or the like), <b>this vector may
 * not change!</b>  The mirror will be created at this point and will
 * not change, so if the native coordinates change further then any
 * requests passed to the mirror will return incorrect results.
 *
 * Subclasses need not implement any of the coordinate lookup routines.
 * The default implementation will be to pass the lookup to the mirror.
 * If any particular lookup can be done more efficiently in the native
 * coordinate system, the corresponding routine should be overridden.
 *
 * Subclasses must however implement the routine makeMirror() which creates
 * the alternate mirror vector from this vector.
 *
 * Note that cloning a vector of this class will \e not clone the
 * mirror.  Thus a clone may be safely modified before its first
 * coordinate lookup routine is called.
 *
 * \todo \prob Allow modification of the vector by overwriting
 * setValue(); this will require documentation changes in both this
 * class and in NNormalSurfaceVector.
 *
 * \ifacespython Not present.
 */
class REGINA_API NNormalSurfaceVectorMirrored : public NNormalSurfaceVector {
    private:
        NNormalSurfaceVector* mirror;
            /**< The mirror vector. */

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalSurfaceVectorMirrored(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalSurfaceVectorMirrored(const Vector<LargeInteger>& cloneMe);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalSurfaceVectorMirrored(const NNormalSurfaceVectorMirrored&
            cloneMe);
        /**
         * Destroys this vector and its mirror if appropriate.
         */
        virtual ~NNormalSurfaceVectorMirrored();

        /**
         * Creates a new mirror vector corresponding to this vector.
         * The mirror vector should represent the same normal surface as
         * this vector, and should have fast coordinate lookup routines
         * (triangles(), quads() and so on).  It is
         * recommended that the mirror vector be a
         * NNormalSurfaceVectorStandard or a NNormalSurfaceVectorANStandard.
         *
         * @param triang the triangulation in which this normal surface
         * lives.
         * @return a newly created mirror vector.
         */
        virtual NNormalSurfaceVector* makeMirror(const NTriangulation* triang)
            const = 0;

        virtual LargeInteger triangles(size_t tetIndex,
            int vertex, const NTriangulation* triang) const;
        virtual LargeInteger orientedTriangles(size_t tetIndex,
            int vertex, const NTriangulation* triang, bool orientation) const;
        virtual LargeInteger quads(size_t tetIndex,
            int quadType, const NTriangulation* triang) const;
        virtual LargeInteger orientedQuads(size_t tetIndex,
            int quadType, const NTriangulation* triang, bool orientation) const;
        virtual LargeInteger octs(size_t tetIndex,
            int octType, const NTriangulation* triang) const;
        virtual LargeInteger edgeWeight(size_t edgeIndex,
            const NTriangulation* triang) const;
        virtual LargeInteger arcs(size_t triIndex,
            int triVertex, const NTriangulation* triang) const;
};

/*@}*/

// Inline functions for NNormalSurfaceVectorMirrored

inline NNormalSurfaceVectorMirrored::NNormalSurfaceVectorMirrored(
        size_t length) : NNormalSurfaceVector(length), mirror(0) {
}
inline NNormalSurfaceVectorMirrored::NNormalSurfaceVectorMirrored(
        const Vector<LargeInteger>& cloneMe) :
        NNormalSurfaceVector(cloneMe), mirror(0) {
}
inline NNormalSurfaceVectorMirrored::NNormalSurfaceVectorMirrored(
        const NNormalSurfaceVectorMirrored& cloneMe) :
        NNormalSurfaceVector(cloneMe), mirror(0) {
}
inline NNormalSurfaceVectorMirrored::~NNormalSurfaceVectorMirrored() {
    if (mirror)
        delete mirror;
}

inline LargeInteger NNormalSurfaceVectorMirrored::triangles(
        size_t tetIndex, int vertex, const NTriangulation* triang) const {
    if (! mirror)
        const_cast<NNormalSurfaceVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->triangles(tetIndex, vertex, triang);
}
inline LargeInteger NNormalSurfaceVectorMirrored::orientedTriangles(
        size_t tetIndex, int vertex, const NTriangulation* triang,
        bool orientation) const {
    if (! mirror)
        const_cast<NNormalSurfaceVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->orientedTriangles(tetIndex, vertex, triang,
        orientation);
}
inline LargeInteger NNormalSurfaceVectorMirrored::quads(
        size_t tetIndex, int quadType, const NTriangulation* triang) const {
    if (! mirror)
        const_cast<NNormalSurfaceVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->quads(tetIndex, quadType, triang);
}
inline LargeInteger NNormalSurfaceVectorMirrored::orientedQuads(
        size_t tetIndex, int quadType, const NTriangulation* triang,
        bool orientation) const {
    if (! mirror)
        const_cast<NNormalSurfaceVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->orientedQuads(tetIndex, quadType, triang,
        orientation);
}
inline LargeInteger NNormalSurfaceVectorMirrored::octs(
        size_t tetIndex, int octType, const NTriangulation* triang) const {
    if (! mirror)
        const_cast<NNormalSurfaceVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->octs(tetIndex, octType, triang);
}
inline LargeInteger NNormalSurfaceVectorMirrored::edgeWeight(
        size_t edgeIndex, const NTriangulation* triang) const {
    if (! mirror)
        const_cast<NNormalSurfaceVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->edgeWeight(edgeIndex, triang);
}
inline LargeInteger NNormalSurfaceVectorMirrored::arcs(
        size_t triIndex, int triVertex,
        const NTriangulation* triang) const {
    if (! mirror)
        const_cast<NNormalSurfaceVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->arcs(triIndex, triVertex, triang);
}

} // namespace regina

#endif

