
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

/*! \file exportdialog.h
 *  \brief Provides a dialog through which the user can select a packet
 *  or packet subtree to export.
 */

#ifndef __EXPORTDIALOG_H
#define __EXPORTDIALOG_H

#include <kdialogbase.h>

class PacketChooser;
class PacketFilter;

namespace regina {
    class NPacket;
};

/**
 * A dialog used to select a packet or packet subtree to export.
 */
class ExportDialog : public KDialogBase {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        PacketChooser* chooser;

        /**
         * Packet tree structure:
         */
        regina::NPacket* tree;
        regina::NPacket* chosenPacket;

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
        ExportDialog(QWidget* parent, regina::NPacket* packetTree,
            regina::NPacket* defaultSelection, PacketFilter* useFilter,
            const QString& dialogTitle);

        /**
         * Returns the packet or packet subtree selected by the user.
         */
        regina::NPacket* selectedPacket();

    protected slots:
        /**
         * KDialogBase overrides.
         */
        virtual void slotOk();
};

inline regina::NPacket* ExportDialog::selectedPacket() {
    return chosenPacket;
}

#endif
