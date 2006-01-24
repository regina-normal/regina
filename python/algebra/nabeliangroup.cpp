
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include "algebra/nabeliangroup.h"
#include "maths/nmatrixint.h"
#include <boost/python.hpp>
#include <boost/python/detail/api_placeholder.hpp> // For len().

using namespace boost::python;
using regina::NAbelianGroup;

namespace {
    void (NAbelianGroup::*addTorsionElement_large)(
        const regina::NLargeInteger&, unsigned) =
        &NAbelianGroup::addTorsionElement;
    void (NAbelianGroup::*addTorsionElement_long)(unsigned long,
        unsigned) = &NAbelianGroup::addTorsionElement;
    void (NAbelianGroup::*addGroup_matrix)(const regina::NMatrixInt&) =
        &NAbelianGroup::addGroup;
    void (NAbelianGroup::*addGroup_group)(const NAbelianGroup&) =
        &NAbelianGroup::addGroup;
    unsigned (NAbelianGroup::*getTorsionRank_large)(
        const regina::NLargeInteger&) const = &NAbelianGroup::getTorsionRank;
    unsigned (NAbelianGroup::*getTorsionRank_long)(unsigned long)
        const = &NAbelianGroup::getTorsionRank;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addRank,
        NAbelianGroup::addRank, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addTorsionElement,
        NAbelianGroup::addTorsionElement, 1, 2);

    void addTorsionElements_dict(NAbelianGroup& g,
            boost::python::list elements) {
        std::multiset<regina::NLargeInteger> set;

        long len = boost::python::len(elements);
        for (long i = 0; i < len; i++) {
            // Accept any type that we know how to convert to a large
            // integer.
            extract<regina::NLargeInteger&> x_large(elements[i]);
            if (x_large.check()) {
                set.insert(x_large());
                continue;
            }
            extract<long> x_long(elements[i]);
            if (x_long.check()) {
                set.insert(x_long());
                continue;
            }
            extract<const char*> x_str(elements[i]);
            if (x_str.check()) {
                set.insert(x_str());
                continue;
            }

            // Throw an exception.
            x_large();
        }

        g.addTorsionElements(set);
    }
}

void addNAbelianGroup() {
    class_<NAbelianGroup, bases<regina::ShareableObject>,
            std::auto_ptr<NAbelianGroup>, boost::noncopyable>("NAbelianGroup")
        .def(init<const NAbelianGroup&>())
        .def("addRank", &NAbelianGroup::addRank, OL_addRank())
        .def("addTorsionElement", addTorsionElement_large,
            OL_addTorsionElement())
        .def("addTorsionElement", addTorsionElement_long,
            OL_addTorsionElement())
        .def("addTorsionElements", addTorsionElements_dict)
        .def("addGroup", addGroup_matrix)
        .def("addGroup", addGroup_group)
        .def("getRank", &NAbelianGroup::getRank)
        .def("getTorsionRank", getTorsionRank_large)
        .def("getTorsionRank", getTorsionRank_long)
        .def("getNumberOfInvariantFactors",
            &NAbelianGroup::getNumberOfInvariantFactors)
        .def("getInvariantFactor", &NAbelianGroup::getInvariantFactor,
            return_value_policy<return_by_value>())
        .def("isTrivial", &NAbelianGroup::isTrivial)
        .def(self == self)
    ;
}

