
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

/*! \file triangulation/dim2/component2.h
 *  \brief Internal header for connected components of a 2-manifold
 *  triangulation.
 *
 *  This file is automatically included from triangulation/dim2.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation2.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation2.h.
#include "triangulation/dim2/triangulation2.h"

#ifndef __COMPONENT2_H
#ifndef __DOXYGEN
#define __COMPONENT2_H
#endif

namespace regina {

/**
 * \weakgroup dim2
 * @{
 */

/**
 * Represents a connected component of a 2-manifold triangulation.
 *
 * This is a specialisation of the generic Component class template; see
 * the Component documentation for an overview of how this class works.
 *
 * This 2-dimensional specialisation contains some extra functionality.
 * In particular, each 2-dimensional component also stores details on
 * lower-dimensional faces (i.e., vertices and edges) and boundary components.
 */
template <>
class REGINA_API Component<2> : public detail::ComponentBase<2>,
        public alias::FaceOfTriangulation<Component<2>, 2>,
        public alias::FacesOfTriangulation<Component<2>, 2> {
    private:
        std::vector<Edge<2>*> edges_;
            /**< List of edges in the component. */
        std::vector<Vertex<2>*> vertices_;
            /**< List of vertices in the component. */
        std::vector<BoundaryComponent<2>*> boundaryComponents_;
            /**< List of boundary components in the component. */

    public:
        /**
         * Returns the number of <i>subdim</i>-faces in this component.
         *
         * \pre The template argument \a subdim is either 0 or 1.
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
         * Returns the number of boundary components in this component.
         *
         * @return the number of boundary components.
         */
        size_t countBoundaryComponents() const;

        /**
         * Returns a reference to the list of all <i>subdim</i>-faces in
         * this component.
         *
         * \pre The template argument \a subdim is either 0 or 1.
         *
         * \ifacespython Python users should call this function in the
         * form <tt>faces(subdim)</tt>.  It will then return a Python list
         * containing all the <i>subdim</i>-faces of the triangulation.
         *
         * @return the list of all <i>subdim</i>-faces.
         */
        template <int subdim>
        const std::vector<Face<2, subdim>*>& faces() const;

        /**
         * Returns the requested <i>subdim</i>-face in this component.
         *
         * Note that the index of a face in the component need
         * not be the index of the same face in the overall triangulation.
         *
         * \pre The template argument \a subdim is either 0 or 1.
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
        Face<2, subdim>* face(size_t index) const;

        /**
         * Returns the requested boundary component in this component.
         *
         * @param index the index of the requested boundary component in
         * this component.  This should be between 0 and
         * countBoundaryComponents()-1 inclusive.
         * Note that the index of a boundary component in the component
         * need not be the index of the same boundary component in the
         * entire triangulation.
         * @return the requested boundary component.
         */
        BoundaryComponent<2>* boundaryComponent(size_t index) const;

        /**
         * Determines if this component is closed.
         * This is the case if and only if it has no boundary.
         *
         * @return \c true if and only if this component is closed.
         */
        bool isClosed() const;

    private:
        /**
         * Default constructor.
         *
         * Marks the component as orientable, with no boundary facets.
         */
        Component();

    friend class Triangulation<2>;
    friend class detail::TriangulationBase<2>;
};

/**
 * Deprecated typedef for backward compatibility.  This typedef will
 * be removed in a future release of Regina.
 *
 * \deprecated Instead of the old typedef Dim2Component, you should use
 * the real class name Component<2>.
 */
REGINA_DEPRECATED typedef Component<2> Dim2Component;

/*@}*/

// Inline functions for Component<2>

inline Component<2>::Component() : detail::ComponentBase<2>() {
}

// Hide specialisations from doxygen, since it cannot handle them.
#ifndef __DOXYGEN
template <>
inline size_t Component<2>::countFaces<1>() const {
    return edges_.size();
}

template <>
inline size_t Component<2>::countFaces<0>() const {
    return vertices_.size();
}

template <>
inline const std::vector<Edge<2>*>& Component<2>::faces<1>() const {
    return edges_;
}

template <>
inline const std::vector<Vertex<2>*>& Component<2>::faces<0>() const {
    return vertices_;
}
#endif // ! __DOXYGEN

inline size_t Component<2>::countBoundaryComponents() const {
    return boundaryComponents_.size();
}

// Hide specialisations from doxygen, since it cannot handle them.
#ifndef __DOXYGEN
template <>
inline Edge<2>* Component<2>::face<1>(size_t index) const {
    return edges_[index];
}

template <>
inline Vertex<2>* Component<2>::face<0>(size_t index) const {
    return vertices_[index];
}
#endif // ! __DOXYGEN

inline BoundaryComponent<2>* Component<2>::boundaryComponent(size_t index)
        const {
    return boundaryComponents_[index];
}

inline bool Component<2>::isClosed() const {
    return (boundaryComponents_.empty());
}

} // namespace regina

#endif

