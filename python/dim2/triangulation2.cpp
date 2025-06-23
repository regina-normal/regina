
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
#include <pybind11/functional.h>
#include <pybind11/stl.h>
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
using regina::Face;
using regina::Isomorphism;
using regina::MarkedAbelianGroup;
using regina::MatrixInt;
using regina::Simplex;
using regina::Triangulation;

void addTriangulation2(pybind11::module_& m, pybind11::module_& internal) {
    RDOC_SCOPE_BEGIN(Triangulation)
    RDOC_SCOPE_BASE_2(detail::TriangulationBase, Snapshottable)

    auto c = pybind11::class_<Triangulation<2>,
            std::shared_ptr<Triangulation<2>>>(m, "Triangulation2", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Triangulation<2>&>(), rdoc::__copy)
        .def(pybind11::init<const Triangulation<2>&, bool, bool>(),
            pybind11::arg("src"),
            pybind11::arg("cloneProps"),
            pybind11::arg("cloneLocks") = true,
            rdoc::__init)
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
        .def("triangle",
            // gcc-10 struggles with casting: even a static_cast fails here
            // because gcc-10 cannot handle the "auto" return type.
            // Just use simplex(), which triangle() is an alias for.
            overload_cast<size_t>(&Triangulation<2>::simplex),
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
            pybind11::arg("k"), rbase::newSimplices)
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
        .def("hasLocks", &Triangulation<2>::hasLocks, rbase::hasLocks)
        .def("lockBoundary", &Triangulation<2>::lockBoundary,
            rbase::lockBoundary)
        .def("unlockAll", &Triangulation<2>::unlockAll, rbase::unlockAll)
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
        .def("translate", overload_cast<const regina::Face<2, 0>*>(
                &Triangulation<2>::translate<0>, pybind11::const_),
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("translate", overload_cast<const regina::Face<2, 1>*>(
                &Triangulation<2>::translate<1>, pybind11::const_),
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("translate", overload_cast<const regina::Simplex<2>*>(
                &Triangulation<2>::translate<2>, pybind11::const_),
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("translate", overload_cast<const regina::FaceEmbedding<2, 0>&>(
                &Triangulation<2>::translate<0>, pybind11::const_),
            rbase::translate_2)
        .def("translate", overload_cast<const regina::FaceEmbedding<2, 1>&>(
                &Triangulation<2>::translate<1>, pybind11::const_),
            rbase::translate_2)
        .def("pairing", &Triangulation<2>::pairing, rbase::pairing)
        .def("isIsomorphicTo", &Triangulation<2>::isIsomorphicTo,
            rbase::isIsomorphicTo)
        .def("findAllIsomorphisms", &Triangulation<2>::findAllIsomorphisms<
                const std::function<bool(const Isomorphism<2>)>&>,
            pybind11::arg("other"), pybind11::arg("action"),
            rbase::findAllIsomorphisms)
        .def("findAllIsomorphisms", [](const Triangulation<2>& t,
                const Triangulation<2>& other) {
            std::vector<Isomorphism<2>> isos;
            t.findAllIsomorphisms(other, [&](const Isomorphism<2>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        }, pybind11::arg("other"), rbase::findAllIsomorphisms)
        .def("makeCanonical", &Triangulation<2>::makeCanonical,
            rbase::makeCanonical)
        .def("isContainedIn", &Triangulation<2>::isContainedIn,
            rbase::isContainedIn)
        .def("findAllSubcomplexesIn", &Triangulation<2>::findAllSubcomplexesIn<
                const std::function<bool(const Isomorphism<2>)>&>,
            pybind11::arg("other"), pybind11::arg("action"),
            rbase::findAllSubcomplexesIn)
        .def("findAllSubcomplexesIn", [](const Triangulation<2>& t,
                const Triangulation<2>& other) {
            std::vector<Isomorphism<2>> isos;
            t.findAllSubcomplexesIn(other, [&](const Isomorphism<2>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        }, pybind11::arg("other"), rbase::findAllSubcomplexesIn)
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
        .def("setGroupPresentation",
             &Triangulation<2>::setGroupPresentation,
            rbase::setGroupPresentation)
        .def("simplifiedFundamentalGroup", // deprecated
            &Triangulation<2>::setGroupPresentation,
            rbase::simplifiedFundamentalGroup)
        .def("isMinimal", &Triangulation<2>::isMinimal, rdoc::isMinimal)
        .def("isSphere", &Triangulation<2>::isSphere, rdoc::isSphere)
        .def("isBall", &Triangulation<2>::isBall, rdoc::isBall)
        .def("reorderBFS", &Triangulation<2>::reorderBFS,
            pybind11::arg("reverse") = false, rbase::reorderBFS)
        .def("randomiseLabelling", &Triangulation<2>::randomiseLabelling,
            pybind11::arg("preserveOrientation") = true,
            rbase::randomiseLabelling)
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
        .def("pachner",
            overload_cast<Face<2, 2>*>(&Triangulation<2>::pachner<2>),
            rbase::pachner)
        .def("pachner",
            overload_cast<Face<2, 1>*>(&Triangulation<2>::pachner<1>),
            rbase::pachner)
        .def("pachner",
            overload_cast<Face<2, 0>*>(&Triangulation<2>::pachner<0>),
            rbase::pachner)
        .def("move20", &Triangulation<2>::move20<0>, rbase::move20)
        .def("shellBoundary",
            overload_cast<Simplex<2>*>(&Triangulation<2>::shellBoundary),
            rbase::shellBoundary)
        .def("hasPachner", &Triangulation<2>::hasPachner<0>, rbase::hasPachner)
        .def("hasPachner", &Triangulation<2>::hasPachner<1>, rbase::hasPachner)
        .def("hasPachner", &Triangulation<2>::hasPachner<2>, rbase::hasPachner)
        .def("has20", &Triangulation<2>::has20<0>, rbase::has20)
        .def("hasShellBoundary", &Triangulation<2>::hasShellBoundary,
            rbase::hasShellBoundary)
        .def("withPachner", &Triangulation<2>::withPachner<0>,
            rbase::withPachner)
        .def("withPachner", &Triangulation<2>::withPachner<1>,
            rbase::withPachner)
        .def("withPachner", &Triangulation<2>::withPachner<2>,
            rbase::withPachner)
        .def("with20", &Triangulation<2>::with20<0>, rbase::with20)
        .def("withShellBoundary", &Triangulation<2>::withShellBoundary,
            rbase::withShellBoundary)
        .def("finiteToIdeal", &Triangulation<2>::finiteToIdeal,
            rbase::finiteToIdeal)
        .def("doubleCover", &Triangulation<2>::doubleCover, rbase::doubleCover)
        .def("makeDoubleCover", [](Triangulation<2>& tri) { // deprecated
            tri = tri.doubleCover();
        }, rbase::makeDoubleCover)
        .def("doubleOverBoundary", &Triangulation<2>::doubleOverBoundary,
            rbase::doubleOverBoundary)
        .def("subdivide", &Triangulation<2>::subdivide, rbase::subdivide)
        .def("barycentricSubdivision", // deprecated
            &Triangulation<2>::subdivide, rbase::barycentricSubdivision)
        .def("insertTriangulation",
            overload_cast<const Triangulation<2>&>(
                &Triangulation<2>::insertTriangulation),
            rbase::insertTriangulation)
        .def("sig", &Triangulation<2>::sig<>, rbase::sig)
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
        .def("source", &Triangulation<2>::source,
            // The default should be Language::Current, but in C++ that
            // evaluates to Language::Cxx.  We need it to evaluate to
            // Language::Python (i.e., the Python implementation of
            // Language::Current), and so we explicitly use that as our
            // default instead.
            pybind11::arg("language") = regina::Language::Python,
            rbase::source)
        .def("dumpConstruction", [](const Triangulation<2>& tri) {
            // Deprecated, so reimplement this ourselves.
            return tri.source(regina::Language::Cxx);
        }, rbase::dumpConstruction)
        .def("dot", &Triangulation<2>::dot,
            pybind11::arg("labels") = false, rbase::dot)
        .def_static("fromGluings", [](size_t size, const std::vector<
                std::tuple<size_t, int, size_t, regina::Perm<3>>>& g) {
            return Triangulation<2>::fromGluings(size, g.begin(), g.end());
        }, pybind11::arg("size"), pybind11::arg("gluings"), rbase::fromGluings)
        .def_readonly_static("dimension", &Triangulation<2>::dimension)
    ;
    #if defined(__GNUC__)
    // The following routines are deprecated, but we still need to bind
    // them.  Silence the inevitable deprecation warnings that will occur.
    #pragma GCC diagnostic push
    #if defined(__clang__)
    #pragma GCC diagnostic ignored "-Wdeprecated"
    #else
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #endif
    #endif
    c.def("pachner",
            overload_cast<Face<2, 2>*, bool, bool>(
                &Triangulation<2>::pachner<2>),
            pybind11::arg(),
            pybind11::arg("ignored"),
            pybind11::arg("perform") = true,
            rbase::pachner_2) // deprecated
        .def("pachner",
            overload_cast<Face<2, 1>*, bool, bool>(
                &Triangulation<2>::pachner<1>),
            pybind11::arg(),
            pybind11::arg("ignored"),
            pybind11::arg("perform") = true,
            rbase::pachner_2) // deprecated
        .def("pachner",
            overload_cast<Face<2, 0>*, bool, bool>(
                &Triangulation<2>::pachner<0>),
            pybind11::arg(),
            pybind11::arg("ignored"),
            pybind11::arg("perform") = true,
            rbase::pachner_2) // deprecated
        // For twoZeroMove(), the new function has a different name (move20).
        // We therefore give a default value for "ignored" in order to preserve
        // backward compatibility in cases where both boolean arguments are
        // omitted.
        .def("twoZeroMove", &Triangulation<2>::twoZeroMove<0>,
            pybind11::arg(),
            pybind11::arg("ignored") = true,
            pybind11::arg("perform") = true,
            rbase::twoZeroMove) // deprecated
        .def("shellBoundary",
            overload_cast<Simplex<2>*, bool, bool>(
                &Triangulation<2>::shellBoundary),
            pybind11::arg(),
            pybind11::arg("ignored"),
            pybind11::arg("perform") = true,
            rbase::shellBoundary_2) // deprecated
    ;
    #if defined(__GNUC__)
    #pragma GCC diagnostic pop
    #endif
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::packet_eq_operators(c, rbase::__eq);
    regina::python::add_packet_data(c);

    regina::python::addListView<decltype(Triangulation<2>().vertices())>(internal);
    regina::python::addListView<decltype(Triangulation<2>().edges())>(internal);
    regina::python::addListView<decltype(Triangulation<2>().triangles())>(internal);
    regina::python::addListView<decltype(Triangulation<2>().components())>(internal);
    regina::python::addListView<
        decltype(Triangulation<2>().boundaryComponents())>(internal);

    auto wrap = regina::python::add_packet_wrapper<Triangulation<2>>(
        m, "PacketOfTriangulation2");
    regina::python::add_packet_constructor<>(wrap, rdoc::__default);
    regina::python::add_packet_constructor<const Triangulation<2>&, bool,
        bool>(wrap,
        pybind11::arg("src"),
        pybind11::arg("cloneProps"),
        pybind11::arg("cloneLocks") = true,
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

