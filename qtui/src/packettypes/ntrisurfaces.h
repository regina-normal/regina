
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

/*! \file ntrisurfaces.h
 *  \brief Provides a normal surface properties viewer for triangulations.
 */

#ifndef __NTRISURFACES_H
#define __NTRISURFACES_H

#include "../packettabui.h"

class QAbstractButton;
class QLabel;

namespace regina {
    class NPacket;
    class NTriangulation;
};

/**
 * A triangulation page for viewing normal surface properties.
 */
class NTriSurfacesUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NTriangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* zeroEff;
        QLabel* splitting;
        QLabel* threeSphere;
        QLabel* threeBall;
        QLabel* solidTorus;
        QAbstractButton* btnZeroEff;
        QAbstractButton* btnSplitting;
        QAbstractButton* btnThreeSphere;
        QAbstractButton* btnThreeBall;
        QAbstractButton* btnSolidTorus;
        QLabel* manifold;

    public:
        /**
         * Constructor.
         */
        NTriSurfacesUI(regina::NTriangulation* packet,
            PacketTabbedUI* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
        void editingElsewhere();

    public slots:
        /**
         * Calculate properties.
         */
        void calculateZeroEff();
        void calculateSplitting();
        void calculateThreeSphere();
        void calculateThreeBall();
        void calculateSolidTorus();

        /**
         * Notify that preferences have changed.
         */
        void updatePreferences();
};

inline void NTriSurfacesUI::updatePreferences() {
    refresh();
}

#endif
