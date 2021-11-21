
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
#include "../pybind11/functional.h"
#include "../pybind11/stl.h"
#include "../helpers.h"
#include "algebra/grouppresentation.h"
#include "triangulation/dim2.h"
#include "triangulation/isosigtype.h"
#include "triangulation/detail/isosig-impl.h"
#include "../generic/facehelper.h"
#include "../generic/isosig-bindings.h"

using pybind11::overload_cast;
using regina::Isomorphism;
using regina::Triangulation;

void addTriangulation2(pybind11::module_& m) {
    auto c = pybind11::class_<Triangulation<2>,
            std::shared_ptr<Triangulation<2>>>(m, "Triangulation2")
        .def(pybind11::init<>())
        .def(pybind11::init<const Triangulation<2>&>())
        .def(pybind11::init<const Triangulation<2>&, bool>())
        .def(pybind11::init<const std::string&>())
        .def("isReadOnlySnapshot", &Triangulation<2>::isReadOnlySnapshot)
        .def("size", &Triangulation<2>::size)
        .def("countTriangles", &Triangulation<2>::countTriangles)
        .def("triangles", &Triangulation<2>::triangles,
            pybind11::keep_alive<0, 1>())
        .def("simplices", &Triangulation<2>::simplices,
            pybind11::keep_alive<0, 1>())
        .def("triangle",
            overload_cast<size_t>(&Triangulation<2>::triangle),
            pybind11::return_value_policy::reference_internal)
        .def("simplex",
            overload_cast<size_t>(&Triangulation<2>::simplex),
            pybind11::return_value_policy::reference_internal)
        .def("newTriangle", overload_cast<>(
            &Triangulation<2>::newTriangle),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplex", overload_cast<>(
            &Triangulation<2>::newSimplex),
            pybind11::return_value_policy::reference_internal)
        .def("newTriangle", overload_cast<const std::string&>(
            &Triangulation<2>::newTriangle),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplex", overload_cast<const std::string&>(
            &Triangulation<2>::newSimplex),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplices", [](Triangulation<2>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal)
        .def("newTriangles", [](Triangulation<2>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal)
        .def("removeTriangle", &Triangulation<2>::removeTriangle)
        .def("removeSimplex", &Triangulation<2>::removeSimplex)
        .def("removeTriangleAt", &Triangulation<2>::removeTriangleAt)
        .def("removeSimplexAt", &Triangulation<2>::removeSimplexAt)
        .def("removeAllTriangles", &Triangulation<2>::removeAllTriangles)
        .def("removeAllSimplices", &Triangulation<2>::removeAllSimplices)
        .def("swap", &Triangulation<2>::swap)
        .def("swapContents", &Triangulation<2>::swap) // deprecated
        .def("moveContentsTo", &Triangulation<2>::moveContentsTo)
        .def("countComponents", &Triangulation<2>::countComponents)
        .def("countBoundaryComponents",
            &Triangulation<2>::countBoundaryComponents)
        .def("countFaces", (regina::python::countFacesFunc<Triangulation<2>>)(
            &Triangulation<2>::countFaces))
        .def("countVertices", &Triangulation<2>::countVertices)
        .def("countEdges", &Triangulation<2>::countEdges)
        .def("fVector", &Triangulation<2>::fVector)
        .def("components", &Triangulation<2>::components,
            pybind11::keep_alive<0, 1>())
        .def("boundaryComponents", &Triangulation<2>::boundaryComponents,
            pybind11::keep_alive<0, 1>())
        .def("faces", (regina::python::facesFunc<Triangulation<2>>)(
            &Triangulation<2>::faces),
            pybind11::keep_alive<0, 1>())
        .def("vertices", &Triangulation<2>::vertices,
            pybind11::keep_alive<0, 1>())
        .def("edges", &Triangulation<2>::edges,
            pybind11::keep_alive<0, 1>())
        .def("component", &Triangulation<2>::component,
            pybind11::return_value_policy::reference_internal)
        .def("boundaryComponent", &Triangulation<2>::boundaryComponent,
            pybind11::return_value_policy::reference_internal)
        .def("face", (regina::python::faceFunc<Triangulation<2>>)(
            &Triangulation<2>::face),
            pybind11::return_value_policy::reference_internal)
        .def("vertex", &Triangulation<2>::vertex,
            pybind11::return_value_policy::reference_internal)
        .def("edge", &Triangulation<2>::edge,
            pybind11::return_value_policy::reference_internal)
        .def("isIdenticalTo", &Triangulation<2>::isIdenticalTo)
        .def("isIsomorphicTo", &Triangulation<2>::isIsomorphicTo)
        .def("findAllIsomorphisms", &Triangulation<2>::findAllIsomorphisms<
                const std::function<bool(const Isomorphism<2>)>&>)
        .def("findAllIsomorphisms", [](const Triangulation<2>& t,
                const Triangulation<2>& other) {
            std::vector<Isomorphism<2>> isos;
            t.findAllIsomorphisms(other, [&](const Isomorphism<2>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        })
        .def("makeCanonical", &Triangulation<2>::makeCanonical)
        .def("isContainedIn", &Triangulation<2>::isContainedIn)
        .def("findAllSubcomplexesIn", &Triangulation<2>::findAllSubcomplexesIn<
                const std::function<bool(const Isomorphism<2>)>&>)
        .def("findAllSubcomplexesIn", [](const Triangulation<2>& t,
                const Triangulation<2>& other) {
            std::vector<Isomorphism<2>> isos;
            t.findAllSubcomplexesIn(other, [&](const Isomorphism<2>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        })
        .def("isEmpty", &Triangulation<2>::isEmpty)
        .def("isValid", &Triangulation<2>::isValid)
        .def("eulerChar", &Triangulation<2>::eulerChar)
        .def("eulerCharTri", &Triangulation<2>::eulerCharTri)
        .def("isClosed", &Triangulation<2>::isClosed)
        .def("hasBoundaryFacets", &Triangulation<2>::hasBoundaryFacets)
        .def("hasBoundaryEdges", &Triangulation<2>::hasBoundaryEdges)
        .def("countBoundaryFacets", &Triangulation<2>::countBoundaryFacets)
        .def("countBoundaryEdges", &Triangulation<2>::countBoundaryEdges)
        .def("isOrientable", &Triangulation<2>::isOrientable)
        .def("isOriented", &Triangulation<2>::isOriented)
        .def("isIdeal", &Triangulation<2>::isIdeal)
        .def("isConnected", &Triangulation<2>::isConnected)
        .def("isMinimal", &Triangulation<2>::isMinimal)
        .def("orient", &Triangulation<2>::orient)
        .def("reflect", &Triangulation<2>::reflect)
        .def("triangulateComponents", &Triangulation<2>::triangulateComponents)
        .def("homology", &Triangulation<2>::homology,
            pybind11::return_value_policy::reference_internal)
        .def("homologyH1", &Triangulation<2>::homologyH1,
            pybind11::return_value_policy::reference_internal)
        .def("pachner", &Triangulation<2>::pachner<2>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("pachner", &Triangulation<2>::pachner<1>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("pachner", &Triangulation<2>::pachner<0>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("finiteToIdeal", &Triangulation<2>::finiteToIdeal)
        .def("makeDoubleCover", &Triangulation<2>::makeDoubleCover)
        .def("barycentricSubdivision",
            &Triangulation<2>::barycentricSubdivision)
        .def("insertTriangulation", &Triangulation<2>::insertTriangulation)
        .def("isoSig", &Triangulation<2>::isoSig<>)
        .def("isoSig_EdgeDegrees",
            &Triangulation<2>::isoSig<regina::IsoSigEdgeDegrees<2>>)
        .def("isoSigDetail", &Triangulation<2>::isoSigDetail<>)
        .def("isoSigDetail_EdgeDegrees",
            &Triangulation<2>::isoSigDetail<regina::IsoSigEdgeDegrees<2>>)
        .def_static("fromIsoSig", &Triangulation<2>::fromIsoSig)
        .def_static("fromSig", &Triangulation<2>::fromSig)
        .def_static("isoSigComponentSize",
            &Triangulation<2>::isoSigComponentSize)
        .def("dumpConstruction", &Triangulation<2>::dumpConstruction)
        .def_readonly_static("dimension", &Triangulation<2>::dimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    regina::python::addListView<decltype(Triangulation<2>().vertices())>(m);
    regina::python::addListView<decltype(Triangulation<2>().edges())>(m);
    regina::python::addListView<decltype(Triangulation<2>().triangles())>(m);
    regina::python::addListView<decltype(Triangulation<2>().components())>(m);
    regina::python::addListView<
        decltype(Triangulation<2>().boundaryComponents())>(m);

    auto wrap = regina::python::add_packet_wrapper<Triangulation<2>>(
        m, "PacketOfTriangulation2");
    regina::python::add_packet_constructor<>(wrap);
    regina::python::add_packet_constructor<const Triangulation<2>&, bool>(wrap);
    regina::python::add_packet_constructor<const std::string&>(wrap);

    m.def("swap",
        (void(*)(Triangulation<2>&, Triangulation<2>&))(regina::swap));

    addIsoSigClassic<2>(m, "IsoSigClassic2");
    addIsoSigEdgeDegrees<2>(m, "IsoSigEdgeDegrees2");
    addIsoSigPrintable<2>(m, "IsoSigPrintable2");
}

