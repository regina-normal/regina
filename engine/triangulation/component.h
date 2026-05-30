
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

#ifndef __REGINA_COMPONENT_H
#ifndef __DOXYGEN
#define __REGINA_COMPONENT_H
#endif

/*! \file triangulation/detail/component.h
 *  \brief Implementation details for connected components of triangulations.
 */

#include <ranges>
#include <vector>
#include "regina-core.h"
#include "core/output.h"
#include "triangulation/forward.h"
#include "triangulation/detail/strings.h"
#include "utilities/markedvector.h"
#include "utilities/typeutils.h"

ENSURE_ESSENTIAL_REGINA_HEADERS

namespace regina {

namespace detail {
    template <int dim> requires (supportedDim(dim)) class TriangulationBase;
}

/**
 * A connected component of a <i>dim</i>-manifold triangulation.
 *
 * Components are highly temporary: whenever a triangulation changes, all
 * of its component objects will be deleted and new ones will be created
 * in their place.
 *
 * For Regina's \ref stddim "standard dimensions", this template is specialised
 * and offers more functionality.  In order to use these specialised classes,
 * you will need to include the corresponding triangulation headers (e.g.,
 * triangulation/dim2.h for \a dim = 2, or triangulation/dim3.h
 * for \a dim = 3).
 *
 * Components do not support value semantics: they cannot be copied, swapped,
 * or manually constructed.  Their location in memory defines them, and
 * they are often passed and compared by pointer.  End users are never
 * responsible for their memory management; this is all taken care of by
 * the Triangulation to which they belong.
 *
 * \python Python does not support templates.  Instead
 * this class can be used by appending the dimension as a suffix
 * (e.g., Component2 and Component3 for dimensions 2 and 3).
 *
 * \tparam dim the dimension of the underlying triangulation.
 *
 * \headerfile triangulation/triangulation.h
 *
 * \ingroup triangulation
 */
template <int dim> requires (supportedDim(dim))
class Component : public ShortOutput<Component<dim>>, public MarkedElement {
    public:
        static constexpr int dimension = dim;
            /**< A compile-time constant that gives the dimension of the
                 component. */

    private:
        std::vector<Simplex<dim>*> simplices_;
            /**< List of top-dimensional simplices in the component. */
        std::vector<BoundaryComponent<dim>*> boundaryComponents_;
            /**< List of boundary components in the component. */

        template <int subdim>
        using FaceVector = std::vector<Face<dim, subdim>*>;
            /**< The type used to hold all <i>subdim</i>-faces of this
                 component. */
        [[no_unique_address]] TupleOverRange<standardDim(dim) ? 0 : dim, dim,
                FaceVector> faces_;
            /**< A tuple of vectors holding faces of this component, but only
                 if \a dim is one of Regina's standard dimensions; otherwise
                 this is an empty data member. */

    protected:
        bool valid_;
            /**< Is this component valid?  See Triangulation<dim>::isValid()
             for details on what this means. */

    private:
        size_t boundaryFacets_;
            /**< The number of boundary facets. */
        bool orientable_;
            /**< Is the component orientable? */
        [[no_unique_address]]
                EnableIf<standardDim(dim) && (dim > 2), bool, false> ideal_;
            /**< Is the component ideal? */

    public:
        /**
         * Returns the index of this component within the underlying
         * triangulation.
         *
         * \return the index of this component.
         */
        size_t index() const;

        /**
         * Returns the triangulation to which this component belongs.
         *
         * \return a reference to the triangulation containing this component.
         */
        Triangulation<dim>& triangulation() const;

        /**
         * Returns the number of top-dimensional simplices in this
         * component.
         *
         * \return the number of top-dimensional simplices.
         */
        size_t size() const;
        /**
         * Returns an object that allows iteration through and random access
         * to all top-dimensional simplices in this component.
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
         * for (Simplex<dim>* s : comp.simplices()) { ... }
         * \endcode
         *
         * The object that is returned will remain valid only for as
         * long as this component object exists.  In particular,
         * the object will become invalid any time that the triangulation
         * changes (since all component objects will be destroyed
         * and others rebuilt in their place).
         * Therefore it is best to treat this object as temporary only,
         * and to call simplices() again each time you need it.
         *
         * \return access to the list of all top-dimensional simplices.
         */
        auto simplices() const;
        /**
         * Returns the top-dimensional simplex at the given index in
         * this component.
         *
         * Note that the index within this component may not be the same
         * as the index within the overall triangulation.
         *
         * \param index specifies which simplex to return; this
         * value should be between 0 and size()-1 inclusive.
         * \return the <i>index</i>th top-dimensional simplex.
         */
        Simplex<dim>* simplex(size_t index) const;

        /**
         * Identifies whether any top-dimensional simplices in this component
         * and/or any of their facets are locked.
         *
         * In short, locking a top-dimensional simplex and/or some of its
         * facets means that that the simplex and/or facets must not be
         * changed.  See Simplex<dim>::lock() and Simplex<dim>::lockFacet()
         * for full details on how locks work and what their implications are.
         *
         * \return \c true if and only if there is at least one locked
         * top-dimensional simplex or at least one locked facet of a
         * top-dimensional simplex within this component.
         */
        bool hasLocks() const;

        /**
         * Returns the number of <i>subdim</i>-faces in this component.
         *
         * This is the fastest way to count faces if you know \a subdim
         * at compile time.
         *
         * For convenience, this routine explicitly supports the case
         * `subdim == dim`.  This is _not_ the case for the routines
         * face() and faces(), which give access to individual faces
         * (the reason relates to the fact that top-dimensional simplices
         * are built manually, whereas lower-dimensional faces are deduced
         * properties).
         *
         * \nopython Instead use the variant `countFaces(subdim)`.
         *
         * \tparam subdim the face dimension.
         *
         * \return the number of <i>subdim</i>-faces.
         */
        template <int subdim> requires (subdim >= 0 && subdim <= dim)
        size_t countFaces() const requires (standardDim(dim));

        /**
         * Returns the number of <i>subdim</i>-faces in this component,
         * where the face dimension does not need to be known until runtime.
         *
         * For C++ programmers who know \a subdim at compile time, you are
         * better off using the template function `countFaces<subdim>()`
         * instead, which is (slightly) faster.
         *
         * For convenience, this routine explicitly supports the case
         * `subdim == dim`.  This is _not_ the case for the routines
         * face() and faces(), which give access to individual faces
         * (the reason relates to the fact that top-dimensional simplices
         * are built manually, whereas lower-dimensional faces are deduced
         * properties).
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., negative or greater than \a dim).
         *
         * \param subdim the face dimension; this must be between 0 and \a dim
         * inclusive.
         * \return the number of <i>subdim</i>-faces.
         */
        size_t countFaces(int subdim) const requires (standardDim(dim));

        /**
         * A dimension-specific alias for countFaces<0>().
         *
         * See countFaces() for further information.
         */
        size_t countVertices() const requires (standardDim(dim));

        /**
         * A dimension-specific alias for countFaces<1>().
         *
         * See countFaces() for further information.
         */
        size_t countEdges() const requires (standardDim(dim));

        /**
         * A dimension-specific alias for countFaces<2>().
         *
         * See countFaces() for further information.
         */
        size_t countTriangles() const requires (standardDim(dim));

        /**
         * A dimension-specific alias for countFaces<3>().
         *
         * See countFaces() for further information.
         */
        size_t countTetrahedra() const requires (standardDim(dim) && dim >= 3);

        /**
         * A dimension-specific alias for countFaces<4>().
         *
         * See countFaces() for further information.
         */
        size_t countPentachora() const requires (standardDim(dim) && dim >= 4);

        /**
         * Returns the number of `(dim-1)`-faces in this component.
         *
         * This is available (and constant time) for components in all
         * dimensions, even though the list of `(dim-1)`-faces is only
         * stored for components in Regina's standard dimensions.
         *
         * \return the number of `(dim-1)`-faces.
         */
        size_t countFacets() const;

        /**
         * Returns the number of boundary components in this component.
         *
         * \return the number of boundary components.
         */
        size_t countBoundaryComponents() const;

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
        template <int subdim> requires (subdim >= 0 && subdim < dim)
        auto faces() const requires (standardDim(dim));

        /**
         * Returns an object that allows iteration through and random access
         * to all <i>subdim</i>-faces in this component, in a way that is
         * optimised for Python programmers.
         *
         * C++ users should not use this routine.  The return type must be fixed
         * at compile time, and so it is typically a `std::variant` that can
         * hold any of the lightweight view types returned from the templated
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
         * the supported range (i.e., negative, or greater than `dim-1`).
         *
         * \param subdim the face dimension; this must be between 0 and `dim-1`
         * inclusive.
         * \return access to the list of all <i>subdim</i>-faces.
         */
        auto faces(int subdim) const requires (standardDim(dim));

        /**
         * A dimension-specific alias for faces<0>().
         *
         * See faces() for further information.
         */
        auto vertices() const requires (standardDim(dim));

        /**
         * A dimension-specific alias for faces<1>().
         *
         * See faces() for further information.
         */
        auto edges() const requires (standardDim(dim));

        /**
         * A dimension-specific alias for faces<2>(), or an alias for
         * simplices() in dimension \a dim = 2.
         *
         * See faces() for further information.
         */
        auto triangles() const requires (standardDim(dim));

        /**
         * A dimension-specific alias for faces<3>(), or an alias for
         * simplices() in dimension \a dim = 3.
         *
         * See faces() for further information.
         */
        auto tetrahedra() const requires (standardDim(dim) && dim >= 3);

        /**
         * A dimension-specific alias for faces<4>(), or an alias for
         * simplices() in dimension \a dim = 4.
         *
         * See faces() for further information.
         */
        auto pentachora() const requires (standardDim(dim) && dim >= 4);

        /**
         * Returns an object that allows iteration through and random access
         * to all boundary components in this component.
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
         * for (BoundaryComponent<dim>* bc : comp.boundaryComponents()) { ... }
         * \endcode
         *
         * The object that is returned will remain valid only for as
         * long as this component object exists.  In particular,
         * the object will become invalid any time that the triangulation
         * changes (since all component objects will be destroyed
         * and others rebuilt in their place).
         * Therefore it is best to treat this object as temporary only,
         * and to call boundaryComponents() again each time you need it.
         *
         * \return access to the list of all boundary components.
         */
        auto boundaryComponents() const;

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
        template <int subdim> requires (subdim >= 0 && subdim < dim)
        Face<dim, subdim>* face(size_t index) const requires (standardDim(dim));

        /**
         * Returns the requested <i>subdim</i>-face in this component, in a
         * way that is optimised for Python programmers.
         *
         * C++ users should not use this routine.  The return type must be
         * fixed at compile time, and so it is typically a `std::variant` that
         * could store a pointer to any class `Face<dim, ...>`.  This means you
         * cannot access the face directly: you will still need some kind of
         * compile-time knowledge of \a subdim before you can extract and use
         * an appropriate `Face<dim, subdim>` object from \a v.  However, once
         * you know \a subdim at compile time, you are better off using the
         * (simpler and faster) routine `face<subdim>()` instead.
         *
         * For Python users, this routine is much more useful: the return type
         * can be chosen at runtime, and so this routine simply returns a
         * `Face<dim, subdim>` object of the appropriate face dimension that
         * you can use immediately.
         *
         * The specific return type for C++ programmers will be
         * `std::variant<Face<dim, 0>*, ..., Face<dim, dim-1>*>`.
         *
         * Note that the index of a face in the component need
         * not be the index of the same face in the overall triangulation.
         *
         * \exception InvalidArgument The face dimension \a subdim is outside
         * the supported range (i.e., negative, or greater than `dim-1`).
         *
         * \param subdim the face dimension; this must be between 0 and `dim-1`
         * inclusive.
         * \param index the index of the desired face, ranging from 0 to
         * countFaces<subdim>()-1 inclusive.
         * \return the requested face.
         */
        auto face(int subdim, size_t index) const requires (standardDim(dim));

        /**
         * A dimension-specific alias for face<0>().
         *
         * See face() for further information.
         */
        TriangulationTraits<dim>::Vertex* vertex(size_t index) const
            requires (standardDim(dim));

        /**
         * A dimension-specific alias for face<1>().
         *
         * See face() for further information.
         */
        TriangulationTraits<dim>::Edge* edge(size_t index) const
            requires (standardDim(dim));

        /**
         * A dimension-specific alias for face<2>().
         *
         * See face() for further information.
         */
        TriangulationTraits<dim>::Triangle* triangle(size_t index) const
            requires (standardDim(dim));

        /**
         * A dimension-specific alias for face<3>().
         *
         * See face() for further information.
         */
        TriangulationTraits<dim>::Tetrahedron* tetrahedron(size_t index) const
            requires (standardDim(dim) && dim >= 3);

        /**
         * A dimension-specific alias for face<4>().
         *
         * See face() for further information.
         */
        TriangulationTraits<dim>::Pentachoron* pentachoron(size_t index) const
            requires (standardDim(dim) && dim >= 4);

        /**
         * Returns the boundary component at the given index in this component.
         *
         * Note that the index of a boundary component within this component
         * may not be the same as its index within the overall triangulation.
         *
         * \param index specifies which boundary component to return;
         * this should be between 0 and countBoundaryComponents()-1 inclusive.
         * \return the requested boundary component.
         */
        BoundaryComponent<dim>* boundaryComponent(size_t index) const;

        /**
         * Determines if this component is valid.
         *
         * This uses the same criteria as Triangulation<dim>::isValid();
         * see the Triangulation<dim>::isValid() documentation for details.
         *
         * As with Triangulation<dim>, this tests for bad self-identifications
         * in all dimensions, but only tests for bad links in Regina's
         * \ref stddim "standard dimensions".
         *
         * \return \c true if and only if this component is valid.
         */
        bool isValid() const;

        /**
         * Determines if this component is orientable.
         *
         * This routine runs in constant time (since orientability is
         * determined in advance, when the component is first created).
         * 
         * \return \c true if and only if this component is orientable.
         */
        bool isOrientable() const;

        /**
         * Determines if this component has any boundary facets.
         *
         * This routine returns \c true if and only if this component
         * contains some top-dimensional simplex with at least one facet
         * that is not glued to an adjacent simplex.
         *
         * \return \c true if and only if this component has boundary facet(s).
         */
        bool hasBoundaryFacets() const;

        /**
         * An alias for hasBoundaryFacets() in dimension 2.
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryEdges() const requires (dim == 2);

        /**
         * An alias for hasBoundaryFacets() in dimension 3.
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryTriangles() const requires (dim == 3);

        /**
         * An alias for hasBoundaryFacets() in dimension 4.
         *
         * See hasBoundaryFacets() for further information.
         */
        bool hasBoundaryTetrahedra() const requires (dim == 4);

        /**
         * Returns the number of boundary facets in this component.
         *
         * A boundary facet is a (<i>dim</i>-1)-dimensional facet of a
         * top-dimensional simplex that is not joined to any adjacent
         * simplex.
         *
         * This routine runs in constant time (since the result is
         * computed in advance, when the component is first created).
         *
         * \return the total number of boundary facets.
         */
        size_t countBoundaryFacets() const;

        /**
         * An alias for countBoundaryFacets() in dimension 2.
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryEdges() const requires (dim == 2);

        /**
         * An alias for countBoundaryFacets() in dimension 3.
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryTriangles() const requires (dim == 3);

        /**
         * An alias for countBoundaryFacets() in dimension 4.
         *
         * See countBoundaryFacets() for further information.
         */
        size_t countBoundaryTetrahedra() const requires (dim == 4);

        /**
         * Determines if this component is ideal.
         * This is the case if and only if it contains an ideal vertex
         * as described by `Vertex<dim>::isIdeal()`.
         *
         * \return \c true if and only if this component is ideal.
         */
        bool isIdeal() const requires (standardDim(dim) && dim > 2);

        /**
         * Determines if this component is closed.
         * This is the case if and only if it has no boundary.
         *
         * Note that ideal components and components with invalid vertices
         * are not closed (these can only appear in dimensions ≥ 3).
         * See `Vertex<dim>::isBoundary()` for details.
         *
         * \return \c true if and only if this component is closed.
         */
        bool isClosed() const requires (standardDim(dim));

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \nopython Use str() instead.
         *
         * \param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;

        // Make this class non-copyable.
        Component(const Component&) = delete;
        Component& operator = (const Component&) = delete;

    protected:
        /**
         * Default constructor.
         *
         * Marks the component as orientable, with no boundary facets.
         */
        Component();

    friend class Triangulation<dim>;
    friend class detail::TriangulationBase<dim>;
};

// Inline functions for Component

template <int dim> requires (supportedDim(dim))
inline Component<dim>::Component() :
        valid_(true), boundaryFacets_(0), orientable_(true) {
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::index() const {
    return markedIndex();
}

template <int dim> requires (supportedDim(dim))
inline Triangulation<dim>& Component<dim>::triangulation() const {
    // This is a connected component, so simplices_ must be non-empty.
    return simplices_.front()->triangulation();
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::size() const {
    return simplices_.size();
}

template <int dim> requires (supportedDim(dim))
inline auto Component<dim>::simplices() const {
    return std::views::all(simplices_);
}

template <int dim> requires (supportedDim(dim))
inline Simplex<dim>* Component<dim>::simplex(size_t index) const {
    return simplices_[index];
}

template <int dim> requires (supportedDim(dim))
inline bool Component<dim>::hasLocks() const {
    for (auto s : simplices_)
        if (s->lockMask())
            return true;
    return false;
}

template <int dim> requires (supportedDim(dim))
template <int subdim> requires (subdim >= 0 && subdim <= dim)
inline size_t Component<dim>::countFaces() const
        requires (standardDim(dim)) {
    if constexpr (subdim == dim)
        return size();
    else
        return std::get<subdim>(faces_).size();
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::countFaces(int subdim) const
        requires (standardDim(dim)) {
    if (subdim == dim)
        return size();
    if (subdim < 0 || subdim > dim)
        throw InvalidArgument("countFaces(): unsupported face dimension");

    return select_constexpr<0, dim, size_t>(subdim, [this](auto k) {
        return std::get<k>(faces_).size();
    });
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::countVertices() const
        requires (standardDim(dim)) {
    return countFaces<0>();
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::countEdges() const
        requires (standardDim(dim)) {
    return countFaces<1>();
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::countTriangles() const
        requires (standardDim(dim)) {
    return countFaces<2>();
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::countTetrahedra() const
        requires (standardDim(dim) && dim >= 3) {
    return countFaces<3>();
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::countPentachora() const
        requires (standardDim(dim) && dim >= 4) {
    return countFaces<4>();
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::countFacets() const {
    if constexpr (standardDim(dim)) {
        return static_cast<const Component<dim>*>(this)->
            template countFaces<dim - 1>();
    } else {
        return ((dim + 1) * simplices_.size() + boundaryFacets_) / 2;
    }
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::countBoundaryComponents() const {
    return boundaryComponents_.size();
}

template <int dim> requires (supportedDim(dim))
template <int subdim> requires (subdim >= 0 && subdim < dim)
inline auto Component<dim>::faces() const requires (standardDim(dim)) {
    return std::views::all(std::get<subdim>(faces_));
}

template <int dim> requires (supportedDim(dim))
inline auto Component<dim>::faces(int subdim) const
        requires (standardDim(dim)) {
    if (subdim < 0 || subdim >= dim)
        throw InvalidArgument("faces(): unsupported face dimension");

    return select_constexpr_as_variant<0, dim>(subdim, [this](auto k) {
        return std::views::all(std::get<k>(faces_));
    });
}

template <int dim> requires (supportedDim(dim))
inline auto Component<dim>::vertices() const requires (standardDim(dim)) {
    return faces<0>();
}

template <int dim> requires (supportedDim(dim))
inline auto Component<dim>::edges() const requires (standardDim(dim)) {
    return faces<1>();
}

template <int dim> requires (supportedDim(dim))
inline auto Component<dim>::triangles() const requires (standardDim(dim)) {
    if constexpr (dim == 2)
        return std::views::all(simplices_);
    else
        return faces<2>();
}

template <int dim> requires (supportedDim(dim))
inline auto Component<dim>::tetrahedra() const
        requires (standardDim(dim) && dim >= 3) {
    if constexpr (dim == 3)
        return std::views::all(simplices_);
    else
        return faces<3>();
}

template <int dim> requires (supportedDim(dim))
inline auto Component<dim>::pentachora() const
        requires (standardDim(dim) && dim >= 4) {
    if constexpr (dim == 4)
        return std::views::all(simplices_);
    else
        return faces<4>();
}

template <int dim> requires (supportedDim(dim))
inline auto Component<dim>::boundaryComponents() const {
    return std::views::all(boundaryComponents_);
}

template <int dim> requires (supportedDim(dim))
template <int subdim> requires (subdim >= 0 && subdim < dim)
inline Face<dim, subdim>* Component<dim>::face(size_t index) const
        requires (standardDim(dim)) {
    return std::get<subdim>(faces_)[index];
}

template <int dim> requires (supportedDim(dim))
inline auto Component<dim>::face(int subdim, size_t index) const
        requires (standardDim(dim)) {
    if (subdim < 0 || subdim >= dim)
        throw InvalidArgument("face(): unsupported face dimension");

    return select_constexpr_as_variant<0, dim>(subdim, [this, index](auto k) {
        return std::get<k>(faces_)[index];
    });
}

template <int dim> requires (supportedDim(dim))
inline TriangulationTraits<dim>::Vertex* Component<dim>::vertex(
        size_t index) const
        requires (standardDim(dim)) {
    return face<0>(index);
}

template <int dim> requires (supportedDim(dim))
inline TriangulationTraits<dim>::Edge* Component<dim>::edge(
        size_t index) const
        requires (standardDim(dim)) {
    return face<1>(index);
}

template <int dim> requires (supportedDim(dim))
inline TriangulationTraits<dim>::Triangle* Component<dim>::triangle(
        size_t index) const
        requires (standardDim(dim)) {
    if constexpr (dim == 2)
        return simplices_[index];
    else
        return face<2>(index);
}

template <int dim> requires (supportedDim(dim))
inline TriangulationTraits<dim>::Tetrahedron* Component<dim>::tetrahedron(
        size_t index) const
        requires (standardDim(dim) && dim >= 3) {
    if constexpr (dim == 3)
        return simplices_[index];
    else
        return face<3>(index);
}

template <int dim> requires (supportedDim(dim))
inline TriangulationTraits<dim>::Pentachoron* Component<dim>::pentachoron(
        size_t index) const
        requires (standardDim(dim) && dim >= 4) {
    if constexpr (dim == 4)
        return simplices_[index];
    else
        return face<4>(index);
}

template <int dim> requires (supportedDim(dim))
inline BoundaryComponent<dim>* Component<dim>::boundaryComponent(size_t index)
        const {
    return boundaryComponents_[index];
}

template <int dim> requires (supportedDim(dim))
inline bool Component<dim>::isValid() const {
    return valid_;
}

template <int dim> requires (supportedDim(dim))
inline bool Component<dim>::isOrientable() const {
    return orientable_;
}

template <int dim> requires (supportedDim(dim))
inline bool Component<dim>::hasBoundaryFacets() const {
    return boundaryFacets_;
}

template <int dim> requires (supportedDim(dim))
inline bool Component<dim>::hasBoundaryEdges() const requires (dim == 2) {
    return boundaryFacets_;
}

template <int dim> requires (supportedDim(dim))
inline bool Component<dim>::hasBoundaryTriangles() const requires (dim == 3) {
    return boundaryFacets_;
}

template <int dim> requires (supportedDim(dim))
inline bool Component<dim>::hasBoundaryTetrahedra() const requires (dim == 4) {
    return boundaryFacets_;
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::countBoundaryFacets() const {
    return boundaryFacets_;
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::countBoundaryEdges() const
        requires (dim == 2) {
    return boundaryFacets_;
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::countBoundaryTriangles() const
        requires (dim == 3) {
    return boundaryFacets_;
}

template <int dim> requires (supportedDim(dim))
inline size_t Component<dim>::countBoundaryTetrahedra()
        const requires (dim == 4) {
    return boundaryFacets_;
}

template <int dim> requires (supportedDim(dim))
inline bool Component<dim>::isIdeal() const
        requires (standardDim(dim) && dim > 2) {
    return ideal_.value;
}

template <int dim> requires (supportedDim(dim))
inline bool Component<dim>::isClosed() const requires (standardDim(dim)) {
    return (boundaryComponents().empty());
}

template <int dim> requires (supportedDim(dim))
void Component<dim>::writeTextShort(std::ostream& out) const {
    if (simplices_.size() == 1)
        out << "Component with 1 " << detail::Strings<dim>::simplex;
    else
        out << "Component with " << simplices_.size() << ' '
            << detail::Strings<dim>::simplices;
    out << ':';

    if (simplices_.front()->triangulation().countComponents() == 1) {
        out << " entire triangulation";
    } else {
        for (auto it = simplices_.begin(); it != simplices_.end(); ++it)
            out << ' ' << (*it)->index();
    }
}

} // namespace regina

#endif

