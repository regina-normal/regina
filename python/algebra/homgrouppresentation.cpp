
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
#include "algebra/abeliangroup.h"
#include "algebra/grouppresentation.h"
#include "algebra/homgrouppresentation.h"
#include "algebra/markedabeliangroup.h"
#include "../helpers.h"

using namespace boost::python;
using regina::GroupExpression;
using regina::GroupPresentation;
using regina::HomGroupPresentation;

namespace {
    GroupExpression (HomGroupPresentation::*evaluate_ulong)(
        unsigned long) const = &HomGroupPresentation::evaluate;
    GroupExpression (HomGroupPresentation::*evaluate_exp)(
        const GroupExpression&) const = &HomGroupPresentation::evaluate;
    GroupExpression (HomGroupPresentation::*invEvaluate_ulong)(
        unsigned long) const = &HomGroupPresentation::invEvaluate;
    GroupExpression (HomGroupPresentation::*invEvaluate_exp)(
        const GroupExpression&) const = &HomGroupPresentation::invEvaluate;
}

void addHomGroupPresentation() {
    class_<HomGroupPresentation, std::auto_ptr<HomGroupPresentation>,
            boost::noncopyable>
            ("HomGroupPresentation", init<const HomGroupPresentation&>())
        .def(init<const GroupPresentation&>())
        .def("domain", &HomGroupPresentation::domain,
            return_internal_reference<>())
        .def("range", &HomGroupPresentation::range,
            return_internal_reference<>())
        .def("knowsInverse", &HomGroupPresentation::knowsInverse)
        .def("evaluate", evaluate_exp)
        .def("evaluate", evaluate_ulong)
        .def("invEvaluate", invEvaluate_exp)
        .def("invEvaluate", invEvaluate_ulong)
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
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
    scope().attr("NHomGroupPresentation") = scope().attr("HomGroupPresentation");
}

