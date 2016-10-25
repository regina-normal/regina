
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

/*! \file dim2triui.h
 *  \brief Provides an interface for viewing 2-manifold triangulations.
 */

#ifndef __DIM2TRIUI_H
#define __DIM2TRIUI_H

#include "../packettabui.h"

class QToolBar;
class Dim2TriGluingsUI;
class Dim2TriSkeletonUI;
class PacketEditIface;
class QLabel;

namespace regina {
    template <int> class Triangulation;
};

/**
 * A packet interface for viewing 2-manifold triangulations.
 */
class Dim2TriangulationUI : public PacketTabbedUI {
    Q_OBJECT

    private:
        /**
         * Internal components
         */
        Dim2TriGluingsUI* gluings;
        Dim2TriSkeletonUI* skeleton;

        PacketEditIface* editIface;

    public:
        /**
         * Constructor and destructor.
         */
        Dim2TriangulationUI(regina::Triangulation<2>* packet,
            PacketPane* newEnclosingPane);
        ~Dim2TriangulationUI();

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
class Dim2TriHeaderUI : public PacketViewerTab {
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
        Dim2TriHeaderUI(regina::Triangulation<2>* packet,
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

inline PacketEditIface* Dim2TriangulationUI::getEditIface() {
    return editIface;
}

inline QToolBar* Dim2TriHeaderUI::getToolBar() {
    return bar;
}

#endif
