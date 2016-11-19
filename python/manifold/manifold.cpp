
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

#include <boost/python.hpp>
#include "algebra/abeliangroup.h"
#include "manifold/manifold.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../safeheldtype.h"

using namespace boost::python;
using namespace regina::python;
using regina::Manifold;

namespace {
    void writeName_stdio(const Manifold& m) {
        m.writeName(std::cout);
    }
    void writeTeXName_stdio(const Manifold& m) {
        m.writeTeXName(std::cout);
    }
    void writeStructure_stdio(const Manifold& m) {
        m.writeStructure(std::cout);
    }
}

void addManifold() {
    class_<Manifold, boost::noncopyable, std::auto_ptr<Manifold> >
            ("Manifold", no_init)
        .def("name", &Manifold::name)
        .def("TeXName", &Manifold::TeXName)
        .def("structure", &Manifold::structure)
        .def("construct", &Manifold::construct,
            return_value_policy<to_held_type<> >())
        .def("homology", &Manifold::homology,
            return_value_policy<manage_new_object>())
        .def("homologyH1", &Manifold::homologyH1,
            return_value_policy<manage_new_object>())
        .def("isHyperbolic", &Manifold::isHyperbolic)
        .def("writeName", writeName_stdio)
        .def("writeTeXName", writeTeXName_stdio)
        .def("writeStructure", writeStructure_stdio)
        .def(self < self)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NManifold") = scope().attr("Manifold");

}

