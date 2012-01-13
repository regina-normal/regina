
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file packetmanager.h
 *  \brief Creates interfaces for packets based on their specific
 *  packet types.
 */

#ifndef __PACKETMANAGER_H
#define __PACKETMANAGER_H

#include <QImage>
#include <QPixmap>

class PacketPane;
class PacketUI;

class QPlainTextEdit;

namespace regina {
    class NPacket;
};

/**
 * Provides a variety of routines for creating visual interfaces for
 * packets.  Each interface will be tailored according to the specific
 * packet type.
 */
class PacketManager {
    private:
        /**
         * Support for overlaying a read-only lock onto an icon.
         */
        static bool lockInitialised;
        static QImage lockSmall;
        static QImage lockBar;

    public:
        /**
         * Returns a small (16x16) icon appropriate for the given packet.
         *
         * If \a allowLock is true and the packet is not editable
         * according to NPacket::isPacketEditable(), a small padlock
         * will be overlaid onto the icon.
         */
        static QPixmap iconSmall(regina::NPacket* packet,
            bool allowLock = false);

        /**
         * Returns a toolbar (22x22) icon appropriate for the given packet.
         *
         * If \a allowLock is true and the packet is not editable
         * according to NPacket::isPacketEditable(), a small padlock
         * will be overlaid onto the icon.
         */
        static QPixmap iconBar(regina::NPacket* packet,
            bool allowLock = false);

        /**
         * Returns a newly created interface appropriate for viewing or
         * editing the given packet.
         *
         * The interface will be created in read-write or read-only mode
         * according to the read-write status of the enclosing KPart as
         * well as the return value of NPacket::isPacketEditable().
         *
         * Note that the interface may be required to change its
         * read-only or read-write status throughout its lifetime.
         * See PacketUI::setReadWrite() for details.
         */
        static PacketUI* createUI(regina::NPacket* packet,
            PacketPane* enclosingPane);

    private:
        /**
         * More icon support.
         */
        static QString iconName(regina::NPacket* packet);
};

#endif
