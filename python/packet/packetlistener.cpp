
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Python Interface                                                      *
 *                                                                        *
 *  Copyright (c) 1999-2018, Ben Burton                                   *
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

using regina::Packet;
using regina::PacketListener;

class PyPacketListener : public PacketListener {
    public:
        void packetToBeChanged(Packet* packet) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                packetToBeChanged, packet);
        }
        void packetWasChanged(Packet* packet) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                packetWasChanged, packet);
        }
        void packetToBeRenamed(Packet* packet) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                packetToBeRenamed, packet);
        }
        void packetWasRenamed(Packet* packet) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                packetWasRenamed, packet);
        }
        void packetToBeDestroyed(Packet* packet) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                packetToBeDestroyed, packet);
        }
        void childToBeAdded(Packet* packet, Packet* child) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                childToBeAdded, packet, child);
        }
        void childWasAdded(Packet* packet, Packet* child) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                childWasAdded, packet, child);
        }
        void childToBeRemoved(Packet* packet, Packet* child,
                bool inParentDestructor) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                childToBeRemoved, packet, child, inParentDestructor);
        }
        void childWasRemoved(Packet* packet, Packet* child,
                bool inParentDestructor) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                childWasRemoved, packet, child, inParentDestructor);
        }
        void childrenToBeReordered(Packet* packet) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                childrenToBeReordered, packet);
        }
        void childrenWereReordered(Packet* packet) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                childrenWereReordered, packet);
        }
        void childToBeRenamed(Packet* packet, Packet* child) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                childToBeRenamed, packet, child);
        }
        void childWasRenamed(Packet* packet, Packet* child) override {
            PYBIND11_OVERLOAD(void, PacketListener,
                childWasRenamed, packet, child);
        }
};

void addPacketListener(pybind11::module& m) {
    auto c = pybind11::class_<regina::PacketListener, PyPacketListener>(
            m, "PacketListener")
        .def(pybind11::init<>())
        .def("unregisterFromAllPackets",
            &PacketListener::unregisterFromAllPackets)
        .def("packetToBeChanged", &PacketListener::packetToBeChanged)
        .def("packetWasChanged", &PacketListener::packetWasChanged)
        .def("packetToBeRenamed", &PacketListener::packetToBeRenamed)
        .def("packetWasRenamed", &PacketListener::packetWasRenamed)
        .def("packetToBeDestroyed", &PacketListener::packetToBeDestroyed)
        .def("childToBeAdded", &PacketListener::childToBeAdded)
        .def("childWasAdded", &PacketListener::childWasAdded)
        .def("childToBeRemoved", &PacketListener::childToBeRemoved)
        .def("childWasRemoved", &PacketListener::childWasRemoved)
        .def("childrenToBeReordered", &PacketListener::childrenToBeReordered)
        .def("childrenWereReordered", &PacketListener::childrenWereReordered)
        .def("childToBeRenamed", &PacketListener::childToBeRenamed)
        .def("childWasRenamed", &PacketListener::childWasRenamed)
        ;
    regina::python::add_eq_operators(c);

    m.attr("NPacketListener") = m.attr("PacketListener");
}

