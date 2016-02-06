
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
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

/*! \file dim4triui.h
 *  \brief Provides an interface for viewing 4-manifold triangulations.
 */

#ifndef __DIM4TRIUI_H
#define __DIM4TRIUI_H

#include "../packettabui.h"

class QToolBar;
class Dim4TriAlgebraUI;
class Dim4TriGluingsUI;
class Dim4TriSkeletonUI;
class PacketEditIface;
class QLabel;

namespace regina {
    template <int> class Triangulation;
    typedef Triangulation<4> Dim4Triangulation;
};

/**
 * A packet interface for viewing 4-manifold triangulations.
 */
class Dim4TriangulationUI : public PacketTabbedUI {
    Q_OBJECT

    private:
        /**
         * Internal components
         */
        Dim4TriGluingsUI* gluings;
        Dim4TriSkeletonUI* skeleton;
        Dim4TriAlgebraUI* algebra;

        PacketEditIface* editIface;

    public:
        /**
         * Constructor and destructor.
         */
        Dim4TriangulationUI(regina::Dim4Triangulation* packet,
            PacketPane* newEnclosingPane);
        ~Dim4TriangulationUI();

        /**
         * PacketUI overrides.
         */
        PacketEditIface* getEditIface();
        const QLinkedList<QAction*>& getPacketTypeActions();
        QString getPacketMenuText() const;
};

/**
 * A header for the 4-manifold triangulation viewer.
 */
class Dim4TriHeaderUI : public PacketViewerTab {
    private:
        /**
         * Packet details
         */
        regina::Dim4Triangulation* tri;

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
        Dim4TriHeaderUI(regina::Dim4Triangulation* packet,
                PacketTabbedUI* useParentUI);

        /**
         * Component queries.
         */
        QToolBar* getToolBar();

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();

        /**
         * Allow other UIs to access the summary information.
         */
        static QString summaryInfo(regina::Dim4Triangulation* tri);
};

inline PacketEditIface* Dim4TriangulationUI::getEditIface() {
    return editIface;
}

inline QToolBar* Dim4TriHeaderUI::getToolBar() {
    return bar;
}

#endif
