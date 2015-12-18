
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

#ifndef __NCOMPONENT_H
#ifndef __DOXYGEN
#define __NCOMPONENT_H
#endif

/*! \file triangulation/ncomponent.h
 *  \brief Deals with connected components of a 3-manifold triangulation.
 */

#include "regina-core.h"
#include "generic/component.h"
#include "generic/alias/face.h"

namespace regina {

class NBoundaryComponent;

template <int> class Simplex;
template <int> class Triangulation;
template <int, int> class Face;
typedef Simplex<3> NTetrahedron;
typedef Triangulation<3> NTriangulation;
typedef Face<3, 0> NVertex;
typedef Face<3, 1> NEdge;
typedef Face<3, 2> NTriangle;

/**
 * \weakgroup triangulation
 * @{
 */

/**
 * Represents a connected component of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Component class template; see
 * the Component documentation for an overview of how this class works.
 *
 * This 3-dimensional specialisation contains some extra functionality.
 * In particular, each 3-dimensional component also stores details on
 * lower-dimensional faces (i.e., vertices, edges and triangles), as well as
 * boundary components.
 */
template <>
class REGINA_API Component<3> : public detail::ComponentBase<3>,
        public alias::FaceOfTriangulation<Component<3>, 3> {
    private:
        std::vector<NTriangle*> triangles_;
            /**< List of triangles in the component. */
        std::vector<NEdge*> edges_;
            /**< List of edges in the component. */
        std::vector<NVertex*> vertices_;
            /**< List of vertices in the component. */
        std::vector<NBoundaryComponent*> boundaryComponents_;
            /**< List of boundary components in the component. */

        bool ideal_;
            /**< Is the component ideal? */

    public:

        /**
         * A dimension-specific alias for size().
         *
         * See size() for further information.
         */
        size_t getNumberOfTetrahedra() const;

        /**
         * Returns the number of <i>subdim</i>-faces in this component.
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
         * Returns the number of boundary components in this component.
         *
         * @return the number of boundary components.
         */
        size_t getNumberOfBoundaryComponents() const;

        /**
         * A dimension-specific alias for simplex().
         *
         * See simplex() for further information.
         */
        NTetrahedron* getTetrahedron(size_t index) const;

        /**
         * Returns the requested <i>subdim</i>-face in this component.
         *
         * Note that the index of a face in the component need
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
         * Returns the requested boundary component in this component.
         *
         * @param index the index of the requested boundary component in
         * this component.  This should be between 0 and
         * getNumberOfBoundaryComponents()-1 inclusive.
         * Note that the index of a boundary component in the component
         * need not be the index of the same boundary component in the
         * entire triangulation.
         * @return the requested boundary component.
         */
        NBoundaryComponent* getBoundaryComponent(size_t index) const;

        /**
         * Determines if this component is ideal.
         * This is the case if and only if it contains an ideal vertex
         * as described by NVertex::isIdeal().
         *
         * @return \c true if and only if this component is ideal.
         */
        bool isIdeal() const;

        /**
         * Determines if this component is closed.
         * This is the case if and only if it has no boundary.
         * Note that ideal components are not closed.
         *
         * @return \c true if and only if this component is closed.
         */
        bool isClosed() const;
        /**
         * A dimension-specific alias for getNumberOfBoundaryFacets().
         *
         * See getNumberOfBoundaryFacets() for further information.
         */
        size_t getNumberOfBoundaryTriangles() const;

    private:
        /**
         * Default constructor.
         *
         * Marks the component as orientable and non-ideal, with
         * no boundary facets.
         */
        Component();

    friend class Triangulation<3>;
    friend class detail::TriangulationBase<3>;
};

/**
 * A convenience typedef for Component<3>.
 */
typedef Component<3> NComponent;

/*@}*/

// Inline functions for Component<3>

inline Component<3>::Component() : detail::ComponentBase<3>(), ideal_(false) {
}

inline size_t Component<3>::getNumberOfTetrahedra() const {
    return size();
}

template <>
inline size_t Component<3>::countFaces<2>() const {
    return triangles_.size();
}

template <>
inline size_t Component<3>::countFaces<1>() const {
    return edges_.size();
}

template <>
inline size_t Component<3>::countFaces<0>() const {
    return vertices_.size();
}

inline size_t Component<3>::getNumberOfBoundaryComponents() const {
    return boundaryComponents_.size();
}

inline NTetrahedron* Component<3>::getTetrahedron(size_t index) const {
    return simplex(index);
}

template <>
inline NTriangle* Component<3>::face<2>(size_t index) const {
    return triangles_[index];
}

template <>
inline NEdge* Component<3>::face<1>(size_t index) const {
    return edges_[index];
}

template <>
inline NVertex* Component<3>::face<0>(size_t index) const {
    return vertices_[index];
}

inline NBoundaryComponent* Component<3>::getBoundaryComponent(size_t index)
        const {
    return boundaryComponents_[index];
}

inline bool Component<3>::isIdeal() const {
    return ideal_;
}

inline bool Component<3>::isClosed() const {
    return (boundaryComponents_.empty());
}

inline size_t Component<3>::getNumberOfBoundaryTriangles() const {
    return getNumberOfBoundaryFacets();
}

} // namespace regina

#endif

