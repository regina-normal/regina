
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

/*! \file triangulation/detail/facenumbering.h
 *  \brief Implementation details for describing how <i>subdim</i>-faces
 *  are numbered within a <i>dim</i>-dimensional simplex.
 */

#ifndef __REGINA_FACENUMBERING_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_FACENUMBERING_H_DETAIL
#endif

#include <algorithm>
#include "regina-core.h"
#include "maths/binom.h"
#include "maths/perm.h"

namespace regina::detail {

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
 * \python This base class is not present, and neither is
 * FaceNumbering<dim, subdim>.  Python users can access these routines
 * through the class Face<dim, subdim> (which in Python
 * becomes Face<i>dim</i>_<i>subdim</i>, or one of the type aliases
 * such as Vertex3, Edge2 and so on).
 *
 * \tparam dim the dimension of the simplex whose faces are being numbered.
 * This must be between 1 and 15 inclusive.
 * \tparam subdim the dimension of the faces being numbered.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 *
 * \ingroup detail
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

#ifdef __APIDOCS
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
        static constexpr Perm<dim + 1> ordering(int face);

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
        static constexpr int faceNumber(Perm<dim + 1> vertices);

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
        static constexpr bool containsVertex(int face, int vertex);
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
 * \python This base class is not present, and neither is
 * FaceNumbering<dim, subdim>.  Python users can access these routines
 * through the class Face<dim, subdim> (which in Python
 * becomes Face<i>dim</i>_<i>subdim</i>, or one of the type aliases
 * such as Vertex3, Edge2 and so on).
 *
 * \tparam dim the dimension of the simplex whose faces are being numbered.
 * This must be between 1 and 15 inclusive.
 * \tparam subdim the dimension of the faces being numbered.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 * \tparam codim the codimension (<i>dim</i>-<i>subdim</i>-1) of the
 * faces being numbered.  Ideally this would be specified directly as
 * `dim-subdim-1` in the partial template specialisation, and this
 * _should_ be legal according to CWG1315; however, it fails to build
 * under some versions of gcc (e.g., 10.2.0).
 *
 * \ingroup detail
 */
template <int dim, int subdim, int codim = dim - subdim - 1>
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
    static_assert(subdim + codim + 1 == dim,
        "The FaceNumberingImpl<dim, subdim, codim> template class "
        "has mismatched face dimension and codimension.");

#ifndef __DOXYGEN
    public:
        using FaceNumberingAPI<dim, subdim>::lexNumbering;
        using FaceNumberingAPI<dim, subdim>::lexDim;

    public:
        // The following routines are documented in FaceNumberingAPI.
        static constexpr Perm<dim + 1> ordering(int face) {
            // We always compute face numbering in dimension lexDim,
            // where faces are numbered in forward lexicographial order.

            // This generic implementation MUST order the images of
            // subdim+1, ..., dim in DESCENDING order, since for
            // higher-dimensional faces (lexDim != subdim) we will
            // reverse the permutation before returning.

            // This implementation runs in linear time in dim
            // (since binomial coefficients are precomputed).

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

            // We construct a permutation code from the individual images.

            using ImagePack = typename Perm<dim + 1>::ImagePack;
            ImagePack code = 0;
            int shift = 0;

            // reverse ordering
            int remaining = binomSmall_[dim+1][lexDim+1] - face - 1;

            int k = lexDim+1;
            int max = dim;

            while (remaining > 0) {
              bool done = false;
              while (! done) {
                int val = (max < k ? 0 : binomSmall_[max][k]);
                if (val <= remaining) {
                  --k;
                  // lexDim-k -> dim-max
                  code |= (static_cast<ImagePack>(dim-max) << shift);
                  shift += Perm<dim + 1>::imageBits;
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
              shift += Perm<dim + 1>::imageBits;
            }

            // At this point, shift == (lexDim + 1) * imageBits.

            int shiftBack = shift - Perm<dim + 1>::imageBits;
            bool done = false;
            for (int i=dim; i>=0; i--) {
              if ((! done) &&
                  ((code >> shiftBack) & Perm<dim + 1>::imageMask) == i) {
                if (shiftBack>0) {
                  shiftBack -= Perm<dim + 1>::imageBits;
                } else {
                  done = true;
                }
                continue;
              }
              // next index -> i
              code |= (static_cast<ImagePack>(i) << shift);
              shift += Perm<dim + 1>::imageBits;
            }

            if constexpr (lexNumbering)
                return Perm<dim + 1>::fromImagePack(code);
            else
                return Perm<dim + 1>::fromImagePack(code).reverse();
        }

        static constexpr int faceNumber(Perm<dim + 1> vertices) {
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
                  val += binomSmall_[dim-i][pos+1];
                }
                ++pos;
              }
            }
            return binomSmall_[dim+1][lexDim+1]-1-val;
        }

        static constexpr bool containsVertex(int face, int vertex) {
            // We always compute face numbering in dimension lexDim,
            // where faces are numbered in forward lexicographial order.
            //
            // For higher-dimensional faces (lexDim != subdim), we simply
            // flip true/false on return.

            // TODO: Make this more efficient - we should be able to
            // implement it "directly", without calling ordering().

            // This implementation runs in linear time in lexDim (assuming 
            // binomial coefficients are precomputed)


            int remaining = binomSmall_[dim+1][lexDim+1] - face - 1;

            int k = lexDim+1;
            int max = dim;

            while (remaining > 0) {
              bool done = false;
              while (! done) {
                int val = (max < k ? 0 : binomSmall_[max][k]);
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
#endif // ! __DOXYGEN
};

template <int dim, int codim>
class FaceNumberingImpl<dim, 0, codim> : public FaceNumberingAPI<dim, 0> {
    static_assert(codim + 1 == dim,
        "The FaceNumberingImpl<dim, 0, codim> template specialisation "
        "has mismatched face dimension and codimension.");

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static constexpr Perm<dim + 1> ordering(int face) {
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
                // Construct a permutation code from the individual images.

                using ImagePack = typename Perm<dim + 1>::ImagePack;
                ImagePack code = face; // 0 -> face

                int shift = Perm<dim + 1>::imageBits;
                for (int i = dim; i >= face + 1; --i) {
                    // dim - i + 1 -> i;
                    code |= (static_cast<ImagePack>(i) << shift);
                    shift += Perm<dim + 1>::imageBits;
                }
                for (int i = face - 1; i >= 0; --i) {
                    // dim - i -> i
                    code |= (static_cast<ImagePack>(i) << shift);
                    shift += Perm<dim + 1>::imageBits;
                }
                return Perm<dim + 1>::fromImagePack(code);
            }
        }

        static constexpr int faceNumber(Perm<dim + 1> vertices) {
            return vertices[0];
        }

        static constexpr bool containsVertex(int face, int vertex) {
            return (face == vertex);
        }
#endif // ! __DOXYGEN
};

template <int dim, int codim>
class FaceNumberingImpl<dim, 1, codim> : public FaceNumberingAPI<dim, 1> {
    static_assert(codim + 2 == dim,
        "The FaceNumberingImpl<dim, 1, codim> template specialisation "
        "has mismatched face dimension and codimension.");
    static_assert(! standardDim(dim),
        "The partial specialisation FaceNumberingImpl<dim, 1> "
        "should not be used for Regina's standard dimensions.");
    static_assert(FaceNumberingAPI<dim, 1>::lexNumbering);

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static constexpr Perm<dim + 1> ordering(int face) {
            std::array<int, dim + 1> image;

            // Find the lower and upper numbered vertices on this face.
            // See containsVertex() for the logic behind it.
            int threshold = binomSmall_[dim+1][2] - face;
            int i = dim;
            while (binomSmall_[i][2] >= threshold)
                --i;
            image[0] = dim - i;
            image[1] = image[0] + binomSmall_[i+1][2] - threshold + 1;

            // Following the generic implementation of ordering(), we now list
            // the remaining elements of the permutation in descending order.
            int pos = 2;
            int nextToAvoid = image[1];
            for (i = dim; i >= 0; --i) {
                if (i == nextToAvoid) {
                    // If we found the upper vertex, then now we need to avoid
                    // the lower vertex.
                    // If we found the lower vertex, this assignment is harmless
                    // since there are no other vertices to avoid.
                    nextToAvoid = image[0];
                } else
                    image[pos++] = i;
            }
            return Perm<dim + 1>(image);
        }

        static constexpr int faceNumber(int vertex0, int vertex1) {
            // Let (u, v) = (vertex0, vertex1), where u < v.
            // Then the edge number is:
            //     [dim + (dim-1) + ... + (dim-u+1)] + (v-u-1)
            //   = (dim+1 choose 2) - (dim-u+1 choose 2) + (v-u-1).

            if (vertex0 > vertex1)
                std::swap(vertex0, vertex1);
            return binomSmall_[dim + 1][2] - binomSmall_[dim - vertex0 + 1][2]
                + vertex1 - vertex0 - 1;
        }

        static constexpr int faceNumber(Perm<dim + 1> vertices) {
            return faceNumber(vertices[0], vertices[1]);
        }

        static constexpr bool containsVertex(int face, int vertex) {
            // Find the lower numbered vertex on this face.
            // This is the largest u for which
            //     face ≥ [dim + (dim-1) + ... + (dim-u+1)].
            // Equivalently:
            //     (dim-u+1 choose 2) ≥ (dim+1 choose 2) - face.

            int threshold = binomSmall_[dim+1][2] - face;
            int i = dim;
            while (binomSmall_[i][2] >= threshold)
                --i;

            // The lower vertex is now u = dim - i.
            int u = dim - i;
            if (vertex == u)
                return true;

            // The upper vertex is now u + binomSmall_[i+1][2] - threshold + 1.
            return (vertex == u + binomSmall_[i+1][2] - threshold + 1);
        }
#endif // ! __DOXYGEN
};

template <int dim, int subdim>
class FaceNumberingImpl<dim, subdim, 0> : public FaceNumberingAPI<dim, dim - 1> {
    static_assert(! standardDim(dim),
        "The partial specialisation FaceNumberingImpl<dim, dim-1> "
        "should not be used for Regina's standard dimensions.");
    static_assert(subdim + 1 == dim,
        "The FaceNumberingImpl<dim, subdim, 0> template specialisation "
        "has mismatched face dimension and codimension.");

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static constexpr Perm<dim + 1> ordering(int face) {
            // Construct a permutation code from the individual images.

            using ImagePack = typename Perm<dim + 1>::ImagePack;
            ImagePack code = 0;

            int shift = 0;
            for (int i = 0; i < face; ++i) {
                // i -> i
                code |= (static_cast<ImagePack>(i) << shift);
                shift += Perm<dim + 1>::imageBits;
            }
            for (int i = face + 1; i <= dim; ++i) {
                // i - 1 -> i
                code |= (static_cast<ImagePack>(i) << shift);
                shift += Perm<dim + 1>::imageBits;
            }
            // dim -> face
            code |= (static_cast<ImagePack>(face) << shift);

            return Perm<dim + 1>::fromImagePack(code);

        }

        static constexpr int faceNumber(Perm<dim + 1> vertices) {
            return vertices[dim];
        }

        static constexpr bool containsVertex(int face, int vertex) {
            return (face != vertex);
        }
#endif // ! __DOXYGEN
};

template <>
class FaceNumberingImpl<1, 0, 0> : public FaceNumberingAPI<1, 0> {
    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static constexpr Perm<2> ordering(int face) {
            return Perm<2>::rot(face);
        }

        static constexpr int faceNumber(Perm<2> vertices) {
            return vertices[0];
        }

        static constexpr bool containsVertex(int face, int vertex) {
            return (face == vertex);
        }
#endif // ! __DOXYGEN
};

template <>
class FaceNumberingImpl<2, 1, 0> : public FaceNumberingAPI<2, 1> {
    private:
        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<3>::Code ordering_[3] = { 2, 1, 0 };

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static constexpr Perm<3> ordering(int face) {
            return Perm<3>::fromPermCode(ordering_[face]);
        }

        static constexpr int faceNumber(Perm<3> vertices) {
            return vertices[2];
        }

        static constexpr int faceNumber(int u, int v) {
            return 3 - u - v;
        }

        static constexpr bool containsVertex(int face, int vertex) {
            return (face != vertex);
        }
#endif // ! __DOXYGEN
};

template <>
class FaceNumberingImpl<3, 1, 1> : public FaceNumberingAPI<3, 1> {
    public:
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

    private:
        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<4>::Code2 ordering_[6] = { 0, 2, 4, 8, 10, 16 };

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static constexpr Perm<4> ordering(int face) {
            return Perm<4>::fromPermCode2(ordering_[face]);
        }

        static constexpr int faceNumber(Perm<4> vertices) {
            return edgeNumber[vertices[0]][vertices[1]];
        }

        static constexpr int faceNumber(int u, int v) {
            return edgeNumber[u][v];
        }

        static constexpr bool containsVertex(int face, int vertex) {
            return (vertex == edgeVertex[face][0] ||
                    vertex == edgeVertex[face][1]);
        }
#endif // ! __DOXYGEN
};

template <>
class FaceNumberingImpl<3, 2, 0> : public FaceNumberingAPI<3, 2> {
    private:
        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<4>::Code2 ordering_[4] = { 9, 2, 1, 0 };

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static constexpr Perm<4> ordering(int face) {
            return Perm<4>::fromPermCode2(ordering_[face]);
        }

        static constexpr int faceNumber(Perm<4> vertices) {
            return vertices[3];
        }

        static constexpr bool containsVertex(int face, int vertex) {
            return (face != vertex);
        }
#endif // ! __DOXYGEN
};

template <>
class FaceNumberingImpl<4, 1, 2> : public FaceNumberingAPI<4, 1> {
    public:
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
        static constexpr Perm<5> ordering(int face) {
            return Perm<5>::fromPermCode2(ordering_[face]);
        }

        static constexpr int faceNumber(Perm<5> vertices) {
            return edgeNumber[vertices[0]][vertices[1]];
        }

        static constexpr int faceNumber(int u, int v) {
            return edgeNumber[u][v];
        }

        static constexpr bool containsVertex(int face, int vertex) {
            return (vertex == edgeVertex[face][0] ||
                    vertex == edgeVertex[face][1]);
        }
#endif // ! __DOXYGEN
};

template <>
class FaceNumberingImpl<4, 2, 1> : public FaceNumberingAPI<4, 2> {
    public:
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
        static constexpr Perm<5> ordering(int face) {
            return Perm<5>::fromPermCode2(ordering_[face]);
        }

        static constexpr int faceNumber(Perm<5> vertices) {
            return triangleNumber[vertices[0]][vertices[1]][vertices[2]];
        }

        static constexpr bool containsVertex(int face, int vertex) {
            return (vertex == triangleVertex[face][0] ||
                    vertex == triangleVertex[face][1] ||
                    vertex == triangleVertex[face][2]);
        }
#endif // ! __DOXYGEN
};

template <>
class FaceNumberingImpl<4, 3, 0> : public FaceNumberingAPI<4, 3> {
    private:
        /**
         * A hard-coded list of all return values for ordering(),
         * given by permutation code.
         */
        static constexpr Perm<5>::Code2 ordering_[5] = { 32, 9, 2, 1, 0 };

    public:
#ifndef __DOXYGEN
        // The following routines are documented in FaceNumberingAPI.
        static constexpr Perm<5> ordering(int face) {
            return Perm<5>::fromPermCode2(ordering_[face]);
        }

        static constexpr int faceNumber(Perm<5> vertices) {
            return vertices[4];
        }

        static constexpr bool containsVertex(int face, int vertex) {
            return (face != vertex);
        }
#endif // ! __DOXYGEN
};

} // namespace regina::detail

#endif

