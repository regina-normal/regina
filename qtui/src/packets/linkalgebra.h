
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file linkalgebra.h
 *  \brief Provides an algebra viewer for links.
 */

#ifndef __LINKALGEBRA_H
#define __LINKALGEBRA_H

#include "packettabui.h"

class GroupWidget;
class QLabel;
class QStackedWidget;

namespace regina {
    class Link;
    class Packet;
};

/**
 * A packet viewer tab for viewing all of the groups associated with a link.
 */
class LinkAlgebraUI : public PacketTabbedViewerTab {
    private:
        regina::PacketOf<regina::Link>* link;
        bool usesKnotLabels;

    public:
        /**
         * Constructor.
         */
        LinkAlgebraUI(regina::PacketOf<regina::Link>* packet,
            PacketTabbedUI* parentUI);

        /**
         * PacketTabbedViewerTab overrides.
         */
        void refresh() override;
};

/**
 * A packet viewer tab for viewing a particular class of link groups.
 */
class LinkGroupUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::PacketOf<regina::Link>* link;
        bool extended;

        /**
         * Internal components
         */
        QWidget* ui;
        QStackedWidget* pages;
        GroupWidget* group;
        GroupWidget* groupAbove;
        GroupWidget* groupBelow;

    public:
        /**
         * Constructor.
         */
        LinkGroupUI(regina::PacketOf<regina::Link>* packet,
            bool extended, PacketTabbedViewerTab* parentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        void refresh() override;

    public slots:
        /**
         * Note that preferences have changed.
         */
        void updatePreferences();
};

#endif
