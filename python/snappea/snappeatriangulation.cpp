
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
#include "../helpers/exception.h"
#include "../docstrings/snappea/snappeatriangulation.h"
#include "../docstrings/triangulation/detail/triangulation.h" // for global_swap

using pybind11::overload_cast;
using regina::Cusp;
using regina::SnapPeaTriangulation;
using regina::Triangulation;

void addSnapPeaTriangulation(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN_MAIN

    regina::python::registerReginaException<regina::SnapPeaFatalError>(m,
        "SnapPeaFatalError", rdoc::SnapPeaFatalError);
    regina::python::registerReginaException<regina::SnapPeaMemoryFull>(m,
        "SnapPeaMemoryFull", rdoc::SnapPeaMemoryFull);

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
                    SnapPeaTriangulation::Cover)>&>,
            pybind11::arg("sheets"),
            pybind11::arg("type"),
            pybind11::arg("action"),
            rdoc::enumerateCovers)
        .def("enumerateCovers", [](const SnapPeaTriangulation& tri,
                int sheets, SnapPeaTriangulation::CoverEnumeration type) {
            pybind11::list ans;
            tri.enumerateCovers(sheets, type, [&](SnapPeaTriangulation&& c,
                    SnapPeaTriangulation::Cover t) {
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

    RDOC_SCOPE_INNER_BEGIN(Solution)

    auto st = pybind11::enum_<SnapPeaTriangulation::Solution>(c2, "Solution",
            rdoc_inner_scope)
        .value("NotAttempted", SnapPeaTriangulation::Solution::NotAttempted,
            rdoc_inner::NotAttempted)
        .value("Geometric", SnapPeaTriangulation::Solution::Geometric,
            rdoc_inner::Geometric)
        .value("Nongeometric", SnapPeaTriangulation::Solution::Nongeometric,
            rdoc_inner::Nongeometric)
        .value("Flat", SnapPeaTriangulation::Solution::Flat, rdoc_inner::Flat)
        .value("Degenerate", SnapPeaTriangulation::Solution::Degenerate,
            rdoc_inner::Degenerate)
        .value("Other", SnapPeaTriangulation::Solution::Other,
            rdoc_inner::Other)
        // Note: we cannot use "None", since this is reserved in Python.
        .value("Nil", SnapPeaTriangulation::Solution::None, rdoc_inner::None)
        .value("External", SnapPeaTriangulation::Solution::External,
            rdoc_inner::External)
    ;

    // Deprecated type alias and constants:
    c2.attr("SolutionType") = c2.attr("Solution");
    c2.attr("not_attempted") = SnapPeaTriangulation::Solution::NotAttempted;
    c2.attr("geometric_solution") = SnapPeaTriangulation::Solution::Geometric;
    c2.attr("nongeometric_solution") =
        SnapPeaTriangulation::Solution::Nongeometric;
    c2.attr("flat_solution") = SnapPeaTriangulation::Solution::Flat;
    c2.attr("degenerate_solution") = SnapPeaTriangulation::Solution::Degenerate;
    c2.attr("other_solution") = SnapPeaTriangulation::Solution::Other;
    c2.attr("no_solution") = SnapPeaTriangulation::Solution::None;
    c2.attr("externally_computed") = SnapPeaTriangulation::Solution::External;

    RDOC_SCOPE_INNER_SWITCH(CoverEnumeration)

    auto cet = pybind11::enum_<SnapPeaTriangulation::CoverEnumeration>(
            c2, "CoverEnumeration", rdoc_inner_scope)
        .value("Cyclic", SnapPeaTriangulation::CoverEnumeration::Cyclic,
            rdoc_inner::Cyclic)
        .value("All", SnapPeaTriangulation::CoverEnumeration::All,
            rdoc_inner::All)
    ;

    // Deprecated type alias and constants:
    c2.attr("CoverEnumerationType") = c2.attr("CoverEnumeration");
    c2.attr("cyclic_covers") = SnapPeaTriangulation::CoverEnumeration::Cyclic;
    c2.attr("all_covers") = SnapPeaTriangulation::CoverEnumeration::All;

    RDOC_SCOPE_INNER_SWITCH(Cover)

    auto ct = pybind11::enum_<SnapPeaTriangulation::Cover>(c2, "Cover",
            rdoc_inner_scope)
        .value("Unknown", SnapPeaTriangulation::Cover::Unknown,
            rdoc_inner::Unknown)
        .value("Irregular", SnapPeaTriangulation::Cover::Irregular,
            rdoc_inner::Irregular)
        .value("Regular", SnapPeaTriangulation::Cover::Regular,
            rdoc_inner::Regular)
        .value("Cyclic", SnapPeaTriangulation::Cover::Cyclic,
            rdoc_inner::Cyclic)
    ;

    // Deprecated type alias and constants:
    c2.attr("CoverType") = c2.attr("Cover");
    c2.attr("unknown_cover") = SnapPeaTriangulation::Cover::Unknown;
    c2.attr("irregular_cover") = SnapPeaTriangulation::Cover::Irregular;
    c2.attr("regular_cover") = SnapPeaTriangulation::Cover::Regular;
    c2.attr("cyclic_cover") = SnapPeaTriangulation::Cover::Cyclic;

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

