
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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

/*! \file ncontainercreator.h
 *  \brief Provides an interface for creating container packets.
 */

#ifndef __NCONTAINERCREATOR_H
#define __NCONTAINERCREATOR_H

#include "../packetcreator.h"

namespace regina {
    class NContainer;
    class NPacket;
};

/**
 * A packet interface for creating container packets.
 */
class NContainerCreator : public PacketCreator {
    public:
        /**
         * PacketCreator overrides.
         */
        regina::NPacket* createPacket(regina::NPacket* parentPacket,
            QWidget* parentWidget);
};

#endif
