
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

#include "file/nxmlfile.h"
#include "packet/npacket.h"
#include <boost/python.hpp>

using namespace boost::python;

namespace {
    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_writeXMLFile,
        regina::writeXMLFile, 2, 3);
}

void addNXMLFile() {
    def("writeXMLFile", regina::writeXMLFile, OL_writeXMLFile());
    def("readXMLFile", regina::readXMLFile,
        return_value_policy<manage_new_object>());
    def("readFileMagic", regina::readFileMagic,
        return_value_policy<manage_new_object>());
}

