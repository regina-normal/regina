
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file ntricomposition.h
 *  \brief Provides a combinatorial composition viewer for triangulations.
 */

#ifndef __NTRICOMPOSITION_H
#define __NTRICOMPOSITION_H

#include "../packettabui.h"

class QListView;
class QListViewItem;

namespace regina {
    class NPacket;
    class NStandardTriangulation;
    class NTriangulation;
};

/**
 * A triangulation page for viewing the combinatorial composition.
 */
class NTriCompositionUI : public PacketViewerTab {
    private:
        /**
         * Packet details
         */
        regina::NTriangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QListView* details;
        QListViewItem* components;
        QListViewItem* lastComponent;

    public:
        /**
         * Constructor.
         */
        NTriCompositionUI(regina::NTriangulation* packet,
                PacketTabbedUI* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
        void editingElsewhere();

    private:
        /**
         * Add new items to the list view.
         */
        QListViewItem* addTopLevelSection(const QString& text);
        QListViewItem* addComponentSection(const QString& text);

        /**
         * Fill the list view with information.
         */
        void findAugTriSolidTori();
        void findLayeredChainPairs();
        void findLayeredLensSpaces();
        void findLayeredLoops();
        void findLayeredSolidTori();
        void findPillowSpheres();
        void findPlugTriSolidTori();
        void findSnappedBalls();
        void findSnappedSpheres();
        void findSpiralSolidTori();

        /**
         * Return string representations of tetrahedron edges.
         */
        static QString edgeString(unsigned long tetIndex, int edge1,
            int edge2);
        static QString edgeString(unsigned long tetIndex,
            const regina::NPerm& roles, int startPreimage, int endPreimage);
};

#endif
