
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
#include "angle/anglestructure.h"
#include "link/link.h"
#include "packet/container.h"
#include "progress/progresstracker.h"
#include "snappea/snappeatriangulation.h"
#include "surfaces/normalsurface.h"
#include "triangulation/example3.h"
#include "triangulation/isosigtype.h"
#include "triangulation/detail/isosig-impl.h"
#include "../generic/facehelper.h"
#include "../generic/isosig-bindings.h"

using pybind11::overload_cast;
using regina::Example;
using regina::Isomorphism;
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
    auto c = pybind11::class_<Triangulation<3>,
            std::shared_ptr<Triangulation<3>>>(m, "Triangulation3")
        .def(pybind11::init<>())
        .def(pybind11::init<const Triangulation<3>&>())
        .def(pybind11::init<const Triangulation<3>&, bool>())
        .def(pybind11::init([](const regina::Link& link) { // deprecated
            return new Triangulation<3>(link.complement());
        }))
        .def(pybind11::init<const std::string&>())
        .def(pybind11::init([](const regina::python::SnapPyObject& obj) {
            return new Triangulation<3>(obj.string_);
        }))
        .def("isReadOnlySnapshot", &Triangulation<3>::isReadOnlySnapshot)
        .def("inAnyPacket", overload_cast<>(&Triangulation<3>::inAnyPacket))
        .def("isSnapPea", overload_cast<>(&Triangulation<3>::isSnapPea))
        .def("size", &Triangulation<3>::size)
        .def("countTetrahedra", &Triangulation<3>::countTetrahedra)
        .def("tetrahedra", &Triangulation<3>::tetrahedra,
            pybind11::keep_alive<0, 1>())
        .def("simplices", &Triangulation<3>::simplices,
            pybind11::keep_alive<0, 1>())
        // Use a C-style cast because GCC struggles with the overload_cast here:
        .def("tetrahedron", (regina::Simplex<3>* (Triangulation<3>::*)(size_t))(
            &Triangulation<3>::tetrahedron),
            pybind11::return_value_policy::reference_internal)
        .def("simplex",
            overload_cast<size_t>(&Triangulation<3>::simplex),
            pybind11::return_value_policy::reference_internal)
        .def("newTetrahedron", overload_cast<>(
            &Triangulation<3>::newTetrahedron),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplex", overload_cast<>(
            &Triangulation<3>::newSimplex),
            pybind11::return_value_policy::reference_internal)
        .def("newTetrahedron", overload_cast<const std::string&>(
            &Triangulation<3>::newTetrahedron),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplex", overload_cast<const std::string&>(
            &Triangulation<3>::newSimplex),
            pybind11::return_value_policy::reference_internal)
        .def("newSimplices", [](Triangulation<3>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal)
        .def("newTetrahedra", [](Triangulation<3>& t, size_t k) {
            pybind11::tuple ans(k);
            for (size_t i = 0; i < k; ++i)
                ans[i] = t.newSimplex();
            return ans;
        }, pybind11::return_value_policy::reference_internal)
        .def("removeTetrahedron", &Triangulation<3>::removeTetrahedron)
        .def("removeSimplex", &Triangulation<3>::removeSimplex)
        .def("removeTetrahedronAt", &Triangulation<3>::removeTetrahedronAt)
        .def("removeSimplexAt", &Triangulation<3>::removeSimplexAt)
        .def("removeAllTetrahedra", &Triangulation<3>::removeAllTetrahedra)
        .def("removeAllSimplices", &Triangulation<3>::removeAllSimplices)
        .def("swap", &Triangulation<3>::swap)
        .def("swapContents", &Triangulation<3>::swap) // deprecated
        .def("moveContentsTo", &Triangulation<3>::moveContentsTo)
        .def("countComponents", &Triangulation<3>::countComponents)
        .def("countBoundaryComponents",
            &Triangulation<3>::countBoundaryComponents)
        .def("countFaces", (regina::python::countFacesFunc<Triangulation<3>>)(
            &Triangulation<3>::countFaces))
        .def("countVertices", &Triangulation<3>::countVertices)
        .def("countEdges", &Triangulation<3>::countEdges)
        .def("countTriangles", &Triangulation<3>::countTriangles)
        .def("fVector", &Triangulation<3>::fVector)
        .def("components", &Triangulation<3>::components,
            pybind11::keep_alive<0, 1>())
        .def("boundaryComponents", &Triangulation<3>::boundaryComponents,
            pybind11::keep_alive<0, 1>())
        .def("faces", (regina::python::facesFunc<Triangulation<3>>)(
            &Triangulation<3>::faces),
            pybind11::keep_alive<0, 1>())
        .def("vertices", &Triangulation<3>::vertices,
            pybind11::keep_alive<0, 1>())
        .def("edges", &Triangulation<3>::edges,
            pybind11::keep_alive<0, 1>())
        .def("triangles", &Triangulation<3>::triangles,
            pybind11::keep_alive<0, 1>())
        .def("component", &Triangulation<3>::component,
            pybind11::return_value_policy::reference_internal)
        .def("boundaryComponent", &Triangulation<3>::boundaryComponent,
            pybind11::return_value_policy::reference_internal)
        .def("face", (regina::python::faceFunc<Triangulation<3>>)(
            &Triangulation<3>::face),
            pybind11::return_value_policy::reference_internal)
        .def("vertex", &Triangulation<3>::vertex,
            pybind11::return_value_policy::reference_internal)
        .def("edge", &Triangulation<3>::edge,
            pybind11::return_value_policy::reference_internal)
        // Use a C-style cast because GCC struggles with the overload_cast here:
        .def("triangle", (regina::Face<3, 2>* (Triangulation<3>::*)(size_t))(
            &Triangulation<3>::triangle),
            pybind11::return_value_policy::reference_internal)
        .def("isIdenticalTo", &Triangulation<3>::isIdenticalTo)
        .def("isIsomorphicTo", &Triangulation<3>::isIsomorphicTo)
        .def("findAllIsomorphisms", &Triangulation<3>::findAllIsomorphisms<
                const std::function<bool(const Isomorphism<3>)>&>)
        .def("findAllIsomorphisms", [](const Triangulation<3>& t,
                const Triangulation<3>& other) {
            std::vector<Isomorphism<3>> isos;
            t.findAllIsomorphisms(other, [&](const Isomorphism<3>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        })
        .def("findAllSubcomplexesIn", &Triangulation<3>::findAllSubcomplexesIn<
                const std::function<bool(const Isomorphism<3>)>&>)
        .def("findAllSubcomplexesIn", [](const Triangulation<3>& t,
                const Triangulation<3>& other) {
            std::vector<Isomorphism<3>> isos;
            t.findAllSubcomplexesIn(other, [&](const Isomorphism<3>& iso) {
                isos.push_back(iso);
                return false;
            });
            return isos;
        })
        .def("makeCanonical", &Triangulation<3>::makeCanonical)
        .def("isContainedIn", &Triangulation<3>::isContainedIn)
        .def("hasTwoSphereBoundaryComponents",
            &Triangulation<3>::hasTwoSphereBoundaryComponents)
        .def("hasNegativeIdealBoundaryComponents",
            &Triangulation<3>::hasNegativeIdealBoundaryComponents)
        .def("isEmpty", &Triangulation<3>::isEmpty)
        .def("eulerCharTri", &Triangulation<3>::eulerCharTri)
        .def("eulerCharManifold", &Triangulation<3>::eulerCharManifold)
        .def("isValid", &Triangulation<3>::isValid)
        .def("isIdeal", &Triangulation<3>::isIdeal)
        .def("isStandard", &Triangulation<3>::isStandard)
        .def("hasBoundaryFacets", &Triangulation<3>::hasBoundaryFacets)
        .def("hasBoundaryTriangles", &Triangulation<3>::hasBoundaryTriangles)
        .def("countBoundaryFacets", &Triangulation<3>::countBoundaryFacets)
        .def("countBoundaryTriangles", &Triangulation<3>::countBoundaryTriangles)
        .def("isClosed", &Triangulation<3>::isClosed)
        .def("isOrientable", &Triangulation<3>::isOrientable)
        .def("isOriented", &Triangulation<3>::isOriented)
        .def("isOrdered", &Triangulation<3>::isOrdered)
        .def("isConnected", &Triangulation<3>::isConnected)
        .def("fundamentalGroup", &Triangulation<3>::fundamentalGroup,
            pybind11::return_value_policy::reference_internal)
        .def("simplifiedFundamentalGroup",
            &Triangulation<3>::simplifiedFundamentalGroup)
        .def("homology", &Triangulation<3>::homology,
            pybind11::return_value_policy::reference_internal)
        .def("homologyH1", &Triangulation<3>::homologyH1,
            pybind11::return_value_policy::reference_internal)
        .def("homologyRel", &Triangulation<3>::homologyRel,
            pybind11::return_value_policy::reference_internal)
        .def("homologyBdry", &Triangulation<3>::homologyBdry,
            pybind11::return_value_policy::reference_internal)
        .def("homologyH2", &Triangulation<3>::homologyH2,
            pybind11::return_value_policy::reference_internal)
        .def("homologyH2Z2", &Triangulation<3>::homologyH2Z2)
        .def("boundaryMap", (MatrixInt (Triangulation<3>::*)(int) const)(
            &Triangulation<3>::boundaryMap))
        .def("turaevViro", &Triangulation<3>::turaevViro,
            pybind11::arg(), pybind11::arg("parity") = true,
            pybind11::arg("alg") = regina::ALG_DEFAULT,
            pybind11::arg("tracker") = nullptr)
        .def("turaevViroApprox", &Triangulation<3>::turaevViroApprox,
            pybind11::arg(), pybind11::arg("whichRoot") = 1,
            pybind11::arg("alg") = regina::ALG_DEFAULT)
        .def("allCalculatedTuraevViro",
            &Triangulation<3>::allCalculatedTuraevViro)
        .def("longitude", &Triangulation<3>::longitude,
            pybind11::return_value_policy::reference_internal)
        .def("meridianLongitude", &Triangulation<3>::meridianLongitude,
            pybind11::return_value_policy::reference_internal)
        .def("isZeroEfficient", &Triangulation<3>::isZeroEfficient)
        .def("knowsZeroEfficient", &Triangulation<3>::knowsZeroEfficient)
        .def("hasSplittingSurface", &Triangulation<3>::hasSplittingSurface)
        .def("nonTrivialSphereOrDisc",
            &Triangulation<3>::nonTrivialSphereOrDisc)
        .def("hasNonTrivialSphereOrDisc", // deprecated
            &Triangulation<3>::nonTrivialSphereOrDisc)
        .def("octagonalAlmostNormalSphere",
            &Triangulation<3>::octagonalAlmostNormalSphere)
        .def("hasOctagonalAlmostNormalSphere", // deprecated
            &Triangulation<3>::octagonalAlmostNormalSphere)
        .def("strictAngleStructure",
            &Triangulation<3>::strictAngleStructure,
            pybind11::return_value_policy::reference_internal)
        .def("findStrictAngleStructure", // deprecated
            &Triangulation<3>::strictAngleStructure,
            pybind11::return_value_policy::reference_internal)
        .def("hasStrictAngleStructure",
            &Triangulation<3>::hasStrictAngleStructure)
        .def("knowsStrictAngleStructure",
            &Triangulation<3>::knowsStrictAngleStructure)
        .def("generalAngleStructure",
            &Triangulation<3>::generalAngleStructure,
            pybind11::return_value_policy::reference_internal)
        .def("hasGeneralAngleStructure",
            &Triangulation<3>::hasGeneralAngleStructure)
        .def("maximalForestInBoundary",
            &Triangulation<3>::maximalForestInBoundary,
            pybind11::return_value_policy::reference_internal)
        .def("maximalForestInSkeleton",
            &Triangulation<3>::maximalForestInSkeleton,
            pybind11::return_value_policy::reference_internal,
            pybind11::arg("canJoinBoundaries") = true)
        .def("intelligentSimplify", &Triangulation<3>::intelligentSimplify)
        .def("simplifyToLocalMinimum",
            &Triangulation<3>::simplifyToLocalMinimum,
            pybind11::arg("perform") = true)
        .def("simplifyExhaustive", &Triangulation<3>::simplifyExhaustive,
            pybind11::arg("height") = 1,
            pybind11::arg("nThreads") = 1,
            pybind11::arg("tracker") = nullptr)
        .def("retriangulate", [](const Triangulation<3>& tri, int height,
                int threads, const std::function<bool(const std::string&,
                    Triangulation<3>&&)>& action) {
            if (threads == 1) {
                return tri.retriangulate(height, 1, nullptr, action);
            } else {
                pybind11::gil_scoped_release release;
                return tri.retriangulate(height, threads, nullptr,
                    [&](const std::string& sig, Triangulation<3>&& t) -> bool {
                        pybind11::gil_scoped_acquire acquire;
                        return action(sig, std::move(t));
                    });
            }
        })
        .def("minimiseBoundary", &Triangulation<3>::minimiseBoundary)
        .def("minimizeBoundary", &Triangulation<3>::minimizeBoundary)
        .def("pachner", &Triangulation<3>::pachner<3>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("pachner", &Triangulation<3>::pachner<2>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("pachner", &Triangulation<3>::pachner<1>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("pachner", &Triangulation<3>::pachner<0>,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("fourFourMove", &Triangulation<3>::fourFourMove,
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("twoZeroMove",
            overload_cast<regina::Vertex<3>*, bool, bool>(
            &Triangulation<3>::twoZeroMove),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("twoZeroMove",
            overload_cast<regina::Edge<3>*, bool, bool>(
            &Triangulation<3>::twoZeroMove),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("twoOneMove", &Triangulation<3>::twoOneMove,
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("zeroTwoMove",
            overload_cast< regina::EdgeEmbedding<3>, int,
            regina::EdgeEmbedding<3>, int, bool, bool >(
                &Triangulation<3>::zeroTwoMove ),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true )
        .def("zeroTwoMove",
            overload_cast< regina::Edge<3>*, size_t, size_t,
            bool, bool >(
                &Triangulation<3>::zeroTwoMove ),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true )
        .def("zeroTwoMove",
            overload_cast< regina::Triangle<3>*, int,
            regina::Triangle<3>*, int, bool, bool >(
                &Triangulation<3>::zeroTwoMove ),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true )
        .def("openBook", &Triangulation<3>::openBook,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("closeBook", &Triangulation<3>::closeBook,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("shellBoundary", &Triangulation<3>::shellBoundary,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("collapseEdge", &Triangulation<3>::collapseEdge,
            pybind11::arg(),
            pybind11::arg("check") = true,
            pybind11::arg("perform") = true)
        .def("reorderTetrahedraBFS", &Triangulation<3>::reorderTetrahedraBFS,
            pybind11::arg("reverse") = false)
        .def("orient", &Triangulation<3>::orient)
        .def("reflect", &Triangulation<3>::reflect)
        .def("order", &Triangulation<3>::order,
            pybind11::arg("forceOriented") = false)
        .def("triangulateComponents", &Triangulation<3>::triangulateComponents)
        .def("summands", &Triangulation<3>::summands)
        .def("isSphere", &Triangulation<3>::isSphere)
        .def("isThreeSphere", &Triangulation<3>::isSphere) // deprecated
        .def("knowsSphere", &Triangulation<3>::knowsSphere)
        .def("knowsThreeSphere", &Triangulation<3>::knowsSphere) // deprecated
        .def("isBall", &Triangulation<3>::isBall)
        .def("knowsBall", &Triangulation<3>::knowsBall)
        .def("isSolidTorus", &Triangulation<3>::isSolidTorus)
        .def("knowsSolidTorus", &Triangulation<3>::knowsSolidTorus)
        .def("isTxI", &Triangulation<3>::isTxI)
        .def("knowsTxI", &Triangulation<3>::knowsTxI)
        .def("isIrreducible", &Triangulation<3>::isIrreducible)
        .def("knowsIrreducible", &Triangulation<3>::knowsIrreducible)
        .def("hasCompressingDisc", &Triangulation<3>::hasCompressingDisc)
        .def("knowsCompressingDisc", &Triangulation<3>::knowsCompressingDisc)
        .def("hasSimpleCompressingDisc",
            &Triangulation<3>::hasSimpleCompressingDisc)
        .def("isHaken", &Triangulation<3>::isHaken)
        .def("knowsHaken", &Triangulation<3>::knowsHaken)
        .def("niceTreeDecomposition", &Triangulation<3>::niceTreeDecomposition,
            pybind11::return_value_policy::reference_internal)
        .def("makeDoubleCover", &Triangulation<3>::makeDoubleCover)
        .def("idealToFinite", &Triangulation<3>::idealToFinite)
        .def("finiteToIdeal", &Triangulation<3>::finiteToIdeal)
        .def("barycentricSubdivision", &Triangulation<3>::barycentricSubdivision)
        .def("pinchEdge", &Triangulation<3>::pinchEdge)
        .def("puncture", &Triangulation<3>::puncture,
            pybind11::arg("tet") = nullptr)
        .def("layerOn", &Triangulation<3>::layerOn,
            pybind11::return_value_policy::reference)
        .def("fillTorus",
            overload_cast<unsigned long, unsigned long, unsigned long,
                regina::BoundaryComponent<3>*>(
            &Triangulation<3>::fillTorus),
            pybind11::arg(), pybind11::arg(), pybind11::arg(),
            pybind11::arg("bc") = nullptr)
        .def("fillTorus",
            overload_cast<regina::Edge<3>*, regina::Edge<3>*, regina::Edge<3>*,
                unsigned long, unsigned long, unsigned long>(
            &Triangulation<3>::fillTorus))
        .def("insertLayeredSolidTorus",
            &Triangulation<3>::insertLayeredSolidTorus,
            pybind11::return_value_policy::reference)
        .def("insertLayeredLensSpace", // deprecated
                [](Triangulation<3>& t, size_t p, size_t q) {
            t.insertTriangulation(Example<3>::lens(p, q));
        })
        .def("insertLayeredLoop", // deprecated
                [](Triangulation<3>& t, size_t len, bool twisted) {
            t.insertTriangulation(Example<3>::layeredLoop(len, twisted));
        })
        .def("insertAugTriSolidTorus", // deprecated
                [](Triangulation<3>& t, long a1, long a2, long b1, long b2,
                    long c1, long c2) {
            t.insertTriangulation(Example<3>::augTriSolidTorus(
                a1, a2, b1, b2, c1, c2));
        })
        .def("insertSFSOverSphere", // deprecated
                [](Triangulation<3>& t, long a1, long a2, long b1, long b2,
                    long c1, long c2) {
            t.insertTriangulation(Example<3>::sfsOverSphere(
                a1, a2, b1, b2, c1, c2));
        })
        .def("connectedSumWith", &Triangulation<3>::connectedSumWith)
        .def("insertTriangulation", &Triangulation<3>::insertTriangulation)
        .def("insertRehydration", [](Triangulation<3>& tri,
                const std::string& str) { // deprecated
            tri.insertTriangulation(Triangulation<3>::rehydrate(str));
        })
        .def("dehydrate", &Triangulation<3>::dehydrate)
        .def_static("rehydrate", &Triangulation<3>::rehydrate)
        .def("isoSig", &Triangulation<3>::isoSig<>)
        .def("isoSig_EdgeDegrees",
            &Triangulation<3>::isoSig<regina::IsoSigEdgeDegrees<3>>)
        .def("isoSig_RidgeDegrees",
            &Triangulation<3>::isoSig<regina::IsoSigRidgeDegrees<3>>)
        .def("isoSigDetail", &Triangulation<3>::isoSigDetail<>)
        .def("isoSigDetail_EdgeDegrees",
            &Triangulation<3>::isoSigDetail<regina::IsoSigEdgeDegrees<3>>)
        .def("isoSigDetail_RidgeDegrees",
            &Triangulation<3>::isoSigDetail<regina::IsoSigRidgeDegrees<3>>)
        .def_static("fromGluings", [](size_t size, const std::vector<
                std::tuple<size_t, int, size_t, regina::Perm<4>>>& g) {
            return Triangulation<3>::fromGluings(size, g.begin(), g.end());
        })
        .def_static("fromIsoSig", &Triangulation<3>::fromIsoSig)
        .def_static("fromSig", &Triangulation<3>::fromSig)
        .def_static("isoSigComponentSize",
            &Triangulation<3>::isoSigComponentSize)
        .def("dumpConstruction", &Triangulation<3>::dumpConstruction)
        .def("snapPea", overload_cast<>(
            &Triangulation<3>::snapPea, pybind11::const_))
        .def("saveSnapPea", &Triangulation<3>::saveSnapPea)
        .def("recogniser", overload_cast<>(
            &Triangulation<3>::recogniser, pybind11::const_))
        .def("recognizer", overload_cast<>(
            &Triangulation<3>::recognizer, pybind11::const_))
        .def("saveRecogniser", &Triangulation<3>::saveRecogniser)
        .def("saveRecognizer", &Triangulation<3>::saveRecognizer)
        .def_static("fromSnapPea", &Triangulation<3>::fromSnapPea)
        .def_static("enterTextTriangulation", []() { // deprecated
            return Triangulation<3>::enterTextTriangulation(
                std::cin, std::cout);
        })
        .def_readonly_static("dimension", &Triangulation<3>::dimension)
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    regina::python::addListView<decltype(Triangulation<3>().vertices())>(m);
    regina::python::addListView<decltype(Triangulation<3>().edges())>(m);
    regina::python::addListView<decltype(Triangulation<3>().triangles())>(m);
    regina::python::addListView<decltype(Triangulation<3>().tetrahedra())>(m);
    regina::python::addListView<decltype(Triangulation<3>().components())>(m);
    regina::python::addListView<
        decltype(Triangulation<3>().boundaryComponents())>(m);

    auto wrap = regina::python::add_packet_wrapper<Triangulation<3>>(
        m, "PacketOfTriangulation3");
    regina::python::add_packet_constructor<>(wrap);
    regina::python::add_packet_constructor<const Triangulation<3>&, bool>(wrap);
    regina::python::add_packet_constructor<const std::string&>(wrap);
    wrap.def(pybind11::init([](const regina::Link& link) { // deprecated
        return regina::make_packet<Triangulation<3>>(link.complement());
    }));
    wrap.def(pybind11::init([](const regina::python::SnapPyObject& obj) {
        return regina::make_packet<Triangulation<3>>(std::in_place,
            obj.string_);
    }));

    // We do not define the global swap() yet for Triangulation<3>, since this
    // needs to come *after* the global swap() for the child class
    // SnapPeaTriangulation.  This is because overloads in python/pybind11
    // are handled by walking through the functions one after another
    // until any matching function is found (as opposed to C++, which has a
    // well-defined notion of "best match").  This means that, if we define
    // the Triangulation<3> swap() first, the SnapPeaTriangulation swap()
    // will never be called at all.

    addIsoSigClassic<3>(m, "IsoSigClassic3");
    addIsoSigEdgeDegrees<3>(m, "IsoSigEdgeDegrees3");
    // IsoSigEdgeDegrees<3> and IsoSigRidgeDegrees<3> are the same type.
    m.attr("IsoSigRidgeDegrees3") = m.attr("IsoSigEdgeDegrees3");
    addIsoSigPrintable<3>(m, "IsoSigPrintable3");
}

