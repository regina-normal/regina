
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

#include <boost/python.hpp>
#include "algebra/ngrouppresentation.h"
#include "dim4/dim4isomorphism.h"
#include "dim4/dim4triangulation.h"

using namespace boost::python;
using regina::Dim4Triangulation;
using regina::Triangulation;

namespace {
    regina::Dim4Pentachoron* (Dim4Triangulation::*newPentachoron_void)() =
        &Dim4Triangulation::newPentachoron;
    regina::Dim4Pentachoron* (Dim4Triangulation::*newPentachoron_string)(
        const std::string&) = &Dim4Triangulation::newPentachoron;
    regina::Dim4Pentachoron* (Dim4Triangulation::*pentachoron_non_const)(
        unsigned long) = &Dim4Triangulation::pentachoron;
    bool (Dim4Triangulation::*twoZeroMove_triangle)(regina::Dim4Triangle*,
        bool, bool) = &Dim4Triangulation::twoZeroMove;
    bool (Dim4Triangulation::*twoZeroMove_edge)(regina::Dim4Edge*,
        bool, bool) = &Dim4Triangulation::twoZeroMove;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_simplifyToLocalMinimum,
        Dim4Triangulation::simplifyToLocalMinimum, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_fourTwoMove,
        Dim4Triangulation::fourTwoMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_threeThreeMove,
        Dim4Triangulation::threeThreeMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_twoFourMove,
        Dim4Triangulation::twoFourMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_oneFiveMove,
        Dim4Triangulation::oneFiveMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_twoZeroMove,
        Dim4Triangulation::twoZeroMove, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_openBook,
        Dim4Triangulation::openBook, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_shellBoundary,
        Dim4Triangulation::shellBoundary, 1, 3);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_collapseEdge,
        Dim4Triangulation::collapseEdge, 1, 3);

    boost::python::list Dim4_getPentachora_list(Dim4Triangulation& t) {
        boost::python::list ans;
        for (Dim4Triangulation::PentachoronIterator it =
                t.getPentachora().begin(); it != t.getPentachora().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim4_getComponents_list(Dim4Triangulation& t) {
        boost::python::list ans;
        for (Dim4Triangulation::ComponentIterator it =
                t.getComponents().begin(); it != t.getComponents().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim4_getBoundaryComponents_list(Dim4Triangulation& t) {
        boost::python::list ans;
        for (Dim4Triangulation::BoundaryComponentIterator it =
                t.getBoundaryComponents().begin();
                it != t.getBoundaryComponents().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim4_vertices_list(Dim4Triangulation& t) {
        boost::python::list ans;
        for (Dim4Triangulation::VertexIterator it =
                t.vertices().begin(); it != t.vertices().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim4_edges_list(Dim4Triangulation& t) {
        boost::python::list ans;
        for (Dim4Triangulation::EdgeIterator it =
                t.edges().begin(); it != t.edges().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim4_triangles_list(Dim4Triangulation& t) {
        boost::python::list ans;
        for (Dim4Triangulation::TriangleIterator it =
                t.triangles().begin(); it != t.triangles().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim4_tetrahedra_list(Dim4Triangulation& t) {
        boost::python::list ans;
        for (Dim4Triangulation::TetrahedronIterator it =
                t.tetrahedra().begin(); it != t.tetrahedra().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    regina::Dim4Isomorphism* isIsomorphicTo_ptr(const Dim4Triangulation& t,
            const Dim4Triangulation& s) {
        return t.isIsomorphicTo(s).release();
    }

    regina::Dim4Isomorphism* isContainedIn_ptr(const Dim4Triangulation& t,
            const Dim4Triangulation& s) {
        return t.isContainedIn(s).release();
    }

    boost::python::list findAllIsomorphisms_list(
        const Dim4Triangulation& t, const Dim4Triangulation& other) {
        boost::python::list ans;

        std::list<regina::Dim4Isomorphism*> isos;
        t.findAllIsomorphisms(other, back_inserter(isos));

        for (std::list<regina::Dim4Isomorphism*>::iterator it =
                 isos.begin(); it != isos.end(); it++) {
            std::auto_ptr<regina::Dim4Isomorphism> iso(*it);
            ans.append(iso);
        }
        return ans;
    }

    boost::python::list findAllSubcomplexesIn_list(
        const Dim4Triangulation& t, const Dim4Triangulation& other) {
        boost::python::list ans;

        std::list<regina::Dim4Isomorphism*> isos;
        t.findAllSubcomplexesIn(other, back_inserter(isos));

        for (std::list<regina::Dim4Isomorphism*>::iterator it =
                 isos.begin(); it != isos.end(); it++) {
            std::auto_ptr<regina::Dim4Isomorphism> iso(*it);
            ans.append(iso);
        }
        return ans;
    }

    void simplifiedFundamentalGroup_own(Dim4Triangulation& tri,
            std::auto_ptr<regina::NGroupPresentation> group) {
        tri.simplifiedFundamentalGroup(group.release());
    }

    std::string isoSig_void(const Dim4Triangulation& t) {
        return t.isoSig();
    }

    boost::python::tuple isoSig_relabelling(const Dim4Triangulation& t) {
        regina::Dim4Isomorphism* iso;
        std::string sig = t.isoSig(&iso);
        return boost::python::make_tuple(
            sig,
            boost::python::object(boost::python::handle<>(
                boost::python::manage_new_object::
                apply<regina::Dim4Isomorphism*>::type()(iso))));
    }
}

void addDim4Triangulation() {
    {
    scope s = class_<Triangulation<4>, bases<regina::NPacket>,
            std::auto_ptr<Triangulation<4>>,
            boost::noncopyable>("Triangulation4")
        .def(init<const Dim4Triangulation&>())
        .def(init<const std::string&>())
        .def("size", &Dim4Triangulation::size)
        .def("getNumberOfPentachora", &Dim4Triangulation::getNumberOfPentachora)
        .def("getNumberOfSimplices", &Dim4Triangulation::getNumberOfSimplices)
        .def("getPentachora", Dim4_getPentachora_list)
        .def("getSimplices", Dim4_getPentachora_list)
        .def("simplices", Dim4_getPentachora_list)
        .def("pentachoron", pentachoron_non_const,
            return_internal_reference<>())
        .def("getPentachoron", pentachoron_non_const,
            return_internal_reference<>())
        .def("simplex", pentachoron_non_const,
            return_internal_reference<>())
        .def("getSimplex", pentachoron_non_const,
            return_internal_reference<>())
        .def("pentachoronIndex", &Dim4Triangulation::pentachoronIndex)
        .def("simplexIndex", &Dim4Triangulation::simplexIndex)
        .def("newPentachoron", newPentachoron_void,
            return_internal_reference<>())
        .def("newSimplex", newPentachoron_void,
            return_internal_reference<>())
        .def("newPentachoron", newPentachoron_string,
            return_internal_reference<>())
        .def("newSimplex", newPentachoron_string,
            return_internal_reference<>())
        .def("removePentachoron", &Dim4Triangulation::removePentachoron)
        .def("removeSimplex", &Dim4Triangulation::removeSimplex)
        .def("removePentachoronAt", &Dim4Triangulation::removePentachoronAt)
        .def("removeSimplexAt", &Dim4Triangulation::removeSimplexAt)
        .def("removeAllPentachora", &Dim4Triangulation::removeAllPentachora)
        .def("removeAllSimplices", &Dim4Triangulation::removeAllSimplices)
        .def("swapContents", &Dim4Triangulation::swapContents)
        .def("moveContentsTo", &Dim4Triangulation::moveContentsTo)
        .def("countComponents", &Dim4Triangulation::countComponents)
        .def("getNumberOfComponents", &Dim4Triangulation::getNumberOfComponents)
        .def("getNumberOfBoundaryComponents",
            &Dim4Triangulation::getNumberOfBoundaryComponents)
        .def("countVertices", &Dim4Triangulation::countVertices)
        .def("getNumberOfVertices", &Dim4Triangulation::getNumberOfVertices)
        .def("countEdges", &Dim4Triangulation::countEdges)
        .def("getNumberOfEdges", &Dim4Triangulation::getNumberOfEdges)
        .def("countTriangles", &Dim4Triangulation::countTriangles)
        .def("getNumberOfTriangles", &Dim4Triangulation::getNumberOfTriangles)
        .def("countTetrahedra", &Dim4Triangulation::countTetrahedra)
        .def("getNumberOfTetrahedra", &Dim4Triangulation::getNumberOfTetrahedra)
        .def("components", Dim4_getComponents_list)
        .def("getComponents", Dim4_getComponents_list)
        .def("getBoundaryComponents", Dim4_getBoundaryComponents_list)
        .def("vertices", Dim4_vertices_list)
        .def("getVertices", Dim4_vertices_list)
        .def("edges", Dim4_edges_list)
        .def("getEdges", Dim4_edges_list)
        .def("triangles", Dim4_triangles_list)
        .def("getTriangles", Dim4_triangles_list)
        .def("tetrahedra", Dim4_tetrahedra_list)
        .def("getTetrahedra", Dim4_tetrahedra_list)
        .def("component", &Dim4Triangulation::component,
            return_value_policy<reference_existing_object>())
        .def("getComponent", &Dim4Triangulation::getComponent,
            return_internal_reference<>())
        .def("getBoundaryComponent", &Dim4Triangulation::getBoundaryComponent,
            return_internal_reference<>())
        .def("vertex", &Dim4Triangulation::vertex,
            return_internal_reference<>())
        .def("getVertex", &Dim4Triangulation::getVertex,
            return_internal_reference<>())
        .def("edge", &Dim4Triangulation::edge,
            return_internal_reference<>())
        .def("getEdge", &Dim4Triangulation::getEdge,
            return_internal_reference<>())
        .def("triangle", &Dim4Triangulation::triangle,
            return_internal_reference<>())
        .def("getTriangle", &Dim4Triangulation::getTriangle,
            return_internal_reference<>())
        .def("tetrahedron", &Dim4Triangulation::tetrahedron,
            return_internal_reference<>())
        .def("getTetrahedron", &Dim4Triangulation::getTetrahedron,
            return_internal_reference<>())
        .def("componentIndex", &Dim4Triangulation::componentIndex)
        .def("boundaryComponentIndex",
            &Dim4Triangulation::boundaryComponentIndex)
        .def("vertexIndex", &Dim4Triangulation::vertexIndex)
        .def("edgeIndex", &Dim4Triangulation::edgeIndex)
        .def("triangleIndex", &Dim4Triangulation::triangleIndex)
        .def("tetrahedronIndex", &Dim4Triangulation::tetrahedronIndex)
        .def("isIdenticalTo", &Dim4Triangulation::isIdenticalTo)
        .def("isIsomorphicTo", isIsomorphicTo_ptr,
            return_value_policy<manage_new_object>())
        .def("makeCanonical", &Dim4Triangulation::makeCanonical)
        .def("isContainedIn", isContainedIn_ptr,
            return_value_policy<manage_new_object>())
        .def("findAllIsomorphisms", findAllIsomorphisms_list)
        .def("findAllSubcomplexesIn", findAllSubcomplexesIn_list)
        .def("isEmpty", &Dim4Triangulation::isEmpty)
        .def("getEulerCharTri", &Dim4Triangulation::getEulerCharTri)
        .def("getEulerCharManifold", &Dim4Triangulation::getEulerCharManifold)
        .def("isValid", &Dim4Triangulation::isValid)
        .def("isIdeal", &Dim4Triangulation::isIdeal)
        .def("hasBoundaryFacets", &Dim4Triangulation::hasBoundaryFacets)
        .def("hasBoundaryTetrahedra", &Dim4Triangulation::hasBoundaryTetrahedra)
        .def("countBoundaryFacets", &Dim4Triangulation::countBoundaryFacets)
        .def("countBoundaryTetrahedra",
            &Dim4Triangulation::countBoundaryTetrahedra)
        .def("isClosed", &Dim4Triangulation::isClosed)
        .def("isOrientable", &Dim4Triangulation::isOrientable)
        .def("isConnected", &Dim4Triangulation::isConnected)
        .def("getFundamentalGroup", &Dim4Triangulation::getFundamentalGroup,
            return_internal_reference<>())
        .def("simplifiedFundamentalGroup", simplifiedFundamentalGroup_own)
        .def("getHomologyH1", &Dim4Triangulation::getHomologyH1,
            return_internal_reference<>())
        .def("getHomologyH2", &Dim4Triangulation::getHomologyH2,
            return_internal_reference<>())
        .def("intelligentSimplify", &Dim4Triangulation::intelligentSimplify)
        .def("simplifyToLocalMinimum",
            &Dim4Triangulation::simplifyToLocalMinimum,
            OL_simplifyToLocalMinimum())
        .def("fourTwoMove", &Dim4Triangulation::fourTwoMove, OL_fourTwoMove())
        .def("threeThreeMove", &Dim4Triangulation::threeThreeMove,
            OL_threeThreeMove())
        .def("twoFourMove", &Dim4Triangulation::twoFourMove, OL_twoFourMove())
        .def("oneFiveMove", &Dim4Triangulation::oneFiveMove, OL_oneFiveMove())
        .def("twoZeroMove", twoZeroMove_triangle, OL_twoZeroMove())
        .def("twoZeroMove", twoZeroMove_edge, OL_twoZeroMove())
        .def("openBook", &Dim4Triangulation::openBook, OL_openBook())
        .def("shellBoundary", &Dim4Triangulation::shellBoundary,
            OL_shellBoundary())
        .def("collapseEdge", &Dim4Triangulation::collapseEdge,
            OL_collapseEdge())
        .def("barycentricSubdivision",
            &Dim4Triangulation::barycentricSubdivision)
        .def("idealToFinite", &Dim4Triangulation::idealToFinite)
        .def("insertTriangulation", &Dim4Triangulation::insertTriangulation)
        .def("isoSig", isoSig_void)
        .def("isoSigDetail", isoSig_relabelling)
        .def("fromIsoSig", &Dim4Triangulation::fromIsoSig,
            return_value_policy<manage_new_object>())
        .def("isoSigComponentSize", &Dim4Triangulation::isoSigComponentSize)
        .def("dumpConstruction", &Dim4Triangulation::dumpConstruction)
        .staticmethod("fromIsoSig")
        .staticmethod("isoSigComponentSize")
    ;

    s.attr("packetType") = regina::PacketType(Dim4Triangulation::packetType);

    implicitly_convertible<std::auto_ptr<Dim4Triangulation>,
        std::auto_ptr<regina::NPacket> >();
    }

    scope().attr("Dim4Triangulation") = scope().attr("Triangulation4");
}

