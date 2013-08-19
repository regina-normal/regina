
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "maths/ninteger.h"

using namespace boost::python;
using regina::NInteger;

namespace {
    NInteger& (NInteger::*divByExact_lazy)(const NInteger&) =
        &NInteger::divByExact;
    NInteger& (NInteger::*divByExact_long)(long) =
        &NInteger::divByExact;
    NInteger (NInteger::*divExact_lazy)(const NInteger&)
        const = &NInteger::divExact;
    NInteger (NInteger::*divExact_long)(long) const =
        &NInteger::divExact;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_stringValue,
        NInteger::stringValue, 0, 1);
}

void addNInteger() {
    scope s = class_<NInteger>("NInteger")
        .def(init<long>())
        .def(init<const NInteger&>())
        .def(init<const char*, optional<int> >())
        .def("isNative", &NInteger::isNative)
        .def("longValue", &NInteger::longValue)
        .def("stringValue", &NInteger::stringValue, OL_stringValue())
        .def("swap", &NInteger::swap)
        .def(self == self)
        .def(self == long())
        .def(self != self)
        .def(self != long())
        .def(self < self)
        .def(self < long())
        .def(self > self)
        .def(self > long())
        .def(self <= self)
        .def(self <= long())
        .def(self >= self)
        .def(self >= long())
        .def(self + self)
        .def(self + long())
        .def(self - self)
        .def(self - long())
        .def(self * self)
        .def(self * long())
        .def(self / self)
        .def(self / long())
        .def("divExact", divExact_lazy)
        .def("divExact", divExact_long)
        .def(self % self)
        .def(self % long())
        .def(- self)
        .def(self += self)
        .def(self += long())
        .def(self -= self)
        .def(self -= long())
        .def(self *= self)
        .def(self *= long())
        .def(self /= self)
        .def(self /= long())
        .def("divByExact", divByExact_lazy, return_internal_reference<>())
        .def("divByExact", divByExact_long, return_internal_reference<>())
        .def(self %= self)
        .def(self %= long())
        .def("negate", &NInteger::negate)
        .def("raiseToPower", &NInteger::raiseToPower)
        .def("abs", &NInteger::abs)
        .def("gcd", &NInteger::gcd)
        .def("lcm", &NInteger::lcm)
        .def("makeLarge", &NInteger::makeLarge)
        .def("tryReduce", &NInteger::tryReduce)
        .def(self_ns::str(self))
    ;

    // Apparently there is no way in python to make a module attribute
    // read-only.
    s.attr("zero") = NInteger::zero;
    s.attr("one") = NInteger::one;

    boost::python::implicitly_convertible<long, NInteger>();
    boost::python::implicitly_convertible<std::string, NInteger>();
}

