
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

#include "dim4/dim4boundarycomponent.h"
#include "dim4/dim4component.h"
#include "dim4/dim4edge.h"
#include "dim4/dim4face.h"
#include "dim4/dim4pentachoron.h"
#include "dim4/dim4vertex.h"
#include "../globalarray.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::Dim4Face;
using regina::Dim4FaceEmbedding;
using regina::python::GlobalArray;
using regina::python::GlobalArray2D;
using regina::python::GlobalArray3D;

namespace {
    GlobalArray3D<int> Dim4Face_faceNumber(Dim4Face::faceNumber, 5);
    GlobalArray2D<int> Dim4Face_faceVertex(Dim4Face::faceVertex, 10);
    GlobalArray<regina::NPerm5> Dim4Face_ordering(Dim4Face::ordering, 10);

    boost::python::list Dim4Face_getEmbeddings_list(const Dim4Face* f) {
        const std::deque<Dim4FaceEmbedding>& embs = f->getEmbeddings();
        std::deque<Dim4FaceEmbedding>::const_iterator it;

        boost::python::list ans;
        for (it = embs.begin(); it != embs.end(); ++it)
            ans.append(*it);
        return ans;
    }
}

void addDim4Face() {
    class_<Dim4FaceEmbedding, boost::noncopyable>("Dim4FaceEmbedding",
            init<regina::Dim4Pentachoron*, int>())
        .def(init<const Dim4FaceEmbedding&>())
        .def("getPentachoron", &Dim4FaceEmbedding::getPentachoron,
            return_value_policy<reference_existing_object>())
        .def("getFace", &Dim4FaceEmbedding::getFace)
        .def("getVertices", &Dim4FaceEmbedding::getVertices)
    ;

    scope s = class_<Dim4Face, bases<regina::ShareableObject>,
            std::auto_ptr<Dim4Face>, boost::noncopyable>("Dim4Face", no_init)
        .def("getEmbeddings", Dim4Face_getEmbeddings_list)
        .def("getNumberOfEmbeddings", &Dim4Face::getNumberOfEmbeddings)
        .def("getEmbedding", &Dim4Face::getEmbedding,
            return_internal_reference<>())
        .def("getComponent", &Dim4Face::getComponent,
            return_value_policy<reference_existing_object>())
        .def("getBoundaryComponent", &Dim4Face::getBoundaryComponent,
            return_value_policy<reference_existing_object>())
        .def("getVertex", &Dim4Face::getVertex,
            return_value_policy<reference_existing_object>())
        .def("getEdge", &Dim4Face::getEdge,
            return_value_policy<reference_existing_object>())
        .def("getEdgeMapping", &Dim4Face::getEdgeMapping)
        .def("getDegree", &Dim4Face::getDegree)
        .def("isBoundary", &Dim4Face::isBoundary)
        .def("isValid", &Dim4Face::isValid)
    ;

    s.attr("faceNumber") = &Dim4Face_faceNumber;
    s.attr("faceVertex") = &Dim4Face_faceVertex;
    s.attr("ordering") = &Dim4Face_ordering;
}

