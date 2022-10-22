
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#ifndef __REGINA_COMPONENT2_H
#ifndef __DOXYGEN
#define __REGINA_COMPONENT2_H
#endif

namespace regina {

/**
 * Represents a connected component of a 2-manifold triangulation.
 *
 * This is a specialisation of the generic Component class template; see
 * the Component documentation for an overview of how this class works.
 *
 * This 2-dimensional specialisation contains some extra functionality.
 * In particular, each 2-dimensional component also stores details on
 * lower-dimensional faces (i.e., vertices and edges).
 *
 * Components do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the Triangulation to which they belong.
 *
 * \headerfile triangulation/dim2.h
 *
 * \ingroup dim2
 */
template <>
class Component<2> : public detail::ComponentBase<2> {
    private:
        std::vector<Edge<2>*> edges_;
            /**< List of edges in the component. */
        std::vector<Vertex<2>*> vertices_;
            /**< List of vertices in the component. */

    public:
        /**
         * Returns the number of <i>subdim</i>-faces in this component.
         *
         * For convenience, this routine explicitly supports the case
         * \a subdim = 2.  This is _not_ the case for the routines
         * face() and faces(), which give access to individual faces
         * (the reason relates to the fact that triangles are built manually,
         * whereas lower-dimensional faces are deduced properties).
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * `countFaces(subdim)`; that is, the template parameter
         * \a subdim becomes the first argument of the function.
         *
         * \tparam subdim the face dimension; this must be between 0 and 2
         * inclusive.
         *
         * \return the number of <i>subdim</i>-faces.
         */
        template <int subdim>
        size_t countFaces() const;

        /**
         * Returns an object that allows iteration through and random access
         * to all <i>subdim</i>-faces in this component.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * range-based \c for loops.  Note that the elements of the list
         * will be pointers, so your code might look like:
         *
         * \code{.cpp}
         * for (Face<dim, subdim>* f : comp.faces<subdim>()) { ... }
         * \endcode
         *
         * The object that is returned will remain valid only for as
         * long as this component object exists.  In particular,
         * the object will become invalid any time that the triangulation
         * changes (since all component objects will be destroyed
         * and others rebuilt in their place).
         * Therefore it is best to treat this object as temporary only,
         * and to call faces() again each time you need it.
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * `faces(subdim)`.
         *
         * \tparam subdim the face dimension; this must be either 0 or 1.
         *
         * \return access to the list of all <i>subdim</i>-faces.
         */
        template <int subdim>
        auto faces() const;

        /**
         * Returns the requested <i>subdim</i>-face in this component.
         *
         * Note that the index of a face in the component need
         * not be the index of the same face in the overall triangulation.
         *
         * \ifacespython Python does not support templates.  Instead,
         * Python users should call this function in the form
         * `face(subdim, index)`; that is, the template parameter
         * \a subdim becomes the first argument of the function.
         *
         * \tparam subdim the face dimension; this must be either 0 or 1.
         *
         * \param index the index of the desired face, ranging from 0 to
         * countFaces<subdim>()-1 inclusive.
         * \return the requested face.
         */
        template <int subdim>
        Face<2, subdim>* face(size_t index) const;

        /**
         * A dimension-specific alias for hasBoundaryFacets().
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryEdges() const;

        /**
         * A dimension-specific alias for countBoundaryFacets().
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryEdges() const;

        /**
         * Determines if this component is closed.
         * This is the case if and only if it has no boundary.
         *
         * \return \c true if and only if this component is closed.
         */
        bool isClosed() const;

    private:
        /**
         * Default constructor.
         */
        Component() = default;

    friend class Triangulation<2>;
    friend class detail::TriangulationBase<2>;
};

// Inline functions for Component<2>

// Hide specialisations from doxygen, since it cannot handle them.
#ifndef __DOXYGEN
template <>
inline size_t Component<2>::countFaces<2>() const {
    return size();
}

template <>
inline size_t Component<2>::countFaces<1>() const {
    return edges_.size();
}

template <>
inline size_t Component<2>::countFaces<0>() const {
    return vertices_.size();
}

template <>
inline auto Component<2>::faces<1>() const {
    return ListView(edges_);
}

template <>
inline auto Component<2>::faces<0>() const {
    return ListView(vertices_);
}
#endif // ! __DOXYGEN

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

inline bool Component<2>::hasBoundaryEdges() const {
    return hasBoundaryFacets();
}

inline size_t Component<2>::countBoundaryEdges() const {
    return countBoundaryFacets();
}

inline bool Component<2>::isClosed() const {
    return (boundaryComponents().empty());
}

} // namespace regina

#endif

