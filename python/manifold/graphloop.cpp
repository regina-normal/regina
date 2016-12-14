
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
#include "manifold/graphloop.h"
#include "manifold/sfs.h"
#include "../helpers.h"

using namespace boost::python;
using regina::GraphLoop;
using regina::Matrix2;
using regina::SFSpace;

namespace {
    GraphLoop* createGraphLoop_longs(const SFSpace& s,
            long a, long b, long c, long d) {
        return new GraphLoop(new SFSpace(s), a, b, c, d);
    }

    GraphLoop* createGraphLoop_matrix(const SFSpace& s, const Matrix2& m) {
        return new GraphLoop(new SFSpace(s), m);
    }
}

void addGraphLoop() {
    class_<GraphLoop, bases<regina::Manifold>,
            std::auto_ptr<GraphLoop>, boost::noncopyable>
            ("GraphLoop", no_init)
        .def("__init__", make_constructor(createGraphLoop_longs))
        .def("__init__", make_constructor(createGraphLoop_matrix))
        .def("sfs", &GraphLoop::sfs,
            return_internal_reference<>())
        .def("matchingReln", &GraphLoop::matchingReln,
            return_internal_reference<>())
        .def(self < self)
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NGraphLoop") = scope().attr("GraphLoop");

    implicitly_convertible<std::auto_ptr<GraphLoop>,
        std::auto_ptr<regina::Manifold> >();
}

