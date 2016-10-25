
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

#include "maths/matrix.h"
#include "snappea/snappeatriangulation.h"
#include "../safeheldtype.h"
#include "../helpers.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Cusp;
using regina::SnapPeaTriangulation;
using regina::Triangulation;

namespace {
    double (SnapPeaTriangulation::*volume_void)() const =
        &SnapPeaTriangulation::volume;
    Triangulation<3>* (SnapPeaTriangulation::*filledTriangulation_void)() const =
        &SnapPeaTriangulation::filledTriangulation;
    Triangulation<3>* (SnapPeaTriangulation::*filledTriangulation_unsigned)
        (unsigned) const = &SnapPeaTriangulation::filledTriangulation;

    boost::python::tuple volume_precision(const SnapPeaTriangulation& t) {
        int precision;
        double volume = t.volume(precision);
        return boost::python::make_tuple(volume, precision);
    }

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_fundamentalGroupFilled,
        SnapPeaTriangulation::fundamentalGroupFilled, 0, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_cusp,
        SnapPeaTriangulation::cusp, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_fill,
        SnapPeaTriangulation::fill, 2, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_unfill,
        SnapPeaTriangulation::unfill, 0, 1);

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_enableKernelMessages,
        SnapPeaTriangulation::enableKernelMessages, 0, 1);
}

void addSnapPeaTriangulation() {
    class_<Cusp, std::auto_ptr<Cusp>, boost::noncopyable>("Cusp", no_init)
        .def("vertex", &Cusp::vertex,
            return_value_policy<reference_existing_object>())
        .def("complete", &Cusp::complete)
        .def("m", &Cusp::m)
        .def("l", &Cusp::l)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    class_<SnapPeaTriangulation, bases<regina::Triangulation<3>>,
            SafeHeldType<SnapPeaTriangulation>, boost::noncopyable>
            ("SnapPeaTriangulation", init<>())
        .def(init<const std::string&>())
        .def(init<const SnapPeaTriangulation&>())
        .def(init<const Triangulation<3>&, optional<bool> >())
        .def("isNull", &SnapPeaTriangulation::isNull)
        .def("name", &SnapPeaTriangulation::name)
        .def("solutionType", &SnapPeaTriangulation::solutionType)
        .def("volume", volume_void)
        .def("volumeWithPrecision", volume_precision)
        .def("volumeZero", &SnapPeaTriangulation::volumeZero)
        .def("shape", &SnapPeaTriangulation::shape,
            return_value_policy<copy_const_reference>())
        .def("minImaginaryShape", &SnapPeaTriangulation::minImaginaryShape)
        .def("gluingEquations", &SnapPeaTriangulation::gluingEquations,
            return_value_policy<manage_new_object>())
        .def("gluingEquationsRect",
            &SnapPeaTriangulation::gluingEquationsRect,
            return_value_policy<manage_new_object>())
        .def("countCusps", &SnapPeaTriangulation::countCusps)
        .def("countCompleteCusps",
            &SnapPeaTriangulation::countCompleteCusps)
        .def("countFilledCusps", &SnapPeaTriangulation::countFilledCusps)
        .def("cusp", &SnapPeaTriangulation::cusp, OL_cusp()[
            return_internal_reference<>()])
        .def("fill", &SnapPeaTriangulation::fill, OL_fill())
        .def("unfill", &SnapPeaTriangulation::unfill, OL_unfill())
        .def("filledTriangulation", filledTriangulation_void,
            return_value_policy<to_held_type<> >())
        .def("filledTriangulation", filledTriangulation_unsigned,
            return_value_policy<to_held_type<> >())
        .def("slopeEquations", &SnapPeaTriangulation::slopeEquations,
            return_value_policy<manage_new_object>())
        .def("fundamentalGroupFilled",
            &SnapPeaTriangulation::fundamentalGroupFilled,
            OL_fundamentalGroupFilled(args(
                "simplify_presentation",
                "fillings_may_affect_generators",
                "minimize_number_of_generators"))
            [return_internal_reference<>()])
        .def("homologyFilled", &SnapPeaTriangulation::homologyFilled,
            return_internal_reference<>())
        .def("protoCanonize", &SnapPeaTriangulation::protoCanonize,
            return_value_policy<to_held_type<> >())
        .def("protoCanonise", &SnapPeaTriangulation::protoCanonise,
            return_value_policy<to_held_type<> >())
        .def("canonize", &SnapPeaTriangulation::canonize,
            return_value_policy<to_held_type<> >())
        .def("canonise", &SnapPeaTriangulation::canonise,
            return_value_policy<to_held_type<> >())
        .def("randomize", &SnapPeaTriangulation::randomize)
        .def("randomise", &SnapPeaTriangulation::randomise)
        .def("kernelMessagesEnabled",
            &SnapPeaTriangulation::kernelMessagesEnabled)
        .def("enableKernelMessages",
            &SnapPeaTriangulation::enableKernelMessages,
            OL_enableKernelMessages())
        .def("disableKernelMessages",
            &SnapPeaTriangulation::disableKernelMessages)
        .staticmethod("kernelMessagesEnabled")
        .staticmethod("enableKernelMessages")
        .staticmethod("disableKernelMessages")
        .attr("typeID") = regina::PACKET_SNAPPEATRIANGULATION
    ;

    enum_<SnapPeaTriangulation::SolutionType>("SolutionType")
        .value("not_attempted", SnapPeaTriangulation::not_attempted)
        .value("geometric_solution",
            SnapPeaTriangulation::geometric_solution)
        .value("nongeometric_solution",
            SnapPeaTriangulation::nongeometric_solution)
        .value("flat_solution", SnapPeaTriangulation::flat_solution)
        .value("degenerate_solution",
            SnapPeaTriangulation::degenerate_solution)
        .value("other_solution", SnapPeaTriangulation::other_solution)
        .value("no_solution", SnapPeaTriangulation::no_solution)
        .value("externally_computed",
            SnapPeaTriangulation::externally_computed)
    ;

    implicitly_convertible<SafeHeldType<SnapPeaTriangulation>,
        SafeHeldType<regina::Triangulation<3>> >();

    FIX_REGINA_BOOST_CONVERTERS(SnapPeaTriangulation);

    scope().attr("NCusp") = scope().attr("Cusp");
    scope().attr("NSnapPeaTriangulation") =
        scope().attr("SnapPeaTriangulation");
}

