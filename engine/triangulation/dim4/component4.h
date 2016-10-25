
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

class Dim4BoundaryComponent;

template <int> class Simplex;
template <int> class Triangulation;
template <int, int> class Face;
typedef Simplex<4> Dim4Pentachoron;
typedef Triangulation<4> Dim4Triangulation;
typedef Face<4, 3> Dim4Tetrahedron;
typedef Face<4, 2> Dim4Triangle;
typedef Face<4, 1> Dim4Edge;
typedef Face<4, 0> Dim4Vertex;

/**
 * \weakgroup dim4
 * @{
 */

namespace detail {

/**
 * Helper class that indicates what data type is used by a connected component
 * of a triangulation to store a list of <i>subdim</i>-faces.
 */
template <int subdim>
struct FaceListHolder<Component<4>, subdim> {
    /**
     * The data type used by Component<4> to store the list of all
     * <i>subdim</i>-faces of the connected component.
     *
     * The function Component<4>::faces<subdim>() returns a const
     * reference to this type.
     */
    typedef std::vector<Face<4, subdim>*> Holder;
};

} // namespace regina::detail

/**
 * Represents a connected component of a 4-manifold triangulation.
 *
 * This is a specialisation of the generic Component class template; see
 * the Component documentation for an overview of how this class works.
 *
 * This 4-dimensional specialisation contains some extra functionality.
 * In particular, each 4-dimensional component also stores details on
 * lower-dimensional faces (i.e., vertices, edges, triangles and tetrahedra),
 * as well as boundary components.
 */
template <>
class REGINA_API Component<4> : public detail::ComponentBase<4>,
        public alias::FaceOfTriangulation<Component<4>, 4>,
        public alias::FacesOfTriangulation<Component<4>, 4> {
    private:
        std::vector<Dim4Tetrahedron*> tetrahedra_;
            /**< List of tetrahedra in the component. */
        std::vector<Dim4Triangle*> triangles_;
            /**< List of triangles in the component. */
        std::vector<Dim4Edge*> edges_;
            /**< List of edges in the component. */
        std::vector<Dim4Vertex*> vertices_;
            /**< List of vertices in the component. */
        std::vector<Dim4BoundaryComponent*> boundaryComponents_;
            /**< List of boundary components in the component. */

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
         * Returns the number of boundary components in this component.
         *
         * @return the number of boundary components.
         */
        size_t countBoundaryComponents() const;

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
        Dim4BoundaryComponent* boundaryComponent(size_t index) const;

        /**
         * Determines if this component is ideal.
         * This is the case if and only if it contains an ideal vertex
         * as described by Dim4Vertex::isIdeal().
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
         * See Dim4Vertex::isBoundary() for details.
         *
         * @return \c true if and only if this component is closed.
         */
        bool isClosed() const;

    private:
        /**
         * Default constructor.
         *
         * Marks the component as orientable and not ideal, with no
         * boundary facets.
         */
        Component();

    friend class Triangulation<4>;
    friend class detail::TriangulationBase<4>;
        /**< Allow access to private members. */
};

/**
 * A convenience typedef for Component<4>.
 */
typedef Component<4> Dim4Component;

/*@}*/

// Inline functions for Component<4>

inline Dim4Component::Component() : detail::ComponentBase<4>(), ideal_(false) {
}

template <>
inline size_t Dim4Component::countFaces<3>() const {
    return tetrahedra_.size();
}

template <>
inline size_t Dim4Component::countFaces<2>() const {
    return triangles_.size();
}

template <>
inline size_t Dim4Component::countFaces<1>() const {
    return edges_.size();
}

template <>
inline size_t Dim4Component::countFaces<0>() const {
    return vertices_.size();
}

inline size_t Dim4Component::countBoundaryComponents() const {
    return boundaryComponents_.size();
}

#ifndef __DOXYGEN // Doxygen gets confused by the specialisations.

template <>
inline const std::vector<Dim4Tetrahedron*>& Component<4>::faces<3>() const {
    return tetrahedra_;
}

template <>
inline const std::vector<Dim4Triangle*>& Component<4>::faces<2>() const {
    return triangles_;
}

template <>
inline const std::vector<Dim4Edge*>& Component<4>::faces<1>() const {
    return edges_;
}

template <>
inline const std::vector<Dim4Vertex*>& Component<4>::faces<0>() const {
    return vertices_;
}

template <>
inline Dim4Tetrahedron* Dim4Component::face<3>(size_t index) const {
    return tetrahedra_[index];
}

template <>
inline Dim4Triangle* Dim4Component::face<2>(size_t index) const {
    return triangles_[index];
}

template <>
inline Dim4Edge* Dim4Component::face<1>(size_t index) const {
    return edges_[index];
}

template <>
inline Dim4Vertex* Dim4Component::face<0>(size_t index) const {
    return vertices_[index];
}

#endif // __DOXYGEN

inline Dim4BoundaryComponent* Dim4Component::boundaryComponent(
        size_t index) const {
    return boundaryComponents_[index];
}

inline bool Dim4Component::isIdeal() const {
    return ideal_;
}

inline bool Dim4Component::isClosed() const {
    return (boundaryComponents_.empty());
}

} // namespace regina

#endif

