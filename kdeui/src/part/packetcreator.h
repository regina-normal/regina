
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

/*! \file packetcreator.h
 *  \brief Provides a basic infrastructure for packet creation.
 */

#ifndef __PACKETCREATOR_H
#define __PACKETCREATOR_H

class QWidget;

namespace regina {
    class NPacket;
};

/**
 * An interface component for creating a packet.  Such interface
 * components are used by NewPacketDialog.
 *
 * This interface component must only contain elements
 * specific to a particular packet type; generic components such as a
 * parent selector and dialog buttons will be provided elsewhere.
 *
 * Different packet types should use different subclasses of
 * PacketCreator for user interaction.
 */
class PacketCreator {
    public:
        /**
         * Virtual destructor.
         */
        virtual ~PacketCreator();

        /**
         * Returns the main interface component for this packet creator.
         * This may be 0 if there are no non-generic interface elements
         * required.
         *
         * The default implementation of this routine returns 0.
         */
        virtual QWidget* getInterface();

        /**
         * Create the packet according to the information entered by the
         * user in the main interface component.
         *
         * This routine must verify that the information entered is
         * valid.  If it is valid, a newly created packet must be
         * returned.  If it is invalid, 0 must be returned and an
         * appropriate error must be displayed (using the argument
         * \a parentWidget as a parent for the message box).
         *
         * This routine may ensure that the newly created packet is
         * placed beneath the given parent packet, though if this is not
         * done then it will be done elsewhere.  It does not need to assign
         * a packet label; this will be also be done elsewhere.
         */
        virtual regina::NPacket* createPacket(regina::NPacket* parentPacket,
            QWidget* parentWidget) = 0;
};

/**
 * A basic interface for creating packets of a particular type.
 *
 * The interface will contain no non-generic interface components, and
 * new packets will be created using the default constructor.
 */
template <class T>
class BasicPacketCreator : public PacketCreator {
    public:
        /**
         * PacketCreator overrides.
         */
        regina::NPacket* createPacket(regina::NPacket*, QWidget*) {
            return new T();
        }
};

inline PacketCreator::~PacketCreator() {
}

inline QWidget* PacketCreator::getInterface() {
    return 0;
}

#endif
