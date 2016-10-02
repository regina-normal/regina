
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

/*! \file packetcreator.h
 *  \brief Provides a basic infrastructure for packet creation.
 */

#ifndef __PACKETCREATOR_H
#define __PACKETCREATOR_H

#include "packet/packet.h"
#include <qstring.h>

class QWidget;

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
         * If the parent packet plays a special role for this packet
         * type, you may wish to customise the corresponding prompt
         * and associated "what's this" text in the new packet dialog.
         * If these routines return null strings, the default
         * "Create beneath:" prompt and and corresponding help text will be
         * used.
         */
        virtual QString parentPrompt();
        virtual QString parentWhatsThis();

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
        virtual regina::Packet* createPacket(regina::Packet* parentPacket,
            QWidget* parentWidget) = 0;

        /**
         * Show a message box to the user explaining why packets of this
         * type cannot (yet) be created.  This will be shown if the user
         * tries to create a new packet but no suitable parents exist.
         *
         * For packets that have constraints on their parents, it is
         * highly recommended to customise this routine; otherwise a
         * generic (and less illuminating) message will be shown.
         */
         virtual void explainNoParents();
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
        regina::Packet* createPacket(regina::Packet*, QWidget*) {
            regina::Packet* ans = new T();
            ans->setLabel(ans->typeName());
            return ans;
        }
};

inline PacketCreator::~PacketCreator() {
}

inline QWidget* PacketCreator::getInterface() {
    return 0;
}

inline QString PacketCreator::parentPrompt() {
    return QString();
}

inline QString PacketCreator::parentWhatsThis() {
    return QString();
}

#endif
