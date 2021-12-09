
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
 * for a given 3-manifold triangulation.
 *
 * The NormalList enumeration refers to the \e contents of the list,
 * whereas the NormalAlgFlags enumeration refers to the \e algorithm
 * used to build it.
 *
 * These flags can be combined using the bitwise OR operator, and then
 * passed to enumeration routines such as the NormalSurfaces
 * class constructor.
 *
 * \ingroup surfaces
 */
enum NormalListFlags {
    /**
     * An empty flag, indicating to an enumeration routine that it
     * should use its default behaviour.
     * The numeric value of this flag is zero (i.e., it has no effect
     * when combined with other flags using bitwise OR).
     */
    NS_LIST_DEFAULT = 0x0000,

    /**
     * Indicates that this list is restricted to properly embedded
     * surfaces only.
     *
     * This flag is incompatible with NS_IMMERSED_SINGULAR.
     */
    NS_EMBEDDED_ONLY = 0x0001,
    /**
     * Indicates that the scope of this list includes not just properly
     * embedded surfaces, but also immersed and/or branched surfaces.
     *
     * This is no guarantee that the list \e contains immersed and/or
     * branched surfaces; it merely states that such surfaces have not
     * been explicitly excluded (in particular, the quadrilateral
     * constraints have not been enforced).
     *
     * This flag is incompatible with NS_EMBEDDED_ONLY.
     */
    NS_IMMERSED_SINGULAR = 0x0002,

    /**
     * Indicates a list of all vertex normal surfaces, with respect to
     * the particular normal coordinate system used by the list.
     *
     * This flag is incompatible with NS_FUNDAMENTAL.
     */
    NS_VERTEX = 0x0004,
    /**
     * Indicates a list of all fundamental normal surfaces, with respect to
     * the particular normal coordinate system used by the list.
     *
     * This flag is incompatible with NS_VERTEX.
     */
    NS_FUNDAMENTAL = 0x0008,

    /**
     * Indicates a list that was constructed using an old version of
     * Regina (4.93 or earlier).
     *
     * These older versions did not retain details of how each list was
     * constructed, beyond whether immersed and/or singular surfaces were
     * included.  Therefore no information is available for such lists,
     * other than the presence or absence of the NS_EMBEDDED_ONLY and/or
     * NS_IMMERSED_SINGULAR flags.
     *
     * If this flag is passed to an enumeration routine, it will be ignored.
     */
    NS_LEGACY = 0x4000,
    /**
     * Indicates some other type of list, typically hand-crafted by the
     * user or built by some customised algorithm.
     *
     * If this flag is passed to an enumeration routine, it will be ignored.
     */
    NS_CUSTOM = 0x8000
};

/**
 * A combination of flags for types of normal surface lists.
 *
 * If a function requires a NormalList object as an argument, you can
 * pass a single NormalListFlags constant, or a combination of such
 * constants using the bitwise OR operator, or empty braces {} to indicate
 * no flags at all.
 *
 * \ingroup surfaces
 */
using NormalList = regina::Flags<NormalListFlags>;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * @param lhs the first flag to combine.
 * @param rhs the second flag to combine.
 * @return the combination of both flags.
 *
 * \ingroup surfaces
 */
inline NormalList operator | (NormalListFlags lhs, NormalListFlags rhs) {
    return NormalList(lhs) | rhs;
}

/**
 * Represents options and variants of algorithms for enumerating various
 * types of normal surfaces in 3-manifold triangulations.
 *
 * These options can be combined using the bitwise OR operator, and then
 * passed to enumeration routines such as the NormalSurfaces
 * class constructor.
 *
 * \ingroup surfaces
 */
enum NormalAlgFlags {
    /**
     * An empty flag, indicating to an enumeration routine that it
     * should use its default behaviour.
     * The numeric value of this flag is zero (i.e., it has no effect
     * when combined with other flags using bitwise OR).
     */
    NS_ALG_DEFAULT = 0x0000,

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
     * requested not to (via the flag NS_VERTEX_STD_DIRECT).
     *
     * For an explanation of this procedure, see B. A. Burton,
     * "Converting between quadrilateral and standard solution sets in
     * normal surface theory", Algebr. Geom. Topol. 9 (2009), 2121-2174.
     *
     * This flag is incompatible with NS_VERTEX_STD_DIRECT.
     */
    NS_VERTEX_VIA_REDUCED = 0x0001,

    /**
     * When enumerating in standard normal or almost normal coordinates,
     * this flag indicates that the algorithm should work directly in
     * that coordinate system, and should not go via the "reduced"
     * (quadrilateral or quadrilateral-octagon) coordinate system.
     *
     * This is typically \e much slower than going via the reduced
     * system, and users should only request this if they have a
     * specialised need.  See NS_VERTEX_VIA_REDUCED for further information.
     *
     * This flag is incompatible with NS_VERTEX_VIA_REDUCED.
     */
    NS_VERTEX_STD_DIRECT = 0x0002,

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
     * This flag is incompatible with NS_VERTEX_DD.
     */
    NS_VERTEX_TREE = 0x0010,
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
     * This flag is incompatible with NS_VERTEX_TREE.
     */
    NS_VERTEX_DD = 0x0020,

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
     * This flag is incompatible with NS_HILBERT_DUAL,
     * NS_HILBERT_CD and NS_HILBERT_FULLCONE.
     */
    NS_HILBERT_PRIMAL = 0x0100,
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
     * This flag is incompatible with NS_HILBERT_PRIMAL,
     * NS_HILBERT_CD and NS_HILBERT_FULLCONE.
     */
    NS_HILBERT_DUAL = 0x0200,
    /**
     * When enumerating fundamental normal surfaces,
     * this flag indicates that a modified Contejean-Devie procedure should
     * be used for enumerating a Hilbert basis.
     *
     * The Contejean-Devie procedure is typically \e much slower
     * than either the primal or dual method, and users should only
     * request it if they have some specialised need.
     *
     * For details and comparisons of the various options for enumerating
     * fundamental normal surfaces, see B. A. Burton, "Enumerating
     * fundamental normal surfaces: Algorithms, experiments and invariants",
     * ALENEX 2014: Proceedings of the Meeting on Algorithm
     * Engineering & Experiments, SIAM, 2014, pp. 112-124.
     *
     * This flag is incompatible with NS_HILBERT_PRIMAL,
     * NS_HILBERT_DUAL and NS_HILBERT_FULLCONE.
     */
    NS_HILBERT_CD = 0x0400,
    /**
     * When enumerating fundamental normal surfaces,
     * this flag indicates that a Hilbert basis for the full solution
     * cone should be constructed, and additional combinatorial
     * constraints (such as the quadrilateral constraints) should only
     * be enforced as the final step.
     *
     * If you are only enumerating properly embedded surfaces
     * then this procedure \e extremely slow, and users should only
     * request it if they have some specialised need.
     *
     * For details and comparisons of the various options for enumerating
     * fundamental normal surfaces, see B. A. Burton, "Enumerating
     * fundamental normal surfaces: Algorithms, experiments and invariants",
     * ALENEX 2014: Proceedings of the Meeting on Algorithm
     * Engineering & Experiments, SIAM, 2014, pp. 112-124.
     *
     * This flag is incompatible with NS_HILBERT_PRIMAL,
     * NS_HILBERT_DUAL and NS_HILBERT_CD.
     */
    NS_HILBERT_FULLCONE = 0x0800,

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
    NS_ALG_LEGACY = 0x4000,
    /**
     * Indicates that a normal surface list was built using a customised
     * algorithm.  In such cases, no further details on the algorithm are
     * available.
     *
     * If this flag is passed to an enumeration algorithm, it will be ignored.
     */
    NS_ALG_CUSTOM = 0x8000
};

/**
 * A combination of flags for normal surface enumeration algorithms.
 *
 * If a function requires a NormalAlg object as an argument, you can
 * pass a single NormalAlgFlags constant, or a combination of such
 * constants using the bitwise OR operator, or empty braces {} to indicate
 * no flags at all.
 *
 * \ingroup surfaces
 */
using NormalAlg = regina::Flags<NormalAlgFlags>;

/**
 * Returns the bitwise OR of the two given flags.
 *
 * @param lhs the first flag to combine.
 * @param rhs the second flag to combine.
 * @return the combination of both flags.
 *
 * \ingroup surfaces
 */
inline NormalAlg operator | (NormalAlgFlags lhs, NormalAlgFlags rhs) {
    return NormalAlg(lhs) | rhs;
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
enum NormalTransform {
    /**
     * Converts the set of all embedded vertex normal surfaces in quadrilateral
     * or quadrilateral-octagon coordinates to the set of all embedded vertex
     * normal surfaces in standard normal or standard almost normal
     * coordinates respectively.
     *
     * It should be emphasised that this routine does \e not simply convert
     * vectors from one coordinate system to another; instead it converts a
     * full set of vertex surfaces in quad or quad-oct coordinates into a
     * full set of vertex surfaces in standard normal or almost normal
     * coordinates.  Typically there are many more vertex surfaces in standard
     * coordinates (all of which this routine will find).
     *
     * This conversion process is typically \e much faster than enumerating
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
    NS_CONV_REDUCED_TO_STD = 0x01,
    /**
     * Converts the set of all embedded vertex normal surfaces in standard
     * normal or standard almost normal coordinates to the set of all embedded
     * vertex normal surfaces in quadrilateral or quadrilateral-octagon
     * coordinates respectively.
     *
     * It should be emphasised that this routine does \e not simply convert
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
    NS_CONV_STD_TO_REDUCED = 0x02,
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
     * For the output list, which() will include the NS_CUSTOM flag, and
     * algorithm() will be precisely NS_ALG_CUSTOM.
     *
     * The preconditions for using this transformation:
     *
     * - The input list contains only embedded normal or almost normal surfaces.
     *   This will be checked by examining NormalSurface::which().
     */
    NS_FILTER_COMPATIBLE = 0x10,
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
     * For the output list, which() will include the NS_CUSTOM flag, and
     * algorithm() will be precisely NS_ALG_CUSTOM.
     *
     * The preconditions for using this transformation:
     *
     * - The input list contains only embedded normal or almost normal surfaces.
     *   This will be checked by examining NormalSurface::which().
     */
    NS_FILTER_DISJOINT = 0x20,
    /**
     * Selects only the surfaces in the input list that "might" represent
     * two-sided incompressible surfaces.
     *
     * More precisely, this transformation considers all two-sided surfaces in
     * the input list, as well as the two-sided double covers of all one-sided
     * surfaces in the input list (see below for details on how one-sided
     * surfaces are handled).  Each of these surfaces is examined using
     * relatively fast heuristic tests for incompressibility.  Any surface
     * that is definitely \e not incompressible is ignored, and all other
     * surfaces are placed in the output list.
     *
     * Therefore, it is guaranteed that every incompressible surface from the
     * input list will be included in the output list.  However, each
     * individual output surface might or might not be incompressible.
     *
     * See NormalSurface::isIncompressible() for the definition of
     * incompressibility that is used here.  Note in particular that
     * spheres are \e never considered incompressible.
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
     * For the output list, which() will include the NS_CUSTOM flag, and
     * algorithm() will be precisely NS_ALG_CUSTOM.
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
    NS_FILTER_INCOMPRESSIBLE = 0x30
};

} // namespace regina

#endif

