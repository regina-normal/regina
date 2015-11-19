
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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

/* end stub */

#include <boost/python.hpp>
#include "maths/nmatrixint.h"
#include "snappea/nsnappeatriangulation.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NCusp;
using regina::NSnapPeaTriangulation;
using regina::NTriangulation;

namespace {
    double (NSnapPeaTriangulation::*volume_void)() const =
        &NSnapPeaTriangulation::volume;
    NTriangulation* (NSnapPeaTriangulation::*filledTriangulation_void)() const =
        &NSnapPeaTriangulation::filledTriangulation;
    NTriangulation* (NSnapPeaTriangulation::*filledTriangulation_unsigned)
        (unsigned) const = &NSnapPeaTriangulation::filledTriangulation;

    boost::python::tuple volume_precision(const NSnapPeaTriangulation& t) {
        int precision;
        double volume = t.volume(precision);
        return make_tuple(volume, precision);
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_fundamentalGroupFilled,
        NSnapPeaTriangulation::fundamentalGroupFilled, 0, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_cusp,
        NSnapPeaTriangulation::cusp, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_fill,
        NSnapPeaTriangulation::fill, 2, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_unfill,
        NSnapPeaTriangulation::unfill, 0, 1);

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_enableKernelMessages,
        NSnapPeaTriangulation::enableKernelMessages, 0, 1);
}

void addNSnapPeaTriangulation() {
    class_<NCusp, std::auto_ptr<NCusp>, boost::noncopyable>("NCusp", no_init)
        .def("vertex", &NCusp::vertex,
            return_value_policy<reference_existing_object>())
        .def("complete", &NCusp::complete)
        .def("m", &NCusp::m)
        .def("l", &NCusp::l)
        .def("str", &NCusp::str)
        .def("toString", &NCusp::toString)
        .def("detail", &NCusp::detail)
        .def("toStringLong", &NCusp::toStringLong)
        .def("__str__", &NCusp::str)
        .def(regina::python::add_eq_operators())
    ;

    scope s = class_<NSnapPeaTriangulation, bases<regina::NTriangulation>,
            std::auto_ptr<NSnapPeaTriangulation>, boost::noncopyable>
            ("NSnapPeaTriangulation", init<>())
        .def(init<const std::string&>())
        .def(init<const NSnapPeaTriangulation&>())
        .def(init<const NTriangulation&, optional<bool> >())
        .def("isNull", &NSnapPeaTriangulation::isNull)
        .def("name", &NSnapPeaTriangulation::name)
        .def("solutionType", &NSnapPeaTriangulation::solutionType)
        .def("volume", volume_void)
        .def("volumeWithPrecision", volume_precision)
        .def("volumeZero", &NSnapPeaTriangulation::volumeZero)
        .def("shape", &NSnapPeaTriangulation::shape,
            return_value_policy<copy_const_reference>())
        .def("minImaginaryShape", &NSnapPeaTriangulation::minImaginaryShape)
        .def("gluingEquations", &NSnapPeaTriangulation::gluingEquations,
            return_value_policy<manage_new_object>())
        .def("gluingEquationsRect", &NSnapPeaTriangulation::gluingEquationsRect,
            return_value_policy<manage_new_object>())
        .def("countCusps", &NSnapPeaTriangulation::countCusps)
        .def("countCompleteCusps", &NSnapPeaTriangulation::countCompleteCusps)
        .def("countFilledCusps", &NSnapPeaTriangulation::countFilledCusps)
        .def("cusp", &NSnapPeaTriangulation::cusp, OL_cusp()[
            return_value_policy<reference_existing_object>()])
        .def("fill", &NSnapPeaTriangulation::fill, OL_fill())
        .def("unfill", &NSnapPeaTriangulation::unfill, OL_unfill())
        .def("filledTriangulation", filledTriangulation_void,
            return_value_policy<manage_new_object>())
        .def("filledTriangulation", filledTriangulation_unsigned,
            return_value_policy<manage_new_object>())
        .def("slopeEquations", &NSnapPeaTriangulation::slopeEquations,
            return_value_policy<manage_new_object>())
        .def("fundamentalGroupFilled",
            &NSnapPeaTriangulation::fundamentalGroupFilled,
            OL_fundamentalGroupFilled(args(
                "simplify_presentation",
                "fillings_may_affect_generators",
                "minimize_number_of_generators"))
            [return_internal_reference<>()])
        .def("homologyFilled", &NSnapPeaTriangulation::homologyFilled,
            return_internal_reference<>())
        .def("verifyTriangulation", &NSnapPeaTriangulation::verifyTriangulation)
        .def("toRegina", &NSnapPeaTriangulation::toRegina,
            return_value_policy<manage_new_object>())
        .def("protoCanonize", &NSnapPeaTriangulation::protoCanonize,
            return_value_policy<manage_new_object>())
        .def("protoCanonise", &NSnapPeaTriangulation::protoCanonise,
            return_value_policy<manage_new_object>())
        .def("canonize", &NSnapPeaTriangulation::canonize,
            return_value_policy<manage_new_object>())
        .def("canonise", &NSnapPeaTriangulation::canonise,
            return_value_policy<manage_new_object>())
        .def("randomize", &NSnapPeaTriangulation::randomize)
        .def("randomise", &NSnapPeaTriangulation::randomise)
        .def("dump", &NSnapPeaTriangulation::dump)
        .def("saveAsSnapPea", &NSnapPeaTriangulation::saveAsSnapPea)
        .def("kernelMessagesEnabled",
            &NSnapPeaTriangulation::kernelMessagesEnabled)
        .def("enableKernelMessages",
            &NSnapPeaTriangulation::enableKernelMessages,
            OL_enableKernelMessages())
        .def("disableKernelMessages",
            &NSnapPeaTriangulation::disableKernelMessages)
        .staticmethod("kernelMessagesEnabled")
        .staticmethod("enableKernelMessages")
        .staticmethod("disableKernelMessages")
    ;

    enum_<NSnapPeaTriangulation::SolutionType>("SolutionType")
        .value("not_attempted", NSnapPeaTriangulation::not_attempted)
        .value("geometric_solution", NSnapPeaTriangulation::geometric_solution)
        .value("nongeometric_solution",
            NSnapPeaTriangulation::nongeometric_solution)
        .value("flat_solution", NSnapPeaTriangulation::flat_solution)
        .value("degenerate_solution",
            NSnapPeaTriangulation::degenerate_solution)
        .value("other_solution", NSnapPeaTriangulation::other_solution)
        .value("no_solution", NSnapPeaTriangulation::no_solution)
    ;

    implicitly_convertible<std::auto_ptr<NSnapPeaTriangulation>,
        std::auto_ptr<regina::NTriangulation> >();
}

