
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

typedef regina::NInteger<false> NFiniteInteger;

namespace {
    NFiniteInteger& (NFiniteInteger::*divByExact_lazy)(const NFiniteInteger&) =
        &NFiniteInteger::divByExact;
    NFiniteInteger& (NFiniteInteger::*divByExact_long)(long) =
        &NFiniteInteger::divByExact;
    NFiniteInteger (NFiniteInteger::*divExact_lazy)(const NFiniteInteger&)
        const = &NFiniteInteger::divExact;
    NFiniteInteger (NFiniteInteger::*divExact_long)(long) const =
        &NFiniteInteger::divExact;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_stringValue,
        NFiniteInteger::stringValue, 0, 1);
}

void addNInteger() {
    scope s = class_<NFiniteInteger>("NInteger")
        .def(init<long>())
        .def(init<const NFiniteInteger&>())
        .def(init<const char*, optional<int> >())
        .def("isNative", &NFiniteInteger::isNative)
        .def("longValue", &NFiniteInteger::longValue)
        .def("stringValue", &NFiniteInteger::stringValue, OL_stringValue())
        .def("swap", &NFiniteInteger::swap)
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
        .def("negate", &NFiniteInteger::negate)
        .def("raiseToPower", &NFiniteInteger::raiseToPower)
        .def("abs", &NFiniteInteger::abs)
        .def("gcd", &NFiniteInteger::gcd)
        .def("lcm", &NFiniteInteger::lcm)
        .def("makeLarge", &NFiniteInteger::makeLarge)
        .def("tryReduce", &NFiniteInteger::tryReduce)
        .def(self_ns::str(self))
    ;

    // Apparently there is no way in python to make a module attribute
    // read-only.
    s.attr("zero") = NFiniteInteger::zero;
    s.attr("one") = NFiniteInteger::one;

    boost::python::implicitly_convertible<long, NFiniteInteger>();
    boost::python::implicitly_convertible<std::string, NFiniteInteger>();
}

