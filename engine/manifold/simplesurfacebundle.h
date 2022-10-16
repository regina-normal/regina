
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file manifold/simplesurfacebundle.h
 *  \brief Deals with simple closed surface bundles.
 */

#ifndef __REGINA_SIMPLESURFACEBUNDLE_H
#ifndef __DOXYGEN
#define __REGINA_SIMPLESURFACEBUNDLE_H
#endif

#include "regina-core.h"
#include "manifold.h"

namespace regina {

/**
 * Represents a particularly simple closed surface bundle over the circle.
 * Only 2-sphere bundles, twisted 2-sphere bundles and projective plane
 * bundles are considered.
 *
 * All optional Manifold routines are implemented for this class.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the other manifold classes.
 *
 * \ingroup manifold
 */
class SimpleSurfaceBundle : public Manifold {
    public:
        /**
         * Represents the orientable 2-sphere bundle over the circle.
         */
        static constexpr int S2xS1 = 1;
        /**
         * Represents the non-orientable twisted 2-sphere bundle over the
         * circle.
         */
        static constexpr int S2xS1_TWISTED = 2;
        /**
         * Represents the projective plane bundle over the circle.
         */
        static constexpr int RP2xS1 = 3;

    private:
        int type_;
            /**< The specific surface bundle being represented.
                 This must be one of the 3-manifold constants defined in
                 this class. */

    public:
        /**
         * Creates a new surface bundle of the given type.
         *
         * @param bundleType the specific type of surface bundle to
         * represent.  This must be one of the 3-manifold constants
         * defined in this class.
         */
        SimpleSurfaceBundle(int bundleType);
        /**
         * Creates a new copy of the given surface bundle.
         */
        SimpleSurfaceBundle(const SimpleSurfaceBundle&) = default;
        /**
         * Returns the specific type of surface bundle being represented.
         *
         * @return the type of surface bundle.  This will be one of the
         * 3-manifold constants defined in this class.
         */
        int type() const;
        /**
         * Determines whether this and the given surface bundle are of
         * the same type (i.e., they represent the same 3-manifold).
         *
         * @param compare the surface bundle with which this will be compared.
         * @return \c true if and only if this and the given surface bundle
         * are of the same type.
         */
        bool operator == (const SimpleSurfaceBundle& compare) const;
        /**
         * Determines whether this and the given surface bundle are of
         * different types (i.e., they represent different 3-manifolds).
         *
         * @param compare the surface bundle with which this will be compared.
         * @return \c true if and only if this and the given surface bundle
         * are of different types.
         */
        bool operator != (const SimpleSurfaceBundle& compare) const;

        /**
         * Sets this to be a copy of the given surface bundle.
         *
         * @return a reference to this surface bundle.
         */
        SimpleSurfaceBundle& operator = (const SimpleSurfaceBundle&) = default;

        /**
         * Swaps the contents of this and the given surface bundle.
         *
         * @param other the surface bundle whose contents should be swapped
         * with this.
         */
        void swap(SimpleSurfaceBundle& other) noexcept;

        Triangulation<3> construct() const override;
        AbelianGroup homology() const override;
        bool isHyperbolic() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
};

/**
 * Swaps the contents of the two given surface bundles.
 *
 * This global routine simply calls SimpleSurfaceBundle::swap(); it is
 * provided so that SimpleSurfaceBundle meets the C++ Swappable requirements.
 *
 * @param a the first surface bundle whose contents should be swapped.
 * @param b the second surface bundle whose contents should be swapped.
 *
 * \ingroup manifold
 */
void swap(SimpleSurfaceBundle& a, SimpleSurfaceBundle& b) noexcept;

// Inline functions for SimpleSurfaceBundle

inline SimpleSurfaceBundle::SimpleSurfaceBundle(int bundleType) :
        type_(bundleType) {
}
inline int SimpleSurfaceBundle::type() const {
    return type_;
}
inline bool SimpleSurfaceBundle::operator ==
        (const SimpleSurfaceBundle& compare) const {
    return (type_ == compare.type_);
}
inline bool SimpleSurfaceBundle::operator !=
        (const SimpleSurfaceBundle& compare) const {
    return (type_ != compare.type_);
}

inline bool SimpleSurfaceBundle::isHyperbolic() const {
    return false;
}

inline void SimpleSurfaceBundle::swap(SimpleSurfaceBundle& other) noexcept {
    std::swap(type_, other.type_);
}

inline void swap(SimpleSurfaceBundle& a, SimpleSurfaceBundle& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

