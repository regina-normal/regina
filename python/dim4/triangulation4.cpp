
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
#include "../pybind11/stl.h"
#include "algebra/grouppresentation.h"
#include "progress/progresstracker.h"
#include "triangulation/dim4.h"
#include "utilities/safeptr.h"
#include "../generic/facehelper.h"

using pybind11::overload_cast;
using regina::Triangulation;
using regina::detail::TriangulationBase;

void addTriangulation4(pybind11::module_& m) {
    auto c = pybind11::class_<Triangulation<4>, regina::Packet,
            regina::SafePtr<Triangulation<4>>>(m, "Triangulation4")
        .def(pybind11::init<>())
        .def(pybind11::init<const Triangulation<4>&>())
        .def(pybind11::init<const Triangulation<4>&, bool>())
        .def(pybind11::init<const std::string&>())
        .def("size", &Triangulation<4>::size)
        .def("countPentachora", &Triangulation<4>::countPentachora)
        .def("pentachora", &Triangulation<4>::pentachora,
            pybind11::return_value_policy::reference_internal)
        .def("simplices", &Triangulation<4>::simplices,
            pybind11::return_value_policy::reference_internal)
        .def("pentachoron",
            overload_cast<size_t>(&Triangulation<4>::pentachoron),
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
        .def("removePentachoron", &Triangulation<4>::removePentachoron)
        .def("removeSimplex", &Triangulation<4>::removeSimplex)
        .def("removePentachoronAt", &Triangulation<4>::removePentachoronAt)
        .def("removeSimplexAt", &Triangulation<4>::removeSimplexAt)
        .def("removeAllPentachora", &Triangulation<4>::removeAllPentachora)
        .def("removeAllSimplices", &Triangulation<4>::removeAllSimplices)
        .def("swapContents", &Triangulation<4>::swapContents)
        .def("moveContentsTo", &Triangulation<4>::moveContentsTo)
        .def("countComponents", &Triangulation<4>::countComponents)
        .def("countBoundaryComponents",
            &Triangulation<4>::countBoundaryComponents)
        .def("countFaces", &regina::python::countFaces<Triangulation<4>, 4>)
        .def("countVertices", &Triangulation<4>::countVertices)
        .def("countEdges", &Triangulation<4>::countEdges)
        .def("countTriangles", &Triangulation<4>::countTriangles)
        .def("countTetrahedra", &Triangulation<4>::countTetrahedra)
        .def("fVector", &Triangulation<4>::fVector)
        .def("components", &Triangulation<4>::components,
            pybind11::return_value_policy::reference_internal)
        .def("boundaryComponents", &Triangulation<4>::boundaryComponents,
            pybind11::return_value_policy::reference_internal)
        .def("faces", &regina::python::faces<Triangulation<4>, 4,
            pybind11::return_value_policy::reference_internal>)
        .def("vertices", &Triangulation<4>::vertices,
            pybind11::return_value_policy::reference_internal)
        .def("edges", &Triangulation<4>::edges,
            pybind11::return_value_policy::reference_internal)
        .def("triangles", &Triangulation<4>::triangles,
            pybind11::return_value_policy::reference_internal)
        .def("tetrahedra", &Triangulation<4>::tetrahedra,
            pybind11::return_value_policy::reference_internal)
        .def("component", &Triangulation<4>::component,
            pybind11::return_value_policy::reference_internal)
        .def("boundaryComponent", &Triangulation<4>::boundaryComponent,
            pybind11::return_value_policy::reference_internal)
        .def("face", &regina::python::face<Triangulation<4>, 4, size_t,
            pybind11::return_value_policy::reference_internal>)
        .def("vertex", &Triangulation<4>::vertex,
            pybind11::return_value_policy::reference_internal)
        .def("edge", &Triangulation<4>::edge,
            pybind11::return_value_policy::reference_internal)
        .def("triangle", &Triangulation<4>::triangle,
            pybind11::return_value_policy::reference_internal)
        .def("tetrahedron", &Triangulation<4>::tetrahedron,
            pybind11::return_value_policy::reference_internal)
        .def("isIdenticalTo", &Triangulation<4>::isIdenticalTo)
        .def("isIsomorphicTo", [](const Triangulation<4>& t,
                const Triangulation<4>& s) {
            return t.isIsomorphicTo(s).release();
        })
        .def("makeCanonical", &Triangulation<4>::makeCanonical)
        .def("isContainedIn", [](const Triangulation<4>& t,
                const Triangulation<4>& s) {
            return t.isContainedIn(s).release();
        })
        .def("findAllIsomorphisms", [](const Triangulation<4>& t,
                const Triangulation<4>& other) {
            std::list<regina::Isomorphism<4>*> isos;
            t.findAllIsomorphisms(other, back_inserter(isos));
            return isos;
        })
        .def("findAllSubcomplexesIn", [](const Triangulation<4>& t,
                const Triangulation<4>& other) {
            std::list<regina::Isomorphism<4>*> isos;
            t.findAllSubcomplexesIn(other, back_inserter(isos));
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
        .def("isClosed", &Triangulation<4>::isClosed)
        .def("isOrientable", &Triangulation<4>::isOrientable)
        .def("isOriented", &Triangulation<4>::isOriented)
        .def("isConnected", &Triangulation<4>::isConnected)
        .def("fundamentalGroup", &Triangulation<4>::fundamentalGroup,
            pybind11::return_value_policy::reference_internal)
        .def("simplifiedFundamentalGroup", [](Triangulation<4>& t,
                const regina::GroupPresentation& group) {
            // Clone the given group to avoid claiming ownership of it.
            t.simplifiedFundamentalGroup(new regina::GroupPresentation(group));
        })
        .def("homology", &Triangulation<4>::homology,
            pybind11::return_value_policy::reference_internal)
        .def("homologyH1", &Triangulation<4>::homologyH1,
            pybind11::return_value_policy::reference_internal)
        .def("homologyH2", &Triangulation<4>::homologyH2,
            pybind11::return_value_policy::reference_internal)
        .def("orient", &Triangulation<4>::orient)
        .def("reflect", &Triangulation<4>::reflect)
        .def("splitIntoComponents", &Triangulation<4>::splitIntoComponents,
            pybind11::arg("componentParent") = nullptr,
            pybind11::arg("setLabels") = true)
        .def("intelligentSimplify", &Triangulation<4>::intelligentSimplify)
        .def("simplifyToLocalMinimum",
            &Triangulation<4>::simplifyToLocalMinimum,
            pybind11::arg("perform") = true)
        .def("simplifyExhaustive", &Triangulation<4>::simplifyExhaustive,
            pybind11::arg("height") = 1,
            pybind11::arg("nThreads") = 1,
            pybind11::arg("tracker") = nullptr)
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
        .def("oneFiveMove", &Triangulation<4>::oneFiveMove,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("twoFourMove", &Triangulation<4>::pachner<3>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("threeThreeMove", &Triangulation<4>::pachner<2>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("fourTwoMove", &Triangulation<4>::pachner<1>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("fiveOneMove", &Triangulation<4>::pachner<0>,
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
        .def("insertTriangulation", &Triangulation<4>::insertTriangulation)
        .def("isoSig", [](const Triangulation<4>& t) {
            return t.isoSig();
        })
        .def("isoSigDetail", [](const Triangulation<4>& t) {
            regina::Isomorphism<4>* iso;
            std::string sig = t.isoSig(&iso);
            return pybind11::make_tuple(sig, iso);
        })
        .def_static("fromIsoSig", &Triangulation<4>::fromIsoSig)
        .def_static("isoSigComponentSize",
            &Triangulation<4>::isoSigComponentSize)
        .def("dumpConstruction", &Triangulation<4>::dumpConstruction)
        // We cannot take the addresses of the following properties, so we
        // define getter functions instead.
        .def_property_readonly_static("typeID", [](pybind11::object) {
            return Triangulation<4>::typeID;
        })
        .def_property_readonly_static("dimension", [](pybind11::object) {
            return 4;
        })
    ;

    m.attr("Dim4Triangulation") = m.attr("Triangulation4");
}

