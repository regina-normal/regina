
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

#include <qvbox.h>

class KMainWindow;
class QLabel;
class QPushButton;
class QTextEdit;
class QToolButton;
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
class PacketUI {
    public:
        virtual ~PacketUI();

        virtual regina::NPacket* getPacket() = 0;
        virtual QWidget* getInterface() = 0;
        virtual void refresh() = 0;
        virtual QTextEdit* getTextComponent();

        virtual bool isReadWrite();
        virtual bool isDirty();
        virtual void commit();
};

class DefaultPacketUI : public PacketUI {
    private:
        regina::NPacket* packet;
        QLabel* label;

    public:
        DefaultPacketUI(regina::NPacket* newPacket);
        virtual regina::NPacket* getPacket();
        virtual QWidget* getInterface();
        virtual void refresh();
};

/**
 * A full-featured component through which the user can view or edit a
 * single packet.
 *
 * Packet panes may be either docked within the main ReginaPart widget
 * or may be floating freely in their own frames.
 */
class PacketPane : public QVBox {
    Q_OBJECT

    private:
        ReginaPart* part;
            /**< The KPart managing this packet pane. */
        KMainWindow* frame;
            /**< The floating frame containing this packet pane, or 0
                 if this packet pane is currently docked. */

        /**
         * Internal components
         */
        PacketHeader* header;
        PacketUI* mainUI;

        /**
         * Action components
         */
        QToolButton* dockUndockBtn;
        QPushButton* commitBtn;
        QPushButton* refreshBtn;

    public:
        /**
         * Constructs a new packet pane, managed by the given KPart,
         * that views or edits the given packet.
         */
        PacketPane(ReginaPart* newPart, regina::NPacket* newPacket,
            QWidget* parent = 0, const char* name = 0);

        /**
         * Does this packet pane contain any changes that have not yet
         * been committed?
         */
        bool isDirty();

        /**
         * Are we allowed to close this packet pane?
         */
        bool queryClose();

    public slots:
        /**
         * Queries the packet and refreshes the interface accordingly.
         * Any uncommitted changes will be lost, though the user will be
         * prompted first.
         */
        void refresh();

        /**
         * Commits any changes made in the user interface to the
         * underlying packet.
         */
        void commit();

        /**
         * Closes this packet pane.  All this routine does is delegate
         * the closure operation to whatever component currently owns
         * this packet pane.
         */
        bool close();

        /**
         * Docks this packet pane within the main ReginaPart widget, if
         * it is not already docked.  If another packet pane is
         * already docked and refuses to be closed, this other pane will
         * be moved into its own freely floating window.
         */
        void dockPane();

        /**
         * Floats this packet pane in its own top-level window, if it is
         * not already in such a state.
         */
        void floatPane();
};

inline PacketUI::~PacketUI() {
}

inline QTextEdit* PacketUI::getTextComponent() {
    return 0;
}

inline bool PacketUI::isReadWrite() {
    return false;
}

inline bool PacketUI::isDirty() {
    return false;
}

inline void PacketUI::commit() {
}

#endif
