
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

/*! \file triangulation/detail/facenumbering.h
 *  \brief Implementation details for describing how <i>subdim</i>-faces
 *  are numbered within a <i>dim</i>-dimensional simplex.
 */

#ifndef __FACENUMBERING_H_DETAIL
#ifndef __DOXYGEN
#define __FACENUMBERING_H_DETAIL
#endif

#include <algorithm>
#include "regina-core.h"
#include "maths/binom.h"
#include "maths/perm.h"

namespace regina::detail {

/**
 * \weakgroup detail
 * @{
 */

/**
 * Placeholder class that outlines the functions provided by
 * FaceNumbering<dim, subdim>.
 * This class exists mainly to help with documentation.
 *
 * The class FaceNumbering<dim, subdim> details how the <i>subdim</i>-faces
 * of a <i>dim</i>-dimensional simplex are numbered.  Its implementation
 * involves many template specialisations, and so this base class
 * FaceNumberingAPI serves to put all the documentation and class constants
 * in one place.
 *
 * End users should access the numbering scheme through either
 * FaceNumbering<dim, subdim> or its child class Face<dim, subdim>.
 * There is no need to refer to FaceNumberingAPI directly.
 *
 * The member functions described here are never implemented in the base
 * class FaceNumberingAPI; instead they are redeclared and implemented in the
 * various specialisations of the child class FaceNumberingImpl.
 *
 * See the FaceNumbering template class notes for further information,
 * including details of how the face numbering scheme works.
 *
 * \ifacespython This base class is not present, and neither is
 * FaceNumbering<dim, subdim>.  Python users can access these routines
 * through the class Face<dim, subdim> (which in Python
 * becomes Face<i>dim</i>_<i>subdim</i>, or one of the type aliases
 * such as Vertex3, Edge2 and so on).
 *
 * \tparam dim the dimension of the simplex whose faces are being numbered.
 * This must be between 1 and 15 inclusive.
 * \tparam subdim the dimension of the faces being numbered.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class FaceNumberingAPI {
    static_assert(0 <= subdim && subdim < dim,
        "FaceNumberingAPI<dim, subdim> requires 0 <= subdim < dim.");

    public:
        static constexpr int oppositeDim = (dim - 1 - subdim);
            /**< The dimension of the faces opposite these in a top-dimensional
                 simplex of a <i>dim</i>-dimensional triangulation. */

        static constexpr bool lexNumbering = (subdim <= oppositeDim);
            /**< \c true if faces are numbered in lexicographical order
                 according to their vertices, or \c false if faces are
                 numbered in reverse lexicographical order. */

    protected:
        static constexpr int lexDim = (lexNumbering ? subdim : oppositeDim);
            /**< Whichever of \a subdim or \a oppositeDim uses lexicographical
                 face numbering. */

    public:
        static constexpr int nFaces = binomSmall(dim + 1, lexDim + 1);
            /**< The total number of <i>subdim</i>-dimensional faces in each
                 <i>dim</i>-dimensional simplex. */

#ifdef __DOXYGEN
    public:
        /**
         * Given a <i>subdim</i>-face number within a <i>dim</i>-dimensional
         * simplex, returns the corresponding canonical ordering of the
         * simplex vertices.
         *
         * If this canonical ordering is \a c, then \a c[0,...,\a subdim]
         * will be the vertices of the given face in increasing numerical
         * order.  That is, \a c[0] &lt; ... &lt; \a c[\a subdim].
         * The remaining images \a c[(\a subdim + 1),...,\a dim] will
         * be ordered arbitrarily.
         *
         * Note that this is \e not the same permutation as returned by
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
         * @param face identifies which <i>subdim</i>-face of a
         * <i>dim</i>-dimensional simplex to query.  This must be between
         * 0 and (<i>dim</i>+1 choose <i>subdim</i>+1)-1 inclusive.
         * @return the corresponding canonical ordering of the simplex vertices.
         */
        static Perm<dim + 1> ordering(unsigned face);

        /**
         * Identifies which <i>subdim</i>-face in a <i>dim</i>-dimensional
         * simplex is represented by the first (\a subdim + 1) elements of the
         * given permutation.
         *
         * In other words, this routine identifies which <i>subdim</i>-face
         * number within a <i>dim</i>-dimensional simplex spans vertices
         * <tt>vertices[0, ..., \a subdim]</tt>.
         *
         * @param vertices a permutation whose first (\a subdim + 1)
         * elements represent some vertex numbers in a <i>dim</i>-simplex.
         * @return the corresponding <i>subdim</i>-face number in the
         * <i>dim</i>-simplex.  This will be between 0 and
         * (<i>dim</i>+1 choose <i>subdim</i>+1)-1 inclusive.
         */
        static unsigned faceNumber(Perm<dim + 1> vertices);

        /**
         * Tests whether the given <i>subdim</i>-face of a
         * <i>dim</i>-dimensional simplex contains the given vertex
         * of the simplex.
         *
         * @param face a <i>subdim</i>-face number in a <i>dim</i>-simplex;
         * this must be between 0 and (<i>dim</i>+1 choose <i>subdim</i>+1)-1
         * inclusive.
         * @param vertex a vertex number in a <i>dim</i>-simplex; this must be
         * between 0 and \a dim inclusive.
         * @return \c true if and only if the given <i>subdim</i>-face
         * contains the given vertex.
         */
        static bool containsVertex(unsigned face, unsigned vertex);
#endif // __DOXYGEN
};

/**
 * Implementation details for numbering <i>subdim</i>-faces of a
 * <i>dim</i>-dimensional simplex.
 *
 * This numbering scheme can be accessed via FaceNumbering<dim, subdim> or
 * Face<dim, subdim>, both of which use this as a base class.
 * End users should not need to refer to FaceNumberingImpl directly.
 *
 * See the FaceNumbering template class notes for further information,
 * including details of how the face numbering scheme works.
 *
 * \ifacespython This base class is not present, and neither is
 * FaceNumbering<dim, subdim>.  Python users can access these routines
 * through the class Face<dim, subdim> (which in Python
 * becomes Face<i>dim</i>_<i>subdim</i>, or one of the type aliases
 * such as Vertex3, Edge2 and so on).
 *
 * \tparam dim the dimension of the simplex whose faces are being numbered.
 * This must be between 1 and 15 inclusive.
 * \tparam subdim the dimension of the faces being numbered.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class FaceNumberingImpl : public FaceNumberingAPI<dim, subdim> {
    static_assert(subdim > 0,
        "The generic FaceNumberingImpl<dim, subdim> class "
        "should not be used for vertices (i.e., subdim == 0).");
    static_assert(subdim < dim - 1,
        "The generic FaceNumberingImpl<dim, subdim> class "
        "should not be used for (dim-1)-faces.");
    static_assert(! standardDim(dim),
        "The generic FaceNumberingImpl<dim, subdim> class "
        "should not be used for Regina's standard dimensions.");

#ifndef __DOXYGEN
    public:
        using FaceNumberingAPI<dim, subdim>::lexNumbering;
        using FaceNumberingAPI<dim, subdim>::lexDim;

    public:
        // The following routines are documented in FaceNumberingAPI.
        static Perm<dim + 1> ordering(unsigned face) {
            // We always compute face numbering in dimension lexDim,
            // where faces are numbered in forward lexicographial order.

            // This generic implementation MUST order the images of
            // subdim+1, ..., dim in DESCENDING order, since for
            // higher-dimensional faces (lexDim != subdim) we will
            // reverse the permutation before returning.

            // This implementation runs in linear time in dim (assuming binomial
            // coefficients are precomputed)
            int perm[dim + 1];
            unsigned val;

            // IDEA: use the combinatorial number system which associates 
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

            // reverse ordering
            unsigned remaining = binomSmall_[dim+1][lexDim+1] - face - 1;

            unsigned k = lexDim+1;
            unsigned max = dim;
            unsigned done, pos, idx;
            int i;

            while (remaining > 0) {
              done = 0;
              while (done == 0) {
                if (max < k) {
                  val = 0;
                } else {
                  val = binomSmall_[max][k];
                }
                if (val <= remaining) {
                  k--;
                  perm[lexDim-k] = dim-max;
                  remaining = remaining - val;
                  done = 1;
                }
                max--;
              }

            }
            while (k > 0) {
              k--;
              perm[lexDim-k]=dim-k;
            }

            pos = lexDim;
            idx = lexDim+1;
            done = 0;
            for (i=dim; i>=0; i--) {
              if (done == 0 && perm[pos] == i) {
                if (pos>0) {
                  pos--;
                } else {
                  done = 1;
                }
                continue;
              }
              perm[idx] = i;
              idx++;
            }

            if constexpr (lexNumbering)
                return Perm<dim + 1>(perm);
            else
                return Perm<dim + 1>(perm).reverse();
        }

        static unsigned faceNumber(Perm<dim + 1> vertices) {
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

            unsigned i;

            int v[dim + 1];
            for (i = 0; i <= lexDim; ++i)
                v[i] = vertices[i];

            // Sort the vertices of the face in increasing order.
            std::sort(v, v + lexDim + 1);

            unsigned val = 0;
            for (i=0; i<=lexDim; i++) {
              if (dim - v[lexDim-i] >= i+1) {
                val += binomSmall_[dim-v[lexDim-i]][i+1];
              }
            }
            return binomSmall_[dim+1][lexDim+1]-1-val;
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            // We always compute face numbering in dimension lexDim,
            // where faces are numbered in forward lexicographial order.
            //
            // For higher-dimensional faces (lexDim != subdim), we simply
            // flip true/false on return.

            // TODO: Make this more efficient - we should be able to
            // implement it "directly", without calling ordering().

            // This implementation runs in linear time in lexDim (assuming 
            // binomial coefficients are precomputed)


            unsigned remaining = binomSmall_[dim+1][lexDim+1] - face - 1;

            unsigned k = lexDim+1;
            unsigned max = dim;
            unsigned done,val;

            while (remaining > 0) {
              done = 0;
              while (done == 0) {
                if (max < k) {
                  val = 0;
                } else {
                  val = binomSmall_[max][k];
                }
                if (val <= remaining) {
                  k--;
                  if (vertex == dim-max) return lexNumbering;
                  remaining = remaining - val;
                  done = 1;
                }
                max--;
              }

            }
            while (k > 0) {
              k--;
              if (vertex == dim-k) return lexNumbering;
            }

            return ! lexNumbering;
        }
#endif // ! __DOXYGEN
};

template <int dim>
class FaceNumberingImpl<dim, 0> : public FaceNumberingAPI<dim, 0> {
    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static constexpr Perm<dim + 1> ordering(unsigned face) {
            if constexpr (dim == 3) {
                switch (face) {
                    case 1: return Perm<4>::fromPermCode2(6); // 1032
                    case 2: return Perm<4>::fromPermCode2(16); // 2301
                    case 3: return Perm<4>::fromPermCode2(22); // 3210
                    default: return Perm<4>(); // 0123
                }
            } else if constexpr (dim <= 4) {
                return Perm<dim + 1>::rot(face);
            } else {
                int p[dim + 1];
                p[0] = face;

                for (int i = 0; i < face; ++i)
                    p[dim - i] = i;
                for (int i = face + 1; i <= dim; ++i)
                    p[dim - i + 1] = i;

                return Perm<dim + 1>(p);
            }
        }

        static unsigned faceNumber(Perm<dim + 1> vertices) {
            return vertices[0];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face == vertex);
        }
#endif // ! __DOXYGEN
};

template <int dim>
class FaceNumberingImpl<dim, dim - 1> : public FaceNumberingAPI<dim, dim - 1> {
    static_assert(! standardDim(dim),
        "The specialisation FaceNumberingImpl<dim, dim-1> "
        "should not be used for Regina's standard dimensions.");

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static constexpr Perm<dim + 1> ordering(unsigned face) {
            int p[dim + 1];

            for (int i = 0; i < face; ++i)
                p[i] = i;
            for (int i = face + 1; i <= dim; ++i)
                p[i - 1] = i;
            p[dim] = face;

            return Perm<dim + 1>(p);
        }

        static unsigned faceNumber(Perm<dim + 1> vertices) {
            return vertices[dim];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face != vertex);
        }
#endif // ! __DOXYGEN
};

template <>
class FaceNumberingImpl<1, 0> : public FaceNumberingAPI<1, 0> {
    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static constexpr Perm<2> ordering(unsigned face) {
            return Perm<2>::rot(face);
        }

        static unsigned faceNumber(Perm<2> vertices) {
            return vertices[0];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face == vertex);
        }
#endif // ! __DOXYGEN
};

template <>
class REGINA_API FaceNumberingImpl<2, 1> : public FaceNumberingAPI<2, 1> {
    private:
        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<3>::Code ordering_[3] = { 2, 1, 0 };

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<3> ordering(unsigned face) {
            return Perm<3>::fromPermCode(ordering_[face]);
        }

        static unsigned faceNumber(Perm<3> vertices) {
            return vertices[2];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face != vertex);
        }
#endif // ! __DOXYGEN
};

template <>
class REGINA_API FaceNumberingImpl<3, 1> : public FaceNumberingAPI<3, 1> {
    public:
        /**
         * A table that maps vertices of a tetrahedron to edge numbers.
         *
         * Edges in a tetrahedron are numbered 0,...,5.  This table
         * converts vertices to edge numbers; in particular, the edge
         * joining vertices \a i and \a j of a tetrahedron is edge
         * number <tt>edgeNumber[i][j]</tt>.  Here \a i and \a j must be
         * distinct, must be between 0 and 3 inclusive, and may be given
         * in any order.  The resulting edge number will be between 0 and 5
         * inclusive.
         *
         * \note Accessing <tt>edgeNumber[i][j]</tt> is equivalent to calling
         * <tt>faceNumber(p)</tt>, where \a p is a permutation that maps 
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
         * in a tetrahedron joins vertices <tt>edgeVertex[i][0]</tt> and
         * <tt>edgeVertex[i][1]</tt>.  Here \a i must be bewteen 0 and 5
         * inclusive; the resulting vertex numbers will be between 0 and 3
         * inclusive.
         *
         * It is guaranteed that <tt>edgeVertex[i][0]</tt> will always
         * be smaller than <tt>edgeVertex[i][1]</tt>.
         *
         * \note Accessing <tt>edgeVertex[i][j]</tt> is equivalent to
         * calling <tt>ordering(i)[j]</tt>.
         */
        static constexpr int edgeVertex[6][2] = {
            { 0, 1 }, { 0, 2 }, { 0, 3 }, { 1, 2 }, { 1, 3 }, { 2, 3 }
        };

    private:
        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<4>::Code2 ordering_[6] = { 0, 2, 4, 8, 10, 16 };

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<4> ordering(unsigned face) {
            return Perm<4>::fromPermCode2(ordering_[face]);
        }

        static unsigned faceNumber(Perm<4> vertices) {
            return edgeNumber[vertices[0]][vertices[1]];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (vertex == edgeVertex[face][0] ||
                    vertex == edgeVertex[face][1]);
        }
#endif // ! __DOXYGEN
};

template <>
class REGINA_API FaceNumberingImpl<3, 2> : public FaceNumberingAPI<3, 2> {
    private:
        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<4>::Code2 ordering_[4] = { 9, 2, 1, 0 };

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<4> ordering(unsigned face) {
            return Perm<4>::fromPermCode2(ordering_[face]);
        }

        static unsigned faceNumber(Perm<4> vertices) {
            return vertices[3];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face != vertex);
        }
#endif // ! __DOXYGEN
};

template <>
class REGINA_API FaceNumberingImpl<4, 1> : public FaceNumberingAPI<4, 1> {
    public:
        /**
         * A table that maps vertices of a pentachoron to edge numbers.
         *
         * Edges in a pentachoron are numbered 0,...,9.  This table
         * converts vertices to edge numbers; in particular, the edge
         * joining vertices \a i and \a j of a pentachoron is
         * edge number <tt>edgeNumber[i][j]</tt>.  Here \a i and \a j
         * must be distinct, must be between 0 and 4 inclusive, and may
         * be given in any order.  The resulting edge number will be
         * between 0 and 9 inclusive.
         *
         * \note Accessing <tt>edgeNumber[i][j]</tt> is equivalent to calling
         * <tt>faceNumber(p)</tt>, where \a p is a permutation that maps 
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
         * in a pentachoron joins vertices <tt>edgeVertex[i][0]</tt> and
         * <tt>edgeVertex[i][1]</tt>.  Here \a i must be between 0 and 9
         * inclusive; the resulting vertex numbers will be between 0 and 4
         * inclusive.
         *
         * It is guaranteed that <tt>edgeVertex[i][0]</tt> will always
         * be smaller than <tt>edgeVertex[i][1]</tt>.
         *
         * \note Accessing <tt>edgeVertex[i][j]</tt> is equivalent to
         * calling <tt>ordering(i)[j]</tt>.
         */
        static constexpr int edgeVertex[10][2] = {
            { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 1, 2 },
            { 1, 3 }, { 1, 4 }, { 2, 3 }, { 2, 4 }, { 3, 4 }
        };

    private:
        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<5>::Code2 ordering_[10] = {
            0, 6, 12, 18, 30, 36, 42, 60, 66, 90
        };

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<5> ordering(unsigned face) {
            return Perm<5>::fromPermCode2(ordering_[face]);
        }

        static unsigned faceNumber(Perm<5> vertices) {
            return edgeNumber[vertices[0]][vertices[1]];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (vertex == edgeVertex[face][0] ||
                    vertex == edgeVertex[face][1]);
        }
#endif // ! __DOXYGEN
};

template <>
class REGINA_API FaceNumberingImpl<4, 2> : public FaceNumberingAPI<4, 2> {
    public:
        /**
         * A table that maps vertices of a pentachoron to triangle numbers.
         *
         * Triangles in a pentachoron are numbered 0,...,9.  This table
         * converts vertices to triangle numbers; in particular, the triangle
         * spanned by vertices \a i, \a j and \a k of a pentachoron is triangle
         * number <tt>triangleNumber[i][j][k]</tt>.  Here \a i, \a j and \a k
         * must be distinct, must be between 0 and 4 inclusive, and may
         * be given in any order.  The resulting triangle number will be
         * between 0 and 9 inclusive.
         *
         * \note Accessing <tt>triangleNumber[i][j][k]</tt> is equivalent to
         * calling <tt>faceNumber(p)</tt>, where \a p is a permutation that
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
         * pentachoron is spanned by vertices <tt>triangleVertex[i][0]</tt>,
         * <tt>triangleVertex[i][1]</tt> and <tt>triangleVertex[i][2]</tt>.
         * Here \a i must be between 0 and 9 inclusive; the resulting
         * vertex numbers will be between 0 and 4 inclusive.
         *
         * It is guaranteed that <tt>triangleVertex[i][0]</tt> will always
         * be smaller than <tt>triangleVertex[i][1]</tt>, which in turn will
         * always be smaller than <tt>triangleVertex[i][2]</tt>.
         *
         * \note Accessing <tt>triangleVertex[i][j]</tt> is equivalent to
         * calling <tt>ordering(i)[j]</tt>.
         */
        static constexpr int triangleVertex[10][3] = {
            { 2, 3, 4 }, { 1, 3, 4 }, { 1, 2, 4 }, { 1, 2, 3 }, { 0, 3, 4 },
            { 0, 2, 4 }, { 0, 2, 3 }, { 0, 1, 4 }, { 0, 1, 3 }, { 0, 1, 2 }
        };

    private:
        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<5>::Code2 ordering_[10] = {
            64, 40, 34, 32, 16, 10, 8, 4, 2, 0
        };

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<5> ordering(unsigned face) {
            return Perm<5>::fromPermCode2(ordering_[face]);
        }

        static unsigned faceNumber(Perm<5> vertices) {
            return triangleNumber[vertices[0]][vertices[1]][vertices[2]];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (vertex == triangleVertex[face][0] ||
                    vertex == triangleVertex[face][1] ||
                    vertex == triangleVertex[face][2]);
        }
#endif // ! __DOXYGEN
};

template <>
class REGINA_API FaceNumberingImpl<4, 3> : public FaceNumberingAPI<4, 3> {
    private:
        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<5>::Code2 ordering_[5] = { 32, 9, 2, 1, 0 };

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<5> ordering(unsigned face) {
            return Perm<5>::fromPermCode2(ordering_[face]);
        }

        static unsigned faceNumber(Perm<5> vertices) {
            return vertices[4];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face != vertex);
        }
#endif // ! __DOXYGEN
};

/*@}*/

} // namespace regina::detail

#endif

