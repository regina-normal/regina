
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file ncontainerui.h
 *  \brief Provides an interface for viewing container packets.
 */

#ifndef __NCONTAINERUI_H
#define __NCONTAINERUI_H

#include "packet/npacketlistener.h"

#include "../packetui.h"

class QLabel;
class QVBox;

namespace regina {
    class NContainer;
    class NPacket;
};

/**
 * A packet interface for viewing containers.
 *
 * Note that this interface is always read-only.
 */
class NContainerUI : public PacketReadOnlyUI, public regina::NPacketListener {
    private:
        /**
         * Packet details
         */
        regina::NContainer* container;

        /**
         * Internal components
         */
        QVBox* interface;
        QLabel* children;
        QLabel* descendants;

    public:
        /**
         * Constructor.
         */
        NContainerUI(regina::NContainer* packet, PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        QString getPacketMenuText() const;
        void refresh();

        /**
         * NPacketListener overrides.
         */
        void childWasAdded(regina::NPacket* packet, regina::NPacket* child);
        void childWasRemoved(regina::NPacket* packet, regina::NPacket* child,
            bool inParentDestructor);
};

#endif
