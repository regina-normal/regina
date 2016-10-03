
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

/*! \file textui.h
 *  \brief Provides an interface for viewing text packets.
 */

#ifndef __TEXTUI_H
#define __TEXTUI_H

#include "../packetui.h"

template <class PacketType, class Sanitise>
class DocWidget;

class DocWidgetNoSanitise;

namespace regina {
    class Packet;
    class Text;
};

/**
 * A packet interface for viewing text packets.
 */
class TextUI : public QObject, public PacketUI {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Text* text;

        /**
         * Internal components
         */
        QWidget* ui;
        DocWidget<regina::Text, DocWidgetNoSanitise>* editWidget;
        PacketEditIface* editIface;

    public:
        /**
         * Constructor and destructor.
         */
        TextUI(regina::Text* packet, PacketPane* newEnclosingPane);
        ~TextUI();

        /**
         * PacketUI overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        PacketEditIface* getEditIface();
        QString getPacketMenuText() const;
        void refresh();
        void setReadWrite(bool readWrite);
};

inline PacketEditIface* TextUI::getEditIface() {
    return editIface;
}

#endif
