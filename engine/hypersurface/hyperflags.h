
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file hypersurface/hyperflags.h
 *  \brief Defines constants and flags for normal hypersurface enumeration.
 */

#ifndef __REGINA_HYPERFLAGS_H
#ifndef __DOXYGEN
#define __REGINA_HYPERFLAGS_H
#endif

#include "regina-core.h"
#include "utilities/flags.h"

namespace regina {

/**
 * Represents different lists of normal hypersurfaces that might be constructed
 * for a given 4-manifold triangulation.  This enumeration type is used with
 * normal hypersurface enumeration routines, such as the NormalHypersurfaces
 * class constructor.
 *
 * The HyperList enumeration refers to the _contents_ of the list, whereas the
 * HyperAlg enumeration refers to the _algorithm_ used to build it.
 *
 * These values can be combined using the bitwise OR operator (resulting in an
 * object of type `Flags<HyperList>`).  In particular, if a hypersurface
 * enumeration function takes an argument of type `Flags<HyperList>`, then you
 * can pass a single HyperList constant, or a bitwise combination of such
 * constants `(flag1 | flag2)`, or empty braces `{}` to indicate no flags at
 * all (which is equivalent to passing `HyperList::Default`).
 *
 * \ingroup hypersurface
 */
enum class HyperList {
    /**
     * An empty flag, indicating to an enumeration routine that it
     * should use its default behaviour.
     * The numeric value of this flag is zero (i.e., it has no effect
     * when combined with other flags using bitwise OR).
     */
    Default = 0x0000,

    /**
     * Indicates that this list is restricted to properly embedded
     * hypersurfaces only.
     *
     * This flag is incompatible with ImmersedSingular.
     */
    EmbeddedOnly = 0x0001,
    /**
     * Indicates that the scope of this list includes not just properly
     * embedded hypersurfaces, but also immersed and/or branched hypersurfaces.
     *
     * This is no guarantee that the list _contains_ immersed and/or
     * branched hypersurfaces; it merely states that such hypersurfaces
     * have not been explicitly excluded (in particular, the prism
     * constraints have not been enforced).
     *
     * This flag is incompatible with EmbeddedOnly.
     */
    ImmersedSingular = 0x0002,

    /**
     * Indicates a list of all vertex normal hypersurfaces, with respect to
     * the particular normal coordinate system used by the list.
     *
     * This flag is incompatible with Fundamental.
     */
    Vertex = 0x0004,
    /**
     * Indicates a list of all fundamental normal hypersurfaces, with respect to
     * the particular normal coordinate system used by the list.
     *
     * This flag is incompatible with Vertex.
     */
    Fundamental = 0x0008,

    /**
     * Indicates a list that was constructed using an older prerelease version
     * of the 4-manifolds code (prior to Regina 5.0).
     *
     * These prerelease versions did not retain details of how each list was
     * constructed, beyond whether immersed and/or singular hypersurfaces were
     * included.  Therefore no information is available for such lists,
     * other than the presence or absence of the EmbeddedOnly and/or
     * ImmersedSingular flags.
     *
     * If this flag is passed to an enumeration routine, it will be ignored.
     */
    Legacy = 0x4000,
    /**
     * Indicates some other type of list, typically hand-crafted by the
     * user or built by some customised algorithm.
     *
     * If this flag is passed to an enumeration routine, it will be ignored.
     */
    Custom = 0x8000
};

/**
 * A deprecated type alias representing different lists of normal hypersurfaces
 * that might be constructed in a 4-manifold triangulation.
 *
 * \deprecated As of Regina 7.4, HyperListFlags is simply an alias for the
 * enumeration type HyperList.  A bitwise _combination_ of such values will
 * have the type `Flags<HyperList>`, though there is usually no need for end
 * users to explicitly refer to the flags type by name.
 *
 * \ingroup hypersurface
 */
using HyperListFlags [[deprecated]] = HyperList;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal
 * hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperList::Default.
 */
[[deprecated]] inline static constexpr HyperList HS_LIST_DEFAULT =
    HyperList::Default;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal
 * hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperList::EmbeddedOnly.
 */
[[deprecated]] inline static constexpr HyperList HS_EMBEDDED_ONLY =
    HyperList::EmbeddedOnly;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal
 * hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperList::ImmersedSingular.
 */
[[deprecated]] inline static constexpr HyperList HS_IMMERSED_SINGULAR =
    HyperList::ImmersedSingular;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal
 * hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperList::Vertex.
 */
[[deprecated]] inline static constexpr HyperList HS_VERTEX =
    HyperList::Vertex;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal
 * hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperList::Fundamental.
 */
[[deprecated]] inline static constexpr HyperList HS_FUNDAMENTAL =
    HyperList::Fundamental;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal
 * hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperList::Legacy.
 */
[[deprecated]] inline static constexpr HyperList HS_LEGACY =
    HyperList::Legacy;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal
 * hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperList::Custom.
 */
[[deprecated]] inline static constexpr HyperList HS_CUSTOM =
    HyperList::Custom;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * \param lhs the first flag to combine.
 * \param rhs the second flag to combine.
 * \return the combination of both flags.
 *
 * \ingroup hypersurface
 */
inline Flags<HyperList> operator | (HyperList lhs, HyperList rhs) {
    return Flags<HyperList>(lhs) | rhs;
}

/**
 * Represents options and variants of algorithms for enumerating various
 * types of normal hypersurfaces in 4-manifold triangulations.  This
 * enumeration type is used with normal hypersurface enumeration routines,
 * such as the NormalHypersurfaces class constructor.
 *
 * These values can be combined using the bitwise OR operator (resulting in an
 * object of type `Flags<HyperAlg>`).  In particular, if a hypersurface
 * enumeration function takes an argument of type `Flags<HyperAlg>`, then you
 * can pass a single HyperAlg constant, or a bitwise combination of such
 * constants `(flag1 | flag2)`, or empty braces `{}` to indicate no flags at
 * all (which is equivalent to passing `HyperAlg::Default`).
 *
 * \ingroup hypersurface
 */
enum class HyperAlg {
    /**
     * An empty flag, indicating to an enumeration routine that it
     * should use its default behaviour.
     * The numeric value of this flag is zero (i.e., it has no effect
     * when combined with other flags using bitwise OR).
     */
    Default = 0x0000,

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
    VertexDD = 0x0020,

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
     * This flag is incompatible with HilbertDual.
     */
    HilbertPrimal = 0x0100,
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
     * This flag is incompatible with HilbertPrimal.
     */
    HilbertDual = 0x0200,

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
    Legacy = 0x4000,
    /**
     * Indicates that a normal hypersurface list was built using a customised
     * algorithm.  In such cases, no further details on the algorithm are
     * available.
     *
     * If this flag is passed to an enumeration algorithm, it will be ignored.
     */
    Custom = 0x8000
};

/**
 * A deprecated type alias representing options and variants of algorithms for
 * enumerating normal hypersurfaces in 4-manifold triangulations.
 *
 * \deprecated As of Regina 7.4, HyperAlgFlags is simply an alias for the
 * enumeration type HyperAlg.  A bitwise _combination_ of such values will
 * have the type `Flags<HyperAlg>`, though there is usually no need for end
 * users to explicitly refer to the flags type by name.
 *
 * \ingroup hypersurface
 */
using HyperAlgFlags [[deprecated]] = HyperAlg;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperAlg::Default.
 */
[[deprecated]] inline static constexpr HyperAlg HS_ALG_DEFAULT =
    HyperAlg::Default;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperAlg::VertexDD.
 */
[[deprecated]] inline static constexpr HyperAlg HS_VERTEX_DD =
    HyperAlg::VertexDD;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperAlg::HilbertPrimal.
 */
[[deprecated]] inline static constexpr HyperAlg HS_HILBERT_PRIMAL =
    HyperAlg::HilbertPrimal;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperAlg::HilbertDual.
 */
[[deprecated]] inline static constexpr HyperAlg HS_HILBERT_DUAL =
    HyperAlg::HilbertDual;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperAlg::Legacy.
 */
[[deprecated]] inline static constexpr HyperAlg HS_ALG_LEGACY =
    HyperAlg::Legacy;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal hypersurfaces in a 4-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * HyperAlg::Custom.
 */
[[deprecated]] inline static constexpr HyperAlg HS_ALG_CUSTOM =
    HyperAlg::Custom;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * \param lhs the first flag to combine.
 * \param rhs the second flag to combine.
 * \return the combination of both flags.
 *
 * \ingroup hypersurface
 */
inline Flags<HyperAlg> operator | (HyperAlg lhs, HyperAlg rhs) {
    return Flags<HyperAlg>(lhs) | rhs;
}

} // namespace regina

#endif

