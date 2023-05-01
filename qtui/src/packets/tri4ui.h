
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

/*! \file tri4ui.h
 *  \brief Provides an interface for viewing 4-manifold triangulations.
 */

#ifndef __TRI4UI_H
#define __TRI4UI_H

#include "packet/packet.h"
#include "triangulation/forward.h"
#include "../packettabui.h"

class Tri4AlgebraUI;
class Tri4GluingsUI;
class Tri4SkeletonUI;
class QLabel;
class QToolBar;

/**
 * A packet interface for viewing 4-manifold triangulations.
 */
class Tri4UI : public PacketTabbedUI {
    Q_OBJECT

    private:
        /**
         * Internal components
         */
        Tri4GluingsUI* gluings;
        Tri4SkeletonUI* skeleton;
        Tri4AlgebraUI* algebra;

    public:
        /**
         * Constructor and destructor.
         */
        Tri4UI(regina::PacketOf<regina::Triangulation<4>>* packet,
            PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        const std::vector<QAction*>& getPacketTypeActions() override;
        QString getPacketMenuText() const override;
};

/**
 * A header for the 4-manifold triangulation viewer.
 */
class Tri4HeaderUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::PacketOf<regina::Triangulation<4>>* tri;

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
        Tri4HeaderUI(regina::PacketOf<regina::Triangulation<4>>* packet,
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
        static QString summaryInfo(regina::Triangulation<4>* tri);
};

inline QToolBar* Tri4HeaderUI::getToolBar() {
    return bar;
}

#endif
