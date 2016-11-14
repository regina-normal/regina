
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
#include "manifold/nsfs.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NGraphTriple;
using regina::Matrix2;
using regina::NSFSpace;

namespace {
    NGraphTriple* createNGraphTriple(const NSFSpace& s1,
            const NSFSpace& s2, const NSFSpace& s3,
            const Matrix2& m1, const Matrix2& m2) {
        return new NGraphTriple(
            new NSFSpace(s1), new NSFSpace(s2), new NSFSpace(s3), m1, m2);
    }
}

void addNGraphTriple() {
    class_<NGraphTriple, bases<regina::NManifold>,
            std::auto_ptr<NGraphTriple>, boost::noncopyable>
            ("NGraphTriple", no_init)
        .def("__init__", make_constructor(createNGraphTriple))
        .def("end", &NGraphTriple::end,
            return_internal_reference<>())
        .def("centre", &NGraphTriple::centre,
            return_internal_reference<>())
        .def("matchingReln", &NGraphTriple::matchingReln,
            return_internal_reference<>())
        .def(self < self)
        .def(regina::python::add_eq_operators())
    ;

    implicitly_convertible<std::auto_ptr<NGraphTriple>,
        std::auto_ptr<regina::NManifold> >();
}

