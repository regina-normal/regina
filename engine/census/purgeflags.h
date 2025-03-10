
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 * census generation algorithms.  This enumeration type is used with census
 * generation routines, such as those found in GluingPermSearcher<3>.
 *
 * Excluding a triangulation from a census is not a promise: for example,
 * a census that uses the flag \a NonMinimal might include some
 * non-minimal triangulations and exclude others.
 *
 * However, _including_ a triangulation _is_ a promise: for example, a
 * census that uses the flag \a NonMinimal will promise to include
 * every _minimal_ triangulation.
 *
 * These values can be combined using the bitwise OR operator (resulting in an
 * object of type `Flags<CensusPurge>`).  In particular, if a census generation
 * function takes an argument of type `Flags<CensusPurge>`, then you can pass a
 * single CensusPurge constant, or a bitwise combination of such constants
 * `(flag1 | flag2)`, or empty braces `{}` to indicate no flags at all
 * (which is equivalent to passing `CensusPurge::None`).
 *
 * \ingroup census
 */
enum class CensusPurge {
    /**
     * Indicates that no triangulations should be ignored.
     *
     * \python This constant is called `Nil`, since `None` is a reserved word
     * in Python.
     */
    None = 0x00,
    /**
     * Indicates that non-minimal triangulations may be ignored.
     */
    NonMinimal = 0x01,
    /**
     * Indicates that any triangulation that is not prime (i.e.,
     * can be written as a non-trivial connected sum) and any bounded
     * triangulation that is reducible over a disc may be ignored.
     */
    NonPrime = 0x02,
    /**
     * Indicates that any triangulation that is not prime (i.e.,
     * can be written as a non-trivial connected sum), any
     * bounded triangulation that is reducible over a disc and
     * any triangulation that is non-minimal may be ignored.
     * Note that this is simply a combination of the constants
     * \a NonMinimal and \a NonPrime.
     */
    NonMinimalPrime = 0x03,
    /**
     * Indicates that any triangulation that is not a minimal ideal
     * triangulation of a cusped finite-volume hyperbolic 3-manifold
     * may be ignored.
     */
    NonMinimalHyp = 0x09,
    /**
     * Indicates that any triangulation containing an embedded
     * two-sided projective plane may be ignored.
     */
    P2Reducible = 0x04
};

/**
 * A deprecated type alias representing a class of triangulations that may be
 * ignored by census generation algorithms.
 *
 * \deprecated As of Regina 7.4, CensusPurgeFlags is simply an alias for
 * the enumeration type CensusPurge.  A bitwise _combination_ of such values
 * will have the type `Flags<CensusPurge>`, though there is usually no need
 * for end users to explicitly refer to the flags type by name.
 *
 * \ingroup census
 */
using CensusPurgeFlags [[deprecated]] = CensusPurge;

/**
 * A deprecated constant indicating a class of triangulations that may be
 * ignored by census generation algorithms.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * CensusPurge::None.
 */
[[deprecated]] inline static constexpr CensusPurge PURGE_NONE =
    CensusPurge::None;

/**
 * A deprecated constant indicating a class of triangulations that may be
 * ignored by census generation algorithms.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * CensusPurge::NonMinimal.
 */
[[deprecated]] inline static constexpr CensusPurge PURGE_NON_MINIMAL =
    CensusPurge::NonMinimal;

/**
 * A deprecated constant indicating a class of triangulations that may be
 * ignored by census generation algorithms.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * CensusPurge::NonPrime.
 */
[[deprecated]] inline static constexpr CensusPurge PURGE_NON_PRIME =
    CensusPurge::NonPrime;

/**
 * A deprecated constant indicating a class of triangulations that may be
 * ignored by census generation algorithms.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * CensusPurge::NonMinimalPrime.
 */
[[deprecated]] inline static constexpr CensusPurge PURGE_NON_MINIMAL_PRIME =
    CensusPurge::NonMinimalPrime;

/**
 * A deprecated constant indicating a class of triangulations that may be
 * ignored by census generation algorithms.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * CensusPurge::NonMinimalHyp.
 */
[[deprecated]] inline static constexpr CensusPurge PURGE_NON_MINIMAL_HYP =
    CensusPurge::NonMinimalHyp;

/**
 * A deprecated constant indicating a class of triangulations that may be
 * ignored by census generation algorithms.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * CensusPurge::P2Reducible.
 */
[[deprecated]] inline static constexpr CensusPurge PURGE_P2_REDUCIBLE =
    CensusPurge::P2Reducible;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * \param lhs the first flag to combine.
 * \param rhs the second flag to combine.
 * \return the combination of both flags.
 *
 * \ingroup census
 */
inline Flags<CensusPurge> operator | (CensusPurge lhs, CensusPurge rhs) {
    return Flags<CensusPurge>(lhs) | rhs;
}

} // namespace regina

#endif

