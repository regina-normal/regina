
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Computational Engine                                                  *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/*! \file nenumconstraint.h
 *  \brief Deals with validity constraints in polytope vertex enumeration.
 */

#ifndef __NENUMCONSTRAINT_H
#ifndef __DOXYGEN
#define __NENUMCONSTRAINT_H
#endif

#include <set>
#include <vector>

namespace regina {

/**
 * \weakgroup enumerate
 * @{
 */

/**
 * Represents an individual validity constraint for use with
 * polytope vertex enumeration.
 *
 * Vertex enumeration routines such as
 * NDoubleDescriptor::enumerateExtremalRays() take a cone, described by
 * its extremal rays and facets, form the intersection of that original
 * cone with a given linear subspace, and return the extremal rays of the
 * new cone that results.
 *
 * In some cases we are only interested in \e valid rays of the new cone.
 * The NEnumConstraintList class stores a number of "validity constraints";
 * a ray is then "valid" if it satisfies all of these constraints.
 *
 * Each individual constraint is presented as a set of integers; the
 * meaning of such a constraint is as follows.  We number the facets of
 * the original cone 0,1,2,... (in the order in which they were passed
 * to the vertex enumeration routine).  If a constraint is described by
 * the integers \a x, \a y, \a z, ..., then it indicates that a ray can only
 * lie outside at most one of the facets numbered \a x, \a y, \a z, ... .
 *
 * In practice, this allows us to represent constraints in normal
 * surface theory.  For instance, to insist that some tetrahedron
 * contains at most one quadrilateral disc type, we add a constraint
 * with three integers, representing the original facets
 * \a q1=0, \a q2=0, \a q3=0 (where \a q1, \a q2 and \a q3 are the three
 * quadrilateral coordinates for that tetrahedron).
 *
 * The NEnumConstraintList class is simply a std::vector of constraints,
 * where each constraint is a std::set of unsigned integers.  Typically
 * one will create a vector containing the desired number of constraints
 * and then walk through each constraint, filling the sets as appropriate.
 *
 * \ifacespython Not present.
 */
class NEnumConstraintList : public std::vector<std::set<unsigned> > {
    public:
        /**
         * Creates an empty list of constraints.
         */
        NEnumConstraintList();

        /**
         * Creates a new list of constraints with the given size.
         * Each constraint will be initialised to an empty set.
         *
         * @param size the number of constraints to include in the new list.
         */
        NEnumConstraintList(unsigned size);
};

/*@}*/

// Inline functions for NEnumConstraintList

inline NEnumConstraintList::NEnumConstraintList() {
}

inline NEnumConstraintList::NEnumConstraintList(unsigned size) :
        std::vector<std::set<unsigned> >(size) {
}

} // namespace regina

#endif

