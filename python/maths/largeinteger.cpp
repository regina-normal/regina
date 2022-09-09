
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "../pybind11/stl.h"
#include "maths/integer.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::LargeInteger;

void addLargeInteger(pybind11::module_& m) {
    auto c = pybind11::class_<LargeInteger>(m, "LargeInteger")
        .def(pybind11::init<>())
        .def(pybind11::init<long>())
        .def(pybind11::init<const LargeInteger&>())
        .def(pybind11::init<const regina::Integer&>())
        .def(pybind11::init<double>())
        .def(pybind11::init<const char*, int>(),
            pybind11::arg(), pybind11::arg("base") = 10)
        .def(pybind11::init([](pybind11::int_ l){
            return new LargeInteger(pybind11::cast<std::string>(
                pybind11::str(l)));
        }))
        .def("isNative", &LargeInteger::isNative)
        .def("isZero", &LargeInteger::isZero)
        .def("sign", &LargeInteger::sign)
        .def("isInfinite", &LargeInteger::isInfinite)
        .def("makeInfinite", &LargeInteger::makeInfinite)
        .def("longValue", &LargeInteger::longValue)
        .def("safeLongValue", &LargeInteger::safeLongValue)
        .def("stringValue", &LargeInteger::stringValue,
            pybind11::arg("base") = 10)
        .def("swap", &LargeInteger::swap)
        .def(pybind11::self == long())
        .def(pybind11::self == regina::Integer())
        .def(pybind11::self != long())
        .def(pybind11::self != regina::Integer())
        .def(pybind11::self < pybind11::self)
        .def(pybind11::self < long())
        .def(pybind11::self > pybind11::self)
        .def(pybind11::self > long())
        .def(pybind11::self <= pybind11::self)
        .def(pybind11::self <= long())
        .def(pybind11::self >= pybind11::self)
        .def(pybind11::self >= long())
        .def("inc", [](LargeInteger& i) {
            return i++;
        })
        .def("dec", [](LargeInteger& i) {
            return i--;
        })
        .def(pybind11::self + pybind11::self)
        .def(pybind11::self + long())
        .def(pybind11::self - pybind11::self)
        .def(pybind11::self - long())
        .def(pybind11::self * pybind11::self)
        .def(pybind11::self * long())
        .def(pybind11::self / pybind11::self)
        .def(pybind11::self / long())
        .def("divExact", overload_cast<const LargeInteger&>(
            &LargeInteger::divExact, pybind11::const_))
        .def("divExact", overload_cast<long>(
            &LargeInteger::divExact, pybind11::const_))
        .def(pybind11::self % pybind11::self)
        .def(pybind11::self % long())
        .def("divisionAlg", overload_cast<const LargeInteger&>(
            &LargeInteger::divisionAlg, pybind11::const_))
        .def(- pybind11::self)
        .def(pybind11::self += pybind11::self)
        .def(pybind11::self += long())
        .def(pybind11::self -= pybind11::self)
        .def(pybind11::self -= long())
        .def(pybind11::self *= pybind11::self)
        .def(pybind11::self *= long())
        .def(pybind11::self /= pybind11::self)
        .def(pybind11::self /= long())
        .def("divByExact", overload_cast<const LargeInteger&>(
            &LargeInteger::divByExact),
            pybind11::return_value_policy::reference_internal)
        .def("divByExact", overload_cast<long>(
            &LargeInteger::divByExact),
            pybind11::return_value_policy::reference_internal)
        .def(pybind11::self %= pybind11::self)
        .def(pybind11::self %= long())
        .def("negate", &LargeInteger::negate)
        .def("raiseToPower", &LargeInteger::raiseToPower)
        .def("abs", &LargeInteger::abs)
        .def("gcdWith", &LargeInteger::gcdWith)
        .def("gcd", &LargeInteger::gcd)
        .def("lcmWith", &LargeInteger::lcmWith)
        .def("lcm", &LargeInteger::lcm)
        .def("gcdWithCoeffs", overload_cast<const LargeInteger&>(
            &LargeInteger::gcdWithCoeffs, pybind11::const_))
        .def("gcdWithCoeffs",
            overload_cast<const LargeInteger&, LargeInteger&, LargeInteger&>(
                &LargeInteger::gcdWithCoeffs, pybind11::const_))
        .def("legendre", &LargeInteger::legendre)
        .def("randomBoundedByThis", &LargeInteger::randomBoundedByThis)
        .def_static("randomBinary", &LargeInteger::randomBinary)
        .def_static("randomCornerBinary", &LargeInteger::randomCornerBinary)
        .def("makeLarge", &LargeInteger::makeLarge)
        .def("tryReduce", &LargeInteger::tryReduce)
        .def(long() + pybind11::self)
        .def(long() * pybind11::self)
        .def_readonly_static("zero", &LargeInteger::zero)
        .def_readonly_static("one", &LargeInteger::one)
        .def_readonly_static("infinity", &LargeInteger::infinity)
    ;
    regina::python::add_tight_encoding(c);
    regina::python::add_eq_operators(c);
    regina::python::add_output_ostream(c, regina::python::PYTHON_REPR_SLIM);

    m.def("swap", (void(*)(LargeInteger&, LargeInteger&))(regina::swap));
    m.def("tightEncoding",
        (std::string(*)(LargeInteger))(regina::tightEncoding));

    pybind11::implicitly_convertible<long, LargeInteger>();
    pybind11::implicitly_convertible<std::string, LargeInteger>();
}

