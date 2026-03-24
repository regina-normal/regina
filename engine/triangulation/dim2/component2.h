
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include <variant>

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

/**
 * Represents a connected component of a 2-manifold triangulation.
 *
 * This is a specialisation of the generic Component class template; see the
 * generic Component documentation for an overview of how the component
 * classes work.  In Python, you can read this generic documentation by
 * looking at a higher dimension: try `help(Component5)`.
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
         * This is the fastest way to count faces if you know \a subdim
         * at compile time.
         *
         * For convenience, this routine explicitly supports the case
         * \a subdim = 2.  This is _not_ the case for the routines
         * face() and faces(), which give access to individual faces
         * (the reason relates to the fact that triangles are built manually,
         * whereas lower-dimensional faces are deduced properties).
         *
         * \nopython Instead use the variant `countFaces(subdim)`.
         *
         * \tparam subdim the face dimension.
         *
         * \return the number of <i>subdim</i>-faces.
         */
        template <int subdim> requires (subdim >= 0 && subdim <= 2)
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
         * \a subdim = 2.  This is _not_ the case for the routines
         * face() and faces(), which give access to individual faces
         * (the reason relates to the fact that triangles are built manually,
         * whereas lower-dimensional faces are deduced properties).
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., negative or greater than 2).
         *
         * \param subdim the face dimension; this must be between 0 and 2
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
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * range-based `for` loops.  Note that the elements of the list
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
         * \nopython Instead use the variant `faces(subdim)`.
         *
         * \tparam subdim the face dimension.
         *
         * \return access to the list of all <i>subdim</i>-faces.
         */
        template <int subdim> requires (subdim >= 0 && subdim < 2)
        auto faces() const;

        /**
         * Returns an object that allows iteration through and random access
         * to all <i>subdim</i>-faces in this component, in a way that is
         * optimised for Python programmers.
         *
         * C++ users should not use this routine.  The return type must be
         * fixed at compile time, and so it is typically a `std::variant` that
         * can hold any of the lightweight return types from the templated
         * `faces<subdim>()` function.  This means that the return value will
         * still need compile-time knowledge of \a subdim to extract and
         * use the appropriate face objects.  However, once you know \a subdim
         * at compile time, you are much better off using the (simpler and
         * faster) routine `faces<subdim>()` instead.
         *
         * For Python users, this routine is much more useful: the return type
         * can be chosen at runtime, and so this routine returns a single
         * lightweight object granting access to all of the <i>subdim</i>-faces
         * of the component, which you can use immediately.
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., negative, or greater than 1).
         *
         * \param subdim the face dimension; this must be between 0 and 1
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
        template <int subdim> requires (subdim >= 0 && subdim < 2)
        Face<2, subdim>* face(size_t index) const;

        /**
         * Returns the requested <i>subdim</i>-face in this component, in a
         * way that is optimised for Python programmers.
         *
         * C++ users should not use this routine.  The return type must be
         * fixed at compile time, and so it is typically a `std::variant` that
         * could store a pointer to any class `Face<2, ...>`.  This means you
         * cannot access the face directly: you will still need some kind of
         * compile-time knowledge of \a subdim before you can extract and use
         * an appropriate `Face<2, subdim>` object from \a v.  However, once
         * you know \a subdim at compile time, you are better off using the
         * (simpler and faster) routine `face<subdim>()` instead.
         *
         * For Python users, this routine is much more useful: the return type
         * can be chosen at runtime, and so this routine simply returns a
         * `Face<2, subdim>` object of the appropriate face dimension that
         * you can use immediately.
         *
         * The specific return type for C++ programmers will be
         * `std::variant<Face<2, 0>*, Face<2, 1>*>`.
         *
         * Note that the index of a face in the component need
         * not be the index of the same face in the overall triangulation.
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., negative, or greater than 1).
         *
         * \param subdim the face dimension; this must be between 0 and 1
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

inline size_t Component<2>::countFaces(int subdim) const {
    switch (subdim) {
        case 2: return size();
        case 1: return edges_.size();
        case 0: return vertices_.size();
        default:
            throw InvalidArgument("countFaces(): unsupported face dimension");
    }
}

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
#endif // ! __DOXYGEN

inline auto Component<2>::faces(int subdim) const {
    using Return = std::variant<
        decltype(ListView(vertices_)),
        decltype(ListView(edges_))>;
    switch (subdim) {
        case 0: return Return(ListView(vertices_));
        case 1: return Return(ListView(edges_));
        default: throw InvalidArgument("faces(): unsupported face dimension");
    }
}

// Hide specialisations from doxygen, since it cannot handle them.
#ifndef __DOXYGEN
template <>
inline auto Component<2>::faces<1>() const {
    return ListView(edges_);
}

template <>
inline auto Component<2>::faces<0>() const {
    return ListView(vertices_);
}
#endif // ! __DOXYGEN

inline auto Component<2>::face(int subdim, size_t index) const {
    using Return = std::variant<Face<2, 0>*, Face<2, 1>*>;
    switch (subdim) {
        case 0: return Return(vertices_[index]);
        case 1: return Return(edges_[index]);
        default: throw InvalidArgument("face(): unsupported face dimension");
    }
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

