
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

#include "surfaces/ndisc.h"
#include "surfaces/nnormalsurface.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NDiscSpec;
using regina::NDiscSetTet;
using regina::NDiscSetSurface;
using regina::NDiscSpecIterator;

namespace {
    void inc_operator(NDiscSpecIterator& s) {
        s++;
    }
    const NDiscSpec& deref_operator(NDiscSpecIterator& s) {
        return *s;
    }
}

void addNDisc() {
    class_<NDiscSpec>("NDiscSpec")
        .def(init<unsigned long, int, unsigned long>())
        .def(init<const NDiscSpec&>())
        .def_readwrite("tetIndex", &NDiscSpec::tetIndex)
        .def_readwrite("type", &NDiscSpec::type)
        .def_readwrite("number", &NDiscSpec::number)
        .def(self == self)
        .def(self_ns::str(self))
    ;

    def("numberDiscsAwayFromVertex", regina::numberDiscsAwayFromVertex);
    def("discOrientationFollowsEdge", regina::discOrientationFollowsEdge);

    class_<NDiscSetTet, std::auto_ptr<NDiscSetTet>,
            boost::noncopyable>("NDiscSetTet",
            init<const regina::NNormalSurface&, unsigned long>())
        .def("nDiscs", &NDiscSetTet::nDiscs)
        .def("arcFromDisc", &NDiscSetTet::arcFromDisc)
        .def("discFromArc", &NDiscSetTet::discFromArc)
    ;

    class_<NDiscSetSurface, std::auto_ptr<NDiscSetSurface>,
            boost::noncopyable>("NDiscSetSurface",
            init<const regina::NNormalSurface&>())
        .def("nTets", &NDiscSetSurface::nTets)
        .def("nDiscs", &NDiscSetSurface::nDiscs)
        .def("tetDiscs", &NDiscSetSurface::tetDiscs,
            return_internal_reference<>())
        .def("adjacentDisc", &NDiscSetSurface::adjacentDisc,
            return_value_policy<manage_new_object>())
    ;

    class_<NDiscSpecIterator, boost::noncopyable>("NDiscSpecIterator")
        .def(init<const NDiscSetSurface&>())
        .def("init", &NDiscSpecIterator::init)
        .def("inc", inc_operator)
        .def("deref", deref_operator, return_value_policy<return_by_value>())
        .def("done", &NDiscSpecIterator::done)
    ;
}

