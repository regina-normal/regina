
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

/*! \file packetmanager.h
 *  \brief Creates interfaces for packets based on their specific
 *  packet types.
 */

#ifndef __PACKETMANAGER_H
#define __PACKETMANAGER_H

#include <QIcon>
#include "iconcache.h"
#include "packetui.h"

class QPlainTextEdit;

namespace regina {
    class Packet;
};

/**
 * Provides a variety of routines for creating visual interfaces for
 * packets.  Each interface will be tailored according to the specific
 * packet type.
 */
class PacketManager {
    public:
        /**
         * Returns an icon appropriate for the given packet.  This icon
         * may be rendered at various different sizes.
         *
         * If \a allowLock is true and the packet is not editable
         * according to Packet::isPacketEditable(), a small padlock
         * will be overlaid onto the icon.
         */
        static QIcon icon(regina::Packet* packet, bool allowLock = false);

        /**
         * Returns a newly created interface appropriate for viewing or
         * editing the given packet.
         *
         * The interface will be created in read-write or read-only mode
         * as appropriate.
         *
         * Note that the interface may be required to change its
         * read-only or read-write status throughout its lifetime.
         * See PacketUI::setReadWrite() for details.
         */
        static PacketUI* createUI(regina::Packet* packet,
            PacketPane* enclosingPane);

        /**
         * If this packet should be viewed using an external viewer
         * (e.g., PDF packets), this routine returns an appropriate function.
         * If this packet should be viewed using an internal viewer, this
         * routine returns 0.
         */
        static PacketExternalViewer externalViewer(regina::Packet* packet);
};

#endif
