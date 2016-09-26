
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
#include "census/ncensus.h"
#include "../helpers.h"

using namespace boost::python;
using regina::NCensus;
using regina::NCensusDB;
using regina::NCensusHit;
using regina::NCensusHits;
using regina::NTriangulation;

namespace {
    NCensusHits* (*lookup_tri)(const NTriangulation&) = &NCensus::lookup;
    NCensusHits* (*lookup_sig)(const std::string&) = &NCensus::lookup;
}

void addNCensus() {
    class_<NCensusDB>("NCensusDB",
            init<const std::string&, const std::string&>())
        .def("filename", &NCensusDB::filename,
            return_value_policy<copy_const_reference>())
        .def("desc", &NCensusDB::desc,
            return_value_policy<copy_const_reference>())
        .def(regina::python::add_eq_operators())
    ;

    class_<NCensusHit, std::auto_ptr<NCensusHit>,
            boost::noncopyable>("NCensusHit", no_init)
        .def("name", &NCensusHit::name,
            return_value_policy<copy_const_reference>())
        .def("db", &NCensusHit::db,
            return_internal_reference<>())
        .def("next", &NCensusHit::next,
            return_internal_reference<>())
        .def(regina::python::add_eq_operators())
    ;

    class_<NCensusHits, std::auto_ptr<NCensusHits>,
            boost::noncopyable>("NCensusHits", init<>())
        .def("first", &NCensusHits::first,
            return_internal_reference<>())
        .def("count", &NCensusHits::count)
        .def("empty", &NCensusHits::empty)
        .def(regina::python::add_eq_operators())
    ;

    class_<NCensus, std::auto_ptr<NCensus>,
            boost::noncopyable>("NCensus", no_init)
        .def("lookup", lookup_tri, return_value_policy<manage_new_object>())
        .def("lookup", lookup_sig, return_value_policy<manage_new_object>())
        .def(regina::python::no_eq_operators())
        .staticmethod("lookup")
    ;
}

