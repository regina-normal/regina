
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
#include "maths/rational.h"
#include "../helpers.h"

using regina::Integer;
using regina::LargeInteger;
using regina::Rational;

void addRational(pybind11::module_& m) {
    auto c = pybind11::class_<Rational>(m, "Rational")
        .def(pybind11::init<>())
        .def(pybind11::init<const Rational&>())
        .def(pybind11::init<const Integer&>())
        .def(pybind11::init<const LargeInteger&>())
        .def(pybind11::init<long>())
        .def(pybind11::init<const Integer&, const Integer&>())
        .def(pybind11::init<const LargeInteger&, const LargeInteger&>())
        .def(pybind11::init<long, unsigned long>())
        .def("swap", &Rational::swap)
        .def("numerator", &Rational::numerator)
        .def("denominator", &Rational::denominator)
        .def(pybind11::self * pybind11::self)
        .def(pybind11::self / pybind11::self)
        .def(pybind11::self + pybind11::self)
        .def(pybind11::self - pybind11::self)
        .def(- pybind11::self)
        .def("inverse", &Rational::inverse)
        .def("abs", &Rational::abs)
        .def(pybind11::self += pybind11::self)
        .def(pybind11::self -= pybind11::self)
        .def(pybind11::self *= pybind11::self)
        .def(pybind11::self /= pybind11::self)
        .def("negate", &Rational::negate)
        .def("invert", &Rational::invert)
        .def(pybind11::self < pybind11::self)
        .def(pybind11::self > pybind11::self)
        .def(pybind11::self <= pybind11::self)
        .def(pybind11::self >= pybind11::self)
        .def("doubleApprox", [](const Rational& r) {
            return r.doubleApprox();
        })
        .def("doubleApproxCheck", [](const Rational& r) {
            bool inRange;
            double ans = r.doubleApprox(&inRange);
            return pybind11::make_tuple(ans, inRange);
        })
        .def("TeX", &Rational::TeX)
        .def("writeTeX", [](const Rational& r) {
            r.writeTeX(std::cout);
        })
        .def_readonly_static("zero", &Rational::zero)
        .def_readonly_static("one", &Rational::one)
        .def_readonly_static("infinity", &Rational::infinity)
        .def_readonly_static("undefined", &Rational::undefined)
    ;
    regina::python::add_eq_operators(c);
    regina::python::add_output_ostream(c, true /* __repr__ */);

    pybind11::implicitly_convertible<Integer, Rational>();
    pybind11::implicitly_convertible<LargeInteger, Rational>();
    pybind11::implicitly_convertible<long, Rational>();

    m.attr("NRational") = m.attr("Rational");
}

