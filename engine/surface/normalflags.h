
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

/*! \file surface/normalflags.h
 *  \brief Defines constants and flags for normal surface enumeration.
 */

#ifndef __REGINA_NORMALFLAGS_H
#ifndef __DOXYGEN
#define __REGINA_NORMALFLAGS_H
#endif

#include "regina-core.h"
#include "utilities/flags.h"

namespace regina {

/**
 * Represents different lists of normal surfaces that might be constructed
 * for a given 3-manifold triangulation.  This enumeration type is used with
 * normal surface enumeration routines, such as the NormalSurfaces class
 * constructor.
 *
 * The NormalList enumeration refers to the _contents_ of the list, whereas
 * the NormalAlg enumeration refers to the _algorithm_ used to build it.
 *
 * These values can be combined using the bitwise OR operator (resulting in an
 * object of type `Flags<NormalList>`).  In particular, if a hypersurface
 * enumeration function takes an argument of type `Flags<NormalList>`, then you
 * can pass a single NormalList constant, or a bitwise combination of such
 * constants `(flag1 | flag2)`, or empty braces `{}` to indicate no flags at
 * all (which is equivalent to passing `NormalList::Default`).
 *
 * \ingroup surfaces
 */
enum class NormalList {
    /**
     * An empty flag, indicating to an enumeration routine that it
     * should use its default behaviour.
     * The numeric value of this flag is zero (i.e., it has no effect
     * when combined with other flags using bitwise OR).
     */
    Default = 0x0000,

    /**
     * Indicates that this list is restricted to properly embedded
     * surfaces only.
     *
     * This flag is incompatible with ImmersedSingular.
     */
    EmbeddedOnly = 0x0001,
    /**
     * Indicates that the scope of this list includes not just properly
     * embedded surfaces, but also immersed and/or branched surfaces.
     *
     * This is no guarantee that the list _contains_ immersed and/or
     * branched surfaces; it merely states that such surfaces have not
     * been explicitly excluded (in particular, the quadrilateral
     * constraints have not been enforced).
     *
     * This flag is incompatible with EmbeddedOnly.
     */
    ImmersedSingular = 0x0002,

    /**
     * Indicates a list of all vertex normal surfaces, with respect to
     * the particular normal coordinate system used by the list.
     *
     * This flag is incompatible with Fundamental.
     */
    Vertex = 0x0004,
    /**
     * Indicates a list of all fundamental normal surfaces, with respect to
     * the particular normal coordinate system used by the list.
     *
     * This flag is incompatible with Vertex.
     */
    Fundamental = 0x0008,

    /**
     * Indicates a list that was constructed using an old version of
     * Regina (4.93 or earlier).
     *
     * These older versions did not retain details of how each list was
     * constructed, beyond whether immersed and/or singular surfaces were
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
 * A deprecated type alias representing different lists of normal surfaces
 * that might be constructed in a 3-manifold triangulation.
 *
 * \deprecated As of Regina 7.4, NormalListFlags is simply an alias for the
 * enumeration type NormalList.  A bitwise _combination_ of such values will
 * have the type `Flags<NormalList>`, though there is usually no need for end
 * users to explicitly refer to the flags type by name.
 *
 * \ingroup surfaces
 */
using NormalListFlags [[deprecated]] = NormalList;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal surfaces
 * in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalList::Default.
 */
[[deprecated]] inline static constexpr NormalList NS_LIST_DEFAULT =
    NormalList::Default;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal surfaces
 * in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalList::EmbeddedOnly.
 */
[[deprecated]] inline static constexpr NormalList NS_EMBEDDED_ONLY =
    NormalList::EmbeddedOnly;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal surfaces
 * in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalList::ImmersedSingular.
 */
[[deprecated]] inline static constexpr NormalList NS_IMMERSED_SINGULAR =
    NormalList::ImmersedSingular;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal surfaces
 * in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalList::Vertex.
 */
[[deprecated]] inline static constexpr NormalList NS_VERTEX =
    NormalList::Vertex;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal surfaces
 * in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalList::Fundamental.
 */
[[deprecated]] inline static constexpr NormalList NS_FUNDAMENTAL =
    NormalList::Fundamental;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal surfaces
 * in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalList::Legacy.
 */
[[deprecated]] inline static constexpr NormalList NS_LEGACY =
    NormalList::Legacy;

/**
 * A deprecated constant indicating some aspect(s) of a list of normal surfaces
 * in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalList::Custom.
 */
[[deprecated]] inline static constexpr NormalList NS_CUSTOM =
    NormalList::Custom;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * \param lhs the first flag to combine.
 * \param rhs the second flag to combine.
 * \return the combination of both flags.
 *
 * \ingroup surfaces
 */
inline Flags<NormalList> operator | (NormalList lhs, NormalList rhs) {
    return Flags<NormalList>(lhs) | rhs;
}

/**
 * Represents options and variants of algorithms for enumerating various
 * types of normal surfaces in 3-manifold triangulations.  This enumeration
 * type is used with normal surface enumeration routines, such as the
 * NormalSurfaces class constructor.
 *
 * These values can be combined using the bitwise OR operator (resulting in an
 * object of type `Flags<NormalAlg>`).  In particular, if a hypersurface
 * enumeration function takes an argument of type `Flags<NormalAlg>`, then you
 * can pass a single NormalAlg constant, or a bitwise combination of such
 * constants `(flag1 | flag2)`, or empty braces `{}` to indicate no flags at
 * all (which is equivalent to passing `NormalAlg::Default`).
 *
 * \ingroup surfaces
 */
enum class NormalAlg {
    /**
     * An empty flag, indicating to an enumeration routine that it
     * should use its default behaviour.
     * The numeric value of this flag is zero (i.e., it has no effect
     * when combined with other flags using bitwise OR).
     */
    Default = 0x0000,

    /**
     * When enumerating in standard normal or almost normal coordinates,
     * this flag indicates that the algorithm should first enumerate in
     * quadrilateral or quadrilateral-octagon coordinates, and then
     * expand this "reduced" solution set to the (typically larger)
     * "standard" solution set.
     *
     * This is typically much faster than a direct enumeration in
     * standard normal or almost normal coordinates, and enumeration
     * routines will use this option where possible unless explicitly
     * requested not to (via the flag VertexStandardDirect).
     *
     * For an explanation of this procedure, see B. A. Burton,
     * "Converting between quadrilateral and standard solution sets in
     * normal surface theory", Algebr. Geom. Topol. 9 (2009), 2121-2174.
     *
     * This flag is incompatible with VertexStandardDirect.
     */
    VertexViaReduced = 0x0001,

    /**
     * When enumerating in standard normal or almost normal coordinates,
     * this flag indicates that the algorithm should work directly in
     * that coordinate system, and should not go via the "reduced"
     * (quadrilateral or quadrilateral-octagon) coordinate system.
     *
     * This is typically _much_ slower than going via the reduced
     * system, and users should only request this if they have a
     * specialised need.  See VertexViaReduced for further information.
     *
     * This flag is incompatible with VertexViaReduced.
     */
    VertexStandardDirect = 0x0002,

    /**
     * When enumerating vertex normal surfaces,
     * this flag indicates that the tree traversal algorithm should be
     * used.
     *
     * This algorithm is based on linear and integer programming
     * techniques, and has many desirable properties including a
     * relatively low overhead.  Enumeration algorithms will use it if
     * possible unless a different method is explicitly requested.
     *
     * For details on the tree traversal algorithm, see B. A. Burton and
     * M. Ozlen, "A tree traversal algorithm for decision problems in
     * knot theory and 3-manifold topology", Algorithmica 65 (2013),
     * pp. 772-801.
     *
     * This flag is incompatible with VertexDD.
     */
    VertexTree = 0x0010,
    /**
     * When enumerating vertex normal surfaces,
     * this flag indicates that a modified double description method
     * should be used.
     *
     * This algorithm can suffer from a combinatorial explosion with
     * larger problems, leading to extremely large time and memory
     * footprints.  Users should only request this if they have some
     * specialised need.
     *
     * For details on the modified double description method, see
     * B. A. Burton, "Optimizing the double description method for
     * normal surface enumeration", Mathematics of Computation
     * 79 (2010), pp. 453-484.
     *
     * This flag is incompatible with VertexTree.
     */
    VertexDD = 0x0020,

    /**
     * When enumerating fundamental normal surfaces,
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
     * This flag is incompatible with HilbertDual, HilbertCD and
     * HilbertFullCone.
     */
    HilbertPrimal = 0x0100,
    /**
     * When enumerating fundamental normal surfaces,
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
     * This flag is incompatible with HilbertPrimal, HilbertCD and
     * HilbertFullCone.
     */
    HilbertDual = 0x0200,
    /**
     * When enumerating fundamental normal surfaces,
     * this flag indicates that a modified Contejean-Devie procedure should
     * be used for enumerating a Hilbert basis.
     *
     * The Contejean-Devie procedure is typically _much_ slower
     * than either the primal or dual method, and users should only
     * request it if they have some specialised need.
     *
     * For details and comparisons of the various options for enumerating
     * fundamental normal surfaces, see B. A. Burton, "Enumerating
     * fundamental normal surfaces: Algorithms, experiments and invariants",
     * ALENEX 2014: Proceedings of the Meeting on Algorithm
     * Engineering & Experiments, SIAM, 2014, pp. 112-124.
     *
     * This flag is incompatible with HilbertPrimal, HilbertDual and
     * HilbertFullCone.
     */
    HilbertCD = 0x0400,
    /**
     * When enumerating fundamental normal surfaces,
     * this flag indicates that a Hilbert basis for the full solution
     * cone should be constructed, and additional combinatorial
     * constraints (such as the quadrilateral constraints) should only
     * be enforced as the final step.
     *
     * If you are only enumerating properly embedded surfaces
     * then this procedure _extremely_ slow, and users should only
     * request it if they have some specialised need.
     *
     * For details and comparisons of the various options for enumerating
     * fundamental normal surfaces, see B. A. Burton, "Enumerating
     * fundamental normal surfaces: Algorithms, experiments and invariants",
     * ALENEX 2014: Proceedings of the Meeting on Algorithm
     * Engineering & Experiments, SIAM, 2014, pp. 112-124.
     *
     * This flag is incompatible with HilbertPrimal, HilbertDual and HilbertCD.
     */
    HilbertFullCone = 0x0800,

    /**
     * Indicates that a normal surface list was enumerated using an
     * older version of Regina (4.93 or earlier).
     *
     * These older versions did not retain details of the algorithm
     * used to build each list, and so in such cases no further
     * algorithmic information is available.
     *
     * If this flag is passed to an enumeration algorithm, it will be ignored.
     */
    Legacy = 0x4000,
    /**
     * Indicates that a normal surface list was built using a customised
     * algorithm.  In such cases, no further details on the algorithm are
     * available.
     *
     * If this flag is passed to an enumeration algorithm, it will be ignored.
     */
    Custom = 0x8000
};

/**
 * A deprecated type alias representing options and variants of algorithms for
 * enumerating normal surfaces in 3-manifold triangulations.
 *
 * \deprecated As of Regina 7.4, NormalAlgFlags is simply an alias for the
 * enumeration type NormalAlg.  A bitwise _combination_ of such values will
 * have the type `Flags<NormalAlg>`, though there is usually no need for end
 * users to explicitly refer to the flags type by name.
 *
 * \ingroup surfaces
 */
using NormalAlgFlags [[deprecated]] = NormalAlg;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal surfaces in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalAlg::Default.
 */
[[deprecated]] inline static constexpr NormalAlg NS_ALG_DEFAULT =
    NormalAlg::Default;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal surfaces in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalAlg::VertexViaReduced.
 */
[[deprecated]] inline static constexpr NormalAlg NS_VERTEX_VIA_REDUCED =
    NormalAlg::VertexViaReduced;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal surfaces in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalAlg::VertexStandardDirect.
 */
[[deprecated]] inline static constexpr NormalAlg NS_VERTEX_STD_DIRECT =
    NormalAlg::VertexStandardDirect;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal surfaces in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalAlg::VertexTree.
 */
[[deprecated]] inline static constexpr NormalAlg NS_VERTEX_TREE =
    NormalAlg::VertexTree;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal surfaces in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalAlg::VertexDD.
 */
[[deprecated]] inline static constexpr NormalAlg NS_VERTEX_DD =
    NormalAlg::VertexDD;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal surfaces in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalAlg::HilbertPrimal.
 */
[[deprecated]] inline static constexpr NormalAlg NS_HILBERT_PRIMAL =
    NormalAlg::HilbertPrimal;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal surfaces in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalAlg::HilbertDual.
 */
[[deprecated]] inline static constexpr NormalAlg NS_HILBERT_DUAL =
    NormalAlg::HilbertDual;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal surfaces in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalAlg::HilbertCD.
 */
[[deprecated]] inline static constexpr NormalAlg NS_HILBERT_CD =
    NormalAlg::HilbertCD;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal surfaces in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalAlg::HilbertFullCone.
 */
[[deprecated]] inline static constexpr NormalAlg NS_HILBERT_FULLCONE =
    NormalAlg::HilbertFullCone;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal surfaces in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalAlg::Legacy.
 */
[[deprecated]] inline static constexpr NormalAlg NS_ALG_LEGACY =
    NormalAlg::Legacy;

/**
 * A deprecated constant indicating an algorithm variant for enumerating
 * normal surfaces in a 3-manifold triangulation.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalAlg::Custom.
 */
[[deprecated]] inline static constexpr NormalAlg NS_ALG_CUSTOM =
    NormalAlg::Custom;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * \param lhs the first flag to combine.
 * \param rhs the second flag to combine.
 * \return the combination of both flags.
 *
 * \ingroup surfaces
 */
inline Flags<NormalAlg> operator | (NormalAlg lhs, NormalAlg rhs) {
    return Flags<NormalAlg>(lhs) | rhs;
}

/**
 * Represents different ways in which Regina can transform one normal
 * surface list into another.
 *
 * Each type of transformation comes with its own preconditions on the
 * original normal surface list and/or its underlying triangulation; these
 * preconditions are documented alongside the individual enumeration values.
 *
 * \ingroup surfaces
 */
enum class NormalTransform {
    /**
     * Converts the set of all embedded vertex normal surfaces in quadrilateral
     * or quadrilateral-octagon coordinates to the set of all embedded vertex
     * normal surfaces in standard normal or standard almost normal
     * coordinates respectively.
     *
     * It should be emphasised that this routine does _not_ simply convert
     * vectors from one coordinate system to another; instead it converts a
     * full set of vertex surfaces in quad or quad-oct coordinates into a
     * full set of vertex surfaces in standard normal or almost normal
     * coordinates.  Typically there are many more vertex surfaces in standard
     * coordinates (all of which this routine will find).
     *
     * This conversion process is typically _much_ faster than enumerating
     * surfaces directly in standard coordinates.  However, normally you
     * would not need to invoke this transformation yourself, since the
     * standard enumeration process will use it automatically when possible.
     * That is, when asked to build a list of standard vertex surfaces,
     * the NormalSurfaces constructor will (if possible) first find all
     * quad or quad-oct vertex surfaces and then use this procedure to convert
     * the solution set.
     *
     * Nevertheless, this standalone transformation is provided as a
     * convenience for users who already have a set of quad or quad-oct
     * vertex surfaces, and who simply wish to convert them to a set of
     * standard vertex surfaces without the implicit cost of enumerating
     * the quad or quad-oct vertex surfaces again.
     *
     * The conversion algorithm is described in detail in "Converting between
     * quadrilateral and standard solution sets in normal surface theory",
     * Benjamin A. Burton, Algebr. Geom. Topol. 9 (2009), 2121-2174.
     *
     * The preconditions for using this transformation:
     *
     * - The underlying triangulation is valid, and has no ideal vertices.
     *
     * - The input to this transformation is exactly the set of all embedded
     *   vertex surfaces in quadrilateral or quadrilateral-octagon coordinates.
     *   This will be checked by examining NormalSurface::coords() and
     *   NormalSurface::which().
     */
    ConvertReducedToStandard = 0x01,
    /**
     * Converts the set of all embedded vertex normal surfaces in standard
     * normal or standard almost normal coordinates to the set of all embedded
     * vertex normal surfaces in quadrilateral or quadrilateral-octagon
     * coordinates respectively.
     *
     * It should be emphasised that this routine does _not_ simply convert
     * vectors from one coordinate system to another; instead it converts a
     * full set of vertex surfaces in standard normal or almost normal
     * coordinates into a full set of vertex surfaces in quad or quad-oct
     * coordinates.  Typically there are far fewer vertex surfaces in
     * quad or quad-oct coordinates (all of which this routine will find).
     *
     * The conversion algorithm is described in detail in "Converting between
     * quadrilateral and standard solution sets in normal surface theory",
     * Benjamin A. Burton, Algebr. Geom. Topol. 9 (2009), 2121-2174.
     *
     * The preconditions for using this transformation:
     *
     * - The underlying triangulation is valid, and has no ideal vertices.
     *
     * - The input to this transformation is exactly the set of all embedded
     *   vertex surfaces in standard normal or almost normal coordinates.
     *   This will be checked by examining NormalSurface::coords() and
     *   NormalSurface::which().
     */
    ConvertStandardToReduced = 0x02,
    /**
     * Selects only the surfaces in the input list that have at least one
     * locally compatible partner.  That is, a surface \a S from the input
     * list will be included in the output list if and only if there is some
     * other surface \a T in the input list for which \a S and \a T are
     * locally compatible.  See NormalSurface::locallyCompatible() for
     * further details on compatibility testing.
     *
     * Be aware that, since vertex links are compatible with everything,
     * if the input list contains a vertex link plus at least one other
     * surface, then the output list will be identical to the input.
     *
     * For the output list, which() will include the flag NormalList::Custom,
     * and algorithm() will be precisely NormalAlg::Custom.
     *
     * The preconditions for using this transformation:
     *
     * - The input list contains only embedded normal or almost normal surfaces.
     *   This will be checked by examining NormalSurface::which().
     */
    FilterCompatible = 0x10,
    /**
     * Selects only the surfaces in the input list that have at least one
     * disjoint partner.  That is, a surface \a S from the input list will
     * be included in the output list if and only if there is some other
     * surface \a T in the input list for which \a S and \a T can be made to
     * intersect nowhere at all, without changing either normal isotopy class.
     * See NormalSurface::disjoint() for further details on disjointness
     * testing.
     *
     * This transformation comes with some caveats:
     *
     * - It cannot deal with empty, disconnected or non-compact surfaces.
     *   Such surfaces will be silently ignored, and will not be used in any
     *   disjointness tests (in particular, they will never be considered as a
     *   "disjoint partner" for any other surface).
     *
     * - Since vertex links can always be made disjoint from other surfaces,
     *   if the input list contains a vertex link plus at least one other
     *   surface, then the output list will be identical to the input.
     *
     * For the output list, which() will include the flag NormalList::Custom,
     * and algorithm() will be precisely NormalAlg::Custom.
     *
     * The preconditions for using this transformation:
     *
     * - The input list contains only embedded normal or almost normal surfaces.
     *   This will be checked by examining NormalSurface::which().
     */
    FilterDisjoint = 0x20,
    /**
     * Selects only the surfaces in the input list that "might" represent
     * two-sided incompressible surfaces.
     *
     * More precisely, this transformation considers all two-sided surfaces in
     * the input list, as well as the two-sided double covers of all one-sided
     * surfaces in the input list (see below for details on how one-sided
     * surfaces are handled).  Each of these surfaces is examined using
     * relatively fast heuristic tests for incompressibility.  Any surface
     * that is definitely _not_ incompressible is ignored, and all other
     * surfaces are placed in the output list.
     *
     * Therefore, it is guaranteed that every incompressible surface from the
     * input list will be included in the output list.  However, each
     * individual output surface might or might not be incompressible.
     *
     * See NormalSurface::isIncompressible() for the definition of
     * incompressibility that is used here.  Note in particular that
     * spheres are _never_ considered incompressible.
     *
     * As indicated above, this filter works exclusively with two-sided
     * surfaces.  If a surface in the input list is one-sided, the heuristic
     * incompressibility tests will be run on its two-sided double cover.
     * Nevertheless, if the tests pass, the original one-sided surface
     * (not the double cover) will be added to the output list.
     *
     * Currently the heuristic tests include (i) throwing away all vertex links
     * and thin edge links, and then (ii) cutting along the remaining surfaces
     * and running Triangulation<3>::hasSimpleCompressingDisc() on the
     * resulting bounded triangulations.  For more details on these tests
     * see "The Weber-Seifert dodecahedral space is non-Haken",
     * Benjamin A. Burton, J. Hyam Rubinstein and Stephan Tillmann,
     * Trans. Amer. Math. Soc. 364:2 (2012), pp. 911-932.
     *
     * For the output list, which() will include the flag NormalList::Custom,
     * and algorithm() will be precisely NormalAlg::Custom.
     *
     * The preconditions for using this transformation:
     *
     * - The underlying 3-manifold triangulation is valid and closed.
     *
     * - The input list contains only compact, connected, embedded normal
     *   surfaces.  In particular, almost normal surfaces are not supported.
     *
     * \warning The behaviour of this transformation is subject to change
     * in future versions of Regina, since additional tests may be added to
     * improve the power of this filtering.
     */
    FilterIncompressible = 0x30
};

/**
 * A deprecated constant indicating some way in which Regina can transform
 * one normal surface list into another.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalTransform::ConvertReducedToStandard.
 */
[[deprecated]] inline static constexpr NormalTransform NS_CONV_REDUCED_TO_STD =
    NormalTransform::ConvertReducedToStandard;

/**
 * A deprecated constant indicating some way in which Regina can transform
 * one normal surface list into another.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalTransform::ConvertStandardToReduced.
 */
[[deprecated]] inline static constexpr NormalTransform NS_CONV_STD_TO_REDUCED =
    NormalTransform::ConvertStandardToReduced;

/**
 * A deprecated constant indicating some way in which Regina can transform
 * one normal surface list into another.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalTransform::FilterCompatible.
 */
[[deprecated]] inline static constexpr NormalTransform NS_FILTER_COMPATIBLE =
    NormalTransform::FilterCompatible;

/**
 * A deprecated constant indicating some way in which Regina can transform
 * one normal surface list into another.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalTransform::FilterIncompressible.
 */
[[deprecated]] inline static constexpr NormalTransform NS_FILTER_INCOMPRESSIBLE =
    NormalTransform::FilterIncompressible;

/**
 * A deprecated constant indicating some way in which Regina can transform
 * one normal surface list into another.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * NormalTransform::FilterDisjoint.
 */
[[deprecated]] inline static constexpr NormalTransform NS_FILTER_DISJOINT =
    NormalTransform::FilterDisjoint;

} // namespace regina

#endif

