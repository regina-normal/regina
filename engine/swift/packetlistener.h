
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Swift User Interface
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#ifndef __REGINA_SWIFT_PACKETLISTENER_H
#ifndef __DOXYGEN
#define __REGINA_SWIFT_PACKETLISTENER_H
#endif

#include <sstream>
#include "packet/packet.h"
#include "utilities/memstream.h"

namespace regina {

class PacketChangeCallback : public PacketListener {
    private:
        void* context_ = nullptr;

    public:
        void (*callbackPacketToBeChanged)(void*) = nullptr;

        PacketChangeCallback() {
            // The pointer we are monitoring is null, and the callbacks will
            // never be triggered.
        }

        PacketChangeCallback(std::shared_ptr<Packet> packet) {
            if (packet)
                packet->listen(this);
        }

        void enableCallbacks(void* context) {
            context_ = context;
        }

        void disableCallbacks() {
            context_ = nullptr;
        }

        void packetToBeChanged(Packet&) override {
            if (callbackPacketToBeChanged && context_)
                callbackPacketToBeChanged(context_);
        }
};

class PacketChildrenCallback : public PacketListener {
    private:
        void* context_ = nullptr;

    public:
        void (*callbackChildToBeAdded)(void*) = nullptr;
        void (*callbackChildToBeRemoved)(void*) = nullptr;
        void (*callbackChildrenToBeReordered)(void*) = nullptr;
        void (*callback)(void*) = nullptr;

        PacketChildrenCallback() {
            // The pointer we are monitoring is null, and the callbacks will
            // never be triggered.
        }

        PacketChildrenCallback(std::shared_ptr<Packet> packet) {
            if (packet)
                packet->listen(this);
        }

        void enableCallbacks(void* context) {
            context_ = context;
        }

        void disableCallbacks() {
            context_ = nullptr;
        }

        void childToBeAdded(Packet&, Packet&) override {
            if (callbackChildToBeAdded && context_)
                callbackChildToBeAdded(context_);
        }

        void childToBeRemoved(Packet&, Packet&) override {
            if (callbackChildToBeRemoved && context_)
                callbackChildToBeRemoved(context_);
        }

        void childrenToBeReordered(Packet&) override {
            if (callbackChildrenToBeReordered && context_)
                callbackChildrenToBeReordered(context_);
        }
};

}

#endif
