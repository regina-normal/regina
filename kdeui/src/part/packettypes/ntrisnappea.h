
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
 *  \brief Provides access to SnapPea calculations for triangulations.
 */

#ifndef __NTRISNAPPEA_H
#define __NTRISNAPPEA_H

#include "../packettabui.h"

class NoSnapPea;
class QLabel;
class QWidgetStack;

namespace regina {
    class NPacket;
    class NSnapPeaTriangulation;
    class NTriangulation;
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
        QWidget* ui;
        QWidgetStack* data;
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
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
        void editingElsewhere();

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

#endif
