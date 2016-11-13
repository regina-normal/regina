
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
#include "maths/perm.h"

namespace regina {
namespace detail {

/**
 * \weakgroup detail
 * @{
 */

/**
 * A lookup table that stores (\a n choose \a k) for all \a n &le; \a 16.
 *
 * For all values 0 &le; \a k &le; \a n &le; 16, the value
 * \a binomSmall_[\a n][\a k] is the binomial coefficient (\a n choose \a k).
 *
 * This array is used in the implementation of the function binomSmall().
 * End users should call binomSmall() instead of referring to this array
 * directly.
 */
REGINA_API extern const int* const binomSmall_[17];

/**
 * Placeholder class that outlines the functions provided by
 * FaceNumbering<dim, subdim>.
 * This class exists merely to help with documentation.
 *
 * The class FaceNumbering<dim, subdim> details how the <i>subdim</i>-faces
 * of a <i>dim</i>-dimensional simplex are numbered.  Its implementation
 * is complex and involves many template specialisations, and so this base
 * class FaceNumberingAPI serves to put all the documentation in one place.
 *
 * End users should access the numbering scheme through either
 * FaceNumbering<dim, subdim> or its child class Face<dim, subdim>.
 * There is no need to refer to FaceNumberingAPI directly.
 *
 * The member functions described here are never implemented in the base
 * class FaceNumberingAPI; instead they are redeclared and implemented in the
 * various specialisations of the child class FaceNumberingImpl.
 *
 * Note that FaceNumberingAPI documents member functions only.  The
 * class FaceNumbering<dim, subdim> provides static constants also (in
 * particular, the constant \a nFaces).
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
 * \tparam lex \c true if faces are numbered in lexicographical order
 * according to their vertices (the scheme for low-dimensional faces),
 * or \c false if faces are numbered in reverse lexicographical order
 * (the scheme for high-dimensional faces).  The value of this parameter
 * is forced by \a dim and \a subdim; its purpose is to help with
 * template specialisations.
 */
template <int dim, int subdim>
class FaceNumberingAPI {
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
 * \tparam lex \c true if faces are numbered in lexicographical order
 * according to their vertices (the scheme for low-dimensional faces),
 * or \c false if faces are numbered in reverse lexicographical order
 * (the scheme for high-dimensional faces).  The value of this parameter
 * is forced by \a dim and \a subdim; its purpose is to help with
 * template specialisations.
 */
template <int dim, int subdim, bool lex>
class FaceNumberingImpl : public FaceNumberingAPI<dim, subdim> {
    static_assert((dim + 1) >= 2 * (subdim + 1),
        "The generic FaceNumberingImpl<dim, subdim, lex> class "
        "should only be used for low-dimensional faces.");
    static_assert(subdim > 0,
        "The generic FaceNumberingImpl<dim, subdim, lex> class "
        "should not be used for vertices (i.e., subdim == 0).");
    static_assert(lex,
        "The generic FaceNumberingImpl<dim, subdim, lex> class "
        "should only be used for lexicographic numbering (i.e., lex == true).");
    static_assert(! standardDim(dim),
        "The generic FaceNumberingImpl<dim, subdim, lex> class "
        "should not be used for Regina's standard dimensions.");

    public:
        /**
         * The total number of <i>subdim</i>-dimensional faces in each
         * <i>dim</i>-dimensional simplex.
         */
        static constexpr int nFaces =
            FaceNumberingImpl<dim - 1, subdim - 1, lex>::nFaces +
            FaceNumberingImpl<dim - 1, subdim,
                (dim >= 2 * (subdim + 1))>::nFaces;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<dim + 1> ordering(unsigned face) {
            // We can assume here that we are numbering faces in forward
            // lexicographical order (i.e., the face dimension subdim is small).

            // This generic implementation MUST order the images of
            // subdim+1, ..., dim in DESCENDING order, since the
            // implementation of ordering() for high-dimensional faces
            // calls this function and reverses the permutation.

            // This implementation runs in linear time in dim (assuming binomial
            // coefficients are precomputed)
            int perm[dim + 1];
            unsigned val;

            // IDEA: use the combinatorial number system which associates 
            //       numbers face = 0, 1, .... , binom(dim+1,subdim+1)-1 
            //       to sets of distinct integers 
            //       dim >= c_(subdim+1) > ... c_1 >= 0
            //       in lexicographic ordering.
            // 
            // ALGORITHM: the last vertex is the maximal number x_(subdim) such 
            //            that
            //            y_(subdim) = ( x_(subdim) \choose k ) <= remaining
            //            the second last vertex is the maximal number 
            //            x_(subdim-1) such that
            //            y_(subdim-1) = ( x_(subdim-1) \choose k-1 ) <= 
            //            remaining - y_(subdim)
            //
            // PROBLEM: we need lexicographic ordering 
            //       0 <= c_1 < ... < c_(subdim+1) <= dim
            //       so we must reverse the ordering and apply the
            //       transformation c_i \mapsto d_i = dim-c_i

            // reverse ordering
            unsigned remaining = binomSmall_[dim+1][subdim+1] - face - 1;

            unsigned k = subdim+1;
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
                  perm[subdim-k] = dim-max;
                  remaining = remaining - val;
                  done = 1;
                }
                max--;
              }

            }
            while (k > 0) {
              k--;
              perm[subdim-k]=dim-k;
            }

            pos = subdim;
            idx = subdim+1;
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

            return Perm<dim + 1>(perm);
        }

        static unsigned faceNumber(Perm<dim + 1> vertices) {
            // We can assume here that we are numbering faces in forward
            // lexicographical order (i.e., the face dimension subdim is small).

            // This implementation runs in linear time in dim (assuming 
            // binomial coefficients are precomputed)

            // IDEA: use the combinatorial number system which associates 
            //       numbers face = 0, 1, .... , binom(dim+1,subdim+1)-1 
            //       to sets of distinct integers 
            //       dim >= c_(subdim+1) > ... c_1 >= 0
            //       in lexicographic ordering.
            // 
            // ALGORITHM: the number N associated to the face vertices 
            //            is given by 
            //            N = binom (c_(subdim+1),subdim+1) + 
            //                binom (c_(subdim),subdim) + 
            //                ... + 
            //                binom (c_1,1) 
            //
            // PROBLEM: we need lexicographic ordering 
            //       0 <= c_1 < ... < c_(subdim+1) <= dim
            //       so we must reverse the ordering and apply the
            //       transformation c_i \mapsto d_i = dim-c_i

            unsigned i;

            int v[dim + 1];
            for (i = 0; i <= subdim; ++i)
                v[i] = vertices[i];

            // Sort the vertices of the face in increasing order.
            std::sort(v, v + subdim + 1);

            unsigned val = 0;
            for (i=0; i<=subdim; i++) {
              if (dim - v[subdim-i] >= i+1) {
                val += binomSmall_[dim-v[subdim-i]][i+1];
              }
            }
            return binomSmall_[dim+1][subdim+1]-1-val;
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            // We can assume here that we are numbering faces in forward
            // lexicographical order (i.e., the face dimension subdim is small).

            // TODO: Make this more efficient - we should be able to
            // implement it "directly", without calling ordering().

            // This implementation runs in linear time in subdim (assuming 
            // binomial coefficients are precomputed)


            unsigned remaining = binomSmall_[dim+1][subdim+1] - face - 1;

            unsigned k = subdim+1;
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
                  if (vertex == dim-max) return true;
                  remaining = remaining - val;
                  done = 1;
                }
                max--;
              }

            }
            while (k > 0) {
              k--;
              if (vertex == dim-k) return true;
            }

            return false;
        }
#endif // ! __DOXYGEN
};

template <int dim, int subdim>
class FaceNumberingImpl<dim, subdim, false> :
        public FaceNumberingAPI<dim, subdim> {
    static_assert((dim + 1) < 2 * (subdim + 1),
        "The specialisation FaceNumberingImpl<dim, subdim, false> "
        "should only be used for high-dimensional faces.");
    static_assert(! standardDim(dim),
        "The specialisation FaceNumberingImpl<dim, subdim, false> "
        "should not be used for Regina's standard dimensions.");

    public:
        /**
         * The total number of <i>subdim</i>-dimensional faces in each
         * <i>dim</i>-dimensional simplex.
         */
        static constexpr int nFaces =
            FaceNumberingImpl<dim, dim - subdim - 1, true>::nFaces;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<dim + 1> ordering(unsigned face) {
            return FaceNumberingImpl<dim, dim - subdim - 1, true>::
                ordering(face).reverse();
        }

        static unsigned faceNumber(Perm<dim + 1> vertices) {
            return FaceNumberingImpl<dim, dim - subdim - 1, true>::
                faceNumber(vertices.reverse());
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return ! FaceNumberingImpl<dim, dim - subdim - 1, true>::
                containsVertex(face, vertex);
        }
#endif // ! __DOXYGEN
};

template <int dim>
class FaceNumberingImpl<dim, 0, true> : public FaceNumberingAPI<dim, 0> {
    static_assert(! standardDim(dim),
        "The specialisation FaceNumberingImpl<dim, 0, true> "
        "should not be used for Regina's standard dimensions.");

    public:
        /**
         * The total number of vertices in each <i>dim</i>-dimensional simplex.
         */
        static constexpr int nFaces = dim + 1;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<dim + 1> ordering(unsigned face) {
            int p[dim + 1];
            p[0] = face;

            int i;
            for (i = 0; i < face; ++i)
                p[dim - i] = i;
            for (i = face + 1; i <= dim; ++i)
                p[dim - i + 1] = i;

            return Perm<dim + 1>(p);
        }

        static unsigned faceNumber(Perm<dim + 1> vertices) {
            return vertices[0];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face == vertex);
        }
#endif // ! __DOXYGEN
};

template <>
class REGINA_API FaceNumberingImpl<1, 0, true> : public FaceNumberingAPI<1, 0> {
    public:
        /**
         * The total number of vertices in each edge.
         */
        static constexpr int nFaces = 2;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<2> ordering(unsigned face) {
            return Perm<2>::fromPermCode(face);
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
class REGINA_API FaceNumberingImpl<2, 0, true> : public FaceNumberingAPI<2, 0> {
    public:
        /**
         * The total number of vertices in each triangle.
         */
        static constexpr int nFaces = 3;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<3> ordering(unsigned face) {
            return Perm<3>(face, (face + 1) % 3, (face + 2) % 3);
        }

        static unsigned faceNumber(Perm<3> vertices) {
            return vertices[0];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face == vertex);
        }
#endif // ! __DOXYGEN
};

template <>
class REGINA_API FaceNumberingImpl<2, 1, false> : public FaceNumberingAPI<2, 1> {
    private:
        static const Perm<3> ordering_[3];
            /**< A hard-coded list of all return values for ordering(). */

    public:
        /**
         * The total number of edges in each triangle.
         */
        static constexpr int nFaces = 3;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<3> ordering(unsigned face) {
            return ordering_[face];
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
class REGINA_API FaceNumberingImpl<3, 0, true> : public FaceNumberingAPI<3, 0> {
    public:
        /**
         * The total number of vertices in each tetrahedron.
         */
        static constexpr int nFaces = 4;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<4> ordering(unsigned face) {
            return (face % 2 == 0 ?
                Perm<4>(face, (face + 1) % 4, (face + 2) % 4, (face + 3) % 4) :
                Perm<4>(face, (face + 3) % 4, (face + 2) % 4, (face + 1) % 4));
        }

        static unsigned faceNumber(Perm<4> vertices) {
            return vertices[0];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face == vertex);
        }
#endif // ! __DOXYGEN
};

template <>
class REGINA_API FaceNumberingImpl<3, 1, true> : public FaceNumberingAPI<3, 1> {
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
        static const int edgeNumber[4][4];

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
        static const int edgeVertex[6][2];

    private:
        static const Perm<4> ordering_[6];
            /**< A hard-coded list of all return values for ordering(). */

    public:
        /**
         * The total number of edges in each tetrahedron.
         */
        static constexpr int nFaces = 6;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<4> ordering(unsigned face) {
            return ordering_[face];
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
class REGINA_API FaceNumberingImpl<3, 2, false> : public FaceNumberingAPI<3, 2> {
    private:
        static const Perm<4> ordering_[4];
            /**< A hard-coded list of all return values for ordering(). */

    public:
        /**
         * The total number of triangles in each tetrahedron.
         */
        static constexpr int nFaces = 4;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<4> ordering(unsigned face) {
            return ordering_[face];
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
class REGINA_API FaceNumberingImpl<4, 0, true> : public FaceNumberingAPI<4, 0> {
    public:
        /**
         * The total number of vertices in each pentachoron.
         */
        static constexpr int nFaces = 5;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<5> ordering(unsigned face) {
            return Perm<5>(face, (face + 1) % 5, (face + 2) % 5,
                (face + 3) % 5, (face + 4) % 5);
        }

        static unsigned faceNumber(Perm<5> vertices) {
            return vertices[0];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face == vertex);
        }
#endif // ! __DOXYGEN
};

template <>
class REGINA_API FaceNumberingImpl<4, 1, true> : public FaceNumberingAPI<4, 1> {
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
        static const int edgeNumber[5][5];

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
        static const int edgeVertex[10][2];

    private:
        static const Perm<5> ordering_[10];
            /**< A hard-coded list of all return values for ordering(). */

    public:
        /**
         * The total number of edges in each pentachoron.
         */
        static constexpr int nFaces = 10;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<5> ordering(unsigned face) {
            return ordering_[face];
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
class REGINA_API FaceNumberingImpl<4, 2, false> : public FaceNumberingAPI<4, 2> {
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
        static const int triangleNumber[5][5][5];

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
        static const int triangleVertex[10][3];

    private:
        static const Perm<5> ordering_[10];
            /**< A hard-coded list of all return values for ordering(). */

    public:
        /**
         * The total number of triangles in each pentachoron.
         */
        static constexpr int nFaces = 10;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<5> ordering(unsigned face) {
            return ordering_[face];
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
class REGINA_API FaceNumberingImpl<4, 3, false> : public FaceNumberingAPI<4, 3> {
    private:
        static const Perm<5> ordering_[5];
            /**< A hard-coded list of all return values for ordering(). */

    public:
        /**
         * The total number of tetrahedra in each pentachoron.
         */
        static constexpr int nFaces = 5;

#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static Perm<5> ordering(unsigned face) {
            return ordering_[face];
        }

        static unsigned faceNumber(Perm<5> vertices) {
            return vertices[4];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face != vertex);
        }
#endif // ! __DOXYGEN
};

} } // namespace regina::detail

#endif

