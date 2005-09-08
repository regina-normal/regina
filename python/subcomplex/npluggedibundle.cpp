
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

#include "subcomplex/nlayeredsolidtorus.h"
#include "subcomplex/npluggedibundle.h"
#include "subcomplex/ntorusplug.h"
#include "triangulation/ntriangulation.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NPluggedIBundle;
using regina::NPluggedIBundleCore;

void addNPluggedIBundle() {
    class_<NPluggedIBundle, bases<regina::NStandardTriangulation>,
            std::auto_ptr<NPluggedIBundle>, boost::noncopyable>
            ("NPluggedIBundle", no_init)
        .def("getPlug", &NPluggedIBundle::getPlug,
            return_value_policy<reference_existing_object>())
        .def("isPluggedIBundle", &NPluggedIBundle::isPluggedIBundle,
            return_value_policy<manage_new_object>())
        .staticmethod("isPluggedIBundle")
    ;

    implicitly_convertible<std::auto_ptr<NPluggedIBundle>,
        std::auto_ptr<regina::NStandardTriangulation> >();

    scope s = class_<NPluggedIBundleCore, boost::noncopyable>
            ("NPluggedIBundleCore", no_init);

    s.attr("T_3_1") = NPluggedIBundleCore::T_3_1;
    s.attr("T_3_2") = NPluggedIBundleCore::T_3_2;
    s.attr("T_5_1") = NPluggedIBundleCore::T_5_1;
    s.attr("T_6_1") = NPluggedIBundleCore::T_6_1;
    s.attr("T_6_2") = NPluggedIBundleCore::T_6_2;
    s.attr("T_6_3") = NPluggedIBundleCore::T_6_3;
    s.attr("T_6_4") = NPluggedIBundleCore::T_6_4;
}

