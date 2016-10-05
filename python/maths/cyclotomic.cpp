
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
#include "maths/cyclotomic.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Cyclotomic;

namespace {
    const regina::Rational& getItem(const Cyclotomic& c, int exp) {
        return c[exp];
    }
    void setItem(Cyclotomic& c, int exp, const regina::Rational& value) {
        c[exp] = value;
    }
    regina::Polynomial<regina::Rational>* cyclotomic(size_t n) {
        return new regina::Polynomial<regina::Rational>(
            Cyclotomic::cyclotomic(n));
    }

    std::string (Cyclotomic::*str_variable)(const char*) const =
        &Cyclotomic::str;
    std::string (Cyclotomic::*utf8_variable)(const char*) const =
        &Cyclotomic::utf8;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_evaluate,
        Cyclotomic::evaluate, 0, 1);
}

void addCyclotomic() {
    class_<Cyclotomic, std::auto_ptr<Cyclotomic> >("Cyclotomic")
        .def(init<size_t>())
        .def(init<size_t, int>())
        .def(init<size_t, const regina::Rational&>())
        .def(init<const Cyclotomic&>())
        .def("init", &Cyclotomic::init)
        .def("field", &Cyclotomic::field)
        .def("degree", &Cyclotomic::degree)
        .def("__getitem__", getItem, return_internal_reference<>())
        .def("__setitem__", setItem)
        .def("polynomial", &Cyclotomic::polynomial,
            return_value_policy<manage_new_object>())
        .def("evaluate", &Cyclotomic::evaluate, OL_evaluate())
        .def("negate", &Cyclotomic::invert)
        .def("invert", &Cyclotomic::invert)
        .def(self *= regina::Rational())
        .def(self /= regina::Rational())
        .def(self += self)
        .def(self -= self)
        .def(self *= self)
        .def(self /= self)
        .def("cyclotomic", &cyclotomic,
            return_value_policy<manage_new_object>())
        .def("str", str_variable)
        .def("utf8", utf8_variable)
        .def(regina::python::add_output())
        .def(self_ns::repr(self)) // add_output only gives __str__
        .def(regina::python::add_eq_operators())
        .staticmethod("cyclotomic")
    ;

    scope().attr("NCyclotomic") = scope().attr("Cyclotomic");
}

