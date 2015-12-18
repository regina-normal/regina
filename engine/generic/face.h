
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

/*! \file generic/face.h
 *  \brief Deals with lower-dimensional faces of triangulations.
 */

#ifndef __FACE_H
#ifndef __DOXYGEN
#define __FACE_H
#endif

#include "generic/detail/face.h"

namespace regina {

/**
 * \weakgroup generic
 * @{
 */

/**
 * Details how a <i>subdim</i>-face of a <i>dim</i>-dimensional triangulation
 * appears within each top-dimensional simplex.
 *
 * For a <i>dim</i>-dimensional triangulation \a T, each <i>subdim</i>-face
 * \a F typically belongs to many top-dimensional simplices of \a T,
 * and therefore has many associated FaceEmbedding objects.  These individual
 * FaceEmbedding objects correspond to the top-dimensional simplices of the
 * link of \a F (which is a (\a dim - \a subdim - 1)-dimensional triangulation).
 *
 * If \a dim is one of Regina's \ref stddim "standard dimensions", then
 * this template is specialised to offer additional dimension-specific
 * aliases.  In order to use these specialised classes, you will need to
 * include the corresponding headers (e.g., dim2/dim2edge.h for
 * (\a dim, \a subdim) = (2, 1), or triangulation/nvertex.h for
 * (\a dim, \a subdim) = (3, 0)).  For convenience, there are typedefs
 * for these specialised classes (such as Dim2EdgeEmbedding and
 * NVertexEmbedding respectively).
 *
 * \ifacespython Python does not support templates.  For standard dimensions
 * this class can be used by appending dimensions \a dim and \a subdim as
 * suffices (e.g., FaceEmbedding2_1 and FaceEmbedding3_0 for the two examples
 * above).  The typedefs mentioned above (e.g., Dim2EdgeEmbedding and
 * NVertexEmbedding) are also available.  Higher-dimensional classes are
 * not available in Python for the time being.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 * \tparam subdim the dimension of the faces of the underlying triangulation.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class FaceEmbedding : public detail::FaceEmbeddingBase<dim, subdim> {
    public:
        /**
         * Default constructor.  This object is unusable until it has
         * some data assigned to it using <tt>operator =</tt>.
         *
         * \ifacespython Not present.
         */
        FaceEmbedding();

        /**
         * Creates a new object containing the given data.
         *
         * @param simplex the top-dimensional simplex in which the
         * underlying <i>subdim</i>-face of the triangulation is contained.
         * @param face the corresponding face number of \a simplex.
         * This must be between 0 and (<i>dim</i>+1 choose <i>subdim</i>+1)-1
         * inclusive.
         */
        FaceEmbedding(Simplex<dim>* simplex, int face);

        /**
         * Creates a new copy of the given object.
         *
         * @param cloneMe the object to copy.
         */
        FaceEmbedding(const FaceEmbedding& cloneMe);
};

/**
 * Represents a <i>subdim</i>-face in the skeleton of a <i>dim</i>-dimensional
 * triangulation.
 *
 * A given <i>subdim</i>-face \a F of the triangulation may appear many
 * times within the various top-dimensional simplices of the underlying
 * triangulation.  As an extreme example, in a 1-vertex triangulation of
 * a 3-manifold, the single vertex makes 4<i>n</i> such appearances,
 * where \a n is the total number of tetrahedra.
 *
 * Each such appearance is described by a single FaceEmbedding object.  You can
 * iterate through these appearances using begin() and end(), or using a
 * C++11 range-based \c for loop: <tt>for (auto& emb : F) { ... }</tt>.
 * You can count these appearances by calling degree(), and you can also
 * examine them using routines such as front(), back() and embedding().
 *
 * \warning Face objects are highly temporary: whenever a triangulation
 * changes, all its face objects will be deleted and new ones will be
 * created in their place.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 * \tparam subdim the dimension of the faces that this class represents.
 * This must be between 0 and <i>dim</i>-1 inclusive.
 */
template <int dim, int subdim>
class Face : public detail::FaceBase<dim, subdim>, Output<Face<dim, subdim>> {
    static_assert(! standardDim(dim),
        "The generic implementation of Face<dim, subdim> "
        "should not be used for Regina's standard dimensions.");

    public:
        /**
         * Writes a short text representation of this face to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this face to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    protected:
        /**
         * Creates a new face.
         *
         * @param component the component of the underlying triangulation
         * to which the new face belongs.
         */
        Face(Component<dim>* component);

    friend class Triangulation<dim>;
    friend class detail::TriangulationBase<dim>;
};

// Note that some of our face-related classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class Face<2, 1>;
template <> class Face<2, 0>;
template <> class Face<3, 2>;
template <> class Face<3, 1>;
template <> class Face<3, 0>;

/*@}*/

// Inline functions for FaceEmbedding

template <int dim, int subdim>
inline FaceEmbedding<dim, subdim>::FaceEmbedding() {
}

template <int dim, int subdim>
inline FaceEmbedding<dim, subdim>::FaceEmbedding(
        Simplex<dim>* simplex, int face) :
        detail::FaceEmbeddingBase<dim, subdim>(simplex, face) {
}

template <int dim, int subdim>
inline FaceEmbedding<dim, subdim>::FaceEmbedding(
        const FaceEmbedding& cloneMe) :
        detail::FaceEmbeddingBase<dim, subdim>(cloneMe) {
}

// Inline functions for Face

template <int dim, int subdim>
inline Face<dim, subdim>::Face(Component<dim>* component) :
        detail::FaceBase<dim, subdim>(component) {
}

template <int dim, int subdim>
inline void Face<dim, subdim>::writeTextShort(std::ostream& out) const {
    // We can't do a partial specialisation on subdim.
    // Instead take cases, which we hope the compiler can optimise given
    // that subdim is a compile-time constant.
    switch (subdim) {
        case 0:
            out << "Vertex"; break;
        case 1:
            out << "Edge"; break;
        case 2:
            out << "Triangle"; break;
        case 3:
            out << "Tetrahedron"; break;
        case 4:
            out << "Pentachoron"; break;
        default:
            out << subdim << "-face"; break;
    }
    out << " of degree " << detail::FaceStorage<dim, dim - subdim>::degree();
}

template <int dim, int subdim>
inline void Face<dim, subdim>::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;

    out << "Appears as:" << std::endl;
    for (auto& emb : *this)
        out << "  " << emb << std::endl;
}

} // namespace regina

#endif

