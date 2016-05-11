
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
#include "../helpers.h"

using namespace boost::python;
using regina::NLargeInteger;

namespace {
    NLargeInteger& (NLargeInteger::*divByExact_large)(const NLargeInteger&) =
        &NLargeInteger::divByExact;
    NLargeInteger& (NLargeInteger::*divByExact_long)(long) =
        &NLargeInteger::divByExact;
    NLargeInteger (NLargeInteger::*divExact_large)(const NLargeInteger&) const =
        &NLargeInteger::divExact;
    NLargeInteger (NLargeInteger::*divExact_long)(long) const =
        &NLargeInteger::divExact;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_stringValue,
        NLargeInteger::stringValue, 0, 1);

    boost::python::tuple divisionAlg(const NLargeInteger& n,
            const NLargeInteger& divisor) {
        NLargeInteger remainder;
        NLargeInteger quotient = n.divisionAlg(divisor, remainder);
        return boost::python::make_tuple(quotient, remainder);
    }
}

void addNLargeInteger() {
    scope s = class_<NLargeInteger>("NLargeInteger")
        .def(init<long>())
        .def(init<const NLargeInteger&>())
        .def(init<const regina::NInteger&>())
        .def(init<double>())
        .def(init<const char*, optional<int> >())
        .def("isNative", &NLargeInteger::isNative)
        .def("isZero", &NLargeInteger::isZero)
        .def("sign", &NLargeInteger::sign)
        .def("isInfinite", &NLargeInteger::isInfinite)
        .def("makeInfinite", &NLargeInteger::makeInfinite)
        .def("longValue", &NLargeInteger::longValue)
        .def("stringValue", &NLargeInteger::stringValue, OL_stringValue())
        .def("swap", &NLargeInteger::swap)
        .def(self == long())
        .def(self == regina::NInteger())
        .def(self != long())
        .def(self != regina::NInteger())
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
        .def("divExact", divExact_large)
        .def("divExact", divExact_long)
        .def(self % self)
        .def(self % long())
        .def("divisionAlg", divisionAlg)
        .def(- self)
        .def(self += self)
        .def(self += long())
        .def(self -= self)
        .def(self -= long())
        .def(self *= self)
        .def(self *= long())
        .def(self /= self)
        .def(self /= long())
        .def("divByExact", divByExact_large, return_internal_reference<>())
        .def("divByExact", divByExact_long, return_internal_reference<>())
        .def(self %= self)
        .def(self %= long())
        .def("negate", &NLargeInteger::negate)
        .def("raiseToPower", &NLargeInteger::raiseToPower)
        .def("abs", &NLargeInteger::abs)
        .def("gcdWith", &NLargeInteger::gcdWith)
        .def("gcd", &NLargeInteger::gcd)
        .def("lcmWith", &NLargeInteger::lcmWith)
        .def("lcm", &NLargeInteger::lcm)
        .def("gcdWithCoeffs", &NLargeInteger::gcdWithCoeffs)
        .def("legendre", &NLargeInteger::legendre)
        .def("randomBoundedByThis", &NLargeInteger::randomBoundedByThis)
        .def("randomBinary", &NLargeInteger::randomBinary)
        .def("randomCornerBinary", &NLargeInteger::randomCornerBinary)
        .def("makeLarge", &NLargeInteger::makeLarge)
        .def("tryReduce", &NLargeInteger::tryReduce)
        .def(long() + self)
        .def(long() * self)
        .def(self_ns::str(self))
        .def(self_ns::repr(self))
        .def(regina::python::add_eq_operators())
        .staticmethod("randomBinary")
        .staticmethod("randomCornerBinary")
    ;

    // Apparently there is no way in python to make a module attribute
    // read-only.
    s.attr("zero") = NLargeInteger::zero;
    s.attr("one") = NLargeInteger::one;
    s.attr("infinity") = NLargeInteger::infinity;

    boost::python::implicitly_convertible<long, NLargeInteger>();
    boost::python::implicitly_convertible<std::string, NLargeInteger>();
}

