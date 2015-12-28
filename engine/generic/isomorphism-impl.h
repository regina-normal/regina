
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

/*! \file generic/isomorphism-impl.h
 *  \brief Contains some of the implementation details for the
 *  NGenericTriangulation class template.
 *
 *  This file is \e not included automatically by ngenerictriangulation.h.
 *  However, typical end users should never need to include it, since
 *  Regina's calculation engine provides full explicit instantiations
 *  of NGenericTriangulation for \ref stddim "standard dimensions".
 */

#include "generic/ngenerictriangulation.h"

namespace regina {

template <int dim>
bool NGenericTriangulation<dim>::isIdenticalTo(
        const typename DimTraits<dim>::Triangulation& other) const {
    const typename DimTraits<dim>::Triangulation& me(
        static_cast<const typename DimTraits<dim>::Triangulation&>(*this));
    if (me.getNumberOfSimplices() != other.getNumberOfSimplices())
        return false;

    unsigned long i;
    unsigned j;
    for (i = 0; i < me.getNumberOfSimplices(); ++i)
        for (j = 0; j <= dim; ++j) {
            if (me.getSimplex(i)->adjacentSimplex(j)) {
                if (! other.getSimplex(i)->adjacentSimplex(j))
                    return false;
                if (me.getSimplex(i)->adjacentSimplex(j)->markedIndex() !=
                        other.getSimplex(i)->adjacentSimplex(j)->markedIndex())
                    return false;
                if (me.getSimplex(i)->adjacentGluing(j) !=
                        other.getSimplex(i)->adjacentGluing(j))
                    return false;
            } else {
                if (other.getSimplex(i)->adjacentSimplex(j))
                    return false;
            }
        }

    return true;
}

template <int dim>
inline std::unique_ptr<typename DimTraits<dim>::Isomorphism>
        NGenericTriangulation<dim>::isIsomorphicTo(
        const typename DimTraits<dim>::Triangulation& other) const {
    std::list<typename DimTraits<dim>::Isomorphism*> results;
    if (static_cast<const typename DimTraits<dim>::Triangulation&>(*this).
            findIsomorphisms(other, results, true, true))
        return std::unique_ptr<typename DimTraits<dim>::Isomorphism>(
            results.front());
    else
        return nullptr;
}

template <int dim>
inline std::unique_ptr<typename DimTraits<dim>::Isomorphism>
        NGenericTriangulation<dim>::isContainedIn(
        const typename DimTraits<dim>::Triangulation& other) const {
    std::list<typename DimTraits<dim>::Isomorphism*> results;
    if (static_cast<const typename DimTraits<dim>::Triangulation&>(*this).
            findIsomorphisms(other, results, false, true))
        return std::unique_ptr<typename DimTraits<dim>::Isomorphism>(
            results.front());
    else
        return nullptr;
}

template <int dim>
inline unsigned long NGenericTriangulation<dim>::findAllIsomorphisms(
        const typename DimTraits<dim>::Triangulation& other,
        std::list<typename DimTraits<dim>::Isomorphism*>& results) const {
    return static_cast<const typename DimTraits<dim>::Triangulation&>(*this).
        findIsomorphisms(other, results, true, false);
}

template <int dim>
inline unsigned long NGenericTriangulation<dim>::findAllSubcomplexesIn(
        const typename DimTraits<dim>::Triangulation& other,
        std::list<typename DimTraits<dim>::Isomorphism*>& results) const {
    return static_cast<const typename DimTraits<dim>::Triangulation&>(*this).
        findIsomorphisms(other, results, false, false);
}

} // namespace regina

