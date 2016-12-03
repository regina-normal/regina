
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

/*! \file tri4skeleton.h
 *  \brief Provides a skeletal properties viewer for 4-manifold triangulations.
 */

#ifndef __TRI4SKELETON_H
#define __TRI4SKELETON_H

#include "triangulation/forward.h"
#include "packettabui.h"
#include "skeletonwindow.h"

namespace regina {
    class Packet;
};

/**
 * A triangulation page for viewing skeletal properties.
 */
class Tri4SkeletonUI : public PacketTabbedViewerTab {
    public:
        /**
         * Constructor.
         */
        Tri4SkeletonUI(regina::Triangulation<4>* packet,
                PacketTabbedUI* useParentUI);
};

/**
 * A triangulation page for accessing individual skeletal components.
 */
class Tri4SkelCompUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Triangulation<4>* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* nVertices;
        QLabel* nEdges;
        QLabel* nTriangles;
        QLabel* nTetrahedra;
        QLabel* nPentachora;
        QLabel* nComps;
        QLabel* nBdryComps;
        QLabel* eulerTri;
        QLabel* eulerManifold;
        QLabel* eulerManifoldLabel;

        /**
         * Skeleton viewers
         */
        QLinkedList<SkeletonWindow*> viewers;

    public:
        /**
         * Constructor and destructor.
         */
        Tri4SkelCompUI(regina::Triangulation<4>* packet,
                PacketTabbedViewerTab* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        void refresh();

    public slots:
        /**
         * Open skeleton windows.
         */
        void viewVertices();
        void viewEdges();
        void viewTriangles();
        void viewTetrahedra();
        void viewComponents();
        void viewBoundaryComponents();
};

#endif
