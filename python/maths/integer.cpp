
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
using regina::Integer;

namespace {
    Integer& (Integer::*divByExact_large)(const Integer&) =
        &Integer::divByExact;
    Integer& (Integer::*divByExact_long)(long) =
        &Integer::divByExact;
    Integer (Integer::*divExact_large)(const Integer&) const =
        &Integer::divExact;
    Integer (Integer::*divExact_long)(long) const =
        &Integer::divExact;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_stringValue,
        Integer::stringValue, 0, 1);

    boost::python::tuple divisionAlg(const Integer& n,
            const Integer& divisor) {
        Integer remainder;
        Integer quotient = n.divisionAlg(divisor, remainder);
        return boost::python::make_tuple(quotient, remainder);
    }
}

void addInteger() {
    {
        scope s = class_<Integer>("Integer")
            .def(init<long>())
            .def(init<const Integer&>())
            .def(init<const regina::LargeInteger&>())
            .def(init<double>())
            .def(init<const char*, optional<int> >())
            .def("isNative", &Integer::isNative)
            .def("isZero", &Integer::isZero)
            .def("sign", &Integer::sign)
            .def("isInfinite", &Integer::isInfinite)
            .def("makeInfinite", &Integer::makeInfinite)
            .def("longValue", &Integer::longValue)
            .def("stringValue", &Integer::stringValue, OL_stringValue())
            .def("swap", &Integer::swap)
            .def(self == long())
            .def(self == regina::LargeInteger())
            .def(self != long())
            .def(self != regina::LargeInteger())
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
            .def("negate", &Integer::negate)
            .def("raiseToPower", &Integer::raiseToPower)
            .def("abs", &Integer::abs)
            .def("gcdWith", &Integer::gcdWith)
            .def("gcd", &Integer::gcd)
            .def("lcmWith", &Integer::lcmWith)
            .def("lcm", &Integer::lcm)
            .def("gcdWithCoeffs", &Integer::gcdWithCoeffs)
            .def("legendre", &Integer::legendre)
            .def("randomBoundedByThis", &Integer::randomBoundedByThis)
            .def("randomBinary", &Integer::randomBinary)
            .def("randomCornerBinary", &Integer::randomCornerBinary)
            .def("makeLarge", &Integer::makeLarge)
            .def("tryReduce", &Integer::tryReduce)
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
        s.attr("zero") = Integer::zero;
        s.attr("one") = Integer::one;
    }

    boost::python::implicitly_convertible<long, Integer>();
    boost::python::implicitly_convertible<std::string, Integer>();

    scope().attr("NInteger") = scope().attr("Integer");
}

