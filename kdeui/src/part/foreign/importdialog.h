
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file importdialog.h
 *  \brief Provides a dialog through which the user can insert
 *  already-imported data into the packet tree.
 */

#ifndef __IMPORTDIALOG_H
#define __IMPORTDIALOG_H

#include <kdialogbase.h>

class PacketChooser;
class PacketFilter;
class QLineEdit;

namespace regina {
    class NPacket;
};

/**
 * A dialog used to inserted previously imported data into the packet tree.
 *
 * If OK is pressed, the imported data will be inserted into the packet
 * tree.  If the dialog is cancelled however, no further action will be
 * taken (and in particular the imported data will need to be destroyed
 * elsewhere).
 */
class ImportDialog : public KDialogBase {
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
        regina::NPacket* tree;
        regina::NPacket* newTree;

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
        ImportDialog(QWidget* parent, regina::NPacket* importedData,
            regina::NPacket* packetTree, regina::NPacket* defaultParent,
            PacketFilter* useFilter, const QString& dialogTitle);

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
         * KDialogBase overrides.
         */
        virtual void slotOk();
};

#endif
