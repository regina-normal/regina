
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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

/*! \file triangulation/dim3/component3.h
 *  \brief Internal header for connected components of a 3-manifold
 *  triangulation.
 *
 *  This file is automatically included from triangulation/dim3.h; there is
 *  no need for end users to include this header explicitly.
 */

// We include triangulation3.h before the header guard, to ensure that the
// various face/component headers are processed in exactly the right order.
// This header will be re-included from within triangulation3.h.
#include "triangulation/dim3/triangulation3.h"

#ifndef __REGINA_COMPONENT3_H
#ifndef __DOXYGEN
#define __REGINA_COMPONENT3_H
#endif

#include <variant>

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * Represents a connected component of a 3-manifold triangulation.
 *
 * This is a specialisation of the generic Component class template; see the
 * generic Component documentation for an overview of how the component
 * classes work.  In Python, you can read this generic documentation by
 * looking at a higher dimension: try `help(Component5)`.
 *
 * This 3-dimensional specialisation contains some extra functionality.
 * In particular, each 3-dimensional component also stores details on
 * lower-dimensional faces (i.e., vertices, edges and triangles).
 *
 * Components do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the Triangulation to which they belong.
 *
 * \headerfile triangulation/dim3.h
 *
 * \ingroup dim3
 */
template <>
class Component<3> : public detail::ComponentBase<3> {
    private:
        std::vector<Triangle<3>*> triangles_;
            /**< List of triangles in the component. */
        std::vector<Edge<3>*> edges_;
            /**< List of edges in the component. */
        std::vector<Vertex<3>*> vertices_;
            /**< List of vertices in the component. */

        bool ideal_;
            /**< Is the component ideal? */

    public:
        /**
         * Returns the number of <i>subdim</i>-faces in this component.
         *
         * This is the fastest way to count faces if you know \a subdim
         * at compile time.
         *
         * For convenience, this routine explicitly supports the case
         * \a subdim = 3.  This is _not_ the case for the routines
         * face() and faces(), which give access to individual faces
         * (the reason relates to the fact that tetrahedra are built manually,
         * whereas lower-dimensional faces are deduced properties).
         *
         * \nopython Instead use the variant `countFaces(subdim)`.
         *
         * \tparam subdim the face dimension.
         *
         * \return the number of <i>subdim</i>-faces.
         */
        template <int subdim> requires (subdim >= 0 && subdim <= 3)
        size_t countFaces() const;

        /**
         * Returns the number of <i>subdim</i>-faces in this component,
         * where the face dimension does not need to be known until runtime.
         *
         * For C++ programmers who know \a subdim at compile time, you are
         * better off using the template function `countFaces<subdim>()`
         * instead, which is (slightly) faster.
         *
         * For convenience, this routine explicitly supports the case
         * \a subdim = 3.  This is _not_ the case for the routines
         * face() and faces(), which give access to individual faces
         * (the reason relates to the fact that tetrahedra are built manually,
         * whereas lower-dimensional faces are deduced properties).
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., negative or greater than 3).
         *
         * \param subdim the face dimension; this must be between 0 and 3
         * inclusive.
         * \return the number of <i>subdim</i>-faces.
         */
        size_t countFaces(int subdim) const;

        /**
         * Returns an object that allows iteration through and random access
         * to all <i>subdim</i>-faces in this component, in a way that is
         * optimised for C++ programmers.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use `auto` (just like this declaration does).
         *
         * The returned object is guaranteed to be a lightweight view type
         * from the `std::ranges` library, which means it supports range-based
         * `for` loops.  Note that the elements of the view will be pointers,
         * so your code might look like:
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
         * \nopython Instead use the variant `faces(subdim)`.
         *
         * \tparam subdim the face dimension.
         *
         * \return access to the list of all <i>subdim</i>-faces.
         */
        template <int subdim> requires (subdim >= 0 && subdim < 3)
        auto faces() const;

        /**
         * Returns an object that allows iteration through and random access
         * to all <i>subdim</i>-faces in this component, in a way that is
         * optimised for Python programmers.
         *
         * C++ users should not use this routine.  The return type must be fixed
         * at compile time, and so it is typically a `std::variant` that can
         * hold any of the lightweight rview types returned from the templated
         * `faces<subdim>()` function.  This means that the return value will
         * still need compile-time knowledge of \a subdim to extract and
         * use the appropriate face objects.  However, once you know \a subdim
         * at compile time, you are much better off using the (simpler and
         * faster) routine `faces<subdim>()` instead.
         *
         * For Python users, this routine is much more useful: the return type
         * can be chosen at runtime, and so this routine returns a single
         * lightweight view granting access to all of the <i>subdim</i>-faces
         * of the component, which you can use immediately.
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., negative, or greater than 2).
         *
         * \param subdim the face dimension; this must be between 0 and 2
         * inclusive.
         * \return access to the list of all <i>subdim</i>-faces.
         */
        auto faces(int subdim) const;

        /**
         * Returns the requested <i>subdim</i>-face in this component, in a
         * way that is optimised for C++ programmers.
         *
         * Note that the index of a face in the component need
         * not be the index of the same face in the overall triangulation.
         *
         * \nopython Instead use the variant `face(subdim, index)`.
         *
         * \tparam subdim the face dimension.
         *
         * \param index the index of the desired face, ranging from 0 to
         * countFaces<subdim>()-1 inclusive.
         * \return the requested face.
         */
        template <int subdim> requires (subdim >= 0 && subdim < 3)
        Face<3, subdim>* face(size_t index) const;

        /**
         * Returns the requested <i>subdim</i>-face in this component, in a
         * way that is optimised for Python programmers.
         *
         * C++ users should not use this routine.  The return type must be
         * fixed at compile time, and so it is typically a `std::variant` that
         * could store a pointer to any class `Face<3, ...>`.  This means you
         * cannot access the face directly: you will still need some kind of
         * compile-time knowledge of \a subdim before you can extract and use
         * an appropriate `Face<3, subdim>` object from \a v.  However, once
         * you know \a subdim at compile time, you are better off using the
         * (simpler and faster) routine `face<subdim>()` instead.
         *
         * For Python users, this routine is much more useful: the return type
         * can be chosen at runtime, and so this routine simply returns a
         * `Face<3, subdim>` object of the appropriate face dimension that
         * you can use immediately.
         *
         * The specific return type for C++ programmers will be
         * `std::variant<Face<3, 0>*, Face<3, 1>*, Face<3, 2>*>`.
         *
         * Note that the index of a face in the component need
         * not be the index of the same face in the overall triangulation.
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., negative, or greater than 2).
         *
         * \param subdim the face dimension; this must be between 0 and 2
         * inclusive.
         * \param index the index of the desired face, ranging from 0 to
         * countFaces<subdim>()-1 inclusive.
         * \return the requested face.
         */
        auto face(int subdim, size_t index) const;

        /**
         * A dimension-specific alias for hasBoundaryFacets().
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryTriangles() const;

        /**
         * A dimension-specific alias for countBoundaryFacets().
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryTriangles() const;

        /**
         * Determines if this component is ideal.
         * This is the case if and only if it contains an ideal vertex
         * as described by Vertex<3>::isIdeal().
         *
         * \return \c true if and only if this component is ideal.
         */
        bool isIdeal() const;

        /**
         * Determines if this component is closed.
         * This is the case if and only if it has no boundary.
         * Note that ideal components are not closed.
         *
         * \return \c true if and only if this component is closed.
         */
        bool isClosed() const;

    private:
        /**
         * Default constructor.
         *
         * Marks the component as non-ideal.
         */
        Component();

    friend class Triangulation<3>;
    friend class detail::TriangulationBase<3>;
};

// Inline functions for Component<3>

inline Component<3>::Component() : detail::ComponentBase<3>(), ideal_(false) {
}

inline size_t Component<3>::countFaces(int subdim) const {
    switch (subdim) {
        case 3: return size();
        case 2: return triangles_.size();
        case 1: return edges_.size();
        case 0: return vertices_.size();
        default:
            throw InvalidArgument("countFaces(): unsupported face dimension");
    }
}

// Hide specialisations from doxygen, since it cannot handle them.
#ifndef __DOXYGEN
template <>
inline size_t Component<3>::countFaces<3>() const {
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
#endif // ! __DOXYGEN

inline auto Component<3>::faces(int subdim) const {
    using Return = std::variant<
        decltype(std::views::all(vertices_)),
        decltype(std::views::all(edges_)),
        decltype(std::views::all(triangles_))>;
    switch (subdim) {
        case 0: return Return(std::views::all(vertices_));
        case 1: return Return(std::views::all(edges_));
        case 2: return Return(std::views::all(triangles_));
        default: throw InvalidArgument("faces(): unsupported face dimension");
    }
}

// Hide specialisations from doxygen, since it cannot handle them.
#ifndef __DOXYGEN
template <>
inline auto Component<3>::faces<2>() const {
    return std::views::all(triangles_);
}

template <>
inline auto Component<3>::faces<1>() const {
    return std::views::all(edges_);
}

template <>
inline auto Component<3>::faces<0>() const {
    return std::views::all(vertices_);
}
#endif // ! __DOXYGEN

inline auto Component<3>::face(int subdim, size_t index) const {
    using Return = std::variant<Face<3, 0>*, Face<3, 1>*, Face<3, 2>*>;
    switch (subdim) {
        case 0: return Return(vertices_[index]);
        case 1: return Return(edges_[index]);
        case 2: return Return(triangles_[index]);
        default: throw InvalidArgument("face(): unsupported face dimension");
    }
}

// Hide specialisations from doxygen, since it cannot handle them.
#ifndef __DOXYGEN
template <>
inline Triangle<3>* Component<3>::face<2>(size_t index) const {
    return triangles_[index];
}

template <>
inline Edge<3>* Component<3>::face<1>(size_t index) const {
    return edges_[index];
}

template <>
inline Vertex<3>* Component<3>::face<0>(size_t index) const {
    return vertices_[index];
}
#endif // ! __DOXYGEN

inline bool Component<3>::hasBoundaryTriangles() const {
    return hasBoundaryFacets();
}

inline size_t Component<3>::countBoundaryTriangles() const {
    return countBoundaryFacets();
}

inline bool Component<3>::isIdeal() const {
    return ideal_;
}

inline bool Component<3>::isClosed() const {
    return (boundaryComponents().empty());
}

} // namespace regina

#endif

