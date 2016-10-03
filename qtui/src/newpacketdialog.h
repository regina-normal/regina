
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

/*! \file newpacketdialog.h
 *  \brief Provides a dialog through which the user can create a new
 *  packet.
 */

#ifndef __NEWPACKETDIALOG_H
#define __NEWPACKETDIALOG_H

#include <QDialog>

class PacketChooser;
class PacketCreator;
class PacketFilter;
class QLineEdit;

namespace regina {
    class Packet;
};

/**
 * A dialog used to create a new packet.
 *
 * New packets of all types should be created using this dialog.
 * The real work is done by the PacketCreator class, which is
 * overridden for each different packet type.
 */
class NewPacketDialog : public QDialog {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        PacketCreator* creator;
        PacketChooser* chooser;

        /**
         * Packet tree structure:
         */
        regina::Packet* tree;
        regina::Packet* newPacket;

    public:
        /**
         * Constructor and destructor.
         *
         * The filter passed is used to restrict the possible parents of
         * the new packet.  It may be 0, in which case any parent will
         * be allowed.
         *
         * This dialog and its components will claim ownership of the
         * given PacketCreator and PacketFilter.
         */
        NewPacketDialog(QWidget* parent, PacketCreator* newCreator,
            regina::Packet* packetTree, regina::Packet* defaultParent,
            PacketFilter* useFilter, const QString& dialogTitle);
        virtual ~NewPacketDialog();

        /**
         * Returns whether or not there are any possible parent packets at
         * all.  If not, an appropriate error is displayed to the user.
         *
         * This routine should be called before the dialog is displayed,
         * and the operation aborted if it returns \c false.
         */
        bool validate();

        /**
         * Returns the packet that was created by this dialog, if any.
         */
        regina::Packet* createdPacket();

    protected slots:
        /**
         * Ok has been clicked.
         */
        virtual void slotOk();
};

inline regina::Packet* NewPacketDialog::createdPacket() {
    return newPacket;
}

#endif
