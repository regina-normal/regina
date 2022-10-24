
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "triangulation/facetpairing.h"
#include "triangulation/isosigtype.h"
#include "triangulation/detail/isosig-impl.h"
#include "../generic/facehelper.h"
#include "../generic/isosig-bindings.h"
#include "../docstrings/triangulation/dim2/triangulation2.h"
#include "../docstrings/triangulation/detail/triangulation.h"
#include "../docstrings/utilities/snapshot.h"

using pybind11::overload_cast;
using regina::AbelianGroup;
using regina::Isomorphism;
using regina::MarkedAbelianGroup;
using regina::MatrixInt;
using regina::Triangulation;

void addTriangulation2(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Triangulation)
    RDOC_SCOPE_BASE_2(detail::TriangulationBase, Snapshottable)

    auto c = pybind11::class_<Triangulation<2>,
            std::shared_ptr<Triangulation<2>>>(m, "Triangulation2", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Triangulation<2>&>(), rdoc::__copy)
        .def(pybind11::init<const Triangulation<2>&, bool>(), rdoc::__init)
        .def(pybind11::init<const std::string&>(), rdoc::__init_2)
        .def("isReadOnlySnapshot", &Triangulation<2>::isReadOnlySnapshot,
            rbase2::isReadOnlySnapshot)
        .def("size", &Triangulation<2>::size, rbase::size)
        .def("countTriangles", &Triangulation<2>::countTriangles,
            rbase::countTriangles)
        .def("triangles", &Triangulation<2>::triangles,
            pybind11::keep_alive<0, 1>(), rbase::triangles)
        .def("simplices", &Triangulation<2>::simplices,
            pybind11::keep_alive<0, 1>(), rbase::simplices)
        // Use a static cast because GCC struggles with the overload_cast here:
        .def("triangle",
            static_cast<regina::Simplex<2>* (Triangulation<2>::*)(size_t)>(
                &Triangulation<2>::triangle),
            pybind11::return_value_policy::reference_internal, rbase::triangle)
        .def("simplex",
            overload_cast<size_t>(&Triangulation<2>::simplex),
            pybind11::return_value_policy::reference_internal, rbase::simplex)
        .def("newTriangle", overload_cast<>(
            &Triangulation<2>::newTriangle),
            pybind11::return_value_policy::reference_internal,
            rdoc::newTriangle)
        .def("newSimplex", overload_cast<>(&Triangulation<2>::newSimplex),
            pybind11::return_value_policy::reference_internal,
            rbase::newSimplex)
        .def("newTriangle", overload_cast<const std::string&>(
            &Triangulation<2>::newTriangle),
            pybind11::return_value_policy::reference_internal,
            rdoc::newTriangle_2)
        .def("newSimplex", overload_cast<const std::string&>(
            &Triangulation<2>::newSimplex),
            pybind11::return_value_policy::reference_internal,
            rbase::newSimplex_2)
        .def("newSimplices", [](Triangulation<2>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal,
            rbase::newSimplices)
        .def("newTriangles", [](Triangulation<2>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal,
            rdoc::newTriangles)
        .def("removeTriangle", &Triangulation<2>::removeTriangle,
            rdoc::removeTriangle)
        .def("removeSimplex", &Triangulation<2>::removeSimplex,
            rbase::removeSimplex)
        .def("removeTriangleAt", &Triangulation<2>::removeTriangleAt,
            rdoc::removeTriangleAt)
        .def("removeSimplexAt", &Triangulation<2>::removeSimplexAt,
            rbase::removeSimplexAt)
        .def("removeAllTriangles", &Triangulation<2>::removeAllTriangles,
            rdoc::removeAllTriangles)
        .def("removeAllSimplices", &Triangulation<2>::removeAllSimplices,
            rbase::removeAllSimplices)
        .def("swap", &Triangulation<2>::swap, rdoc::swap)
        .def("moveContentsTo", &Triangulation<2>::moveContentsTo,
            rbase::moveContentsTo)
        .def("countComponents", &Triangulation<2>::countComponents,
            rbase::countComponents)
        .def("countBoundaryComponents",
            &Triangulation<2>::countBoundaryComponents,
            rbase::countBoundaryComponents)
        .def("countFaces", (regina::python::countFacesFunc<Triangulation<2>>)(
            &Triangulation<2>::countFaces), rbase::countFaces)
        .def("countVertices", &Triangulation<2>::countVertices,
            rbase::countVertices)
        .def("countEdges", &Triangulation<2>::countEdges, rbase::countEdges)
        .def("fVector", &Triangulation<2>::fVector, rbase::fVector)
        .def("components", &Triangulation<2>::components,
            pybind11::keep_alive<0, 1>(), rbase::components)
        .def("boundaryComponents", &Triangulation<2>::boundaryComponents,
            pybind11::keep_alive<0, 1>(), rbase::boundaryComponents)
        .def("faces", (regina::python::facesFunc<Triangulation<2>>)(
            &Triangulation<2>::faces),
            pybind11::keep_alive<0, 1>(), rbase::faces)
        .def("vertices", &Triangulation<2>::vertices,
            pybind11::keep_alive<0, 1>(), rbase::vertices)
        .def("edges", &Triangulation<2>::edges,
            pybind11::keep_alive<0, 1>(), rbase::edges)
        .def("component", &Triangulation<2>::component,
            pybind11::return_value_policy::reference_internal,
            rbase::component)
        .def("boundaryComponent", &Triangulation<2>::boundaryComponent,
            pybind11::return_value_policy::reference_internal,
            rbase::boundaryComponent)
        .def("face", (regina::python::faceFunc<Triangulation<2>>)(
            &Triangulation<2>::face),
            pybind11::return_value_policy::reference_internal, rbase::face)
        .def("vertex", &Triangulation<2>::vertex,
            pybind11::return_value_policy::reference_internal, rbase::vertex)
        .def("edge", &Triangulation<2>::edge,
            pybind11::return_value_policy::reference_internal, rbase::edge)
        .def("translate", &Triangulation<2>::translate<0>,
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("translate", &Triangulation<2>::translate<1>,
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("pairing", &Triangulation<2>::pairing, rbase::pairing)
        .def("isIsomorphicTo", &Triangulation<2>::isIsomorphicTo,
            rbase::isIsomorphicTo)
        .def("findAllIsomorphisms", &Triangulation<2>::findAllIsomorphisms<
                const std::function<bool(const Isomorphism<2>)>&>,
            rbase::findAllIsomorphisms)
        .def("findAllIsomorphisms", [](const Triangulation<2>& t,
                const Triangulation<2>& other) {
            std::vector<Isomorphism<2>> isos;
            t.findAllIsomorphisms(other, [&](const Isomorphism<2>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        }, rbase::findAllIsomorphisms)
        .def("makeCanonical", &Triangulation<2>::makeCanonical,
            rbase::makeCanonical)
        .def("isContainedIn", &Triangulation<2>::isContainedIn,
            rbase::isContainedIn)
        .def("findAllSubcomplexesIn", &Triangulation<2>::findAllSubcomplexesIn<
                const std::function<bool(const Isomorphism<2>)>&>,
            rbase::findAllSubcomplexesIn)
        .def("findAllSubcomplexesIn", [](const Triangulation<2>& t,
                const Triangulation<2>& other) {
            std::vector<Isomorphism<2>> isos;
            t.findAllSubcomplexesIn(other, [&](const Isomorphism<2>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        }, rbase::findAllSubcomplexesIn)
        .def("isEmpty", &Triangulation<2>::isEmpty, rbase::isEmpty)
        .def("isValid", &Triangulation<2>::isValid, rbase::isValid)
        .def("eulerChar", &Triangulation<2>::eulerChar, rdoc::eulerChar)
        .def("eulerCharTri", &Triangulation<2>::eulerCharTri,
            rbase::eulerCharTri)
        .def("isClosed", &Triangulation<2>::isClosed, rdoc::isClosed)
        .def("hasBoundaryFacets", &Triangulation<2>::hasBoundaryFacets,
            rbase::hasBoundaryFacets)
        .def("hasBoundaryEdges", &Triangulation<2>::hasBoundaryEdges,
            rdoc::hasBoundaryEdges)
        .def("countBoundaryFacets", &Triangulation<2>::countBoundaryFacets,
            rbase::countBoundaryFacets)
        .def("countBoundaryEdges", &Triangulation<2>::countBoundaryEdges,
            rdoc::countBoundaryEdges)
        .def("countBoundaryFaces",
            static_cast<size_t (Triangulation<2>::*)(int) const>(
            &Triangulation<2>::countBoundaryFaces),
            rbase::countBoundaryFaces)
        .def("isOrientable", &Triangulation<2>::isOrientable,
            rbase::isOrientable)
        .def("isOriented", &Triangulation<2>::isOriented, rbase::isOriented)
        .def("isIdeal", &Triangulation<2>::isIdeal, rdoc::isIdeal)
        .def("isConnected", &Triangulation<2>::isConnected,
            rbase::isConnected)
        .def("group", &Triangulation<2>::group,
            pybind11::return_value_policy::reference_internal, rbase::group)
        .def("fundamentalGroup", &Triangulation<2>::fundamentalGroup,
            pybind11::return_value_policy::reference_internal,
            rbase::fundamentalGroup)
        .def("simplifiedFundamentalGroup",
            &Triangulation<2>::simplifiedFundamentalGroup,
            rbase::simplifiedFundamentalGroup)
        .def("isMinimal", &Triangulation<2>::isMinimal, rdoc::isMinimal)
        .def("orient", &Triangulation<2>::orient, rbase::orient)
        .def("reflect", &Triangulation<2>::reflect, rbase::reflect)
        .def("triangulateComponents", &Triangulation<2>::triangulateComponents,
            rbase::triangulateComponents)
        .def("homology",
            static_cast<AbelianGroup (Triangulation<2>::*)(int) const>(
                &Triangulation<2>::homology),
            pybind11::arg("k") = 1, rbase::homology)
        .def("markedHomology",
            static_cast<MarkedAbelianGroup (Triangulation<2>::*)(int) const>(
                &Triangulation<2>::markedHomology),
            pybind11::arg("k") = 1, rbase::markedHomology)
        .def("boundaryMap",
            static_cast<MatrixInt (Triangulation<2>::*)(int) const>(
            &Triangulation<2>::boundaryMap), rbase::boundaryMap)
        .def("dualBoundaryMap",
            static_cast<MatrixInt (Triangulation<2>::*)(int) const>(
            &Triangulation<2>::dualBoundaryMap), rbase::dualBoundaryMap)
        .def("dualToPrimal",
            static_cast<MatrixInt (Triangulation<2>::*)(int) const>(
            &Triangulation<2>::dualToPrimal), rbase::dualToPrimal)
        .def("pachner", &Triangulation<2>::pachner<2>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner)
        .def("pachner", &Triangulation<2>::pachner<1>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner)
        .def("pachner", &Triangulation<2>::pachner<0>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner)
        .def("finiteToIdeal", &Triangulation<2>::finiteToIdeal,
            rbase::finiteToIdeal)
        .def("makeDoubleCover", &Triangulation<2>::makeDoubleCover,
            rbase::makeDoubleCover)
        .def("subdivide", &Triangulation<2>::subdivide, rbase::subdivide)
        .def("barycentricSubdivision", // deprecated
            &Triangulation<2>::subdivide, rbase::barycentricSubdivision)
        .def("insertTriangulation", &Triangulation<2>::insertTriangulation,
            rbase::insertTriangulation)
        .def("isoSig", &Triangulation<2>::isoSig<>, rbase::isoSig)
        .def("isoSig_EdgeDegrees",
            &Triangulation<2>::isoSig<regina::IsoSigEdgeDegrees<2>>,
            rbase::isoSig)
        .def("isoSig_RidgeDegrees",
            &Triangulation<2>::isoSig<regina::IsoSigRidgeDegrees<2>>,
            rbase::isoSig)
        .def("isoSigDetail", &Triangulation<2>::isoSigDetail<>,
            rbase::isoSigDetail)
        .def("isoSigDetail_EdgeDegrees",
            &Triangulation<2>::isoSigDetail<regina::IsoSigEdgeDegrees<2>>,
            rbase::isoSigDetail)
        .def("isoSigDetail_RidgeDegrees",
            &Triangulation<2>::isoSigDetail<regina::IsoSigRidgeDegrees<2>>,
            rbase::isoSigDetail)
        .def_static("fromIsoSig", &Triangulation<2>::fromIsoSig,
            rbase::fromIsoSig)
        .def_static("fromSig", &Triangulation<2>::fromSig, rbase::fromSig)
        .def_static("isoSigComponentSize",
            &Triangulation<2>::isoSigComponentSize, rbase::isoSigComponentSize)
        .def("dumpConstruction", &Triangulation<2>::dumpConstruction,
            rbase::dumpConstruction)
        .def_static("fromGluings", [](size_t size, const std::vector<
                std::tuple<size_t, int, size_t, regina::Perm<3>>>& g) {
            return Triangulation<2>::fromGluings(size, g.begin(), g.end());
        }, rbase::fromGluings)
        .def_readonly_static("dimension", &Triangulation<2>::dimension)
    ;
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::packet_eq_operators(c, rbase::__eq, rbase::__ne);
    regina::python::add_packet_data(c);

    regina::python::addListView<decltype(Triangulation<2>().vertices())>(m);
    regina::python::addListView<decltype(Triangulation<2>().edges())>(m);
    regina::python::addListView<decltype(Triangulation<2>().triangles())>(m);
    regina::python::addListView<decltype(Triangulation<2>().components())>(m);
    regina::python::addListView<
        decltype(Triangulation<2>().boundaryComponents())>(m);

    auto wrap = regina::python::add_packet_wrapper<Triangulation<2>>(
        m, "PacketOfTriangulation2");
    regina::python::add_packet_constructor<>(wrap, rdoc::__default);
    regina::python::add_packet_constructor<const Triangulation<2>&, bool>(wrap,
        rdoc::__init);
    regina::python::add_packet_constructor<const std::string&>(wrap,
        rdoc::__init_2);

    // We cannot use add_global_swap() here, since add_global_swap() cannot
    // resolve regina::swap to the templated triangulation swap function.
    m.def("swap", static_cast<void(&)(Triangulation<2>&, Triangulation<2>&)>(
        regina::swap), rdoc::global_swap);

    RDOC_SCOPE_END

    addIsoSigClassic<2>(m, "IsoSigClassic2");
    addIsoSigEdgeDegrees<2>(m, "IsoSigEdgeDegrees2");
    addIsoSigRidgeDegrees<2>(m, "IsoSigRidgeDegrees2");
    addIsoSigPrintable<2>(m, "IsoSigPrintable2");
}

