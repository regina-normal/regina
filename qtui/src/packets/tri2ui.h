
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file tri2ui.h
 *  \brief Provides an interface for viewing 2-manifold triangulations.
 */

#ifndef __TRI2UI_H
#define __TRI2UI_H

#include "triangulation/forward.h"
#include "../packettabui.h"

class QToolBar;
class Tri2GluingsUI;
class Tri2SkeletonUI;
class PacketEditIface;
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
        Tri2GluingsUI* gluings;
        Tri2SkeletonUI* skeleton;

        PacketEditIface* editIface;

    public:
        /**
         * Constructor and destructor.
         */
        Tri2UI(regina::Triangulation<2>* packet,
            PacketPane* newEnclosingPane);
        ~Tri2UI();

        /**
         * PacketUI overrides.
         */
        PacketEditIface* getEditIface();
        const QLinkedList<QAction*>& getPacketTypeActions();
        QString getPacketMenuText() const;
};

/**
 * A header for the 2-manifold triangulation viewer.
 */
class Tri2HeaderUI : public PacketViewerTab {
    private:
        /**
         * Packet details
         */
        regina::Triangulation<2>* tri;

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
        Tri2HeaderUI(regina::Triangulation<2>* packet,
                PacketTabbedUI* useParentUI);

        /**
         * Component queries.
         */
        QToolBar* getToolBar();

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        void refresh();

        /**
         * Allow other UIs to access the summary information.
         */
        static QString summaryInfo(regina::Triangulation<2>* tri);
};

inline PacketEditIface* Tri2UI::getEditIface() {
    return editIface;
}

inline QToolBar* Tri2HeaderUI::getToolBar() {
    return bar;
}

#endif
