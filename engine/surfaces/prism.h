
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

/*! \file surfaces/prism.h
 *  \brief Deals with triangular prisms defined by slicing along normal
 *  quads in a tetrahedron.
 */

#ifndef __PRISM_H
#ifndef __DOXYGEN
#define __PRISM_H
#endif

#include <iostream>
#include "regina-core.h"

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

class NormalSurface;

/**
 * Specifies a single triangular prism in a tetrahedron.
 *
 * If a tetrahedron contains normal quads, slicing along these quads
 * splits the tetrahedron into two triangular prisms (and possibly some
 * additional product regions).  Each triangular prism contains two of
 * the vertices and one of the edges of the original tetrahedron.
 *
 * \pre This class should only be used with \a embedded
 * normal surfaces.
 */
struct REGINA_API PrismSpec {
    size_t tetIndex;
        /**< The index in the triangulation of the tetrahedron
             containing the prism. */
    int edge;
        /**< The edge of the tetrahedron that is contained in this prism. */

    /**
     * Creates a new uninitialised prism specifier.
     */
    PrismSpec();
    /**
     * Creates a new prism specifier containing the given values.
     *
     * @param newTetIndex the index in the triangulation of the tetrahedron
     * containing the prism.
     * @param newEdge the edge of the tetrahedron that is contained in
     * this prism; this must be between 0 and 5 inclusive.
     */
    PrismSpec(size_t newTetIndex, int newEdge);
    /**
     * Creates a new prism specifier that is a clone of the given specifier.
     *
     * @param cloneMe the prism specifier to clone.
     */
    PrismSpec(const PrismSpec& cloneMe);

    /**
     * Copies the values from the given prism specifier into this specifier.
     *
     * @param cloneMe the prism specifier whose values should be copied.
     * @return a reference to this prism specifier.
     */
    PrismSpec& operator = (const PrismSpec& cloneMe);
    /**
     * Determines if this and the given prism specifier contain identical
     * information.
     *
     * @param other the prism specifier to compare with this.
     * @return \c true if and only if this and the given prism specifier
     * contain identical information.
     */
    bool operator == (const PrismSpec& other) const;
    /**
     * Determines if this and the given prism specifier contain different
     * information.
     *
     * @param other the prism specifier to compare with this.
     * @return \c true if and only if this and the given prism specifier
     * contain different information.
     */
    bool operator != (const PrismSpec& other) const;

    friend std::ostream& operator << (std::ostream& out,
        const PrismSpec& spec);
};

/**
 * Writes the given prism specifier to the given output stream.
 * The prism specifier will be written as a pair <tt>(tetIndex, edge)</tt>.
 *
 * @param out the output stream to which to write.
 * @param spec the prism specifier to write.
 * @return a reference to \a out.
 */
REGINA_API std::ostream& operator << (std::ostream& out,
    const PrismSpec& spec);

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated The class NPrismSpec has now been renamed to PrismSpec.
 */
REGINA_DEPRECATED typedef PrismSpec NPrismSpec;

/*@}*/

// Inline functions for PrismSpec

inline PrismSpec::PrismSpec() {
}
inline PrismSpec::PrismSpec(size_t newTetIndex, int newEdge) :
        tetIndex(newTetIndex), edge(newEdge) {
}
inline PrismSpec::PrismSpec(const PrismSpec& cloneMe) :
        tetIndex(cloneMe.tetIndex), edge(cloneMe.edge) {
}

inline PrismSpec& PrismSpec::operator = (const PrismSpec& cloneMe) {
    tetIndex = cloneMe.tetIndex;
    edge = cloneMe.edge;
    return *this;
}
inline bool PrismSpec::operator == (const PrismSpec& other) const {
    return (tetIndex == other.tetIndex && edge == other.edge);
}
inline bool PrismSpec::operator != (const PrismSpec& other) const {
    return (tetIndex != other.tetIndex || edge != other.edge);
}

} // namespace regina

#endif

