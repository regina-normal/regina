
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

/*! \file surfacessummaryui.h
 *  \brief Provides a tab that summarises all normal surfaces in a list.
 */

#ifndef __SURFACESSUMMARYUI_H
#define __SURFACESSUMMARYUI_H

#include "../packettabui.h"

namespace regina {
    class Packet;
    class NormalSurfaces;
};

class QTreeWidget;

/**
 * A normal surface page for viewing surface coordinates.
 */
class SurfacesSummaryUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::PacketOf<regina::NormalSurfaces>* surfaces;

        /**
         * Internal components
         */
        QWidget* ui;
        QWidget* pane;
        QLabel* tot;
        QLabel* totClosed;
        QLabel* totBounded;
        QLabel* totSpun;
        QTreeWidget* tableClosed;
        QTreeWidget* tableBounded;

    public:
        /**
         * Constructor and destructor.
         */
        SurfacesSummaryUI(regina::PacketOf<regina::NormalSurfaces>* packet,
            PacketTabbedUI* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        void refresh() override;
};

#endif
