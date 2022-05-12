
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

/*! \file manifold/handlebody.h
 *  \brief Deals with arbitrary handlebodies.
 */

#ifndef __REGINA_HANDLEBODY_H
#ifndef __DOXYGEN
#define __REGINA_HANDLEBODY_H
#endif

#include "regina-core.h"
#include "manifold.h"

namespace regina {

/**
 * Represents an orientable handlebody.
 *
 * All optional Manifold routines except for Manifold::construct() are
 * implemented for this class.
 *
 * This class supports copying but does not implement separate move operations,
 * since its internal data is so small that copying is just as efficient.
 * It implements the C++ Swappable requirement via its own member and global
 * swap() functions, for consistency with the other manifold classes.
 *
 * \ingroup manifold
 */
class Handlebody : public Manifold {
    private:
        size_t genus_;
            /**< The number of handles. */

    public:
        /**
         * Creates a new orientable handlebody of the given genus.
         *
         * @param genus the number of handles.
         */
        Handlebody(size_t genus);
        /**
         * Creates a new copy of the given handlebody.
         */
        Handlebody(const Handlebody&) = default;
        /**
         * Returns the genus of this handlebody.
         *
         * @return the genus; that is, number of handles.
         */
        size_t genus() const;
        /**
         * Determines whether this and the given handlebody have the same
         * genus.
         *
         * Handlebodies with the same genus are homeomorphic as 3-manifolds.
         *
         * @param compare the handlebody with which this will be compared.
         * @return \c true if and only if this and the given handlebody
         * have the same genus.
         */
        bool operator == (const Handlebody& compare) const;
        /**
         * Determines whether this and the given handlebody do not have
         * the same genus.
         *
         * Handlebodies with the same genus are homeomorphic as 3-manifolds.
         *
         * @param compare the handlebody with which this will be compared.
         * @return \c true if and only if this and the given handlebody
         * do not have the same genus.
         */
        bool operator != (const Handlebody& compare) const;

        /**
         * Sets this to be a copy of the given handlebody.
         *
         * @return a reference to this handlebody.
         */
        Handlebody& operator = (const Handlebody&) = default;

        /**
         * Swaps the contents of this and the given handlebody.
         *
         * @param other the handlebody whose contents should be swapped
         * with this.
         */
        void swap(Handlebody& other) noexcept;

        AbelianGroup homology() const override;
        bool isHyperbolic() const override;
        std::ostream& writeName(std::ostream& out) const override;
        std::ostream& writeTeXName(std::ostream& out) const override;
};

/**
 * Swaps the contents of the two given handlebodies.
 *
 * This global routine simply calls Handlebody::swap(); it is provided so
 * that LensSpace meets the C++ Handlebody requirements.
 *
 * @param a the first handlebody whose contents should be swapped.
 * @param b the second handlebody whose contents should be swapped.
 *
 * \ingroup manifold
 */
void swap(Handlebody& a, Handlebody& b) noexcept;

// Inline functions for Handlebody

inline Handlebody::Handlebody(size_t genus) : genus_(genus) {
}
inline size_t Handlebody::genus() const {
    return genus_;
}
inline bool Handlebody::operator == (const Handlebody& compare) const {
    return (genus_ == compare.genus_);
}
inline bool Handlebody::operator != (const Handlebody& compare) const {
    return (genus_ != compare.genus_);
}

inline bool Handlebody::isHyperbolic() const {
    return false;
}

inline void Handlebody::swap(Handlebody& other) noexcept {
    std::swap(genus_, other.genus_);
}

inline void swap(Handlebody& a, Handlebody& b) noexcept {
    a.swap(b);
}

} // namespace regina

#endif

