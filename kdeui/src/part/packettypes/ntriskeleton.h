
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

/*! \file ntriskeleton.h
 *  \brief Provides a skeletal properties viewer for triangulations.
 */

#ifndef __NTRISKELETON_H
#define __NTRISKELETON_H

#include "reginaprefset.h"
#include "../packettabui.h"
#include "../reginapart.h"

#include <qptrlist.h>

class NTriFaceGraphUI;
class SkeletonWindow;

class QWidgetStack;

namespace regina {
    class NPacket;
    class NTriangulation;
};

/**
 * A triangulation page for viewing skeletal properties.
 */
class NTriSkeletonUI : public PacketTabbedViewerTab {
    private:
        /**
         * Internal components
         */
        NTriFaceGraphUI* faceGraph;

    public:
        /**
         * Constructor.
         */
        NTriSkeletonUI(regina::NTriangulation* packet,
                PacketTabbedUI* useParentUI, const ReginaPrefSet& prefs);

        /**
         * Propagate any preference changes to our children.
         */
        void updatePreferences(const ReginaPrefSet& newPrefs);
};

/**
 * A triangulation page for accessing individual skeletal components.
 */
class NTriSkelCompUI : public QObject, public PacketViewerTab {
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
        NTriSkelCompUI(regina::NTriangulation* packet,
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
        void viewFaces();
        void viewComponents();
        void viewBoundaryComponents();
};

/**
 * A triangulation page for viewing the face pairing graph.
 */
class NTriFaceGraphUI : public QObject, public PacketViewerTab {
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
        QWidgetStack* stack;
        QWidget* layerGraph;
        QWidget* layerInfo;
        QWidget* layerError;
        QLabel* msgInfo;
        QLabel* msgError;
        QLabel* graph;

        /**
         * The Graphviz executable.
         */
        QString graphvizExec;

    public:
        /**
         * Constructor and destructor.
         */
        NTriFaceGraphUI(regina::NTriangulation* packet,
                PacketTabbedViewerTab* useParentUI,
                const QString& useGraphvizExec);

        /**
         * Update preferences.
         */
        void setGraphvizExec(const QString& newGraphvizExec);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
        void editingElsewhere();

    private:
        /**
         * Set up internal components.
         */
        QWidget* messageLayer(QLabel*& text, const char* icon);
        void showError(const QString& msg);

        /**
         * Returns the full path to the Graphviz executable, or QString::null
         * if the Graphviz executable does not appear to be valid.
         */
        QString verifyGraphvizExec();
};

inline void NTriSkeletonUI::updatePreferences(const ReginaPrefSet& newPrefs) {
    faceGraph->setGraphvizExec(newPrefs.triGraphvizExec);
}

inline void NTriFaceGraphUI::setGraphvizExec(const QString& newGraphvizExec) {
    graphvizExec = newGraphvizExec;
}

#endif
