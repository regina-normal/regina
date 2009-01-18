
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

#include "algebra/nabeliangroup.h"
#include "manifold/nmanifold.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NManifold;

namespace {
    void writeName_stdio(const NManifold& m) {
        m.writeName(std::cout);
    }
    void writeTeXName_stdio(const NManifold& m) {
        m.writeTeXName(std::cout);
    }
    void writeStructure_stdio(const NManifold& m) {
        m.writeStructure(std::cout);
    }
}

void addNManifold() {
    class_<NManifold, boost::noncopyable, bases<regina::ShareableObject>,
            std::auto_ptr<NManifold> >("NManifold", no_init)
        .def("getName", &NManifold::getName)
        .def("getTeXName", &NManifold::getTeXName)
        .def("getStructure", &NManifold::getStructure)
        .def("construct", &NManifold::construct,
            return_value_policy<manage_new_object>())
        .def("getHomologyH1", &NManifold::getHomologyH1,
            return_value_policy<manage_new_object>())
        .def("writeName", writeName_stdio)
        .def("writeTeXName", writeTeXName_stdio)
        .def("writeStructure", writeStructure_stdio)
        .def(self < self)
    ;
}

