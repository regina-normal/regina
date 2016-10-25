
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

/*! \file surfaces/nsvectormirrored.h
 *  \brief Provides a normal surface vector that is mirrored in another
 *  coordinate system to avoid frequent lengthy calculations.
 */

#ifndef __NSVECTORMIRRORED_H
#ifndef __DOXYGEN
#define __NSVECTORMIRRORED_H
#endif

#include "regina-core.h"
#include "surfaces/normalsurface.h"

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
 * Subclasses need not implement any of the coordinate lookup routines.
 * The default implementation will be to pass the lookup to the mirror.
 * If any particular lookup can be done more efficiently in the native
 * coordinate system, the corresponding routine should be overridden.
 *
 * Subclasses must however implement two variants of makeMirror():
 *
 * - a static function
 *   NormalSurfaceVector* makeMirror(const Ray&, const Triangulation<3>*),
 *   which builds the mirror vector from the native vector;
 * - a virtual function
 *   NormalSurfaceVector* makeMirror(const Triangulation<3>*) const,
 *   which simply calls the static function described above.
 *
 * \ifacespython Not present.
 */
class REGINA_API NSVectorMirrored : public NormalSurfaceVector {
    private:
        NormalSurfaceVector* mirror;
            /**< The mirror vector. */

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NSVectorMirrored(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NSVectorMirrored(const Vector<LargeInteger>& cloneMe);
        /**
         * Destroys this vector and its mirror if appropriate.
         */
        virtual ~NSVectorMirrored();

        /**
         * Creates a new mirror vector corresponding to this vector.
         *
         * This function must return a newly created vector that describes
         * this normal surface in the corresponding "standard" coordinate
         * system (i.e., NSVectorStandard if this subclass is NSVectorQuad, or
         * NSVectorANStandard if this subclass is NSVectorQuadOct, and so on.)
         *
         * Typically subclasses should implement this function by calling the
         * corresponding static makeMirror() function, which does the real work.
         *
         * @param triang the triangulation in which this normal surface lives.
         * @return a newly created vector that describes the same normal
         * surface in a "standard" coordinate system, as described above.
         */
        virtual NormalSurfaceVector* makeMirror(const Triangulation<3>* triang)
            const = 0;

#ifdef __DOXYGEN
        /**
         * Creates a new mirror vector corresponding to the given vector.
         * This function should be implemented by each subclass of
         * NSVectorMirrored.
         *
         * This function must return a newly created vector that describes
         * the given normal surface in a different coordinate system:
         *
         * - The given vector must be in the coordinate system described
         *   by the subclass in which this function is implemented.
         *
         * - The return vector must be in the corresponding "standard"
         *   coordinate system (i.e., NSVectorStandard if this subclass is
         *   NSVectorQuad, or NSVectorANStandard if this subclass is
         *   NSVectorQuadOct, and so on.)
         *
         * @param original a vector in the coordinate system corresponding to
         * the subclass in which this function is implemented.
         * @param triang the triangulation in which the corresponding
         * normal surface lives.
         * @return a newly created vector that describes the same normal
         * surface in a "standard" coordinate system, as described above.
         */
        static NormalSurfaceVector* makeMirror(const Ray& original,
            const Triangulation<3>* triang);
#endif

        virtual void setElement(size_t index, const LargeInteger& value)
            override;
        virtual void operator += (const NormalSurfaceVector& other) override;
        virtual void scaleDown() override;

        virtual LargeInteger triangles(size_t tetIndex,
            int vertex, const Triangulation<3>* triang) const override;
        virtual LargeInteger orientedTriangles(size_t tetIndex,
            int vertex, const Triangulation<3>* triang, bool orientation) const
            override;
        virtual LargeInteger quads(size_t tetIndex,
            int quadType, const Triangulation<3>* triang) const override;
        virtual LargeInteger orientedQuads(size_t tetIndex,
            int quadType, const Triangulation<3>* triang, bool orientation) const
            override;
        virtual LargeInteger octs(size_t tetIndex,
            int octType, const Triangulation<3>* triang) const override;
        virtual LargeInteger edgeWeight(size_t edgeIndex,
            const Triangulation<3>* triang) const override;
        virtual LargeInteger arcs(size_t triIndex,
            int triVertex, const Triangulation<3>* triang) const override;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NNormalSurfaceVectorMirrored has now been renamed to
 * NSVectorMirrored.
 */
REGINA_DEPRECATED typedef NSVectorMirrored NNormalSurfaceVectorMirrored;

/*@}*/

// Inline functions for NSVectorMirrored

inline NSVectorMirrored::NSVectorMirrored(
        size_t length) : NormalSurfaceVector(length), mirror(0) {
}
inline NSVectorMirrored::NSVectorMirrored(
        const Vector<LargeInteger>& cloneMe) :
        NormalSurfaceVector(cloneMe), mirror(0) {
}
inline NSVectorMirrored::~NSVectorMirrored() {
    delete mirror;
}

inline void NSVectorMirrored::setElement(size_t index,
        const LargeInteger& value) {
    coords_.setElement(index, value);
    delete mirror;
}

inline void NSVectorMirrored::operator += (
        const NormalSurfaceVector& other) {
    coords_ += other.coords();
    delete mirror;
}

inline void NSVectorMirrored::scaleDown() {
    coords_.scaleDown();
    delete mirror;
}

inline LargeInteger NSVectorMirrored::triangles(
        size_t tetIndex, int vertex, const Triangulation<3>* triang) const {
    if (! mirror)
        const_cast<NSVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->triangles(tetIndex, vertex, triang);
}
inline LargeInteger NSVectorMirrored::orientedTriangles(
        size_t tetIndex, int vertex, const Triangulation<3>* triang,
        bool orientation) const {
    if (! mirror)
        const_cast<NSVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->orientedTriangles(tetIndex, vertex, triang,
        orientation);
}
inline LargeInteger NSVectorMirrored::quads(
        size_t tetIndex, int quadType, const Triangulation<3>* triang) const {
    if (! mirror)
        const_cast<NSVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->quads(tetIndex, quadType, triang);
}
inline LargeInteger NSVectorMirrored::orientedQuads(
        size_t tetIndex, int quadType, const Triangulation<3>* triang,
        bool orientation) const {
    if (! mirror)
        const_cast<NSVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->orientedQuads(tetIndex, quadType, triang,
        orientation);
}
inline LargeInteger NSVectorMirrored::octs(
        size_t tetIndex, int octType, const Triangulation<3>* triang) const {
    if (! mirror)
        const_cast<NSVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->octs(tetIndex, octType, triang);
}
inline LargeInteger NSVectorMirrored::edgeWeight(
        size_t edgeIndex, const Triangulation<3>* triang) const {
    if (! mirror)
        const_cast<NSVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->edgeWeight(edgeIndex, triang);
}
inline LargeInteger NSVectorMirrored::arcs(
        size_t triIndex, int triVertex,
        const Triangulation<3>* triang) const {
    if (! mirror)
        const_cast<NSVectorMirrored*>(this)->mirror =
            makeMirror(triang);
    return mirror->arcs(triIndex, triVertex, triang);
}

} // namespace regina

#endif

