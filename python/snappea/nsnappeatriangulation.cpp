
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "snappea/nsnappeatriangulation.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NSnapPeaTriangulation;
using regina::NTriangulation;

namespace {
    double (NSnapPeaTriangulation::*volume_void)() const =
        &NSnapPeaTriangulation::volume;
    double (NSnapPeaTriangulation::*volume_precision)(int&) const =
        &NSnapPeaTriangulation::volume;

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_enableKernelMessages,
        NSnapPeaTriangulation::enableKernelMessages, 0, 1);
}

void addNSnapPeaTriangulation() {
    scope s = class_<NSnapPeaTriangulation, bases<regina::ShareableObject>,
            std::auto_ptr<NSnapPeaTriangulation>, boost::noncopyable>
            ("NSnapPeaTriangulation", init<const NSnapPeaTriangulation&>())
        .def(init<const NTriangulation&>())
        .def("isNull", &NSnapPeaTriangulation::isNull)
        .def("solutionType", &NSnapPeaTriangulation::solutionType)
        .def("volume", volume_void)
        .def("volume", volume_precision)
        .def("dump", &NSnapPeaTriangulation::dump)
        .def("saveAsSnapPea", &NSnapPeaTriangulation::saveAsSnapPea)
        .def("kernelMessagesEnabled",
            &NSnapPeaTriangulation::kernelMessagesEnabled)
        .def("enableKernelMessages",
            &NSnapPeaTriangulation::enableKernelMessages,
            OL_enableKernelMessages())
        .def("disableKernelMessages",
            &NSnapPeaTriangulation::disableKernelMessages)
        .staticmethod("kernelMessagesEnabled")
        .staticmethod("enableKernelMessages")
        .staticmethod("disableKernelMessages")
    ;

    enum_<NSnapPeaTriangulation::SolutionType>("SolutionType")
        .value("not_attempted", NSnapPeaTriangulation::not_attempted)
        .value("geometric_solution", NSnapPeaTriangulation::geometric_solution)
        .value("nongeometric_solution",
            NSnapPeaTriangulation::nongeometric_solution)
        .value("flat_solution", NSnapPeaTriangulation::flat_solution)
        .value("degenerate_solution",
            NSnapPeaTriangulation::degenerate_solution)
        .value("other_solution", NSnapPeaTriangulation::other_solution)
        .value("no_solution", NSnapPeaTriangulation::no_solution)
    ;
}

