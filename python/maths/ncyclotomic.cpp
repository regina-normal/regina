
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
#include "maths/ncyclotomic.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NCyclotomic;

namespace {
    const regina::NRational& getItem(const NCyclotomic& c, int exp) {
        return c[exp];
    }
    void setItem(NCyclotomic& c, int exp, const regina::NRational& value) {
        c[exp] = value;
    }
    regina::NPolynomial<regina::NRational>* cyclotomic(size_t n) {
        return new regina::NPolynomial<regina::NRational>(
            NCyclotomic::cyclotomic(n));
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_evaluate,
        NCyclotomic::evaluate, 0, 1);
}

void addNCyclotomic() {
    scope s = class_<NCyclotomic, std::auto_ptr<NCyclotomic> >("NCyclotomic")
        .def(init<size_t>())
        .def(init<size_t, int>())
        .def(init<size_t, const regina::NRational&>())
        .def(init<const NCyclotomic&>())
        .def("init", &NCyclotomic::init)
        .def("field", &NCyclotomic::field)
        .def("degree", &NCyclotomic::degree)
        .def("__getitem__", getItem, return_internal_reference<>())
        .def("__setitem__", setItem)
        .def("polynomial", &NCyclotomic::polynomial,
            return_value_policy<manage_new_object>())
        .def("evaluate", &NCyclotomic::evaluate, OL_evaluate())
        .def("negate", &NCyclotomic::invert)
        .def("invert", &NCyclotomic::invert)
        .def(self *= regina::NRational())
        .def(self /= regina::NRational())
        .def(self += self)
        .def(self -= self)
        .def(self *= self)
        .def(self /= self)
        .def("cyclotomic", &cyclotomic,
            return_value_policy<manage_new_object>())
        .def(self_ns::str(self))
        .def(self_ns::repr(self))
        .def(regina::python::add_eq_operators())
        .staticmethod("cyclotomic")
    ;
}

