
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#ifndef __REGINA_COMPONENT_H_DETAIL
#ifndef __DOXYGEN
#define __REGINA_COMPONENT_H_DETAIL
#endif

/*! \file triangulation/detail/component.h
 *  \brief Implementation details for connected components of triangulations.
 */

#include <vector>
#include "regina-core.h"
#include "core/output.h"
#include "triangulation/alias/face.h"
#include "triangulation/alias/simplex.h"
#include "triangulation/forward.h"
#include "utilities/listview.h"
#include "utilities/markedvector.h"

namespace regina::detail {

template <int> class TriangulationBase;

/**
 * Helper class that provides core functionality for a connected component
 * of a <i>dim</i>-manifold triangulation.
 *
 * Each connected component is represented by the class Component<dim>,
 * which uses this as a base class.  End users should not need to refer
 * to ComponentBase directly.
 *
 * See the Component class notes for further information.
 *
 * Neither this class nor the "end user" class Component<dim> support
 * value semantics: they cannot be copied, swapped, or manually constructed.
 * Their memory is managed by the Triangulation class, and their locations
 * in memory define them.  See Component<dim> for further details.
 *
 * \ifacespython This base class is not present, but the "end user"
 * class Component<dim> is.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be between 2 and 15 inclusive.
 *
 * \ingroup detail
 */
template <int dim>
class ComponentBase :
        public Output<ComponentBase<dim>>,
        public alias::Simplices<ComponentBase<dim>, dim>,
        public MarkedElement {
    public:
        static constexpr int dimension = dim;
            /**< A compile-time constant that gives the dimension of the
                 component. */

    private:
        std::vector<Simplex<dim>*> simplices_;
            /**< List of top-dimensional simplices in the component. */
        std::vector<BoundaryComponent<dim>*> boundaryComponents_;
            /**< List of boundary components in the component. */

    protected:
        bool valid_;
            /**< Is this component valid?  See Triangulation<dim>::isValid()
             for details on what this means. */

    private:
        size_t boundaryFacets_;
            /**< The number of boundary facets. */
        bool orientable_;
            /**< Is the component orientable? */

    public:
        /**
         * Returns the index of this component within the underlying
         * triangulation.
         *
         * @return the index of this component.
         */
        size_t index() const;

        /**
         * Returns the number of top-dimensional simplices in this
         * component.
         *
         * @return The number of top-dimensional simplices.
         */
        size_t size() const;
        /**
         * Returns an object that allows iteration through and random access
         * to all top-dimensional simplices in this component.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * C++11 range-based \c for loops.  Note that the elements of the list
         * will be pointers, so your code might look like:
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
         * @return access to the list of all top-dimensional simplices.
         */
        auto simplices() const;
        /**
         * Returns the top-dimensional simplex at the given index in
         * this component.
         *
         * Note that the index within this component may not be the same
         * as the index within the overall triangulation.
         *
         * @param index specifies which simplex to return; this
         * value should be between 0 and size()-1 inclusive.
         * @return the <i>index</i>th top-dimensional simplex.
         */
        Simplex<dim>* simplex(size_t index) const;

        /**
         * Returns the number of boundary components in this component.
         *
         * @return the number of boundary components.
         */
        size_t countBoundaryComponents() const;
        /**
         * Returns an object that allows iteration through and random access
         * to all boundary components in this component.
         *
         * The object that is returned is lightweight, and can be happily
         * copied by value.  The C++ type of the object is subject to change,
         * so C++ users should use \c auto (just like this declaration does).
         *
         * The returned object is guaranteed to be an instance of ListView,
         * which means it offers basic container-like functions and supports
         * C++11 range-based \c for loops.  Note that the elements of the list
         * will be pointers, so your code might look like:
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
         * @return access to the list of all boundary components.
         */
        auto boundaryComponents() const;
        /**
         * Returns the boundary component at the given index in this component.
         *
         * Note that the index of a boundary component within this component
         * may not be the same as its index within the overall triangulation.
         *
         * @param index specifies which boundary component to return;
         * this should be between 0 and countBoundaryComponents()-1 inclusive.
         * @return the requested boundary component.
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
         * @return \c true if and only if this component is valid.
         */
        bool isValid() const;

        /**
         * Determines if this component is orientable.
         *
         * This routine runs in constant time (since orientability is
         * determined in advance, when the component is first created).
         * 
         * @return \c true if and only if this component is orientable.
         */
        bool isOrientable() const;

        /**
         * Determines if this component has any boundary facets.
         *
         * This routine returns \c true if and only if this component
         * contains some top-dimensional simplex with at least one facet
         * that is not glued to an adjacent simplex.
         *
         * @return \c true if and only if this component has boundary facet(s).
         */
        bool hasBoundaryFacets() const;
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
         * @return the total number of boundary facets.
         */
        size_t countBoundaryFacets() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present; use str() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present; use detail() instead.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

        // Make this class non-copyable.
        ComponentBase(const ComponentBase&) = delete;
        ComponentBase& operator = (const ComponentBase&) = delete;

    protected:
        /**
         * Default constructor.
         *
         * Marks the component as orientable, with no boundary facets.
         */
        ComponentBase();

    friend class Triangulation<dim>;
    friend class TriangulationBase<dim>;
};

// Inline functions for ComponentBase

template <int dim>
inline ComponentBase<dim>::ComponentBase() :
        valid_(true), boundaryFacets_(0), orientable_(true) {
}

template <int dim>
inline size_t ComponentBase<dim>::index() const {
    return markedIndex();
}

template <int dim>
inline size_t ComponentBase<dim>::size() const {
    return simplices_.size();
}

template <int dim>
inline auto ComponentBase<dim>::simplices() const {
    return ListView(simplices_);
}

template <int dim>
inline Simplex<dim>* ComponentBase<dim>::simplex(size_t index) const {
    return simplices_[index];
}

template <int dim>
inline size_t ComponentBase<dim>::countBoundaryComponents() const {
    return boundaryComponents_.size();
}

template <int dim>
inline auto ComponentBase<dim>::boundaryComponents() const {
    return ListView(boundaryComponents_);
}

template <int dim>
inline BoundaryComponent<dim>* ComponentBase<dim>::boundaryComponent(
        size_t index) const {
    return boundaryComponents_[index];
}

template <int dim>
inline bool ComponentBase<dim>::isValid() const {
    return valid_;
}

template <int dim>
inline bool ComponentBase<dim>::isOrientable() const {
    return orientable_;
}

template <int dim>
inline bool ComponentBase<dim>::hasBoundaryFacets() const {
    return boundaryFacets_;
}

template <int dim>
inline size_t ComponentBase<dim>::countBoundaryFacets() const {
    return boundaryFacets_;
}

template <int dim>
void ComponentBase<dim>::writeTextShort(std::ostream& out) const {
    if (simplices_.size() == 1)
        out << "Component with 1 " << dim << "-simplex";
    else
        out << "Component with " << simplices_.size() << ' '
            << dim << "-simplices";
}

template <int dim>
void ComponentBase<dim>::writeTextLong(std::ostream& out) const {
    writeTextShort(out);
    out << std::endl;

    out << (simplices_.size() == 1 ? "Simplex:" : "Simplices:");
    for (auto it = simplices_.begin(); it != simplices_.end(); ++it)
        out << ' ' << (*it)->markedIndex();
    out << std::endl;
}

} // namespace regina::detail

#endif

