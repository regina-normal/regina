
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
 *  For further details contact Ben Burton (bab@debian.org).              *
 *                                                                        *
 *  This program is free software; you can redistribute it and/or         *
 *  modify it under the terms of the GNU General Public License as        *
 *  published by the Free Software Foundation; either version 2 of the    *
 *  License, or (at your option) any later version.                       *
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

/* end stub */

/*! \file ntriangulationui.h
 *  \brief Provides an interface for viewing 3-manifold triangulations.
 */

#ifndef __NTRIANGULATIONUI_H
#define __NTRIANGULATIONUI_H

#include "../packettabui.h"

class QToolBar;
class NTriAlgebraUI;
class NTriGluingsUI;
class NTriSkeletonUI;
class NTriSurfacesUI;
class NTriSnapPeaUI;
class PacketEditIface;
class QLabel;

namespace regina {
    class NTriangulation;
};

/**
 * A packet interface for viewing 3-manifold triangulations.
 */
class NTriangulationUI : public PacketTabbedUI {
    Q_OBJECT

    private:
        /**
         * Internal components
         */
        NTriGluingsUI* gluings;
        NTriSkeletonUI* skeleton;
        NTriAlgebraUI* algebra;
        NTriSurfacesUI* surfaces;
        NTriSnapPeaUI* snapPea;

        PacketEditIface* editIface;

    public:
        /**
         * Constructor and destructor.
         */
        NTriangulationUI(regina::NTriangulation* packet,
            PacketPane* newEnclosingPane);
        ~NTriangulationUI();

        /**
         * PacketUI overrides.
         */
        PacketEditIface* getEditIface();
        const QLinkedList<QAction*>& getPacketTypeActions();
        QString getPacketMenuText() const;
};

/**
 * A header for the 3-manifold triangulation viewer.
 */
class NTriHeaderUI : public PacketViewerTab {
    private:
        /**
         * Packet details
         */
        regina::NTriangulation* tri;

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
        NTriHeaderUI(regina::NTriangulation* packet,
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
        void editingElsewhere();

        /**
         * Allow other UIs to access the summary information.
         */
        static QString summaryInfo(regina::NTriangulation* tri);
};

inline PacketEditIface* NTriangulationUI::getEditIface() {
    return editIface;
}

inline QToolBar* NTriHeaderUI::getToolBar() {
    return bar;
}

#endif
