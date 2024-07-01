
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

#include "../pybind11/pybind11.h"
#include "../pybind11/functional.h"
#include "../pybind11/stl.h"
#include "../helpers.h"
#include "triangulation/facetpairing.h"
#include "triangulation/generic.h"
#include "triangulation/isosigtype.h"
#include "triangulation/detail/isosig-impl.h"
#include "utilities/typeutils.h"
#include "../generic/facehelper.h"
#include "../docstrings/triangulation/generic/triangulation.h"
#include "../docstrings/triangulation/detail/triangulation.h"
#include "../docstrings/utilities/snapshot.h"

using pybind11::overload_cast;
using regina::AbelianGroup;
using regina::Isomorphism;
using regina::MarkedAbelianGroup;
using regina::MatrixInt;
using regina::Triangulation;

template <int dim>
void addTriangulation(pybind11::module_& m, const char* name) {
    RDOC_SCOPE_BEGIN(Triangulation)
    RDOC_SCOPE_BASE_2(detail::TriangulationBase, Snapshottable)

    auto c = pybind11::class_<Triangulation<dim>,
            std::shared_ptr<Triangulation<dim>>>(m, name, rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Triangulation<dim>&>(), rdoc::__copy)
        .def(pybind11::init<const Triangulation<dim>&, bool, bool>(),
            pybind11::arg("src"),
            pybind11::arg("cloneProps"),
            pybind11::arg("cloneLocks") = true,
            rdoc::__init)
        .def("isReadOnlySnapshot", &Triangulation<dim>::isReadOnlySnapshot,
            rbase2::isReadOnlySnapshot)
        .def("size", &Triangulation<dim>::size, rbase::size)
        .def("simplices", &Triangulation<dim>::simplices,
            pybind11::keep_alive<0, 1>(), rbase::simplices)
        .def("simplex", overload_cast<size_t>(&Triangulation<dim>::simplex),
            pybind11::return_value_policy::reference_internal, rbase::simplex)
        .def("newSimplex",
            overload_cast<>(&Triangulation<dim>::newSimplex),
            pybind11::return_value_policy::reference_internal,
            rbase::newSimplex)
        .def("newSimplex",
            overload_cast<const std::string&>(&Triangulation<dim>::newSimplex),
            pybind11::return_value_policy::reference_internal,
            rbase::newSimplex_2)
        .def("newSimplices", [](Triangulation<dim>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal,
            pybind11::arg("k"), rbase::newSimplices)
        .def("removeSimplex", &Triangulation<dim>::removeSimplex,
            rbase::removeSimplex)
        .def("removeSimplexAt", &Triangulation<dim>::removeSimplexAt,
            rbase::removeSimplexAt)
        .def("removeAllSimplices", &Triangulation<dim>::removeAllSimplices,
            rbase::removeAllSimplices)
        .def("swap", &Triangulation<dim>::swap, rdoc::swap)
        .def("moveContentsTo", &Triangulation<dim>::moveContentsTo,
            rbase::moveContentsTo)
        .def("hasLocks", &Triangulation<dim>::hasLocks, rbase::hasLocks)
        .def("lockBoundary", &Triangulation<dim>::lockBoundary,
            rbase::lockBoundary)
        .def("unlockAll", &Triangulation<dim>::unlockAll, rbase::unlockAll)
        .def("countComponents", &Triangulation<dim>::countComponents,
            rbase::countComponents)
        .def("countBoundaryComponents",
            &Triangulation<dim>::countBoundaryComponents,
            rbase::countBoundaryComponents)
        .def("countFaces",
            static_cast<regina::python::countFacesFunc<Triangulation<dim>>>(
            &Triangulation<dim>::countFaces), rbase::countFaces)
        .def("fVector", &Triangulation<dim>::fVector, rbase::fVector)
        .def("components", &Triangulation<dim>::components,
            pybind11::keep_alive<0, 1>(), rbase::components)
        .def("boundaryComponents", &Triangulation<dim>::boundaryComponents,
            pybind11::keep_alive<0, 1>(), rbase::boundaryComponents)
        .def("faces",
            static_cast<regina::python::facesFunc<Triangulation<dim>>>(
                &Triangulation<dim>::faces),
            pybind11::keep_alive<0, 1>(), rbase::faces)
        .def("component", &Triangulation<dim>::component,
            pybind11::return_value_policy::reference_internal, rbase::component)
        .def("boundaryComponent", &Triangulation<dim>::boundaryComponent,
            pybind11::return_value_policy::reference_internal,
            rbase::boundaryComponent)
        .def("face", static_cast<regina::python::faceFunc<Triangulation<dim>>>(
            &Triangulation<dim>::face),
            pybind11::return_value_policy::reference_internal, rbase::face)
        .def("countVertices", &Triangulation<dim>::countVertices,
            rbase::countVertices)
        .def("countEdges", &Triangulation<dim>::countEdges, rbase::countEdges)
        .def("countTriangles", &Triangulation<dim>::countTriangles,
            rbase::countTriangles)
        .def("countTetrahedra", &Triangulation<dim>::countTetrahedra,
            rbase::countTetrahedra)
        .def("countPentachora", &Triangulation<dim>::countPentachora,
            rbase::countPentachora)
        .def("vertices", &Triangulation<dim>::vertices,
            pybind11::keep_alive<0, 1>(), rbase::vertices)
        .def("edges", &Triangulation<dim>::edges,
            pybind11::keep_alive<0, 1>(), rbase::edges)
        .def("triangles", &Triangulation<dim>::triangles,
            pybind11::keep_alive<0, 1>(), rbase::triangles)
        .def("tetrahedra", &Triangulation<dim>::tetrahedra,
            pybind11::keep_alive<0, 1>(), rbase::tetrahedra)
        .def("pentachora", &Triangulation<dim>::pentachora,
            pybind11::keep_alive<0, 1>(), rbase::pentachora)
        .def("vertex", &Triangulation<dim>::vertex,
            pybind11::return_value_policy::reference_internal, rbase::vertex)
        .def("edge", &Triangulation<dim>::edge,
            pybind11::return_value_policy::reference_internal, rbase::edge)
        // Use static casts because GCC struggles with overload_cast here:
        .def("triangle",
            static_cast<regina::Face<dim, 2>* (Triangulation<dim>::*)(size_t)>(
                &Triangulation<dim>::triangle),
            pybind11::return_value_policy::reference_internal, rbase::triangle)
        .def("tetrahedron",
            static_cast<regina::Face<dim, 3>* (Triangulation<dim>::*)(size_t)>(
                &Triangulation<dim>::tetrahedron),
            pybind11::return_value_policy::reference_internal,
            rbase::tetrahedron)
        .def("pentachoron",
            static_cast<regina::Face<dim, 4>* (Triangulation<dim>::*)(size_t)>(
                &Triangulation<dim>::pentachoron),
            pybind11::return_value_policy::reference_internal,
            rbase::pentachoron)
        .def("pairing", &Triangulation<dim>::pairing, rbase::pairing)
        .def("isEmpty", &Triangulation<dim>::isEmpty, rbase::isEmpty)
        .def("isValid", &Triangulation<dim>::isValid, rbase::isValid)
        .def("hasBoundaryFacets", &Triangulation<dim>::hasBoundaryFacets,
            rbase::hasBoundaryFacets)
        .def("countBoundaryFacets", &Triangulation<dim>::countBoundaryFacets,
            rbase::countBoundaryFacets)
        .def("countBoundaryFaces",
            static_cast<size_t (Triangulation<dim>::*)(int) const>(
            &Triangulation<dim>::countBoundaryFaces), rbase::countBoundaryFaces)
        .def("isOrientable", &Triangulation<dim>::isOrientable,
            rbase::isOrientable)
        .def("isOriented", &Triangulation<dim>::isOriented, rbase::isOriented)
        .def("isConnected", &Triangulation<dim>::isConnected,
            rbase::isConnected)
        .def("reorderBFS", &Triangulation<dim>::reorderBFS,
            pybind11::arg("reverse") = false, rbase::reorderBFS)
        .def("randomiseLabelling", &Triangulation<dim>::randomiseLabelling,
            pybind11::arg("preserveOrientation") = true,
            rbase::randomiseLabelling)
        .def("orient", &Triangulation<dim>::orient, rbase::orient)
        .def("reflect", &Triangulation<dim>::reflect, rbase::reflect)
        .def("triangulateComponents",
            &Triangulation<dim>::triangulateComponents,
            rbase::triangulateComponents)
        .def("eulerCharTri", &Triangulation<dim>::eulerCharTri,
            rbase::eulerCharTri)
        .def("group", &Triangulation<dim>::group,
            pybind11::return_value_policy::reference_internal, rbase::group)
        .def("fundamentalGroup", &Triangulation<dim>::fundamentalGroup,
            pybind11::return_value_policy::reference_internal,
            rbase::fundamentalGroup)
        .def("setGroupPresentation",
             &Triangulation<dim>::setGroupPresentation,
            rbase::setGroupPresentation)
        .def("simplifiedFundamentalGroup", // deprecated
            &Triangulation<dim>::setGroupPresentation,
            rbase::simplifiedFundamentalGroup)
        .def("homology",
            static_cast<AbelianGroup (Triangulation<dim>::*)(int) const>(
                &Triangulation<dim>::homology),
            pybind11::arg("k") = 1, rbase::homology)
        .def("markedHomology",
            static_cast<MarkedAbelianGroup (Triangulation<dim>::*)(int) const>(
                &Triangulation<dim>::markedHomology),
            pybind11::arg("k") = 1, rbase::markedHomology)
        .def("boundaryMap",
            static_cast<MatrixInt (Triangulation<dim>::*)(int) const>(
            &Triangulation<dim>::boundaryMap), rbase::boundaryMap)
        .def("dualBoundaryMap",
            static_cast<MatrixInt (Triangulation<dim>::*)(int) const>(
            &Triangulation<dim>::dualBoundaryMap), rbase::dualBoundaryMap)
        .def("dualToPrimal",
            static_cast<MatrixInt (Triangulation<dim>::*)(int) const>(
            &Triangulation<dim>::dualToPrimal), rbase::dualToPrimal)
        .def("finiteToIdeal", &Triangulation<dim>::finiteToIdeal,
            rbase::finiteToIdeal)
        .def("doubleCover", &Triangulation<dim>::doubleCover,
            rbase::doubleCover)
        .def("makeDoubleCover", [](Triangulation<dim>& tri) { // deprecated
            tri = tri.doubleCover();
        }, rbase::makeDoubleCover)
        .def("isIsomorphicTo", &Triangulation<dim>::isIsomorphicTo,
            rbase::isIsomorphicTo)
        .def("isContainedIn", &Triangulation<dim>::isContainedIn,
            rbase::isContainedIn)
        .def("findAllIsomorphisms", &Triangulation<dim>::template
                findAllIsomorphisms<
                const std::function<bool(const Isomorphism<dim>)>&>,
            pybind11::arg("other"), pybind11::arg("action"),
            rbase::findAllIsomorphisms)
        .def("findAllIsomorphisms", [](const Triangulation<dim>& t,
                const Triangulation<dim>& other) {
            std::vector<Isomorphism<dim>> isos;
            t.findAllIsomorphisms(other, [&](const Isomorphism<dim>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        }, pybind11::arg("other"), rbase::findAllIsomorphisms)
        .def("findAllSubcomplexesIn", &Triangulation<dim>::template
                findAllSubcomplexesIn<
                const std::function<bool(const Isomorphism<dim>)>&>,
            pybind11::arg("other"), pybind11::arg("action"),
            rbase::findAllSubcomplexesIn)
        .def("findAllSubcomplexesIn", [](const Triangulation<dim>& t,
                const Triangulation<dim>& other) {
            std::vector<Isomorphism<dim>> isos;
            t.findAllSubcomplexesIn(other, [&](const Isomorphism<dim>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        }, pybind11::arg("other"), rbase::findAllSubcomplexesIn)
        .def("makeCanonical", &Triangulation<dim>::makeCanonical,
            rbase::makeCanonical)
        .def("insert",
            overload_cast<const Triangulation<dim>&>(
                &Triangulation<dim>::insert),
            rbase::insert)
        .def("insertTriangulation", // deprecated
            overload_cast<const Triangulation<dim>&>(
                &Triangulation<dim>::insert),
            rbase::insertTriangulation)
        .def("sig", &Triangulation<dim>::template sig<>, rbase::sig)
        .def("isoSig", &Triangulation<dim>::template isoSig<>, rbase::isoSig)
        .def("isoSig_EdgeDegrees", &Triangulation<dim>::
            template isoSig<regina::IsoSigEdgeDegrees<dim>>, rbase::isoSig)
        .def("isoSig_RidgeDegrees", &Triangulation<dim>::
            template isoSig<regina::IsoSigRidgeDegrees<dim>>, rbase::isoSig)
        .def("isoSigDetail", &Triangulation<dim>::template isoSigDetail<>,
            rbase::isoSigDetail)
        .def("isoSigDetail_EdgeDegrees", &Triangulation<dim>::
            template isoSigDetail<regina::IsoSigEdgeDegrees<dim>>,
            rbase::isoSigDetail)
        .def("isoSigDetail_RidgeDegrees", &Triangulation<dim>::
            template isoSigDetail<regina::IsoSigRidgeDegrees<dim>>,
            rbase::isoSigDetail)
        .def_static("fromIsoSig", &Triangulation<dim>::fromIsoSig,
            rbase::fromIsoSig)
        .def_static("fromSig", &Triangulation<dim>::fromSig, rbase::fromSig)
        .def_static("isoSigComponentSize",
            &Triangulation<dim>::isoSigComponentSize,
            rbase::isoSigComponentSize)
        .def("source", &Triangulation<dim>::source,
            // The default should be Language::Current, but in C++ that
            // evaluates to Language::Cxx.  We need it to evaluate to
            // Language::Python (i.e., the Python implementation of
            // Language::Current), and so we explicitly use that as our
            // default instead.
            pybind11::arg("language") = regina::Language::Python,
            rbase::source)
        .def("dumpConstruction", [](const Triangulation<dim>& tri) {
            // Deprecated, so reimplement this ourselves.
            return tri.source(regina::Language::Cxx);
        }, rbase::dumpConstruction)
        .def("dot", &Triangulation<dim>::dot,
            pybind11::arg("labels") = false, rbase::dot)
        .def_static("fromGluings", [](size_t size, const std::vector<
                std::tuple<size_t, int, size_t, regina::Perm<dim+1>>>& g) {
            return Triangulation<dim>::fromGluings(size, g.begin(), g.end());
        }, pybind11::arg("size"), pybind11::arg("gluings"), rbase::fromGluings)
        .def_readonly_static("dimension", &Triangulation<dim>::dimension)
    ;
    regina::for_constexpr<0, dim>([&c](auto k) {
        c.def("translate", &Triangulation<dim>::template translate<k>,
            pybind11::return_value_policy::reference_internal,
            rbase::translate);
    });
    regina::for_constexpr<0, dim + 1>([&c](auto k) {
        c.def("pachner", &Triangulation<dim>::template pachner<k>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner);
    });
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::packet_eq_operators(c, rbase::__eq, rbase::__ne);
    regina::python::add_packet_data(c);

    // The ListView classes for faces() are wrapped in face-bindings.h,
    // since this needs to be done for each subdimension.
    regina::python::addListView<decltype(Triangulation<dim>().components())>(m);
    regina::python::addListView<
        decltype(Triangulation<dim>().boundaryComponents())>(m);

    auto wrap = regina::python::add_packet_wrapper<Triangulation<dim>>(
        m, (std::string("PacketOf") + name).c_str());
    regina::python::add_packet_constructor<>(wrap, rdoc::__default);
    regina::python::add_packet_constructor<const Triangulation<dim>&, bool,
        bool>(wrap,
        pybind11::arg("src"),
        pybind11::arg("cloneProps"),
        pybind11::arg("cloneLocks") = true,
        rdoc::__init);

    // We cannot use add_global_swap() here, since add_global_swap() cannot
    // resolve regina::swap to the templated triangulation swap function.
    m.def("swap", static_cast<void(&)(Triangulation<dim>&,
        Triangulation<dim>&)>(regina::swap), rdoc::global_swap);

    RDOC_SCOPE_END
}

