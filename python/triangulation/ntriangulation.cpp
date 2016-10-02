
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

#include "algebra/ngrouppresentation.h"
#include "angle/anglestructure.h"
#include "progress/progresstracker.h"
#include "surfaces/nnormalsurface.h"
#include "triangulation/nisomorphism.h"
#include "triangulation/ntriangulation.h"
#include "../safeheldtype.h"
#include "../generic/facehelper.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::NTriangulation;
using regina::Triangulation;
using regina::detail::TriangulationBase;

namespace {
    regina::NTetrahedron* (NTriangulation::*newTetrahedron_void)() =
        &NTriangulation::newTetrahedron;
    regina::NTetrahedron* (NTriangulation::*newTetrahedron_string)(
        const std::string&) = &NTriangulation::newTetrahedron;
    regina::NTetrahedron* (NTriangulation::*tetrahedron_non_const)(
        size_t) = &NTriangulation::tetrahedron;
    bool (NTriangulation::*twoZeroMove_vertex)(regina::NVertex*, bool, bool) =
        &NTriangulation::twoZeroMove;
    bool (NTriangulation::*twoZeroMove_edge)(regina::NEdge*, bool, bool) =
        &NTriangulation::twoZeroMove;
    std::string (NTriangulation::*snapPea_void)() const =
        &NTriangulation::snapPea;
    std::string (NTriangulation::*recogniser_void)() const =
        &NTriangulation::recogniser;
    std::string (NTriangulation::*recognizer_void)() const =
        &NTriangulation::recognizer;
    size_t (NTriangulation::*splitIntoComponents)(regina::NPacket*, bool) =
        &NTriangulation::splitIntoComponents;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplifyToLocalMinimum,
        NTriangulation::simplifyToLocalMinimum, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplifyExhaustive,
        NTriangulation::simplifyExhaustive, 0, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_threeTwoMove,
        NTriangulation::threeTwoMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_twoThreeMove,
        NTriangulation::twoThreeMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_oneFourMove,
        NTriangulation::oneFourMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_fourFourMove,
        NTriangulation::fourFourMove, 2, 4);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_twoZeroMove,
        NTriangulation::twoZeroMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_twoOneMove,
        NTriangulation::twoOneMove, 2, 4);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_openBook,
        NTriangulation::openBook, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_closeBook,
        NTriangulation::closeBook, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_shellBoundary,
        NTriangulation::shellBoundary, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_collapseEdge,
        NTriangulation::collapseEdge, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_reorderTetrahedraBFS,
        NTriangulation::reorderTetrahedraBFS, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_order,
        NTriangulation::order, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_splitIntoComponents,
        NTriangulation::splitIntoComponents, 0, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_connectedSumDecomposition,
        NTriangulation::connectedSumDecomposition, 0, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_puncture,
        NTriangulation::puncture, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_turaevViro,
        NTriangulation::turaevViro, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_turaevViroApprox,
        NTriangulation::turaevViroApprox, 1, 3);

    void simplifiedFundamentalGroup_clone(NTriangulation& tri,
            const regina::NGroupPresentation& group) {
        tri.simplifiedFundamentalGroup(new regina::NGroupPresentation(group));
    }

    NTriangulation* enterTextTriangulation_stdio() {
        return NTriangulation::enterTextTriangulation(std::cin, std::cout);
    }

    boost::python::list tetrahedra_list(NTriangulation& t) {
        boost::python::list ans;
        for (auto s : t.tetrahedra())
            ans.append(boost::python::ptr(s));
        return ans;
    }

    boost::python::list components_list(NTriangulation& t) {
        boost::python::list ans;
        for (auto c : t.components())
            ans.append(boost::python::ptr(c));
        return ans;
    }

    boost::python::list boundaryComponents_list(NTriangulation& t) {
        boost::python::list ans;
        for (auto b : t.boundaryComponents())
            ans.append(boost::python::ptr(b));
        return ans;
    }

    boost::python::list fVector_list(const NTriangulation& t) {
        boost::python::list ans;
        for (auto i : t.fVector())
            ans.append(i);
        return ans;
    }

    boost::python::list findAllIsomorphisms_list(
        const NTriangulation& t, const NTriangulation& other) {
        boost::python::list ans;

        std::list<regina::NIsomorphism*> isos;
        t.findAllIsomorphisms(other, back_inserter(isos));

        for (std::list<regina::NIsomorphism*>::iterator it =
                 isos.begin(); it != isos.end(); it++) {
            std::auto_ptr<regina::NIsomorphism> iso(*it);
            ans.append(iso);
        }
        return ans;
    }

    boost::python::list findAllSubcomplexesIn_list(
        const NTriangulation& t, const NTriangulation& other) {
        boost::python::list ans;

        std::list<regina::NIsomorphism*> isos;
        t.findAllSubcomplexesIn(other, back_inserter(isos));

        for (std::list<regina::NIsomorphism*>::iterator it =
                 isos.begin(); it != isos.end(); it++) {
            std::auto_ptr<regina::NIsomorphism> iso(*it);
            ans.append(iso);
        }
        return ans;
    }

    std::string isoSig_void(const NTriangulation& t) {
        return t.isoSig();
    }

    boost::python::tuple isoSig_relabelling(const NTriangulation& t) {
        regina::NIsomorphism* iso;
        std::string sig = t.isoSig(&iso);
        return boost::python::make_tuple(
            sig,
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::NIsomorphism*>::type()(iso))));
    }
}

void addNTriangulation() {
    {
    scope global;

    enum_<regina::TuraevViroAlg>("TuraevViroAlg")
        .value("TV_DEFAULT", regina::TV_DEFAULT)
        .value("TV_BACKTRACK", regina::TV_BACKTRACK)
        .value("TV_TREEWIDTH", regina::TV_TREEWIDTH)
        .value("TV_NAIVE", regina::TV_NAIVE)
        ;

    global.attr("TV_DEFAULT") = regina::TV_DEFAULT;
    global.attr("TV_BACKTRACK") = regina::TV_BACKTRACK;
    global.attr("TV_TREEWIDTH") = regina::TV_TREEWIDTH;
    global.attr("TV_NAIVE") = regina::TV_NAIVE;

    scope s = class_<Triangulation<3>, bases<regina::NPacket>,
            SafeHeldType<Triangulation<3>>,
            boost::noncopyable>("Triangulation3")
        .def(init<const NTriangulation&>())
        .def(init<const std::string&>())
        .def("size", &NTriangulation::size)
        .def("countTetrahedra", &NTriangulation::countTetrahedra)
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
        .def("removeTetrahedron", &NTriangulation::removeTetrahedron)
        .def("removeSimplex", &NTriangulation::removeSimplex)
        .def("removeTetrahedronAt", &NTriangulation::removeTetrahedronAt)
        .def("removeSimplexAt", &NTriangulation::removeSimplexAt)
        .def("removeAllTetrahedra", &NTriangulation::removeAllTetrahedra)
        .def("removeAllSimplices", &NTriangulation::removeAllSimplices)
        .def("swapContents", &NTriangulation::swapContents)
        .def("moveContentsTo", &NTriangulation::moveContentsTo)
        .def("countComponents", &NTriangulation::countComponents)
        .def("countBoundaryComponents",
            &NTriangulation::countBoundaryComponents)
        .def("countFaces", &regina::python::countFaces<NTriangulation, 3>)
        .def("countVertices", &NTriangulation::countVertices)
        .def("countEdges", &NTriangulation::countEdges)
        .def("countTriangles", &NTriangulation::countTriangles)
        .def("fVector", fVector_list)
        .def("components", components_list)
        .def("boundaryComponents", boundaryComponents_list)
        .def("faces", &regina::python::faces<NTriangulation, 3>)
        .def("vertices", regina::python::faces_list<NTriangulation, 3, 0>)
        .def("edges", regina::python::faces_list<NTriangulation, 3, 1>)
        .def("triangles", regina::python::faces_list<NTriangulation, 3, 2>)
        .def("component", &NTriangulation::component,
            return_internal_reference<>())
        .def("boundaryComponent", &NTriangulation::boundaryComponent,
            return_internal_reference<>())
        .def("face", &regina::python::face<NTriangulation, 3, size_t>)
        .def("vertex", &NTriangulation::vertex,
            return_internal_reference<>())
        .def("edge", &NTriangulation::edge,
            return_internal_reference<>())
        .def("triangle", &NTriangulation::triangle,
            return_internal_reference<>())
        .def("isIdenticalTo", &NTriangulation::isIdenticalTo)
        .def("isIsomorphicTo",
            +[](const NTriangulation& t, const NTriangulation& s) {
                return t.isIsomorphicTo(s).release(); },
            return_value_policy<manage_new_object>())
        .def("findAllIsomorphisms", findAllIsomorphisms_list)
        .def("findAllSubcomplexesIn", findAllSubcomplexesIn_list)
        .def("makeCanonical", &NTriangulation::makeCanonical)
        .def("isContainedIn",
            +[](const NTriangulation& t, const NTriangulation& s) {
                return t.isContainedIn(s).release(); },
            return_value_policy<manage_new_object>())
        .def("hasTwoSphereBoundaryComponents",
            &NTriangulation::hasTwoSphereBoundaryComponents)
        .def("hasNegativeIdealBoundaryComponents",
            &NTriangulation::hasNegativeIdealBoundaryComponents)
        .def("isEmpty", &NTriangulation::isEmpty)
        .def("eulerCharTri", &NTriangulation::eulerCharTri)
        .def("eulerCharManifold", &NTriangulation::eulerCharManifold)
        .def("isValid", &NTriangulation::isValid)
        .def("isIdeal", &NTriangulation::isIdeal)
        .def("isStandard", &NTriangulation::isStandard)
        .def("hasBoundaryFacets", &NTriangulation::hasBoundaryFacets)
        .def("hasBoundaryTriangles", &NTriangulation::hasBoundaryTriangles)
        .def("countBoundaryFacets", &NTriangulation::countBoundaryFacets)
        .def("countBoundaryTriangles", &NTriangulation::countBoundaryTriangles)
        .def("isClosed", &NTriangulation::isClosed)
        .def("isOrientable", &NTriangulation::isOrientable)
        .def("isOriented", &NTriangulation::isOriented)
        .def("isOrdered", &NTriangulation::isOrdered)
        .def("isConnected", &NTriangulation::isConnected)
        .def("fundamentalGroup", &NTriangulation::fundamentalGroup,
            return_internal_reference<>())
        .def("simplifiedFundamentalGroup", simplifiedFundamentalGroup_clone)
        .def("homology", &NTriangulation::homology,
            return_internal_reference<>())
        .def("homologyH1", &NTriangulation::homologyH1,
            return_internal_reference<>())
        .def("homologyRel", &NTriangulation::homologyRel,
            return_internal_reference<>())
        .def("homologyBdry", &NTriangulation::homologyBdry,
            return_internal_reference<>())
        .def("homologyH2", &NTriangulation::homologyH2,
            return_internal_reference<>())
        .def("homologyH2Z2", &NTriangulation::homologyH2Z2)
        .def("turaevViro", &NTriangulation::turaevViro, OL_turaevViro())
        .def("turaevViroApprox", &NTriangulation::turaevViroApprox,
            OL_turaevViroApprox())
        .def("isZeroEfficient", &NTriangulation::isZeroEfficient)
        .def("knowsZeroEfficient", &NTriangulation::knowsZeroEfficient)
        .def("hasSplittingSurface", &NTriangulation::hasSplittingSurface)
        .def("knowsSplittingSurface", &NTriangulation::knowsSplittingSurface)
        .def("hasNonTrivialSphereOrDisc",
            &NTriangulation::hasNonTrivialSphereOrDisc,
            return_value_policy<manage_new_object>())
        .def("hasOctagonalAlmostNormalSphere",
            &NTriangulation::hasOctagonalAlmostNormalSphere,
            return_value_policy<manage_new_object>())
        .def("findStrictAngleStructure",
            &NTriangulation::findStrictAngleStructure,
            return_internal_reference<>())
        .def("hasStrictAngleStructure",
            &NTriangulation::hasStrictAngleStructure)
        .def("knowsStrictAngleStructure",
            &NTriangulation::knowsStrictAngleStructure)
        .def("intelligentSimplify", &NTriangulation::intelligentSimplify)
        .def("simplifyToLocalMinimum", &NTriangulation::simplifyToLocalMinimum,
            OL_simplifyToLocalMinimum())
        .def("simplifyExhaustive", &NTriangulation::simplifyExhaustive,
            OL_simplifyExhaustive())
        .def("threeTwoMove", &NTriangulation::threeTwoMove, OL_threeTwoMove())
        .def("twoThreeMove", &NTriangulation::twoThreeMove, OL_twoThreeMove())
        .def("oneFourMove", &NTriangulation::oneFourMove, OL_oneFourMove())
        .def("fourFourMove", &NTriangulation::fourFourMove, OL_fourFourMove())
        .def("twoZeroMove", twoZeroMove_vertex, OL_twoZeroMove())
        .def("twoZeroMove", twoZeroMove_edge, OL_twoZeroMove())
        .def("twoOneMove", &NTriangulation::twoOneMove, OL_twoOneMove())
        .def("openBook", &NTriangulation::openBook, OL_openBook())
        .def("closeBook", &NTriangulation::closeBook, OL_closeBook())
        .def("shellBoundary", &NTriangulation::shellBoundary,
            OL_shellBoundary())
        .def("collapseEdge", &NTriangulation::collapseEdge, OL_collapseEdge())
        .def("reorderTetrahedraBFS", &NTriangulation::reorderTetrahedraBFS,
            OL_reorderTetrahedraBFS())
        .def("orient", &NTriangulation::orient)
        .def("order", &NTriangulation::order, OL_order(args("force_oriented")))
        .def("splitIntoComponents", splitIntoComponents,
            OL_splitIntoComponents())
        .def("connectedSumDecomposition",
            &NTriangulation::connectedSumDecomposition,
            OL_connectedSumDecomposition())
        .def("isThreeSphere", &NTriangulation::isThreeSphere)
        .def("knowsThreeSphere", &NTriangulation::knowsThreeSphere)
        .def("isBall", &NTriangulation::isBall)
        .def("knowsBall", &NTriangulation::knowsBall)
        .def("isSolidTorus", &NTriangulation::isSolidTorus)
        .def("knowsSolidTorus", &NTriangulation::knowsSolidTorus)
        .def("isIrreducible", &NTriangulation::isIrreducible)
        .def("knowsIrreducible", &NTriangulation::knowsIrreducible)
        .def("hasCompressingDisc", &NTriangulation::hasCompressingDisc)
        .def("knowsCompressingDisc", &NTriangulation::knowsCompressingDisc)
        .def("hasSimpleCompressingDisc",
            &NTriangulation::hasSimpleCompressingDisc)
        .def("isHaken", &NTriangulation::isHaken)
        .def("knowsHaken", &NTriangulation::knowsHaken)
        .def("niceTreeDecomposition", &NTriangulation::niceTreeDecomposition,
            return_internal_reference<>())
        .def("makeDoubleCover", &NTriangulation::makeDoubleCover)
        .def("idealToFinite", &NTriangulation::idealToFinite)
        .def("finiteToIdeal", &NTriangulation::finiteToIdeal)
        .def("barycentricSubdivision", &NTriangulation::barycentricSubdivision)
        .def("drillEdge", &NTriangulation::drillEdge)
        .def("puncture", &NTriangulation::puncture, OL_puncture())
        .def("layerOn", &NTriangulation::layerOn,
            return_value_policy<reference_existing_object>())
        .def("insertLayeredSolidTorus",
            &NTriangulation::insertLayeredSolidTorus,
            return_value_policy<reference_existing_object>())
        .def("insertLayeredLensSpace",
            &NTriangulation::insertLayeredLensSpace)
        .def("insertLayeredLoop", &NTriangulation::insertLayeredLoop)
        .def("insertAugTriSolidTorus", &NTriangulation::insertAugTriSolidTorus)
        .def("insertSFSOverSphere", &NTriangulation::insertSFSOverSphere)
        .def("connectedSumWith", &NTriangulation::connectedSumWith)
        .def("insertTriangulation", &NTriangulation::insertTriangulation)
        .def("insertRehydration", &NTriangulation::insertRehydration)
        .def("dehydrate", &NTriangulation::dehydrate)
        .def("rehydrate", &NTriangulation::rehydrate,
            return_value_policy<to_held_type<> >())
        .def("isoSig", isoSig_void)
        .def("isoSigDetail", isoSig_relabelling)
        .def("fromIsoSig", &NTriangulation::fromIsoSig,
            return_value_policy<to_held_type<> >())
        .def("isoSigComponentSize", &NTriangulation::isoSigComponentSize)
        .def("dumpConstruction", &NTriangulation::dumpConstruction)
        .def("snapPea", snapPea_void)
        .def("saveSnapPea", &NTriangulation::saveSnapPea)
        .def("recogniser", recogniser_void)
        .def("recognizer", recognizer_void)
        .def("saveRecogniser", &NTriangulation::saveRecogniser)
        .def("saveRecognizer", &NTriangulation::saveRecognizer)
        .def("fromSnapPea", &NTriangulation::fromSnapPea,
            return_value_policy<to_held_type<> >())
        .def("enterTextTriangulation", enterTextTriangulation_stdio,
            return_value_policy<to_held_type<> >())
        .staticmethod("rehydrate")
        .staticmethod("fromIsoSig")
        .staticmethod("isoSigComponentSize")
        .staticmethod("fromSnapPea")
        .staticmethod("enterTextTriangulation")
    ;

    s.attr("typeID") = regina::PACKET_TRIANGULATION;

    implicitly_convertible<SafeHeldType<NTriangulation>,
        SafeHeldType<regina::NPacket> >();
    }

    FIX_REGINA_BOOST_CONVERTERS(NTriangulation);

    scope().attr("NTriangulation") = scope().attr("Triangulation3");
}

