
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
#include "../docstrings/maths/integer.h"

using pybind11::overload_cast;
using regina::LargeInteger;

void addLargeInteger(pybind11::module_& m) {
    namespace global = regina::python::doc;

    RDOC_SCOPE_BEGIN(IntegerBase)

    auto c = pybind11::class_<LargeInteger>(m, "LargeInteger", rdoc_scope)
        .def(pybind11::init<>(), rdoc::IntegerBase)
        .def(pybind11::init<long>(), rdoc::IntegerBase_2)
        .def(pybind11::init<const LargeInteger&>(), rdoc::IntegerBase_3)
        .def(pybind11::init<const regina::Integer&>(), rdoc::IntegerBase_4)
        .def(pybind11::init([](pybind11::int_ l){
            return new LargeInteger(pybind11::cast<std::string>(
                pybind11::str(l)));
        }), rdoc::IntegerBase_6)
        .def(pybind11::init<double>(), rdoc::IntegerBase_7)
        .def(pybind11::init<const char*, int>(),
            pybind11::arg(), pybind11::arg("base") = 10,
            rdoc::IntegerBase_8)
        .def("isNative", &LargeInteger::isNative, rdoc::isNative)
        .def("isZero", &LargeInteger::isZero, rdoc::isZero)
        .def("sign", &LargeInteger::sign, rdoc::sign)
        .def("isInfinite", &LargeInteger::isInfinite, rdoc::isInfinite)
        .def("makeInfinite", &LargeInteger::makeInfinite, rdoc::makeInfinite)
        .def("longValue", &LargeInteger::longValue, rdoc::longValue)
        .def("safeLongValue", &LargeInteger::safeLongValue, rdoc::safeLongValue)
        .def("stringValue", &LargeInteger::stringValue,
            pybind11::arg("base") = 10,
            rdoc::stringValue)
        .def("swap", &LargeInteger::swap, rdoc::swap)
        .def(pybind11::self == regina::Integer(), rdoc::__eq_2)
        .def(pybind11::self == long(), rdoc::__eq_3)
        .def(pybind11::self != regina::Integer(), rdoc::__ne_2)
        .def(pybind11::self != long(), rdoc::__ne_3)
        .def(pybind11::self < pybind11::self, rdoc::__lt)
        .def(pybind11::self < long(), rdoc::__lt_2)
        .def(pybind11::self > pybind11::self, rdoc::__gt)
        .def(pybind11::self > long(), rdoc::__gt_2)
        .def(pybind11::self <= pybind11::self, rdoc::__le)
        .def(pybind11::self <= long(), rdoc::__le_2)
        .def(pybind11::self >= pybind11::self, rdoc::__ge)
        .def(pybind11::self >= long(), rdoc::__ge_2)
        .def("inc", [](LargeInteger& i) {
            return i++;
        }, rdoc::__inc)
        .def("dec", [](LargeInteger& i) {
            return i--;
        }, rdoc::__dec)
        .def(pybind11::self + pybind11::self, rdoc::__add)
        .def(pybind11::self + long(), rdoc::__add_2)
        .def(pybind11::self - pybind11::self, rdoc::__sub)
        .def(pybind11::self - long(), rdoc::__sub_2)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def(pybind11::self * long(), rdoc::__mul_2)
        .def(pybind11::self / pybind11::self, rdoc::__div)
        .def(pybind11::self / long(), rdoc::__div_2)
        .def("divExact", overload_cast<const LargeInteger&>(
            &LargeInteger::divExact, pybind11::const_), rdoc::divExact)
        .def("divExact", overload_cast<long>(
            &LargeInteger::divExact, pybind11::const_), rdoc::divExact_2)
        .def(pybind11::self % pybind11::self, rdoc::__mod)
        .def(pybind11::self % long(), rdoc::__mod_2)
        .def("divisionAlg", overload_cast<const LargeInteger&>(
            &LargeInteger::divisionAlg, pybind11::const_), rdoc::divisionAlg)
        .def(- pybind11::self, rdoc::__sub_3)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self += long(), rdoc::__iadd_2)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self -= long(), rdoc::__isub_2)
        .def(pybind11::self *= pybind11::self, rdoc::__imul)
        .def(pybind11::self *= long(), rdoc::__imul_2)
        .def(pybind11::self /= pybind11::self, rdoc::__idiv)
        .def(pybind11::self /= long(), rdoc::__idiv_2)
        .def("divByExact", overload_cast<const LargeInteger&>(
            &LargeInteger::divByExact),
            pybind11::return_value_policy::reference_internal,
            rdoc::divByExact)
        .def("divByExact", overload_cast<long>(
            &LargeInteger::divByExact),
            pybind11::return_value_policy::reference_internal,
            rdoc::divByExact_2)
        .def(pybind11::self %= pybind11::self, rdoc::__imod)
        .def(pybind11::self %= long(), rdoc::__imod_2)
        .def("negate", &LargeInteger::negate, rdoc::negate)
        .def("raiseToPower", &LargeInteger::raiseToPower, rdoc::raiseToPower)
        .def("abs", &LargeInteger::abs, rdoc::abs)
        .def("gcdWith", &LargeInteger::gcdWith, rdoc::gcdWith)
        .def("gcd", &LargeInteger::gcd, rdoc::gcd)
        .def("lcmWith", &LargeInteger::lcmWith, rdoc::lcmWith)
        .def("lcm", &LargeInteger::lcm, rdoc::lcm)
        .def("gcdWithCoeffs", overload_cast<const LargeInteger&>(
            &LargeInteger::gcdWithCoeffs, pybind11::const_),
            rdoc::gcdWithCoeffs)
        .def("gcdWithCoeffs",
            overload_cast<const LargeInteger&, LargeInteger&, LargeInteger&>(
                &LargeInteger::gcdWithCoeffs, pybind11::const_),
            rdoc::gcdWithCoeffs_2)
        .def("legendre", &LargeInteger::legendre, rdoc::legendre)
        .def("randomBoundedByThis", &LargeInteger::randomBoundedByThis,
            rdoc::randomBoundedByThis)
        .def_static("randomBinary", &LargeInteger::randomBinary,
            rdoc::randomBinary)
        .def_static("randomCornerBinary", &LargeInteger::randomCornerBinary,
            rdoc::randomCornerBinary)
        .def("makeLarge", &LargeInteger::makeLarge, rdoc::makeLarge)
        .def("tryReduce", &LargeInteger::tryReduce, rdoc::tryReduce)
        .def(long() + pybind11::self, global::__add)
        .def(long() * pybind11::self, global::__mul)
        .def_readonly_static("zero", &LargeInteger::zero)
        .def_readonly_static("one", &LargeInteger::one)
        .def_readonly_static("infinity", &LargeInteger::infinity)
    ;
    regina::python::add_tight_encoding(c, rdoc::tightEncoding,
        rdoc::tightDecoding);
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);
    regina::python::add_output_ostream(c, regina::python::PYTHON_REPR_SLIM);

    RDOC_SCOPE_SWITCH_MAIN

    m.def("tightEncoding", static_cast<std::string(&)(LargeInteger)>(
        regina::tightEncoding), rdoc::tightEncoding);
    regina::python::add_global_swap<LargeInteger>(m, rdoc::swap);

    RDOC_SCOPE_END

    pybind11::implicitly_convertible<long, LargeInteger>();
    pybind11::implicitly_convertible<std::string, LargeInteger>();
}

