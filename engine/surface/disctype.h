
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file surface/disctype.h
 *  \brief Deals with normal and almost normal disc types.
 */

#ifndef __REGINA_DISCTYPE_H
#ifndef __DOXYGEN
#define __REGINA_DISCTYPE_H
#endif

#include <iostream>
#include "regina-core.h"

namespace regina {

/**
 * Identifies a single normal or almost normal disc type within a
 * triangulation.
 *
 * A disc type is identified by a tetrahedron index (the data member
 * \a tetIndex), and a disc type within that tetrahedron (the data
 * member \a type).  The latter could mean any number of things according
 * to the application at hand.  For instance, if we are tracking quad types
 * then \a type might be an integer between 0 and 2 inclusive, or if we
 * are tracking all normal discs in standard coordinates then \a type
 * might be an integer between 0 and 6 inclusive.  Ultimately, the
 * specific meaning of \a type is left to the user.
 *
 * It is however assumed that \a type will always be non-negative for
 * "meaningful" disc types.
 *
 * This class can also store a _null_ disc type; this is obtained by calling
 * the default constructor, and it will have a type of -1 and a tetrahedron
 * index of 0.  You can test for a null disc type by casting to \c bool.
 *
 * Note that this class tracks disc \a types, not discs themselves.
 * To track individual normal discs, see the DiscSpec class instead.
 *
 * These objects are small enough to pass by value and swap with std::swap(),
 * with no need for any specialised move operations or swap functions.
 *
 * \ingroup surfaces
 */
struct DiscType {
    size_t tetIndex;
        /**< The index within the triangulation of the tetrahedron
             containing this disc type.  This must be between 0 and
             Triangulation<3>::size()-1 inclusive. */
    int type;
        /**< Identifies the disc type within the specified tetrahedron.
             The precise meaning of this member is left up to the user,
             though it must be non-negative for "meaningful" disc types.
             See the DiscType class notes for details. */

    /**
     * Creates a new null disc type, as described in the class notes.
     */
    constexpr DiscType();
    /**
     * Creates a new disc type initialised with the given values.
     *
     * \param newTetIndex the index within the triangulation of the
     * tetrahedron containing this disc type.
     * \param newType the specific disc type within the given
     * tetrahedron; see the class notes for the meaning of this field.
     */
    constexpr DiscType(size_t newTetIndex, int newType);
    /**
     * Creates a copy of the given disc type.
     */
    constexpr DiscType(const DiscType&) = default;

    /**
     * Returns \c true if this disc type is non-null.
     *
     * The implementation will assume that a non-null disc type
     * has non-negative \a type (as explained in the class notes).
     *
     * \return \c true if and only if this is not a null disc type.
     */
    constexpr operator bool() const;

    /**
     * Sets this to a copy of the given disc type.
     *
     * \return a reference to this disc type.
     */
    DiscType& operator = (const DiscType&) = default;
    /**
     * Determines if this and the given disc type are identical.
     *
     * Regarding null disc types: two null DiscType objects that were both
     * created using the default constructor will be considered equal to
     * each other, and will not be equal to any "meaningful" disc type
     * (where \a type is non-negative).
     *
     * \return \c true if this and the given disc type are identical, or
     * \c false if they are different.
     */
    constexpr bool operator == (const DiscType&) const = default;
    /**
     * Compares two disc types.
     * Types are ordered first by \a tetrahedron and then by \a type.
     * The null disc type is considered less than all "meaningful" disc types.
     *
     * This generates all of the usual comparison operators, including
     * `<`, `<=`, `>`, and `>=`.
     *
     * \python This spaceship operator `x <=> y` is not available, but the
     * other comparison operators that it generates _are_ available.
     *
     * \return The result of the comparison between this and the given
     * disc type.
     */
    constexpr std::strong_ordering operator <=> (const DiscType&) const =
        default;
};

/**
 * Writes the given disc type to the given output stream.
 * The disc type will be written as a pair `(tetIndex, type)`.
 *
 * \param out the output stream to which to write.
 * \param type the disc type to write.
 * \return a reference to the given output stream.
 *
 * \ingroup surfaces
 */
std::ostream& operator << (std::ostream& out, const DiscType& type);

// Inline functions for DiscType

inline constexpr DiscType::DiscType() : tetIndex(0), type(-1) {
}

inline constexpr DiscType::DiscType(size_t newTetIndex, int newType) :
        tetIndex(newTetIndex), type(newType) {
}

inline constexpr DiscType::operator bool() const {
    return (type >= 0);
}

inline std::ostream& operator << (std::ostream& out, const DiscType& type) {
    return out << type.tetIndex << ':' << type.type;
}

} // namespace regina

#endif

