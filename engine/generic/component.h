
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

#ifndef __COMPONENT_H
#ifndef __DOXYGEN
#define __COMPONENT_H
#endif

/*! \file generic/component.h
 *  \brief Deals with connected components of triangulations.
 */

#include <vector>
#include "regina-core.h"
#include "output.h"
#include "utilities/nmarkedvector.h"
#include <boost/noncopyable.hpp>

namespace regina {

template <int> class Simplex;
template <int> class Triangulation;
template <int> class TriangulationBase;

/**
 * \weakgroup generic
 * @{
 */

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
 * \ifacespython This base class is not present, but the "end user"
 * class Component<dim> is.
 *
 * \tparam the dimension of the underlying triangulation.  This must be
 * at least 2.
 */
template <int dim>
class REGINA_API ComponentBase :
        public Output<ComponentBase<dim>>,
        public boost::noncopyable,
        public NMarkedElement {
    private:
        std::vector<Simplex<dim>*> simplices_;
            /**< List of triangles in the component. */

        size_t boundaryFacets_;
            /**< The number of boundary facets. */
        bool orientable_;
            /**< Is the component orientable? */

    public:
        /**
         * Returns the index of this component in the underlying
         * triangulation.  <tt>t->componentIndex(this)</tt>, where
         * \a t represents the underlying triangulation.
         *
         * @return the index of this component vertex.
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
         * Deprecated routine that returns the number of top-dimensional
         * simplices in this component.
         *
         * \deprecated Simply call size() instead.
         *
         * @return the number of top-dimensional simplices.
         */
        size_t getNumberOfSimplices() const;
        /**
         * Returns all top-dimensional simplices in this component.
         *
         * The reference that is returned will remain valid only for as long
         * as this component object exists.  In particular, the reference
         * will become invalid any time that the triangulation changes
         * (since all component objects will be destroyed and others rebuilt
         * in their place).
         *
         * \ifacespython This routine returns a python list.
         *
         * @return the list of all top-dimensional simplices.
         */
        const std::vector<Simplex<dim>*>& simplices() const;
        /**
         * Deprecated routine that returns all of the top-dimensional
         * simplices in this component.
         *
         * \deprecated Simply call simplices() instead.
         *
         * See simplices() for further details.
         */
        const std::vector<Simplex<dim>*>& getSimplices() const;
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
         * Deprecated routine that returns the top-dimensional simplex
         * at the given index in this component.
         *
         * \deprecated Simply call simplex() instead.
         *
         * See simplices() for further details.
         */
        Simplex<dim>* getSimplex(size_t index) const;

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
         * Deprecated routine that returns the number of boundary facets
         * in this component.
         *
         * \deprecated Simply call countBoundaryFacets() instead.
         *
         * See countBoundaryFacets() for further details.
         */
        size_t getNumberOfBoundaryFacets() const;

        /**
         * Writes a short text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextShort(std::ostream& out) const;
        /**
         * Writes a detailed text representation of this object to the
         * given output stream.
         *
         * \ifacespython Not present.
         *
         * @param out the output stream to which to write.
         */
        void writeTextLong(std::ostream& out) const;

    protected:
        /**
         * Default constructor.
         *
         * Marks the component as orientable, with no boundary facets.
         */
        ComponentBase();

    friend class TriangulationBase<dim>;
};

/**
 * A connected component of a <i>dim</i>-manifold triangulation.
 *
 * Components are highly temporary: whenever a triangulation changes, all
 * of its component objects will be deleted and new ones will be created
 * in their place.
 *
 * Component objects are all created, managed and destroyed by the
 * class Triangulation<dim>.  See the Triangulation notes for further
 * information on working with <i>dim</i>-dimensional triangulations.
 *
 * For dimensions 2 and 3, this template is specialised and offers more
 * functionality.  In order to use these specialised classes, you will
 * need to include the corresponding headers (dim2/dim2component.h for
 * \a dim = 2, or triangulation/ncomponent.h for \a dim = 3).  For
 * convenience, there are typedefs available for these specialised classes
 * (Dim2Component and NComponent respectively).
 *
 * \ifacespython Python does not support templates.  For \a dim = 2 and 3,
 * this class is available in Python under the names Component2 and
 * Component3 respectively (as well as the typedefs mentioned above).
 * Higher-dimensional classes are not available in Python for the time being.
 *
 * \tparam dim the dimension of the underlying triangulation.
 * This must be at least 2.
 */
template <int dim>
class REGINA_API Component : public ComponentBase<dim> {
    private:
        /**
         * Default constructor.
         *
         * Marks the component as orientable, with no boundary facets.
         */
        Component();

    friend class TriangulationBase<dim>;
};

// Note that some of our component classes are specialised elsewhere.
// Do not explicitly drag in the specialised headers for now.
template <> class Component<2>;
template <> class Component<3>;

/*@}*/

// Inline functions for ComponentBase

template <int dim>
inline ComponentBase<dim>::ComponentBase() :
        orientable_(true), boundaryFacets_(0) {
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
inline size_t ComponentBase<dim>::getNumberOfSimplices() const {
    return simplices_.size();
}

template <int dim>
inline const std::vector<Simplex<dim>*>& ComponentBase<dim>::simplices() const {
    return simplices_;
}

template <int dim>
inline const std::vector<Simplex<dim>*>& ComponentBase<dim>::getSimplices()
        const {
    return simplices_;
}

template <int dim>
inline Simplex<dim>* ComponentBase<dim>::simplex(size_t index) const {
    return simplices_[index];
}

template <int dim>
inline Simplex<dim>* ComponentBase<dim>::getSimplex(size_t index) const {
    return simplices_[index];
}

template <int dim>
inline bool ComponentBase<dim>::isOrientable() const {
    return orientable_;
}

template <int dim>
inline size_t ComponentBase<dim>::countBoundaryFacets() const {
    return boundaryFacets_;
}

template <int dim>
inline size_t ComponentBase<dim>::getNumberOfBoundaryFacets() const {
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

// Inline functions for Component

template <int dim>
inline Component<dim>::Component() : ComponentBase<dim>() {
}

} // namespace regina

#endif

