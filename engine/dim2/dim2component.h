
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

#ifndef __DIM2COMPONENT_H
#ifndef __DOXYGEN
#define __DIM2COMPONENT_H
#endif

/*! \file dim2/dim2component.h
 *  \brief Deals with connected components of a 2-manifold triangulation.
 */

#include "regina-core.h"
#include "generic/component.h"
#include "generic/alias/face.h"

namespace regina {

class Dim2BoundaryComponent;

template <int> class Simplex;
template <int> class Triangulation;
template <int, int> class Face;
typedef Simplex<2> Dim2Triangle;
typedef Triangulation<2> Dim2Triangulation;
typedef Face<2, 0> Dim2Vertex;
typedef Face<2, 1> Dim2Edge;

/**
 * \weakgroup dim2
 * @{
 */

namespace detail {

/**
 * Helper class that indicates what data type is used by a connected component
 * of a triangulation to store a list of <i>subdim</i>-faces.
 */
template <int subdim>
struct FaceListHolder<Component<2>, subdim> {
    /**
     * The data type used by Component<2> to store the list of all
     * <i>subdim</i>-faces of the connected component.
     *
     * The function Component<2>::faces<subdim>() returns a const
     * reference to this type.
     */
    typedef std::vector<Face<2, subdim>*> Holder;
};

} // namespace regina::detail

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
        std::vector<Dim2Edge*> edges_;
            /**< List of edges in the component. */
        std::vector<Dim2Vertex*> vertices_;
            /**< List of vertices in the component. */
        std::vector<Dim2BoundaryComponent*> boundaryComponents_;
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
         * Deprecated function that returns the number of boundary
         * components in this component.
         *
         * \deprecated Simply call countBoundaryComponents() instead.
         */
        REGINA_DEPRECATED size_t getNumberOfBoundaryComponents() const;

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
        Dim2BoundaryComponent* boundaryComponent(size_t index) const;
        /**
         * Deprecated routine that returns the requested boundary component
         * of this triangulation.
         *
         * \deprecated This routine has been renamed to boundaryComponent().
         * See the boundaryComponent() documentation for further details.
         */
        REGINA_DEPRECATED Dim2BoundaryComponent* getBoundaryComponent(
            size_t index) const;

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
 * A convenience typedef for Component<2>.
 */
typedef Component<2> Dim2Component;

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
inline const std::vector<Dim2Edge*>& Component<2>::faces<1>() const {
    return edges_;
}

template <>
inline const std::vector<Dim2Vertex*>& Component<2>::faces<0>() const {
    return vertices_;
}
#endif // ! __DOXYGEN

inline size_t Component<2>::countBoundaryComponents() const {
    return boundaryComponents_.size();
}

inline size_t Component<2>::getNumberOfBoundaryComponents() const {
    return boundaryComponents_.size();
}

// Hide specialisations from doxygen, since it cannot handle them.
#ifndef __DOXYGEN
template <>
inline Dim2Edge* Component<2>::face<1>(size_t index) const {
    return edges_[index];
}

template <>
inline Dim2Vertex* Component<2>::face<0>(size_t index) const {
    return vertices_[index];
}
#endif // ! __DOXYGEN

inline Dim2BoundaryComponent* Component<2>::boundaryComponent(size_t index)
        const {
    return boundaryComponents_[index];
}

inline Dim2BoundaryComponent* Component<2>::getBoundaryComponent(size_t index)
        const {
    return boundaryComponents_[index];
}

inline bool Component<2>::isClosed() const {
    return (boundaryComponents_.empty());
}

} // namespace regina

#endif

