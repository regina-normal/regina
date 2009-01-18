
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

#include "algebra/ngrouppresentation.h"
#include "dim4/dim4triangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::Dim4Triangulation;

namespace {
    regina::Dim4Pentachoron* (Dim4Triangulation::*getPentachoron_non_const)(
        unsigned long) = &Dim4Triangulation::getPentachoron;

    void addPentachoron_own(Dim4Triangulation& tri,
            std::auto_ptr<regina::Dim4Pentachoron> pent) {
        tri.addPentachoron(pent.get());
        pent.release();
    }

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

    boost::python::list Dim4_getVertices_list(Dim4Triangulation& t) {
        boost::python::list ans;
        for (Dim4Triangulation::VertexIterator it =
                t.getVertices().begin(); it != t.getVertices().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim4_getEdges_list(Dim4Triangulation& t) {
        boost::python::list ans;
        for (Dim4Triangulation::EdgeIterator it =
                t.getEdges().begin(); it != t.getEdges().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim4_getFaces_list(Dim4Triangulation& t) {
        boost::python::list ans;
        for (Dim4Triangulation::FaceIterator it =
                t.getFaces().begin(); it != t.getFaces().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim4_getTetrahedra_list(Dim4Triangulation& t) {
        boost::python::list ans;
        for (Dim4Triangulation::TetrahedronIterator it =
                t.getTetrahedra().begin(); it != t.getTetrahedra().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    void simplifiedFundamentalGroup_own(Dim4Triangulation& tri,
            std::auto_ptr<regina::NGroupPresentation> group) {
        tri.simplifiedFundamentalGroup(group.release());
    }
}

void addDim4Triangulation() {
    scope s = class_<Dim4Triangulation, bases<regina::NPacket>,
            std::auto_ptr<Dim4Triangulation>,
            boost::noncopyable>("Dim4Triangulation")
        .def(init<const Dim4Triangulation&>())
        .def("getNumberOfPentachora", &Dim4Triangulation::getNumberOfPentachora)
        .def("getPentachora", Dim4_getPentachora_list)
        .def("getPentachoron", getPentachoron_non_const,
            return_value_policy<reference_existing_object>())
        .def("pentachoronIndex", &Dim4Triangulation::pentachoronIndex)
        .def("addPentachoron", addPentachoron_own)
        .def("removePentachoron", &Dim4Triangulation::removePentachoron,
            return_value_policy<manage_new_object>())
        .def("removePentachoronAt", &Dim4Triangulation::removePentachoronAt,
            return_value_policy<manage_new_object>())
        .def("removeAllPentachora", &Dim4Triangulation::removeAllPentachora)
        .def("gluingsHaveChanged", &Dim4Triangulation::gluingsHaveChanged)
        .def("getNumberOfComponents", &Dim4Triangulation::getNumberOfComponents)
        .def("getNumberOfBoundaryComponents",
            &Dim4Triangulation::getNumberOfBoundaryComponents)
        .def("getNumberOfVertices", &Dim4Triangulation::getNumberOfVertices)
        .def("getNumberOfEdges", &Dim4Triangulation::getNumberOfEdges)
        .def("getNumberOfFaces", &Dim4Triangulation::getNumberOfFaces)
        .def("getNumberOfTetrahedra", &Dim4Triangulation::getNumberOfTetrahedra)
        .def("getComponents", Dim4_getComponents_list)
        .def("getBoundaryComponents", Dim4_getBoundaryComponents_list)
        .def("getVertices", Dim4_getVertices_list)
        .def("getEdges", Dim4_getEdges_list)
        .def("getFaces", Dim4_getFaces_list)
        .def("getTetrahedra", Dim4_getTetrahedra_list)
        .def("getComponent", &Dim4Triangulation::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim4Triangulation::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4Triangulation::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim4Triangulation::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getFace", &Dim4Triangulation::getFace,
            return_value_policy<reference_existing_object>())
        .def("getTetrahedron", &Dim4Triangulation::getTetrahedron,
            return_value_policy<reference_existing_object>())
        .def("componentIndex", &Dim4Triangulation::componentIndex)
        .def("boundaryComponentIndex",
            &Dim4Triangulation::boundaryComponentIndex)
        .def("vertexIndex", &Dim4Triangulation::vertexIndex)
        .def("edgeIndex", &Dim4Triangulation::edgeIndex)
        .def("faceIndex", &Dim4Triangulation::faceIndex)
        .def("tetrahedronIndex", &Dim4Triangulation::tetrahedronIndex)
        .def("getEulerCharTri", &Dim4Triangulation::getEulerCharTri)
        .def("isValid", &Dim4Triangulation::isValid)
        .def("isIdeal", &Dim4Triangulation::isIdeal)
        .def("hasBoundaryTetrahedra", &Dim4Triangulation::hasBoundaryTetrahedra)
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
        .def("insertTriangulation", &Dim4Triangulation::insertTriangulation)
        .def("dumpConstruction", &Dim4Triangulation::dumpConstruction)
    ;

    s.attr("packetType") = Dim4Triangulation::packetType;

    implicitly_convertible<std::auto_ptr<Dim4Triangulation>,
        std::auto_ptr<regina::NPacket> >();
}

