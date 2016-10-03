
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

/*! \file ntrisurfaces.h
 *  \brief Provides access to SnapPea calculations for triangulations.
 */

#ifndef __NTRISNAPPEA_H
#define __NTRISNAPPEA_H

#include "../packettabui.h"

class NoSnapPea;
class QLabel;
class QStackedWidget;

namespace regina {
    class Packet;
    class NSnapPeaTriangulation;
    template <int> class Triangulation;
    typedef Triangulation<3> NTriangulation;
};

/**
 * A triangulation page for viewing normal surface properties.
 */
class NTriSnapPeaUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NTriangulation* reginaTri;
        regina::NSnapPeaTriangulation* snappeaTri;

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
        NTriSnapPeaUI(regina::NTriangulation* packet,
            PacketTabbedUI* useParentUI);
        ~NTriSnapPeaUI();

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        void refresh();

    public slots:
        /**
         * Update properties.
         */
        void updatePreferences();

        /**
         * Convert this to a full SnapPea triangulation.
         */
        void toSnapPea();

    private:
        /**
         * These routines convert an NSnapPeaTriangulation::SolutionType
         * into various types of human-readable string.  They are declared
         * to take an int so that the calculation engine headers need not
         * be included.
         */
        static QString solutionTypeString(int solnType);
        static QString solutionTypeExplanation(int solnType);
};

inline void NTriSnapPeaUI::updatePreferences() {
    refresh();
}

#endif
