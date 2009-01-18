
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

#include "surfaces/nnormalsurface.h"
#include "surfaces/nsurfaceset.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NSurfaceSet;

namespace {
    void writeAllSurfaces_stdio(const NSurfaceSet& s) {
        s.writeAllSurfaces(std::cout);
    }
}

void addNSurfaceSet() {
    class_<NSurfaceSet, boost::noncopyable, std::auto_ptr<NSurfaceSet> >
            ("NSurfaceSet", no_init)
        .def("getFlavour", &NSurfaceSet::getFlavour)
        .def("allowsAlmostNormal", &NSurfaceSet::allowsAlmostNormal)
        .def("isEmbeddedOnly", &NSurfaceSet::isEmbeddedOnly)
        .def("getTriangulation", &NSurfaceSet::getTriangulation,
            return_value_policy<reference_existing_object>())
        .def("getNumberOfSurfaces", &NSurfaceSet::getNumberOfSurfaces)
        .def("getSurface", &NSurfaceSet::getSurface,
            return_internal_reference<>())
        .def("getShareableObject", &NSurfaceSet::getShareableObject,
            return_internal_reference<>())
        .def("writeAllSurfaces", writeAllSurfaces_stdio)
    ;
}

