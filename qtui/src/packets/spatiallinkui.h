
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

/*! \file spatiallinkui.h
 *  \brief Provides an interface for viewing spatial links.
 */

#ifndef __SPATIALLINKUI_H
#define __SPATIALLINKUI_H

#include "link/spatiallink.h"
#include "../packetui.h"

class QLabel;
class QPushButton;
class QTreeView;

namespace regina {
    class Packet;
    class SpatialLink;
};

/**
 * A packet interface for viewing spatial links.
 */
class SpatialLinkUI : public QObject, public PacketUI {
    Q_OBJECT

    public:
        /**
         * The maximum number of nodes that we are willing to render.
         */
        static constexpr size_t MAX_NODES = 500;

    private:
        /**
         * Packet details
         */
        regina::PacketOf<regina::SpatialLink>* link_;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* linkStats;
        QLabel* renderingStats;

        /**
         * Spatial link actions
         */
        QAction* actThinner;
        QAction* actThicker;
        QAction* actRefine;
        std::vector<QAction*> actionList;

    public:
        /**
         * Constructor and destructor.
         */
        SpatialLinkUI(regina::PacketOf<regina::SpatialLink>* packet,
                PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        const std::vector<QAction*>& getPacketTypeActions() override;
        QString getPacketMenuText() const override;
        void refresh() override;

    public slots:
        /**
         * Spatial link actions.
         */
        void makeThinner();
        void makeThicker();
        void refine();

        /**
         * Open a python console to work with this spatial link.
         */
        void pythonConsole();
};

#endif
