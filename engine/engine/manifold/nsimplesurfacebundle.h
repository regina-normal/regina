
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful, but   *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *  General Public License for more details.                              *
 *                                                                        *
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nsimplesurfacebundle.h
 *  \brief Deals with simple closed surface bundles.
 */

#ifndef __NSIMPLESURFACEBUNDLE_H
#ifndef __DOXYGEN
#define __NSIMPLESURFACEBUNDLE_H
#endif

#include "nmanifold.h"

namespace regina {

/**
 * \weakgroup manifold
 * @{
 */

/**
 * Represents a particularly simple closed surface bundle over the circle.
 * Only 2-sphere bundles, twisted 2-sphere bundles and projective plane
 * bundles are considered.
 *
 * All optional NManifold routines are implemented for this class.
 */
class NSimpleSurfaceBundle : public NManifold {
    public:
        /**
         * Represents the orientable 2-sphere bundle over the circle.
         */
        static const int S2xS1;
        /**
         * Represents the non-orientable twisted 2-sphere bundle over the
         * circle.
         */
        static const int S2xS1_TWISTED;
        /**
         * Represents the projective plane bundle over the circle.
         */
        static const int RP2xS1;
    private:
        int type;
            /**< The specific surface bundle being represented.
                 This must be one of the 3-manifold constants defined in
                 this class. */

    public:
        /**
         * Creates a new surface bundle of the given type.
         *
         * @param newType the specific type of surface bundle to
         * represent.  This must be one of the 3-manifold constants
         * defined in this class.
         */
        NSimpleSurfaceBundle(int newType);
        /**
         * Creates a clone of the given surface bundle.
         *
         * @param cloneMe the surface bundle to clone.
         */
        NSimpleSurfaceBundle(const NSimpleSurfaceBundle& cloneMe);
        /**
         * Returns the specific type of surface bundle being represented.
         *
         * @return the type of surface bundle.  This will be one of the
         * 3-manifold constants defined in this class.
         */
        int getType() const;
        /**
         * Determines whether this and the given surface bundle represent
         * the same 3-manifold.
         *
         * @param compare the surface bundle with which this will be compared.
         * @return \c true if and only if this and the given surface bundle
         * are homeomorphic.
         */
        bool operator == (const NSimpleSurfaceBundle& compare) const;

        virtual NTriangulation* construct() const;
        NAbelianGroup* getHomologyH1() const;
        std::ostream& writeName(std::ostream& out) const;
        std::ostream& writeTeXName(std::ostream& out) const;
};

/*@}*/

// Inline functions for NSimpleSurfaceBundle

inline NSimpleSurfaceBundle::NSimpleSurfaceBundle(
        int newType) : type(newType) {
}
inline NSimpleSurfaceBundle::NSimpleSurfaceBundle(
        const NSimpleSurfaceBundle& cloneMe) : NManifold(),
        type(cloneMe.type) {
}
inline int NSimpleSurfaceBundle::getType() const {
    return type;
}
inline bool NSimpleSurfaceBundle::operator ==
        (const NSimpleSurfaceBundle& compare) const {
    return (type == compare.type);
}

} // namespace regina

#endif

