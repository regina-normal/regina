
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

/*! \file triangulation/facenumbering.h
 *  \brief Describes the way in which <i>subdim</i>-faces are numbered
 *  within a <i>dim</i>-dimensional simplex.
 */

#ifndef __FACENUMBERING_H
#ifndef __DOXYGEN
#define __FACENUMBERING_H
#endif

#include "triangulation/detail/facenumbering.h"

namespace regina {

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Returns the binomial coefficient \n choose \a k for small arguments
 * \a n and \a k.
 *
 * This routine is very fast, since it uses a constant-time lookup.
 * The trade-off is that it can only be used for \a n &le; 16.
 *
 * If you need a compile-time constant, you should use the constant
 * FaceNumbering<n-1, k-1>::nFaces instead.  This function is provided for
 * situations where \a n and/or \a k are not known until runtime.
 *
 * \note The constraint \a n &le; 16 is large enough for working with
 * triangulations in Regina, since Regina restricts its triangulations to
 * dimension &le; 15 (where each simplex has 16 vertices).
 *
 * @param n the parameter \a n in (\a n choose \a k); this must be
 * between 0 and 16 inclusive.
 * @param k the parameter \a k in (\a n choose \a k); this must be
 * between 0 and \a n inclusive.
 * @return the binomial coefficient \a n choose \a k.
 */
int binomSmall(int n, int k);

/**
 * Specifies how <i>subdim</i>-faces are numbered within a
 * <i>dim</i>-dimensional simplex.
 *
 * Regina uses the following general scheme for numbering faces:
 *
 * - For low-dimensional faces (\a subdim &lt; \a dim / 2), faces are
 *   numbered in lexicographical order according to their vertices.
 *   For example, in a 3-dimensional triangulation, edges 0,...,5 contain
 *   vertices 01, 02, 03, 12, 13, 23 respectively.
 *
 * - For high-dimensional faces (\a subdim &ge; \a dim / 2), faces are
 *   numbered in \e reverse lexicographical order according to their vertices.
 *   For example, in a 3-dimensional triangulation, triangles 0,...,3 contain
 *   vertices 123, 023, 013, 012 respectively.
 *
 * - As a consequence, unless \a subdim = (<i>dim</i>-1)/2, we always have
 *   <i>subdim</i>-face number \a i opposite (<i>dim</i>-1-<i>subdim</i>)-face
 *   number \a i.  For the special "halfway case" \a subdim = (<i>dim</i>-1)/2,
 *   where each <i>subdim</i>-face is opposite another <i>subdim</i>-face,
 *   we always have <i>subdim</i>-face number \a i opposite
 *   <i>subdim</i>-face number (<i>nFaces</i>-1-\a i).
 *
 * Every class Face<dim, subdim> inherits from this class, which means
 * you can access these routines as Face<dim, subdim>::ordering(),
 * Face<dim, subdim>::faceNumber(), and so on.
 *
 * An advantage of referring to FaceNumbering<dim, subdim> directly (as
 * opposed to Face<dim, subdim>) is that its header is lightweight: it does not
 * pull in the large and complex headers required by Face<dim, subdim>.
 *
 * This class is specialised (and optimised) in Regina's
 * \ref stddim "standard dimensions".
 *
 * \ifacespython This class is not available in Python.  However, all of
 * its routines can be accessed through Face<dim, subdim> (which in Python
 * becomes Face<i>dim</i>_<i>subdim</i>, or one of the type aliases such as
 * Vertex3, Edge2 and so on).
 *
 * \tparam dim the dimension of the simplex whose faces we are numbering.
 * This must be between 1 and 15 inclusive.
 * \tparam subdim the dimension of the faces that we are numbering.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class FaceNumbering : public detail::FaceNumberingImpl<
        dim, subdim, ((dim + 1) >= 2 * (subdim + 1))> {
};

/**
 * Returns the (<i>dim</i>-2)-face number that is opposite the edge
 * joining vertices \a i and \a j in a <i>dim</i>-dimensional simplex.
 *
 * This function is offered because its implementation is faster than
 * working through the FaceNumbering class.
 *
 * The arguments \a i and \a j do not need to appear in ascending order.
 *
 * @param i the first vertex of an edge in a <i>dim</i>-dimensional simplex.
 * This must be between 0 and \a dim inclusive.
 * @param j the second vertex of an edge in a <i>dim</i>-dimensional simplex.
 * This must be between 0 and \a dim inclusive, and must be different from \a i.
 * @return the number of the (<i>dim</i>-2)-face opposite the given edge.
 */
template <int dim>
inline int faceOppositeEdge(int i, int j);

// Inline functions

inline int binomSmall(int n, int k) {
    return detail::binomSmall_[n][k];
}

template <int dim>
int faceOppositeEdge(int i, int j) {
    static_assert(dim >= 5,
        "The generic implementation of faceOpposite() can only be used "
        "for dimensions dim >= 5.");

    // In dimension >= 5, the requested (dim-2)-face has the same number
    // as the edge (i, j), and edges are numbered in increasing
    // lexicographical order by their vertices.
    if (i > j)
        std::swap(i, j);

    unsigned ans = detail::binomSmall_[dim + 1][2] - 1; // index of last edge
    if (i <= dim - 2)
        ans -= detail::binomSmall_[dim - i][2]; // index of last edge (i, _)
    return ans - (dim - j); // index of edge (i, j)
}

#ifndef __DOXYGEN

template <>
inline int faceOppositeEdge<2>(int i, int j) {
    // We want the vertex number opposite edge (i, j).
    return (3 - i - j);
}

template <>
inline int faceOppositeEdge<3>(int i, int j) {
    // We want the edge number opposite edge (i, j).
    // We can get this using the 3-D lookup table.
    return 5 - FaceNumbering<3, 1>::edgeNumber[i][j];
}

template <>
inline int faceOppositeEdge<4>(int i, int j) {
    // The triangle opposite edge (i, j) has the same number as edge(i, j).
    // We can get this using the 4-D lookup table.
    return FaceNumbering<4, 1>::edgeNumber[i][j];
}

#endif // __DOXYGEN

} // namespace regina

#endif

