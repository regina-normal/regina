
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

/*! \file surfaces/disctype.h
 *  \brief Deals with normal and almost normal disc types.
 */

#ifndef __DISCTYPE_H
#ifndef __DOXYGEN
#define __DISCTYPE_H
#endif

#include <iostream>
#include "regina-core.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

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
 * "meaningful" disc types; this is to ensure that the constant NONE
 * does not clash with any meaningful values.
 *
 * Note that this class tracks disc \a types, not discs themselves.
 * To track individual normal discs, see the DiscSpec class instead.
 */
struct REGINA_API DiscType {
    static const DiscType NONE;
        /**< Represents a "null" disc type.  Here the \a type member is
             negative, to distinguish it from "meaningful" disc types
             in which \a type is always zero or positive. */

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
     * Creates a new disc type initialised to NONE.
     */
    DiscType();
    /**
     * Creates a new disc type initialised with the given values.
     *
     * @param newTetIndex the index within the triangulation of the
     * tetrahedron containing this disc type.
     * @param newType the specific disc type within the given
     * tetrahedron; see the class notes for the meaning of this field.
     */
    DiscType(size_t newTetIndex, int newType);
    /**
     * Creates a copy of the given disc type.
     *
     * @param cloneMe the disc type to clone.
     */
    DiscType(const DiscType& cloneMe);

    /**
     * Sets this to a copy of the given disc type.
     *
     * @param cloneMe the disc type to clone.
     * @return a reference to this disc type.
     */
    DiscType& operator = (const DiscType& cloneMe);
    /**
     * Determines if this and the given disc type are identical.
     *
     * Note that NONE is considered identical to NONE, and that NONE will
     * not be equal to any "meaningful" disc type (specifically, a disc type
     * for which \a type is non-negative).
     *
     * @return \c true if this and the given disc type are identical, or
     * \c false if they are different.
     */
    bool operator == (const DiscType& compare) const;
    /**
     * Determines if this and the given disc type are different.
     *
     * This is the negation of the equality test; see operator == for
     * further details.
     *
     * @return \c true if this and the given disc type are different, or
     * \c false if they are identical.
     */
    bool operator != (const DiscType& compare) const;
    /**
     * Provides an ordering of disc types.  Types are ordered first by
     * \a tetrahedron and then by \a type.  NONE is considered less than
     * all "meaningful" disc types.
     *
     * @return \c true if this disc type appears before the given disc type
     * in the ordering, or \c false if not.
     */
    bool operator < (const DiscType& compare) const;
};

/**
 * Writes the given disc type to the given output stream.
 * The disc type will be written as a pair <tt>(tetIndex, type)</tt>.
 *
 * @param out the output stream to which to write.
 * @param type the disc type to write.
 * @return a reference to the given output stream.
 */
REGINA_API std::ostream& operator << (std::ostream& out, const DiscType& type);

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NDiscType has now been renamed to DiscType.
 */
REGINA_DEPRECATED typedef DiscType NDiscType;

/*@}*/

// Inline functions for DiscType

inline DiscType::DiscType() : tetIndex(0), type(-1) {
}

inline DiscType::DiscType(size_t newTetIndex, int newType) :
        tetIndex(newTetIndex), type(newType) {
}

inline DiscType::DiscType(const DiscType& cloneMe) :
        tetIndex(cloneMe.tetIndex), type(cloneMe.type) {
}

inline DiscType& DiscType::operator = (const DiscType& cloneMe) {
    tetIndex = cloneMe.tetIndex;
    type = cloneMe.type;
    return *this;
}

inline bool DiscType::operator == (const DiscType& compare) const {
    return (tetIndex == compare.tetIndex && type == compare.type);
}

inline bool DiscType::operator != (const DiscType& compare) const {
    return (tetIndex != compare.tetIndex || type != compare.type);
}

inline bool DiscType::operator < (const DiscType& compare) const {
    return (tetIndex < compare.tetIndex ||
        (tetIndex == compare.tetIndex && type < compare.type));
}

inline std::ostream& operator << (std::ostream& out, const DiscType& type) {
    return out << '(' << type.tetIndex << ", " << type.type << ')';
}

} // namespace regina

#endif

