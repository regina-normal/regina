
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
#include "angle/anglestructure.h"
#include "link/link.h"
#include "packet/container.h"
#include "progress/progresstracker.h"
#include "snappea/snappeatriangulation.h"
#include "surface/normalsurface.h"
#include "triangulation/example3.h"
#include "triangulation/facetpairing3.h"
#include "triangulation/isosigtype.h"
#include "triangulation/detail/isosig-impl.h"
#include "../generic/facehelper.h"
#include "../generic/isosig-bindings.h"
#include "../docstrings/triangulation/dim3/triangulation3.h"
#include "../docstrings/triangulation/detail/triangulation.h"
#include "../docstrings/utilities/snapshot.h"

using pybind11::overload_cast;
using regina::python::GILCallbackManager;
using regina::AbelianGroup;
using regina::Example;
using regina::Isomorphism;
using regina::MarkedAbelianGroup;
using regina::MatrixInt;
using regina::Triangulation;

/**
 * An internal C++ representation of a snappy.Manifold or snappy.Triangulation.
 *
 * The only purpose of this class is to facilitate constructors of the form
 * Triangulation3(snappy.Manifold) / Triangulation3(snappy.Triangulation).
 *
 * We declare these Triangulation3 constructors to take a SnapPyObject, and
 * then we tell pybind11 to support implicit conversions to SnapPyObject
 * from anything with a _to_string() method.
 */
namespace regina::python {
    struct SnapPyObject {
        std::string string_;
    };
} // namespace regina::python

/**
 * Tell pybind11 how to convert a Python object with a _to_string() method
 * (which we assume is a snappy.Manifold or a snappy.Triangulation) to our
 * internal SnapPyObject class.
 */
namespace pybind11::detail {
    template <>
    struct type_caster<regina::python::SnapPyObject> {
        public:
            PYBIND11_TYPE_CASTER(regina::python::SnapPyObject,
                _("SnapPyObject"));

            bool load(handle src, bool) {
                if (! pybind11::hasattr(src, "_to_string"))
                    return false;

                value.string_ = pybind11::str(src.attr("_to_string")());
                return true;
            }

            static handle cast(const regina::python::SnapPyObject& /* src */,
                    return_value_policy, handle /* parent */) {
                // We never convert from C++ back to Python, since the user
                // should never directly hold a SnapPyObject type.
                return nullptr;
            }
    };
} // namespace pybind11::detail

void addTriangulation3(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(Triangulation)
    RDOC_SCOPE_BASE_2(detail::TriangulationBase, Snapshottable)

    auto c = pybind11::class_<Triangulation<3>,
            std::shared_ptr<Triangulation<3>>>(m, "Triangulation3", rdoc_scope)
        .def(pybind11::init<>(), rdoc::__default)
        .def(pybind11::init<const Triangulation<3>&>(), rdoc::__copy)
        .def(pybind11::init<const Triangulation<3>&, bool>(), rdoc::__init)
        .def(pybind11::init<const regina::Link&, bool>(),
            pybind11::arg(), pybind11::arg("simplify") = true, rdoc::__init_2)
        .def(pybind11::init<const std::string&>(), rdoc::__init_3)
        .def(pybind11::init([](const regina::python::SnapPyObject& obj) {
            return new Triangulation<3>(obj.string_);
        }), rdoc::__init_4)
        .def("isReadOnlySnapshot", &Triangulation<3>::isReadOnlySnapshot,
            rbase2::isReadOnlySnapshot)
        .def("inAnyPacket", overload_cast<>(&Triangulation<3>::inAnyPacket),
            rdoc::inAnyPacket)
        .def("isSnapPea", overload_cast<>(&Triangulation<3>::isSnapPea),
            rdoc::isSnapPea)
        .def("size", &Triangulation<3>::size, rbase::size)
        .def("countTetrahedra", &Triangulation<3>::countTetrahedra,
            rbase::countTetrahedra)
        .def("tetrahedra", &Triangulation<3>::tetrahedra,
            pybind11::keep_alive<0, 1>(), rbase::tetrahedra)
        .def("simplices", &Triangulation<3>::simplices,
            pybind11::keep_alive<0, 1>(), rbase::simplices)
        // Use a static cast because GCC struggles with the overload_cast here:
        .def("tetrahedron",
            static_cast<regina::Simplex<3>* (Triangulation<3>::*)(size_t)>(
                &Triangulation<3>::tetrahedron),
            pybind11::return_value_policy::reference_internal,
            rbase::tetrahedron)
        .def("simplex",
            overload_cast<size_t>(&Triangulation<3>::simplex),
            pybind11::return_value_policy::reference_internal, rbase::simplex)
        .def("newTetrahedron", overload_cast<>(
            &Triangulation<3>::newTetrahedron),
            pybind11::return_value_policy::reference_internal,
            rdoc::newTetrahedron)
        .def("newSimplex", overload_cast<>(&Triangulation<3>::newSimplex),
            pybind11::return_value_policy::reference_internal,
            rbase::newSimplex)
        .def("newTetrahedron", overload_cast<const std::string&>(
            &Triangulation<3>::newTetrahedron),
            pybind11::return_value_policy::reference_internal,
            rdoc::newTetrahedron_2)
        .def("newSimplex", overload_cast<const std::string&>(
            &Triangulation<3>::newSimplex),
            pybind11::return_value_policy::reference_internal,
            rbase::newSimplex_2)
        .def("newSimplices", [](Triangulation<3>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal,
            rbase::newSimplices)
        .def("newTetrahedra", [](Triangulation<3>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal,
            rdoc::newTetrahedra)
        .def("removeTetrahedron", &Triangulation<3>::removeTetrahedron,
            rdoc::removeTetrahedron)
        .def("removeSimplex", &Triangulation<3>::removeSimplex,
            rbase::removeSimplex)
        .def("removeTetrahedronAt", &Triangulation<3>::removeTetrahedronAt,
            rdoc::removeTetrahedronAt)
        .def("removeSimplexAt", &Triangulation<3>::removeSimplexAt,
            rbase::removeSimplexAt)
        .def("removeAllTetrahedra", &Triangulation<3>::removeAllTetrahedra,
            rdoc::removeAllTetrahedra)
        .def("removeAllSimplices", &Triangulation<3>::removeAllSimplices,
            rbase::removeAllSimplices)
        .def("swap", &Triangulation<3>::swap, rdoc::swap)
        .def("moveContentsTo", &Triangulation<3>::moveContentsTo,
            rbase::moveContentsTo)
        .def("countComponents", &Triangulation<3>::countComponents,
            rbase::countComponents)
        .def("countBoundaryComponents",
            &Triangulation<3>::countBoundaryComponents,
            rbase::countBoundaryComponents)
        .def("countFaces", (regina::python::countFacesFunc<Triangulation<3>>)(
            &Triangulation<3>::countFaces), rbase::countFaces)
        .def("countVertices", &Triangulation<3>::countVertices,
            rbase::countVertices)
        .def("countEdges", &Triangulation<3>::countEdges, rbase::countEdges)
        .def("countTriangles", &Triangulation<3>::countTriangles,
            rbase::countTriangles)
        .def("fVector", &Triangulation<3>::fVector, rbase::fVector)
        .def("components", &Triangulation<3>::components,
            pybind11::keep_alive<0, 1>(), rbase::components)
        .def("boundaryComponents", &Triangulation<3>::boundaryComponents,
            pybind11::keep_alive<0, 1>(), rbase::boundaryComponents)
        .def("faces", (regina::python::facesFunc<Triangulation<3>>)(
            &Triangulation<3>::faces),
            pybind11::keep_alive<0, 1>(), rbase::faces)
        .def("vertices", &Triangulation<3>::vertices,
            pybind11::keep_alive<0, 1>(), rbase::vertices)
        .def("edges", &Triangulation<3>::edges,
            pybind11::keep_alive<0, 1>(), rbase::edges)
        .def("triangles", &Triangulation<3>::triangles,
            pybind11::keep_alive<0, 1>(), rbase::triangles)
        .def("component", &Triangulation<3>::component,
            pybind11::return_value_policy::reference_internal,
            rbase::component)
        .def("boundaryComponent", &Triangulation<3>::boundaryComponent,
            pybind11::return_value_policy::reference_internal,
            rbase::boundaryComponent)
        .def("face", (regina::python::faceFunc<Triangulation<3>>)(
            &Triangulation<3>::face),
            pybind11::return_value_policy::reference_internal, rbase::face)
        .def("vertex", &Triangulation<3>::vertex,
            pybind11::return_value_policy::reference_internal, rbase::vertex)
        .def("edge", &Triangulation<3>::edge,
            pybind11::return_value_policy::reference_internal, rbase::edge)
        // Use a static cast because GCC struggles with the overload_cast here:
        .def("triangle",
            static_cast<regina::Face<3, 2>* (Triangulation<3>::*)(size_t)>(
                &Triangulation<3>::triangle),
            pybind11::return_value_policy::reference_internal, rbase::triangle)
        .def("translate", &Triangulation<3>::translate<0>,
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("translate", &Triangulation<3>::translate<1>,
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("translate", &Triangulation<3>::translate<2>,
            pybind11::return_value_policy::reference_internal, rbase::translate)
        .def("pairing", &Triangulation<3>::pairing, rbase::pairing)
        .def("isIsomorphicTo", &Triangulation<3>::isIsomorphicTo,
            rbase::isIsomorphicTo)
        .def("findAllIsomorphisms", &Triangulation<3>::findAllIsomorphisms<
                const std::function<bool(const Isomorphism<3>)>&>,
            rbase::findAllIsomorphisms)
        .def("findAllIsomorphisms", [](const Triangulation<3>& t,
                const Triangulation<3>& other) {
            std::vector<Isomorphism<3>> isos;
            t.findAllIsomorphisms(other, [&](const Isomorphism<3>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        }, rbase::findAllIsomorphisms)
        .def("findAllSubcomplexesIn", &Triangulation<3>::findAllSubcomplexesIn<
                const std::function<bool(const Isomorphism<3>)>&>,
            rbase::findAllSubcomplexesIn)
        .def("findAllSubcomplexesIn", [](const Triangulation<3>& t,
                const Triangulation<3>& other) {
            std::vector<Isomorphism<3>> isos;
            t.findAllSubcomplexesIn(other, [&](const Isomorphism<3>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        }, rbase::findAllSubcomplexesIn)
        .def("makeCanonical", &Triangulation<3>::makeCanonical,
            rbase::makeCanonical)
        .def("isContainedIn", &Triangulation<3>::isContainedIn,
            rbase::isContainedIn)
        .def("hasTwoSphereBoundaryComponents",
            &Triangulation<3>::hasTwoSphereBoundaryComponents,
            rdoc::hasTwoSphereBoundaryComponents)
        .def("hasNegativeIdealBoundaryComponents",
            &Triangulation<3>::hasNegativeIdealBoundaryComponents,
            rdoc::hasNegativeIdealBoundaryComponents)
        .def("hasMinimalBoundary", &Triangulation<3>::hasMinimalBoundary,
            rdoc::hasMinimalBoundary)
        .def("hasMinimalVertices", &Triangulation<3>::hasMinimalVertices,
            rdoc::hasMinimalVertices)
        .def("isEmpty", &Triangulation<3>::isEmpty, rbase::isEmpty)
        .def("eulerCharTri", &Triangulation<3>::eulerCharTri,
            rbase::eulerCharTri)
        .def("eulerCharManifold", &Triangulation<3>::eulerCharManifold,
            rdoc::eulerCharManifold)
        .def("isValid", &Triangulation<3>::isValid, rbase::isValid)
        .def("isIdeal", &Triangulation<3>::isIdeal, rdoc::isIdeal)
        .def("isStandard", &Triangulation<3>::isStandard, rdoc::isStandard)
        .def("hasBoundaryFacets", &Triangulation<3>::hasBoundaryFacets,
            rbase::hasBoundaryFacets)
        .def("hasBoundaryTriangles", &Triangulation<3>::hasBoundaryTriangles,
            rdoc::hasBoundaryTriangles)
        .def("countBoundaryFacets", &Triangulation<3>::countBoundaryFacets,
            rbase::countBoundaryFacets)
        .def("countBoundaryTriangles",
            &Triangulation<3>::countBoundaryTriangles,
            rdoc::countBoundaryTriangles)
        .def("countBoundaryFaces",
            static_cast<size_t (Triangulation<3>::*)(int) const>(
            &Triangulation<3>::countBoundaryFaces),
            rbase::countBoundaryFaces)
        .def("isClosed", &Triangulation<3>::isClosed, rdoc::isClosed)
        .def("isOrientable", &Triangulation<3>::isOrientable,
            rbase::isOrientable)
        .def("isOriented", &Triangulation<3>::isOriented, rbase::isOriented)
        .def("isOrdered", &Triangulation<3>::isOrdered, rdoc::isOrdered)
        .def("isConnected", &Triangulation<3>::isConnected,
            rbase::isConnected)
        .def("group", &Triangulation<3>::group,
            pybind11::return_value_policy::reference_internal, rbase::group)
        .def("fundamentalGroup", &Triangulation<3>::fundamentalGroup,
            pybind11::return_value_policy::reference_internal,
            rbase::fundamentalGroup)
        .def("simplifiedFundamentalGroup",
            &Triangulation<3>::simplifiedFundamentalGroup,
            rbase::simplifiedFundamentalGroup)
        .def("homology",
            static_cast<AbelianGroup (Triangulation<3>::*)(int) const>(
                &Triangulation<3>::homology),
            pybind11::arg("k") = 1, rbase::homology)
        .def("homologyRel", &Triangulation<3>::homologyRel,
            pybind11::return_value_policy::reference_internal,
            rdoc::homologyRel)
        .def("homologyBdry", &Triangulation<3>::homologyBdry,
            pybind11::return_value_policy::reference_internal,
            rdoc::homologyBdry)
        .def("homologyH2Z2", &Triangulation<3>::homologyH2Z2,
            rdoc::homologyH2Z2)
        .def("markedHomology",
            static_cast<MarkedAbelianGroup (Triangulation<3>::*)(int) const>(
                &Triangulation<3>::markedHomology),
            pybind11::arg("k") = 1, rbase::markedHomology)
        .def("boundaryMap",
            static_cast<MatrixInt (Triangulation<3>::*)(int) const>(
            &Triangulation<3>::boundaryMap), rbase::boundaryMap)
        .def("dualBoundaryMap",
            static_cast<MatrixInt (Triangulation<3>::*)(int) const>(
            &Triangulation<3>::dualBoundaryMap), rbase::dualBoundaryMap)
        .def("dualToPrimal",
            static_cast<MatrixInt (Triangulation<3>::*)(int) const>(
            &Triangulation<3>::dualToPrimal), rbase::dualToPrimal)
        .def("turaevViro", &Triangulation<3>::turaevViro,
            pybind11::arg(), pybind11::arg("parity") = true,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::turaevViro)
        .def("turaevViroApprox", &Triangulation<3>::turaevViroApprox,
            pybind11::arg(), pybind11::arg("whichRoot") = 1,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            rdoc::turaevViroApprox)
        .def("allCalculatedTuraevViro",
            &Triangulation<3>::allCalculatedTuraevViro,
            rdoc::allCalculatedTuraevViro)
        .def("longitudeCuts", &Triangulation<3>::longitudeCuts,
            rdoc::longitudeCuts)
        .def("longitude", &Triangulation<3>::longitude,
            pybind11::return_value_policy::reference_internal, rdoc::longitude)
        .def("meridian", &Triangulation<3>::meridian,
            pybind11::return_value_policy::reference_internal, rdoc::meridian)
        .def("meridianLongitude", &Triangulation<3>::meridianLongitude,
            pybind11::return_value_policy::reference_internal,
            rdoc::meridianLongitude)
        .def("linkingSurface", &Triangulation<3>::linkingSurface<0>,
            rdoc::linkingSurface)
        .def("linkingSurface", &Triangulation<3>::linkingSurface<1>,
            rdoc::linkingSurface)
        .def("linkingSurface", &Triangulation<3>::linkingSurface<2>,
            rdoc::linkingSurface)
        .def("isZeroEfficient", &Triangulation<3>::isZeroEfficient,
            rdoc::isZeroEfficient)
        .def("knowsZeroEfficient", &Triangulation<3>::knowsZeroEfficient,
            rdoc::knowsZeroEfficient)
        .def("hasSplittingSurface", &Triangulation<3>::hasSplittingSurface,
            rdoc::hasSplittingSurface)
        .def("nonTrivialSphereOrDisc",
            &Triangulation<3>::nonTrivialSphereOrDisc,
            rdoc::nonTrivialSphereOrDisc)
        .def("octagonalAlmostNormalSphere",
            &Triangulation<3>::octagonalAlmostNormalSphere,
            rdoc::octagonalAlmostNormalSphere)
        .def("strictAngleStructure",
            &Triangulation<3>::strictAngleStructure,
            pybind11::return_value_policy::reference_internal,
            rdoc::strictAngleStructure)
        .def("hasStrictAngleStructure",
            &Triangulation<3>::hasStrictAngleStructure,
            rdoc::hasStrictAngleStructure)
        .def("knowsStrictAngleStructure",
            &Triangulation<3>::knowsStrictAngleStructure,
            rdoc::knowsStrictAngleStructure)
        .def("generalAngleStructure",
            &Triangulation<3>::generalAngleStructure,
            pybind11::return_value_policy::reference_internal,
            rdoc::generalAngleStructure)
        .def("hasGeneralAngleStructure",
            &Triangulation<3>::hasGeneralAngleStructure,
            rdoc::hasGeneralAngleStructure)
        .def("maximalForestInBoundary",
            &Triangulation<3>::maximalForestInBoundary,
            pybind11::return_value_policy::reference_internal,
            rdoc::maximalForestInBoundary)
        .def("maximalForestInSkeleton",
            &Triangulation<3>::maximalForestInSkeleton,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("canJoinBoundaries") = true,
            rdoc::maximalForestInSkeleton)
        .def("intelligentSimplify",
            &Triangulation<3>::intelligentSimplify, rdoc::intelligentSimplify)
        .def("simplifyToLocalMinimum",
            &Triangulation<3>::simplifyToLocalMinimum,
            pybind11::arg("perform") = true, rdoc::simplifyToLocalMinimum)
        .def("simplifyExhaustive", &Triangulation<3>::simplifyExhaustive,
            pybind11::arg("height") = 1,
            pybind11::arg("nThreads") = 1,
            pybind11::arg("tracker") = nullptr,
            pybind11::call_guard<regina::python::GILScopedRelease>(),
            rdoc::simplifyExhaustive)
        .def("retriangulate", [](const Triangulation<3>& tri, int height,
                int threads, const std::function<bool(const std::string&,
                    Triangulation<3>&&)>& action) {
            if (threads == 1) {
                return tri.retriangulate(height, 1, nullptr, action);
            } else {
                GILCallbackManager manager;
                return tri.retriangulate(height, threads, nullptr,
                    [&](const std::string& sig, Triangulation<3>&& t) -> bool {
                        GILCallbackManager<>::ScopedAcquire acquire(manager);
                        return action(sig, std::move(t));
                    });
            }
        }, rdoc::retriangulate)
        .def("minimiseBoundary", &Triangulation<3>::minimiseBoundary,
            rdoc::minimiseBoundary)
        .def("minimizeBoundary", // deprecated
            &Triangulation<3>::minimiseBoundary, rdoc::minimizeBoundary)
        .def("minimiseVertices", &Triangulation<3>::minimiseVertices,
            rdoc::minimiseVertices)
        .def("minimizeVertices", // deprecated
            &Triangulation<3>::minimiseVertices, rdoc::minimizeVertices)
        .def("pachner", &Triangulation<3>::pachner<3>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner)
        .def("pachner", &Triangulation<3>::pachner<2>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner)
        .def("pachner", &Triangulation<3>::pachner<1>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner)
        .def("pachner", &Triangulation<3>::pachner<0>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rbase::pachner)
        .def("fourFourMove", &Triangulation<3>::fourFourMove,
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::fourFourMove)
        .def("twoZeroMove",
            overload_cast<regina::Edge<3>*, bool, bool>(
            &Triangulation<3>::twoZeroMove),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::twoZeroMove)
        .def("twoZeroMove",
            overload_cast<regina::Vertex<3>*, bool, bool>(
            &Triangulation<3>::twoZeroMove),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::twoZeroMove_2)
        .def("twoOneMove", &Triangulation<3>::twoOneMove,
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::twoOneMove)
        .def("zeroTwoMove",
            overload_cast<regina::EdgeEmbedding<3>, int,
                regina::EdgeEmbedding<3>, int, bool, bool>(
                &Triangulation<3>::zeroTwoMove),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::zeroTwoMove)
        .def("zeroTwoMove",
            overload_cast<regina::Edge<3>*, size_t, size_t, bool, bool>(
                &Triangulation<3>::zeroTwoMove),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::zeroTwoMove_2)
        .def("zeroTwoMove",
            overload_cast<regina::Triangle<3>*, int,
                regina::Triangle<3>*, int, bool, bool>(
                &Triangulation<3>::zeroTwoMove),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::zeroTwoMove_3)
        .def("openBook", &Triangulation<3>::openBook,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::openBook)
        .def("closeBook", &Triangulation<3>::closeBook,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::closeBook)
        .def("shellBoundary", &Triangulation<3>::shellBoundary,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::shellBoundary)
        .def("collapseEdge", &Triangulation<3>::collapseEdge,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true,
            rdoc::collapseEdge)
        .def("reorderTetrahedraBFS", &Triangulation<3>::reorderTetrahedraBFS,
            pybind11::arg("reverse") = false, rdoc::reorderTetrahedraBFS)
        .def("orient", &Triangulation<3>::orient, rbase::orient)
        .def("reflect", &Triangulation<3>::reflect, rbase::reflect)
        .def("order", &Triangulation<3>::order,
            pybind11::arg("forceOriented") = false, rdoc::order)
        .def("triangulateComponents", &Triangulation<3>::triangulateComponents,
            rbase::triangulateComponents)
        .def("summands", &Triangulation<3>::summands, rdoc::summands)
        .def("isSphere", &Triangulation<3>::isSphere, rdoc::isSphere)
        .def("knowsSphere", &Triangulation<3>::knowsSphere, rdoc::knowsSphere)
        .def("isBall", &Triangulation<3>::isBall, rdoc::isBall)
        .def("knowsBall", &Triangulation<3>::knowsBall, rdoc::knowsBall)
        .def("isSolidTorus", &Triangulation<3>::isSolidTorus,
            rdoc::isSolidTorus)
        .def("knowsSolidTorus", &Triangulation<3>::knowsSolidTorus,
            rdoc::knowsSolidTorus)
        .def("recogniseHandlebody", &Triangulation<3>::recogniseHandlebody,
            rdoc::recogniseHandlebody)
        .def("isHandlebody", [](const Triangulation<3>&) -> ssize_t {
            // Removed completely in v7.2.  C++ does not get a deprecated
            // alias.  Keep this here for Python users for a little while so
            // that people can see what needs to be done.  We can remove this
            // placeholder routine in Regina 8.0.
            throw std::runtime_error(
                "Triangulation3::isHandlebody() has been renamed to "
                "Triangulation3::recogniseHandlebody(), since treating the "
                "integer return value as a boolean could lead to "
                "misinterpretation.");
        }, R"doc(Old routine that was renamed in Regina 7.2.

Please update your code to use Triangulation3.recogniseHandlebody()
instead, which does exactly what Triangulation3.isHandlebody() used to
do in Regina 7.1.  The name isHandlebody() has _not_ been kept as an
alias, to avoid people misinterpreting the return value as a boolean.)doc")
        .def("knowsHandlebody", &Triangulation<3>::knowsHandlebody,
            rdoc::knowsHandlebody)
        .def("isTxI", &Triangulation<3>::isTxI, rdoc::isTxI)
        .def("knowsTxI", &Triangulation<3>::knowsTxI, rdoc::knowsTxI)
        .def("isIrreducible", &Triangulation<3>::isIrreducible,
            rdoc::isIrreducible)
        .def("knowsIrreducible", &Triangulation<3>::knowsIrreducible,
            rdoc::knowsIrreducible)
        .def("hasCompressingDisc", &Triangulation<3>::hasCompressingDisc,
            rdoc::hasCompressingDisc)
        .def("knowsCompressingDisc", &Triangulation<3>::knowsCompressingDisc,
            rdoc::knowsCompressingDisc)
        .def("hasSimpleCompressingDisc",
            &Triangulation<3>::hasSimpleCompressingDisc,
            rdoc::hasSimpleCompressingDisc)
        .def("isHaken", &Triangulation<3>::isHaken, rdoc::isHaken)
        .def("knowsHaken", &Triangulation<3>::knowsHaken, rdoc::knowsHaken)
        .def("niceTreeDecomposition", &Triangulation<3>::niceTreeDecomposition,
            pybind11::return_value_policy::reference_internal,
            rdoc::niceTreeDecomposition)
        .def("makeDoubleCover", &Triangulation<3>::makeDoubleCover,
            rbase::makeDoubleCover)
        .def("idealToFinite", &Triangulation<3>::idealToFinite,
            rdoc::idealToFinite)
        .def("finiteToIdeal", &Triangulation<3>::finiteToIdeal,
            rbase::finiteToIdeal)
        .def("subdivide", &Triangulation<3>::subdivide, rbase::subdivide)
        .def("barycentricSubdivision", // deprecated
            &Triangulation<3>::subdivide, rbase::barycentricSubdivision)
        .def("pinchEdge", &Triangulation<3>::pinchEdge, rdoc::pinchEdge)
        .def("puncture", &Triangulation<3>::puncture,
            pybind11::arg("tet") = nullptr, rdoc::puncture)
        .def("layerOn", &Triangulation<3>::layerOn,
            pybind11::return_value_policy::reference, rdoc::layerOn)
        .def("fillTorus",
            overload_cast<unsigned long, unsigned long, unsigned long,
                regina::BoundaryComponent<3>*>(
            &Triangulation<3>::fillTorus),
            pybind11::arg(), pybind11::arg(), pybind11::arg(),
            pybind11::arg("bc") = nullptr,
            rdoc::fillTorus)
        .def("fillTorus",
            overload_cast<regina::Edge<3>*, regina::Edge<3>*, regina::Edge<3>*,
                unsigned long, unsigned long, unsigned long>(
            &Triangulation<3>::fillTorus), rdoc::fillTorus_2)
        .def("insertLayeredSolidTorus",
            &Triangulation<3>::insertLayeredSolidTorus,
            pybind11::return_value_policy::reference,
            rdoc::insertLayeredSolidTorus)
        .def("connectedSumWith", &Triangulation<3>::connectedSumWith,
            rdoc::connectedSumWith)
        .def("insertTriangulation", &Triangulation<3>::insertTriangulation,
            rbase::insertTriangulation)
        .def("dehydrate", &Triangulation<3>::dehydrate, rdoc::dehydrate)
        .def_static("rehydrate", &Triangulation<3>::rehydrate, rdoc::rehydrate)
        .def("isoSig", &Triangulation<3>::isoSig<>, rbase::isoSig)
        .def("isoSig_EdgeDegrees",
            &Triangulation<3>::isoSig<regina::IsoSigEdgeDegrees<3>>,
            rbase::isoSig)
        .def("isoSig_RidgeDegrees",
            &Triangulation<3>::isoSig<regina::IsoSigRidgeDegrees<3>>,
            rbase::isoSig)
        .def("isoSigDetail", &Triangulation<3>::isoSigDetail<>,
            rbase::isoSigDetail)
        .def("isoSigDetail_EdgeDegrees",
            &Triangulation<3>::isoSigDetail<regina::IsoSigEdgeDegrees<3>>,
            rbase::isoSigDetail)
        .def("isoSigDetail_RidgeDegrees",
            &Triangulation<3>::isoSigDetail<regina::IsoSigRidgeDegrees<3>>,
            rbase::isoSigDetail)
        .def_static("fromGluings", [](size_t size, const std::vector<
                std::tuple<size_t, int, size_t, regina::Perm<4>>>& g) {
            return Triangulation<3>::fromGluings(size, g.begin(), g.end());
        }, rbase::fromGluings)
        .def_static("fromIsoSig", &Triangulation<3>::fromIsoSig,
            rbase::fromIsoSig)
        .def_static("fromSig", &Triangulation<3>::fromSig, rbase::fromSig)
        .def_static("isoSigComponentSize",
            &Triangulation<3>::isoSigComponentSize, rbase::isoSigComponentSize)
        .def("dumpConstruction", &Triangulation<3>::dumpConstruction,
            rbase::dumpConstruction)
        .def("snapPea", overload_cast<>(
            &Triangulation<3>::snapPea, pybind11::const_), rdoc::snapPea)
        .def("saveSnapPea", &Triangulation<3>::saveSnapPea, rdoc::saveSnapPea)
        .def("recogniser", overload_cast<>(
            &Triangulation<3>::recogniser, pybind11::const_), rdoc::recogniser)
        .def("recognizer", overload_cast<>(
            &Triangulation<3>::recognizer, pybind11::const_), rdoc::recognizer)
        .def("saveRecogniser", &Triangulation<3>::saveRecogniser,
            rdoc::saveRecogniser)
        .def("saveRecognizer", &Triangulation<3>::saveRecognizer,
            rdoc::saveRecognizer)
        .def_static("fromSnapPea", &Triangulation<3>::fromSnapPea,
            rdoc::fromSnapPea)
        .def_readonly_static("dimension", &Triangulation<3>::dimension)
    ;
    regina::python::add_output(c);
    regina::python::add_tight_encoding(c);
    regina::python::packet_eq_operators(c, rbase::__eq, rbase::__ne);
    regina::python::add_packet_data(c);

    regina::python::addListView<decltype(Triangulation<3>().vertices())>(m);
    regina::python::addListView<decltype(Triangulation<3>().edges())>(m);
    regina::python::addListView<decltype(Triangulation<3>().triangles())>(m);
    regina::python::addListView<decltype(Triangulation<3>().tetrahedra())>(m);
    regina::python::addListView<decltype(Triangulation<3>().components())>(m);
    regina::python::addListView<
        decltype(Triangulation<3>().boundaryComponents())>(m);

    auto wrap = regina::python::add_packet_wrapper<Triangulation<3>>(
        m, "PacketOfTriangulation3");
    regina::python::add_packet_constructor<>(wrap, rdoc::__default);
    regina::python::add_packet_constructor<const Triangulation<3>&, bool>(wrap,
        rdoc::__init);
    regina::python::add_packet_constructor<const regina::Link&, bool>(wrap,
        pybind11::arg(), pybind11::arg("simplify") = true,
        rdoc::__init_2);
    regina::python::add_packet_constructor<const std::string&>(wrap,
        rdoc::__init_3);
    wrap.def(pybind11::init([](const regina::python::SnapPyObject& obj) {
        return regina::make_packet<Triangulation<3>>(std::in_place,
            obj.string_);
    }), rdoc::__init_4);

    // We do not define the global swap() yet for Triangulation<3>, since this
    // needs to come *after* the global swap() for the child class
    // SnapPeaTriangulation.  This is because overloads in python/pybind11
    // are handled by walking through the functions one after another
    // until any matching function is found (as opposed to C++, which has a
    // well-defined notion of "best match").  This means that, if we define
    // the Triangulation<3> swap() first, the SnapPeaTriangulation swap()
    // will never be called at all.

    RDOC_SCOPE_END

    addIsoSigClassic<3>(m, "IsoSigClassic3");
    addIsoSigEdgeDegrees<3>(m, "IsoSigEdgeDegrees3");
    // IsoSigEdgeDegrees<3> and IsoSigRidgeDegrees<3> are the same type.
    m.attr("IsoSigRidgeDegrees3") = m.attr("IsoSigEdgeDegrees3");
    addIsoSigPrintable<3>(m, "IsoSigPrintable3");
}

