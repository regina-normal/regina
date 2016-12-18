
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

/*! \file triangulation/dim4/component4.h
 *  \brief Internal header for connected components of a 4-manifold
 *  triangulation.
 *
 *  This file is automatically included from triangulation/dim4.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation4.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation4.h.
#include "triangulation/dim4/triangulation4.h"

#ifndef __COMPONENT4_H
#ifndef __DOXYGEN
#define __COMPONENT4_H
#endif

namespace regina {

/**
 * \weakgroup dim4
 * @{
 */

/**
 * Represents a connected component of a 4-manifold triangulation.
 *
 * This is a specialisation of the generic Component class template; see
 * the Component documentation for an overview of how this class works.
 *
 * This 4-dimensional specialisation contains some extra functionality.
 * In particular, each 4-dimensional component also stores details on
 * lower-dimensional faces (i.e., vertices, edges, triangles and tetrahedra).
 *
 * \headerfile triangulation/dim4.h
 */
template <>
class REGINA_API Component<4> : public detail::ComponentBase<4>,
        public alias::FaceOfTriangulation<Component<4>, 4>,
        public alias::FacesOfTriangulation<Component<4>, 4> {
    private:
        std::vector<Tetrahedron<4>*> tetrahedra_;
            /**< List of tetrahedra in the component. */
        std::vector<Triangle<4>*> triangles_;
            /**< List of triangles in the component. */
        std::vector<Edge<4>*> edges_;
            /**< List of edges in the component. */
        std::vector<Vertex<4>*> vertices_;
            /**< List of vertices in the component. */

        bool ideal_;
            /**< Is the component ideal? */

    public:
        /**
         * Returns the number of <i>subdim</i>-faces in this component.
         *
         * \pre The template argument \a subdim is between 0 and 3 inclusive.
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
         * Returns a reference to the list of all <i>subdim</i>-faces in
         * this component.
         *
         * \pre The template argument \a subdim is between 0 and 3 inclusive.
         *
         * \ifacespython Python users should call this function in the
         * form <tt>faces(subdim)</tt>.  It will then return a Python list
         * containing all the <i>subdim</i>-faces of the triangulation.
         *
         * @return the list of all <i>subdim</i>-faces.
         */
        template <int subdim>
        const std::vector<Face<4, subdim>*>& faces() const;

        /**
         * Returns the requested <i>subdim</i>-face in this component.
         *
         * Note that the index of a face in the component need
         * not be the index of the same face in the overall triangulation.
         *
         * \pre The template argument \a subdim is between 0 and 3 inclusive.
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
        Face<4, subdim>* face(size_t index) const;

        /**
         * Determines if this component is ideal.
         * This is the case if and only if it contains an ideal vertex
         * as described by Vertex<4>::isIdeal().
         *
         * @return \c true if and only if this component is ideal.
         */
        bool isIdeal() const;

        /**
         * Determines if this component is closed.
         * This is the case if and only if it has no boundary.
         *
         * Note that ideal components are not closed.  Likewise,
         * components with invalid vertices are not closed.
         * See Vertex<4>::isBoundary() for details.
         *
         * @return \c true if and only if this component is closed.
         */
        bool isClosed() const;

    private:
        /**
         * Default constructor.
         *
         * Marks the component as non-ideal.
         */
        Component();

    friend class Triangulation<4>;
    friend class detail::TriangulationBase<4>;
        /**< Allow access to private members. */
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim4Component, you should use
 * the real class name Component<4>.
 */
REGINA_DEPRECATED typedef Component<4> Dim4Component;

/*@}*/

// Inline functions for Component<4>

inline Component<4>::Component() : detail::ComponentBase<4>(), ideal_(false) {
}

#ifndef __DOXYGEN // Doxygen gets confused by the specialisations.

template <>
inline size_t Component<4>::countFaces<3>() const {
    return tetrahedra_.size();
}

template <>
inline size_t Component<4>::countFaces<2>() const {
    return triangles_.size();
}

template <>
inline size_t Component<4>::countFaces<1>() const {
    return edges_.size();
}

template <>
inline size_t Component<4>::countFaces<0>() const {
    return vertices_.size();
}

template <>
inline const std::vector<Tetrahedron<4>*>& Component<4>::faces<3>() const {
    return tetrahedra_;
}

template <>
inline const std::vector<Triangle<4>*>& Component<4>::faces<2>() const {
    return triangles_;
}

template <>
inline const std::vector<Edge<4>*>& Component<4>::faces<1>() const {
    return edges_;
}

template <>
inline const std::vector<Vertex<4>*>& Component<4>::faces<0>() const {
    return vertices_;
}

template <>
inline Tetrahedron<4>* Component<4>::face<3>(size_t index) const {
    return tetrahedra_[index];
}

template <>
inline Triangle<4>* Component<4>::face<2>(size_t index) const {
    return triangles_[index];
}

template <>
inline Edge<4>* Component<4>::face<1>(size_t index) const {
    return edges_[index];
}

template <>
inline Vertex<4>* Component<4>::face<0>(size_t index) const {
    return vertices_[index];
}

#endif // __DOXYGEN

inline bool Component<4>::isIdeal() const {
    return ideal_;
}

inline bool Component<4>::isClosed() const {
    return (boundaryComponents().empty());
}

} // namespace regina

#endif

