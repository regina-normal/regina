
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

#include "packet/npacket.h"
#include <boost/python.hpp>

using namespace boost::python;
using regina::NPacket;

namespace {
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_clone,
        NPacket::clone, 0, 2);
}

void addNPacket() {
    class_<NPacket, boost::noncopyable,
            bases<regina::ShareableObject> >("NPacket", no_init)
        .def("getPacketType", &NPacket::getPacketType)
        .def("getPacketTypeName", &NPacket::getPacketTypeName)
        .def("getPacketLabel", &NPacket::getPacketLabel,
            return_value_policy<return_by_value>())
        .def("setPacketLabel", &NPacket::setPacketLabel)
        .def("getFullName", &NPacket::getFullName)
        .def("makeUniqueLabel", &NPacket::makeUniqueLabel)
        .def("makeUniqueLabels", &NPacket::makeUniqueLabels)
        .def("hasTag", &NPacket::hasTag)
        .def("hasTags", &NPacket::hasTags)
        .def("addTag", &NPacket::addTag)
        .def("removeTag", &NPacket::removeTag)
        .def("removeAllTags", &NPacket::removeAllTags)
        // .def("getTags", &NPacket::getTags)
        .def("getTreeParent", &NPacket::getTreeParent,
            return_value_policy<reference_existing_object>())
        .def("getFirstTreeChild", &NPacket::getFirstTreeChild,
            return_value_policy<reference_existing_object>())
        .def("getLastTreeChild", &NPacket::getLastTreeChild,
            return_value_policy<reference_existing_object>())
        .def("getNextTreeSibling", &NPacket::getNextTreeSibling,
            return_value_policy<reference_existing_object>())
        .def("getPrevTreeSibling", &NPacket::getPrevTreeSibling,
            return_value_policy<reference_existing_object>())
        .def("getTreeMatriarch", &NPacket::getTreeMatriarch,
            return_value_policy<reference_existing_object>())
        .def("levelsDownTo", &NPacket::levelsDownTo)
        .def("levelsUpTo", &NPacket::levelsUpTo)
        .def("isGrandparentOf", &NPacket::isGrandparentOf)
        .def("getNumberOfChildren", &NPacket::getNumberOfChildren)
        .def("getNumberOfDescendants", &NPacket::getNumberOfDescendants)
        .def("getTotalTreeSize", &NPacket::getTotalTreeSize)
        .def("insertChildFirst", &NPacket::insertChildFirst)
        .def("insertChildLast", &NPacket::insertChildLast)
        .def("insertChildAfter", &NPacket::insertChildAfter)
        .def("makeOrphan", &NPacket::makeOrphan)
        .def("swapWithNextSibling", &NPacket::swapWithNextSibling)
        .def("dependsOnParent", &NPacket::dependsOnParent)
        .def("isPacketEditable", &NPacket::isPacketEditable)
        // .def("clone", &NPacket::clone, OL_clone(),
            // return_value_policy<reference_existing_object>())
    ;
}

