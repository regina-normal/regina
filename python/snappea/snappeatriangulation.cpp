
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
#include "../pybind11/complex.h"
#include "maths/matrix.h"
#include "snappea/snappeatriangulation.h"
#include "utilities/safeptr.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Cusp;
using regina::SnapPeaTriangulation;
using regina::Triangulation;

void addSnapPeaTriangulation(pybind11::module_& m) {
    auto c1 = pybind11::class_<Cusp>(m, "Cusp")
        .def("vertex", &Cusp::vertex,
            pybind11::return_value_policy::reference)
        .def("complete", &Cusp::complete)
        .def("m", &Cusp::m)
        .def("l", &Cusp::l)
    ;
    regina::python::add_output(c1);
    regina::python::add_eq_operators(c1);

    auto c2 = pybind11::class_<SnapPeaTriangulation, regina::Triangulation<3>,
            regina::SafePtr<SnapPeaTriangulation>>(m, "SnapPeaTriangulation")
        .def(pybind11::init<>())
        .def(pybind11::init<const std::string&>())
        .def(pybind11::init<const SnapPeaTriangulation&>())
        .def(pybind11::init<const Triangulation<3>&, bool>(),
            pybind11::arg(), pybind11::arg("ignored") = false)
        .def("isNull", &SnapPeaTriangulation::isNull)
        .def("name", &SnapPeaTriangulation::name)
        .def("solutionType", &SnapPeaTriangulation::solutionType)
        .def("volume", overload_cast<>(
            &SnapPeaTriangulation::volume, pybind11::const_))
        .def("volumeWithPrecision", [](const SnapPeaTriangulation& t) {
            int precision;
            double volume = t.volume(precision);
            return pybind11::make_tuple(volume, precision);
        })
        .def("volumeZero", &SnapPeaTriangulation::volumeZero)
        .def("shape", &SnapPeaTriangulation::shape)
        .def("minImaginaryShape", &SnapPeaTriangulation::minImaginaryShape)
        .def("gluingEquations", &SnapPeaTriangulation::gluingEquations)
        .def("gluingEquationsRect",
            &SnapPeaTriangulation::gluingEquationsRect)
        .def("countCusps", &SnapPeaTriangulation::countCusps)
        .def("countCompleteCusps",
            &SnapPeaTriangulation::countCompleteCusps)
        .def("countFilledCusps", &SnapPeaTriangulation::countFilledCusps)
        .def("cusp", &SnapPeaTriangulation::cusp,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("whichCusp") = 0)
        .def("fill", &SnapPeaTriangulation::fill,
            pybind11::arg(), pybind11::arg(), pybind11::arg("whichCusp")= 0)
        .def("unfill", &SnapPeaTriangulation::unfill,
            pybind11::arg("whichCusp")= 0)
        .def("filledTriangulation", overload_cast<>(
            &SnapPeaTriangulation::filledTriangulation, pybind11::const_))
        .def("filledTriangulation", overload_cast<unsigned>(
            &SnapPeaTriangulation::filledTriangulation, pybind11::const_))
        .def("slopeEquations", &SnapPeaTriangulation::slopeEquations)
        .def("fundamentalGroupFilled",
            &SnapPeaTriangulation::fundamentalGroupFilled,
            pybind11::arg("simplifyPresentation") = true,
            pybind11::arg("fillingsMayAffectGenerators") = true,
            pybind11::arg("minimiseNumberOfGenerators") = true,
            pybind11::arg("tryHardToShortenRelators") = true,
            pybind11::return_value_policy::reference_internal)
        .def("homologyFilled", &SnapPeaTriangulation::homologyFilled,
            pybind11::return_value_policy::reference_internal)
        .def("protoCanonize", &SnapPeaTriangulation::protoCanonize)
        .def("protoCanonise", &SnapPeaTriangulation::protoCanonise)
        .def("canonize", &SnapPeaTriangulation::canonize)
        .def("canonise", &SnapPeaTriangulation::canonise)
        .def("randomize", &SnapPeaTriangulation::randomize)
        .def("randomise", &SnapPeaTriangulation::randomise)
        .def_static("kernelMessagesEnabled",
            &SnapPeaTriangulation::kernelMessagesEnabled)
        .def_static("enableKernelMessages",
            &SnapPeaTriangulation::enableKernelMessages,
            pybind11::arg("enabled") = true)
        .def_static("disableKernelMessages",
            &SnapPeaTriangulation::disableKernelMessages)
        .def_property_readonly_static("typeID", [](pybind11::object) {
            // We cannot take the address of typeID, so use a getter function.
            return SnapPeaTriangulation::typeID;
        })
    ;

    auto st = pybind11::enum_<SnapPeaTriangulation::SolutionType>(
            c2, "SolutionType")
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
        .export_values()
    ;

    m.attr("NCusp") = m.attr("Cusp");
    m.attr("NSnapPeaTriangulation") = m.attr("SnapPeaTriangulation");

    // For backward compatibility with the old boost.python bindings:
    m.attr("SolutionType") = st;
}

