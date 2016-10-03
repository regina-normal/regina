
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

/*! \file importdialog.h
 *  \brief Provides a dialog through which the user can insert
 *  already-imported data into the packet tree.
 */

#ifndef __IMPORTDIALOG_H
#define __IMPORTDIALOG_H

#include <QDialog>

class PacketChooser;
class PacketFilter;
class QLineEdit;

namespace regina {
    class Packet;
};

/**
 * A dialog used to inserted previously imported data into the packet tree.
 *
 * If OK is pressed, the imported data will be inserted into the packet
 * tree.  If the dialog is cancelled however, no further action will be
 * taken (and in particular the imported data will need to be destroyed
 * elsewhere).
 */
class ImportDialog : public QDialog {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        PacketChooser* chooser;
        QLineEdit* label;

        /**
         * Packet tree structure:
         */
        regina::Packet* tree;
        regina::Packet* newTree;

    public:
        /**
         * Dialog constructor.
         *
         * The filter passed is used to restrict the possible parents of
         * the imported data.  It may be 0, in which case any parent will
         * be allowed.
         *
         * This dialog and its components will claim ownership of the
         * given packet filter.
         */
        ImportDialog(QWidget* parent, regina::Packet* importedData,
            regina::Packet* packetTree, regina::Packet* defaultParent,
            PacketFilter* useFilter, bool useCodec, const QString& dialogTitle);

        /**
         * Returns whether or not there are any possible parent packets
         * at all.  If not, an appropriate error is displayed to the user.
         *
         * This routine should be called before the dialog is displayed,
         * and the operation aborted if it returns \c false.
         */
        bool validate();

    protected slots:
        /**
         * Ok has been clicked.
         */
        virtual void slotOk();

        /**
         * The user wishes to learn more about text encodings.
         */
        virtual void slotEncodingInfo();
};

#endif
