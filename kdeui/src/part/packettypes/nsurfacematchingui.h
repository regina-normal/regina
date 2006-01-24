
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

/*! \file nsurfacematchingui.h
 *  \brief Provides a matching equation viewer for normal surface lists.
 */

#ifndef __NSURFACEMATCHINGUI_H
#define __NSURFACEMATCHINGUI_H

#include "../packettabui.h"

#include <memory>
#include <qtooltip.h>

class QHeader;
class QListView;
class MatchingHeaderToolTip;

namespace regina {
    class NMatrixInt;
    class NNormalSurfaceList;
    class NPacket;
};

/**
 * A surface list page for viewing matching equations.
 */
class NSurfaceMatchingUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NNormalSurfaceList* surfaces;
        std::auto_ptr<regina::NMatrixInt> eqns;

        /**
         * Internal components
         */
        QWidget* ui;
        QListView* table;
        MatchingHeaderToolTip* headerTips;

        /**
         * Status of any ongoing actions.
         */
        bool currentlyAutoResizing;

    public:
        /**
         * Constructor and destructor.
         */
        NSurfaceMatchingUI(regina::NNormalSurfaceList* packet,
                PacketTabbedUI* useParentUI);
        ~NSurfaceMatchingUI();

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();

    public slots:
        /**
         * Provides auto-resizing of columns.
         */
        void columnResized(int section, int oldSize, int newSize);
};

/**
 * A utility clsas for displaying tooltips for table headers.
 */
class MatchingHeaderToolTip : public QToolTip {
    private:
        /**
         * Matching equation information
         */
        regina::NTriangulation* tri;
        int coordSystem;

    public:
        /**
         * Constructor.
         */
        MatchingHeaderToolTip(regina::NTriangulation* useTri,
            int useCoordSystem, QHeader* header, QToolTipGroup* group = 0);

    protected:
        /**
         * QToolTip overrides.
         */
        void maybeTip(const QPoint& p);
};

#endif
