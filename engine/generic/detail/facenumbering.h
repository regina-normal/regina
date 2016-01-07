
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

/*! \file detail/generic/facenumbering.h
 *  \brief Implementation details for describing how <i>subdim</i>-faces
 *  are numbered within a <i>dim</i>-dimensional simplex.
 */

#ifndef __FACENUMBERING_H_DETAIL
#ifndef __DOXYGEN
#define __FACENUMBERING_H_DETAIL
#endif

#include "regina-core.h"
#include "maths/nperm2.h"
#include "maths/nperm3.h"
#include "maths/nperm4.h"
#include "maths/nperm5.h"

namespace regina {
namespace detail {

/**
 * \weakgroup detail
 * @{
 */

/**
 * TODO: Document, including \a lex.
 */
template <int dim, int subdim, bool lex>
class FaceNumberingImpl {
    static_assert((dim + 1) >= 2 * (subdim + 1),
        "The generic FaceNumberingImpl<dim, subdim, lex> class "
        "should only be used for low-dimensional faces.");
    static_assert(subdim > 0,
        "The generic FaceNumberingImpl<dim, subdim, lex> class "
        "should not be used for vertices (i.e., subdim == 0).");
    static_assert(lex,
        "The generic FaceNumberingImpl<dim, subdim, lex> class "
        "should only be used for lexicographic numbering (i.e., lex == true).");

    public:
        /**
         * The total number of <i>subdim</i>-dimensional faces in each
         * <i>dim</i>-dimensional simplex.
         */
        static constexpr int nFaces =
            FaceNumberingImpl<dim - 1, subdim - 1, lex>::nFaces +
            FaceNumberingImpl<dim - 1, subdim,
                (dim >= 2 * (subdim + 1))>::nFaces;

        /**
         * Given a <i>subdim</i>-face number within a <i>dim</i>-dimensional
         * simplex, returns the corresponding canonical ordering of the
         * simplex vertices.
         *
         * If this canonical ordering is \a c, then \a c[0,...,\a subdim]
         * will be the vertices of the given face in increasing numerical
         * order.  That is, \a c[0] &lt; ... &lt; \a c[\a subdim].
         * For faces of dimension \a subdim &le; <i>dim</i>-2, the remaining
         * images \a c[(\a subdim + 1),...,\a dim] will be chosen to make
         * the permutation even.
         *
         * Note that this is \e not the same permutation as returned by
         * Simplex<dim>::faceMapping<subdim>():
         *
         * - ordering() is a static function, which returns the same permutation
         *   for the same face number, regardless of which <i>dim</i>-simplex
         *   we are looking at.  The images of 0,...,\a subdim will always
         *   appear in increasing order, and (for dimensions
         *   \a subdim &le; <i>dim</i>-2) the permutation will always be even.
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
        static NPerm<dim + 1> ordering(unsigned face);

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
        static unsigned faceNumber(NPerm<dim + 1> vertices);

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
};

#ifndef __DOXYGEN

// Specialisations of FaceNumbering

template <int dim, int subdim>
class FaceNumberingImpl<dim, subdim, false> {
    static_assert((dim + 1) < 2 * (subdim + 1),
        "The specialisation FaceNumberingImpl<dim, subdim, false> "
        "should only be used for high-dimensional faces.");

    public:
        static constexpr int nFaces =
            FaceNumberingImpl<dim, dim - subdim - 1, true>::nFaces;

        static NPerm<dim + 1> ordering(unsigned face) {
            return FaceNumberingImpl<dim, dim - subdim - 1, true>::
                ordering(face).reverse();
        }

        static unsigned faceNumber(NPerm<dim + 1> vertices) {
            return FaceNumberingImpl<dim, dim - subdim - 1, true>::
                faceNumber(vertices.reverse());
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return ! FaceNumberingImpl<dim, dim - subdim - 1, true>::
                containsVertex(face, vertex);
        }
};

template <int dim>
class FaceNumberingImpl<dim, 0, true> {
    public:
        static constexpr int nFaces = dim + 1;

        static NPerm<dim + 1> ordering(unsigned face) {
            // TODO: (dim, 0) implementation
            return NPerm<dim + 1>();
        }

        static unsigned faceNumber(NPerm<dim + 1> vertices) {
            return vertices[0];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face == vertex);
        }
};

template <>
class FaceNumberingImpl<1, 0, true> {
    public:
        static constexpr int nFaces = 2;

        static NPerm<2> ordering(unsigned face) {
            return NPerm2::fromPermCode(face);
        }

        static unsigned faceNumber(NPerm<2> vertices) {
            return vertices[0];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face == vertex);
        }
};

template <>
class FaceNumberingImpl<2, 0, true> {
    public:
        static constexpr int nFaces = 3;

        static NPerm<3> ordering(unsigned face) {
            // TODO: Unused vertices lexicographic?
            return NPerm<3>(face, (face + 1) % 3, (face + 2) % 3);
        }

        static unsigned faceNumber(NPerm<3> vertices) {
            return vertices[0];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face == vertex);
        }
};

template <>
class FaceNumberingImpl<2, 1, false> {
    private:
        static const NPerm<3> ordering_[3];

    public:
        static constexpr int nFaces = 3;

        static NPerm<3> ordering(unsigned face) {
            return ordering_[face];
        }

        static unsigned faceNumber(NPerm<3> vertices) {
            return vertices[2];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face != vertex);
        }
};

template <>
class FaceNumberingImpl<3, 0, true> {
    public:
        static constexpr int nFaces = 4;

        static NPerm<4> ordering(unsigned face) {
            // TODO: Unused vertices
            return (face % 2 == 0 ?
                NPerm4(face, (face + 1) % 4, (face + 2) % 4, (face + 3) % 4) :
                NPerm4(face, (face + 3) % 4, (face + 2) % 4, (face + 1) % 4));
        }

        static unsigned faceNumber(NPerm<4> vertices) {
            return vertices[0];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face == vertex);
        }
};

template <>
class FaceNumberingImpl<3, 1, true> {
    private:
        static const NPerm<4> ordering_[6];
        static const int faceNumber_[4][4];
        static const int vertex_[6][2];

    public:
        static constexpr int nFaces = 6;

        static NPerm<4> ordering(unsigned face) {
            return ordering_[face];
        }

        static unsigned faceNumber(NPerm<4> vertices) {
            return faceNumber_[vertices[0]][vertices[1]];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (vertex == vertex_[face][0] ||
                    vertex == vertex_[face][1]);
        }
};

template <>
class FaceNumberingImpl<3, 2, false> {
    private:
        static const NPerm<4> ordering_[4];

    public:
        static constexpr int nFaces = 4;

        static NPerm<4> ordering(unsigned face) {
            return ordering_[face];
        }

        static unsigned faceNumber(NPerm<4> vertices) {
            return vertices[3];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face != vertex);
        }
};

template <>
class FaceNumberingImpl<4, 0, true> {
    public:
        static constexpr int nFaces = 5;

        static NPerm<5> ordering(unsigned face) {
            return NPerm5(face, (face + 1) % 5, (face + 2) % 5,
                (face + 3) % 5, (face + 4) % 5);
        }

        static unsigned faceNumber(NPerm<5> vertices) {
            return vertices[0];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face == vertex);
        }
};

template <>
class FaceNumberingImpl<4, 1, true> {
    private:
        static const NPerm<5> ordering_[10];
        static const int faceNumber_[5][5];
        static const int vertex_[10][2];

    public:
        static constexpr int nFaces = 10;

        static NPerm<5> ordering(unsigned face) {
            return ordering_[face];
        }

        static unsigned faceNumber(NPerm<5> vertices) {
            return faceNumber_[vertices[0]][vertices[1]];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (vertex == vertex_[face][0] ||
                    vertex == vertex_[face][1]);
        }
};

template <>
class FaceNumberingImpl<4, 2, false> {
    private:
        static const NPerm<5> ordering_[10];
        static const int faceNumber_[5][5][5];
        static const int vertex_[10][3];

    public:
        static constexpr int nFaces = 10;

        static NPerm<5> ordering(unsigned face) {
            return ordering_[face];
        }

        static unsigned faceNumber(NPerm<5> vertices) {
            return faceNumber_[vertices[0]][vertices[1]][vertices[2]];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (vertex == vertex_[face][0] ||
                    vertex == vertex_[face][1] ||
                    vertex == vertex_[face][2]);
        }
};

template <>
class FaceNumberingImpl<4, 3, false> {
    private:
        static const NPerm<5> ordering_[5];

    public:
        static constexpr int nFaces = 5;

        static NPerm<5> ordering(unsigned face) {
            return ordering_[face];
        }

        static unsigned faceNumber(NPerm<5> vertices) {
            return vertices[4];
        }

        static bool containsVertex(unsigned face, unsigned vertex) {
            return (face != vertex);
        }
};

#endif // __DOXYGEN

// Inline functions for FaceNumbering

template <int dim, int subdim, bool lex>
inline NPerm<dim + 1> FaceNumberingImpl<dim, subdim, lex>::ordering(
        unsigned face) {
    static_assert(! standardDim(dim),
        "The generic implementation of FaceBaseImpl::ordering() "
        "should not be used for Regina's standard dimensions.");
    static_assert(lex,
        "The generic implementation of FaceBaseImpl::ordering() "
        "should only be used for low-dimensional faces.");

    // TODO: Implement ordering().
    return NPerm<dim + 1>();
}

template <int dim, int subdim, bool lex>
inline unsigned FaceNumberingImpl<dim, subdim, lex>::faceNumber(
        NPerm<dim + 1> vertices) {
    static_assert(! standardDim(dim),
        "The generic implementation of FaceBaseImpl::faceNumber() "
        "should not be used for Regina's standard dimensions.");
    static_assert(lex,
        "The generic implementation of FaceBaseImpl::faceNumber() "
        "should only be used for low-dimensional faces.");

    // TODO: Implement faceNumber().
    return 0;
}

template <int dim, int subdim, bool lex>
inline bool FaceNumberingImpl<dim, subdim, lex>::containsVertex(unsigned face,
        unsigned vertex) {
    static_assert(! standardDim(dim),
        "The generic implementation of FaceBaseImpl::containsVertex() "
        "should not be used for Regina's standard dimensions.");
    static_assert(lex,
        "The generic implementation of FaceBaseImpl::containsVertex() "
        "should only be used for low-dimensional faces.");

    // TODO: Implement containsVertex().
    return false;
}

} } // namespace regina::detail

#endif

