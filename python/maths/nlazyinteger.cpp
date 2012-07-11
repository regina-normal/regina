
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
#include "maths/nlazyinteger.h"

using namespace boost::python;
using regina::NLazyInteger;

namespace {
    NLazyInteger& (NLazyInteger::*divByExact_lazy)(const NLazyInteger&) =
        &NLazyInteger::divByExact;
    NLazyInteger& (NLazyInteger::*divByExact_long)(long) =
        &NLazyInteger::divByExact;
    NLazyInteger (NLazyInteger::*divExact_lazy)(const NLazyInteger&) const =
        &NLazyInteger::divExact;
    NLazyInteger (NLazyInteger::*divExact_long)(long) const =
        &NLazyInteger::divExact;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_stringValue,
        NLazyInteger::stringValue, 0, 1);
}

void addNLazyInteger() {
    scope s = class_<NLazyInteger>("NLazyInteger")
        .def(init<long>())
        .def(init<const NLazyInteger&>())
        .def(init<const char*, optional<int> >())
        .def("isNative", &NLazyInteger::isNative)
        .def("longValue", &NLazyInteger::longValue)
        .def("stringValue", &NLazyInteger::stringValue, OL_stringValue())
        .def("swap", &NLazyInteger::swap)
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
        .def("negate", &NLazyInteger::negate)
        .def("abs", &NLazyInteger::abs)
        .def("gcd", &NLazyInteger::gcd)
        .def("lcm", &NLazyInteger::lcm)
        .def("makeLarge", &NLazyInteger::makeLarge)
        .def("tryReduce", &NLazyInteger::tryReduce)
        .def(self_ns::str(self))
    ;

    // Apparently there is no way in python to make a module attribute
    // read-only.
    s.attr("zero") = NLazyInteger::zero;
    s.attr("one") = NLazyInteger::one;

    boost::python::implicitly_convertible<long, NLazyInteger>();
    boost::python::implicitly_convertible<std::string, NLazyInteger>();
}

