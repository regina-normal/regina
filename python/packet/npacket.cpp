
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
    NPacket* (NPacket::*firstTreePacket_non_const)(const std::string&) =
        &NPacket::firstTreePacket;
    NPacket* (NPacket::*nextTreePacket_non_const)(const std::string&) =
        &NPacket::nextTreePacket;
    NPacket* (NPacket::*findPacketLabel_non_const)(const std::string&) =
        &NPacket::findPacketLabel;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_moveUp,
        NPacket::moveUp, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_moveDown,
        NPacket::moveDown, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_nextTreePacket,
        NPacket::nextTreePacket, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_clone,
        NPacket::clone, 0, 2);

    void insertChildFirst_own(NPacket& parent, std::auto_ptr<NPacket> child) {
        parent.insertChildFirst(child.get());
        child.release();
    }
    void insertChildLast_own(NPacket& parent, std::auto_ptr<NPacket> child) {
        parent.insertChildLast(child.get());
        child.release();
    }
    void insertChildAfter_own(NPacket& parent, std::auto_ptr<NPacket> child,
            NPacket* prevChild) {
        parent.insertChildAfter(child.get(), prevChild);
        child.release();
    }

    boost::python::list getTags_list(const NPacket* p) {
        const std::set<std::string>& tags = p->getTags();
        std::set<std::string>::const_iterator it;

        boost::python::list ans;
        for (it = tags.begin(); it != tags.end(); it++)
            ans.append(*it);
        return ans;
    }
}

void addNPacket() {
    class_<NPacket, boost::noncopyable,
            bases<regina::ShareableObject>,
            std::auto_ptr<NPacket> >("NPacket", no_init)
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
        .def("getTags", getTags_list)
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
        .def("insertChildFirst", insertChildFirst_own)
        .def("insertChildLast", insertChildLast_own)
        .def("insertChildAfter", insertChildAfter_own)
        .def("makeOrphan", &NPacket::makeOrphan)
        .def("swapWithNextSibling", &NPacket::swapWithNextSibling)
        .def("moveUp", &NPacket::moveUp, OL_moveUp())
        .def("moveDown", &NPacket::moveDown, OL_moveDown())
        .def("moveToFirst", &NPacket::moveToFirst)
        .def("moveToLast", &NPacket::moveToLast)
        .def("nextTreePacket", nextTreePacket_non_const, OL_nextTreePacket()
            [return_value_policy<reference_existing_object>()])
        .def("firstTreePacket", firstTreePacket_non_const,
            return_value_policy<reference_existing_object>())
        .def("findPacketLabel", findPacketLabel_non_const,
            return_value_policy<reference_existing_object>())
        .def("dependsOnParent", &NPacket::dependsOnParent)
        .def("isPacketEditable", &NPacket::isPacketEditable)
        .def("clone", &NPacket::clone, OL_clone()[
            return_value_policy<reference_existing_object>()])
    ;
}

