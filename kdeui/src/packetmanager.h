
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

/*! \file packetmanager.h
 *  \brief Creates interfaces for packets based on their specific
 *  packet types.
 */

#ifndef __PACKETMANAGER_H
#define __PACKETMANAGER_H

#include <qpixmap.h>

namespace regina {
    class NPacket;
};

/**
 * Provides a variety of routines for creating visual interfaces for
 * packets.  Each interface will be tailored according to the specific
 * packet type.
 */
class PacketManager {
    public:
        /**
         * Returns a small (16x16) icon appropriate for the given packet.
         */
        static QPixmap iconSmall(regina::NPacket* packet);
};

#endif
