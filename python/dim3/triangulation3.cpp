
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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
#include "angle/anglestructure.h"
#include "progress/progresstracker.h"
#include "surfaces/normalsurface.h"
#include "triangulation/dim3.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

using pybind11::overload_cast;
using regina::Triangulation;
using regina::detail::TriangulationBase;

namespace {
#if 0
    regina::Tetrahedron<3>* (Triangulation<3>::*newTetrahedron_void)() =
        &Triangulation<3>::newTetrahedron;
    regina::Tetrahedron<3>* (Triangulation<3>::*newTetrahedron_string)(
        const std::string&) = &Triangulation<3>::newTetrahedron;
    bool (Triangulation<3>::*twoZeroMove_vertex)(regina::Vertex<3>*, bool, bool) =
        &Triangulation<3>::twoZeroMove;
    bool (Triangulation<3>::*twoZeroMove_edge)(regina::Edge<3>*, bool, bool) =
        &Triangulation<3>::twoZeroMove;
    std::string (Triangulation<3>::*snapPea_void)() const =
        &Triangulation<3>::snapPea;
    std::string (Triangulation<3>::*recogniser_void)() const =
        &Triangulation<3>::recogniser;
    std::string (Triangulation<3>::*recognizer_void)() const =
        &Triangulation<3>::recognizer;
    size_t (Triangulation<3>::*splitIntoComponents)(regina::Packet*, bool) =
        &Triangulation<3>::splitIntoComponents;
    bool (Triangulation<3>::*pachner_14)(regina::Simplex<3>*, bool, bool) =
        &Triangulation<3>::pachner;
    bool (Triangulation<3>::*pachner_23)(regina::Triangle<3>*, bool, bool) =
        &Triangulation<3>::pachner;
    bool (Triangulation<3>::*pachner_32)(regina::Edge<3>*, bool, bool) =
        &Triangulation<3>::pachner;
    bool (Triangulation<3>::*pachner_41)(regina::Vertex<3>*, bool, bool) =
        &Triangulation<3>::pachner;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_fillTorus_bc,
        Triangulation<3>::fillTorus, 3, 4);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_pachner,
        Triangulation<3>::pachner, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_oneFourMove,
        Triangulation<3>::oneFourMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplifyToLocalMinimum,
        Triangulation<3>::simplifyToLocalMinimum, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplifyExhaustive,
        Triangulation<3>::simplifyExhaustive, 0, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_fourFourMove,
        Triangulation<3>::fourFourMove, 2, 4);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_twoZeroMove,
        Triangulation<3>::twoZeroMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_twoOneMove,
        Triangulation<3>::twoOneMove, 2, 4);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_openBook,
        Triangulation<3>::openBook, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_closeBook,
        Triangulation<3>::closeBook, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_shellBoundary,
        Triangulation<3>::shellBoundary, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_collapseEdge,
        Triangulation<3>::collapseEdge, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_reorderTetrahedraBFS,
        Triangulation<3>::reorderTetrahedraBFS, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_order,
        Triangulation<3>::order, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_splitIntoComponents,
        Triangulation<3>::splitIntoComponents, 0, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_connectedSumDecomposition,
        Triangulation<3>::connectedSumDecomposition, 0, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_puncture,
        Triangulation<3>::puncture, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_turaevViro,
        Triangulation<3>::turaevViro, 1, 4);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_turaevViroApprox,
        Triangulation<3>::turaevViroApprox, 1, 3);

    void simplifiedFundamentalGroup_clone(Triangulation<3>& tri,
            const regina::GroupPresentation& group) {
        tri.simplifiedFundamentalGroup(new regina::GroupPresentation(group));
    }

    boost::python::list components_list(Triangulation<3>& t) {
        boost::python::list ans;
        for (auto c : t.components())
            ans.append(boost::python::ptr(c));
        return ans;
    }

    boost::python::list boundaryComponents_list(Triangulation<3>& t) {
        boost::python::list ans;
        for (auto b : t.boundaryComponents())
            ans.append(boost::python::ptr(b));
        return ans;
    }

    boost::python::tuple meridianLongitude_tuple(Triangulation<3>& t) {
        std::pair<const regina::Edge<3>*, const regina::Edge<3>*> ans =
            t.meridianLongitude();
        return boost::python::make_tuple(boost::python::ptr(ans.first),
            boost::python::ptr(ans.second));
    }
#endif
}

void addTriangulation3(pybind11::module& m) {
    auto c = pybind11::class_<Triangulation<3>, regina::Packet,
            regina::python::SafeHeldType<Triangulation<3>>>(m, "Triangulation3")
        .def(pybind11::init<>())
        .def(pybind11::init<const Triangulation<3>&>())
        .def(pybind11::init<const std::string&>())
        .def("size", &Triangulation<3>::size)
        .def("countTetrahedra", &Triangulation<3>::countTetrahedra)
        .def("tetrahedra", &Triangulation<3>::tetrahedra,
            pybind11::return_value_policy::reference_internal)
        .def("simplices", &Triangulation<3>::simplices,
            pybind11::return_value_policy::reference_internal)
        .def("tetrahedron",
            overload_cast<size_t>(&Triangulation<3>::tetrahedron),
            pybind11::return_value_policy::reference_internal)
        .def("simplex",
            overload_cast<size_t>(&Triangulation<3>::simplex),
            pybind11::return_value_policy::reference_internal)
        // .def("newTetrahedron", newTetrahedron_void,
            // pybind11::return_value_policy::reference_internal)
        // .def("newSimplex", newTetrahedron_void,
            // pybind11::return_value_policy::reference_internal)
        // .def("newTetrahedron", newTetrahedron_string,
            // pybind11::return_value_policy::reference_internal)
        // .def("newSimplex", newTetrahedron_string,
            // pybind11::return_value_policy::reference_internal)
        .def("removeTetrahedron", &Triangulation<3>::removeTetrahedron)
        .def("removeSimplex", &Triangulation<3>::removeSimplex)
        .def("removeTetrahedronAt", &Triangulation<3>::removeTetrahedronAt)
        .def("removeSimplexAt", &Triangulation<3>::removeSimplexAt)
        .def("removeAllTetrahedra", &Triangulation<3>::removeAllTetrahedra)
        .def("removeAllSimplices", &Triangulation<3>::removeAllSimplices)
        .def("swapContents", &Triangulation<3>::swapContents)
        .def("moveContentsTo", &Triangulation<3>::moveContentsTo)
        .def("countComponents", &Triangulation<3>::countComponents)
        .def("countBoundaryComponents",
            &Triangulation<3>::countBoundaryComponents)
        .def("countFaces", &regina::python::countFaces<Triangulation<3>, 3>)
        .def("countVertices", &Triangulation<3>::countVertices)
        .def("countEdges", &Triangulation<3>::countEdges)
        .def("countTriangles", &Triangulation<3>::countTriangles)
        .def("fVector", &Triangulation<3>::fVector)
        // .def("components", components_list)
        // .def("boundaryComponents", boundaryComponents_list)
        .def("faces", &regina::python::faces<Triangulation<3>, 3>)
        // .def("vertices", regina::python::faces_list<Triangulation<3>, 3, 0>)
        // .def("edges", regina::python::faces_list<Triangulation<3>, 3, 1>)
        .def("triangles", regina::python::faces_list<Triangulation<3>, 3, 2>)
        .def("component", &Triangulation<3>::component,
            pybind11::return_value_policy::reference_internal)
        .def("boundaryComponent", &Triangulation<3>::boundaryComponent,
            pybind11::return_value_policy::reference_internal)
        .def("face", &regina::python::face<Triangulation<3>, 3, size_t>)
        .def("vertex", &Triangulation<3>::vertex,
            pybind11::return_value_policy::reference_internal)
        .def("edge", &Triangulation<3>::edge,
            pybind11::return_value_policy::reference_internal)
        .def("triangle", &Triangulation<3>::triangle,
            pybind11::return_value_policy::reference_internal)
        .def("isIdenticalTo", &Triangulation<3>::isIdenticalTo)
        .def("isIsomorphicTo", [](const Triangulation<3>& t,
                const Triangulation<3>& s) {
            return t.isIsomorphicTo(s).release();
        })
        .def("findAllIsomorphisms", [](const Triangulation<3>& t,
                const Triangulation<3>& other) {
            std::list<regina::Isomorphism<3>*> isos;
            t.findAllIsomorphisms(other, back_inserter(isos));
            return isos;
        })
        .def("findAllSubcomplexesIn", [](const Triangulation<3>& t,
                const Triangulation<3>& other) {
            std::list<regina::Isomorphism<3>*> isos;
            t.findAllSubcomplexesIn(other, back_inserter(isos));
            return isos;
        })
        .def("makeCanonical", &Triangulation<3>::makeCanonical)
        .def("isContainedIn", [](const Triangulation<3>& t,
                const Triangulation<3>& s) {
            return t.isContainedIn(s).release();
        })
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
        // .def("simplifiedFundamentalGroup", simplifiedFundamentalGroup_clone)
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
        // .def("turaevViro", &Triangulation<3>::turaevViro, OL_turaevViro())
        // .def("turaevViroApprox", &Triangulation<3>::turaevViroApprox,
            // OL_turaevViroApprox())
        .def("longitude", &Triangulation<3>::longitude,
            pybind11::return_value_policy::reference_internal)
        // .def("meridianLongitude", meridianLongitude_tuple)
        .def("isZeroEfficient", &Triangulation<3>::isZeroEfficient)
        .def("knowsZeroEfficient", &Triangulation<3>::knowsZeroEfficient)
        .def("hasSplittingSurface", &Triangulation<3>::hasSplittingSurface)
        .def("knowsSplittingSurface", &Triangulation<3>::knowsSplittingSurface)
        .def("hasNonTrivialSphereOrDisc",
            &Triangulation<3>::hasNonTrivialSphereOrDisc)
        .def("hasOctagonalAlmostNormalSphere",
            &Triangulation<3>::hasOctagonalAlmostNormalSphere)
        .def("findStrictAngleStructure",
            &Triangulation<3>::findStrictAngleStructure,
            pybind11::return_value_policy::reference_internal)
        .def("hasStrictAngleStructure",
            &Triangulation<3>::hasStrictAngleStructure)
        .def("knowsStrictAngleStructure",
            &Triangulation<3>::knowsStrictAngleStructure)
        .def("intelligentSimplify", &Triangulation<3>::intelligentSimplify)
        // .def("simplifyToLocalMinimum",
            // &Triangulation<3>::simplifyToLocalMinimum,
            // OL_simplifyToLocalMinimum())
        // .def("simplifyExhaustive", &Triangulation<3>::simplifyExhaustive,
            // OL_simplifyExhaustive())
        // .def("pachner", pachner_14, OL_pachner())
        // .def("pachner", pachner_23, OL_pachner())
        // .def("pachner", pachner_32, OL_pachner())
        // .def("pachner", pachner_41, OL_pachner())
        // .def("oneFourMove", &Triangulation<3>::oneFourMove,
            // OL_oneFourMove())
        // .def("twoThreeMove", pachner_23, OL_pachner())
        // .def("threeTwoMove", pachner_32, OL_pachner())
        // .def("fourOneMove", pachner_41, OL_pachner())
        // .def("fourFourMove", &Triangulation<3>::fourFourMove, OL_fourFourMove())
        // .def("twoZeroMove", twoZeroMove_vertex, OL_twoZeroMove())
        // .def("twoZeroMove", twoZeroMove_edge, OL_twoZeroMove())
        // .def("twoOneMove", &Triangulation<3>::twoOneMove, OL_twoOneMove())
        // .def("openBook", &Triangulation<3>::openBook, OL_openBook())
        // .def("closeBook", &Triangulation<3>::closeBook, OL_closeBook())
        // .def("shellBoundary", &Triangulation<3>::shellBoundary,
            // OL_shellBoundary())
        // .def("collapseEdge", &Triangulation<3>::collapseEdge, OL_collapseEdge())
        // .def("reorderTetrahedraBFS", &Triangulation<3>::reorderTetrahedraBFS,
            // OL_reorderTetrahedraBFS())
        .def("orient", &Triangulation<3>::orient)
        .def("reflect", &Triangulation<3>::reflect)
        // .def("order", &Triangulation<3>::order, OL_order(args("force_oriented")))
        // .def("splitIntoComponents", splitIntoComponents,
            // OL_splitIntoComponents())
        // .def("connectedSumDecomposition",
            // &Triangulation<3>::connectedSumDecomposition,
            // OL_connectedSumDecomposition())
        .def("isThreeSphere", &Triangulation<3>::isThreeSphere)
        .def("knowsThreeSphere", &Triangulation<3>::knowsThreeSphere)
        .def("isBall", &Triangulation<3>::isBall)
        .def("knowsBall", &Triangulation<3>::knowsBall)
        .def("isSolidTorus", &Triangulation<3>::isSolidTorus)
        .def("knowsSolidTorus", &Triangulation<3>::knowsSolidTorus)
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
        .def("drillEdge", &Triangulation<3>::drillEdge)
        // .def("puncture", &Triangulation<3>::puncture, OL_puncture())
        .def("layerOn", &Triangulation<3>::layerOn,
            pybind11::return_value_policy::reference)
        // .def("fillTorus",
            // overload_cast<unsigned long, unsigned long, unsigned long,
                // regina::BoundaryComponent<3>*>(
                // &Triangulation<3>::fillTorus),
            // OL_fillTorus_bc())
        .def("fillTorus",
            overload_cast<regina::Edge<3>*, regina::Edge<3>*, regina::Edge<3>*,
                unsigned long, unsigned long, unsigned long>(
            &Triangulation<3>::fillTorus))
        .def("insertLayeredSolidTorus",
            &Triangulation<3>::insertLayeredSolidTorus,
            pybind11::return_value_policy::reference)
        .def("insertLayeredLensSpace",
            &Triangulation<3>::insertLayeredLensSpace)
        .def("insertLayeredLoop", &Triangulation<3>::insertLayeredLoop)
        .def("insertAugTriSolidTorus",
            &Triangulation<3>::insertAugTriSolidTorus)
        .def("insertSFSOverSphere", &Triangulation<3>::insertSFSOverSphere)
        .def("connectedSumWith", &Triangulation<3>::connectedSumWith)
        .def("insertTriangulation", &Triangulation<3>::insertTriangulation)
        .def("insertRehydration", &Triangulation<3>::insertRehydration)
        .def("dehydrate", &Triangulation<3>::dehydrate)
        .def_static("rehydrate", &Triangulation<3>::rehydrate)
        .def("isoSig", [](const Triangulation<3>& t) {
            return t.isoSig();
        })
        .def("isoSigDetail", [](const Triangulation<3>& t) {
            regina::Isomorphism<3>* iso;
            std::string sig = t.isoSig(&iso);
            return pybind11::make_tuple(sig, iso);
        })
        .def_static("fromIsoSig", &Triangulation<3>::fromIsoSig)
        .def_static("isoSigComponentSize",
            &Triangulation<3>::isoSigComponentSize)
        .def("dumpConstruction", &Triangulation<3>::dumpConstruction)
        // .def("snapPea", snapPea_void)
        .def("saveSnapPea", &Triangulation<3>::saveSnapPea)
        // .def("recogniser", recogniser_void)
        // .def("recognizer", recognizer_void)
        .def("saveRecogniser", &Triangulation<3>::saveRecogniser)
        .def("saveRecognizer", &Triangulation<3>::saveRecognizer)
        .def_static("fromSnapPea", &Triangulation<3>::fromSnapPea)
        .def_static("enterTextTriangulation", []() {
            return Triangulation<3>::enterTextTriangulation(
                std::cin, std::cout);
        })
        // We cannot take the addresses of the following properties, so we
        // define getter functions instead.
        .def_property_readonly_static("typeID", [](pybind11::object) {
            return Triangulation<3>::typeID;
        })
        .def_property_readonly_static("dimension", [](pybind11::object) {
            return 3;
        })
    ;

    m.attr("NTriangulation") = m.attr("Triangulation3");
    m.attr("TuraevViroAlg") = m.attr("Algorithm");
}

