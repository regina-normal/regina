
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
#include "manifold/graphtriple.h"
#include "manifold/sfs.h"
#include "../helpers.h"

using namespace boost::python;
using regina::GraphTriple;
using regina::Matrix2;
using regina::SFSpace;

namespace {
    GraphTriple* createGraphTriple(const SFSpace& s1,
            const SFSpace& s2, const SFSpace& s3,
            const Matrix2& m1, const Matrix2& m2) {
        return new GraphTriple(
            new SFSpace(s1), new SFSpace(s2), new SFSpace(s3), m1, m2);
    }
}

void addGraphTriple() {
    class_<GraphTriple, bases<regina::Manifold>,
            std::auto_ptr<GraphTriple>, boost::noncopyable>
            ("GraphTriple", no_init)
        .def("__init__", make_constructor(createGraphTriple))
        .def("end", &GraphTriple::end,
            return_internal_reference<>())
        .def("centre", &GraphTriple::centre,
            return_internal_reference<>())
        .def("matchingReln", &GraphTriple::matchingReln,
            return_internal_reference<>())
        .def(self < self)
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NGraphTriple") = scope().attr("GraphTriple");

    implicitly_convertible<std::auto_ptr<GraphTriple>,
        std::auto_ptr<regina::Manifold> >();
}

