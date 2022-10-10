
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "../pybind11/iostream.h"
#include "packet/packet.h"
#include "../helpers.h"
#include "../docstrings/packet/packet.h"

using pybind11::overload_cast;
using regina::ChildIterator;
using regina::Packet;
using regina::PacketChildren;
using regina::PacketDescendants;
using regina::PacketShell;
using regina::PacketType;
using regina::SubtreeIterator;

// Docstrings that are generated once but need to be reused across many
// source files:
namespace regina::python::doc::common {
    const char* PacketData_anonID = regina::python::doc::PacketData_::anonID;
    const char* PacketData_packet = regina::python::doc::PacketData_::packet;
    const char* PacketOf = regina::python::doc::PacketOf;
    const char* PacketOf_copy = regina::python::doc::PacketOf_::PacketOf_2;
    const char* make_packet = regina::python::doc::make_packet;
    const char* make_packet_2 = regina::python::doc::make_packet_2;
}

namespace {
    // Support for iterables and iterators:
    template <typename Iterator>
    std::shared_ptr<Packet> next(Iterator& it) {
        if (it)
            return (*it++).shared_from_this();
        else
            throw pybind11::stop_iteration();
    }
}

void addPacket(pybind11::module_& m) {
    auto c1 = pybind11::class_<PacketChildren<false>>(m, "PacketChildren")
        .def("__iter__", [](PacketChildren<false> c) {
            return c.begin();
        })
        ;
    regina::python::add_eq_operators(c1);

    auto c2 = pybind11::class_<PacketDescendants<false>>(m, "PacketDescendants")
        .def("__iter__", [](PacketDescendants<false> d) {
            return d.begin();
        })
        ;
    regina::python::add_eq_operators(c2);

    auto c3 = pybind11::class_<ChildIterator<false>>(m, "ChildIterator")
        .def("__next__", next<ChildIterator<false>>)
        ;
    regina::python::add_eq_operators(c3);

    auto c4 = pybind11::class_<SubtreeIterator<false>>(m, "SubtreeIterator")
        .def("__iter__", [](pybind11::object const& it) {
            return it;
        })
        .def("__next__", next<SubtreeIterator<false>>)
        ;
    regina::python::add_eq_operators(c4);

    auto c = pybind11::class_<Packet, std::shared_ptr<Packet>>(m, "Packet")
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
        .def("samePacket", &Packet::samePacket)
        .def("hasParent", &Packet::hasParent)
        .def("parent", &Packet::parent)
        .def("firstChild", &Packet::firstChild)
        .def("lastChild", &Packet::lastChild)
        .def("nextSibling", &Packet::nextSibling)
        .def("prevSibling", &Packet::prevSibling)
        .def("root", &Packet::root)
        .def("levelsDownTo", &Packet::levelsDownTo)
        .def("levelsUpTo", &Packet::levelsUpTo)
        .def("isAncestorOf", &Packet::isAncestorOf)
        .def("countChildren", &Packet::countChildren)
        .def("countDescendants", &Packet::countDescendants)
        .def("totalTreeSize", &Packet::totalTreeSize)
        .def("prepend", &Packet::prepend)
        .def("insertChildFirst", &Packet::prepend) // deprecated
        .def("append", &Packet::append)
        .def("insertChildLast", &Packet::append) // deprecated
        .def("insert", &Packet::insert)
        .def("insertChildAfter", &Packet::insert) // deprecated
        .def("makeOrphan", &Packet::makeOrphan)
        .def("reparent", &Packet::reparent,
            pybind11::arg(), pybind11::arg("first") = false)
        .def("transferChildren", &Packet::transferChildren)
        .def("swapWithNextSibling", &Packet::swapWithNextSibling)
        .def("moveUp", &Packet::moveUp,
            pybind11::arg("steps") = 1)
        .def("moveDown", &Packet::moveDown,
            pybind11::arg("steps") = 1)
        .def("moveToFirst", &Packet::moveToFirst)
        .def("moveToLast", &Packet::moveToLast)
        .def("sortChildren", &Packet::sortChildren)
        .def("__iter__", overload_cast<>(&Packet::begin)) // non-const
        .def("children", overload_cast<>(&Packet::children)) // non-const
        .def("descendants", overload_cast<>(&Packet::descendants)) // non-const
        .def("subtree", overload_cast<>(&Packet::begin)) // non-const
        .def("nextTreePacket",
            overload_cast<>(&Packet::nextTreePacket))
        .def("nextTreePacket",
            overload_cast<PacketType>(&Packet::nextTreePacket))
        .def("firstTreePacket",
            overload_cast<PacketType>(&Packet::firstTreePacket))
        .def("findPacketLabel",
            overload_cast<const std::string&>(&Packet::findPacketLabel))
        .def("cloneAsSibling", &Packet::cloneAsSibling,
            pybind11::arg("cloneDescendants") = false,
            pybind11::arg("end") = true)
        .def("save",
            overload_cast<const char*, bool, regina::FileFormat>(
                &Packet::save, pybind11::const_),
            pybind11::arg(),
            pybind11::arg("compressed") = true,
            pybind11::arg("format") = regina::REGINA_CURRENT_FILE_FORMAT)
        .def("writeXMLFile", [](const Packet& p, pybind11::object file,
                regina::FileFormat format) {
            pybind11::scoped_ostream_redirect stream(std::cout, file);
            p.writeXMLFile(std::cout, format);
        }, pybind11::arg(),
            pybind11::arg("format") = regina::REGINA_CURRENT_FILE_FORMAT)
        .def("internalID", &Packet::internalID)
        .def("__eq__", [](const Packet* p, PacketShell s) {
            return (s == p);
        }, pybind11::is_operator())
        .def("__ne__", [](const Packet* p, PacketShell s) {
            return (s != p);
        }, pybind11::is_operator())
    ;
    // Leave the output routines for subclasses to wrap, since __repr__
    // will include the (derived) class name.
    // Also leave the equality operators for subclasses to wrap, since
    // each subclass of Packet provides its own custom == and != operators.
    regina::python::no_eq_operators(c);

    m.def("open", static_cast<std::shared_ptr<Packet>(&)(const char*)>(
        regina::open));
}

