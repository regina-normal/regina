
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

/*! \file hyperui.h
 *  \brief Provides an interface for viewing normal hypersurface lists.
 */

#ifndef __HYPERUI_H
#define __HYPERUI_H

#include "packettabui.h"
#include "reginamain.h"

class HyperCompatibilityUI;
class HyperCoordinateUI;
class QLabel;

namespace regina {
    class NormalHypersurfaces;
};

/**
 * A packet interface for viewing normal surface lists.
 */
class HyperUI : public PacketTabbedUI {
    Q_OBJECT

    private:
        /**
         * Internal components
         */
        HyperCoordinateUI* coords;
        HyperCompatibilityUI* compat;

    public:
        /**
         * Constructor.
         */
        HyperUI(regina::PacketOf<regina::NormalHypersurfaces>* packet,
            PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        const std::vector<QAction*>& getPacketTypeActions() override;
        QString getPacketMenuText() const override;
};

/**
 * A header for the normal surface list viewer.
 */
class HyperHeaderUI : public QObject, public PacketViewerTab,
        public regina::PacketListener {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::PacketOf<regina::NormalHypersurfaces>* surfaces;
        bool triDestroyed { false };

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* header;

    public:
        /**
         * Constructor.
         */
        HyperHeaderUI(regina::PacketOf<regina::NormalHypersurfaces>* packet,
                PacketTabbedUI* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        void refresh() override;

        /**
         * PacketListener overrides.
         */
        void packetWasRenamed(regina::Packet& packet) override;
        void packetWasChanged(regina::Packet&) override;
        void packetBeingDestroyed(regina::PacketShell) override;

    private slots:
        /**
         * View the underlying triangulation.
         */
        void viewTriangulation();
};

#endif
