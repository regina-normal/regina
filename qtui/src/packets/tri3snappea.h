
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file tri3snappea.h
 *  \brief Provides access to SnapPea calculations for triangulations.
 */

#ifndef __TRI3SNAPPEA_H
#define __TRI3SNAPPEA_H

#include "triangulation/forward.h"
#include "../packettabui.h"

class NoSnapPea;
class QLabel;
class QStackedWidget;

namespace regina {
    class Packet;
    class SnapPeaTriangulation;
};

/**
 * A triangulation page for viewing normal surface properties.
 */
class Tri3SnapPeaUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::PacketOf<regina::Triangulation<3>>* reginaTri;
        regina::SnapPeaTriangulation* snappeaTri;

        /**
         * Internal components
         */
        QStackedWidget* ui;
        QWidget* dataValid;
        QWidget* dataNull;
        QLabel* solutionType;
        QLabel* solutionTypeLabel;
        QString solutionTypeExplnBase;
        QLabel* volume;
        NoSnapPea* unavailable;

    public:
        /**
         * Constructor and destructor.
         */
        Tri3SnapPeaUI(regina::PacketOf<regina::Triangulation<3>>* packet,
            PacketTabbedUI* useParentUI);
        ~Tri3SnapPeaUI() override;

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        void refresh() override;

    public slots:
        /**
         * Update properties.
         */
        void updatePreferences();

        /**
         * Convert this to a full SnapPea triangulation.
         */
        void toSnapPea();
};

inline void Tri3SnapPeaUI::updatePreferences() {
    refresh();
}

#endif
