
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

/*! \file linkui.h
 *  \brief Provides an interface for viewing knots and links.
 */

#ifndef __LINKUI_H
#define __LINKUI_H

#include "../packettabui.h"

class LinkCrossingsUI;
class PacketEditIface;
class QLabel;
class QToolBar;

namespace regina {
    class Link;
}

/**
 * A packet interface for viewing knots and links.
 */
class LinkUI : public PacketTabbedUI {
    Q_OBJECT

    private:
        /**
         * Internal components
         */
        LinkCrossingsUI* crossings;

        PacketEditIface* editIface;

    public:
        /**
         * Constructor and destructor.
         */
        LinkUI(regina::PacketOf<regina::Link>* packet,
            PacketPane* newEnclosingPane);
        ~LinkUI() override;

        /**
         * PacketUI overrides.
         */
        PacketEditIface* getEditIface() override;
        const std::vector<QAction*>& getPacketTypeActions() override;
        QString getPacketMenuText() const override;
};

/**
 * A header for the knot/link viewer.
 */
class LinkHeaderUI : public PacketViewerTab {
    private:
        /**
         * Packet details
         */
        regina::PacketOf<regina::Link>* link;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* header;
        QToolBar* bar;

    public:
        /**
         * Constructor.
         */
        LinkHeaderUI(regina::PacketOf<regina::Link>* packet,
            PacketTabbedUI* useParentUI);

        /**
         * Component queries.
         */
        QToolBar* getToolBar();

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        void refresh() override;

        /**
         * Allow other UIs to access the summary information.
         */
        static QString summaryInfo(const regina::Link& link);
};

inline PacketEditIface* LinkUI::getEditIface() {
    return editIface;
}

inline QToolBar* LinkHeaderUI::getToolBar() {
    return bar;
}

#endif
