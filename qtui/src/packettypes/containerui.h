
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

/*! \file containerui.h
 *  \brief Provides an interface for viewing container packets.
 */

#ifndef __CONTAINERUI_H
#define __CONTAINERUI_H

#include "packet/packetlistener.h"

#include "../packetui.h"

class QLabel;

namespace regina {
    class Container;
    class Packet;
};

/**
 * A packet interface for viewing containers.
 *
 * Note that this interface is always read-only.
 */
class ContainerUI : public PacketReadOnlyUI, public regina::PacketListener {
    private:
        /**
         * Packet details
         */
        regina::Container* container;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* children;
        QLabel* descendants;

    public:
        /**
         * Constructor.
         */
        ContainerUI(regina::Container* packet, PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        QString getPacketMenuText() const;
        void refresh();

        /**
         * PacketListener overrides.
         */
        void childWasAdded(regina::Packet* packet, regina::Packet* child);
        void childWasRemoved(regina::Packet* packet, regina::Packet* child,
            bool inParentDestructor);
};

#endif
