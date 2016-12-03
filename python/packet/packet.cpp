
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

#include "packet/packet.h"
#include "../safeheldtype.h"
#include "../helpers.h"

// Held type must be declared before boost/python.hpp
#include <boost/python.hpp>

using namespace boost::python;
using namespace regina::python;
using regina::Packet;

namespace {
    Packet* (Packet::*firstTreePacket_non_const)(const std::string&) =
        &Packet::firstTreePacket;
    Packet* (Packet::*nextTreePacket_non_const)(const std::string&) =
        &Packet::nextTreePacket;
    Packet* (Packet::*findPacketLabel_non_const)(const std::string&) =
        &Packet::findPacketLabel;
    bool (Packet::*save_filename)(const char*, bool) const = &Packet::save;
    Packet* (*open_filename)(const char*) = &regina::open;

    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_moveUp,
        Packet::moveUp, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_moveDown,
        Packet::moveDown, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_nextTreePacket,
        Packet::nextTreePacket, 0, 1);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_clone,
        Packet::clone, 0, 2);
    BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(OL_save,
        Packet::save, 1, 2);

    void reparent_check(Packet& child, Packet* newParent,
            bool first = false) {
        if (child.parent())
            child.reparent(newParent, first);
        else {
            PyErr_SetString(PyExc_AssertionError,
                "reparent() cannot be used on packets with no parent");
            ::boost::python::throw_error_already_set();
        }
    }

    void writeXMLFile_stdout(const Packet& p) {
        p.writeXMLFile(std::cout);
    }

    BOOST_PYTHON_FUNCTION_OVERLOADS(OL_reparent, reparent_check, 2, 3);

    boost::python::list tags_list(const Packet* p) {
        boost::python::list ans;
        for (auto& t : p->tags())
            ans.append(t);
        return ans;
    }
}

void addPacket() {
    class_<Packet, boost::noncopyable,
            SafeHeldType<Packet> >("Packet", no_init)
        .def("type", &Packet::type)
        .def("typeName", &Packet::typeName)
        .def("label", &Packet::label,
            return_value_policy<return_by_value>())
        .def("humanLabel", &Packet::humanLabel)
        .def("adornedLabel", &Packet::adornedLabel)
        .def("setLabel", &Packet::setLabel)
        .def("fullName", &Packet::fullName)
        .def("hasTag", &Packet::hasTag)
        .def("hasTags", &Packet::hasTags)
        .def("addTag", &Packet::addTag)
        .def("removeTag", &Packet::removeTag)
        .def("removeAllTags", &Packet::removeAllTags)
        .def("tags", tags_list)
        .def("parent", &Packet::parent,
            return_value_policy<to_held_type<> >())
        .def("firstChild", &Packet::firstChild,
            return_value_policy<to_held_type<> >())
        .def("lastChild", &Packet::lastChild,
            return_value_policy<to_held_type<> >())
        .def("nextSibling", &Packet::nextSibling,
            return_value_policy<to_held_type<> >())
        .def("prevSibling", &Packet::prevSibling,
            return_value_policy<to_held_type<> >())
        .def("root", &Packet::root,
            return_value_policy<to_held_type<> >())
        .def("hasOwner", &Packet::hasOwner)
        .def("levelsDownTo", &Packet::levelsDownTo)
        .def("levelsUpTo", &Packet::levelsUpTo)
        .def("isGrandparentOf", &Packet::isGrandparentOf)
        .def("countChildren", &Packet::countChildren)
        .def("countDescendants", &Packet::countDescendants)
        .def("totalTreeSize", &Packet::totalTreeSize)
        .def("insertChildFirst", &Packet::insertChildFirst)
        .def("insertChildLast", &Packet::insertChildLast)
        .def("insertChildAfter", &Packet::insertChildAfter)
        .def("makeOrphan", &Packet::makeOrphan)
        .def("reparent", reparent_check, OL_reparent())
        .def("transferChildren", &Packet::transferChildren)
        .def("swapWithNextSibling", &Packet::swapWithNextSibling)
        .def("moveUp", &Packet::moveUp, OL_moveUp())
        .def("moveDown", &Packet::moveDown, OL_moveDown())
        .def("moveToFirst", &Packet::moveToFirst)
        .def("moveToLast", &Packet::moveToLast)
        .def("sortChildren", &Packet::sortChildren)
        .def("nextTreePacket", nextTreePacket_non_const, OL_nextTreePacket()
            [return_value_policy<to_held_type<> >()])
        .def("firstTreePacket", firstTreePacket_non_const,
            return_value_policy<to_held_type<> >())
        .def("findPacketLabel", findPacketLabel_non_const,
            return_value_policy<to_held_type<> >())
        .def("dependsOnParent", &Packet::dependsOnParent)
        .def("isPacketEditable", &Packet::isPacketEditable)
        .def("clone", &Packet::clone,
            OL_clone()[return_value_policy<to_held_type<> >()])
        .def("save", save_filename, OL_save())
        .def("writeXMLFile", writeXMLFile_stdout)
        .def("internalID", &Packet::internalID)
        .def(regina::python::add_output())
        .def(regina::python::add_eq_operators())
    ;

    def("open", open_filename, return_value_policy<to_held_type<> >());

    FIX_REGINA_BOOST_CONVERTERS(Packet);

    scope().attr("NPacket") = scope().attr("Packet");
}

