
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

/*! \file ntriskeleton.h
 *  \brief Provides a skeletal properties viewer for triangulations.
 */

#ifndef __NTRISKELETON_H
#define __NTRISKELETON_H

#include "../packettabui.h"

#include <qptrlist.h>

class SkeletonWindow;

namespace regina {
    class NPacket;
    class NTriangulation;
};

/**
 * A triangulation page for viewing skeletal properties.
 */
class NTriSkeletonUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NTriangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* nVertices;
        QLabel* nEdges;
        QLabel* nFaces;
        QLabel* nTets;
        QLabel* nComps;
        QLabel* nBdryComps;

        /**
         * Skeleton viewers
         */
        QPtrList<SkeletonWindow> viewers;

    public:
        /**
         * Constructor and destructor.
         */
        NTriSkeletonUI(regina::NTriangulation* packet,
                PacketTabbedUI* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
        void editingElsewhere();

    public slots:
        /**
         * Open skeleton windows.
         */
        void viewVertices();
        void viewEdges();
        void viewFaces();
        void viewComponents();
        void viewBoundaryComponents();
};

#endif
