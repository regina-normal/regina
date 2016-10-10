
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
#include "surfaces/disc.h"
#include "surfaces/normalsurface.h"
#include "../helpers.h"

using namespace boost::python;
using regina::DiscSpec;
using regina::DiscSetTet;
using regina::DiscSetSurface;
using regina::DiscSpecIterator;

namespace {
    void inc_operator(DiscSpecIterator& s) {
        s++;
    }
    const DiscSpec& deref_operator(DiscSpecIterator& s) {
        return *s;
    }
}

void addDisc() {
    class_<DiscSpec>("DiscSpec")
        .def(init<unsigned long, int, unsigned long>())
        .def(init<const DiscSpec&>())
        .def_readwrite("tetIndex", &DiscSpec::tetIndex)
        .def_readwrite("type", &DiscSpec::type)
        .def_readwrite("number", &DiscSpec::number)
        .def(self_ns::str(self))
        .def(regina::python::add_eq_operators())
    ;

    def("numberDiscsAwayFromVertex", regina::numberDiscsAwayFromVertex);
    def("discOrientationFollowsEdge", regina::discOrientationFollowsEdge);

    class_<DiscSetTet, std::auto_ptr<DiscSetTet>,
            boost::noncopyable>("DiscSetTet",
            init<const regina::NormalSurface&, unsigned long>())
        .def("nDiscs", &DiscSetTet::nDiscs)
        .def("arcFromDisc", &DiscSetTet::arcFromDisc)
        .def("discFromArc", &DiscSetTet::discFromArc)
        .def(regina::python::add_eq_operators())
    ;

    class_<DiscSetSurface, std::auto_ptr<DiscSetSurface>,
            boost::noncopyable>("DiscSetSurface",
            init<const regina::NormalSurface&>())
        .def("nTets", &DiscSetSurface::nTets)
        .def("nDiscs", &DiscSetSurface::nDiscs)
        .def("tetDiscs", &DiscSetSurface::tetDiscs,
            return_internal_reference<>())
        .def("adjacentDisc", &DiscSetSurface::adjacentDisc,
            return_value_policy<manage_new_object>())
        .def(regina::python::add_eq_operators())
    ;

    class_<DiscSpecIterator, boost::noncopyable>("DiscSpecIterator")
        .def(init<const DiscSetSurface&>())
        .def("init", &DiscSpecIterator::init)
        .def("inc", inc_operator)
        .def("deref", deref_operator, return_value_policy<return_by_value>())
        .def("done", &DiscSpecIterator::done)
        .def(regina::python::add_eq_operators())
    ;

    scope().attr("NDiscSpec") = scope().attr("DiscSpec");
    scope().attr("NDiscSetTet") = scope().attr("DiscSetTet");
    scope().attr("NDiscSetSurface") = scope().attr("DiscSetSurface");
    scope().attr("NDiscSpecIterator") = scope().attr("DiscSpecIterator");
}

