
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file linkgraph.h
 *  \brief Provides a tab for viewing the graphs related to a knot or link.
 */

#ifndef __LINKGRAPH_H
#define __LINKGRAPH_H

#include "packettabui.h"

class MessageLayer;

class QComboBox;
class QLabel;
class QScrollArea;
class QStackedWidget;
class QSvgWidget;

namespace regina {
    class Link;
    class Packet;
};

/**
 * A packet viewer tab for viewing graphs related to a link.
 */
class LinkGraphUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * The link itself.
         */
        regina::Link* link;

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

    public:
        /**
         * Constructor and destructor.
         */
        LinkGraphUI(regina::Link* useLink, PacketTabbedUI* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        void refresh();

    private:
        /**
         * Set up internal components.
         */
        void showInfo(const QString& msg);
        void showError(const QString& msg);

        std::string treeDecomp(bool nice, int& bags, int& width);

    private slots:
        /**
         * Change graphs.
         */
        void changeType(int index);
};

#endif
