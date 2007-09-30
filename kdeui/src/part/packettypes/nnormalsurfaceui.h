
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

/*! \file nnormalsurfaceui.h
 *  \brief Provides an interface for viewing normal surface lists.
 */

#ifndef __NNORMALSURFACEUI_H
#define __NNORMALSURFACEUI_H

#include "../packettabui.h"

class NSurfaceCoordinateUI;
class QLabel;

namespace regina {
    class NNormalSurfaceList;
};

/**
 * A packet interface for viewing normal surface lists.
 */
class NNormalSurfaceUI : public PacketTabbedUI {
    Q_OBJECT

    private:
        /**
         * Internal components
         */
        NSurfaceCoordinateUI* coords;

    public:
        /**
         * Constructor.
         */
        NNormalSurfaceUI(regina::NNormalSurfaceList* packet,
            PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        const QPtrList<KAction>& getPacketTypeActions();
        QString getPacketMenuText() const;
};

/**
 * A header for the normal surface list viewer.
 */
class NSurfaceHeaderUI : public PacketViewerTab {
    private:
        /**
         * Packet details
         */
        regina::NNormalSurfaceList* surfaces;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* header;

    public:
        /**
         * Constructor.
         */
        NSurfaceHeaderUI(regina::NNormalSurfaceList* packet,
                PacketTabbedUI* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
};

#endif
