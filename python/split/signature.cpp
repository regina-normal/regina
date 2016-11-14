
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

#include "split/signature.h"
#include "triangulation/dim3.h"
#include "../helpers.h"
#include "../safeheldtype.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Signature;

namespace {
    void writeCycles(const Signature& sig, const std::string& cycleOpen,
            const std::string& cycleClose, const std::string& cycleJoin) {
        sig.writeCycles(std::cout, cycleOpen, cycleClose, cycleJoin);
    }
}

void addSignature() {
    class_<Signature, std::auto_ptr<Signature>,
        boost::noncopyable>("Signature", init<const Signature&>())
        .def("order", &Signature::order)
        .def("parse", &Signature::parse,
            return_value_policy<manage_new_object>())
        .def("triangulate", &Signature::triangulate,
            return_value_policy<to_held_type<> >())
        .def("writeCycles", &writeCycles)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
        .staticmethod("parse")
    ;

    scope().attr("NSignature") = scope().attr("Signature");
}

