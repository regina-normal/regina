
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

/*! \file hypersurface/nhsmirrored.h
 *  \brief Provides a normal hypersurface vector that is mirrored in another
 *  coordinate system to avoid frequent lengthy calculations.
 */

#ifndef __NHSMIRRORED_H
#ifndef __DOXYGEN
#define __NHSMIRRORED_H
#endif

#include "regina-core.h"
#include "hypersurface/nnormalhypersurface.h"

namespace regina {

/**
 * \weakgroup hypersurface
 * @{
 */

/**
 * A normal hypersurface vector that is mirrored in another coordinate system
 * to avoid frequent lengthy calculations.  When it is difficult to
 * convert from the native coordinate system to standard tetrahedron-prism
 * coordinates, use this as a base class.  The conversion of the entire
 * vector will be done once only, and future coordinate lookups will be
 * performed through the pre-converted mirror vector.
 *
 * Once the first coordinate lookup has taken place (via
 * tetrahedra() or the like), <b>this vector may
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
 * class and in NNormalHypersurfaceVector.
 *
 * \ifacespython Not present.
 */
class REGINA_API NNormalHypersurfaceVectorMirrored :
        public NNormalHypersurfaceVector {
    private:
        NNormalHypersurfaceVector* mirror_;
            /**< The mirror vector. */

    public:
        /**
         * Creates a new vector all of whose entries are initialised to
         * zero.
         *
         * @param length the number of elements in the new vector.
         */
        NNormalHypersurfaceVectorMirrored(size_t length);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalHypersurfaceVectorMirrored(
            const Vector<NLargeInteger>& cloneMe);
        /**
         * Creates a new vector that is a clone of the given vector.
         *
         * @param cloneMe the vector to clone.
         */
        NNormalHypersurfaceVectorMirrored(
            const NNormalHypersurfaceVectorMirrored& cloneMe);
        /**
         * Destroys this vector and its mirror if appropriate.
         */
        virtual ~NNormalHypersurfaceVectorMirrored();

        /**
         * Creates a new mirror vector corresponding to this vector.
         * The mirror vector should represent the same normal hypersurface as
         * this vector, and should have fast coordinate lookup routines
         * (tetrahedra(), prisms() and so on).  It is
         * recommended that the mirror vector be of the class
         * NNormalHypersurfaceVectorStandard.
         *
         * @param triang the triangulation in which this normal hypersurface
         * lives.
         * @return a newly created mirror vector.
         */
        virtual NNormalHypersurfaceVector* makeMirror(
            const Dim4Triangulation* triang) const = 0;

        virtual NLargeInteger tetrahedra(size_t pentIndex,
            int vertex, const Dim4Triangulation* triang) const;
        virtual NLargeInteger prisms(size_t pentIndex,
            int prismType, const Dim4Triangulation* triang) const;
        virtual NLargeInteger edgeWeight(size_t edgeIndex,
            const Dim4Triangulation* triang) const;
};

/*@}*/

// Inline functions for NNormalHypersurfaceVectorMirrored

inline NNormalHypersurfaceVectorMirrored::NNormalHypersurfaceVectorMirrored(
        size_t length) : NNormalHypersurfaceVector(length), mirror_(0) {
}
inline NNormalHypersurfaceVectorMirrored::NNormalHypersurfaceVectorMirrored(
        const Vector<NLargeInteger>& cloneMe) :
        NNormalHypersurfaceVector(cloneMe), mirror_(0) {
}
inline NNormalHypersurfaceVectorMirrored::NNormalHypersurfaceVectorMirrored(
        const NNormalHypersurfaceVectorMirrored& cloneMe) :
        NNormalHypersurfaceVector(cloneMe), mirror_(0) {
}
inline NNormalHypersurfaceVectorMirrored::~NNormalHypersurfaceVectorMirrored() {
    if (mirror_)
        delete mirror_;
}

inline NLargeInteger NNormalHypersurfaceVectorMirrored::tetrahedra(
        size_t pentIndex, int vertex, const Dim4Triangulation* triang) const {
    if (! mirror_)
        const_cast<NNormalHypersurfaceVectorMirrored*>(this)->mirror_ =
            makeMirror(triang);
    return mirror_->tetrahedra(pentIndex, vertex, triang);
}
inline NLargeInteger NNormalHypersurfaceVectorMirrored::prisms(
        size_t pentIndex, int prismType, const Dim4Triangulation* triang)
        const {
    if (! mirror_)
        const_cast<NNormalHypersurfaceVectorMirrored*>(this)->mirror_ =
            makeMirror(triang);
    return mirror_->prisms(pentIndex, prismType, triang);
}
inline NLargeInteger NNormalHypersurfaceVectorMirrored::edgeWeight(
        size_t edgeIndex, const Dim4Triangulation* triang) const {
    if (! mirror_)
        const_cast<NNormalHypersurfaceVectorMirrored*>(this)->mirror_ =
            makeMirror(triang);
    return mirror_->edgeWeight(edgeIndex, triang);
}

} // namespace regina

#endif

