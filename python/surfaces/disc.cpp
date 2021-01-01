
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "../pybind11/pybind11.h"
#include "surfaces/disc.h"
#include "surfaces/normalsurface.h"
#include "../helpers.h"

using regina::DiscSpec;
using regina::DiscSetTet;
using regina::DiscSetSurface;
using regina::DiscSpecIterator;

namespace {
    // Support for iterables and iterators:
    DiscSpec nextDiscSpec(DiscSpecIterator& it) {
        if (! it.done())
            return *it++;
        else
            throw pybind11::stop_iteration();
    }
}
void addDisc(pybind11::module_& m) {
    auto d = pybind11::class_<DiscSpec>(m, "DiscSpec")
        .def(pybind11::init<>())
        .def(pybind11::init<unsigned long, int, unsigned long>())
        .def(pybind11::init<const DiscSpec&>())
        .def_readwrite("tetIndex", &DiscSpec::tetIndex)
        .def_readwrite("type", &DiscSpec::type)
        .def_readwrite("number", &DiscSpec::number)
    ;
    regina::python::add_output_ostream(d);
    regina::python::add_eq_operators(d);

    m.def("numberDiscsAwayFromVertex", regina::numberDiscsAwayFromVertex);
    m.def("discOrientationFollowsEdge", regina::discOrientationFollowsEdge);

    auto t = pybind11::class_<DiscSetTet>(m, "DiscSetTet")
        .def(pybind11::init<const regina::NormalSurface&, unsigned long>())
        .def("nDiscs", &DiscSetTet::nDiscs)
        .def("arcFromDisc", &DiscSetTet::arcFromDisc)
        .def("discFromArc", &DiscSetTet::discFromArc)
    ;
    regina::python::add_eq_operators(t);

    auto s = pybind11::class_<DiscSetSurface>(m, "DiscSetSurface")
        .def(pybind11::init<const regina::NormalSurface&>())
        .def("nTets", &DiscSetSurface::nTets)
        .def("nDiscs", &DiscSetSurface::nDiscs)
        .def("tetDiscs", &DiscSetSurface::tetDiscs,
            pybind11::return_value_policy::reference_internal)
        .def("adjacentDisc", &DiscSetSurface::adjacentDisc)
        .def("__iter__", [](const DiscSetSurface& d) {
            return DiscSpecIterator(d);
        })
    ;
    regina::python::add_eq_operators(s);

    auto it = pybind11::class_<DiscSpecIterator>(m, "DiscSpecIterator")
        .def("next", nextDiscSpec) // for python 2
        .def("__next__", nextDiscSpec) // for python 3
    ;
    regina::python::add_eq_operators(it);

    m.attr("NDiscSpec") = m.attr("DiscSpec");
    m.attr("NDiscSetTet") = m.attr("DiscSetTet");
    m.attr("NDiscSetSurface") = m.attr("DiscSetSurface");
    m.attr("NDiscSpecIterator") = m.attr("DiscSpecIterator");
}

