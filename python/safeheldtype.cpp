
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

// Explicitly making Python.h the first included file here to make it work
// with old versions of Mac OS X (see https://bugs.python.org/issue10910)
#include "Python.h"

#include "safeheldtype.h"

#include <boost/python/object.hpp>

#include <boost/version.hpp>
#if BOOST_VERSION >= 105600
#define USE_BOOST_DEMANGLE
#endif
#ifdef USE_BOOST_DEMANGLE
#include <boost/core/demangle.hpp>
#endif



namespace regina {
namespace python {

void raiseExpiredException(const std::type_info& info)
{
    #ifdef USE_BOOST_DEMANGLE
    const std::string typeName = boost::core::demangle(info.name());
    #else
    const std::string typeName = info.name();
    #endif

    const std::string msg =
        "Python reference to object of type " + typeName + " expired.";
    
    PyErr_SetString(PyExc_RuntimeError, msg.c_str());
}

} } // regina::python
