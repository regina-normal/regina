
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

/*! \file triangulation/facenumbering.h
 *  \brief Describes the way in which <i>subdim</i>-faces are numbered
 *  within a <i>dim</i>-dimensional simplex.
 */

#ifndef __REGINA_FACENUMBERING_H
#ifndef __DOXYGEN
#define __REGINA_FACENUMBERING_H
#endif

#include <algorithm>
#include "regina-core.h"
#include "maths/binom.h"
#include "maths/perm.h"

namespace regina {
namespace detail {

/**
 * Provides hard-coded lookup tables for face numbering in a handful of small
 * dimensions.  These tables are used for numbering <i>subdim</i>-faces of a
 * <i>dim</i>-dimensional simplex.
 *
 * For most dimensions Regina has no such tables, and so this class will offer
 * nothing more than the class constant \a hasNumberingTables (which will be
 * \c false).  Currently the only dimensions where Regina provides publicly
 * accessible tables are:
 *
 * - edges in 3-D and 4-D triangulations, where this class provides the
 *   tables `edgeNumber[][]` and `edgeVertex[][]`, as well as the class
 *   constant \a hasNumberingTables (which will be \c true);
 *
 * - triangles in 4-D triangulations, where this class provides the tables
 *   `triangleNumber[][][]` and `triangleVertex[][]`, and again the class
 *   constant \a hasNumberingTables (which will be \c true).
 *
 * You can access these lookup tables through the end-user classes
 * `FaceNumbering<dim, subdim>` or `Face<dim, subdim>` (both of which use
 * FaceNumberingTables as a base class).  There is no need to refer to
 * the class FaceNumberingTables directly.
 *
 * Independent of the public tables described above, some dimensions might
 * provide private tables for internal use by the face numbering
 * implementations.  These should be considered implementation details only.
 *
 * See the FaceNumbering template class notes for further information,
 * including details of how the face numbering scheme works.
 *
 * \python This base class is not present; instead Python users can access
 * these routines through the class `Face<dim, subdim>` (which in Python
 * becomes Face<i>dim</i>_<i>subdim</i>, or one of the type aliases
 * such as Vertex3, Edge2 and so on).
 *
 * \tparam dim the dimension of the simplex whose faces are being numbered.
 * \tparam subdim the dimension of the faces being numbered.
 *
 * \ingroup detail
 */
template <int dim, int subdim>
requires (dim >= 1 && dim <= maxDim() && subdim >= 0 && subdim < dim)
class FaceNumberingTables {
    public:
        /**
         * Indicates whether Regina provides public lookup tables for this
         * combination of triangulation and face dimensions.
         */
        static constexpr bool hasNumberingTables = false;

    protected:
        /**
         * Indicates whether Regina provides private lookup tables for the
         * ordering() function.
         */
        static constexpr bool hasOrderingTables = false;
};

template <>
class FaceNumberingTables<2, 1> {
    public:
        /**
         * Indicates whether Regina provides public lookup tables for this
         * combination of triangulation and face dimensions.
         */
        static constexpr bool hasNumberingTables = false;

    protected:
        /**
         * Indicates whether Regina provides private lookup tables for the
         * ordering() function.
         */
        static constexpr bool hasOrderingTables = true;

        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<3>::Code faceOrdering[3] = { 2, 1, 0 };
};

template <>
class FaceNumberingTables<3, 0> {
    public:
        /**
         * Indicates whether Regina provides public lookup tables for this
         * combination of triangulation and face dimensions.
         */
        static constexpr bool hasNumberingTables = false;

    protected:
        /**
         * Indicates whether Regina provides private lookup tables for the
         * ordering() function.
         */
        static constexpr bool hasOrderingTables = true;

        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<4>::Code2 faceOrdering[4] { 0, 6, 16, 22 };
};

template <>
class FaceNumberingTables<3, 1> {
    public:
        /**
         * Indicates whether Regina provides public lookup tables for this
         * combination of triangulation and face dimensions.
         */
        static constexpr bool hasNumberingTables = true;

        /**
         * A table that maps vertices of a tetrahedron to edge numbers.
         *
         * Edges in a tetrahedron are numbered 0,...,5.  This table
         * converts vertices to edge numbers; in particular, the edge
         * joining vertices \a i and \a j of a tetrahedron is edge
         * number `edgeNumber[i][j]`.  Here \a i and \a j must be
         * distinct, must be between 0 and 3 inclusive, and may be given
         * in any order.  The resulting edge number will be between 0 and 5
         * inclusive.
         *
         * \note Accessing `edgeNumber[i][j]` is equivalent to calling
         * `faceNumber(p)`, where \a p is a permutation that maps
         * 0,1 to \a i,\a j in some order.
         */
        static constexpr int edgeNumber[4][4] = {
            { -1, 0, 1, 2 }, { 0, -1, 3, 4 }, { 1, 3, -1, 5 }, { 2, 4, 5, -1 }
        };

        /**
         * A table that maps edges of a tetrahedron to vertex numbers.
         *
         * Edges in a tetrahedron are numbered 0,...,5.  This table
         * converts edge numbers to vertices; in particular, edge \a i
         * in a tetrahedron joins vertices `edgeVertex[i][0]` and
         * `edgeVertex[i][1]`.  Here \a i must be bewteen 0 and 5
         * inclusive; the resulting vertex numbers will be between 0 and 3
         * inclusive.
         *
         * It is guaranteed that `edgeVertex[i][0]` will always
         * be smaller than `edgeVertex[i][1]`.
         *
         * \note Accessing `edgeVertex[i][j]` is equivalent to
         * calling `ordering(i)[j]`.
         */
        static constexpr int edgeVertex[6][2] = {
            { 0, 1 }, { 0, 2 }, { 0, 3 }, { 1, 2 }, { 1, 3 }, { 2, 3 }
        };

    protected:
        /**
         * Indicates whether Regina provides private lookup tables for the
         * ordering() function.
         */
        static constexpr bool hasOrderingTables = true;

        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<4>::Code2 faceOrdering[6] = {
            0, 2, 4, 8, 10, 16
        };
};

template <>
class FaceNumberingTables<3, 2> {
    public:
        /**
         * Indicates whether Regina provides public lookup tables for this
         * combination of triangulation and face dimensions.
         */
        static constexpr bool hasNumberingTables = false;

    protected:
        /**
         * Indicates whether Regina provides private lookup tables for the
         * ordering() function.
         */
        static constexpr bool hasOrderingTables = true;

        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<4>::Code2 faceOrdering[4] = { 9, 2, 1, 0 };
};

template <>
class FaceNumberingTables<4, 1> {
    public:
        /**
         * Indicates whether Regina provides public lookup tables for this
         * combination of triangulation and face dimensions.
         */
        static constexpr bool hasNumberingTables = true;

        /**
         * A table that maps vertices of a pentachoron to edge numbers.
         *
         * Edges in a pentachoron are numbered 0,...,9.  This table
         * converts vertices to edge numbers; in particular, the edge
         * joining vertices \a i and \a j of a pentachoron is
         * edge number `edgeNumber[i][j]`.  Here \a i and \a j
         * must be distinct, must be between 0 and 4 inclusive, and may
         * be given in any order.  The resulting edge number will be
         * between 0 and 9 inclusive.
         *
         * \note Accessing `edgeNumber[i][j]` is equivalent to calling
         * `faceNumber(p)`, where \a p is a permutation that maps
         * 0,1 to \a i,\a j in some order.
         */
        static constexpr int edgeNumber[5][5] = {
            { -1, 0, 1, 2, 3 }, { 0, -1, 4, 5, 6 }, { 1, 4, -1, 7, 8 },
            { 2, 5, 7, -1, 9 }, { 3, 6, 8, 9, -1 }
        };

        /**
         * A table that maps edges of a pentachoron to vertex numbers.
         *
         * Edges in a pentachoron are numbered 0,...,9.  This table
         * converts edge numbers to vertices; in particular, edge \a i
         * in a pentachoron joins vertices `edgeVertex[i][0]` and
         * `edgeVertex[i][1]`.  Here \a i must be between 0 and 9
         * inclusive; the resulting vertex numbers will be between 0 and 4
         * inclusive.
         *
         * It is guaranteed that `edgeVertex[i][0]` will always
         * be smaller than `edgeVertex[i][1]`.
         *
         * \note Accessing `edgeVertex[i][j]` is equivalent to
         * calling `ordering(i)[j]`.
         */
        static constexpr int edgeVertex[10][2] = {
            { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 1, 2 },
            { 1, 3 }, { 1, 4 }, { 2, 3 }, { 2, 4 }, { 3, 4 }
        };

    protected:
        /**
         * Indicates whether Regina provides private lookup tables for the
         * ordering() function.
         */
        static constexpr bool hasOrderingTables = true;

        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<5>::Code2 faceOrdering[10] = {
            0, 6, 12, 18, 30, 36, 42, 60, 66, 90
        };
};

template <>
class FaceNumberingTables<4, 2> {
    public:
        /**
         * Indicates whether Regina provides public lookup tables for this
         * combination of triangulation and face dimensions.
         */
        static constexpr bool hasNumberingTables = true;

        /**
         * A table that maps vertices of a pentachoron to triangle numbers.
         *
         * Triangles in a pentachoron are numbered 0,...,9.  This table
         * converts vertices to triangle numbers; in particular, the triangle
         * spanned by vertices \a i, \a j and \a k of a pentachoron is triangle
         * number `triangleNumber[i][j][k]`.  Here \a i, \a j and \a k
         * must be distinct, must be between 0 and 4 inclusive, and may
         * be given in any order.  The resulting triangle number will be
         * between 0 and 9 inclusive.
         *
         * \note Accessing `triangleNumber[i][j][k]` is equivalent to
         * calling `faceNumber(p)`, where \a p is a permutation that
         * maps 0,1,2 to \a i,\a j,\a k in some order.
         */
        static constexpr int triangleNumber[5][5][5] = {
            { { -1,-1,-1,-1,-1 }, { -1,-1, 9, 8, 7 }, { -1, 9,-1, 6, 5 },
              { -1, 8, 6,-1, 4 }, { -1, 7, 5, 4,-1 } },
            { { -1,-1, 9, 8, 7 }, { -1,-1,-1,-1,-1 }, {  9,-1,-1, 3, 2 },
              {  8,-1, 3,-1, 1 }, {  7,-1, 2, 1,-1 } },
            { { -1, 9,-1, 6, 5 }, {  9,-1,-1, 3, 2 }, { -1,-1,-1,-1,-1 },
              {  6, 3,-1,-1, 0 }, {  5, 2,-1, 0,-1 } },
            { { -1, 8, 6,-1, 4 }, {  8,-1, 3,-1, 1 }, {  6, 3,-1,-1, 0 },
              { -1,-1,-1,-1,-1 }, {  4, 1, 0,-1,-1 } },
            { { -1, 7, 5, 4,-1 }, {  7,-1, 2, 1,-1 }, {  5, 2,-1, 0,-1 },
              {  4, 1, 0,-1,-1 }, { -1,-1,-1,-1,-1 } }
        };

        /**
         * A table that maps triangles of a pentachoron to vertex numbers.
         *
         * Triangles in a pentachoron are numbered 0,...,9.  This table converts
         * triangle numbers to vertices; in particular, triangle \a i in a
         * pentachoron is spanned by vertices `triangleVertex[i][0]`,
         * `triangleVertex[i][1]` and `triangleVertex[i][2]`.
         * Here \a i must be between 0 and 9 inclusive; the resulting
         * vertex numbers will be between 0 and 4 inclusive.
         *
         * It is guaranteed that `triangleVertex[i][0]` will always
         * be smaller than `triangleVertex[i][1]`, which in turn will
         * always be smaller than `triangleVertex[i][2]`.
         *
         * \note Accessing `triangleVertex[i][j]` is equivalent to
         * calling `ordering(i)[j]`.
         */
        static constexpr int triangleVertex[10][3] = {
            { 2, 3, 4 }, { 1, 3, 4 }, { 1, 2, 4 }, { 1, 2, 3 }, { 0, 3, 4 },
            { 0, 2, 4 }, { 0, 2, 3 }, { 0, 1, 4 }, { 0, 1, 3 }, { 0, 1, 2 }
        };

    protected:
        /**
         * Indicates whether Regina provides private lookup tables for the
         * ordering() function.
         */
        static constexpr bool hasOrderingTables = true;

        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<5>::Code2 faceOrdering[10] = {
            64, 40, 34, 32, 16, 10, 8, 4, 2, 0
        };
};

template <>
class FaceNumberingTables<4, 3> {
    public:
        /**
         * Indicates whether Regina provides public lookup tables for this
         * combination of triangulation and face dimensions.
         */
        static constexpr bool hasNumberingTables = false;

    protected:
        /**
         * Indicates whether Regina provides private lookup tables for the
         * ordering() function.
         */
        static constexpr bool hasOrderingTables = true;

        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<5>::Code2 faceOrdering[5] = { 32, 9, 2, 1, 0 };
};

} // namespace detail

/**
 * Specifies how <i>subdim</i>-faces are numbered within a
 * <i>dim</i>-dimensional simplex.
 *
 * Regina uses the following general scheme for numbering faces:
 *
 * - For low-dimensional faces (`subdim < dim / 2`), faces are
 *   numbered in lexicographical order according to their vertices.
 *   For example, in a 3-dimensional triangulation, edges 0,...,5 contain
 *   vertices 01, 02, 03, 12, 13, 23 respectively.
 *
 * - For high-dimensional faces (`subdim ≥ dim / 2`), faces are
 *   numbered in _reverse_ lexicographical order according to their vertices.
 *   For example, in a 3-dimensional triangulation, triangles 0,...,3 contain
 *   vertices 123, 023, 013, 012 respectively.
 *
 * - As a consequence, unless `subdim = (dim-1)/2`, we always have
 *   <i>subdim</i>-face number \a i opposite (<i>dim</i>-1-<i>subdim</i>)-face
 *   number \a i.  For the special "halfway case" `subdim = (dim-1)/2`,
 *   where each <i>subdim</i>-face is opposite another <i>subdim</i>-face,
 *   we always have <i>subdim</i>-face number \a i opposite
 *   <i>subdim</i>-face number `(nFaces-1-i)`.
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
 * \python This class is not available in Python.  However, all of
 * its routines can be accessed through Face<dim, subdim> (which in Python
 * becomes Face<i>dim</i>_<i>subdim</i>, or one of the type aliases such as
 * Vertex3, Edge2 and so on).
 *
 * \tparam dim the dimension of the simplex whose faces we are numbering.
 * Note that dimension 1 _is_ supported for the purpose of face numbering,
 * even though it is not supported for building fully-fledged triangulations.
 * \tparam subdim the dimension of the faces that we are numbering.
 *
 * \ingroup triangulation
 */
template <int dim, int subdim>
requires (dim >= 1 && dim <= maxDim() && subdim >= 0 && subdim < dim)
class FaceNumbering : public detail::FaceNumberingTables<dim, subdim> {
    public:
        static constexpr int oppositeDim = (dim - 1 - subdim);
            /**< The dimension of the faces opposite these in a top-dimensional
                 simplex of a <i>dim</i>-dimensional triangulation. */

        static constexpr bool lexNumbering = (subdim <= oppositeDim);
            /**< \c true if faces are numbered in lexicographical order
                 according to their vertices, or \c false if faces are
                 numbered in reverse lexicographical order. */

    private:
        static constexpr int lexDim = (lexNumbering ? subdim : oppositeDim);
            /**< Whichever of \a subdim or \a oppositeDim uses lexicographical
                 face numbering. */

        using Tables = detail::FaceNumberingTables<dim, subdim>;
            /**< A type alias for easy access to the parent class. */

        using Perm_ = Perm<dim + 1>;
            /**< Shorthand for the relevant permutation class. */

    public:
        static constexpr int nFaces = binomSmall(dim + 1, lexDim + 1);
            /**< The total number of <i>subdim</i>-dimensional faces in each
                 <i>dim</i>-dimensional simplex. */

    public:
        /**
         * Given a <i>subdim</i>-face number within a <i>dim</i>-dimensional
         * simplex, returns the corresponding canonical ordering of the
         * simplex vertices.
         *
         * If this canonical ordering is \a c, then \a c[0,...,\a subdim]
         * will be the vertices of the given face in increasing numerical
         * order.  That is, \a c[0] \< ... \< \a c[\a subdim].
         * The remaining images \a c[(\a subdim + 1),...,\a dim] will
         * be ordered arbitrarily.
         *
         * Note that this is _not_ the same permutation as returned by
         * Simplex<dim>::faceMapping<subdim>():
         *
         * - ordering() is a static function, which returns the same permutation
         *   for the same face number, regardless of which <i>dim</i>-simplex
         *   we are looking at.  The images of 0,...,\a subdim will always
         *   appear in increasing order, and the images of
         *   (\a subdim + 1),...,\a dim will be arbitrary.
         *
         * - faceMapping() examines the underlying face \a F of the
         *   triangulation and, across all appearances of \a F in different
         *   <i>dim</i>-simplices: (i) chooses the images of 0,...,\a subdim to
         *   map to the same respective vertices of \a F; and (ii) chooses the
         *   images of (\a subdim + 1),...,\a dim to maintain a "consistent
         *   orientation" constraint.
         *
         * \param face identifies which <i>subdim</i>-face of a
         * <i>dim</i>-dimensional simplex to query.  This must be between
         * 0 and (<i>dim</i>+1 choose <i>subdim</i>+1)-1 inclusive.
         * \return the corresponding canonical ordering of the simplex vertices.
         */
        static constexpr Perm<dim + 1> ordering(int face) {
            if constexpr (Tables::hasOrderingTables) {
                if constexpr (dim <= 2) {
                    return Perm_::fromPermCode(Tables::faceOrdering[face]);
                } else {
                    return Perm_::fromPermCode2(Tables::faceOrdering[face]);
                }
            } else if constexpr (subdim == 0 && dim <= 4) {
                // Dimension 3 is special: for legacy reasons we do not use
                // rot(), but instead we hard-code the permutations that were
                // used in ancient versions of Regina (which give different
                // values for the unused elements of the permutation).
                static_assert(dim != 3);
                return Perm_::rot(face);
            } else {
                // Fall back to a generic implementation.
                static_assert(! standardDim(dim));

                if constexpr (subdim == 0) {
                    // Construct a permutation code from the individual images.
                    using ImagePack = typename Perm_::ImagePack;
                    ImagePack code = face; // 0 -> face

                    int shift = Perm_::imageBits;
                    for (int i = dim; i >= face + 1; --i) {
                        // dim - i + 1 -> i;
                        code |= (static_cast<ImagePack>(i) << shift);
                        shift += Perm_::imageBits;
                    }
                    for (int i = face - 1; i >= 0; --i) {
                        // dim - i -> i
                        code |= (static_cast<ImagePack>(i) << shift);
                        shift += Perm_::imageBits;
                    }
                    return Perm_::fromImagePack(code);
                } else if constexpr (subdim == dim - 1) {
                    // Construct a permutation code from the individual images.
                    using ImagePack = typename Perm_::ImagePack;
                    ImagePack code = 0;

                    int shift = 0;
                    for (int i = 0; i < face; ++i) {
                        // i -> i
                        code |= (static_cast<ImagePack>(i) << shift);
                        shift += Perm_::imageBits;
                    }
                    for (int i = face + 1; i <= dim; ++i) {
                        // i - 1 -> i
                        code |= (static_cast<ImagePack>(i) << shift);
                        shift += Perm_::imageBits;
                    }
                    // dim -> face
                    code |= (static_cast<ImagePack>(face) << shift);

                    return Perm_::fromImagePack(code);
                } else if constexpr (subdim == 1) {
                    std::array<int, dim + 1> image;

                    // Find the lower and upper numbered vertices on this face.
                    // See containsVertex() for the logic behind it.
                    int threshold = detail::binomSmall_[dim+1][2] - face;
                    int i = dim;
                    while (detail::binomSmall_[i][2] >= threshold)
                        --i;
                    image[0] = dim - i;
                    image[1] = image[0] + detail::binomSmall_[i+1][2]
                        - threshold + 1;

                    // Following the generic implementation of ordering(), we
                    // now list the remaining elements of the permutation in
                    // descending order.
                    int pos = 2;
                    int nextToAvoid = image[1];
                    for (i = dim; i >= 0; --i) {
                        if (i == nextToAvoid) {
                            // If we found the upper vertex, then now we need to
                            // avoid the lower vertex.
                            // If we found the lower vertex, this assignment is
                            // harmless since there are no other vertices to
                            // avoid.
                            nextToAvoid = image[0];
                        } else
                            image[pos++] = i;
                    }
                    return Perm_(image);
                } else {
                    // We are left with the cases 2 ≤ subdim ≤ dim - 2.

                    // We always compute face numbering in dimension lexDim,
                    // where faces are numbered in forward lexicographial order.

                    // This generic implementation MUST order the images of
                    // subdim+1, ..., dim in DESCENDING order, since for
                    // higher-dimensional faces (lexDim != subdim) we will
                    // reverse the permutation before returning.

                    // This implementation runs in linear time in dim
                    // (since binomial coefficients are precomputed).

                    // IDEA: use the combinatorial number system that associates
                    //       numbers face = 0, 1, .... , binom(dim+1,lexDim+1)-1
                    //       to sets of distinct integers
                    //       dim >= c_(lexDim+1) > ... c_1 >= 0
                    //       in lexicographic ordering.
                    //
                    // ALGM: the last vertex is the maximal number x_(lexDim)
                    //       such that
                    //       y_(lexDim) = ( x_(lexDim) \choose k ) <= remaining
                    //       the second last vertex is the maximal number
                    //       x_(lexDim-1) such that
                    //       y_(lexDim-1) = ( x_(lexDim-1) \choose k-1 ) <=
                    //       remaining - y_(lexDim)
                    //
                    // PROBLEM: we need lexicographic ordering
                    //       0 <= c_1 < ... < c_(lexDim+1) <= dim
                    //       so we must reverse the ordering and apply the
                    //       transformation c_i \mapsto d_i = dim-c_i

                    // We construct a permutation code from the individual
                    // images.

                    using ImagePack = typename Perm_::ImagePack;
                    ImagePack code = 0;
                    int shift = 0;

                    // reverse ordering
                    int remaining = detail::binomSmall_[dim+1][lexDim+1]
                        - face - 1;

                    int k = lexDim+1;
                    int max = dim;

                    while (remaining > 0) {
                        bool done = false;
                        while (! done) {
                            int val = (max < k ? 0 :
                                detail::binomSmall_[max][k]);
                            if (val <= remaining) {
                                --k;
                                // lexDim-k -> dim-max
                                code |=
                                    (static_cast<ImagePack>(dim-max) << shift);
                                shift += Perm_::imageBits;
                                remaining = remaining - val;
                                done = true;
                            }
                            --max;
                        }
                    }
                    while (k > 0) {
                        --k;
                        // lexDim-k -> dim-k
                        code |= (static_cast<ImagePack>(dim-k) << shift);
                        shift += Perm_::imageBits;
                    }

                    // At this point, shift == (lexDim + 1) * imageBits.

                    int shiftBack = shift - Perm_::imageBits;
                    bool done = false;
                    for (int i=dim; i>=0; i--) {
                        if ((! done) &&
                                ((code >> shiftBack) & Perm_::imageMask) == i) {
                            if (shiftBack > 0)
                                shiftBack -= Perm_::imageBits;
                            else
                                done = true;
                            continue;
                        }
                        // next index -> i
                        code |= (static_cast<ImagePack>(i) << shift);
                        shift += Perm_::imageBits;
                    }

                    if constexpr (lexNumbering)
                        return Perm_::fromImagePack(code);
                    else
                        return Perm_::fromImagePack(code).reverse();
                }
            }
        }

        /**
         * Identifies which edge in a <i>dim</i>-dimensional simplex joins the
         * two given vertices of the simplex.
         *
         * This is essentially a slicker implementation of
         * `faceNumber(Perm<dim + 1>)`, specifically for edge numbering,
         * that does not require an entire permutation to be passed as input.
         *
         * The two given vertex numbers must be distinct, and may appear in
         * either order (i.e., they do not need to be sorted).
         *
         * \param vertex0 some vertex number of a simplex; this must be between
         * 0 and \a dim inclusive.
         * \param vertex1 another vertex number of a simplex; this must also be
         * between 0 and \a dim inclusive, and must be different from
         * \a vertex0.
         * \return the number of the simplex edge spanned by the two given
         * vertices.  This will be between 0 and `(dim+1 choose 2)-1` inclusive.
         */
        static constexpr int faceNumber(int vertex0, int vertex1)
                requires (subdim == 1) {
            if constexpr (Tables::hasNumberingTables) {
                return Tables::edgeNumber[vertex0][vertex1];
            } else if constexpr (dim == 2) {
                return 3 - vertex0 - vertex1;
            } else {
                // Fall back to a generic implementation.
                static_assert(! standardDim(dim));

                // Let (u, v) = (vertex0, vertex1), where u < v.
                // Then the edge number is:
                //     [dim + (dim-1) + ... + (dim-u+1)] + (v-u-1)
                //   = (dim+1 choose 2) - (dim-u+1 choose 2) + (v-u-1).

                if (vertex0 > vertex1)
                    std::swap(vertex0, vertex1);
                return detail::binomSmall_[dim + 1][2]
                    - detail::binomSmall_[dim - vertex0 + 1][2]
                    + vertex1 - vertex0 - 1;
            }
        }

        /**
         * Identifies which <i>subdim</i>-face in a <i>dim</i>-dimensional
         * simplex is represented by the first (\a subdim + 1) elements of the
         * given permutation.
         *
         * In other words, this routine identifies which <i>subdim</i>-face
         * number within a <i>dim</i>-dimensional simplex spans vertices
         * `vertices[0, ..., subdim]`.
         *
         * For the special case `subdim == 1` (i.e., _edges_ in a
         * <i>dim</i>-dimensional simplex), you can also call this function in
         * the form `faceNumber(u, v)`, where \a u and \a v are two distinct
         * vertex numbers in the range `0 ≤ u,v ≤ dim`: this will return the
         * number of the edge spanned by simplex vertices \a u and \a v.
         * It does not matter whether \a u is smaller or larger than \a v.
         *
         * \param vertices a permutation whose first (\a subdim + 1)
         * elements represent some vertex numbers in a <i>dim</i>-simplex.
         * \return the corresponding <i>subdim</i>-face number in the
         * <i>dim</i>-simplex.  This will be between 0 and
         * (<i>dim</i>+1 choose <i>subdim</i>+1)-1 inclusive.
         */
        static constexpr int faceNumber(Perm<dim + 1> vertices) {
            if constexpr (Tables::hasNumberingTables) {
                // Our pre-cooked tables are only for edges and triangles.
                static_assert(subdim == 1 || subdim == 2);

                if constexpr (subdim == 1) {
                    return Tables::edgeNumber[vertices[0]][vertices[1]];
                } else {
                    return Tables::triangleNumber[vertices[0]][vertices[1]]
                        [vertices[2]];
                }
            } else if constexpr (subdim == 0) {
                return vertices[0];
            } else if constexpr (subdim == dim - 1) {
                return vertices[dim];
            } else if constexpr (subdim == 1) {
                return faceNumber(vertices[0], vertices[1]);
            } else {
                // Fall back to a generic implementation.
                static_assert(! standardDim(dim));

                // We always compute face numbering in dimension lexDim,
                // where faces are numbered in forward lexicographial order.
                //
                // For higher-dimensional faces (lexDim != subdim), we must
                // therefore reverse the permutation that was provided as input.
                if (! lexNumbering)
                    vertices = vertices.reverse();

                // This implementation runs in linear time in dim (assuming
                // binomial coefficients are precomputed)

                // IDEA: use the combinatorial number system which associates
                //       numbers face = 0, 1, .... , binom(dim+1,lexDim+1)-1
                //       to sets of distinct integers
                //       dim >= c_(lexDim+1) > ... c_1 >= 0
                //       in lexicographic ordering.
                //
                // ALGORITHM: the number N associated to the face vertices
                //            is given by
                //            N = binom (c_(lexDim+1),lexDim+1) +
                //                binom (c_(lexDim),lexDim) +
                //                ... +
                //                binom (c_1,1)
                //
                // PROBLEM: we need lexicographic ordering
                //       0 <= c_1 < ... < c_(lexDim+1) <= dim
                //       so we must reverse the ordering and apply the
                //       transformation c_i \mapsto d_i = dim-c_i

                // The (i)th bit of v will indicate whether i is a vertex of
                // this face.  We are using a bitmask here to avoid the need
                // to call std::sort(), which was not constexpr until C++20
                // (and possibly using a bitmask is faster anyway, since we know
                // we are sorting distinct integers in the range [0, dim]).
                static_assert(sizeof(unsigned) * 8 >= dim + 1);
                unsigned v = 0;
                for (int i = 0; i <= lexDim; ++i)
                    v |= ((unsigned)1 << vertices[i]);

                // Walk through the vertices from highest to lowest.
                int val = 0;
                int pos = 0;
                for (int i = dim; pos <= lexDim; --i) {
                  if (v & ((unsigned)1 << i)) {
                    // Vertex i is the (pos)th last vertex of this face.
                    if (dim - i > pos) {
                      val += detail::binomSmall_[dim-i][pos+1];
                    }
                    ++pos;
                  }
                }
                return detail::binomSmall_[dim+1][lexDim+1]-1-val;
            }
        }

        /**
         * Tests whether the given <i>subdim</i>-face of a
         * <i>dim</i>-dimensional simplex contains the given vertex
         * of the simplex.
         *
         * \param face a <i>subdim</i>-face number in a <i>dim</i>-simplex;
         * this must be between 0 and (<i>dim</i>+1 choose <i>subdim</i>+1)-1
         * inclusive.
         * \param vertex a vertex number in a <i>dim</i>-simplex; this must be
         * between 0 and \a dim inclusive.
         * \return \c true if and only if the given <i>subdim</i>-face
         * contains the given vertex.
         */
        static constexpr bool containsVertex(int face, int vertex) {
            if constexpr (Tables::hasNumberingTables) {
                // Our pre-cooked tables are only for edges and triangles.
                static_assert(subdim == 1 || subdim == 2);

                if constexpr (subdim == 1) {
                    return (vertex == Tables::edgeVertex[face][0] ||
                            vertex == Tables::edgeVertex[face][1]);
                } else {
                    return (vertex == Tables::triangleVertex[face][0] ||
                            vertex == Tables::triangleVertex[face][1] ||
                            vertex == Tables::triangleVertex[face][2]);
                }
            } else if constexpr (subdim == 0) {
                return (face == vertex);
            } else if constexpr (subdim == dim - 1) {
                return (face != vertex);
            } else {
                // Fall back to a generic implementation.
                static_assert(! standardDim(dim));

                if constexpr (subdim == 1) {
                    // Find the lower numbered vertex on this face.
                    // This is the largest u for which
                    //     face ≥ [dim + (dim-1) + ... + (dim-u+1)].
                    // Equivalently:
                    //     (dim-u+1 choose 2) ≥ (dim+1 choose 2) - face.

                    int threshold = detail::binomSmall_[dim+1][2] - face;
                    int i = dim;
                    while (detail::binomSmall_[i][2] >= threshold)
                        --i;

                    // The lower vertex is now u = dim - i, and the upper vertex
                    // is now u + binomSmall_[i+1][2] - threshold + 1.
                    int u = dim - i;
                    return (vertex == u ||
                        vertex == u + detail::binomSmall_[i+1][2]
                        - threshold + 1);
                } else {
                    // We always compute face numbering in dimension lexDim,
                    // where faces are numbered in forward lexicographial order.
                    //
                    // For higher-dimensional faces (lexDim != subdim), we
                    // simply flip true/false on return.

                    // TODO: Make this more efficient - we should be able to
                    // implement it "directly", without calling ordering().

                    // This implementation runs in linear time in lexDim
                    // (assuming binomial coefficients are precomputed).

                    int remaining = detail::binomSmall_[dim+1][lexDim+1]
                        - face - 1;

                    int k = lexDim+1;
                    int max = dim;

                    while (remaining > 0) {
                      bool done = false;
                      while (! done) {
                        int val = (max < k ? 0 : detail::binomSmall_[max][k]);
                        if (val <= remaining) {
                          --k;
                          if (vertex == dim-max)
                            return lexNumbering;
                          remaining = remaining - val;
                          done = true;
                        }
                        --max;
                      }

                    }
                    while (k > 0) {
                      --k;
                      if (vertex == dim-k) return lexNumbering;
                    }

                    return ! lexNumbering;
                }
            }
        }
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
 * \python Python does not support templates.  Instead, Python users
 * should call this function in the form `faceOppositeEdge(dim, i, j)`;
 * that is, the template parameter \a dim becomes the first argument of
 * the function.
 *
 * \tparam dim the dimension of simplex that we are working with.
 *
 * \param i the first vertex of an edge in a <i>dim</i>-dimensional simplex.
 * This must be between 0 and \a dim inclusive.
 * \param j the second vertex of an edge in a <i>dim</i>-dimensional simplex.
 * This must be between 0 and \a dim inclusive, and must be different from \a i.
 * \return the number of the (<i>dim</i>-2)-face opposite the given edge.
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
constexpr int faceOppositeEdge(int i, int j) {
    if constexpr (dim == 2) {
        // We want the vertex number opposite edge (i, j).
        return (3 - i - j);
    } else if constexpr (dim == 3) {
        // We want the edge number opposite edge (i, j).
        // We can get this using the 3-D lookup table.
        return 5 - FaceNumbering<3, 1>::edgeNumber[i][j];
    } else if constexpr (dim == 4) {
        // The triangle opposite edge (i, j) has the same number as edge(i, j).
        // We can get this using the 4-D lookup table.
        return FaceNumbering<4, 1>::edgeNumber[i][j];
    } else {
        // In dimension >= 5, the requested (dim-2)-face has the same number
        // as the edge (i, j), and edges are numbered in increasing
        // lexicographical order by their vertices.
        if (i > j)
            std::swap(i, j);

        int ans = detail::binomSmall_[dim + 1][2] - 1; // index of last edge
        if (i <= dim - 2)
            ans -= detail::binomSmall_[dim - i][2]; // index of last edge (i, _)
        return ans - (dim - j); // index of edge (i, j)
    }
}

} // namespace regina

#endif

