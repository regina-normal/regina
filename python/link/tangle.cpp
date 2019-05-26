
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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

#include "link/tangle.h"
#include "../safeheldtype.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>
#include "../helpers.h"

using namespace boost::python;
using regina::Crossing;
using regina::StrandRef;
using regina::Tangle;

namespace {
    bool (Tangle::*r1a)(Crossing*, bool, bool) = &Tangle::r1;
    bool (Tangle::*r2a)(Crossing*, bool, bool) = &Tangle::r2;
    bool (Tangle::*r2b)(StrandRef, bool, bool) = &Tangle::r2;

    std::string (Tangle::*orientedGauss_str)() const = &Tangle::orientedGauss;
    Tangle* (*fromOrientedGauss_str)(const std::string&) =
        &Tangle::fromOrientedGauss;

    Tangle* fromOrientedGauss_list(boost::python::list terms) {
        long len = boost::python::len(terms);

        std::string* s = new std::string[len];
        for (long i = 0; i < len; ++i) {
            extract<std::string> val(terms[i]);
            if (! val.check()) {
                // Throw an exception.
                delete[] s;
                val();
            }
            s[i] = val();
        }

        Tangle* ans = Tangle::fromOrientedGauss(s, s + len);
        delete[] s;
        return ans;
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_twist, Tangle::twist, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_turn, Tangle::turn, 0, 1);

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_r1a, Tangle::r1, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_r2a, Tangle::r2, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_r2b, Tangle::r2, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplifyToLocalMinimum,
        Tangle::simplifyToLocalMinimum, 0, 1);
}

void addTangle() {
    class_<Tangle, std::auto_ptr<Tangle>, boost::noncopyable>("Tangle")
        .def(init<int>())
        .def(init<int, int>())
        .def(init<const regina::Link&>())
        .def(init<const Tangle&>())
        .def("type", &Tangle::type)
        .def("size", &Tangle::size)
        .def("crossing", &Tangle::crossing, return_internal_reference<>())
        .def("begin", &Tangle::begin)
        .def("end", &Tangle::end)
        .def("translate", &Tangle::translate)
        .def("swapContents", &Tangle::swapContents)
        .def("twist", &Tangle::twist, OL_twist())
        .def("turn", &Tangle::turn, OL_turn())
        .def("changeAll", &Tangle::changeAll)
        .def("add", &Tangle::add)
        .def("box", &Tangle::box)
        .def("negate", &Tangle::negate)
        .def("numClosure", &Tangle::numClosure,
            return_value_policy<regina::python::to_held_type<>>())
        .def("denClosure", &Tangle::denClosure,
            return_value_policy<regina::python::to_held_type<>>())
        .def("r1", r1a, OL_r1a())
        .def("r2", r2a, OL_r2a())
        .def("r2", r2b, OL_r2b())
        .def("simplifyToLocalMinimum", &Tangle::simplifyToLocalMinimum,
             OL_simplifyToLocalMinimum())
        .def("orientedGauss", orientedGauss_str)
        .def("fromOrientedGauss", fromOrientedGauss_list,
            return_value_policy<manage_new_object>())
        .def("fromOrientedGauss", fromOrientedGauss_str,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("fromOrientedGauss")
    ;
}
