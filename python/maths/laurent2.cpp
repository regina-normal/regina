
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
#include "maths/ninteger.h"
#include "maths/laurent2.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Laurent2;
using regina::NInteger;

namespace {
    const regina::NInteger& getItem(const Laurent2<NInteger>& p,
            boost::python::tuple exponents) {
        if (boost::python::len(exponents) != 2) {
            PyErr_SetString(PyExc_ValueError,
                "Argument to [...] must be a pair of exponents.");
            boost::python::throw_error_already_set();
        }

        extract<long> x(exponents[0]);
        extract<long> y(exponents[1]);
        return p(x(), y());
    }

    void setItem(Laurent2<NInteger>& p, boost::python::tuple exponents,
            const regina::NInteger& value) {
        if (boost::python::len(exponents) != 2) {
            PyErr_SetString(PyExc_ValueError,
                "Argument to [...] must be a pair of exponents.");
            boost::python::throw_error_already_set();
        }

        extract<long> x(exponents[0]);
        extract<long> y(exponents[1]);
        p.set(x(), y(), value);
    }

    void (Laurent2<NInteger>::*init_void)() =
        &Laurent2<NInteger>::init;
    void (Laurent2<NInteger>::*init_degrees)(long, long) =
        &Laurent2<NInteger>::init;
    std::string (Laurent2<NInteger>::*str_variables)(const char*, const char*)
        const = &Laurent2<NInteger>::str;
    std::string (Laurent2<NInteger>::*utf8_variables)(const char*, const char*)
        const = &Laurent2<NInteger>::utf8;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_str_variables,
        Laurent2<NInteger>::str, 1, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_utf8_variables,
        Laurent2<NInteger>::utf8, 1, 2);
}

void addLaurent2() {
    scope s = class_<Laurent2<NInteger>,
            std::auto_ptr<Laurent2<NInteger> >,
            boost::noncopyable>("Laurent2")
        .def(init<long, long>())
        .def(init<const Laurent2<NInteger>&>())
        .def("init", init_void)
        .def("init", init_degrees)
        .def("isZero", &Laurent2<NInteger>::isZero)
        .def("set", &Laurent2<NInteger>::set)
        .def("swap", &Laurent2<NInteger>::swap)
        .def("negate", &Laurent2<NInteger>::negate)
        .def("str", str_variables, OL_str_variables())
        .def("utf8", utf8_variables, OL_utf8_variables())
        .def("__getitem__", getItem, return_internal_reference<>())
        .def("__setitem__", setItem)
        .def(self *= NInteger())
        .def(self /= NInteger())
        .def(self += self)
        .def(self -= self)
        .def(self *= self)
        .def(regina::python::add_output())
        .def(self_ns::repr(self)) // add_output only gives __str__
        .def(regina::python::add_eq_operators())
    ;
}

