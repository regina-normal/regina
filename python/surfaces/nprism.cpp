
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

#include "surfaces/nprism.h"
#include "surfaces/nnormalsurface.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NPrismSpec;
using regina::NPrismSetSurface;

void addNPrism() {
    class_<NPrismSpec>("NPrismSpec")
        .def(init<unsigned long, int>())
        .def(init<const NPrismSpec&>())
        .def_readwrite("tetIndex", &NPrismSpec::tetIndex)
        .def_readwrite("edge", &NPrismSpec::edge)
        .def(self == self)
        .def(self_ns::str(self))
    ;

    class_<NPrismSetSurface, std::auto_ptr<NPrismSetSurface>,
            boost::noncopyable>("NPrismSetSurface",
            init<const regina::NNormalSurface&>())
        .def("getQuadType", &NPrismSetSurface::getQuadType)
    ;
}

