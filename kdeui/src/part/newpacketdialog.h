
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

/*! \file newpacketdialog.h
 *  \brief Provides a dialog through which the user can create a new
 *  packet.
 */

#ifndef __NEWPACKETDIALOG_H
#define __NEWPACKETDIALOG_H

#include <kdialogbase.h>

class PacketChooser;
class PacketCreator;
class PacketFilter;
class QLineEdit;

namespace regina {
    class NPacket;
};

/**
 * A dialog used to create a new packet.
 *
 * New packets of all types should be created using this dialog.
 * The real work is done by the PacketCreator class, which is
 * overridden for each different packet type.
 */
class NewPacketDialog : public KDialogBase {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        PacketCreator* creator;
        PacketChooser* chooser;
        QLineEdit* label;

        /**
         * Packet tree structure:
         */
        regina::NPacket* tree;
        regina::NPacket* newPacket;

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
            regina::NPacket* packetTree, regina::NPacket* defaultParent,
            PacketFilter* useFilter, const QString& dialogTitle,
            const QString& suggestedLabel);
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
        regina::NPacket* createdPacket();

    protected slots:
        /**
         * KDialogBase overrides.
         */
        virtual void slotOk();
};

inline regina::NPacket* NewPacketDialog::createdPacket() {
    return newPacket;
}

#endif
