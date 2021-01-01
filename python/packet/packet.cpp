
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

#include "../pybind11/pybind11.h"
#include "../pybind11/stl.h"
#include "packet/packet.h"
#include "utilities/safeptr.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::ChildIterator;
using regina::Packet;
using regina::PacketChildren;
using regina::PacketDescendants;
using regina::PacketShell;
using regina::SubtreeIterator;

namespace {
    // Support for iterables and iterators:
    template <typename Iterator>
    Packet* next(Iterator& it) {
        if (*it)
            return *it++;
        else
            throw pybind11::stop_iteration();
    }
}

void addPacket(pybind11::module_& m) {
    auto c1 = pybind11::class_<regina::PacketChildren>(m, "PacketChildren")
        .def("__iter__", [](regina::PacketChildren c) {
            return c.begin();
        })
        ;
    regina::python::add_eq_operators(c1);

    auto c2 = pybind11::class_<regina::PacketDescendants>(m, "PacketDescendants")
        .def("__iter__", [](regina::PacketDescendants d) {
            return d.begin();
        })
        ;
    regina::python::add_eq_operators(c2);

    auto c3 = pybind11::class_<regina::ChildIterator>(m, "ChildIterator")
        .def("next", next<ChildIterator>) // for python 2
        .def("__next__", next<ChildIterator>) // for python 3
        ;
    regina::python::add_eq_operators(c3);

    auto c4 = pybind11::class_<regina::SubtreeIterator>(m, "SubtreeIterator")
        .def("__iter__", [](pybind11::object const& it) {
            return it;
        })
        .def("next", next<SubtreeIterator>) // for python 2
        .def("__next__", next<SubtreeIterator>) // for python 3
        ;
    regina::python::add_eq_operators(c4);

    auto c = pybind11::class_<Packet, regina::SafePtr<Packet>>(m, "Packet")
        .def("type", &Packet::type)
        .def("typeName", &Packet::typeName)
        .def("label", &Packet::label)
        .def("humanLabel", &Packet::humanLabel)
        .def("adornedLabel", &Packet::adornedLabel)
        .def("setLabel", &Packet::setLabel)
        .def("fullName", &Packet::fullName)
        .def("hasTag", &Packet::hasTag)
        .def("hasTags", &Packet::hasTags)
        .def("addTag", &Packet::addTag)
        .def("removeTag", &Packet::removeTag)
        .def("removeAllTags", &Packet::removeAllTags)
        .def("tags", &Packet::tags) /* returns python set */
        .def("listen", &Packet::listen)
        .def("isListening", &Packet::isListening)
        .def("unlisten", &Packet::unlisten)
        .def("parent", &Packet::parent)
        .def("firstChild", &Packet::firstChild)
        .def("lastChild", &Packet::lastChild)
        .def("nextSibling", &Packet::nextSibling)
        .def("prevSibling", &Packet::prevSibling)
        .def("root", &Packet::root)
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
        .def("reparent", [](Packet& child, Packet* newParent, bool first) {
            if (child.parent())
                child.reparent(newParent, first);
            else
                throw std::invalid_argument(
                    "reparent() cannot be used on packets with no parent");
        }, pybind11::arg(), pybind11::arg("first") = false)
        .def("transferChildren", &Packet::transferChildren)
        .def("swapWithNextSibling", &Packet::swapWithNextSibling)
        .def("moveUp", &Packet::moveUp,
            pybind11::arg("steps") = 1)
        .def("moveDown", &Packet::moveDown,
            pybind11::arg("steps") = 1)
        .def("moveToFirst", &Packet::moveToFirst)
        .def("moveToLast", &Packet::moveToLast)
        .def("sortChildren", &Packet::sortChildren)
        .def("__iter__", &Packet::begin)
        .def("children", &Packet::children)
        .def("descendants", &Packet::descendants)
        .def("subtree", &Packet::begin)
        .def("nextTreePacket",
            overload_cast<>(&Packet::nextTreePacket))
        .def("nextTreePacket",
            overload_cast<const std::string&>(&Packet::nextTreePacket))
        .def("firstTreePacket",
            overload_cast<const std::string&>(&Packet::firstTreePacket))
        .def("findPacketLabel",
            overload_cast<const std::string&>(&Packet::findPacketLabel))
        .def("dependsOnParent", &Packet::dependsOnParent)
        .def("isPacketEditable", &Packet::isPacketEditable)
        .def("clone", &Packet::clone,
            pybind11::arg("cloneDescendants") = false,
            pybind11::arg("end") = true)
        .def("save",
            overload_cast<const char*, bool>(&Packet::save, pybind11::const_),
            pybind11::arg(), pybind11::arg("compressed") = true)
        .def("writeXMLFile", [](const Packet& p) {
            p.writeXMLFile(std::cout);
        })
        .def("internalID", &Packet::internalID)
        .def("__eq__", [](const Packet* p, PacketShell s) {
            return (s == p);
        }, pybind11::is_operator())
        .def("__ne__", [](const Packet* p, PacketShell s) {
            return (s != p);
        }, pybind11::is_operator())
    ;
    regina::python::add_output(c);
    regina::python::add_eq_operators(c);

    m.def("open", (Packet* (*)(const char*)) &regina::open);

    m.attr("NPacket") = m.attr("Packet");
}

