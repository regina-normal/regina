
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
#include "maths/rational.h"
#include "../helpers.h"
#include "../docstrings/maths/rational.h"

using regina::Integer;
using regina::LargeInteger;
using regina::Rational;

void addRational(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Rational)

    auto c = pybind11::class_<Rational>(m, "Rational", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__init)
        .def(pybind11::init<const Rational&>(), rdoc::__copy)
        .def(pybind11::init<const Integer&>(), rdoc::__init_2)
        .def(pybind11::init<const LargeInteger&>(), rdoc::__init_2)
        .def(pybind11::init<long>(), rdoc::__init_3)
        .def(pybind11::init<const Integer&, const Integer&>(), rdoc::__init_4)
        .def(pybind11::init<const LargeInteger&, const LargeInteger&>(),
            rdoc::__init_4)
        .def(pybind11::init<long, unsigned long>(), rdoc::__init_5)
        .def("swap", &Rational::swap, rdoc::swap)
        .def("numerator", &Rational::numerator, rdoc::numerator)
        .def("denominator", &Rational::denominator, rdoc::denominator)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def(pybind11::self / pybind11::self, rdoc::__div)
        .def(pybind11::self + pybind11::self, rdoc::__add)
        .def(pybind11::self - pybind11::self, rdoc::__sub)
        .def(- pybind11::self, rdoc::__sub_2)
        .def("inverse", &Rational::inverse, rdoc::inverse)
        .def("abs", &Rational::abs, rdoc::abs)
        .def(pybind11::self += pybind11::self, rdoc::__iadd)
        .def(pybind11::self -= pybind11::self, rdoc::__isub)
        .def(pybind11::self *= pybind11::self, rdoc::__imul)
        .def(pybind11::self /= pybind11::self, rdoc::__idiv)
        .def("negate", &Rational::negate, rdoc::negate)
        .def("invert", &Rational::invert, rdoc::invert)
        .def(pybind11::self < pybind11::self, rdoc::__lt)
        .def(pybind11::self > pybind11::self, rdoc::__gt)
        .def(pybind11::self <= pybind11::self, rdoc::__le)
        .def(pybind11::self >= pybind11::self, rdoc::__ge)
        .def("doubleApprox", &Rational::doubleApprox, rdoc::doubleApprox)
        .def("tex", &Rational::tex, rdoc::tex)
        .def_readonly_static("zero", &Rational::zero)
        .def_readonly_static("one", &Rational::one)
        .def_readonly_static("infinity", &Rational::infinity)
        .def_readonly_static("undefined", &Rational::undefined)
    ;
    regina::python::add_eq_operators(c, rdoc::__eq, rdoc::__ne);
    regina::python::add_output_ostream(c, regina::python::PYTHON_REPR_SLIM);

    regina::python::add_global_swap<Rational>(m, rdoc::global_swap);

    RDOC_SCOPE_END

    pybind11::implicitly_convertible<Integer, Rational>();
    pybind11::implicitly_convertible<LargeInteger, Rational>();
    pybind11::implicitly_convertible<long, Rational>();
}

