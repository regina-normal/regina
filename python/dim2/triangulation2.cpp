
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
#include "triangulation/dim2.h"
#include "utilities/safeptr.h"
#include "../generic/facehelper.h"

using pybind11::overload_cast;
using regina::Triangulation;
using regina::detail::TriangulationBase;

void addTriangulation2(pybind11::module_& m) {
    auto c = pybind11::class_<Triangulation<2>, regina::Packet,
            regina::SafePtr<Triangulation<2>>>(m, "Triangulation2")
        .def(pybind11::init<>())
        .def(pybind11::init<const Triangulation<2>&>())
        .def(pybind11::init<const Triangulation<2>&, bool>())
        .def(pybind11::init<const std::string&>())
        .def("size", &Triangulation<2>::size)
        .def("countTriangles", &Triangulation<2>::countTriangles)
        .def("triangles", &Triangulation<2>::triangles,
            pybind11::return_value_policy::reference_internal)
        .def("simplices", &Triangulation<2>::simplices,
            pybind11::return_value_policy::reference_internal)
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
        .def("removeTriangle", &Triangulation<2>::removeTriangle)
        .def("removeSimplex", &Triangulation<2>::removeSimplex)
        .def("removeTriangleAt", &Triangulation<2>::removeTriangleAt)
        .def("removeSimplexAt", &Triangulation<2>::removeSimplexAt)
        .def("removeAllTriangles", &Triangulation<2>::removeAllTriangles)
        .def("removeAllSimplices", &Triangulation<2>::removeAllSimplices)
        .def("swapContents", &Triangulation<2>::swapContents)
        .def("moveContentsTo", &Triangulation<2>::moveContentsTo)
        .def("countComponents", &Triangulation<2>::countComponents)
        .def("countBoundaryComponents",
            &Triangulation<2>::countBoundaryComponents)
        .def("countFaces", &regina::python::countFaces<Triangulation<2>, 2>)
        .def("countVertices", &Triangulation<2>::countVertices)
        .def("countEdges", &Triangulation<2>::countEdges)
        .def("fVector", &Triangulation<2>::fVector)
        .def("components", &Triangulation<2>::components,
            pybind11::return_value_policy::reference_internal)
        .def("boundaryComponents", &Triangulation<2>::boundaryComponents,
            pybind11::return_value_policy::reference_internal)
        .def("faces", &regina::python::faces<Triangulation<2>, 2,
            pybind11::return_value_policy::reference_internal>)
        .def("vertices", &Triangulation<2>::vertices,
            pybind11::return_value_policy::reference_internal)
        .def("edges", &Triangulation<2>::edges,
            pybind11::return_value_policy::reference_internal)
        .def("component", &Triangulation<2>::component,
            pybind11::return_value_policy::reference_internal)
        .def("boundaryComponent", &Triangulation<2>::boundaryComponent,
            pybind11::return_value_policy::reference_internal)
        .def("face", &regina::python::face<Triangulation<2>, 2, size_t,
            pybind11::return_value_policy::reference_internal>)
        .def("vertex", &Triangulation<2>::vertex,
            pybind11::return_value_policy::reference_internal)
        .def("edge", &Triangulation<2>::edge,
            pybind11::return_value_policy::reference_internal)
        .def("isIdenticalTo", &Triangulation<2>::isIdenticalTo)
        .def("isIsomorphicTo", [](const Triangulation<2>& t,
                const Triangulation<2>& s) {
            return t.isIsomorphicTo(s).release();
        })
        .def("findAllIsomorphisms", [](const Triangulation<2>& t,
                const Triangulation<2>& other) {
            std::list<regina::Isomorphism<2>*> isos;
            t.findAllIsomorphisms(other, back_inserter(isos));
            return isos;
        })
        .def("makeCanonical", &Triangulation<2>::makeCanonical)
        .def("isContainedIn", [](const Triangulation<2>& t,
                const Triangulation<2>& s) {
            return t.isContainedIn(s).release();
        })
        .def("findAllSubcomplexesIn", [](const Triangulation<2>& t,
                const Triangulation<2>& other) {
            std::list<regina::Isomorphism<2>*> isos;
            t.findAllSubcomplexesIn(other, back_inserter(isos));
            return isos;
        })
        .def("isEmpty", &Triangulation<2>::isEmpty)
        .def("isValid", &Triangulation<2>::isValid)
        .def("eulerChar", &Triangulation<2>::eulerChar)
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
        .def("splitIntoComponents", &Triangulation<2>::splitIntoComponents,
            pybind11::arg("componentParent") = nullptr,
            pybind11::arg("setLabels") = true)
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
        .def("oneThreeMove", &Triangulation<2>::oneThreeMove,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("twoTwoMove", &Triangulation<2>::pachner<1>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("threeOneMove", &Triangulation<2>::pachner<0>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("finiteToIdeal", &Triangulation<2>::finiteToIdeal)
        .def("makeDoubleCover", &Triangulation<2>::makeDoubleCover)
        .def("barycentricSubdivision",
            &Triangulation<2>::barycentricSubdivision)
        .def("insertTriangulation", &Triangulation<2>::insertTriangulation)
        .def("isoSig", [](const Triangulation<2>& t) {
            return t.isoSig();
        })
        .def("isoSigDetail", [](const Triangulation<2>& t) {
            regina::Isomorphism<2>* iso;
            std::string sig = t.isoSig(&iso);
            return pybind11::make_tuple(sig, iso);
        })
        .def_static("fromIsoSig", &Triangulation<2>::fromIsoSig)
        .def_static("isoSigComponentSize",
            &Triangulation<2>::isoSigComponentSize)
        .def("dumpConstruction", &Triangulation<2>::dumpConstruction)
        // We cannot take the addresses of the following properties, so we
        // define getter functions instead.
        .def_property_readonly_static("typeID", [](pybind11::object) {
            return Triangulation<2>::typeID;
        })
        .def_property_readonly_static("dimension", [](pybind11::object) {
            return 2;
        })
    ;

    m.attr("Dim2Triangulation") = m.attr("Triangulation2");
}

