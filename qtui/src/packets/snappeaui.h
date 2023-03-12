
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

/*! \file snappeaui.h
 *  \brief Provides an interface for viewing SnapPea triangulations.
 */

#ifndef __SNAPPEAUI_H
#define __SNAPPEAUI_H

#include "packet/packet.h"
#include "../packettabui.h"

class SnapPeaAlgebraUI;
class SnapPeaGluingsUI;
class SnapPeaShapesUI;
class Tri3SkeletonUI;
class PacketEditIface;
class QLabel;
class QToolBar;

namespace regina {
    class SnapPeaTriangulation;
};

/**
 * A packet interface for viewing 3-manifold triangulations.
 */
class SnapPeaUI : public PacketTabbedUI {
    Q_OBJECT

    private:
        /**
         * Internal components
         */
        SnapPeaShapesUI* shapes;
        SnapPeaGluingsUI* gluings;
        Tri3SkeletonUI* skeleton;
        SnapPeaAlgebraUI* algebra;

        PacketEditIface* editIface;

    public:
        /**
         * Constructor and destructor.
         */
        SnapPeaUI(regina::PacketOf<regina::SnapPeaTriangulation>* packet,
            PacketPane* newEnclosingPane);
        ~SnapPeaUI() override;

        /**
         * PacketUI overrides.
         */
        PacketEditIface* getEditIface() override;
        const std::vector<QAction*>& getPacketTypeActions() override;
        QString getPacketMenuText() const override;
};

/**
 * A header for the SnapPea triangulation viewer.
 */
class SnapPeaHeaderUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::PacketOf<regina::SnapPeaTriangulation>* tri;

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
        SnapPeaHeaderUI(regina::PacketOf<regina::SnapPeaTriangulation>* packet,
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
        static QString summaryInfo(regina::SnapPeaTriangulation* tri);
};

inline PacketEditIface* SnapPeaUI::getEditIface() {
    return editIface;
}

inline QToolBar* SnapPeaHeaderUI::getToolBar() {
    return bar;
}

#endif
