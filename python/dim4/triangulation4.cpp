
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
#include <pybind11/functional.h>
#include <pybind11/stl.h>
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
#include "../docstrings/triangulation/dim4/triangulation4.h"
#include "../docstrings/triangulation/detail/triangulation.h"
#include "../docstrings/utilities/snapshot.h"

using pybind11::overload_cast;
using regina::python::GILCallbackManager;
using regina::AbelianGroup;
using regina::Isomorphism;
using regina::MarkedAbelianGroup;
using regina::MatrixInt;
using regina::Triangulation;

void addTriangulation4(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Triangulation)
    RDOC_SCOPE_BASE_2(detail::TriangulationBase, Snapshottable)

    auto c = pybind11::class_<Triangulation<4>,
            std::shared_ptr<Triangulation<4>>>(m, "Triangulation4", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Triangulation<4>&>(), rdoc::__copy)
        .def(pybind11::init<const Triangulation<4>&, bool, bool>(),
            pybind11::arg("src"),
            pybind11::arg("cloneProps"),
            pybind11::arg("cloneLocks") = true,
            rdoc::__init)
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
            pybind11::arg("k"), rbase::newSimplices)
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
        .def("translate", overload_cast<const regina::Face<4, 0>*>(
                &Triangulation<4>::translate<0>, pybind11::const_),
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("translate", overload_cast<const regina::Face<4, 1>*>(
                &Triangulation<4>::translate<1>, pybind11::const_),
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("translate", overload_cast<const regina::Face<4, 2>*>(
                &Triangulation<4>::translate<2>, pybind11::const_),
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("translate", overload_cast<const regina::Face<4, 3>*>(
                &Triangulation<4>::translate<3>, pybind11::const_),
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("translate", overload_cast<const regina::Simplex<4>*>(
                &Triangulation<4>::translate<4>, pybind11::const_),
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("translate", overload_cast<const regina::FaceEmbedding<4, 0>&>(
                &Triangulation<4>::translate<0>, pybind11::const_),
            rbase::translate_2)
        .def("translate", overload_cast<const regina::FaceEmbedding<4, 1>&>(
                &Triangulation<4>::translate<1>, pybind11::const_),
            rbase::translate_2)
        .def("translate", overload_cast<const regina::FaceEmbedding<4, 2>&>(
                &Triangulation<4>::translate<2>, pybind11::const_),
            rbase::translate_2)
        .def("translate", overload_cast<const regina::FaceEmbedding<4, 3>&>(
                &Triangulation<4>::translate<3>, pybind11::const_),
            rbase::translate_2)
        .def("pairing", &Triangulation<4>::pairing, rbase::pairing)
        .def("isIsomorphicTo", &Triangulation<4>::isIsomorphicTo,
            rbase::isIsomorphicTo)
        .def("makeCanonical", &Triangulation<4>::makeCanonical,
            rbase::makeCanonical)
        .def("isContainedIn", &Triangulation<4>::isContainedIn,
            rbase::isContainedIn)
        .def("findAllIsomorphisms", &Triangulation<4>::findAllIsomorphisms<
                const std::function<bool(const Isomorphism<4>)>&>,
            pybind11::arg("other"), pybind11::arg("action"),
            rbase::findAllIsomorphisms)
        .def("findAllIsomorphisms", [](const Triangulation<4>& t,
                const Triangulation<4>& other) {
            std::vector<Isomorphism<4>> isos;
            t.findAllIsomorphisms(other, [&](const Isomorphism<4>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        }, pybind11::arg("other"), rbase::findAllIsomorphisms)
        .def("findAllSubcomplexesIn", &Triangulation<4>::findAllSubcomplexesIn<
                const std::function<bool(const Isomorphism<4>)>&>,
            pybind11::arg("other"), pybind11::arg("action"),
            rbase::findAllSubcomplexesIn)
        .def("findAllSubcomplexesIn", [](const Triangulation<4>& t,
                const Triangulation<4>& other) {
            std::vector<Isomorphism<4>> isos;
            t.findAllSubcomplexesIn(other, [&](const Isomorphism<4>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        }, pybind11::arg("other"), rbase::findAllSubcomplexesIn)
        .def("isEmpty", &Triangulation<4>::isEmpty, rbase::isEmpty)
        .def("eulerCharTri", &Triangulation<4>::eulerCharTri,
            rbase::eulerCharTri)
        .def("eulerCharManifold", &Triangulation<4>::eulerCharManifold,
            rdoc::eulerCharManifold)
        .def("isValid", &Triangulation<4>::isValid, rbase::isValid)
        .def("isIdeal", &Triangulation<4>::isIdeal, rdoc::isIdeal)
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
        .def("homology",
            static_cast<AbelianGroup (Triangulation<4>::*)(int) const>(
                &Triangulation<4>::homology),
            pybind11::arg("k") = 1, rbase::homology)
        .def("markedHomology",
            static_cast<MarkedAbelianGroup (Triangulation<4>::*)(int) const>(
                &Triangulation<4>::markedHomology),
            pybind11::arg("k") = 1, rbase::markedHomology)
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
            pybind11::arg("reverse") = false, rbase::reorderBFS)
        .def("randomiseLabelling", &Triangulation<4>::randomiseLabelling,
            pybind11::arg("preserveOrientation") = true,
            rbase::randomiseLabelling)
        .def("orient", &Triangulation<4>::orient, rbase::orient)
        .def("reflect", &Triangulation<4>::reflect, rbase::reflect)
        .def("triangulateComponents", &Triangulation<4>::triangulateComponents,
            rbase::triangulateComponents)
        .def("simplify", &Triangulation<4>::simplify, rdoc::simplify)
        .def("intelligentSimplify", &Triangulation<4>::simplify, // deprecated
            rdoc::intelligentSimplify)
        .def("simplifyToLocalMinimum",
            &Triangulation<4>::simplifyToLocalMinimum,
            pybind11::arg("perform") = true, rdoc::simplifyToLocalMinimum)
        .def("simplifyUpDown", &Triangulation<4>::simplifyUpDown,
            pybind11::arg("max24") = -1,
            pybind11::arg("max33") = -1,
            pybind11::arg("alwaysModify") = false,
            rdoc::simplifyUpDown)
        .def("simplifyExhaustive", &Triangulation<4>::simplifyExhaustive,
            pybind11::arg("height") = 1,
            pybind11::arg("threads") = 1,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::simplifyExhaustive)
        .def("retriangulate", [](const Triangulation<4>& tri, int height,
                int threads, const std::function<bool(const std::string&,
                    Triangulation<4>&&)>& action) {
            if (threads == 1) {
                return tri.retriangulate(height, 1, nullptr, action);
            } else {
                GILCallbackManager manager;
                return tri.retriangulate(height, threads, nullptr,
                    [&](const std::string& sig, Triangulation<4>&& t) -> bool {
                        GILCallbackManager<>::ScopedAcquire acquire(manager);
                        return action(sig, std::move(t));
                    });
            }
        }, pybind11::arg("height"),
            pybind11::arg("threads"),
            pybind11::arg("action"),
            rdoc::retriangulate)
        .def("pachner", &Triangulation<4>::pachner<4>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner)
        .def("pachner", &Triangulation<4>::pachner<3>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner)
        .def("pachner", &Triangulation<4>::pachner<2>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner)
        .def("pachner", &Triangulation<4>::pachner<1>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner)
        .def("pachner", &Triangulation<4>::pachner<0>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner)
        .def("withPachner", &Triangulation<4>::withPachner<0>,
            rbase::withPachner)
        .def("withPachner", &Triangulation<4>::withPachner<1>,
            rbase::withPachner)
        .def("withPachner", &Triangulation<4>::withPachner<2>,
            rbase::withPachner)
        .def("withPachner", &Triangulation<4>::withPachner<3>,
            rbase::withPachner)
        .def("withPachner", &Triangulation<4>::withPachner<4>,
            rbase::withPachner)
        .def("twoZeroMove",
            overload_cast<regina::Triangle<4>*, bool, bool>(
            &Triangulation<4>::twoZeroMove),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::twoZeroMove)
        .def("twoZeroMove",
            overload_cast<regina::Edge<4>*, bool, bool>(
            &Triangulation<4>::twoZeroMove),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::twoZeroMove_2)
        .def("twoZeroMove",
            overload_cast<regina::Vertex<4>*, bool, bool>(
            &Triangulation<4>::twoZeroMove),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::twoZeroMove_3)
        .def("with20", overload_cast<regina::Triangle<4>*>(
                &Triangulation<4>::with20, pybind11::const_),
            rdoc::with20)
        .def("with20", overload_cast<regina::Edge<4>*>(
                &Triangulation<4>::with20, pybind11::const_),
            rdoc::with20_2)
        .def("with20", overload_cast<regina::Vertex<4>*>(
                &Triangulation<4>::with20, pybind11::const_),
            rdoc::with20_3)
        .def("fourFourMove", &Triangulation<4>::fourFourMove,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::fourFourMove)
        .def("with44", &Triangulation<4>::with44, rdoc::with44)
        .def("openBook", &Triangulation<4>::openBook,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::openBook)
        .def("withOpenBook", &Triangulation<4>::withOpenBook,
            rdoc::withOpenBook)
        .def("shellBoundary", &Triangulation<4>::shellBoundary,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::shellBoundary)
        .def("withShellBoundary", &Triangulation<4>::withShellBoundary,
            rdoc::withShellBoundary)
        .def("collapseEdge", &Triangulation<4>::collapseEdge,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::collapseEdge)
        .def("withCollapseEdge", &Triangulation<4>::withCollapseEdge,
            rdoc::withCollapseEdge)
        .def("snapEdge", &Triangulation<4>::snapEdge,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::snapEdge)
        .def("withSnapEdge", &Triangulation<4>::withSnapEdge,
            rdoc::withSnapEdge)
        .def("finiteToIdeal", &Triangulation<4>::finiteToIdeal,
            rbase::finiteToIdeal)
        .def("doubleCover", &Triangulation<4>::doubleCover, rbase::doubleCover)
        .def("makeDoubleCover", [](Triangulation<4>& tri) { // deprecated
            tri = tri.doubleCover();
        }, rbase::makeDoubleCover)
        .def("subdivide", &Triangulation<4>::subdivide, rbase::subdivide)
        .def("barycentricSubdivision", // deprecated
            &Triangulation<4>::subdivide, rbase::barycentricSubdivision)
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
        .def("sig", &Triangulation<4>::sig<>, rbase::sig)
        .def("isoSig", &Triangulation<4>::isoSig<>, rbase::isoSig)
        .def("isoSig_EdgeDegrees",
            &Triangulation<4>::isoSig<regina::IsoSigEdgeDegrees<4>>,
            rbase::isoSig)
        .def("isoSig_RidgeDegrees",
            &Triangulation<4>::isoSig<regina::IsoSigRidgeDegrees<4>>,
            rbase::isoSig)
        .def("isoSigDetail", &Triangulation<4>::isoSigDetail<>,
            rbase::isoSigDetail)
        .def("isoSigDetail_EdgeDegrees",
            &Triangulation<4>::isoSigDetail<regina::IsoSigEdgeDegrees<4>>,
            rbase::isoSigDetail)
        .def("isoSigDetail_RidgeDegrees",
            &Triangulation<4>::isoSigDetail<regina::IsoSigRidgeDegrees<4>>,
            rbase::isoSigDetail)
        .def_static("fromIsoSig", &Triangulation<4>::fromIsoSig,
            rbase::fromIsoSig)
        .def_static("fromSig", &Triangulation<4>::fromSig, rbase::fromSig)
        .def_static("isoSigComponentSize",
            &Triangulation<4>::isoSigComponentSize, rbase::isoSigComponentSize)
        .def("source", &Triangulation<4>::source,
            // The default should be Language::Current, but in C++ that
            // evaluates to Language::Cxx.  We need it to evaluate to
            // Language::Python (i.e., the Python implementation of
            // Language::Current), and so we explicitly use that as our
            // default instead.
            pybind11::arg("language") = regina::Language::Python,
            rbase::source)
        .def("dumpConstruction", [](const Triangulation<4>& tri) {
            // Deprecated, so reimplement this ourselves.
            return tri.source(regina::Language::Cxx);
        }, rbase::dumpConstruction)
        .def("dot", &Triangulation<4>::dot,
            pybind11::arg("labels") = false, rbase::dot)
        .def_static("fromGluings", [](size_t size, const std::vector<
                std::tuple<size_t, int, size_t, regina::Perm<5>>>& g) {
            return Triangulation<4>::fromGluings(size, g.begin(), g.end());
        }, pybind11::arg("size"), pybind11::arg("gluings"), rbase::fromGluings)
        .def_readonly_static("dimension", &Triangulation<4>::dimension)
    ;
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::packet_eq_operators(c, rbase::__eq);
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
    regina::python::add_packet_constructor<>(wrap, rdoc::__default);
    regina::python::add_packet_constructor<const Triangulation<4>&, bool,
        bool>(wrap,
        pybind11::arg("src"),
        pybind11::arg("cloneProps"),
        pybind11::arg("cloneLocks") = true,
        rdoc::__init);
    regina::python::add_packet_constructor<const std::string&>(wrap,
        rdoc::__init_2);

    // We cannot use add_global_swap() here, since add_global_swap() cannot
    // resolve regina::swap to the templated triangulation swap function.
    m.def("swap", static_cast<void(&)(Triangulation<4>&, Triangulation<4>&)>(
        regina::swap), rdoc::global_swap);

    RDOC_SCOPE_END

    addIsoSigClassic<4>(m, "IsoSigClassic4");
    addIsoSigEdgeDegrees<4>(m, "IsoSigEdgeDegrees4");
    addIsoSigRidgeDegrees<4>(m, "IsoSigRidgeDegrees4");
    addIsoSigPrintable<4>(m, "IsoSigPrintable4");
}

