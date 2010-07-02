
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file dim4triui.h
 *  \brief Provides an interface for viewing 4-manifold triangulations.
 */

#ifndef __DIM4TRIUI_H
#define __DIM4TRIUI_H

#include "../packettabui.h"

class KToolBar;
class Dim4TriAlgebraUI;
class Dim4TriGluingsUI;
class Dim4TriSkeletonUI;
class QLabel;
class ReginaPrefSet;

namespace regina {
    class Dim4Triangulation;
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

    public:
        /**
         * Constructor.
         */
        Dim4TriangulationUI(regina::Dim4Triangulation* packet,
            PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        const QPtrList<KAction>& getPacketTypeActions();
        QString getPacketMenuText() const;

    public slots:
        /**
         * Notify this interface that the global preferences have been
         * updated.
         */
        void updatePreferences(const ReginaPrefSet& newPrefs);
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
        KToolBar* bar;

    public:
        /**
         * Constructor.
         */
        Dim4TriHeaderUI(regina::Dim4Triangulation* packet,
                PacketTabbedUI* useParentUI);

        /**
         * Component queries.
         */
        KToolBar* getToolBar();

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
        void editingElsewhere();
};

inline KToolBar* Dim4TriHeaderUI::getToolBar() {
    return bar;
}

#endif
