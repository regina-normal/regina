
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file ntrisurfaces.h
 *  \brief Provides a normal surface properties viewer for triangulations.
 */

#ifndef __NTRISURFACES_H
#define __NTRISURFACES_H

#include "../packettabui.h"

class QButton;
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
        QButton* btnZeroEff;
        QButton* btnSplitting;
        QButton* btnThreeSphere;

        /**
         * Properties
         */
        unsigned autoCalcThreshold;

    public:
        /**
         * Constructor.
         */
        NTriSurfacesUI(regina::NTriangulation* packet,
            PacketTabbedUI* useParentUI, unsigned newAutoCalcThreshold);

        /**
         * Update properties.
         */
        void setAutoCalcThreshold(unsigned newThreshold);

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
};

inline void NTriSurfacesUI::setAutoCalcThreshold(unsigned newThreshold) {
    autoCalcThreshold = newThreshold;
}

#endif
