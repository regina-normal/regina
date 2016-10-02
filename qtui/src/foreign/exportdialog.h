
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

/*! \file exportdialog.h
 *  \brief Provides a dialog through which the user can select a packet
 *  or packet subtree to export.
 */

#ifndef __EXPORTDIALOG_H
#define __EXPORTDIALOG_H

#include <QDialog>

class PacketChooser;
class PacketFilter;

namespace regina {
    class Packet;
};

/**
 * A dialog used to select a packet or packet subtree to export.
 */
class ExportDialog : public QDialog {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        PacketChooser* chooser;

        /**
         * Packet tree structure:
         */
        regina::Packet* tree;
        regina::Packet* chosenPacket;

    public:
        /**
         * Dialog constructor.
         *
         * The filter passed is used to restrict the possible selections.
         * It may be 0, in which case any packet or packet subtree
         * will be allowed.
         *
         * This dialog and its components will claim ownership of the
         * given packet filter.
         */
        ExportDialog(QWidget* parent, regina::Packet* packetTree,
            regina::Packet* defaultSelection, PacketFilter* useFilter,
            bool useCodec, const QString& dialogTitle);

        /**
         * Returns whether or not there are any packets at all made
         * available for export.  If not, an appropriate error is
         * displayed to the user.
         *
         * This routine should be called before the dialog is displayed,
         * and the operation aborted if it returns \c false.
         */
        bool validate();

        /**
         * Returns the packet or packet subtree selected by the user.
         */
        regina::Packet* selectedPacket();

    protected slots:
        /**
         * Ok has been clicked.
         */
        virtual void slotOk();

        /**
         * The user wants to learn more about text encodings.
         */
        virtual void slotEncodingInfo();
};

inline regina::Packet* ExportDialog::selectedPacket() {
    return chosenPacket;
}

#endif
