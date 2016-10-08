
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

/*! \file hypersurface/hyperflags.h
 *  \brief Defines constants and flags for normal hypersurface enumeration.
 */

#ifndef __HYPERFLAGS_H
#ifndef __DOXYGEN
#define __HYPERFLAGS_H
#endif

#include "regina-core.h"
#include "utilities/flags.h"

namespace regina {

/**
 * \weakgroup hypersurface
 * @{
 */

/**
 * Represents different lists of normal hypersurfaces that might be constructed
 * for a given 4-manifold triangulation.
 *
 * The HyperList enumeration refers to the \e contents of the list,
 * whereas the HyperAlgFlags enumeration refers to the \e algorithm
 * used to build it.
 *
 * \ifacespython The values in this enumeration type are present, but
 * they are treated by Python as HyperList objects (and they can be
 * combined and/or queried as such).
 * The underlying enumeration type is not exposed to Python.
 */
enum HyperListFlags {
    /**
     * An empty flag, indicating to an enumeration routine that it
     * should use its default behaviour.
     * The numeric value of this flag is zero (i.e., it has no effect
     * when combined with other flags using bitwise OR).
     */
    HS_LIST_DEFAULT = 0x0000,

    /**
     * Indicates that this list is restricted to properly embedded
     * hypersurfaces only.
     *
     * This flag is incompatible with HS_IMMERSED_SINGULAR.
     */
    HS_EMBEDDED_ONLY = 0x0001,
    /**
     * Indicates that the scope of this list includes not just properly
     * embedded hypersurfaces, but also immersed and/or branched hypersurfaces.
     *
     * This is no guarantee that the list \e contains immersed and/or
     * branched hypersurfaces; it merely states that such hypersurfaces
     * have not been explicitly excluded (in particular, the prism
     * constraints have not been enforced).
     *
     * This flag is incompatible with HS_EMBEDDED_ONLY.
     */
    HS_IMMERSED_SINGULAR = 0x0002,

    /**
     * Indicates a list of all vertex normal hypersurfaces, with respect to
     * the particular normal coordinate system used by the list.
     *
     * This flag is incompatible with HS_FUNDAMENTAL.
     */
    HS_VERTEX = 0x0004,
    /**
     * Indicates a list of all fundamental normal hypersurfaces, with respect to
     * the particular normal coordinate system used by the list.
     *
     * This flag is incompatible with HS_VERTEX.
     */
    HS_FUNDAMENTAL = 0x0008,

    /**
     * Indicates a list that was constructed using an older prerelease version
     * of the 4-manifolds code (prior to Regina 5.0).
     *
     * These prerelease versions did not retain details of how each list was
     * constructed, beyond whether immersed and/or singular hypersurfaces were
     * included.  Therefore no information is available for such lists,
     * other than the presence or absence of the HS_EMBEDDED_ONLY and/or
     * HS_IMMERSED_SINGULAR flags.
     *
     * If this flag is passed to an enumeration routine, it will be ignored.
     */
    HS_LEGACY = 0x4000,
    /**
     * Indicates some other type of list, typically hand-crafted by the
     * user or built by some customised algorithm.
     *
     * If this flag is passed to an enumeration routine, it will be ignored.
     */
    HS_CUSTOM = 0x8000
};

/**
 * A combination of flags for types of normal hypersurface lists.
 *
 * \ifacespython This is present, and all values in the HyperListFlags
 * enumeration type are treated as members of this HyperList class.
 */
typedef regina::Flags<HyperListFlags> HyperList;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * @param lhs the first flag to combine.
 * @param rhs the second flag to combine.
 * @return the combination of both flags.
 */
inline HyperList operator | (HyperListFlags lhs, HyperListFlags rhs) {
    return HyperList(lhs) | rhs;
}

/**
 * Represents options and variants of algorithms for enumerating various
 * types of normal hypersurfaces in 4-manifold triangulations.
 *
 * These options are typically combined in a bitwise fashion using the
 * HyperAlg type, and then passed to enumeration routines such as
 * NormalHypersurfaces::enumerate().
 *
 * \ifacespython The values in this enumeration type are present, but
 * they are treated by Python as HyperList objects (and they can be
 * combined and/or queried as such).
 * The underlying enumeration type is not exposed to Python.
 */
enum HyperAlgFlags {
    /**
     * An empty flag, indicating to an enumeration routine that it
     * should use its default behaviour.
     * The numeric value of this flag is zero (i.e., it has no effect
     * when combined with other flags using bitwise OR).
     */
    HS_ALG_DEFAULT = 0x0000,

    /**
     * When enumerating vertex normal hypersurfaces,
     * this flag indicates that a modified double description method
     * should be used.
     *
     * This algorithm can suffer from a combinatorial explosion with
     * larger problems, leading to extremely large time and memory
     * footprints.  Users should only request this if they have some
     * specialised need.
     *
     * Having said that: this is the only algorithm that Regina currently
     * supports for 4-dimensional vertex enumeration (unlike the 3-dimensional
     * case, where several alternative algorithms are available).
     * Therefore Regina will use the double description method regardless.
     * Alternative algorithms will be implemented in future versions of Regina.
     *
     * For details on the modified double description method, see
     * B. A. Burton, "Optimizing the double description method for
     * normal surface enumeration", Mathematics of Computation
     * 79 (2010), pp. 453-484.
     */
    HS_VERTEX_DD = 0x0020,

    /**
     * When enumerating fundamental normal hypersurfaces,
     * this flag indicates that the primal method should be used for
     * enumerating a Hilbert basis.
     *
     * The primal method is recommended, and enumeration algorithms will
     * use it if possible unless a different method is explicitly requested.
     * This method uses code from Normaliz for parts of its processing.
     *
     * For details and comparisons of the various options for enumerating
     * fundamental normal surfaces, see B. A. Burton, "Enumerating
     * fundamental normal surfaces: Algorithms, experiments and invariants",
     * ALENEX 2014: Proceedings of the Meeting on Algorithm
     * Engineering & Experiments, SIAM, 2014, pp. 112-124.
     *
     * This flag is incompatible with HS_HILBERT_DUAL.
     */
    HS_HILBERT_PRIMAL = 0x0100,
    /**
     * When enumerating fundamental normal hypersurfaces,
     * this flag indicates that the dual method should be used for
     * enumerating a Hilbert basis.
     *
     * The dual method is fast (like the primal method), but its
     * performance is highly variable; for this reason the primal method
     * is recommended instead.
     * This method does not make use of Normaliz, and is the recommended
     * method for situations in which Normaliz is not available for some
     * reason.
     *
     * For details and comparisons of the various options for enumerating
     * fundamental normal surfaces, see B. A. Burton, "Enumerating
     * fundamental normal surfaces: Algorithms, experiments and invariants",
     * ALENEX 2014: Proceedings of the Meeting on Algorithm
     * Engineering & Experiments, SIAM, 2014, pp. 112-124.
     *
     * This flag is incompatible with HS_HILBERT_PRIMAL.
     */
    HS_HILBERT_DUAL = 0x0200,

    /**
     * Indicates that a normal hypersurface list was enumerated using an
     * older prerelease version of the 4-manifolds code (prior to Regina 5.0).
     *
     * These prerelease versions did not retain details of the algorithm
     * used to build each list, and so in such cases no further
     * algorithmic information is available.
     *
     * If this flag is passed to an enumeration algorithm, it will be ignored.
     */
    HS_ALG_LEGACY = 0x4000,
    /**
     * Indicates that a normal hypersurface list was built using a customised
     * algorithm.  In such cases, no further details on the algorithm are
     * available.
     *
     * If this flag is passed to an enumeration algorithm, it will be ignored.
     */
    HS_ALG_CUSTOM = 0x8000
};

/**
 * A combination of flags for types of normal surface lists.
 *
 * \ifacespython This is present, and all values in the HyperAlgFlags
 * enumeration type are treated as members of this HyperAlg class.
 */
typedef regina::Flags<HyperAlgFlags> HyperAlg;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * @param lhs the first flag to combine.
 * @param rhs the second flag to combine.
 * @return the combination of both flags.
 */
inline HyperAlg operator | (HyperAlgFlags lhs, HyperAlgFlags rhs) {
    return HyperAlg(lhs) | rhs;
}

/*@}*/

} // namespace regina

#endif

