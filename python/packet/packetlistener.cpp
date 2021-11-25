
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
#include "packet/packet.h"
#include "../helpers.h"

using pybind11::overload_cast;
using regina::Packet;
using regina::PacketListener;
using regina::PacketShell;

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

void addPacketListener(pybind11::module_& m) {
    auto s = pybind11::class_<PacketShell>(m, "PacketShell")
        .def(pybind11::init<const Packet*>())
        .def(pybind11::init<const PacketShell&>())
        .def("label", &PacketShell::label)
        .def("humanLabel", &PacketShell::humanLabel)
        .def("hasTag", &PacketShell::hasTag)
        .def("hasTags", &PacketShell::hasTags)
        .def("tags", &PacketShell::tags) /* returns python set */
        .def("internalID", &PacketShell::internalID)
        .def("__eq__", [](const PacketShell& s, const Packet* p) {
            return (s == p);
        }, pybind11::is_operator())
        .def("__ne__", [](const PacketShell& s, const Packet* p) {
            return (s != p);
        }, pybind11::is_operator())
        ;
    regina::python::add_eq_operators(s);

    auto l = pybind11::class_<PacketListener, PyPacketListener>(
            m, "PacketListener")
        .def(pybind11::init<>()) // necessary for pure python subclasses
        .def("isListening", &PacketListener::isListening)
        .def("unlisten", &PacketListener::unlisten)
        .def("unregisterFromAllPackets", // deprecated
            &PacketListener::unlisten)
        .def("packetToBeChanged", overload_cast<Packet&>(
            &PacketListener::packetToBeChanged))
        .def("packetWasChanged", overload_cast<Packet&>(
            &PacketListener::packetWasChanged))
        .def("packetToBeRenamed", overload_cast<Packet&>(
            &PacketListener::packetToBeRenamed))
        .def("packetWasRenamed", overload_cast<Packet&>(
            &PacketListener::packetWasRenamed))
        .def("packetBeingDestroyed", &PacketListener::packetBeingDestroyed)
        .def("childToBeAdded", overload_cast<Packet&, Packet&>(
            &PacketListener::childToBeAdded))
        .def("childWasAdded", overload_cast<Packet&, Packet&>(
            &PacketListener::childWasAdded))
        .def("childToBeRemoved", overload_cast<Packet&, Packet&>(
            &PacketListener::childToBeRemoved))
        .def("childWasRemoved", overload_cast<Packet&, Packet&>(
            &PacketListener::childWasRemoved))
        .def("childrenToBeReordered", overload_cast<Packet&>(
            &PacketListener::childrenToBeReordered))
        .def("childrenWereReordered", overload_cast<Packet&>(
            &PacketListener::childrenWereReordered))
        .def("childToBeRenamed", overload_cast<Packet&, Packet&>(
            &PacketListener::childToBeRenamed))
        .def("childWasRenamed", overload_cast<Packet&, Packet&>(
            &PacketListener::childWasRenamed))
        ;
    regina::python::add_eq_operators(l);
}

