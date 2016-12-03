
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "algebra/abeliangroup.h"
#include "maths/matrix.h"
#include "../helpers.h"
#include <boost/python/detail/api_placeholder.hpp> // For len().

using namespace boost::python;
using regina::AbelianGroup;
using regina::Integer;
using regina::MatrixInt;

namespace {
    void (AbelianGroup::*addTorsionElement_large)(
        const regina::Integer&, unsigned) =
        &AbelianGroup::addTorsionElement;
    void (AbelianGroup::*addTorsionElement_long)(unsigned long,
        unsigned) = &AbelianGroup::addTorsionElement;
    void (AbelianGroup::*addGroup_matrix)(const MatrixInt&) =
        &AbelianGroup::addGroup;
    void (AbelianGroup::*addGroup_group)(const AbelianGroup&) =
        &AbelianGroup::addGroup;
    unsigned (AbelianGroup::*torsionRank_large)(
        const regina::Integer&) const = &AbelianGroup::torsionRank;
    unsigned (AbelianGroup::*torsionRank_long)(unsigned long)
        const = &AbelianGroup::torsionRank;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addRank,
        AbelianGroup::addRank, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_addTorsionElement,
        AbelianGroup::addTorsionElement, 1, 2);

    void addTorsionElements_dict(AbelianGroup& g,
            boost::python::list elements) {
        std::multiset<regina::Integer> set;

        long len = boost::python::len(elements);
        for (long i = 0; i < len; i++) {
            // Accept any type that we know how to convert to a large
            // integer.
            extract<regina::Integer&> x_large(elements[i]);
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

void addAbelianGroup() {
    class_<AbelianGroup, std::auto_ptr<AbelianGroup>, boost::noncopyable>
            ("AbelianGroup")
        .def(init<const AbelianGroup&>())
        .def(init<const MatrixInt&, const MatrixInt&>())
        .def(init<const MatrixInt&, const MatrixInt&, const Integer&>())
        .def("addRank", &AbelianGroup::addRank, OL_addRank())
        .def("addTorsionElement", addTorsionElement_large,
            OL_addTorsionElement())
        .def("addTorsionElement", addTorsionElement_long,
            OL_addTorsionElement())
        .def("addTorsionElements", addTorsionElements_dict)
        .def("addGroup", addGroup_matrix)
        .def("addGroup", addGroup_group)
        .def("rank", &AbelianGroup::rank)
        .def("torsionRank", torsionRank_large)
        .def("torsionRank", torsionRank_long)
        .def("countInvariantFactors", &AbelianGroup::countInvariantFactors)
        .def("invariantFactor", &AbelianGroup::invariantFactor,
            return_value_policy<return_by_value>())
        .def("isTrivial", &AbelianGroup::isTrivial)
        .def("isZ", &AbelianGroup::isZ)
        .def("isZn", &AbelianGroup::isZn)
        .def("utf8", &AbelianGroup::utf8)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NAbelianGroup") = scope().attr("AbelianGroup");
}

