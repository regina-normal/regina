
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "surfaces/nnormalsurfacelist.h"
#include "surfaces/nsstandard.h"
#include "surfaces/nsquad.h"
#include "surfaces/nsanstandard.h"
#include "surfaces/nsquadoct.h"

namespace regina {

/**
 * Constants, typedefs and operations for working with normal (as
 * opposed to almost normal) surfaces.  See the declaration in
 * nnormalsurfacelist.h for further details.
 *
 * For both normal and almost normal surfaces, we refer to different
 * coordinate systems by the generic names \e standard form and
 * \e reduced form.  Standard form is where we store coordinates for all
 * disc types (e.g., the standard tri-quad coordinates for normal
 * surfaces), and reduced form is where we only store coordinates for
 * non-triangular disc types (e.g., the quadrilateral coordinates of
 * Tollefson).
 */
struct NNormalSurfaceList::NormalSpec {
    /**
     * The underlying class for vectors in standard form.
     */
    typedef NNormalSurfaceVectorStandard StandardVector;
    /**
     * The underlying class for vectors in reduced form.
     */
    typedef NNormalSurfaceVectorQuad ReducedVector;

    /**
     * Returns the flavour constant in NNormalSurfaceList corresponding to
     * standard form.
     *
     * This has been made a routine (not a constant) to avoid nasty
     * initialisation problems (since the "value" we seek is in fact a static
     * constant in another class that also needs initialising).  The
     * real solution is to use an enum in NNormalSurfaceList.. some day.
     */
    inline static int standardFlavour() {
        return NNormalSurfaceList::STANDARD;
    }
    /**
     * Returns the flavour constant in NNormalSurfaceList corresponding to
     * reduced form.  See above for why this is a routine and not a constant.
     */
    inline static int reducedFlavour() {
        return NNormalSurfaceList::QUAD;
    }

    /**
     * Returns the number of coordinates per tetrahedron in standard form.
     */
    static const unsigned totalCoords = 7;
    /**
     * Returns the number of coordinates per tetrahedron in reduced form.
     */
    static const unsigned reducedCoords = 3;

    /**
     * Returns the total length of a vector in standard form for the
     * given number of tetrahedra.
     */
    inline static unsigned stdLen(unsigned nTet) {
        return 7 * nTet;
    }
    /**
     * Returns the total length of a vector in reduced form for the
     * given number of tetrahedra.
     */
    inline static unsigned redLen(unsigned nTet) {
        return 3 * nTet;
    }

    /**
     * Returns the coordinate number in standard form that corresponds
     * to the given disc type within the given tetrahedron.
     */
    inline static unsigned stdPos(unsigned tet, unsigned discType) {
        return 7 * tet + discType;
    }
    /**
     * Returns the coordinate number in reduced form that corresponds
     * to the given disc type within the given tetrahedron.
     */
    inline static unsigned redPos(unsigned tet, unsigned discType) {
        return 3 * tet + discType;
    }
};

/**
 * Constants, typedefs and operations for working with almost normal (as
 * opposed to almost normal) surfaces.  See the declaration in
 * nnormalsurfacelist.h for further details.
 *
 * For both normal and almost normal surfaces, we refer to different
 * coordinate systems by the generic names \e standard form and
 * \e reduced form.  Standard form is where we store coordinates for all
 * disc types (e.g., the standard tri-quad-oct coordinates for almost normal
 * surfaces), and reduced form is where we only store coordinates for
 * non-triangular disc types (e.g., quad-oct coordinates for almost
 * normal surfaces).
 */
struct NNormalSurfaceList::AlmostNormalSpec {
    /**
     * The underlying class for vectors in standard form.
     */
    typedef NNormalSurfaceVectorANStandard StandardVector;

    /**
     * The underlying class for vectors in reduced form.
     */
    typedef NNormalSurfaceVectorQuadOct ReducedVector;

    /**
     * Returns the flavour constant in NNormalSurfaceList corresponding to
     * standard form.
     *
     * This has been made a routine (not a constant) to avoid nasty
     * initialisation problems (since the "value" we seek is in fact a static
     * constant in another class that also needs initialising).  The
     * real solution is to use an enum in NNormalSurfaceList.. some day.
     */
    inline static int standardFlavour() {
        return NNormalSurfaceList::AN_STANDARD;
    }

    /**
     * Returns the flavour constant in NNormalSurfaceList corresponding to
     * reduced form.
     */
    inline static int reducedFlavour() {
        return NNormalSurfaceList::AN_QUAD_OCT;
    }

    /**
     * Returns the number of coordinates per tetrahedron in standard form.
     */
    static const unsigned totalCoords = 10;
    /**
     * Returns the number of coordinates per tetrahedron in reduced form.
     */
    static const unsigned reducedCoords = 6;

    /**
     * Returns the total length of a vector in standard form for the
     * given number of tetrahedra.
     */
    inline static unsigned stdLen(unsigned nTet) {
        return 10 * nTet;
    }
    /**
     * Returns the total length of a vector in reduced form for the
     * given number of tetrahedra.
     */
    inline static unsigned redLen(unsigned nTet) {
        return 6 * nTet;
    }

    /**
     * Returns the coordinate number in standard form that corresponds
     * to the given disc type within the given tetrahedron.
     */
    inline static unsigned stdPos(unsigned tet, unsigned discType) {
        return 10 * tet + discType;
    }
    /**
     * Returns the coordinate number in reduced form that corresponds
     * to the given disc type within the given tetrahedron.
     */
    inline static unsigned redPos(unsigned tet, unsigned discType) {
        return 6 * tet + discType;
    }
};

} // namespace regina

