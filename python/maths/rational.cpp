
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
#include "maths/rational.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Integer;
using regina::LargeInteger;
using regina::Rational;

namespace {
    double doubleApprox_void(const Rational& r) {
        return r.doubleApprox();
    }

    boost::python::tuple doubleApprox_bool(const Rational& r) {
        bool inRange;
        double ans = r.doubleApprox(&inRange);
        return boost::python::make_tuple(ans, inRange);
    }

    void writeTeX_stdio(const Rational& r) {
        r.writeTeX(std::cout);
    }
}

void addRational() {
    {
        scope s = class_<Rational>("Rational")
            .def(init<const Rational&>())
            .def(init<const Integer&>())
            .def(init<const LargeInteger&>())
            .def(init<long>())
            .def(init<const Integer&, const Integer&>())
            .def(init<const LargeInteger&, const LargeInteger&>())
            .def(init<long, unsigned long>())
            .def("swap", &Rational::swap)
            .def("numerator", &Rational::numerator)
            .def("denominator", &Rational::denominator)
            .def(self * self)
            .def(self / self)
            .def(self + self)
            .def(self - self)
            .def(- self)
            .def("inverse", &Rational::inverse)
            .def("abs", &Rational::abs)
            .def(self += self)
            .def(self -= self)
            .def(self *= self)
            .def(self /= self)
            .def("negate", &Rational::negate)
            .def("invert", &Rational::invert)
            .def(self < self)
            .def(self > self)
            .def(self <= self)
            .def(self >= self)
            .def("doubleApprox", doubleApprox_void)
            .def("doubleApproxCheck", doubleApprox_bool)
            .def("TeX", &Rational::TeX)
            .def("writeTeX", writeTeX_stdio)
            .def(self_ns::str(self))
            .def(self_ns::repr(self))
            .def(regina::python::add_eq_operators())
        ;

        // Apparently there is no way in python to make a module attribute
        // read-only.
        s.attr("zero") = Rational::zero;
        s.attr("one") = Rational::one;
        s.attr("infinity") = Rational::infinity;
        s.attr("undefined") = Rational::undefined;
    }

    boost::python::implicitly_convertible<Integer, Rational>();
    boost::python::implicitly_convertible<LargeInteger, Rational>();
    boost::python::implicitly_convertible<long, Rational>();

    scope().attr("NRational") = scope().attr("Rational");
}

