
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/*! \file hypersurface/nhsstandard.h
 *  \brief Implements normal hypersurface vectors using standard
 *  tetrahedron-prism coordinates.
 */

#ifndef __NHSSTANDARD_H
#ifndef __DOXYGEN
#define __NHSSTANDARD_H
#endif

#include "regina-core.h"
#include "hypersurface/nnormalhypersurface.h"

namespace regina {

class NMatrixInt;
class NNormalHypersurfaceVectorStandard;

/**
 * \weakgroup hypersurface
 * @{
 */

#ifndef __DOXYGEN // Doxygen complains about undocumented specialisations.
template <>
struct HyperInfo<HS_STANDARD> {
    typedef NNormalHypersurfaceVectorStandard Class;
    inline static const char* name() {
        return "Standard normal (tet-prism)";
    }
};
#endif

/**
 * A normal hypersurface vector using standard tetrahedron-prism coordinates.
 *
 * If there are \a p pentachora in the underlying
 * triangulation, there must be precisely 15<i>p</i> coordinates.
 * The first 15 coordinates will be for the first pentachoron, the
 * next 15 for the second pentachoron and so on.  For each
 * pentachoron, the first five represent the number of
 * tetrahedron pieces about vertex 0,...,4, and the next
 * ten represent the number of prism pieces of type 0,...,9
 * (see NNormalHypersurface::prisms() for details).
 *
 * \ifacespython Not present.
 */
class REGINA_API NNormalHypersurfaceVectorStandard :
        public NNormalHypersurfaceVector {
    REGINA_NORMAL_HYPERSURFACE_FLAVOUR(NNormalHypersurfaceVectorStandard, HS_STANDARD)

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalHypersurfaceVectorStandard(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalHypersurfaceVectorStandard(
            const NVector<NLargeInteger>& cloneMe);

        virtual NLargeInteger tetrahedra(size_t pentIndex,
            int vertex, const Dim4Triangulation* triang) const;
        virtual NLargeInteger prisms(size_t pentIndex,
            int prismType, const Dim4Triangulation* triang) const;
        virtual NLargeInteger edgeWeight(size_t edgeIndex,
            const Dim4Triangulation* triang) const;

        static NNormalHypersurfaceVector* makeZeroVector(
            const Dim4Triangulation* triangulation);
        static NMatrixInt* makeMatchingEquations(
            const Dim4Triangulation* triangulation);
        static NEnumConstraintList* makeEmbeddedConstraints(
            const Dim4Triangulation* triangulation);
};

/*@}*/

// Inline functions for NNormalHypersurfaceVectorStandard

inline NNormalHypersurfaceVectorStandard::NNormalHypersurfaceVectorStandard(
        size_t length) : NNormalHypersurfaceVector(length) {
}
inline NNormalHypersurfaceVectorStandard::NNormalHypersurfaceVectorStandard(
        const NVector<NLargeInteger>& cloneMe) :
        NNormalHypersurfaceVector(cloneMe) {
}

inline NLargeInteger NNormalHypersurfaceVectorStandard::tetrahedra(
        size_t pentIndex, int vertex, const Dim4Triangulation*) const {
    return (*this)[15 * pentIndex + vertex];
}
inline NLargeInteger NNormalHypersurfaceVectorStandard::prisms(
        size_t pentIndex, int prismType, const Dim4Triangulation*) const {
    return (*this)[15 * pentIndex + 5 + prismType];
}

} // namespace regina

#endif

