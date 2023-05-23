
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
 * passed then Regina will fall back to ALG_DEFAULT.
 *
 * This enumeration type does _not_ allow constants to be combined using
 * the OR operator.
 *
 * For some computations where the user can exert more fine-grained control
 * over the underlying algorithm, a more specialised enumeration type is
 * used instead.  For example, see the NormalSurfaces class constructor, which
 * uses the specialised algorithm types NormalAlg and NormalAlgFlags.
 *
 * \ingroup engine
 */
enum Algorithm {
    /**
     * The default algorithm.  Here Regina will choose whichever
     * algorithm it thinks (rightly or wrongly) is most appropriate.
     */
    ALG_DEFAULT = 0,
    /**
     * An optimised backtracking algorithm.  This typically works over
     * some search tree (often of exponential size or worse), but
     * include significant optimisations to prune the search tree and/or
     * cache computations where possible.
     */
    ALG_BACKTRACK = 1,
    /**
     * A treewidth-based algorithm.  Typically this uses dynamic
     * programming over a tree decomposition of some underlying graph.
     * Such algorithms are often fast for triangulations or links with
     * small treewidth, but may require large amounts of memory.
     */
    ALG_TREEWIDTH = 2,
    /**
     * A naive algorithm.  This typically works directly with the
     * underlying definitions (e.g., computing Turaev-Viro as a state sum),
     * without further optimisations.
     *
     * \warning Naive algorithms should only be used for comparison and
     * experimentation.  Due to their slow performance, they are not
     * suitable for "real" applications.
     */
    ALG_NAIVE = 3,
};

/**
 * Indicates different ways in which a code could change a topological object,
 * such as a link or triangulation.
 *
 * Currently this enumeration is only used with triangulations, but it is
 * expected that it will be used with other objects (in particular, links)
 * in a future releae of Regina.
 *
 * \nopython This enumeration is only used internally by private and/or
 * protected class templates, and is therefore not made available to Python.
 */
enum ChangeType {
    /**
     * Indicates a change that could potentially alter any aspect of the object.
     */
    CHANGE_GENERAL = 0,
    /**
     * Indicates a change that could alter the representation of the object,
     * but will not alter its topology.  Examples of such changes could include
     * Pachner moves (which alter the triangulation but not the underlying
     * manifold), or Reidemeister moves (which alter the link diagram but not
     * the underlying link).
     *
     * The specific meaning of "will not alter its topology" depends upon the
     * type of object.  For example, the inner class
     * TriangulationBase<dim>::TopologyLock describes exactly what this means
     * in the setting of triangulations.
     */
    CHANGE_PRESERVE_TOPOLOGY = 1,
    /**
     * Indicates a change that is cosmetic in nature, in that it will not alter
     * any computed properties of the object.  Examples of such changes include
     * modifying simplex locks in triangulations, or reordering the
     * top-dimensional simplices in a triangulation.
     */
    CHANGE_PRESERVE_ALL_PROPERTIES = 2
};

} // namespace regina

#endif
