
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

/*! \file dim4trialgebra.h
 *  \brief Provides an algebra viewer for 4-dimensional triangulations.
 */

#ifndef __DIM4TRIALGEBRA_H
#define __DIM4TRIALGEBRA_H

#include "../packettabui.h"


class QToolBar;
class Dim4TriAlgebraUI;
class Dim4TriFundGroupUI;
class QLabel;
class QWidget;
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
        QLabel* H1Rel;
        QLabel* H1Bdry;
        QLabel* H2;
        QLabel* H2Z2;

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

    private:
        /**
         * Returns the full path to the GAP executable, or QString::null
         * if the GAP executable does not appear to be valid.
         */
        QString verifyGAPExec();
};

/**
 * A page for viewing detailed cellular information about the manifold.
 *
 * \author Ryan Budney
 */
class Dim4TriCellularInfoUI: public PacketViewerTab {
    private:
        /**
         * Packet details
         */
        regina::Dim4Triangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* Cells;
        QLabel* DualCells;
        QLabel* EulerChar;
        QLabel* H0H1H2H3;
        QLabel* HBdry;
        QLabel* BdryMap;
        QLabel* TorForOrders; // torsion subgroup prime power decomp
        QLabel* TorForSigma;  // the Kawauchi-Kojima 2-torsion sigma vector
        QLabel* TorForLegendre; // the odd p-torsion Legendre symbol vector
        QLabel* AlexInv; // Alexander module invariants (if exists)
        QLabel* EmbeddingComments; // comments on what the manifold may
                                   // or may not embed in.

    public:
        /**
         * Constructor.
         */
        Dim4TriCellularInfoUI(regina::Dim4Triangulation* packet,
                PacketTabbedViewerTab* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        void refresh();
        void editingElsewhere();
};

#endif
