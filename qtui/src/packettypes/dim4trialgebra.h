
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

/*! \file dim4trialgebra.h
 *  \brief Provides an algebra viewer for 4-dimensional triangulations.
 */

#ifndef __DIM4TRIALGEBRA_H
#define __DIM4TRIALGEBRA_H

#include "../packettabui.h"

class Dim4TriFundGroupUI;
class QLabel;
class QLineEdit;
class QListWidget;
class QTreeWidget;
class QPushButton;

namespace regina {
    class NPacket;
    class Dim4Triangulation;
};

/**
 * A triangulation page for viewing algebraic properties.
 */
class Dim4TriAlgebraUI : public PacketTabbedViewerTab {
    private:
        /**
         * Internal components
         */
        Dim4TriFundGroupUI* fundGroup;

    public:
        /**
         * Constructor.
         */
        Dim4TriAlgebraUI(regina::Dim4Triangulation* packet,
                PacketTabbedUI* useParentUI);
};

/**
 * A triangulation page for viewing homology groups.
 */
class Dim4TriHomologyUI : public PacketViewerTab {
    private:
        /**
         * Packet details
         */
        regina::Dim4Triangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* H1;
        QLabel* H2;

    public:
        /**
         * Constructor.
         */
        Dim4TriHomologyUI(regina::Dim4Triangulation* packet,
                PacketTabbedViewerTab* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
        void editingElsewhere();
};

/**
 * A triangulation page for viewing the fundamental group.
 */
class Dim4TriFundGroupUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Dim4Triangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* fundName;
        QLabel* fundGens;
        QLabel* fundRelCount;
        QListWidget* fundRels;
        QPushButton* btnGAP;
        QPushButton* btnSimp;
        unsigned simpDepth;

    public:
        /**
         * Constructor.
         */
        Dim4TriFundGroupUI(regina::Dim4Triangulation* packet,
                PacketTabbedViewerTab* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
        void editingElsewhere();

    public slots:
        /**
         * Group simplification actions.
         */
        void simplifyGAP();
        /**
         * Our internal pi1 simplification code.
         */
        void simplifyPi1();

    private:
        /**
         * Returns the full path to the GAP executable, or QString::null
         * if the GAP executable does not appear to be valid.
         */
        QString verifyGAPExec();
};

#endif
