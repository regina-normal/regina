
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
#include "census/census.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Census;
using regina::CensusDB;
using regina::CensusHit;
using regina::CensusHitIterator;
using regina::CensusHits;
using regina::Triangulation;

namespace {
    // Support for iterables and iterators:
    const CensusHit* nextHit(CensusHitIterator& it) {
        if (*it)
            return *it++;
        else
            throw pybind11::stop_iteration();
    }
}

void addCensus(pybind11::module_& m) {
    auto db = pybind11::class_<CensusDB>(m, "CensusDB")
        .def(pybind11::init<const std::string&, const std::string&>())
        .def(pybind11::init<const CensusDB&>())
        .def("filename", &CensusDB::filename)
        .def("desc", &CensusDB::desc)
        .def("lookup", &CensusDB::lookup)
    ;
    regina::python::add_eq_operators(db);

    auto h = pybind11::class_<CensusHit>(m, "CensusHit")
        .def("name", &CensusHit::name)
        .def("db", &CensusHit::db,
            pybind11::return_value_policy::reference)
        .def("next", &CensusHit::next,
            pybind11::return_value_policy::reference_internal)
    ;
    regina::python::add_eq_operators(h);

    auto hs = pybind11::class_<CensusHits>(m, "CensusHits")
        .def(pybind11::init<>())
        .def("first", &CensusHits::first,
            pybind11::return_value_policy::reference_internal)
        .def("count", &CensusHits::count)
        .def("empty", &CensusHits::empty)
        .def("begin", &CensusHits::begin)
        .def("end", &CensusHits::end)
        .def("__iter__", &CensusHits::begin)
    ;
    regina::python::add_eq_operators(hs);

    auto it = pybind11::class_<CensusHitIterator>(m, "CensusHitIterator")
        .def(pybind11::init<>())
        .def(pybind11::init<const CensusHitIterator&>())
        .def(pybind11::init<const CensusHit*>())
        .def("next", nextHit, // for python 2
            pybind11::return_value_policy::reference)
        .def("__next__", nextHit, // for python 3
            pybind11::return_value_policy::reference)
    ;
    regina::python::add_eq_operators(it);

    auto c = pybind11::class_<Census>(m, "Census")
        .def_static("lookup",
            overload_cast<const Triangulation<3>&>(&Census::lookup))
        .def_static("lookup",
            overload_cast<const std::string&>(&Census::lookup))
    ;
    regina::python::no_eq_operators(c);

    m.attr("NCensusDB") = m.attr("CensusDB");
    m.attr("NCensusHit") = m.attr("CensusHit");
    m.attr("NCensusHits") = m.attr("CensusHits");
    m.attr("NCensus") = m.attr("Census");
}

