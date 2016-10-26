
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

/*! \file triangulation/dim3/boundarycomponent3.h
 *  \brief Internal header for components of the boundary of a
 *  3-manifold triangulation.
 *
 *  This file is automatically included from triangulation/dim3.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation3.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation3.h.
#include "triangulation/dim3/triangulation3.h"

#ifndef __BOUNDARYCOMPONENT3_H
#ifndef __DOXYGEN
#define __BOUNDARYCOMPONENT3_H
#endif

namespace regina {

typedef Component<3> NComponent;
typedef Simplex<3> NTetrahedron;
typedef Face<3, 0> NVertex;
typedef Face<3, 1> NEdge;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Represents a component of the boundary of a triangulation.
 *
 * Note that an ideal vertex constitutes a boundary component of its
 * own - it consists of one vertex, no edges, and no triangles.
 *
 * We can run into some interesting cases with invalid triangulations.
 * Suppose some vertex link is a multiply punctured surface (which makes
 * the vertex and hence the entire triangulation invalid).  This means
 * that different parts of the 3-manifold boundary are effectively
 * "pinched" together.  If this happens, the different parts of the
 * boundary that are pinched might or might not be listed as part of the
 * same boundary component; if not then the offending vertex will be
 * included in all of these boundary components.  Nevertheless, only one
 * of these can be considered the "official" boundary component of the
 * vertex as returned by NVertex::boundaryComponent().  This is all a
 * bit of a mess, but then again the entire triangulation is invalid and
 * so you almost certainly have bigger problems to deal with.
 *
 * Boundary components are highly temporary; once a triangulation
 * changes, all its boundary component objects will be deleted and new
 * ones will be created.
 */
class REGINA_API NBoundaryComponent :
        public Output<NBoundaryComponent>,
        public alias::FaceOfTriangulation<NBoundaryComponent, 3>,
        public MarkedElement {
    private:
        std::vector<Triangle<3>*> triangles_;
            /**< List of triangles in the component. */
        std::vector<NEdge*> edges_;
            /**< List of edges in the component. */
        std::vector<NVertex*> vertices_;
            /**< List of vertices in the component. */

        bool orientable_;
            /**< Is this boundary component orientable? */

    public:
        /**
         * Returns the index of this boundary component in the underlying
         * triangulation.
         *
         * @return the index of this boundary component.
         */
        REGINA_INLINE_REQUIRED
        size_t index() const;

        /**
         * Returns the number of <i>subdim</i>-faces in this boundary component.
         *
         * \pre The template argument \a subdim is between 0 and 2 inclusive.
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * <tt>countFaces(subdim)</tt>; that is, the template parameter
         * \a subdim becomes the first argument of the function.
         *
         * @return the number of <i>subdim</i>-faces.
         */
        template <int subdim>
        size_t countFaces() const;

        /**
         * Returns the requested <i>subdim</i>-face in this boundary component.
         *
         * Note that the index of a face in the boundary component need
         * not be the index of the same face in the overall triangulation.
         *
         * \pre The template argument \a subdim is between 0 and 2 inclusive.
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * <tt>face(subdim, index)</tt>; that is, the template parameter
         * \a subdim becomes the first argument of the function.
         *
         * @param index the index of the desired face, ranging from 0 to
         * countFaces<subdim>()-1 inclusive.
         * @return the requested face.
         */
        template <int subdim>
        Face<3, subdim>* face(size_t index) const;

        /**
         * Returns the component of the triangulation to which this
         * boundary component belongs.
         *
         * @return the component containing this boundary component.
         */
        NComponent* component() const;

        /**
         * Returns the Euler characteristic of this boundary component.
         * If this boundary component is ideal, the Euler characteristic
         * of the link of the corresponding ideal vertex is returned.
         *
         * @return the Euler characteristic.
         */
        long eulerChar() const;

        /**
         * Determines if this boundary component is ideal.
         * This is the case if and only if it consists of a single
         * (ideal) vertex and no triangles.
         *
         * @return \c true if and only if this boundary component is
         * ideal.
         */
        REGINA_INLINE_REQUIRED
        bool isIdeal() const;

        /**
         * Determines if this boundary component is orientable.
         * If the boundary component is ideal, the orientability
         * of the link of the corresponding ideal vertex is returned.
         *
         * @return \c true if and only if this boundary component is
         * orientable.
         */
        bool isOrientable() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    private:
        /**
         * Default constructor.
         */
        NBoundaryComponent();

        /**
         * Creates a new boundary component consisting only of the given
         * ideal vertex.
         *
         * \pre The given vertex is ideal as returned by NVertex::isIdeal().
         *
         * @param idealVertex the vertex to place in the new boundary
         * component.
         */
        NBoundaryComponent(NVertex* idealVertex);

    friend class Triangulation<3>;
        /**< Allow access to private members. */
};

/*@}*/

// Inline functions for NBoundaryComponent

inline NBoundaryComponent::NBoundaryComponent() {
}

inline NBoundaryComponent::NBoundaryComponent(NVertex* idealVertex) {
    vertices_.push_back(idealVertex);
}

inline size_t NBoundaryComponent::index() const {
    return markedIndex();
}

// Hide specialisations from doxygen, since it cannot handle them.
#ifndef __DOXYGEN
template <>
inline size_t NBoundaryComponent::countFaces<2>() const {
    return triangles_.size();
}

template <>
inline size_t NBoundaryComponent::countFaces<1>() const {
    return edges_.size();
}

template <>
inline size_t NBoundaryComponent::countFaces<0>() const {
    return vertices_.size();
}

template <>
inline Triangle<3>* NBoundaryComponent::face<2>(size_t index) const {
    return triangles_[index];
}

template <>
inline NEdge* NBoundaryComponent::face<1>(size_t index) const {
    return edges_[index];
}

template <>
inline NVertex* NBoundaryComponent::face<0>(size_t index) const {
    return vertices_[index];
}
#endif // ! __DOXYGEN

inline NComponent* NBoundaryComponent::component() const {
    // There may be no triangles, but there is always a vertex.
    return vertices_.front()->component();
}

inline long NBoundaryComponent::eulerChar() const {
    return (isIdeal() ?
        vertices_.front()->linkEulerChar() :
        long(vertices_.size()) - long(edges_.size()) + long(triangles_.size()));
}

inline bool NBoundaryComponent::isIdeal() const {
    return triangles_.empty();
}

inline bool NBoundaryComponent::isOrientable() const {
    return orientable_;
}

inline void NBoundaryComponent::writeTextShort(std::ostream& out) const {
    out << (isIdeal() ? "Ideal " : "Finite ") << "boundary component";
}

} // namespace regina

#endif

