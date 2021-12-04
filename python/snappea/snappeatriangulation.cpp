
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
#include "../pybind11/functional.h"
#include "../pybind11/stl.h"
#include "link/link.h"
#include "maths/matrix.h"
#include "snappea/snappeatriangulation.h"
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
            std::shared_ptr<SnapPeaTriangulation>>(
            m, "SnapPeaTriangulation")
        .def(pybind11::init<>())
        .def(pybind11::init<const std::string&>())
        .def(pybind11::init<const SnapPeaTriangulation&>())
        .def(pybind11::init<const Triangulation<3>&, bool>(),
            pybind11::arg(), pybind11::arg("ignored") = false)
        .def(pybind11::init<const regina::Link&>())
        .def("swap", &SnapPeaTriangulation::swap)
        .def("nullify", &SnapPeaTriangulation::nullify)
        .def("isNull", &SnapPeaTriangulation::isNull)
        .def("name", &SnapPeaTriangulation::name)
        .def("solutionType", &SnapPeaTriangulation::solutionType)
        .def("volume", &SnapPeaTriangulation::volume)
        .def("volumeWithPrecision", &SnapPeaTriangulation::volumeWithPrecision)
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
        .def("cusps", &SnapPeaTriangulation::cusps,
            pybind11::keep_alive<0, 1>())
        .def("fill", &SnapPeaTriangulation::fill,
            pybind11::arg(), pybind11::arg(), pybind11::arg("whichCusp")= 0)
        .def("unfill", &SnapPeaTriangulation::unfill,
            pybind11::arg("whichCusp")= 0)
        .def("filledPartial", overload_cast<>(
            &SnapPeaTriangulation::filledPartial, pybind11::const_))
        .def("filledPartial", overload_cast<unsigned>(
            &SnapPeaTriangulation::filledPartial, pybind11::const_))
        .def("filledAll", &SnapPeaTriangulation::filledAll)
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
        .def("enumerateCovers", &SnapPeaTriangulation::enumerateCovers<
                const std::function<void(SnapPeaTriangulation&&,
                    SnapPeaTriangulation::CoverType)>&>)
        .def("enumerateCovers", [](const SnapPeaTriangulation& tri,
                int sheets, SnapPeaTriangulation::CoverEnumerationType type) {
            pybind11::list ans;
            tri.enumerateCovers(sheets, type, [&](SnapPeaTriangulation&& c,
                    SnapPeaTriangulation::CoverType t) {
                pybind11::tuple pair(2);
                pair[0] = pybind11::cast(new SnapPeaTriangulation(std::move(c)));
                pair[1] = t;
                ans.append(pair);
            });
            return ans;
        })
        .def_static("kernelMessagesEnabled",
            &SnapPeaTriangulation::kernelMessagesEnabled)
        .def_static("enableKernelMessages",
            &SnapPeaTriangulation::enableKernelMessages,
            pybind11::arg("enabled") = true)
        .def_static("disableKernelMessages",
            &SnapPeaTriangulation::disableKernelMessages)
    ;
    // SnapPeaTriangulation overrides the output routines; make sure we do not
    // get left with the inherited versions from Triangulation<3>.
    regina::python::add_output(c2, regina::python::PYTHON_REPR_NONE);

    regina::python::addListView<decltype(SnapPeaTriangulation().cusps())>(m);

    auto wrap = regina::python::add_packet_wrapper<SnapPeaTriangulation>(
        m, "PacketOfSnapPeaTriangulation");
    regina::python::add_packet_constructor<>(wrap);
    regina::python::add_packet_constructor<const std::string&>(wrap);
    regina::python::add_packet_constructor<const Triangulation<3>&, bool>(wrap,
        pybind11::arg(), pybind11::arg("ignored") = false);
    regina::python::add_packet_constructor<const regina::Link&>(wrap);

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

    // For backward compatibility with the old boost.python bindings:
    m.attr("SolutionType") = st;

    auto cet = pybind11::enum_<SnapPeaTriangulation::CoverEnumerationType>(
            c2, "CoverEnumerationType")
        .value("cyclic_covers", SnapPeaTriangulation::cyclic_covers)
        .value("all_covers", SnapPeaTriangulation::all_covers)
        .export_values()
    ;

    auto ct = pybind11::enum_<SnapPeaTriangulation::CoverType>(c2, "CoverType")
        .value("unknown_cover", SnapPeaTriangulation::unknown_cover)
        .value("irregular_cover", SnapPeaTriangulation::irregular_cover)
        .value("regular_cover", SnapPeaTriangulation::regular_cover)
        .value("cyclic_cover", SnapPeaTriangulation::cyclic_cover)
        .export_values()
    ;

    m.def("swap",
        (void(*)(SnapPeaTriangulation&, SnapPeaTriangulation&))(regina::swap));

    // Now we can define the global swap for the parent Triangulation<3> class.
    // See the notes beneath the Triangulation<3> bindings as to why this had
    // to wait until *after* the SnapPeaTriangulation swap.
    m.def("swap",
        (void(*)(Triangulation<3>&, Triangulation<3>&))(regina::swap));
}

