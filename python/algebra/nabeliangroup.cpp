
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "algebra/nabeliangroup.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NAbelianGroup;
using regina::NLargeInteger;

void (NAbelianGroup::*addTorsionElement_large)(const NLargeInteger&,
    unsigned) = &NAbelianGroup::addTorsionElement;
void (NAbelianGroup::*addTorsionElement_long)(unsigned long,
    unsigned) = &NAbelianGroup::addTorsionElement;

namespace {
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addRank,
        NAbelianGroup::addRank, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addTorsionElement,
        NAbelianGroup::addTorsionElement, 1, 2);
}

void addNAbelianGroup() {
    class_<NAbelianGroup, bases<regina::ShareableObject> >("NAbelianGroup")
        .def(init<const NAbelianGroup&>())
        .def("addRank", &NAbelianGroup::addRank, OL_addRank())
        .def("getRank", &NAbelianGroup::getRank)
        .def("addTorsionElement", addTorsionElement_large,
            OL_addTorsionElement())
        .def("addTorsionElement", addTorsionElement_long,
            OL_addTorsionElement())
        .def("getNumberOfInvariantFactors",
            &NAbelianGroup::getNumberOfInvariantFactors)
        .def("getInvariantFactor", &NAbelianGroup::getInvariantFactor,
            return_value_policy<return_by_value>())
    ;
}

