
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/iostream.h>
#include "packet/packet.h"
#include "../helpers.h"
#include "../docstrings/packet/packet.h"

using pybind11::overload_cast;
using regina::ChildIterator;
using regina::Packet;
using regina::PacketChildren;
using regina::PacketDescendants;
using regina::PacketListener;
using regina::PacketShell;
using regina::PacketType;
using regina::SubtreeIterator;

// Docstrings that are generated once but need to be reused across many
// source files:
namespace regina::python::doc::common {
    const char* Packet_append = regina::python::doc::Packet_::append;
    const char* PacketData_anonID = regina::python::doc::PacketData_::anonID;
    const char* PacketData_packet = regina::python::doc::PacketData_::packet;
    const char* PacketOf = regina::python::doc::PacketOf;
    const char* PacketOf_copy = regina::python::doc::PacketOf_::__init;
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

class PyPacketListener : public PacketListener {
    public:
        void packetToBeChanged(Packet& packet) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                packetToBeChanged, packet);
        }
        void packetWasChanged(Packet& packet) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                packetWasChanged, packet);
        }
        void packetToBeRenamed(Packet& packet) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                packetToBeRenamed, packet);
        }
        void packetWasRenamed(Packet& packet) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                packetWasRenamed, packet);
        }
        void packetBeingDestroyed(PacketShell packet) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                packetBeingDestroyed, packet);
        }
        void childToBeAdded(Packet& packet, Packet& child) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                childToBeAdded, packet, child);
        }
        void childWasAdded(Packet& packet, Packet& child) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                childWasAdded, packet, child);
        }
        void childToBeRemoved(Packet& packet, Packet& child) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                childToBeRemoved, packet, child);
        }
        void childWasRemoved(Packet& packet, Packet& child) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                childWasRemoved, packet, child);
        }
        void childrenToBeReordered(Packet& packet) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                childrenToBeReordered, packet);
        }
        void childrenWereReordered(Packet& packet) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                childrenWereReordered, packet);
        }
        void childToBeRenamed(Packet& packet, Packet& child) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                childToBeRenamed, packet, child);
        }
        void childWasRenamed(Packet& packet, Packet& child) override {
            PYBIND11_OVERRIDE(void, PacketListener,
                childWasRenamed, packet, child);
        }
};

void addPacket(pybind11::module_& m) {
    RDOC_SCOPE_BEGIN(PacketChildren)

    auto c1 = pybind11::class_<PacketChildren<false>>(m, "PacketChildren",
            rdoc_scope)
        .def("__iter__", [](const PacketChildren<false>& c) {
            return c.begin();
        }, rdoc::__iter__)
        ;
    regina::python::add_eq_operators(c1, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_SWITCH(PacketDescendants)

    auto c2 = pybind11::class_<PacketDescendants<false>>(m, "PacketDescendants",
            rdoc_scope)
        .def("__iter__", [](const PacketDescendants<false>& d) {
            return d.begin();
        }, rdoc::__iter__)
        ;
    regina::python::add_eq_operators(c2, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_SWITCH(ChildIterator)

    auto c3 = pybind11::class_<ChildIterator<false>>(m, "ChildIterator",
            rdoc_scope)
        .def("__next__", next<ChildIterator<false>>, rdoc::__next__)
        ;
    regina::python::add_eq_operators(c3, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_SWITCH(SubtreeIterator)

    auto c4 = pybind11::class_<SubtreeIterator<false>>(m, "SubtreeIterator",
            rdoc_scope)
        .def("__iter__", [](pybind11::object const& it) {
            return it;
        }, rdoc::__iter__)
        .def("__next__", next<SubtreeIterator<false>>, rdoc::__next__)
        ;
    regina::python::add_eq_operators(c4, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_SWITCH(Packet)

    auto c = pybind11::class_<Packet, std::shared_ptr<Packet>>(m, "Packet",
            rdoc_scope)
        .def("type", &Packet::type, rdoc::type)
        .def("typeName", &Packet::typeName, rdoc::typeName)
        .def("label", &Packet::label, rdoc::label)
        .def("humanLabel", &Packet::humanLabel, rdoc::humanLabel)
        .def("adornedLabel", &Packet::adornedLabel, rdoc::adornedLabel)
        .def("setLabel", &Packet::setLabel, rdoc::setLabel)
        .def("fullName", &Packet::fullName, rdoc::fullName)
        .def("hasTag", &Packet::hasTag, rdoc::hasTag)
        .def("hasTags", &Packet::hasTags, rdoc::hasTags)
        .def("addTag", &Packet::addTag, rdoc::addTag)
        .def("removeTag", &Packet::removeTag, rdoc::removeTag)
        .def("removeAllTags", &Packet::removeAllTags, rdoc::removeAllTags)
        .def("tags", &Packet::tags, rdoc::tags) /* returns python set */
        .def("listen", &Packet::listen, rdoc::listen)
        .def("isListening", &Packet::isListening, rdoc::isListening)
        .def("unlisten", &Packet::unlisten, rdoc::unlisten)
        .def("samePacket", &Packet::samePacket, rdoc::samePacket)
        .def("hasParent", &Packet::hasParent, rdoc::hasParent)
        .def("parent", &Packet::parent, rdoc::parent)
        .def("firstChild", &Packet::firstChild, rdoc::firstChild)
        .def("lastChild", &Packet::lastChild, rdoc::lastChild)
        .def("nextSibling", &Packet::nextSibling, rdoc::nextSibling)
        .def("prevSibling", &Packet::prevSibling, rdoc::prevSibling)
        .def("root", &Packet::root, rdoc::root)
        .def("levelsDownTo", &Packet::levelsDownTo, rdoc::levelsDownTo)
        .def("levelsUpTo", &Packet::levelsUpTo, rdoc::levelsUpTo)
        .def("isAncestorOf", &Packet::isAncestorOf, rdoc::isAncestorOf)
        .def("countChildren", &Packet::countChildren, rdoc::countChildren)
        .def("countDescendants", &Packet::countDescendants,
            rdoc::countDescendants)
        .def("totalTreeSize", &Packet::totalTreeSize, rdoc::totalTreeSize)
        .def("prepend", &Packet::prepend, rdoc::prepend)
        .def("insertChildFirst", &Packet::prepend, // deprecated
            rdoc::insertChildFirst)
        .def("append", &Packet::append, rdoc::append)
        .def("insertChildLast", &Packet::append, // deprecated
            rdoc::insertChildLast)
        .def("insert", &Packet::insert, rdoc::insert)
        .def("insertChildAfter", &Packet::insert, // deprecated
            rdoc::insertChildAfter)
        .def("makeOrphan", &Packet::makeOrphan, rdoc::makeOrphan)
        .def("reparent", &Packet::reparent,
            pybind11::arg(), pybind11::arg("first") = false, rdoc::reparent)
        .def("transferChildren", &Packet::transferChildren,
            rdoc::transferChildren)
        .def("swapWithNextSibling", &Packet::swapWithNextSibling,
            rdoc::swapWithNextSibling)
        .def("moveUp", &Packet::moveUp,
            pybind11::arg("steps") = 1, rdoc::moveUp)
        .def("moveDown", &Packet::moveDown,
            pybind11::arg("steps") = 1, rdoc::moveDown)
        .def("moveToFirst", &Packet::moveToFirst, rdoc::moveToFirst)
        .def("moveToLast", &Packet::moveToLast, rdoc::moveToLast)
        .def("sortChildren", &Packet::sortChildren, rdoc::sortChildren)
        .def("__iter__", overload_cast<>(&Packet::begin), // non-const
            rdoc::__iter__)
        .def("children", overload_cast<>(&Packet::children), // non-const
            rdoc::children)
        .def("descendants", overload_cast<>(&Packet::descendants), // non-const
            rdoc::descendants)
        .def("subtree", overload_cast<>(&Packet::begin), // non-const
            rdoc::__iter__)
        .def("nextTreePacket",
            overload_cast<>(&Packet::nextTreePacket),
            rdoc::nextTreePacket)
        .def("nextTreePacket",
            overload_cast<PacketType>(&Packet::nextTreePacket),
            rdoc::nextTreePacket_3)
        .def("firstTreePacket",
            overload_cast<PacketType>(&Packet::firstTreePacket),
            rdoc::firstTreePacket)
        .def("findPacketLabel",
            overload_cast<const std::string&>(&Packet::findPacketLabel),
            rdoc::findPacketLabel)
        .def("cloneAsSibling", &Packet::cloneAsSibling,
            pybind11::arg("cloneDescendants") = false,
            pybind11::arg("end") = true,
            rdoc::cloneAsSibling)
        .def("save",
            overload_cast<const char*, bool, regina::FileFormat>(
                &Packet::save, pybind11::const_),
            pybind11::arg(),
            pybind11::arg("compressed") = true,
            pybind11::arg("format") = regina::REGINA_CURRENT_FILE_FORMAT,
            rdoc::save)
        .def("writeXMLFile", [](const Packet& p, pybind11::object file,
                regina::FileFormat format) {
            pybind11::scoped_ostream_redirect stream(std::cout, file);
            p.writeXMLFile(std::cout, format);
        }, pybind11::arg(),
            pybind11::arg("format") = regina::REGINA_CURRENT_FILE_FORMAT,
            rdoc::writeXMLFile)
        .def("internalID", &Packet::internalID, rdoc::internalID)
        .def("__eq__", [](const Packet* p, PacketShell s) {
            return (s == p);
        }, pybind11::is_operator(), rdoc_global::__eq)
        .def("__ne__", [](const Packet* p, PacketShell s) {
            return (s != p);
        }, pybind11::is_operator(), rdoc_global::__ne)
    ;
    // Leave the output routines for subclasses to wrap, since __repr__
    // will include the (derived) class name.
    // Also leave the equality operators for subclasses to wrap, since
    // each subclass of Packet provides its own custom == and != operators.
    regina::python::no_eq_abstract(c);

    RDOC_SCOPE_SWITCH_MAIN

    m.def("open", static_cast<std::shared_ptr<Packet>(&)(const char*)>(
        regina::open), rdoc::open);

    RDOC_SCOPE_SWITCH(PacketShell)

    auto s = pybind11::class_<PacketShell>(m, "PacketShell", rdoc_scope)
        .def(pybind11::init<const Packet*>(), rdoc::__init)
        .def(pybind11::init<const PacketShell&>(), rdoc::__copy)
        .def("label", &PacketShell::label, rdoc::label)
        .def("humanLabel", &PacketShell::humanLabel, rdoc::humanLabel)
        .def("hasTag", &PacketShell::hasTag, rdoc::hasTag)
        .def("hasTags", &PacketShell::hasTags, rdoc::hasTags)
        .def("tags", &PacketShell::tags, rdoc::tags) /* returns python set */
        .def("internalID", &PacketShell::internalID, rdoc::internalID)
        .def("__eq__", [](const PacketShell& s, const Packet* p) {
            return (s == p);
        }, pybind11::is_operator(), rdoc::__eq_2)
        .def("__ne__", [](const PacketShell& s, const Packet* p) {
            return (s != p);
        }, pybind11::is_operator(), rdoc::__ne_2)
        ;
    regina::python::add_output_custom(s, [](const PacketShell& p,
            std::ostream& s) {
        // Avoid calling any virtual functions, or functions specific to
        // the packet type - the subclass destructor may well have already
        // run by this point.
        s << "Shell for packet [" << p.internalID() << ']';
        if (! p.label().empty())
            s << ": " << p.label();
    });
    regina::python::add_eq_operators(s, rdoc::__eq, rdoc::__ne);

    RDOC_SCOPE_SWITCH(PacketListener)

    auto l = pybind11::class_<PacketListener, PyPacketListener>(
            m, "PacketListener", rdoc_scope)
        .def(pybind11::init<>(), // necessary for pure python subclasses
            "Default base class constructor that does nothing.")
        .def("isListening", &PacketListener::isListening, rdoc::isListening)
        .def("unlisten", &PacketListener::unlisten, rdoc::unlisten)
        .def("packetToBeChanged", overload_cast<Packet&>(
            &PacketListener::packetToBeChanged), rdoc::packetToBeChanged)
        .def("packetWasChanged", overload_cast<Packet&>(
            &PacketListener::packetWasChanged), rdoc::packetWasChanged)
        .def("packetToBeRenamed", overload_cast<Packet&>(
            &PacketListener::packetToBeRenamed), rdoc::packetToBeRenamed)
        .def("packetWasRenamed", overload_cast<Packet&>(
            &PacketListener::packetWasRenamed), rdoc::packetWasRenamed)
        .def("packetBeingDestroyed", &PacketListener::packetBeingDestroyed,
            rdoc::packetBeingDestroyed)
        .def("childToBeAdded", overload_cast<Packet&, Packet&>(
            &PacketListener::childToBeAdded), rdoc::childToBeAdded)
        .def("childWasAdded", overload_cast<Packet&, Packet&>(
            &PacketListener::childWasAdded), rdoc::childWasAdded)
        .def("childToBeRemoved", overload_cast<Packet&, Packet&>(
            &PacketListener::childToBeRemoved), rdoc::childToBeRemoved)
        .def("childWasRemoved", overload_cast<Packet&, Packet&>(
            &PacketListener::childWasRemoved), rdoc::childWasRemoved)
        .def("childrenToBeReordered", overload_cast<Packet&>(
            &PacketListener::childrenToBeReordered),
                rdoc::childrenToBeReordered)
        .def("childrenWereReordered", overload_cast<Packet&>(
            &PacketListener::childrenWereReordered),
                rdoc::childrenWereReordered)
        .def("childToBeRenamed", overload_cast<Packet&, Packet&>(
            &PacketListener::childToBeRenamed), rdoc::childToBeRenamed)
        .def("childWasRenamed", overload_cast<Packet&, Packet&>(
            &PacketListener::childWasRenamed), rdoc::childWasRenamed)
        ;
    regina::python::add_eq_operators(l);

    RDOC_SCOPE_END
}

