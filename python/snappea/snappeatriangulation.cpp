
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

#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "link/link.h"
#include "maths/matrix.h"
#include "snappea/snappeatriangulation.h"
#include "../helpers.h"
#include "../docstrings/snappea/snappeatriangulation.h"
#include "../docstrings/triangulation/detail/triangulation.h" // for global_swap

using pybind11::overload_cast;
using regina::Cusp;
using regina::SnapPeaTriangulation;
using regina::Triangulation;

void addSnapPeaTriangulation(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    pybind11::register_exception<regina::SnapPeaFatalError>(m,
        "SnapPeaFatalError", PyExc_RuntimeError)
        .doc() = rdoc::SnapPeaFatalError;
    pybind11::register_exception<regina::SnapPeaMemoryFull>(m,
        "SnapPeaMemoryFull", PyExc_RuntimeError)
        .doc() = rdoc::SnapPeaMemoryFull;

    RDOC_SCOPE_SWITCH(Cusp)

    auto c1 = pybind11::class_<Cusp>(m, "Cusp", rdoc_scope)
        .def(pybind11::init<const Cusp&>(), rdoc::__copy)
        .def("vertex", &Cusp::vertex,
            pybind11::return_value_policy::reference, rdoc::vertex)
        .def("complete", &Cusp::complete, rdoc::complete)
        .def("m", &Cusp::m, rdoc::m)
        .def("l", &Cusp::l, rdoc::l)
    ;
    regina::python::add_output(c1);
    regina::python::add_eq_operators(c1, rdoc::__eq);

    RDOC_SCOPE_SWITCH(SnapPeaTriangulation)

    auto c2 = pybind11::class_<SnapPeaTriangulation, regina::Triangulation<3>,
            std::shared_ptr<SnapPeaTriangulation>>(
            m, "SnapPeaTriangulation", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const std::string&>(), rdoc::__init)
        .def(pybind11::init<const SnapPeaTriangulation&>(), rdoc::__copy)
        .def(pybind11::init<const Triangulation<3>&, bool>(),
            pybind11::arg(), pybind11::arg("ignored") = false,
            rdoc::__init_2)
        .def(pybind11::init<const regina::Link&>(), rdoc::__init_3)
        .def("swap", &SnapPeaTriangulation::swap, rdoc::swap)
        .def("nullify", &SnapPeaTriangulation::nullify, rdoc::nullify)
        .def("isNull", &SnapPeaTriangulation::isNull, rdoc::isNull)
        .def("name", &SnapPeaTriangulation::name, rdoc::name)
        .def("solutionType", &SnapPeaTriangulation::solutionType,
            rdoc::solutionType)
        .def("volume", &SnapPeaTriangulation::volume, rdoc::volume)
        .def("volumeWithPrecision", &SnapPeaTriangulation::volumeWithPrecision,
            rdoc::volumeWithPrecision)
        .def("volumeZero", &SnapPeaTriangulation::volumeZero, rdoc::volumeZero)
        .def("shape", &SnapPeaTriangulation::shape, rdoc::shape)
        .def("minImaginaryShape", &SnapPeaTriangulation::minImaginaryShape,
            rdoc::minImaginaryShape)
        .def("gluingEquations", &SnapPeaTriangulation::gluingEquations,
            rdoc::gluingEquations)
        .def("gluingEquationsRect",
            &SnapPeaTriangulation::gluingEquationsRect,
            rdoc::gluingEquationsRect)
        .def("countCusps", &SnapPeaTriangulation::countCusps, rdoc::countCusps)
        .def("countCompleteCusps",
            &SnapPeaTriangulation::countCompleteCusps, rdoc::countCompleteCusps)
        .def("countFilledCusps", &SnapPeaTriangulation::countFilledCusps,
            rdoc::countFilledCusps)
        .def("cusp", &SnapPeaTriangulation::cusp,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("whichCusp") = 0,
            rdoc::cusp)
        .def("cusps", &SnapPeaTriangulation::cusps,
            pybind11::keep_alive<0, 1>(), rdoc::cusps)
        .def("fill", &SnapPeaTriangulation::fill,
            pybind11::arg(), pybind11::arg(), pybind11::arg("whichCusp")= 0,
            rdoc::fill)
        .def("unfill", &SnapPeaTriangulation::unfill,
            pybind11::arg("whichCusp")= 0, rdoc::unfill)
        .def("filledPartial", overload_cast<unsigned>(
            &SnapPeaTriangulation::filledPartial, pybind11::const_),
            rdoc::filledPartial)
        .def("filledPartial", overload_cast<>(
            &SnapPeaTriangulation::filledPartial, pybind11::const_),
            rdoc::filledPartial_2)
        .def("filledAll", &SnapPeaTriangulation::filledAll, rdoc::filledAll)
        .def("slopeEquations", &SnapPeaTriangulation::slopeEquations,
            rdoc::slopeEquations)
        .def("fundamentalGroupFilled",
            &SnapPeaTriangulation::fundamentalGroupFilled,
            pybind11::arg("simplifyPresentation") = true,
            pybind11::arg("fillingsMayAffectGenerators") = true,
            pybind11::arg("minimiseNumberOfGenerators") = true,
            pybind11::arg("tryHardToShortenRelators") = true,
            pybind11::return_value_policy::reference_internal,
            rdoc::fundamentalGroupFilled)
        .def("homologyFilled", &SnapPeaTriangulation::homologyFilled,
            pybind11::return_value_policy::reference_internal,
            rdoc::homologyFilled)
        .def("protoCanonize", &SnapPeaTriangulation::protoCanonize,
            rdoc::protoCanonize)
        .def("protoCanonise", &SnapPeaTriangulation::protoCanonise,
            rdoc::protoCanonise)
        .def("canonize", &SnapPeaTriangulation::canonize, rdoc::canonize)
        .def("canonise", &SnapPeaTriangulation::canonise, rdoc::canonise)
        .def("randomize", &SnapPeaTriangulation::randomize, rdoc::randomize)
        .def("randomise", &SnapPeaTriangulation::randomise, rdoc::randomise)
        .def("enumerateCovers", &SnapPeaTriangulation::enumerateCovers<
                const std::function<void(SnapPeaTriangulation&&,
                    SnapPeaTriangulation::CoverType)>&>,
            pybind11::arg("sheets"),
            pybind11::arg("type"),
            pybind11::arg("action"),
            rdoc::enumerateCovers)
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
        },
            pybind11::arg("sheets"), pybind11::arg("type"),
            rdoc::enumerateCovers)
        .def_static("kernelMessagesEnabled",
            &SnapPeaTriangulation::kernelMessagesEnabled,
            rdoc::kernelMessagesEnabled)
        .def_static("enableKernelMessages",
            &SnapPeaTriangulation::enableKernelMessages,
            pybind11::arg("enabled") = true,
            rdoc::enableKernelMessages)
        .def_static("disableKernelMessages",
            &SnapPeaTriangulation::disableKernelMessages,
            rdoc::disableKernelMessages)
        .def("snapPea", overload_cast<>(
            &SnapPeaTriangulation::snapPea, pybind11::const_), rdoc::snapPea)
        .def("saveSnapPea", &SnapPeaTriangulation::saveSnapPea,
            rdoc::saveSnapPea)
    ;
    // SnapPeaTriangulation overrides the output routines; make sure we do not
    // get left with the inherited versions from Triangulation<3>.
    regina::python::add_output(c2);
    regina::python::add_packet_data(c2);
    regina::python::packet_eq_operators(c2, rdoc::__eq);

    regina::python::addListView<decltype(SnapPeaTriangulation().cusps())>(m);

    auto wrap = regina::python::add_packet_wrapper<SnapPeaTriangulation>(
        m, "PacketOfSnapPeaTriangulation");
    regina::python::add_packet_constructor<>(wrap, rdoc::__default);
    regina::python::add_packet_constructor<const std::string&>(wrap,
        rdoc::__init);
    regina::python::add_packet_constructor<const Triangulation<3>&, bool>(wrap,
        pybind11::arg(), pybind11::arg("ignored") = false,
        rdoc::__init_2);
    regina::python::add_packet_constructor<const regina::Link&>(wrap,
        rdoc::__init_3);

    RDOC_SCOPE_INNER_BEGIN(SolutionType)

    auto st = pybind11::enum_<SnapPeaTriangulation::SolutionType>(
            c2, "SolutionType", rdoc_inner_scope)
        .value("not_attempted", SnapPeaTriangulation::not_attempted,
            rdoc_inner::not_attempted)
        .value("geometric_solution",
            SnapPeaTriangulation::geometric_solution,
                rdoc_inner::geometric_solution)
        .value("nongeometric_solution",
            SnapPeaTriangulation::nongeometric_solution,
                rdoc_inner::nongeometric_solution)
        .value("flat_solution", SnapPeaTriangulation::flat_solution,
            rdoc_inner::flat_solution)
        .value("degenerate_solution",
            SnapPeaTriangulation::degenerate_solution,
                rdoc_inner::degenerate_solution)
        .value("other_solution", SnapPeaTriangulation::other_solution,
            rdoc_inner::other_solution)
        .value("no_solution", SnapPeaTriangulation::no_solution,
            rdoc_inner::no_solution)
        .value("externally_computed",
            SnapPeaTriangulation::externally_computed,
                rdoc_inner::externally_computed)
        .export_values()
    ;

    // For backward compatibility with the old boost.python bindings:
    m.attr("SolutionType") = st;

    RDOC_SCOPE_INNER_SWITCH(CoverEnumerationType)

    auto cet = pybind11::enum_<SnapPeaTriangulation::CoverEnumerationType>(
            c2, "CoverEnumerationType", rdoc_inner_scope)
        .value("cyclic_covers", SnapPeaTriangulation::cyclic_covers,
            rdoc_inner::cyclic_covers)
        .value("all_covers", SnapPeaTriangulation::all_covers,
            rdoc_inner::all_covers)
        .export_values()
    ;

    RDOC_SCOPE_INNER_SWITCH(CoverType)

    auto ct = pybind11::enum_<SnapPeaTriangulation::CoverType>(c2, "CoverType",
            rdoc_inner_scope)
        .value("unknown_cover", SnapPeaTriangulation::unknown_cover,
            rdoc_inner::unknown_cover)
        .value("irregular_cover", SnapPeaTriangulation::irregular_cover,
            rdoc_inner::irregular_cover)
        .value("regular_cover", SnapPeaTriangulation::regular_cover,
            rdoc_inner::regular_cover)
        .value("cyclic_cover", SnapPeaTriangulation::cyclic_cover,
            rdoc_inner::cyclic_cover)
        .export_values()
    ;

    RDOC_SCOPE_INNER_END

    regina::python::add_global_swap<SnapPeaTriangulation>(m, rdoc::global_swap);

    RDOC_SCOPE_SWITCH_MAIN

    // Now we can define the global swap for the parent Triangulation<3> class.
    // See the notes beneath the Triangulation<3> bindings as to why this had
    // to wait until *after* the SnapPeaTriangulation swap.
    regina::python::add_global_swap<Triangulation<3>>(m,
        rdoc::Triangulation_::global_swap);

    RDOC_SCOPE_END
}

