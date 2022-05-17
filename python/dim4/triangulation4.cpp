
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
#include "algebra/intersectionform.h"
#include "progress/progresstracker.h"
#include "triangulation/dim4.h"
#include "triangulation/facetpairing.h"
#include "triangulation/isosigtype.h"
#include "triangulation/detail/isosig-impl.h"
#include "../generic/facehelper.h"
#include "../generic/isosig-bindings.h"

using pybind11::overload_cast;
using regina::Isomorphism;
using regina::MatrixInt;
using regina::Triangulation;

void addTriangulation4(pybind11::module_& m) {
    auto c = pybind11::class_<Triangulation<4>,
            std::shared_ptr<Triangulation<4>>>(m, "Triangulation4")
        .def(pybind11::init<>())
        .def(pybind11::init<const Triangulation<4>&>())
        .def(pybind11::init<const Triangulation<4>&, bool>())
        .def(pybind11::init<const std::string&>())
        .def("isReadOnlySnapshot", &Triangulation<4>::isReadOnlySnapshot)
        .def("size", &Triangulation<4>::size)
        .def("countPentachora", &Triangulation<4>::countPentachora)
        .def("pentachora", &Triangulation<4>::pentachora,
            pybind11::keep_alive<0, 1>())
        .def("simplices", &Triangulation<4>::simplices,
            pybind11::keep_alive<0, 1>())
        // Use a C-style cast because GCC struggles with the overload_cast here:
        .def("pentachoron", (regina::Simplex<4>* (Triangulation<4>::*)(size_t))(
            &Triangulation<4>::pentachoron),
            pybind11::return_value_policy::reference_internal)
        .def("simplex",
            overload_cast<size_t>(&Triangulation<4>::simplex),
            pybind11::return_value_policy::reference_internal)
        .def("newPentachoron", overload_cast<>(
            &Triangulation<4>::newPentachoron),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplex", overload_cast<>(
            &Triangulation<4>::newSimplex),
            pybind11::return_value_policy::reference_internal)
        .def("newPentachoron", overload_cast<const std::string&>(
            &Triangulation<4>::newPentachoron),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplex", overload_cast<const std::string&>(
            &Triangulation<4>::newSimplex),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplices", [](Triangulation<4>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal)
        .def("newPentachora", [](Triangulation<4>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal)
        .def("removePentachoron", &Triangulation<4>::removePentachoron)
        .def("removeSimplex", &Triangulation<4>::removeSimplex)
        .def("removePentachoronAt", &Triangulation<4>::removePentachoronAt)
        .def("removeSimplexAt", &Triangulation<4>::removeSimplexAt)
        .def("removeAllPentachora", &Triangulation<4>::removeAllPentachora)
        .def("removeAllSimplices", &Triangulation<4>::removeAllSimplices)
        .def("swap", &Triangulation<4>::swap)
        .def("moveContentsTo", &Triangulation<4>::moveContentsTo)
        .def("countComponents", &Triangulation<4>::countComponents)
        .def("countBoundaryComponents",
            &Triangulation<4>::countBoundaryComponents)
        .def("countFaces", (regina::python::countFacesFunc<Triangulation<4>>)(
            &Triangulation<4>::countFaces))
        .def("countVertices", &Triangulation<4>::countVertices)
        .def("countEdges", &Triangulation<4>::countEdges)
        .def("countTriangles", &Triangulation<4>::countTriangles)
        .def("countTetrahedra", &Triangulation<4>::countTetrahedra)
        .def("fVector", &Triangulation<4>::fVector)
        .def("components", &Triangulation<4>::components,
            pybind11::keep_alive<0, 1>())
        .def("boundaryComponents", &Triangulation<4>::boundaryComponents,
            pybind11::keep_alive<0, 1>())
        .def("faces", (regina::python::facesFunc<Triangulation<4>>)(
            &Triangulation<4>::faces),
            pybind11::keep_alive<0, 1>())
        .def("vertices", &Triangulation<4>::vertices,
            pybind11::keep_alive<0, 1>())
        .def("edges", &Triangulation<4>::edges,
            pybind11::keep_alive<0, 1>())
        .def("triangles", &Triangulation<4>::triangles,
            pybind11::keep_alive<0, 1>())
        .def("tetrahedra", &Triangulation<4>::tetrahedra,
            pybind11::keep_alive<0, 1>())
        .def("component", &Triangulation<4>::component,
            pybind11::return_value_policy::reference_internal)
        .def("boundaryComponent", &Triangulation<4>::boundaryComponent,
            pybind11::return_value_policy::reference_internal)
        .def("face", (regina::python::faceFunc<Triangulation<4>>)(
            &Triangulation<4>::face),
            pybind11::return_value_policy::reference_internal)
        .def("vertex", &Triangulation<4>::vertex,
            pybind11::return_value_policy::reference_internal)
        .def("edge", &Triangulation<4>::edge,
            pybind11::return_value_policy::reference_internal)
        // Use C-style casts because GCC struggles with overload_cast here:
        .def("triangle", (regina::Face<4, 2>* (Triangulation<4>::*)(size_t))(
            &Triangulation<4>::triangle),
            pybind11::return_value_policy::reference_internal)
        .def("tetrahedron", (regina::Face<4, 3>* (Triangulation<4>::*)(size_t))(
            &Triangulation<4>::tetrahedron),
            pybind11::return_value_policy::reference_internal)
        .def("pairing", &Triangulation<4>::pairing)
        .def("isIsomorphicTo", &Triangulation<4>::isIsomorphicTo)
        .def("makeCanonical", &Triangulation<4>::makeCanonical)
        .def("isContainedIn", &Triangulation<4>::isContainedIn)
        .def("findAllIsomorphisms", &Triangulation<4>::findAllIsomorphisms<
                const std::function<bool(const Isomorphism<4>)>&>)
        .def("findAllIsomorphisms", [](const Triangulation<4>& t,
                const Triangulation<4>& other) {
            std::vector<Isomorphism<4>> isos;
            t.findAllIsomorphisms(other, [&](const Isomorphism<4>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        })
        .def("findAllSubcomplexesIn", &Triangulation<4>::findAllSubcomplexesIn<
                const std::function<bool(const Isomorphism<4>)>&>)
        .def("findAllSubcomplexesIn", [](const Triangulation<4>& t,
                const Triangulation<4>& other) {
            std::vector<Isomorphism<4>> isos;
            t.findAllSubcomplexesIn(other, [&](const Isomorphism<4>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        })
        .def("isEmpty", &Triangulation<4>::isEmpty)
        .def("eulerCharTri", &Triangulation<4>::eulerCharTri)
        .def("eulerCharManifold", &Triangulation<4>::eulerCharManifold)
        .def("isValid", &Triangulation<4>::isValid)
        .def("isIdeal", &Triangulation<4>::isIdeal)
        .def("hasBoundaryFacets", &Triangulation<4>::hasBoundaryFacets)
        .def("hasBoundaryTetrahedra", &Triangulation<4>::hasBoundaryTetrahedra)
        .def("countBoundaryFacets", &Triangulation<4>::countBoundaryFacets)
        .def("countBoundaryTetrahedra",
            &Triangulation<4>::countBoundaryTetrahedra)
        .def("countBoundaryFaces", (size_t (Triangulation<4>::*)(int) const)(
            &Triangulation<4>::countBoundaryFaces))
        .def("isClosed", &Triangulation<4>::isClosed)
        .def("isOrientable", &Triangulation<4>::isOrientable)
        .def("isOriented", &Triangulation<4>::isOriented)
        .def("isConnected", &Triangulation<4>::isConnected)
        .def("fundamentalGroup", &Triangulation<4>::fundamentalGroup,
            pybind11::return_value_policy::reference_internal)
        .def("simplifiedFundamentalGroup",
            &Triangulation<4>::simplifiedFundamentalGroup)
        .def("homology",
            (regina::AbelianGroup (Triangulation<4>::*)(int) const)(
            &Triangulation<4>::homology),
            pybind11::arg("k") = 1)
        .def("markedHomology",
            (regina::MarkedAbelianGroup (Triangulation<4>::*)(int) const)(
            &Triangulation<4>::markedHomology),
            pybind11::arg("k") = 1)
        .def("boundaryMap", (MatrixInt (Triangulation<4>::*)(int) const)(
            &Triangulation<4>::boundaryMap))
        .def("dualBoundaryMap", (MatrixInt (Triangulation<4>::*)(int) const)(
            &Triangulation<4>::dualBoundaryMap))
        .def("dualToPrimal", (MatrixInt (Triangulation<4>::*)(int) const)(
            &Triangulation<4>::dualToPrimal))
        .def("intersectionForm", &Triangulation<4>::intersectionForm)
        .def("orient", &Triangulation<4>::orient)
        .def("reflect", &Triangulation<4>::reflect)
        .def("triangulateComponents", &Triangulation<4>::triangulateComponents)
        .def("intelligentSimplify", &Triangulation<4>::intelligentSimplify)
        .def("simplifyToLocalMinimum",
            &Triangulation<4>::simplifyToLocalMinimum,
            pybind11::arg("perform") = true)
        .def("simplifyExhaustive", &Triangulation<4>::simplifyExhaustive,
            pybind11::arg("height") = 1,
            pybind11::arg("nThreads") = 1,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("retriangulate", [](const Triangulation<4>& tri, int height,
                int threads, const std::function<bool(const std::string&,
                    Triangulation<4>&&)>& action) {
            if (threads == 1) {
                return tri.retriangulate(height, 1, nullptr, action);
            } else {
                pybind11::gil_scoped_release release;
                return tri.retriangulate(height, threads, nullptr,
                    [&](const std::string& sig, Triangulation<4>&& t) -> bool {
                        pybind11::gil_scoped_acquire acquire;
                        return action(sig, std::move(t));
                    });
            }
        })
        .def("pachner", &Triangulation<4>::pachner<4>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("pachner", &Triangulation<4>::pachner<3>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("pachner", &Triangulation<4>::pachner<2>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("pachner", &Triangulation<4>::pachner<1>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("pachner", &Triangulation<4>::pachner<0>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("twoZeroMove",
            overload_cast<regina::Triangle<4>*, bool, bool>(
            &Triangulation<4>::twoZeroMove),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("twoZeroMove",
            overload_cast<regina::Edge<4>*, bool, bool>(
            &Triangulation<4>::twoZeroMove),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("twoZeroMove",
            overload_cast<regina::Vertex<4>*, bool, bool>(
            &Triangulation<4>::twoZeroMove),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("fourFourMove", &Triangulation<4>::fourFourMove,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true )
        .def("openBook", &Triangulation<4>::openBook,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("shellBoundary", &Triangulation<4>::shellBoundary,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("collapseEdge", &Triangulation<4>::collapseEdge,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("finiteToIdeal", &Triangulation<4>::finiteToIdeal)
        .def("makeDoubleCover", &Triangulation<4>::makeDoubleCover)
        .def("barycentricSubdivision",
            &Triangulation<4>::barycentricSubdivision)
        .def("idealToFinite", &Triangulation<4>::idealToFinite)
        .def("snapEdge", &Triangulation<4>::snapEdge,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true )
        .def("insertTriangulation", &Triangulation<4>::insertTriangulation)
        .def("isoSig", &Triangulation<4>::isoSig<>)
        .def("isoSig_EdgeDegrees",
            &Triangulation<4>::isoSig<regina::IsoSigEdgeDegrees<4>>)
        .def("isoSig_RidgeDegrees",
            &Triangulation<4>::isoSig<regina::IsoSigRidgeDegrees<4>>)
        .def("isoSigDetail", &Triangulation<4>::isoSigDetail<>)
        .def("isoSigDetail_EdgeDegrees",
            &Triangulation<4>::isoSigDetail<regina::IsoSigEdgeDegrees<4>>)
        .def("isoSigDetail_RidgeDegrees",
            &Triangulation<4>::isoSigDetail<regina::IsoSigRidgeDegrees<4>>)
        .def_static("fromIsoSig", &Triangulation<4>::fromIsoSig)
        .def_static("fromSig", &Triangulation<4>::fromSig)
        .def_static("isoSigComponentSize",
            &Triangulation<4>::isoSigComponentSize)
        .def("dumpConstruction", &Triangulation<4>::dumpConstruction)
        .def_readonly_static("dimension", &Triangulation<4>::dimension)
    ;
    regina::python::add_output(c);
    regina::python::packet_eq_operators(c);
    regina::python::add_packet_data(c);

    regina::python::addListView<decltype(Triangulation<4>().vertices())>(m);
    regina::python::addListView<decltype(Triangulation<4>().edges())>(m);
    regina::python::addListView<decltype(Triangulation<4>().triangles())>(m);
    regina::python::addListView<decltype(Triangulation<4>().tetrahedra())>(m);
    regina::python::addListView<decltype(Triangulation<4>().pentachora())>(m);
    regina::python::addListView<decltype(Triangulation<4>().components())>(m);
    regina::python::addListView<
        decltype(Triangulation<4>().boundaryComponents())>(m);

    auto wrap = regina::python::add_packet_wrapper<Triangulation<4>>(
        m, "PacketOfTriangulation4");
    regina::python::add_packet_constructor<>(wrap);
    regina::python::add_packet_constructor<const Triangulation<4>&, bool>(wrap);
    regina::python::add_packet_constructor<const std::string&>(wrap);

    m.def("swap",
        (void(*)(Triangulation<4>&, Triangulation<4>&))(regina::swap));

    addIsoSigClassic<4>(m, "IsoSigClassic4");
    addIsoSigEdgeDegrees<4>(m, "IsoSigEdgeDegrees4");
    addIsoSigRidgeDegrees<4>(m, "IsoSigRidgeDegrees4");
    addIsoSigPrintable<4>(m, "IsoSigPrintable4");
}

