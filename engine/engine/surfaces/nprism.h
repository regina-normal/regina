
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file nprism.h
 *  \brief Deals with triangular prisms defined by slicing along normal
 *  quads in a tetrahedron.
 */

#ifndef __NPRISM_H
#ifndef __DOXYGEN
#define __NPRISM_H
#endif

#include <iostream>

namespace regina {

/**
 * \weakgroup surfaces
 * @{
 */

class NNormalSurface;

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
struct NPrismSpec {
    unsigned long tetIndex;
        /**< The index in the triangulation of the tetrahedron
             containing the prism. */
    int edge;
        /**< The edge of the tetrahedron that is contained in this prism. */

    /**
     * Creates a new uninitialised prism specifier.
     */
    NPrismSpec();
    /**
     * Creates a new prism specifier containing the given values.
     *
     * @param newTetIndex the index in the triangulation of the tetrahedron
     * containing the prism.
     * @param newEdge the edge of the tetrahedron that is contained in
     * this prism; this must be between 0 and 5 inclusive.
     */
    NPrismSpec(unsigned long newTetIndex, int newEdge);
    /**
     * Creates a new prism specifier that is a clone of the given specifier.
     *
     * @param cloneMe the prism specifier to clone.
     */
    NPrismSpec(const NPrismSpec& cloneMe);

    /**
     * Copies the values from the given prism specifier into this specifier.
     *
     * @param cloneMe the prism specifier whose values should be copied.
     * @return a reference to this prism specifier.
     */
    NPrismSpec& operator = (const NPrismSpec& cloneMe);
    /**
     * Determines if this and the given prism specifier contain identical
     * information.
     *
     * @param other the prism specifier to compare with this.
     * @return \c true if and only if this and the given prism specifier
     * contain identical information.
     */
    bool operator == (const NPrismSpec& other) const;

    friend std::ostream& operator << (std::ostream& out,
        const NPrismSpec& spec);
};

/**
 * Writes the given prism specifier to the given output stream.
 * The prism specifier will be written as a pair <tt>(tetIndex, edge)</tt>.
 *
 * @param out the output stream to which to write.
 * @param spec the prism specifier to write.
 * @return a reference to \a out.
 */
std::ostream& operator << (std::ostream& out, const NPrismSpec& spec);

/**
 * Represents the set of prisms defined by slicing along all the quads
 * in a particular normal surface.
 *
 * Note that each tetrahedron in the underlying triangulation will supply
 * either zero or two prisms (depending upon whether or not it contains
 * any normal quads).
 *
 * \pre This class should only be used with \e embedded normal surfaces
 * containing no octahedral discs.
 *
 * \warning This class doesn't really do much as yet.
 */
class NPrismSetSurface {
    private:
        signed char* quadType;
            /**< A list of which types of normal quad are contained in which
                 tetrahedra.  Tetrahedra containing no quads at all
                 will have quad type -1 in this array. */

    public:
        /**
         * Creates a new prism set corresponding to the prisms defined
         * by the given normal surface.
         *
         * \pre The given normal surface is embedded and contains no
         * octahedral discs.
         *
         * @param surface the normal surface that defines the prisms in
         * this set.
         */
        NPrismSetSurface(const NNormalSurface& surface);
        /**
         * Destroys this prism set.
         */
        virtual ~NPrismSetSurface();

        /**
         * Returns the quadrilateral type with which the underlying
         * normal surface meets the given tetrahedron.  Note that the
         * surface might contain many quadrilateral discs of this type.
         * However, since the underlying surface is embedded, there
         * cannot be more than one such quadrilateral type.
         *
         * @param tetIndex the index in the triangulation of the
         * tetrahedron in which we are interested; this should be
         * between 0 and NTriangulation::getNumberOfTetrahedra()-1
         * inclusive.
         * @return the quadrilateral type found within this tetrahedron.
         * This is 0, 1 or 2 and represents the same type parameter as is
         * used by NNormalSurface::getQuadCoord().  If the underlying surface
         * does not meet the given tetrahedron in any quadrilateral discs,
         * this routine returns -1.
         */
        signed char getQuadType(unsigned long tetIndex) const;
};

/*@}*/

// Inline functions for NPrismSpec

inline NPrismSpec::NPrismSpec() {
}
inline NPrismSpec::NPrismSpec(unsigned long newTetIndex, int newEdge) :
        tetIndex(newTetIndex), edge(newEdge) {
}
inline NPrismSpec::NPrismSpec(const NPrismSpec& cloneMe) :
        tetIndex(cloneMe.tetIndex), edge(cloneMe.edge) {
}

inline NPrismSpec& NPrismSpec::operator = (const NPrismSpec& cloneMe) {
    tetIndex = cloneMe.tetIndex;
    edge = cloneMe.edge;
    return *this;
}
inline bool NPrismSpec::operator == (const NPrismSpec& other) const {
    return (tetIndex == other.tetIndex && edge == other.edge);
}

// Inline functions for NPrismSetSurface

inline NPrismSetSurface::~NPrismSetSurface() {
    if (quadType)
        delete[] quadType;
}

inline signed char NPrismSetSurface::getQuadType(unsigned long tetIndex)
        const {
    return quadType[tetIndex];
}

} // namespace regina

#endif

