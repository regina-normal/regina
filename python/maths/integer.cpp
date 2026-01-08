
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include "maths/integer.h"
#include "../helpers.h"
#include "../docstrings/maths/integer.h"

using pybind11::overload_cast;
using regina::python::doc::common::neq_value;

template <bool inf>
void addIntegerBase(pybind11::module_& m, const char* className) {
    using Int = regina::IntegerBase<inf>;
    using AltInt = regina::IntegerBase<! inf>; // Integer <-> LargeInteger

    RDOC_SCOPE_BEGIN(IntegerBase)

    auto c = pybind11::class_<Int>(m, className, rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<long>(), rdoc::__init)
        .def(pybind11::init<const Int&>(), rdoc::__copy)
        .def(pybind11::init<const AltInt&>(), rdoc::__init_2)
        .def(pybind11::init([](pybind11::int_ l){
            return new Int(pybind11::cast<std::string>(pybind11::str(l)));
        }), rdoc::__init_4)
        .def(pybind11::init<double>(), rdoc::__init_5)
        .def(pybind11::init<const char*, int>(),
            pybind11::arg(), pybind11::arg("base") = 10,
            rdoc::__init_6)
        .def("isNative", &Int::isNative, rdoc::isNative)
        .def("isZero", &Int::isZero, rdoc::isZero)
        .def("sign", &Int::sign, rdoc::sign)
        .def("isInfinite", &Int::isInfinite, rdoc::isInfinite)
        .def("makeInfinite", &Int::makeInfinite, rdoc::makeInfinite)
        .def("safeValue", &Int::template safeValue<long>, rdoc::safeValue)
        .def("unsafeValue", &Int::template unsafeValue<long>, rdoc::unsafeValue)
        .def("safeLongValue", &Int::template safeValue<long>,
            rdoc::safeLongValue) // deprecated
        .def("longValue", &Int::template unsafeValue<long>,
            rdoc::longValue) // deprecated
        .def("stringValue", &Int::stringValue,
            pybind11::arg("base") = 10,
            rdoc::stringValue)
        .def("str", &Int::str, rdoc::str)
        .def("pythonValue", [](const Int& i) {
            try {
                return pybind11::int_(i.template unsafeValue<long>());
            } catch (const NoSolution&) {
                if (i.isInfinite())
                    throw pybind11::value_error("Cannot represent infinity "
                        "as a Python int");
                return pybind11::reinterpret_steal<pybind11::int_>(
                    PyLong_FromString(i.stringValue(16).c_str(), nullptr, 16));
            }
        }, rdoc::pythonValue)
        .def("swap", &Int::swap, rdoc::swap)
        .def(pybind11::self == AltInt(), rdoc::__eq_2)
        .def(pybind11::self == long(), rdoc::__eq_3)
        .def(pybind11::self != AltInt(), neq_value)
        .def(pybind11::self != long(), neq_value)
        .def(pybind11::self < long(), rdoc::__cmp_2)
        .def(pybind11::self > long(), rdoc::__cmp_2)
        .def(pybind11::self <= long(), rdoc::__cmp_2)
        .def(pybind11::self >= long(), rdoc::__cmp_2)
        .def("inc", [](Int& i) {
            return i++;
        }, rdoc::__inc)
        .def("dec", [](Int& i) {
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
        // overload_cast has trouble with templated vs non-templated overloads.
        // Just cast directly.
        .def("divExact",
            static_cast<Int (Int::*)(const Int&) const>(&Int::divExact),
            rdoc::divExact)
        .def("divExact",
            static_cast<Int (Int::*)(long) const>(&Int::divExact),
            rdoc::divExact_2)
        .def(pybind11::self % pybind11::self, rdoc::__mod)
        .def(pybind11::self % long(), rdoc::__mod_2)
        .def("divisionAlg", overload_cast<const Int&>(
            &Int::divisionAlg, pybind11::const_), rdoc::divisionAlg)
        .def(- pybind11::self, rdoc::__sub_3)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self += long(), rdoc::__iadd_2)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self -= long(), rdoc::__isub_2)
        .def(pybind11::self *= pybind11::self, rdoc::__imul)
        .def(pybind11::self *= long(), rdoc::__imul_2)
        .def(pybind11::self /= pybind11::self, rdoc::__idiv)
        .def(pybind11::self /= long(), rdoc::__idiv_2)
        .def("divByExact",
            static_cast<Int& (Int::*)(const Int&)>(&Int::divByExact),
            pybind11::return_value_policy::reference_internal,
            rdoc::divByExact)
        .def("divByExact",
            static_cast<Int& (Int::*)(long)>(&Int::divByExact),
            pybind11::return_value_policy::reference_internal,
            rdoc::divByExact_2)
        .def(pybind11::self %= pybind11::self, rdoc::__imod)
        .def(pybind11::self %= long(), rdoc::__imod_2)
        .def("negate", &Int::negate, rdoc::negate)
        .def("raiseToPower", &Int::raiseToPower, rdoc::raiseToPower)
        .def("abs", &Int::abs, rdoc::abs)
        .def("gcdWith", &Int::gcdWith, rdoc::gcdWith)
        .def("gcd", &Int::gcd, rdoc::gcd)
        .def("lcmWith", &Int::lcmWith, rdoc::lcmWith)
        .def("lcm", &Int::lcm, rdoc::lcm)
        .def("gcdWithCoeffs", overload_cast<const Int&>(
            &Int::gcdWithCoeffs, pybind11::const_),
            rdoc::gcdWithCoeffs)
        .def("gcdWithCoeffs", overload_cast<const Int&, Int&, Int&>(
            &Int::gcdWithCoeffs, pybind11::const_),
            rdoc::gcdWithCoeffs_2)
        .def("legendre", &Int::legendre, rdoc::legendre)
        .def("randomBoundedByThis", &Int::randomBoundedByThis,
            rdoc::randomBoundedByThis)
        .def_static("randomBinary", &Int::randomBinary, rdoc::randomBinary)
        .def_static("randomCornerBinary", &Int::randomCornerBinary,
            rdoc::randomCornerBinary)
        .def("makeLarge", &Int::makeLarge, rdoc::makeLarge)
        .def("tryReduce", &Int::tryReduce, rdoc::tryReduce)
        .def(long() + pybind11::self, rdoc_global::__add)
        .def(long() * pybind11::self, rdoc_global::__mul)
        .def_readonly_static("supportsInfinity", &Int::supportsInfinity)
        .def_readonly_static("zero", &Int::zero)
        .def_readonly_static("one", &Int::one)
    ;
    if constexpr (inf)
        c.def_readonly_static("infinity", &Int::infinity);

    regina::python::add_tight_encoding(c, rdoc::tightEncoding,
        rdoc::tightDecoding, rdoc::hash);
    regina::python::add_eq_operators(c, rdoc::__eq);
    regina::python::add_cmp_operators(c, rdoc::__cmp);
    regina::python::add_output_ostream(c, regina::python::ReprStyle::Slim);

    m.def("tightEncoding", static_cast<std::string(&)(Int)>(
        regina::tightEncoding), rdoc_global::tightEncoding);
    regina::python::add_global_swap<Int>(m, rdoc::global_swap);

    RDOC_SCOPE_END

    pybind11::implicitly_convertible<long, Int>();
    pybind11::implicitly_convertible<std::string, Int>();
}

void addInteger(pybind11::module_& m) {
    addIntegerBase<false>(m, "Integer");
    addIntegerBase<true>(m, "LargeInteger");
}

