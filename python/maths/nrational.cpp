
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
#include "maths/nrational.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NInteger;
using regina::NLargeInteger;
using regina::NRational;

namespace {
    double doubleApprox_void(const NRational& r) {
        return r.doubleApprox();
    }

    boost::python::tuple doubleApprox_bool(const NRational& r) {
        bool inRange;
        double ans = r.doubleApprox(&inRange);
        return boost::python::make_tuple(ans, inRange);
    }

    void writeTeX_stdio(const NRational& r) {
        r.writeTeX(std::cout);
    }
}

void addNRational() {
    scope s = class_<NRational>("NRational")
        .def(init<const NRational&>())
        .def(init<const NInteger&>())
        .def(init<const NLargeInteger&>())
        .def(init<long>())
        .def(init<const NInteger&, const NInteger&>())
        .def(init<const NLargeInteger&, const NLargeInteger&>())
        .def(init<long, unsigned long>())
        .def("swap", &NRational::swap)
        .def("numerator", &NRational::numerator)
        .def("getNumerator", &NRational::numerator)
        .def("denominator", &NRational::denominator)
        .def("getDenominator", &NRational::denominator)
        .def(self * self)
        .def(self / self)
        .def(self + self)
        .def(self - self)
        .def(- self)
        .def("inverse", &NRational::inverse)
        .def("abs", &NRational::abs)
        .def(self += self)
        .def(self -= self)
        .def(self *= self)
        .def(self /= self)
        .def("negate", &NRational::negate)
        .def("invert", &NRational::invert)
        .def(self < self)
        .def(self > self)
        .def(self <= self)
        .def(self >= self)
        .def("doubleApprox", doubleApprox_void)
        .def("doubleApproxCheck", doubleApprox_bool)
        .def("TeX", &NRational::TeX)
        .def("getTeX", &NRational::TeX)
        .def("writeTeX", writeTeX_stdio)
        .def(self_ns::str(self))
        .def(self_ns::repr(self))
        .def(regina::python::add_eq_operators())
    ;

    // Apparently there is no way in python to make a module attribute
    // read-only.
    s.attr("zero") = NRational::zero;
    s.attr("one") = NRational::one;
    s.attr("infinity") = NRational::infinity;
    s.attr("undefined") = NRational::undefined;

    boost::python::implicitly_convertible<NInteger, NRational>();
    boost::python::implicitly_convertible<NLargeInteger, NRational>();
    boost::python::implicitly_convertible<long, NRational>();

}

