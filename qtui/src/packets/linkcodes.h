
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file linkcodes.h
 *  \brief Provides a tab for viewing text-based codes for links.
 */

#ifndef __LINKCODES_H
#define __LINKCODES_H

#include "packettabui.h"

class QTextEdit;
class QComboBox;

namespace regina {
    class Link;
    class Packet;
};

/**
 * A packet viewer tab for viewing text-based codes for a link.
 */
class LinkCodesUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * The link itself
         */
        regina::Link* link;

        /**
         * Internal components
         */
        QWidget* ui;
        QComboBox* type;
        QTextEdit* code;
        PacketEditIface* editIface;

    public:
        /**
         * Constructor and destructor.
         */
        LinkCodesUI(regina::Link* useLink, PacketTabbedUI* useParentUI);
        ~LinkCodesUI();

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        void refresh();
        PacketEditIface* getEditIface();

    public slots:
        /**
         * Change which code is displayed.
         */
        void typeChanged(int index);

        /**
         * Notify that preferences have changed.
         */
        void updatePreferences();
};

inline PacketEditIface* LinkCodesUI::getEditIface() {
    return editIface;
}

#endif
