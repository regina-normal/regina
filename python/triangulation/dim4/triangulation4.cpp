
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include "algebra/grouppresentation.h"
#include "algebra/intersectionform.h"
#include "progress/progresstracker.h"
#include "triangulation/dim4.h"
#include "triangulation/facetpairing.h"
#include "triangulation/detail/isosig-impl.h"
#include "../../helpers.h"
#include "../../helpers/packet.h"
#include "../facehelper.h"
#include "../../docstrings/triangulation/dim4/triangulation4.h"
#include "../../docstrings/triangulation/triangulation.h"
#include "../../docstrings/utilities/snapshot.h"
#include "../isosig-bindings.h" // must come after docstrings

using namespace pybind11::literals;

using pybind11::overload_cast;
using regina::python::GILCallbackManager;
using regina::AbelianGroup;
using regina::ByteSequence;
using regina::Face;
using regina::FaceEmbedding;
using regina::Isomorphism;
using regina::MarkedAbelianGroup;
using regina::MatrixInt;
using regina::Simplex;
using regina::Triangulation;

void addTriangulation4(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(Triangulation4)
    RDOC_SCOPE_BASE_2(detail::TriangulationBase, Snapshottable)

    auto c = pybind11::class_<Triangulation<4>,
            std::shared_ptr<Triangulation<4>>>(m, "Triangulation4",
            rdoc::__class)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Triangulation<4>&>(), rdoc::__copy)
        .def(pybind11::init<const Triangulation<4>&, bool, bool>(),
            "src"_a, "cloneProps"_a, "cloneLocks"_a = true, rdoc::__init)
        .def(pybind11::init<const std::string&>(), rdoc::__init_2)
        .def("isReadOnlySnapshot", &Triangulation<4>::isReadOnlySnapshot,
            rbase2::isReadOnlySnapshot)
        .def("size", &Triangulation<4>::size, rbase::size)
        .def("countPentachora", &Triangulation<4>::countPentachora,
            rbase::countPentachora)
        .def("pentachora", &Triangulation<4>::pentachora,
            pybind11::keep_alive<0, 1>(), rbase::pentachora)
        .def("simplices", &Triangulation<4>::simplices,
            pybind11::keep_alive<0, 1>(), rbase::simplices)
        .def("pentachoron",
            // gcc-10 struggles with casting: even a static_cast fails here
            // because gcc-10 cannot handle the "auto" return type.
            // Just use simplex(), which pentachoron() is an alias for.
            overload_cast<size_t>(&Triangulation<4>::simplex),
            pybind11::return_value_policy::reference_internal,
            rbase::pentachoron)
        .def("simplex",
            overload_cast<size_t>(&Triangulation<4>::simplex),
            pybind11::return_value_policy::reference_internal, rbase::simplex)
        .def("newPentachoron", overload_cast<>(
            &Triangulation<4>::newPentachoron),
            pybind11::return_value_policy::reference_internal,
            rdoc::newPentachoron)
        .def("newSimplex", overload_cast<>(&Triangulation<4>::newSimplex),
            pybind11::return_value_policy::reference_internal,
            rbase::newSimplex)
        .def("newPentachoron", overload_cast<const std::string&>(
            &Triangulation<4>::newPentachoron),
            pybind11::return_value_policy::reference_internal,
            rdoc::newPentachoron_2)
        .def("newSimplex", overload_cast<const std::string&>(
            &Triangulation<4>::newSimplex),
            pybind11::return_value_policy::reference_internal,
            rbase::newSimplex_2)
        .def("newSimplices", [](Triangulation<4>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal,
            "k"_a, rbase::newSimplices)
        .def("newPentachora", [](Triangulation<4>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal,
            rdoc::newPentachora)
        .def("removePentachoron", &Triangulation<4>::removePentachoron,
            rdoc::removePentachoron)
        .def("removeSimplex", &Triangulation<4>::removeSimplex,
            rbase::removeSimplex)
        .def("removePentachoronAt", &Triangulation<4>::removePentachoronAt,
            rdoc::removePentachoronAt)
        .def("removeSimplexAt", &Triangulation<4>::removeSimplexAt,
            rbase::removeSimplexAt)
        .def("removeAllPentachora", &Triangulation<4>::removeAllPentachora,
            rdoc::removeAllPentachora)
        .def("removeAllSimplices", &Triangulation<4>::removeAllSimplices,
            rbase::removeAllSimplices)
        .def("swap", &Triangulation<4>::swap, rdoc::swap)
        .def("moveContentsTo", &Triangulation<4>::moveContentsTo,
            rbase::moveContentsTo)
        .def("hasLocks", &Triangulation<4>::hasLocks, rbase::hasLocks)
        .def("lockBoundary", &Triangulation<4>::lockBoundary,
            rbase::lockBoundary)
        .def("unlockAll", &Triangulation<4>::unlockAll, rbase::unlockAll)
        .def("countComponents", &Triangulation<4>::countComponents,
            rbase::countComponents)
        .def("countBoundaryComponents",
            &Triangulation<4>::countBoundaryComponents,
            rbase::countBoundaryComponents)
        .def("countFaces", (regina::python::countFacesFunc<Triangulation<4>>)(
            &Triangulation<4>::countFaces), rbase::countFaces)
        .def("countVertices", &Triangulation<4>::countVertices,
            rbase::countVertices)
        .def("countEdges", &Triangulation<4>::countEdges, rbase::countEdges)
        .def("countTriangles", &Triangulation<4>::countTriangles,
            rbase::countTriangles)
        .def("countTetrahedra", &Triangulation<4>::countTetrahedra,
            rbase::countTetrahedra)
        .def("fVector", &Triangulation<4>::fVector, rbase::fVector)
        .def("components", &Triangulation<4>::components,
            pybind11::keep_alive<0, 1>(), rbase::components)
        .def("boundaryComponents", &Triangulation<4>::boundaryComponents,
            pybind11::keep_alive<0, 1>(), rbase::boundaryComponents)
        .def("faces", (regina::python::facesFunc<Triangulation<4>>)(
                &Triangulation<4>::faces),
            pybind11::keep_alive<0, 1>(), rbase::faces)
        .def("vertices", &Triangulation<4>::vertices,
            pybind11::keep_alive<0, 1>(), rbase::vertices)
        .def("edges", &Triangulation<4>::edges,
            pybind11::keep_alive<0, 1>(), rbase::edges)
        .def("triangles", &Triangulation<4>::triangles,
            pybind11::keep_alive<0, 1>(), rbase::triangles)
        .def("tetrahedra", &Triangulation<4>::tetrahedra,
            pybind11::keep_alive<0, 1>(), rbase::tetrahedra)
        .def("component", &Triangulation<4>::component,
            pybind11::return_value_policy::reference_internal,
            rbase::component)
        .def("boundaryComponent", &Triangulation<4>::boundaryComponent,
            pybind11::return_value_policy::reference_internal,
            rbase::boundaryComponent)
        .def("face", (regina::python::faceFunc<Triangulation<4>>)(
                &Triangulation<4>::face),
            pybind11::return_value_policy::reference_internal, rbase::face)
        .def("vertex", &Triangulation<4>::vertex,
            pybind11::return_value_policy::reference_internal, rbase::vertex)
        .def("edge", &Triangulation<4>::edge,
            pybind11::return_value_policy::reference_internal, rbase::edge)
        .def("triangle",
            // gcc-10 struggles with casting: even a static_cast fails here
            // because gcc-10 cannot handle the "auto" return type.
            // Just use face<2>(), which triangle() is an alias for.
            overload_cast<size_t>(&Triangulation<4>::face<2>, pybind11::const_),
            pybind11::return_value_policy::reference_internal, rbase::triangle)
        .def("tetrahedron",
            // gcc-10 struggles with casting: even a static_cast fails here
            // because gcc-10 cannot handle the "auto" return type.
            // Just use face<3>(), which tetrahedron() is an alias for.
            overload_cast<size_t>(&Triangulation<4>::face<3>, pybind11::const_),
            pybind11::return_value_policy::reference_internal,
            rbase::tetrahedron)
        .def("translate", overload_cast<const Face<4, 0>*>(
                &Triangulation<4>::translate<0>, pybind11::const_),
            pybind11::return_value_policy::reference_internal,
            rbase::translate_face)
        .def("translate", overload_cast<const Face<4, 1>*>(
                &Triangulation<4>::translate<1>, pybind11::const_),
            pybind11::return_value_policy::reference_internal,
            rbase::translate_face)
        .def("translate", overload_cast<const Face<4, 2>*>(
                &Triangulation<4>::translate<2>, pybind11::const_),
            pybind11::return_value_policy::reference_internal,
            rbase::translate_face)
        .def("translate", overload_cast<const Face<4, 3>*>(
                &Triangulation<4>::translate<3>, pybind11::const_),
            pybind11::return_value_policy::reference_internal,
            rbase::translate_face)
        .def("translate",
            static_cast<Simplex<4>* (Triangulation<4>::*)(const Simplex<4>*)
                const>(&Triangulation<4>::translate),
            pybind11::return_value_policy::reference_internal,
            rbase::translate_simplex)
        .def("translate", overload_cast<const FaceEmbedding<4, 0>&>(
                &Triangulation<4>::translate<0>, pybind11::const_),
            rbase::translate_embedding)
        .def("translate", overload_cast<const FaceEmbedding<4, 1>&>(
                &Triangulation<4>::translate<1>, pybind11::const_),
            rbase::translate_embedding)
        .def("translate", overload_cast<const FaceEmbedding<4, 2>&>(
                &Triangulation<4>::translate<2>, pybind11::const_),
            rbase::translate_embedding)
        .def("translate", overload_cast<const FaceEmbedding<4, 3>&>(
                &Triangulation<4>::translate<3>, pybind11::const_),
            rbase::translate_embedding)
        .def("pairing", &Triangulation<4>::pairing, rbase::pairing)
        .def("isIsomorphicTo", &Triangulation<4>::isIsomorphicTo,
            rbase::isIsomorphicTo)
        .def("makeCanonical", &Triangulation<4>::makeCanonical,
            rbase::makeCanonical)
        .def("isContainedIn", &Triangulation<4>::isContainedIn,
            rbase::isContainedIn)
        .def("findAllIsomorphisms", &Triangulation<4>::findAllIsomorphisms<
                const std::function<bool(const Isomorphism<4>)>&>,
            "other"_a, "action"_a, rbase::findAllIsomorphisms)
        .def("findAllIsomorphisms", [](const Triangulation<4>& t,
                const Triangulation<4>& other) {
            std::vector<Isomorphism<4>> isos;
            t.findAllIsomorphisms(other, [&](const Isomorphism<4>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        }, "other"_a, rbase::findAllIsomorphisms)
        .def("findAllSubcomplexesIn", &Triangulation<4>::findAllSubcomplexesIn<
                const std::function<bool(const Isomorphism<4>)>&>,
            "other"_a, "action"_a, rbase::findAllSubcomplexesIn)
        .def("findAllSubcomplexesIn", [](const Triangulation<4>& t,
                const Triangulation<4>& other) {
            std::vector<Isomorphism<4>> isos;
            t.findAllSubcomplexesIn(other, [&](const Isomorphism<4>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        }, "other"_a, rbase::findAllSubcomplexesIn)
        .def("isEmpty", &Triangulation<4>::isEmpty, rbase::isEmpty)
        .def("eulerCharTri", &Triangulation<4>::eulerCharTri,
            rbase::eulerCharTri)
        .def("eulerCharManifold", &Triangulation<4>::eulerCharManifold,
            rdoc::eulerCharManifold)
        .def("isValid", &Triangulation<4>::isValid, rbase::isValid)
        .def("layer44", &Triangulation<4>::layer44, rdoc::layer44)
        .def("isIdeal", &Triangulation<4>::isIdeal, rdoc::isIdeal)
        .def("layerOn", overload_cast<regina::Triangle<4>*>(
            &Triangulation<4>::layerOn),
            pybind11::return_value_policy::reference, rdoc::layerOn)
        .def("layerOn", overload_cast<regina::Edge<4>*>(
            &Triangulation<4>::layerOn),
            pybind11::return_value_policy::reference, rdoc::layerOn_2)
        .def("hasBoundaryFacets", &Triangulation<4>::hasBoundaryFacets,
            rbase::hasBoundaryFacets)
        .def("hasBoundaryTetrahedra", &Triangulation<4>::hasBoundaryTetrahedra,
            rdoc::hasBoundaryTetrahedra)
        .def("countBoundaryFacets", &Triangulation<4>::countBoundaryFacets,
            rbase::countBoundaryFacets)
        .def("countBoundaryTetrahedra",
            &Triangulation<4>::countBoundaryTetrahedra,
            rdoc::countBoundaryTetrahedra)
        .def("countBoundaryFaces",
            static_cast<size_t (Triangulation<4>::*)(int) const>(
            &Triangulation<4>::countBoundaryFaces),
            rbase::countBoundaryFaces)
        .def("isClosed", &Triangulation<4>::isClosed, rdoc::isClosed)
        .def("isOrientable", &Triangulation<4>::isOrientable,
            rbase::isOrientable)
        .def("isOriented", &Triangulation<4>::isOriented, rbase::isOriented)
        .def("isConnected", &Triangulation<4>::isConnected,
            rbase::isConnected)
        .def("group", &Triangulation<4>::group,
            pybind11::return_value_policy::reference_internal, rbase::group)
        .def("fundamentalGroup", &Triangulation<4>::fundamentalGroup,
            pybind11::return_value_policy::reference_internal,
            rbase::fundamentalGroup)
        .def("setGroupPresentation",
             &Triangulation<4>::setGroupPresentation,
            rbase::setGroupPresentation)
        .def("simplifiedFundamentalGroup", // deprecated
            &Triangulation<4>::setGroupPresentation,
            rbase::simplifiedFundamentalGroup)
        .def("cachedGroup", &Triangulation<4>::cachedGroup, rbase::cachedGroup)
        .def("homology",
            static_cast<AbelianGroup (Triangulation<4>::*)(int) const>(
                &Triangulation<4>::homology),
            "k"_a = 1, rbase::homology)
        .def("knowsHomology",
            static_cast<bool (Triangulation<4>::*)(int, bool) const>(
                &Triangulation<4>::knowsHomology),
            "k"_a = 1, "cachedOnly"_a = false, rbase::knowsHomology)
        .def("markedHomology",
            static_cast<MarkedAbelianGroup (Triangulation<4>::*)(int) const>(
                &Triangulation<4>::markedHomology),
            "k"_a = 1, rbase::markedHomology)
        .def("boundaryMap",
            static_cast<MatrixInt (Triangulation<4>::*)(int) const>(
            &Triangulation<4>::boundaryMap), rbase::boundaryMap)
        .def("dualBoundaryMap",
            static_cast<MatrixInt (Triangulation<4>::*)(int) const>(
            &Triangulation<4>::dualBoundaryMap), rbase::dualBoundaryMap)
        .def("dualToPrimal",
            static_cast<MatrixInt (Triangulation<4>::*)(int) const>(
            &Triangulation<4>::dualToPrimal), rbase::dualToPrimal)
        .def("intersectionForm", &Triangulation<4>::intersectionForm,
            rdoc::intersectionForm)
        .def("reorderBFS", &Triangulation<4>::reorderBFS,
            "reverse"_a = false, rbase::reorderBFS)
        .def("randomiseLabelling", &Triangulation<4>::randomiseLabelling,
            "preserveOrientation"_a = true, rbase::randomiseLabelling)
        .def("orient", &Triangulation<4>::orient, rbase::orient)
        .def("reflect", &Triangulation<4>::reflect, rbase::reflect)
        .def("triangulateComponents", &Triangulation<4>::triangulateComponents,
            rbase::triangulateComponents)
        .def("simplify", &Triangulation<4>::simplify,
            "tracker"_a = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::simplify)
        .def("intelligentSimplify", &Triangulation<4>::simplify, // deprecated
            rdoc::intelligentSimplify)
        .def("simplifyToLocalMinimum",
            &Triangulation<4>::simplifyToLocalMinimum,
            "perform"_a = true, rdoc::simplifyToLocalMinimum)
        .def("simplifyUpDown", &Triangulation<4>::simplifyUpDown,
            "max24"_a = -1, "max33"_a = -1, "alwaysModify"_a = false,
            rdoc::simplifyUpDown)
        .def("simplifyExhaustive", &Triangulation<4>::simplifyExhaustive,
            "height"_a = 1, "threads"_a = 1, "tracker"_a = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::simplifyExhaustive)
        .def("retriangulate", [](const Triangulation<4>& tri, int height,
                int threads, const std::function<bool(const ByteSequence&,
                    Triangulation<4>&&)>& action) {
            if (threads == 1) {
                return tri.retriangulate(height, 1, nullptr, action);
            } else {
                GILCallbackManager manager;
                return tri.retriangulate(height, threads, nullptr,
                    [&](const ByteSequence& sig, Triangulation<4>&& t) -> bool {
                        GILCallbackManager<>::ScopedAcquire acquire(manager);
                        return action(sig, std::move(t));
                    });
            }
        }, "height"_a, "threads"_a, "action"_a, rdoc::retriangulate)
        .def("pachner",
            static_cast<bool (Triangulation<4>::*)(Simplex<4>*)>(
                &Triangulation<4>::pachner),
            rbase::pachner_simplex)
        .def("pachner",
            overload_cast<Face<4, 3>*>(&Triangulation<4>::pachner<3>),
            rbase::pachner_face)
        .def("pachner",
            overload_cast<Face<4, 2>*>(&Triangulation<4>::pachner<2>),
            rbase::pachner_face)
        .def("pachner",
            overload_cast<Face<4, 1>*>(&Triangulation<4>::pachner<1>),
            rbase::pachner_face)
        .def("pachner",
            overload_cast<Face<4, 0>*>(&Triangulation<4>::pachner<0>),
            rbase::pachner_face)
        .def("puncture", &Triangulation<4>::puncture,
            "location"_a = nullptr, rdoc::puncture)
        .def("connectedSumWith", &Triangulation<4>::connectedSumWith,
            rdoc::connectedSumWith)
        .def("move20", &Triangulation<4>::move20<0>, rbase::move20)
        .def("move20", &Triangulation<4>::move20<1>, rbase::move20)
        .def("move20", &Triangulation<4>::move20<2>, rbase::move20)
        .def("move44", &Triangulation<4>::move44, rdoc::move44)
        .def("openBook",
            overload_cast<regina::Tetrahedron<4>*>(&Triangulation<4>::openBook),
            rdoc::openBook)
        .def("shellBoundary",
            overload_cast<Simplex<4>*>(&Triangulation<4>::shellBoundary),
            rbase::shellBoundary)
        .def("collapseEdge",
            overload_cast<regina::Edge<4>*>(&Triangulation<4>::collapseEdge),
            rdoc::collapseEdge)
        .def("snapEdge",
            overload_cast<regina::Edge<4>*>(&Triangulation<4>::snapEdge),
            rdoc::snapEdge)
        .def("hasPachner", &Triangulation<4>::hasPachner<0>,
            rbase::hasPachner_face)
        .def("hasPachner", &Triangulation<4>::hasPachner<1>,
            rbase::hasPachner_face)
        .def("hasPachner", &Triangulation<4>::hasPachner<2>,
            rbase::hasPachner_face)
        .def("hasPachner", &Triangulation<4>::hasPachner<3>,
            rbase::hasPachner_face)
        .def("hasPachner",
            static_cast<bool (Triangulation<4>::*)(Simplex<4>*) const>(
                &Triangulation<4>::hasPachner),
            rbase::hasPachner_simplex)
        .def("has20", &Triangulation<4>::has20<0>, rbase::has20)
        .def("has20", &Triangulation<4>::has20<1>, rbase::has20)
        .def("has20", &Triangulation<4>::has20<2>, rbase::has20)
        .def("has44", &Triangulation<4>::has44, rdoc::has44)
        .def("hasOpenBook", &Triangulation<4>::hasOpenBook, rdoc::hasOpenBook)
        .def("hasShellBoundary", &Triangulation<4>::hasShellBoundary,
            rbase::hasShellBoundary)
        .def("hasCollapseEdge", &Triangulation<4>::hasCollapseEdge,
            rdoc::hasCollapseEdge)
        .def("hasSnapEdge", &Triangulation<4>::hasSnapEdge, rdoc::hasSnapEdge)
        .def("withPachner", &Triangulation<4>::withPachner<0>,
            rbase::withPachner_face)
        .def("withPachner", &Triangulation<4>::withPachner<1>,
            rbase::withPachner_face)
        .def("withPachner", &Triangulation<4>::withPachner<2>,
            rbase::withPachner_face)
        .def("withPachner", &Triangulation<4>::withPachner<3>,
            rbase::withPachner_face)
        .def("withPachner",
            static_cast<std::optional<Triangulation<4>>
                (Triangulation<4>::*)(Simplex<4>*) const>(
                &Triangulation<4>::withPachner),
            rbase::withPachner_simplex)
        .def("with20", &Triangulation<4>::with20<0>, rbase::with20)
        .def("with20", &Triangulation<4>::with20<1>, rbase::with20)
        .def("with20", &Triangulation<4>::with20<2>, rbase::with20)
        .def("with44", &Triangulation<4>::with44, rdoc::with44)
        .def("withOpenBook", &Triangulation<4>::withOpenBook,
            rdoc::withOpenBook)
        .def("withShellBoundary", &Triangulation<4>::withShellBoundary,
            rbase::withShellBoundary)
        .def("withCollapseEdge", &Triangulation<4>::withCollapseEdge,
            rdoc::withCollapseEdge)
        .def("withSnapEdge", &Triangulation<4>::withSnapEdge,
            rdoc::withSnapEdge)
        .def("makeIdeal", &Triangulation<4>::makeIdeal, rbase::makeIdeal)
        .def("finiteToIdeal", &Triangulation<4>::finiteToIdeal,
            rbase::finiteToIdeal)
        .def("doubleCover", &Triangulation<4>::doubleCover, rbase::doubleCover)
        .def("makeDoubleCover", [](Triangulation<4>& tri) { // deprecated
            tri = tri.doubleCover();
        }, rbase::makeDoubleCover)
        .def("doubleOverBoundary", &Triangulation<4>::doubleOverBoundary,
            rbase::doubleOverBoundary)
        .def("subdivide", &Triangulation<4>::subdivide, rbase::subdivide)
        .def("barycentricSubdivision", // deprecated
            &Triangulation<4>::subdivide, rbase::barycentricSubdivision)
        .def("truncateIdeal", &Triangulation<4>::truncateIdeal,
            rdoc::truncateIdeal)
        .def("idealToFinite", &Triangulation<4>::idealToFinite,
            rdoc::idealToFinite)
        .def("linkingSurface", &Triangulation<4>::linkingSurface<0>,
            rdoc::linkingSurface)
        .def("linkingSurface", &Triangulation<4>::linkingSurface<1>,
            rdoc::linkingSurface)
        .def("linkingSurface", &Triangulation<4>::linkingSurface<2>,
            rdoc::linkingSurface)
        .def("linkingSurface", &Triangulation<4>::linkingSurface<3>,
            rdoc::linkingSurface)
        .def("insertTriangulation",
            overload_cast<const Triangulation<4>&>(
                &Triangulation<4>::insertTriangulation),
            rbase::insertTriangulation)
        // Variants of isoSig() are handled through add_isosig_variants() below.
        // With fromSig(), the byte sequence variant _must_ come first.
        // This is because pybind11 performs automatic conversion from bytes
        // to std::string, and so if the string variant appears first then
        // pybind11 will use it even with a pybind11::bytes argument.
        .def_static("fromSig",
            overload_cast<const ByteSequence&>(&Triangulation<4>::fromSig),
            rbase::fromSig_2)
        .def_static("fromSig",
            overload_cast<const std::string&>(&Triangulation<4>::fromSig),
            rbase::fromSig)
        .def_static("fromIsoSig", // deprecated
            overload_cast<const std::string&>(&Triangulation<4>::fromSig),
            rbase::fromIsoSig)
        .def_static("sigGeneration", &Triangulation<4>::sigGeneration,
            rbase::sigGeneration)
        .def_static("sigComponentSize",
            &Triangulation<4>::sigComponentSize, rbase::sigComponentSize)
        .def_static("isoSigComponentSize", // deprecated
            &Triangulation<4>::sigComponentSize, rbase::isoSigComponentSize)
        .def("source", &Triangulation<4>::source,
            // The default should be Language::Current, but in C++ that
            // evaluates to Language::Cxx.  We need it to evaluate to
            // Language::Python (i.e., the Python implementation of
            // Language::Current), and so we explicitly use that as our
            // default instead.
            "language"_a = regina::Language::Python, rbase::source)
        .def("dumpConstruction", [](const Triangulation<4>& tri) {
            // Deprecated, so reimplement this ourselves.
            return tri.source(regina::Language::Cxx);
        }, rbase::dumpConstruction)
        .def("dot", &Triangulation<4>::dot,
            "labels"_a = false, rbase::dot)
        .def_static("fromGluings", [](size_t size, const std::vector<
                std::tuple<size_t, int, size_t, regina::Perm<5>>>& g) {
            return Triangulation<4>::fromGluings(size, g.begin(), g.end());
        }, "size"_a, "gluings"_a, rbase::fromGluings)
        .def_readonly_static("dimension", &Triangulation<4>::dimension)
    ;
    regina::python::add_isosig_variants<4>(c);
    regina::python::add_output_rich(c);
    regina::python::add_tight_encoding(c);
    regina::python::packet_eq_operators(c, rbase::__eq);
    regina::python::add_packet_data(c);
    // The global swap is bound later, when we have a different rdoc alias.

    regina::python::addStdView<decltype(Triangulation<4>().vertices())>(
        internal, "Triangulation4_vertices");
    regina::python::addStdView<decltype(Triangulation<4>().edges())>(
        internal, "Triangulation4_edges");
    regina::python::addStdView<decltype(Triangulation<4>().triangles())>(
        internal, "Triangulation4_triangles");
    regina::python::addStdView<decltype(Triangulation<4>().tetrahedra())>(
        internal, "Triangulation4_tetrahedra");
    regina::python::addStdView<decltype(Triangulation<4>().pentachora())>(
        internal, "Triangulation4_pentachora");
    regina::python::addStdView<decltype(Triangulation<4>().components())>(
        internal, "Triangulation4_components");
    regina::python::addStdView<
        decltype(Triangulation<4>().boundaryComponents())>(
        internal, "Triangulation4_boundaryComponents");

    auto wrap = regina::python::add_packet_wrapper<Triangulation<4>>(
        m, "PacketOfTriangulation4");
    regina::python::add_packet_constructor<>(wrap, rdoc::__default);
    regina::python::add_packet_constructor<const Triangulation<4>&, bool,
        bool>(wrap,
        "src"_a, "cloneProps"_a, "cloneLocks"_a = true, rdoc::__init);
    regina::python::add_packet_constructor<const std::string&>(wrap,
        rdoc::__init_2);

    RDOC_SCOPE_SWITCH(Triangulation)

    regina::python::add_global_swap<Triangulation<4>, rdoc>(m);

    RDOC_SCOPE_END

    addIsoSigClassic<4>(m, "IsoSigClassic4");
    addIsoSigEdgeDegrees<4>(m, "IsoSigEdgeDegrees4");
    addIsoSigRidgeDegrees<4>(m, "IsoSigRidgeDegrees4");
    addIsoSigData<1, 4>(m, "IsoSigData1_4");
    addIsoSigData<2, 4>(m, "IsoSigData2_4");
}

INSTANTIATE_ISOSIG_BINDING_FUNCTIONS(4)

