
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

#include "regina-config.h" // for REGINA_HIGHDIM

namespace regina {

/**
 * Indicates whether the given dimension is one of Regina's
 * \ref stddim "standard dimensions".
 * Standard dimensions offer significantly richer functionality for
 * triangulations than generic dimensions.
 *
 * \param dim the dimension in question.
 * \return \c true if and only if \a dim is one of Regina's standard dimensions.
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
 * \ingroup engine
 */
constexpr int maxDim() {
#ifdef REGINA_HIGHDIM
    return 15;
#else
    return 8;
#endif
}

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
 */
[[deprecated]] inline constexpr Algorithm ALG_DEFAULT = Algorithm::Default;
/**
 * A deprecated constant indicating one of the available classes of algorithm.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * Algorithm::Backtrack.
 */
[[deprecated]] inline constexpr Algorithm ALG_BACKTRACK = Algorithm::Backtrack;
/**
 * A deprecated constant indicating one of the available classes of algorithm.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * Algorithm::Treewidth.
 */
[[deprecated]] inline constexpr Algorithm ALG_TREEWIDTH = Algorithm::Treewidth;
/**
 * A deprecated constant indicating one of the available classes of algorithm.
 *
 * \deprecated This has been renamed to the scoped enumeration constant
 * Algorithm::Naive.
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
 */
inline constexpr Unprotected unprotected;

} // namespace regina

#endif
