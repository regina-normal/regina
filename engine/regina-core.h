
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

/*! \file regina-core.h
 *  \brief Core definitions that must be included in every Regina header file.
 */

#ifndef __REGINA_CORE_H
#ifndef __DOXYGEN
#define __REGINA_CORE_H
#endif

#include "regina-config.h"

namespace regina {

/**
 * Indicates whether this build of Regina supports triangulations of the given
 * dimension.
 *
 * A normal build of Regina will support triangulations of dimensions 2 to 8
 * inclusive.  If necessary, you can make a "high-dimenisional build" of Regina
 * using the `REGINA_HIGHDIM` option at compile time, which will enable support
 * for triangulations of dimensions 2 to 15 inclusive.
 *
 * See the page on \ref stddim "standard dimensions" for further discussion of
 * supported versus standard dimensions.
 *
 * \param dim the dimension in question.
 * \return \c true if and only if \a dim is one of Regina's supported
 * dimensions.
 *
 * \see standardDim()
 * \see maxDim()
 *
 * \ingroup engine
 */
constexpr bool supportedDim(int dim) {
#ifdef REGINA_HIGHDIM
    return (dim >= 2 && dim <= 15);
#else
    return (dim >= 2 && dim <= 8);
#endif
}

/**
 * Indicates whether the given dimension is one of Regina's
 * _standard dimensions_ for triangulations.
 *
 * Regina's standard dimensions are 2, 3, and 4.  These dimensions are
 * Regina's main focus: here triangulations are highly optimised and offer
 * rich functionality.  This is in contrast to other (higher) supported
 * dimensions, where Regina's functionality is much more basic.
 *
 * See the page on \ref stddim "standard dimensions" for further discussion of
 * supported versus standard dimensions.
 *
 * \param dim the dimension in question.
 * \return \c true if and only if \a dim is one of Regina's standard dimensions.
 *
 * \see supportedDim()
 * \see maxDim()
 *
 * \ingroup engine
 */
constexpr bool standardDim(int dim) {
    return (dim == 2 || dim == 3 || dim == 4);
}

/**
 * Indicates that largest dimension of triangulation that Regina can work with.
 *
 * If Regina was built with the \c REGINA_HIGHDIM option, then this will be 15;
 * otherwise it will be 8 (the default for ordinary builds).
 *
 * \return Regina's maximum dimension of triangulation.
 *
 * \see supportedDim()
 * \see standardDim()
 *
 * \ingroup engine
 */
constexpr int maxDim() {
#ifdef REGINA_HIGHDIM
    return 15;
#else
    return 8;
#endif
}

#ifdef __DOXYGEN
    /**
     * Defined if and only if native 128-bit arithmetic is available on
     * this platform.
     *
     * If this macro is defined, then you can access native signed and
     * unsigned 128-bit integers through the type aliases Int128 and UInt128
     * respectively.
     *
     * If this macro is not defined, then the types Int128 and UInt128
     * will be left undefined.
     *
     * \ingroup utilities
     */
    #define INT128_AVAILABLE

    /**
     * A native signed 128-bit integer type, if available on this platform.
     *
     * If native 128-bit arithmetic is not available on this platform, then
     * the types Int128 and UInt128 will be left undefined.
     *
     * See also the preprocessor macro `INT128_AVAILABLE`, which is defined
     * precisely when these type aliases Int128 and UInt128 exist.
     *
     * \ingroup utilities
     */
    using Int128 = int128_t;

    /**
     * A native unsigned 128-bit integer type, if available on this platform.
     *
     * If native 128-bit arithmetic is not available on this platform, then
     * the types Int128 and UInt128 will be left undefined.
     *
     * See also the preprocessor macro `INT128_AVAILABLE`, which is defined
     * precisely when these type aliases Int128 and UInt128 exist.
     *
     * \ingroup utilities
     */
    using UInt128 = uint128_t;
#else
    #if defined(INTERNAL___INT128_FOUND)
        #define INT128_AVAILABLE
        using Int128 = __int128;
        using UInt128 = __uint128;
    #elif defined(INTERNAL___INT128_T_FOUND)
        #define INT128_AVAILABLE
        using Int128 = __int128_t;
        using UInt128 = __uint128_t;
    #elif defined(INTERNAL_INT128_T_FOUND)
        #define INT128_AVAILABLE
        using Int128 = int128_t;
        using UInt128 = uint128_t;
    #else
        #undef INT128_AVAILABLE
    #endif
#endif

/**
 * Represents various classes of algorithms that Regina can use for
 * computations.  A function that takes an Algorithm argument need not
 * support all types of algorithm - if an unsupported algorithm is
 * passed then Regina will fall back to Algorithm::Default.
 *
 * This enumeration type does _not_ allow constants to be combined using
 * the OR operator.
 *
 * For some computations where the user can exert more fine-grained control
 * over the underlying algorithm, a more specialised enumeration type is
 * used instead.  For example, see the NormalSurfaces class constructor, which
 * can take a combination of flags of the specialised algorithm type NormalAlg.
 *
 * \ingroup engine
 */
enum class Algorithm {
    /**
     * The default algorithm.  Here Regina will choose whichever
     * algorithm it thinks (rightly or wrongly) is most appropriate.
     */
    Default = 0,
    /**
     * An optimised backtracking algorithm.  This typically works over
     * some search tree (often of exponential size or worse), but
     * include significant optimisations to prune the search tree and/or
     * cache computations where possible.
     */
    Backtrack = 1,
    /**
     * A treewidth-based algorithm.  Typically this uses dynamic
     * programming over a tree decomposition of some underlying graph.
     * Such algorithms are often fast for triangulations or links with
     * small treewidth, but may require large amounts of memory.
     */
    Treewidth = 2,
    /**
     * A naive algorithm.  This typically works directly with the
     * underlying definitions (e.g., computing Turaev-Viro as a state sum),
     * without further optimisations.
     *
     * \warning Naive algorithms should only be used for comparison and
     * experimentation.  Due to their slow performance, they are not
     * suitable for "real" applications.
     */
    Naive = 3
};

/**
 * A deprecated constant indicating one of the available classes of algorithm.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * Algorithm::Default.
 *
 * \ingroup engine
 */
[[deprecated]] inline constexpr Algorithm ALG_DEFAULT = Algorithm::Default;
/**
 * A deprecated constant indicating one of the available classes of algorithm.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * Algorithm::Backtrack.
 *
 * \ingroup engine
 */
[[deprecated]] inline constexpr Algorithm ALG_BACKTRACK = Algorithm::Backtrack;
/**
 * A deprecated constant indicating one of the available classes of algorithm.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * Algorithm::Treewidth.
 *
 * \ingroup engine
 */
[[deprecated]] inline constexpr Algorithm ALG_TREEWIDTH = Algorithm::Treewidth;
/**
 * A deprecated constant indicating one of the available classes of algorithm.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * Algorithm::Naive.
 *
 * \ingroup engine
 */
[[deprecated]] inline constexpr Algorithm ALG_NAIVE = Algorithm::Naive;

/**
 * Represents different programming languages that Regina supports.
 *
 * \ingroup engine
 */
enum class Language {
    /**
     * The C++ programming language.
     */
    Cxx = 1,
    /**
     * The Python programming language.
     */
    Python = 2,
    /**
     * The programming language currently being used.
     *
     * When writing C++ code, this is equivalent to Language::Cxx.
     * When using the Python bindings, this is equivalent to Language::Python.
     *
     * \warning For developers who are _writing_ the Python bindings, remember
     * that your binding code (which is written in C++) will treat this as
     * Language::Cxx, whereas your Python users will expect Language::Python.
     * See the code for binding for Link::source() as an example of where this
     * issue arises and how it is resolved.
     */
    Current = Cxx
};

/**
 * Indicates different ways in which code could change a topological object,
 * such as a link or triangulation.
 *
 * \nopython This enumeration is only used internally by private and/or
 * protected class templates, and is therefore not made available to Python.
 *
 * \ingroup engine
 */
enum class ChangeType {
    /**
     * Indicates a change that could potentially alter any aspect of the object.
     */
    General = 0,
    /**
     * Indicates a change that could alter the representation of the object,
     * but will not alter its topology.  Examples of such changes could include
     * Pachner moves (which alter the triangulation but not the underlying
     * manifold), or Reidemeister moves (which alter the link diagram but not
     * the underlying link).
     *
     * The specific meaning of "will not alter its topology" depends upon the
     * particular type of object.  See the TopologyLockable class notes for
     * further details on what this means.
     */
    PreserveTopology = 1,
    /**
     * Indicates a change that is cosmetic in nature, in that it will not alter
     * any computed properties of the object.  Examples of such changes include
     * modifying simplex locks in triangulations, or reordering the
     * top-dimensional simplices in a triangulation, or changing the rendering
     * radius of a spatial link.
     */
    Cosmetic = 2
};

/**
 * An empty type that can be used to disable in-built safety checks in some
 * specific circumstances.
 *
 * There are many settings in which Regina checks that an operation is safe or
 * legal before performing it.  However, in some settings these checks are
 * expensive.  Therefore some particular functions may offer a variant that
 * takes an extra Unprotected argument, allowing the programmer to indicate
 * that they promise the checks will succeed, and there is no need to perform
 * them.
 *
 * An example is Pachner moves on triangulations, where the legality check
 * becomes expensive when operating on a face of large codimension.
 *
 * Functions that offer an unprotected variant are rare, and are typically
 * functions that are used in speed-critical scenarios (such as Pachner
 * moves being used as atomic operations within larger composite moves and/or
 * simplification algorithms).
 *
 * The Unprotected type itself is an empty type.  Typically you would just
 * use the constant `regina::unprotected`.
 *
 * \nopython By design, Python users are not able to circumvent any of
 * Regina's automatic checks.  If speed is essential, you should be using C++.
 *
 * \ingroup engine
 */
struct Unprotected {
    /**
     * Creates a new empty object.  Typically there is no need to create your
     * own objects; you can just use the constant `regina::unprotected`.
     */
    explicit Unprotected() = default;
};

/**
 * An empty object that can be used to disable Regina's in-built safety checks
 * in some specific circumstances.  See the Unprotected class notes for details.
 *
 * \nopython By design, Python users are not able to circumvent any of
 * Regina's automatic checks.  If speed is essential, you should be using C++.
 *
 * \ingroup engine
 */
inline constexpr Unprotected unprotected;

/**
 * A legitimate C++ type that can never be used in an implementation.
 *
 * This type is declared but never defined.
 *
 * For an example of its use, see TriangulationTraits (where it acts as a
 * placeholder for non-existent classes that would otherwise cause a compile
 * error).
 *
 * \nopython
 *
 * \ingroup engine
 */
struct NoSuchType;

} // namespace regina

#endif
