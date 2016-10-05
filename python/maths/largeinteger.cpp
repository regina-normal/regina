
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
#include "maths/integer.h"
#include "../helpers.h"

using namespace boost::python;
using regina::LargeInteger;

namespace {
    LargeInteger& (LargeInteger::*divByExact_large)(const LargeInteger&) =
        &LargeInteger::divByExact;
    LargeInteger& (LargeInteger::*divByExact_long)(long) =
        &LargeInteger::divByExact;
    LargeInteger (LargeInteger::*divExact_large)(const LargeInteger&) const =
        &LargeInteger::divExact;
    LargeInteger (LargeInteger::*divExact_long)(long) const =
        &LargeInteger::divExact;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_stringValue,
        LargeInteger::stringValue, 0, 1);

    boost::python::tuple divisionAlg(const LargeInteger& n,
            const LargeInteger& divisor) {
        LargeInteger remainder;
        LargeInteger quotient = n.divisionAlg(divisor, remainder);
        return boost::python::make_tuple(quotient, remainder);
    }
}

void addLargeInteger() {
    {
        scope s = class_<LargeInteger>("LargeInteger")
            .def(init<long>())
            .def(init<const LargeInteger&>())
            .def(init<const regina::Integer&>())
            .def(init<double>())
            .def(init<const char*, optional<int> >())
            .def("isNative", &LargeInteger::isNative)
            .def("isZero", &LargeInteger::isZero)
            .def("sign", &LargeInteger::sign)
            .def("isInfinite", &LargeInteger::isInfinite)
            .def("makeInfinite", &LargeInteger::makeInfinite)
            .def("longValue", &LargeInteger::longValue)
            .def("stringValue", &LargeInteger::stringValue, OL_stringValue())
            .def("swap", &LargeInteger::swap)
            .def(self == long())
            .def(self == regina::Integer())
            .def(self != long())
            .def(self != regina::Integer())
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
            .def("negate", &LargeInteger::negate)
            .def("raiseToPower", &LargeInteger::raiseToPower)
            .def("abs", &LargeInteger::abs)
            .def("gcdWith", &LargeInteger::gcdWith)
            .def("gcd", &LargeInteger::gcd)
            .def("lcmWith", &LargeInteger::lcmWith)
            .def("lcm", &LargeInteger::lcm)
            .def("gcdWithCoeffs", &LargeInteger::gcdWithCoeffs)
            .def("legendre", &LargeInteger::legendre)
            .def("randomBoundedByThis", &LargeInteger::randomBoundedByThis)
            .def("randomBinary", &LargeInteger::randomBinary)
            .def("randomCornerBinary", &LargeInteger::randomCornerBinary)
            .def("makeLarge", &LargeInteger::makeLarge)
            .def("tryReduce", &LargeInteger::tryReduce)
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
        s.attr("zero") = LargeInteger::zero;
        s.attr("one") = LargeInteger::one;
        s.attr("infinity") = LargeInteger::infinity;
    }

    boost::python::implicitly_convertible<long, LargeInteger>();
    boost::python::implicitly_convertible<std::string, LargeInteger>();

    scope().attr("NLargeInteger") = scope().attr("LargeInteger");
}

