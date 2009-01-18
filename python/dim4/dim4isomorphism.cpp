
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include "dim4/dim4isomorphism.h"
#include "dim4/dim4triangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::Dim4Isomorphism;

namespace {
    int (Dim4Isomorphism::*pentImage_const)(unsigned) const =
        &Dim4Isomorphism::pentImage;
    regina::NPerm5 (Dim4Isomorphism::*facetPerm_const)(unsigned) const =
        &Dim4Isomorphism::facetPerm;

    regina::Dim4PentFacet Dim4iso_getItem(const Dim4Isomorphism& iso,
            const regina::Dim4PentFacet& f) {
        return iso[f];
    }
}

void addDim4Isomorphism() {
    class_<Dim4Isomorphism, bases<regina::ShareableObject>,
            std::auto_ptr<Dim4Isomorphism>, boost::noncopyable>
            ("Dim4Isomorphism", init<const Dim4Isomorphism&>())
        .def("getSourcePentachora", &Dim4Isomorphism::getSourcePentachora)
        .def("pentImage", pentImage_const)
        .def("facetPerm", facetPerm_const)
        .def("__getitem__", Dim4iso_getItem)
        .def("isIdentity", &Dim4Isomorphism::isIdentity)
        .def("apply", &Dim4Isomorphism::apply,
            return_value_policy<manage_new_object>())
    ;
}

