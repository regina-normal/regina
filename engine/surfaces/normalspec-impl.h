
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

/*! \file utilities/normalspec-impl.h
 *  \brief Contains implementations of template classes from
 *  normalsurfaces.h.
 *
 *  This file is automatically included from normalsurfaces.h; there
 *  is no need for end users to include it explicitly.
 */

// Make sure normalsurfaces.h is included first.
#include "surfaces/normalsurfaces.h"

#ifndef __NORMALSPEC_IMPL_H
#ifndef __DOXYGEN
#define __NORMALSPEC_IMPL_H
#endif

namespace regina {

class NSVectorStandard;
class NSVectorQuad;
class NSVectorANStandard;
class NSVectorQuadOct;

/**
 * Constants, typedefs and operations for working with normal (as
 * opposed to almost normal) surfaces.  See the declaration in
 * normalsurfaces.h for further details.
 *
 * For both normal and almost normal surfaces, we refer to different
 * coordinate systems by the generic names \e standard form and
 * \e reduced form.  Standard form is where we store coordinates for all
 * disc types (e.g., the standard tri-quad coordinates for normal
 * surfaces), and reduced form is where we only store coordinates for
 * non-triangular disc types (e.g., the quadrilateral coordinates of
 * Tollefson).
 */
struct REGINA_API NormalSurfaces::NormalSpec {
    /**
     * The underlying class for vectors in standard form.
     */
    typedef NSVectorStandard StandardVector;
    /**
     * The underlying class for vectors in reduced form.
     */
    typedef NSVectorQuad ReducedVector;

    /**
     * Returns the coordinate system constant corresponding to standard form.
     *
     * This has been made a routine (not a constant) to ensure the
     * correct return type.  Perhaps this can be changed to an enum some day...
     */
    inline static NormalCoords standardCoords() {
        return NS_STANDARD;
    }
    /**
     * Returns the coordinate system constant corresponding to reduced form.
     *
     * This has been made a routine (not a constant) to ensure the
     * correct return type.  Perhaps this can be changed to an enum some day...
     */
    inline static NormalCoords reducedCoords() {
        return NS_QUAD;
    }

    /**
     * Returns the number of coordinates per tetrahedron in standard form.
     */
    static const unsigned totalPerTet = 7;
    /**
     * Returns the number of coordinates per tetrahedron in reduced form.
     */
    static const unsigned reducedPerTet = 3;

    /**
     * Returns the total length of a vector in standard form for the
     * given number of tetrahedra.
     */
    inline static size_t stdLen(size_t nTet) {
        return 7 * nTet;
    }
    /**
     * Returns the total length of a vector in reduced form for the
     * given number of tetrahedra.
     */
    inline static size_t redLen(size_t nTet) {
        return 3 * nTet;
    }

    /**
     * Returns the coordinate number in standard form that corresponds
     * to the given disc type within the given tetrahedron.
     */
    inline static size_t stdPos(size_t tet, unsigned discType) {
        return 7 * tet + discType;
    }
    /**
     * Returns the coordinate number in reduced form that corresponds
     * to the given disc type within the given tetrahedron.
     */
    inline static size_t redPos(size_t tet, unsigned discType) {
        return 3 * tet + discType;
    }
};

/**
 * Constants, typedefs and operations for working with almost normal (as
 * opposed to almost normal) surfaces.  See the declaration in
 * normalsurfaces.h for further details.
 *
 * For both normal and almost normal surfaces, we refer to different
 * coordinate systems by the generic names \e standard form and
 * \e reduced form.  Standard form is where we store coordinates for all
 * disc types (e.g., the standard tri-quad-oct coordinates for almost normal
 * surfaces), and reduced form is where we only store coordinates for
 * non-triangular disc types (e.g., quad-oct coordinates for almost
 * normal surfaces).
 */
struct REGINA_API NormalSurfaces::AlmostNormalSpec {
    /**
     * The underlying class for vectors in standard form.
     */
    typedef NSVectorANStandard StandardVector;

    /**
     * The underlying class for vectors in reduced form.
     */
    typedef NSVectorQuadOct ReducedVector;

    /**
     * Returns the coordinate system constant corresponding to standard form.
     *
     * This has been made a routine (not a constant) to ensure the
     * correct return type.  Perhaps this can be changed to an enum some day...
     */
    inline static NormalCoords standardCoords() {
        return NS_AN_STANDARD;
    }

    /**
     * Returns the coordinate system constant corresponding to reduced form.
     *
     * This has been made a routine (not a constant) to ensure the
     * correct return type.  Perhaps this can be changed to an enum some day...
     */
    inline static NormalCoords reducedCoords() {
        return NS_AN_QUAD_OCT;
    }

    /**
     * Returns the number of coordinates per tetrahedron in standard form.
     */
    static const unsigned totalPerTet = 10;
    /**
     * Returns the number of coordinates per tetrahedron in reduced form.
     */
    static const unsigned reducedPerTet = 6;

    /**
     * Returns the total length of a vector in standard form for the
     * given number of tetrahedra.
     */
    inline static size_t stdLen(size_t nTet) {
        return 10 * nTet;
    }
    /**
     * Returns the total length of a vector in reduced form for the
     * given number of tetrahedra.
     */
    inline static size_t redLen(size_t nTet) {
        return 6 * nTet;
    }

    /**
     * Returns the coordinate number in standard form that corresponds
     * to the given disc type within the given tetrahedron.
     */
    inline static size_t stdPos(size_t tet, unsigned discType) {
        return 10 * tet + discType;
    }
    /**
     * Returns the coordinate number in reduced form that corresponds
     * to the given disc type within the given tetrahedron.
     */
    inline static size_t redPos(size_t tet, unsigned discType) {
        return 6 * tet + discType;
    }
};

} // namespace regina

#endif
