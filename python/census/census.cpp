
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
#include "census/census.h"
#include "triangulation/dim3.h"
#include "../helpers.h"

using namespace boost::python;
using regina::Census;
using regina::CensusDB;
using regina::CensusHit;
using regina::CensusHits;
using regina::Triangulation;

namespace {
    CensusHits* (*lookup_tri)(const Triangulation<3>&) = &Census::lookup;
    CensusHits* (*lookup_sig)(const std::string&) = &Census::lookup;
}

void addCensus() {
    class_<CensusDB>("CensusDB",
            init<const std::string&, const std::string&>())
        .def("filename", &CensusDB::filename,
            return_value_policy<copy_const_reference>())
        .def("desc", &CensusDB::desc,
            return_value_policy<copy_const_reference>())
        .def(regina::python::add_eq_operators())
    ;

    class_<CensusHit, std::auto_ptr<CensusHit>,
            boost::noncopyable>("CensusHit", no_init)
        .def("name", &CensusHit::name,
            return_value_policy<copy_const_reference>())
        .def("db", &CensusHit::db,
            return_internal_reference<>())
        .def("next", &CensusHit::next,
            return_internal_reference<>())
        .def(regina::python::add_eq_operators())
    ;

    class_<CensusHits, std::auto_ptr<CensusHits>,
            boost::noncopyable>("CensusHits", init<>())
        .def("first", &CensusHits::first,
            return_internal_reference<>())
        .def("count", &CensusHits::count)
        .def("empty", &CensusHits::empty)
        .def(regina::python::add_eq_operators())
    ;

    class_<Census, std::auto_ptr<Census>,
            boost::noncopyable>("Census", no_init)
        .def("lookup", lookup_tri, return_value_policy<manage_new_object>())
        .def("lookup", lookup_sig, return_value_policy<manage_new_object>())
        .def(regina::python::no_eq_operators())
        .staticmethod("lookup")
    ;

    scope().attr("NCensusDB") = scope().attr("CensusDB");
    scope().attr("NCensusHit") = scope().attr("CensusHit");
    scope().attr("NCensusHits") = scope().attr("CensusHits");
    scope().attr("NCensus") = scope().attr("Census");
}

