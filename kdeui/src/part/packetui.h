
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

/*! \file packetui.h
 *  \brief Provides a basic infrastructure for packet interface components.
 */

#ifndef __PACKETUI_H
#define __PACKETUI_H

#include <kmainwindow.h>
#include <qvbox.h>

class PacketWindow;
class QLabel;
class ReginaPart;

namespace regina {
    class NPacket;
};

/**
 * A packet header, containing an appropriate icon and text title.
 */
class PacketHeader : public QHBox {
    Q_OBJECT

    private:
        regina::NPacket* packet;
            /**< The packet described by this header. */

        QLabel* icon;
            /**< The icon depicting the packet type. */
        QLabel* title;
            /**< The text title. */

    public:
        /**
         * Constructor.
         */
        PacketHeader(regina::NPacket* pkt, QWidget* parent = 0,
            const char* name = 0);

    public slots:
        /**
         * Updates the text title according to the current packet label.
         */
        void refresh();
};

/**
 * A single item in a Regina packet tree.
 */
/*
class PacketUI {
    public:
        virtual ~PacketUI() {
        }

        NPacket* getPacket();
        QWidget* getInterface();
        QWidget* getPrimaryTextComponent();
        void reflectPacket();

        bool isEditor();
        bool hasChanges();
        void applyChanges();
};
*/

class PacketPane : public QVBox {
    Q_OBJECT

    private:
        ReginaPart* part;
            /**< The KPart responsible for this packet pane. */
        KMainWindow* frame;
            /**< The floating frame containing this packet pane, or 0
                 if this packet pane is currently docked. */
        PacketHeader* header;
            /**< The header with which the packet is identified. */

    public:
        PacketPane(ReginaPart* newPart, regina::NPacket* newPacket,
            QWidget* parent = 0, const char* name = 0);

        bool queryClose();
};

class PacketWindow : public KMainWindow {
    Q_OBJECT

    private:
        PacketPane* heldPane;

    public:
        PacketWindow(PacketPane* newPane, QWidget* parent = 0);

    protected:
        virtual bool queryClose();
};

#endif
