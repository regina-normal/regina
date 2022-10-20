
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

/*! \file census/purgeflags.h
 *  \brief Defines constants and flags for census generation.
 */

#ifndef __REGINA_PURGEFLAGS_H
#ifndef __DOXYGEN
#define __REGINA_PURGEFLAGS_H
#endif

#include "regina-core.h"
#include "utilities/flags.h"

namespace regina {

/**
 * Represents different classes of triangulations that may be ignored by
 * census generation algorithms.
 *
 * Excluding a triangulation from a census is not a promise: for example,
 * a census that uses the flag PURGE_NON_MINIMAL might include some
 * non-minimal triangulations and exclude others.
 *
 * However, \e including a triangulation \e is a promise: for example, a
 * census that uses the flag PURGE_NON_MINIMAL will promise to include
 * every \e minimal triangulations.
 *
 * These flags can be combined using the bitwise OR operator.
 * See (for example) the GluingPermSearcher<3> constructor documentation for
 * further details on how these flags are used.
 *
 * \ingroup census
 */
enum CensusPurgeFlags {
    /**
     * Indicates that no triangulations should be ignored.
     */
    PURGE_NONE = 0x00,
    /**
     * Indicates that non-minimal triangulations may be ignored.
     */
    PURGE_NON_MINIMAL = 0x01,
    /**
     * Indicates that any triangulation that is not prime (i.e.,
     * can be written as a non-trivial connected sum) and any bounded
     * triangulation that is reducible over a disc may be ignored.
     */
    PURGE_NON_PRIME = 0x02,
    /**
     * Indicates that any triangulation that is not prime (i.e.,
     * can be written as a non-trivial connected sum), any
     * bounded triangulation that is reducible over a disc and
     * any triangulation that is non-minimal may be ignored.
     * Note that this is simply a combination of the constants
     * \a PURGE_NON_MINIMAL and \a PURGE_NON_PRIME.
     */
    PURGE_NON_MINIMAL_PRIME = 0x03,
    /**
     * Indicates that any triangulation that is not a minimal ideal
     * triangulation of a cusped finite-volume hyperbolic 3-manifold
     * may be ignored.
     */
    PURGE_NON_MINIMAL_HYP = 0x09,
    /**
     * Indicates that any triangulation containing an embedded
     * two-sided projective plane may be ignored.
     */
    PURGE_P2_REDUCIBLE = 0x04
};

/**
 * A combination of flags for census generation.
 *
 * If a function requires a CensusPurge object as an argument, you can
 * pass a single CensusPurgeFlags constant, or a combination of such
 * constants using the bitwise OR operator, or empty braces {} to indicate
 * no flags at all.
 *
 * \ingroup census
 */
using CensusPurge = regina::Flags<CensusPurgeFlags>;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * \param lhs the first flag to combine.
 * \param rhs the second flag to combine.
 * \return the combination of both flags.
 *
 * \ingroup census
 */
inline CensusPurge operator | (CensusPurgeFlags lhs, CensusPurgeFlags rhs) {
    return CensusPurge(lhs) | rhs;
}

} // namespace regina

#endif

