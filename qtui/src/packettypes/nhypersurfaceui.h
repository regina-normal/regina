
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

/*! \file nhypersurfaceui.h
 *  \brief Provides an interface for viewing normal hypersurface lists.
 */

#ifndef __NHYPERSURFACEUI_H
#define __NHYPERSURFACEUI_H

#include "packettabui.h"
#include "reginamain.h"

#include <QLinkedList>

class NHyperCompatibilityUI;
class NHyperCoordinateUI;
class QLabel;

namespace regina {
    class NNormalHypersurfaceList;
};

/**
 * A packet interface for viewing normal surface lists.
 */
class NHyperSurfaceUI : public PacketTabbedUI {
    Q_OBJECT

    private:
        /**
         * Internal components
         */
        NHyperCoordinateUI* coords;
        NHyperCompatibilityUI* compat;

    public:
        /**
         * Constructor.
         */
        NHyperSurfaceUI(regina::NNormalHypersurfaceList* packet,
            PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        const QLinkedList<QAction*>& getPacketTypeActions();
        QString getPacketMenuText() const;
};

/**
 * A header for the normal surface list viewer.
 */
class NHyperHeaderUI : public QObject, public PacketViewerTab,
        public regina::PacketListener {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NNormalHypersurfaceList* surfaces;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* header;

    public:
        /**
         * Constructor.
         */
        NHyperHeaderUI(regina::NNormalHypersurfaceList* packet,
                PacketTabbedUI* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();

        /**
         * PacketListener overrides.
         */
        void packetWasRenamed(regina::NPacket* packet);

    private slots:
        /**
         * View the underlying triangulation.
         */
        void viewTriangulation();
};

#endif
