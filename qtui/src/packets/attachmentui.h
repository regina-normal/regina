
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file attachmentui.h
 *  \brief Provides an interface for viewing attachment packets.
 */

#ifndef __ATTACHMENTUI_H
#define __ATTACHMENTUI_H

#include "packetui.h"

class QPushButton;

namespace regina {
    class Attachment;
}

/**
 * A packet interface for viewing or saving attachments.
 */
class AttachmentUI : public QObject, public PacketReadOnlyUI {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Attachment* attachment;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* filename;
        QLabel* size;
        QPushButton* viewButton;
        QPushButton* saveButton;

    public:
        /**
         * Constructor.
         */
        AttachmentUI(regina::Attachment* packet, PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        QString getPacketMenuText() const override;
        void refresh() override;

    public slots:
        /**
         * Access the attachment data.
         */
        void view();
        void save();
};

#endif
