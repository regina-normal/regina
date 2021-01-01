
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "../pybind11/pybind11.h"
#include "../pybind11/operators.h"
#include "maths/integer.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Integer;

void addInteger(pybind11::module_& m) {
    auto c = pybind11::class_<Integer>(m, "Integer")
        .def(pybind11::init<>())
        .def(pybind11::init<long>())
        .def(pybind11::init<const Integer&>())
        .def(pybind11::init<const regina::LargeInteger&>())
        .def(pybind11::init<double>())
        .def(pybind11::init<const char*, int>(),
            pybind11::arg(), pybind11::arg("base") = 10)
        .def(pybind11::init([](pybind11::int_ l){
            return new Integer(pybind11::cast<std::string>(pybind11::str(l)));
        }))
        .def("isNative", &Integer::isNative)
        .def("isZero", &Integer::isZero)
        .def("sign", &Integer::sign)
        .def("isInfinite", &Integer::isInfinite)
        .def("makeInfinite", &Integer::makeInfinite)
        .def("longValue", &Integer::longValue)
        .def("stringValue", &Integer::stringValue,
            pybind11::arg("base") = 10)
        .def("swap", &Integer::swap)
        .def(pybind11::self == long())
        .def(pybind11::self == regina::LargeInteger())
        .def(pybind11::self != long())
        .def(pybind11::self != regina::LargeInteger())
        .def(pybind11::self < pybind11::self)
        .def(pybind11::self < long())
        .def(pybind11::self > pybind11::self)
        .def(pybind11::self > long())
        .def(pybind11::self <= pybind11::self)
        .def(pybind11::self <= long())
        .def(pybind11::self >= pybind11::self)
        .def(pybind11::self >= long())
        .def(pybind11::self + pybind11::self)
        .def(pybind11::self + long())
        .def(pybind11::self - pybind11::self)
        .def(pybind11::self - long())
        .def(pybind11::self * pybind11::self)
        .def(pybind11::self * long())
        .def(pybind11::self / pybind11::self)
        .def(pybind11::self / long())
        .def("divExact", overload_cast<const Integer&>(
            &Integer::divExact, pybind11::const_))
        .def("divExact", overload_cast<long>(
            &Integer::divExact, pybind11::const_))
        .def(pybind11::self % pybind11::self)
        .def(pybind11::self % long())
        .def("divisionAlg", [](const Integer& n, const Integer& divisor) {
            Integer remainder;
            Integer quotient = n.divisionAlg(divisor, remainder);
            return pybind11::make_tuple(quotient, remainder);
        })
        .def(- pybind11::self)
        .def(pybind11::self += pybind11::self)
        .def(pybind11::self += long())
        .def(pybind11::self -= pybind11::self)
        .def(pybind11::self -= long())
        .def(pybind11::self *= pybind11::self)
        .def(pybind11::self *= long())
        .def(pybind11::self /= pybind11::self)
        .def(pybind11::self /= long())
        .def("divByExact", overload_cast<const Integer&>(&Integer::divByExact),
            pybind11::return_value_policy::reference_internal)
        .def("divByExact", overload_cast<long>(&Integer::divByExact),
            pybind11::return_value_policy::reference_internal)
        .def(pybind11::self %= pybind11::self)
        .def(pybind11::self %= long())
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
        .def_static("randomBinary", &Integer::randomBinary)
        .def_static("randomCornerBinary", &Integer::randomCornerBinary)
        .def("makeLarge", &Integer::makeLarge)
        .def("tryReduce", &Integer::tryReduce)
        .def(long() + pybind11::self)
        .def(long() * pybind11::self)
        .def_readonly_static("zero", &Integer::zero)
        .def_readonly_static("one", &Integer::one)
    ;
    regina::python::add_eq_operators(c);
    regina::python::add_output_ostream(c, true /* __repr__ */);

    pybind11::implicitly_convertible<long, Integer>();
    pybind11::implicitly_convertible<std::string, Integer>();

    m.attr("NInteger") = m.attr("Integer");
}

