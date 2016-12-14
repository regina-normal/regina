
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

#include "algebra/grouppresentation.h"
#include "angle/anglestructure.h"
#include "progress/progresstracker.h"
#include "surfaces/normalsurface.h"
#include "triangulation/dim3.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Triangulation;
using regina::detail::TriangulationBase;

namespace {
    regina::Tetrahedron<3>* (Triangulation<3>::*newTetrahedron_void)() =
        &Triangulation<3>::newTetrahedron;
    regina::Tetrahedron<3>* (Triangulation<3>::*newTetrahedron_string)(
        const std::string&) = &Triangulation<3>::newTetrahedron;
    regina::Tetrahedron<3>* (Triangulation<3>::*tetrahedron_non_const)(
        size_t) = &Triangulation<3>::tetrahedron;
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

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplifyToLocalMinimum,
        Triangulation<3>::simplifyToLocalMinimum, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplifyExhaustive,
        Triangulation<3>::simplifyExhaustive, 0, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_threeTwoMove,
        Triangulation<3>::threeTwoMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_twoThreeMove,
        Triangulation<3>::twoThreeMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_oneFourMove,
        Triangulation<3>::oneFourMove, 1, 3);
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

    Triangulation<3>* enterTextTriangulation_stdio() {
        return Triangulation<3>::enterTextTriangulation(std::cin, std::cout);
    }

    boost::python::list tetrahedra_list(Triangulation<3>& t) {
        boost::python::list ans;
        for (auto s : t.tetrahedra())
            ans.append(boost::python::ptr(s));
        return ans;
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

    boost::python::list fVector_list(const Triangulation<3>& t) {
        boost::python::list ans;
        for (auto i : t.fVector())
            ans.append(i);
        return ans;
    }

    boost::python::list findAllIsomorphisms_list(
        const Triangulation<3>& t, const Triangulation<3>& other) {
        boost::python::list ans;

        std::list<regina::Isomorphism<3>*> isos;
        t.findAllIsomorphisms(other, back_inserter(isos));

        for (std::list<regina::Isomorphism<3>*>::iterator it =
                 isos.begin(); it != isos.end(); it++) {
            std::auto_ptr<regina::Isomorphism<3>> iso(*it);
            ans.append(iso);
        }
        return ans;
    }

    boost::python::list findAllSubcomplexesIn_list(
        const Triangulation<3>& t, const Triangulation<3>& other) {
        boost::python::list ans;

        std::list<regina::Isomorphism<3>*> isos;
        t.findAllSubcomplexesIn(other, back_inserter(isos));

        for (std::list<regina::Isomorphism<3>*>::iterator it =
                 isos.begin(); it != isos.end(); it++) {
            std::auto_ptr<regina::Isomorphism<3>> iso(*it);
            ans.append(iso);
        }
        return ans;
    }

    std::string isoSig_void(const Triangulation<3>& t) {
        return t.isoSig();
    }

    boost::python::tuple isoSig_relabelling(const Triangulation<3>& t) {
        regina::Isomorphism<3>* iso;
        std::string sig = t.isoSig(&iso);
        return boost::python::make_tuple(
            sig,
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::Isomorphism<3>*>::type()(iso))));
    }
}

void addTriangulation3() {
    enum_<regina::TuraevViroAlg>("TuraevViroAlg")
        .value("TV_DEFAULT", regina::TV_DEFAULT)
        .value("TV_BACKTRACK", regina::TV_BACKTRACK)
        .value("TV_TREEWIDTH", regina::TV_TREEWIDTH)
        .value("TV_NAIVE", regina::TV_NAIVE)
        ;

    scope().attr("TV_DEFAULT") = regina::TV_DEFAULT;
    scope().attr("TV_BACKTRACK") = regina::TV_BACKTRACK;
    scope().attr("TV_TREEWIDTH") = regina::TV_TREEWIDTH;
    scope().attr("TV_NAIVE") = regina::TV_NAIVE;

    {
        boost::python::scope s = class_<Triangulation<3>, bases<regina::Packet>,
                SafeHeldType<Triangulation<3>>,
                boost::noncopyable>("Triangulation3")
            .def(init<const Triangulation<3>&>())
            .def(init<const std::string&>())
            .def("size", &Triangulation<3>::size)
            .def("countTetrahedra", &Triangulation<3>::countTetrahedra)
            .def("tetrahedra", tetrahedra_list)
            .def("simplices", tetrahedra_list)
            .def("tetrahedron", tetrahedron_non_const,
                return_internal_reference<>())
            .def("simplex", tetrahedron_non_const,
                return_internal_reference<>())
            .def("newTetrahedron", newTetrahedron_void,
                return_internal_reference<>())
            .def("newSimplex", newTetrahedron_void,
                return_internal_reference<>())
            .def("newTetrahedron", newTetrahedron_string,
                return_internal_reference<>())
            .def("newSimplex", newTetrahedron_string,
                return_internal_reference<>())
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
            .def("fVector", fVector_list)
            .def("components", components_list)
            .def("boundaryComponents", boundaryComponents_list)
            .def("faces", &regina::python::faces<Triangulation<3>, 3>)
            .def("vertices", regina::python::faces_list<Triangulation<3>, 3, 0>)
            .def("edges", regina::python::faces_list<Triangulation<3>, 3, 1>)
            .def("triangles", regina::python::faces_list<Triangulation<3>, 3, 2>)
            .def("component", &Triangulation<3>::component,
                return_internal_reference<>())
            .def("boundaryComponent", &Triangulation<3>::boundaryComponent,
                return_internal_reference<>())
            .def("face", &regina::python::face<Triangulation<3>, 3, size_t>)
            .def("vertex", &Triangulation<3>::vertex,
                return_internal_reference<>())
            .def("edge", &Triangulation<3>::edge,
                return_internal_reference<>())
            .def("triangle", &Triangulation<3>::triangle,
                return_internal_reference<>())
            .def("isIdenticalTo", &Triangulation<3>::isIdenticalTo)
            .def("isIsomorphicTo",
                +[](const Triangulation<3>& t, const Triangulation<3>& s) {
                    return t.isIsomorphicTo(s).release(); },
                return_value_policy<manage_new_object>())
            .def("findAllIsomorphisms", findAllIsomorphisms_list)
            .def("findAllSubcomplexesIn", findAllSubcomplexesIn_list)
            .def("makeCanonical", &Triangulation<3>::makeCanonical)
            .def("isContainedIn",
                +[](const Triangulation<3>& t, const Triangulation<3>& s) {
                    return t.isContainedIn(s).release(); },
                return_value_policy<manage_new_object>())
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
                return_internal_reference<>())
            .def("simplifiedFundamentalGroup", simplifiedFundamentalGroup_clone)
            .def("homology", &Triangulation<3>::homology,
                return_internal_reference<>())
            .def("homologyH1", &Triangulation<3>::homologyH1,
                return_internal_reference<>())
            .def("homologyRel", &Triangulation<3>::homologyRel,
                return_internal_reference<>())
            .def("homologyBdry", &Triangulation<3>::homologyBdry,
                return_internal_reference<>())
            .def("homologyH2", &Triangulation<3>::homologyH2,
                return_internal_reference<>())
            .def("homologyH2Z2", &Triangulation<3>::homologyH2Z2)
            .def("turaevViro", &Triangulation<3>::turaevViro, OL_turaevViro())
            .def("turaevViroApprox", &Triangulation<3>::turaevViroApprox,
                OL_turaevViroApprox())
            .def("isZeroEfficient", &Triangulation<3>::isZeroEfficient)
            .def("knowsZeroEfficient", &Triangulation<3>::knowsZeroEfficient)
            .def("hasSplittingSurface", &Triangulation<3>::hasSplittingSurface)
            .def("knowsSplittingSurface", &Triangulation<3>::knowsSplittingSurface)
            .def("hasNonTrivialSphereOrDisc",
                &Triangulation<3>::hasNonTrivialSphereOrDisc,
                return_value_policy<manage_new_object>())
            .def("hasOctagonalAlmostNormalSphere",
                &Triangulation<3>::hasOctagonalAlmostNormalSphere,
                return_value_policy<manage_new_object>())
            .def("findStrictAngleStructure",
                &Triangulation<3>::findStrictAngleStructure,
                return_internal_reference<>())
            .def("hasStrictAngleStructure",
                &Triangulation<3>::hasStrictAngleStructure)
            .def("knowsStrictAngleStructure",
                &Triangulation<3>::knowsStrictAngleStructure)
            .def("intelligentSimplify", &Triangulation<3>::intelligentSimplify)
            .def("simplifyToLocalMinimum", &Triangulation<3>::simplifyToLocalMinimum,
                OL_simplifyToLocalMinimum())
            .def("simplifyExhaustive", &Triangulation<3>::simplifyExhaustive,
                OL_simplifyExhaustive())
            .def("threeTwoMove", &Triangulation<3>::threeTwoMove, OL_threeTwoMove())
            .def("twoThreeMove", &Triangulation<3>::twoThreeMove, OL_twoThreeMove())
            .def("oneFourMove", &Triangulation<3>::oneFourMove, OL_oneFourMove())
            .def("fourFourMove", &Triangulation<3>::fourFourMove, OL_fourFourMove())
            .def("twoZeroMove", twoZeroMove_vertex, OL_twoZeroMove())
            .def("twoZeroMove", twoZeroMove_edge, OL_twoZeroMove())
            .def("twoOneMove", &Triangulation<3>::twoOneMove, OL_twoOneMove())
            .def("openBook", &Triangulation<3>::openBook, OL_openBook())
            .def("closeBook", &Triangulation<3>::closeBook, OL_closeBook())
            .def("shellBoundary", &Triangulation<3>::shellBoundary,
                OL_shellBoundary())
            .def("collapseEdge", &Triangulation<3>::collapseEdge, OL_collapseEdge())
            .def("reorderTetrahedraBFS", &Triangulation<3>::reorderTetrahedraBFS,
                OL_reorderTetrahedraBFS())
            .def("orient", &Triangulation<3>::orient)
            .def("order", &Triangulation<3>::order, OL_order(args("force_oriented")))
            .def("splitIntoComponents", splitIntoComponents,
                OL_splitIntoComponents())
            .def("connectedSumDecomposition",
                &Triangulation<3>::connectedSumDecomposition,
                OL_connectedSumDecomposition())
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
                return_internal_reference<>())
            .def("makeDoubleCover", &Triangulation<3>::makeDoubleCover)
            .def("idealToFinite", &Triangulation<3>::idealToFinite)
            .def("finiteToIdeal", &Triangulation<3>::finiteToIdeal)
            .def("barycentricSubdivision", &Triangulation<3>::barycentricSubdivision)
            .def("drillEdge", &Triangulation<3>::drillEdge)
            .def("puncture", &Triangulation<3>::puncture, OL_puncture())
            .def("layerOn", &Triangulation<3>::layerOn,
                return_value_policy<reference_existing_object>())
            .def("insertLayeredSolidTorus",
                &Triangulation<3>::insertLayeredSolidTorus,
                return_value_policy<reference_existing_object>())
            .def("insertLayeredLensSpace",
                &Triangulation<3>::insertLayeredLensSpace)
            .def("insertLayeredLoop", &Triangulation<3>::insertLayeredLoop)
            .def("insertAugTriSolidTorus", &Triangulation<3>::insertAugTriSolidTorus)
            .def("insertSFSOverSphere", &Triangulation<3>::insertSFSOverSphere)
            .def("connectedSumWith", &Triangulation<3>::connectedSumWith)
            .def("insertTriangulation", &Triangulation<3>::insertTriangulation)
            .def("insertRehydration", &Triangulation<3>::insertRehydration)
            .def("dehydrate", &Triangulation<3>::dehydrate)
            .def("rehydrate", &Triangulation<3>::rehydrate,
                return_value_policy<to_held_type<> >())
            .def("isoSig", isoSig_void)
            .def("isoSigDetail", isoSig_relabelling)
            .def("fromIsoSig", &Triangulation<3>::fromIsoSig,
                return_value_policy<to_held_type<> >())
            .def("isoSigComponentSize", &Triangulation<3>::isoSigComponentSize)
            .def("dumpConstruction", &Triangulation<3>::dumpConstruction)
            .def("snapPea", snapPea_void)
            .def("saveSnapPea", &Triangulation<3>::saveSnapPea)
            .def("recogniser", recogniser_void)
            .def("recognizer", recognizer_void)
            .def("saveRecogniser", &Triangulation<3>::saveRecogniser)
            .def("saveRecognizer", &Triangulation<3>::saveRecognizer)
            .def("fromSnapPea", &Triangulation<3>::fromSnapPea,
                return_value_policy<to_held_type<> >())
            .def("enterTextTriangulation", enterTextTriangulation_stdio,
                return_value_policy<to_held_type<> >())
            .staticmethod("rehydrate")
            .staticmethod("fromIsoSig")
            .staticmethod("isoSigComponentSize")
            .staticmethod("fromSnapPea")
            .staticmethod("enterTextTriangulation")
        ;

        s.attr("typeID") = regina::PACKET_TRIANGULATION3;
        s.attr("dimension") = 3;
    }

    implicitly_convertible<SafeHeldType<Triangulation<3>>,
        SafeHeldType<regina::Packet> >();

    FIX_REGINA_BOOST_CONVERTERS(Triangulation<3>);

    scope().attr("NTriangulation") = scope().attr("Triangulation3");
}

