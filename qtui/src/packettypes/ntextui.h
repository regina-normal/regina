
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

/*! \file ntextui.h
 *  \brief Provides an interface for viewing text packets.
 */

#ifndef __NTEXTUI_H
#define __NTEXTUI_H

#include "../packetui.h"

class QPlainTextEdit;

namespace regina {
    class NPacket;
    class NText;
};

/**
 * A packet interface for viewing text packets.
 */
class NTextUI : public QObject, public PacketUI {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NText* text;

        /**
         * Internal components
         */
        QWidget* ui;
        QPlainTextEdit* document;
        PacketEditIface* editIface;

    public:
        /**
         * Constructor and destructor.
         */
        NTextUI(regina::NText* packet, PacketPane* newEnclosingPane);
        ~NTextUI();

        /**
         * PacketUI overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        PacketEditIface* getEditIface();
        QString getPacketMenuText() const;
        void commit();
        void refresh();
        void setReadWrite(bool readWrite);

    public slots:
        /**
         * Called whenever the text in the interface changes.
         */
        void notifyTextChanged();
};

inline PacketEditIface* NTextUI::getEditIface() {
    return editIface;
}

#endif
