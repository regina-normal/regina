
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

/*! \file nsurfacesummaryui.h
 *  \brief Provides a tab that summarises all normal surfaces in a list.
 */

#ifndef __NSURFACESUMMARYUI_H
#define __NSURFACESUMMARYUI_H

#include "packet/npacketlistener.h"

#include "../packettabui.h"

namespace regina {
    class NPacket;
    class NNormalSurfaceList;
};

/**
 * A normal surface page for viewing surface coordinates.
 */
class NSurfaceSummaryUI : public QObject, public PacketViewerTab,
        public regina::NPacketListener {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NNormalSurfaceList* surfaces;

        /**
         * Internal components
         */
        QWidget* ui;

    public:
        /**
         * Constructor and destructor.
         */
        NSurfaceSummaryUI(regina::NNormalSurfaceList* packet,
            PacketTabbedUI* useParentUI);
        ~NSurfaceSummaryUI();

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
};

#endif
