
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

/*! \file ntriangulationui.h
 *  \brief Provides an interface for viewing triangulations.
 */

#ifndef __NTRIANGULATIONUI_H
#define __NTRIANGULATIONUI_H

#include "../packettabui.h"

class KToolBar;
class NTriAlgebraUI;
class NTriGluingsUI;
class NTriSkeletonUI;
class NTriSurfacesUI;
class NTriSnapPeaUI;
class QLabel;
class ReginaPrefSet;

namespace regina {
    class NTriangulation;
};

/**
 * A packet interface for viewing triangulations.
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

    public:
        /**
         * Constructor.
         */
        NTriangulationUI(regina::NTriangulation* packet,
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
 * A header for the triangulation viewer.
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
        KToolBar* bar;

    public:
        /**
         * Constructor.
         */
        NTriHeaderUI(regina::NTriangulation* packet,
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

inline KToolBar* NTriHeaderUI::getToolBar() {
    return bar;
}

#endif
