
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
#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "algebra/homgrouppresentation.h"
#include "algebra/markedabeliangroup.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::GroupExpression;
using regina::GroupPresentation;
using regina::HomGroupPresentation;

void addHomGroupPresentation(pybind11::module_& m) {
    auto c = pybind11::class_<HomGroupPresentation>(m, "HomGroupPresentation")
        .def(pybind11::init<const HomGroupPresentation&>())
        .def(pybind11::init<const GroupPresentation&>())
        .def("domain", &HomGroupPresentation::domain,
            pybind11::return_value_policy::reference_internal)
        .def("range", &HomGroupPresentation::range,
            pybind11::return_value_policy::reference_internal)
        .def("knowsInverse", &HomGroupPresentation::knowsInverse)
        .def("evaluate", overload_cast<unsigned long>(
            &HomGroupPresentation::evaluate, pybind11::const_))
        .def("evaluate", overload_cast<const GroupExpression&>(
            &HomGroupPresentation::evaluate, pybind11::const_))
        .def("invEvaluate", overload_cast<unsigned long>(
            &HomGroupPresentation::invEvaluate, pybind11::const_))
        .def("invEvaluate", overload_cast<const GroupExpression&>(
            &HomGroupPresentation::invEvaluate, pybind11::const_))
        .def("intelligentSimplify",
            &HomGroupPresentation::intelligentSimplify)
        .def("intelligentNielsen",
            &HomGroupPresentation::intelligentNielsen)
        .def("smallCancellation",
            &HomGroupPresentation::smallCancellation)
        .def("composeWith",
            &HomGroupPresentation::composeWith)
        .def("invert", &HomGroupPresentation::invert)
        .def("verify", &HomGroupPresentation::verify)
        .def("verifyIsomorphism", &HomGroupPresentation::verifyIsomorphism)
        .def("markedAbelianisation",
            &HomGroupPresentation::markedAbelianisation)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);
    m.attr("NHomGroupPresentation") = m.attr("HomGroupPresentation");
}

