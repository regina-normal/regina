
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

/*! \file facetgraphtab.h
 *  \brief Provides a tab for viewing the facet pairing graph of
 *  triangulations of arbitrary dimension.
 */

#ifndef __FACETGRAPHTAB_H
#define __FACETGRAPHTAB_H

#include "triangulation/forward.h"
#include "packettabui.h"
#include "skeletonwindow.h"

class MessageLayer;

class QComboBox;
class QLabel;
class QScrollArea;
class QStackedWidget;
class QSvgWidget;

namespace regina {
    class Packet;
};

/**
 * Contains dimension-specific information for viewing a facet pairing
 * graph.  Each dimension requires its own separate subclass.
 */
class FacetGraphData {
    public:
        virtual ~FacetGraphData() {}

        virtual regina::Packet* getPacket() = 0;
        virtual std::string dual(bool withLabels) = 0;
        virtual std::string treeDecomp(bool nice, int& bags, int& width) = 0;
        virtual size_t numberOfSimplices() = 0;
        virtual QString simplexName() = 0;
        virtual QString simplicesName() = 0;
        virtual QString facetName() = 0;
        virtual QString facetsName() = 0;
};

class Dim2EdgeGraphData : public FacetGraphData {
    private:
        regina::Triangulation<2>* tri_;

    public:
        Dim2EdgeGraphData(regina::Triangulation<2>* tri) : tri_(tri) {}

        regina::Packet* getPacket();
        std::string dual(bool withLabels);
        std::string treeDecomp(bool nice, int& bags, int& width);
        size_t numberOfSimplices();
        QString simplexName();
        QString simplicesName();
        QString facetName();
        QString facetsName();
};

class Dim3FaceGraphData : public FacetGraphData {
    private:
        regina::Triangulation<3>* tri_;

    public:
        Dim3FaceGraphData(regina::Triangulation<3>* tri) : tri_(tri) {}

        regina::Packet* getPacket();
        std::string dual(bool withLabels);
        std::string treeDecomp(bool nice, int& bags, int& width);
        size_t numberOfSimplices();
        QString simplexName();
        QString simplicesName();
        QString facetName();
        QString facetsName();
};

class Dim4FacetGraphData : public FacetGraphData {
    private:
        regina::Triangulation<4>* tri_;

    public:
        Dim4FacetGraphData(regina::Triangulation<4>* tri) : tri_(tri) {}

        regina::Packet* getPacket();
        std::string dual(bool withLabels);
        std::string treeDecomp(bool nice, int& bags, int& width);
        size_t numberOfSimplices();
        QString simplexName();
        QString simplicesName();
        QString facetName();
        QString facetsName();
};

/**
 * A packet viewer tab for viewing the face pairing graph.
 * This can work with triangulations of arbitrary dimension, as long as
 * an appropriate subclass of FacetGraphData is defined.
 */
class FacetGraphTab : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet and graphing details
         */
        FacetGraphData* data;

        /**
         * Internal components
         */
        QWidget* ui;
        QComboBox* chooseType;
        QLabel* graphMetrics;
        QStackedWidget* stack;
        QScrollArea* layerGraph;
        MessageLayer* layerInfo;
        MessageLayer* layerError;
        QSvgWidget* graph;
        bool neverDrawn;

        /**
         * Graphviz options.
         */
        bool graphvizLabels;

    public:
        /**
         * Constructor and destructor.
         * This object will take ownership of \a useData.
         */
        FacetGraphTab(FacetGraphData* useData,
                PacketTabbedViewerTab* useParentUI);
        ~FacetGraphTab();

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        void refresh();

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

    private slots:
        /**
         * Change graphs.
         */
        void changeType(int index);
};

inline FacetGraphTab::~FacetGraphTab() {
    delete data;
}

#endif
