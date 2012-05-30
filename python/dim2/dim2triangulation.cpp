
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include <boost/python.hpp>
#include "algebra/ngrouppresentation.h"
#include "dim2/dim2isomorphism.h"
#include "dim2/dim2triangulation.h"

using namespace boost::python;
using regina::Dim2Triangulation;

namespace {
    regina::Dim2Face* (Dim2Triangulation::*newFace_void)() =
        &Dim2Triangulation::newFace;
    regina::Dim2Face* (Dim2Triangulation::*newFace_string)(
        const std::string&) = &Dim2Triangulation::newFace;
    regina::Dim2Face* (Dim2Triangulation::*getFace_non_const)(
        unsigned long) = &Dim2Triangulation::getFace;
    std::string (Dim2Triangulation::*isoSig_void)() const =
        &Dim2Triangulation::isoSig;

    boost::python::list Dim2_getFaces_list(Dim2Triangulation& t) {
        boost::python::list ans;
        for (Dim2Triangulation::FaceIterator it =
                t.getFaces().begin(); it != t.getFaces().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim2_getComponents_list(Dim2Triangulation& t) {
        boost::python::list ans;
        for (Dim2Triangulation::ComponentIterator it =
                t.getComponents().begin(); it != t.getComponents().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim2_getBoundaryComponents_list(Dim2Triangulation& t) {
        boost::python::list ans;
        for (Dim2Triangulation::BoundaryComponentIterator it =
                t.getBoundaryComponents().begin();
                it != t.getBoundaryComponents().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim2_getVertices_list(Dim2Triangulation& t) {
        boost::python::list ans;
        for (Dim2Triangulation::VertexIterator it =
                t.getVertices().begin(); it != t.getVertices().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    boost::python::list Dim2_getEdges_list(Dim2Triangulation& t) {
        boost::python::list ans;
        for (Dim2Triangulation::EdgeIterator it =
                t.getEdges().begin(); it != t.getEdges().end(); ++it)
            ans.append(boost::python::ptr(*it));
        return ans;
    }

    regina::Dim2Isomorphism* isIsomorphicTo_ptr(Dim2Triangulation& t,
            Dim2Triangulation& s) {
        return t.isIsomorphicTo(s).release();
    }

    regina::Dim2Isomorphism* isContainedIn_ptr(Dim2Triangulation& t,
            Dim2Triangulation& s) {
        return t.isContainedIn(s).release();
    }
}

void addDim2Triangulation() {
    scope s = class_<Dim2Triangulation, bases<regina::NPacket>,
            std::auto_ptr<Dim2Triangulation>,
            boost::noncopyable>("Dim2Triangulation")
        .def(init<const Dim2Triangulation&>())
        .def("getNumberOfFaces", &Dim2Triangulation::getNumberOfFaces)
        .def("getFaces", Dim2_getFaces_list)
        .def("getFace", getFace_non_const,
            return_value_policy<reference_existing_object>())
        .def("faceIndex", &Dim2Triangulation::faceIndex)
        .def("newFace", newFace_void,
            return_value_policy<reference_existing_object>())
        .def("newFace", newFace_string,
            return_value_policy<reference_existing_object>())
        .def("removeFace", &Dim2Triangulation::removeFace)
        .def("removeFaceAt", &Dim2Triangulation::removeFaceAt)
        .def("removeAllFaces", &Dim2Triangulation::removeAllFaces)
        .def("swapContents", &Dim2Triangulation::swapContents)
        .def("moveContentsTo", &Dim2Triangulation::moveContentsTo)
        .def("getNumberOfComponents", &Dim2Triangulation::getNumberOfComponents)
        .def("getNumberOfBoundaryComponents",
            &Dim2Triangulation::getNumberOfBoundaryComponents)
        .def("getNumberOfVertices", &Dim2Triangulation::getNumberOfVertices)
        .def("getNumberOfEdges", &Dim2Triangulation::getNumberOfEdges)
        .def("getComponents", Dim2_getComponents_list)
        .def("getBoundaryComponents", Dim2_getBoundaryComponents_list)
        .def("getVertices", Dim2_getVertices_list)
        .def("getEdges", Dim2_getEdges_list)
        .def("getComponent", &Dim2Triangulation::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim2Triangulation::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim2Triangulation::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim2Triangulation::getEdge,
            return_value_policy<reference_existing_object>())
        .def("componentIndex", &Dim2Triangulation::componentIndex)
        .def("boundaryComponentIndex",
            &Dim2Triangulation::boundaryComponentIndex)
        .def("vertexIndex", &Dim2Triangulation::vertexIndex)
        .def("edgeIndex", &Dim2Triangulation::edgeIndex)
        .def("isIsomorphicTo", isIsomorphicTo_ptr,
            return_value_policy<manage_new_object>())
        .def("makeCanonical", &Dim2Triangulation::makeCanonical)
        .def("isContainedIn", isContainedIn_ptr,
            return_value_policy<manage_new_object>())
        .def("getEulerChar", &Dim2Triangulation::getEulerChar)
        .def("isClosed", &Dim2Triangulation::isClosed)
        .def("isOrientable", &Dim2Triangulation::isOrientable)
        .def("isConnected", &Dim2Triangulation::isConnected)
        .def("insertTriangulation", &Dim2Triangulation::insertTriangulation)
        .def("isoSig", isoSig_void)
        .def("fromIsoSig", &Dim2Triangulation::fromIsoSig,
            return_value_policy<manage_new_object>())
        .def("dumpConstruction", &Dim2Triangulation::dumpConstruction)
        .staticmethod("fromIsoSig")
    ;

    s.attr("packetType") = Dim2Triangulation::packetType;

    implicitly_convertible<std::auto_ptr<Dim2Triangulation>,
        std::auto_ptr<regina::NPacket> >();
}

