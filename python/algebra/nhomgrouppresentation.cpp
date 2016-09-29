
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
#include "algebra/nabeliangroup.h"
#include "algebra/ngrouppresentation.h"
#include "algebra/nhomgrouppresentation.h"
#include "algebra/nmarkedabeliangroup.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NGroupExpression;
using regina::NGroupPresentation;
using regina::NHomGroupPresentation;

namespace {
    NGroupExpression (NHomGroupPresentation::*evaluate_ulong)(
        unsigned long) const = &NHomGroupPresentation::evaluate;
    NGroupExpression (NHomGroupPresentation::*evaluate_exp)(
        const NGroupExpression&) const = &NHomGroupPresentation::evaluate;
    NGroupExpression (NHomGroupPresentation::*invEvaluate_ulong)(
        unsigned long) const = &NHomGroupPresentation::invEvaluate;
    NGroupExpression (NHomGroupPresentation::*invEvaluate_exp)(
        const NGroupExpression&) const = &NHomGroupPresentation::invEvaluate;
}

void addNHomGroupPresentation() {
    class_<NHomGroupPresentation, std::auto_ptr<NHomGroupPresentation>,
            boost::noncopyable>
            ("NHomGroupPresentation", init<const NHomGroupPresentation&>())
        .def(init<const NGroupPresentation&>())
        .def("domain", &NHomGroupPresentation::domain,
            return_internal_reference<>())
        .def("getDomain", &NHomGroupPresentation::domain,
            return_internal_reference<>())
        .def("range", &NHomGroupPresentation::range,
            return_internal_reference<>())
        .def("getRange", &NHomGroupPresentation::range,
            return_internal_reference<>())
        .def("knowsInverse", &NHomGroupPresentation::knowsInverse)
        .def("evaluate", evaluate_exp)
        .def("evaluate", evaluate_ulong)
        .def("invEvaluate", invEvaluate_exp)
        .def("invEvaluate", invEvaluate_ulong)
        .def("intelligentSimplify",
            &NHomGroupPresentation::intelligentSimplify)
        .def("intelligentNielsen",
            &NHomGroupPresentation::intelligentNielsen)
        .def("smallCancellation",
            &NHomGroupPresentation::smallCancellation)
        .def("composeWith",
            &NHomGroupPresentation::composeWith)
        .def("invert", &NHomGroupPresentation::invert)
        .def("verify", &NHomGroupPresentation::verify)
        .def("verifyIsomorphism", &NHomGroupPresentation::verifyIsomorphism)
        .def("markedAbelianisation",
            &NHomGroupPresentation::markedAbelianisation)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;
}

