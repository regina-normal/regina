
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

/*! \file tri2ui.h
 *  \brief Provides an interface for viewing 2-manifold triangulations.
 */

#ifndef __TRI2UI_H
#define __TRI2UI_H

#include "triangulation/forward.h"
#include "../packettabui.h"

class QToolBar;
class Tri2GluingsUI;
class Tri2HeaderUI;
class Tri2SkeletonUI;
class QLabel;

/**
 * A packet interface for viewing 2-manifold triangulations.
 */
class Tri2UI : public PacketTabbedUI {
    Q_OBJECT

    private:
        /**
         * Internal components
         */
        Tri2HeaderUI* header;
        Tri2GluingsUI* gluings;
        Tri2SkeletonUI* skeleton;

        bool simpleToolbars;

    public:
        /**
         * Constructor and destructor.
         */
        Tri2UI(regina::PacketOf<regina::Triangulation<2>>* packet,
            PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        const std::vector<QAction*>& getPacketTypeActions() override;
        QString getPacketMenuText() const override;

    public slots:
        /**
         * Reflect preference changes.
         */
        void updatePreferences();
};

/**
 * A header for the 2-manifold triangulation viewer.
 */
class Tri2HeaderUI : public PacketViewerTab {
    private:
        /**
         * Packet details
         */
        regina::PacketOf<regina::Triangulation<2>>* tri;

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
        Tri2HeaderUI(regina::PacketOf<regina::Triangulation<2>>* packet,
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
        static QString summaryInfo(regina::Triangulation<2>* tri);
};

inline QToolBar* Tri2HeaderUI::getToolBar() {
    return bar;
}

#endif
