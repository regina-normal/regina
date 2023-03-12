
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

/*! \file angle/angleflags.h
 *  \brief Defines constants and flags for angle structure enumeration.
 */

#ifndef __REGINA_ANGLEFLAGS_H
#ifndef __DOXYGEN
#define __REGINA_ANGLEFLAGS_H
#endif

#include "regina-core.h"
#include "utilities/flags.h"

namespace regina {

/**
 * Represents options and variants of algorithms for enumerating various
 * types of angle structures on 3-manifold triangulations.
 *
 * These options can be combined using the bitwise OR operator, and then
 * passed to enumeration routines such as the AngleStructures
 * class constructor.
 *
 * \ingroup angle
 */
enum AngleAlgFlags {
    /**
     * An empty flag, indicating to an enumeration routine that it
     * should use its default behaviour.
     * The numeric value of this flag is zero (i.e., it has no effect
     * when combined with other flags using bitwise OR).
     */
    AS_ALG_DEFAULT = 0x0000,
    /**
     * When enumerating taut angle structures, this flag indicates that
     * the tree traversal algorithm should be used.
     *
     * This algorithm is based on linear and integer programming
     * techniques, and has many desirable properties including a
     * relatively low overhead.  Enumeration algorithms will use it if
     * possible unless a different method is explicitly requested.
     *
     * This is a variant of the tree traversal algorithm from B. A. Burton
     * and M. Ozlen, "A tree traversal algorithm for decision problems in
     * knot theory and 3-manifold topology", Algorithmica 65 (2013),
     * pp. 772-801.
     *
     * This flag is incompatible with AS_ALG_DD.
     */
    AS_ALG_TREE = 0x0010,
    /**
     * When enumerating vertex or taut angle structures, this flag indicates
     * that a modified double description method should be used.
     *
     * This is currently the only supported algorithm for enumerating
     * all vertex angle structures (not just taut structures).
     *
     * This flag is incompatible with AS_ALG_TREE.
     */
    AS_ALG_DD = 0x0020,
    /**
     * Indicates that an angle structure list was enumerated using an
     * older version of Regina (6.0.1 or earlier).
     *
     * These older versions did not retain details of the algorithm
     * used to build each list, and so in such cases no further
     * algorithmic information is available.
     *
     * If this flag is passed to an enumeration algorithm, it will be ignored.
     */
    AS_ALG_LEGACY = 0x4000,
    /**
     * Indicates that an angle structure list was built using a customised
     * algorithm.  In such cases, no further details on the algorithm are
     * available.
     *
     * If this flag is passed to an enumeration algorithm, it will be ignored.
     */
    AS_ALG_CUSTOM = 0x8000
};

/**
 * A combination of flags for angle structure enumeration algorithms.
 *
 * If a function requires an AngleAlg object as an argument, you can
 * pass a single AngleAlgFlags constant, or a combination of such
 * constants using the bitwise OR operator, or empty braces {} to indicate
 * no flags at all.
 *
 * \ingroup angle
 */
using AngleAlg = regina::Flags<AngleAlgFlags>;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * \param lhs the first flag to combine.
 * \param rhs the second flag to combine.
 * \return the combination of both flags.
 *
 * \ingroup angle
 */
inline AngleAlg operator | (AngleAlgFlags lhs, AngleAlgFlags rhs) {
    return AngleAlg(lhs) | rhs;
}

} // namespace regina

#endif

