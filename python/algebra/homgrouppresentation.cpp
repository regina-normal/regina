
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "algebra/homgrouppresentation.h"
#include "algebra/markedabeliangroup.h"
#include "../helpers.h"
#include "../docstrings/algebra/homgrouppresentation.h"

using pybind11::overload_cast;
using regina::GroupExpression;
using regina::GroupPresentation;
using regina::HomGroupPresentation;

void addHomGroupPresentation(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(HomGroupPresentation)

    auto c = pybind11::class_<HomGroupPresentation>(m, "HomGroupPresentation",
            rdoc_scope)
        .def(pybind11::init<GroupPresentation, GroupPresentation,
            std::vector<GroupExpression>>(), rdoc::__init)
        .def(pybind11::init<GroupPresentation, GroupPresentation,
            std::vector<GroupExpression>, std::vector<GroupExpression>>(),
            rdoc::__init_2)
        .def(pybind11::init<const GroupPresentation&>(), rdoc::__init_3)
        .def(pybind11::init<const HomGroupPresentation&>(), rdoc::__copy)
        .def("swap", &HomGroupPresentation::swap, rdoc::swap)
        .def("domain", &HomGroupPresentation::domain,
            pybind11::return_value_policy::reference_internal, rdoc::domain)
        .def("codomain", &HomGroupPresentation::codomain,
            pybind11::return_value_policy::reference_internal, rdoc::codomain)
        .def("knowsInverse", &HomGroupPresentation::knowsInverse,
            rdoc::knowsInverse)
        .def("evaluate", overload_cast<GroupExpression>(
            &HomGroupPresentation::evaluate, pybind11::const_),
            rdoc::evaluate)
        .def("evaluate", overload_cast<unsigned long>(
            &HomGroupPresentation::evaluate, pybind11::const_),
            rdoc::evaluate_2)
        .def("invEvaluate", overload_cast<GroupExpression>(
            &HomGroupPresentation::invEvaluate, pybind11::const_),
            rdoc::invEvaluate)
        .def("invEvaluate", overload_cast<unsigned long>(
            &HomGroupPresentation::invEvaluate, pybind11::const_),
            rdoc::invEvaluate_2)
        .def("simplify", &HomGroupPresentation::simplify, rdoc::simplify)
        .def("intelligentSimplify", // deprecated
            &HomGroupPresentation::simplify, rdoc::intelligentSimplify)
        .def("nielsen", &HomGroupPresentation::nielsen, rdoc::nielsen)
        .def("intelligentNielsen", // deprecated
            &HomGroupPresentation::nielsen, rdoc::intelligentNielsen)
        .def("smallCancellation",
            &HomGroupPresentation::smallCancellation, rdoc::smallCancellation)
        .def(pybind11::self * pybind11::self, rdoc::__mul)
        .def("invert", &HomGroupPresentation::invert, rdoc::invert)
        .def("verify", &HomGroupPresentation::verify, rdoc::verify)
        .def("verifyIsomorphism", &HomGroupPresentation::verifyIsomorphism,
            rdoc::verifyIsomorphism)
        .def("markedAbelianisation",
            &HomGroupPresentation::markedAbelianisation,
            rdoc::markedAbelianisation)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c, rdoc::__eq);

    regina::python::add_global_swap<HomGroupPresentation>(m, rdoc::global_swap);

    RDOC_SCOPE_END
}

