
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

/*! \file nsurfacematchingui.h
 *  \brief Provides a matching equation viewer for normal surface lists.
 */

#ifndef __NSURFACEMATCHINGUI_H
#define __NSURFACEMATCHINGUI_H

#include "../packettabui.h"

class QListView;

namespace regina {
    class NNormalSurfaceList;
    class NPacket;
};

/**
 * A surface list page for viewing matching equations.
 */
class NSurfaceMatchingUI : public PacketViewerTab {
    private:
        /**
         * Packet details
         */
        regina::NNormalSurfaceList* surfaces;

        /**
         * Internal components
         */
        QWidget* ui;
        QListView* table;

    public:
        /**
         * Constructor.
         */
        NSurfaceMatchingUI(regina::NNormalSurfaceList* packet,
                PacketTabbedUI* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
};

#endif
