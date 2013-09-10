
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

/*! \file dim2triskeleton.h
 *  \brief Provides a skeletal properties viewer for 2-manifold triangulations.
 */

#ifndef __DIM2TRISKELETON_H
#define __DIM2TRISKELETON_H

#include "packettabui.h"
#include "skeletonwindow.h"


class MessageLayer;

class QScrollArea;
class QStackedWidget;

namespace regina {
    class Dim2Triangulation;
    class NPacket;
};

/**
 * A triangulation page for viewing skeletal properties.
 */
class Dim2TriSkeletonUI : public PacketTabbedViewerTab {
    private:
        /**
         * Internal components
         */
        // TODO NTriFaceGraphUI* faceGraph;

    public:
        /**
         * Constructor.
         */
        Dim2TriSkeletonUI(regina::Dim2Triangulation* packet,
                PacketTabbedUI* useParentUI);
};

/**
 * A triangulation page for accessing individual skeletal components.
 */
class Dim2TriSkelCompUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Dim2Triangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* nVertices;
        QLabel* nEdges;
        QLabel* nTriangles;
        QLabel* nComps;
        QLabel* nBdryComps;

        /**
         * Skeleton viewers
         */
        QLinkedList<SkeletonWindow*> viewers;

    public:
        /**
         * Constructor and destructor.
         */
        Dim2TriSkelCompUI(regina::Dim2Triangulation* packet,
                PacketTabbedViewerTab* useParentUI);

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
        void viewComponents();
        void viewBoundaryComponents();
};

#if 0
/**
 * A triangulation page for viewing the face pairing graph.
 */
class NTriFaceGraphUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Dim2Triangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QStackedWidget* stack;
        QScrollArea* layerGraph;
        MessageLayer* layerInfo;
        MessageLayer* layerError;
        QLabel* graph;
        bool neverDrawn;

        /**
         * Graphviz options.
         */
        QString graphvizExec;
        bool graphvizLabels;

    public:
        /**
         * Constructor and destructor.
         */
        NTriFaceGraphUI(regina::Dim2Triangulation* packet,
                PacketTabbedViewerTab* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
        void editingElsewhere();

    public slots:
        /**
         * Reflect preference changes.
         */
        void updatePreferences();

    private:
        /**
         * Set up internal components.
         */
        void showInfo(const QString& msg);
        void showError(const QString& msg);
};
#endif

#endif
