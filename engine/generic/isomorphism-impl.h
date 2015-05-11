
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

#include "generic/ngenerictriangulation.h"

namespace regina {

template <int dim>
inline std::auto_ptr<Isomorphism<dim>>
        NGenericTriangulation<dim>::isIsomorphicTo(
        const Triangulation<dim>& other) const {
    std::list<Isomorphism<dim>*> results;
    if (static_cast<const Triangulation<dim>&>(*this).
            findIsomorphisms(other, results, true, true))
        return std::auto_ptr<Isomorphism<dim>>(results.front());
    else
        return std::auto_ptr<Isomorphism<dim>>(0);
}

template <int dim>
inline std::auto_ptr<Isomorphism<dim>>
        NGenericTriangulation<dim>::isContainedIn(
        const Triangulation<dim>& other) const {
    std::list<Isomorphism<dim>*> results;
    if (static_cast<const Triangulation<dim>&>(*this).
            findIsomorphisms(other, results, false, true))
        return std::auto_ptr<Isomorphism<dim>>(results.front());
    else
        return std::auto_ptr<Isomorphism<dim>>(0);
}

template <int dim>
inline unsigned long NGenericTriangulation<dim>::findAllIsomorphisms(
        const Triangulation<dim>& other,
        std::list<Isomorphism<dim>*>& results) const {
    return static_cast<const Triangulation<dim>&>(*this).
        findIsomorphisms(other, results, true, false);
}

template <int dim>
inline unsigned long NGenericTriangulation<dim>::findAllSubcomplexesIn(
        const Triangulation<dim>& other,
        std::list<Isomorphism<dim>*>& results) const {
    return static_cast<const Triangulation<dim>&>(*this).
        findIsomorphisms(other, results, false, false);
}

} // namespace regina

