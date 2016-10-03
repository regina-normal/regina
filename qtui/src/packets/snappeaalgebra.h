
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

/*! \file snappeaalgebra.h
 *  \brief Provides an algebra viewer for SnapPea triangulations.
 */

#ifndef __SNAPPEAALGEBRA_H
#define __SNAPPEAALGEBRA_H

#include "../packettabui.h"

class GroupWidget;
class QLabel;

namespace regina {
    class Packet;
    class SnapPeaTriangulation;
};

/**
 * A triangulation page for viewing algebraic properties.
 */
class SnapPeaAlgebraUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::SnapPeaTriangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;

        QLabel* filledH1;
        QLabel* unfilledH1;
        GroupWidget* filledFundGroup;
        GroupWidget* unfilledFundGroup;

        QLabel* filledH1Title;
        QLabel* unfilledH1Title;
        QLabel* filledFundGroupTitle;
        QLabel* unfilledFundGroupTitle;

    public:
        /**
         * Constructor.
         */
        SnapPeaAlgebraUI(regina::SnapPeaTriangulation* packet,
                PacketTabbedUI* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        void refresh();

    public slots:
        /**
         * Note that preferences have changed.
         */
        void updatePreferences();
};

#endif
