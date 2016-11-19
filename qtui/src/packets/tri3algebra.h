
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

/*! \file tri3algebra.h
 *  \brief Provides an algebra viewer for triangulations.
 */

#ifndef __TRI3ALGEBRA_H
#define __TRI3ALGEBRA_H

#include "triangulation/forward.h"
#include "../packettabui.h"

class GroupWidget;
class QLabel;
class QLineEdit;
class QTreeWidget;
class QPushButton;

namespace regina {
    class Packet;
};

/**
 * A triangulation page for viewing algebraic properties.
 */
class Tri3AlgebraUI : public PacketTabbedViewerTab {
    public:
        /**
         * Constructor.
         */
        Tri3AlgebraUI(regina::Triangulation<3>* packet,
                PacketTabbedUI* useParentUI);
};

/**
 * A triangulation page for viewing homology and the fundamental group.
 */
class Tri3HomologyFundUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Triangulation<3>* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* labelH1;
        QLabel* labelH1Rel;
        QLabel* labelH1Bdry;
        QLabel* labelH2;
        QLabel* labelH2Z2;
        QLabel* H1;
        QLabel* H1Rel;
        QLabel* H1Bdry;
        QLabel* H2;
        QLabel* H2Z2;
        QLabel* fgMsg;
        GroupWidget* fgGroup;

    public:
        /**
         * Constructor.
         */
        Tri3HomologyFundUI(regina::Triangulation<3>* packet,
                PacketTabbedViewerTab* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        void refresh();

    public slots:
        /**
         * Notify us that the presentation has been simplified.
         */
        void fundGroupSimplified();
        /**
         * Note that preferences have changed.
         */
        void updatePreferences();

    private:
        /**
         * Update the static labels according to current unicode preferences.
         */
        void refreshLabels();
};

/**
 * A triangulation page for viewing Turaev-Viro invariants.
 */
class Tri3TuraevViroUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Triangulation<3>* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* paramsLabel;
        QLineEdit* params;
        QPushButton* calculate;
        QTreeWidget* invariants;

    public:
        /**
         * Constructor.
         */
        Tri3TuraevViroUI(regina::Triangulation<3>* packet,
                PacketTabbedViewerTab* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        void refresh();

    public slots:
        /**
         * Calculate the requested Turaev-Viro invariant.
         */
        void calculateInvariant();
        /**
         * Note that preferences have changed.
         */
        void updatePreferences();

    private:
        bool calculateInvariant(unsigned long r, bool parity);
};

/**
 * A page for viewing detailed cellular information about the manifold.
 *
 * \author Ryan Budney
 */
class Tri3CellularInfoUI: public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Triangulation<3>* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* labelBdryMap;
        QLabel* Cells;
        QLabel* DualCells;
        QLabel* EulerChar;
        QLabel* H0H1H2H3;
        QLabel* HBdry;
        QLabel* BdryMap;
        QLabel* TorForOrders; // torsion subgroup prime power decomp
        QLabel* TorForSigma;  // the Kawauchi-Kojima 2-torsion sigma vector
        QLabel* TorForLegendre; // the odd p-torsion Legendre symbol vector
        QLabel* EmbeddingComments; // comments on what the manifold may
                                   // or may not embed in.

    public:
        /**
         * Constructor.
         */
        Tri3CellularInfoUI(regina::Triangulation<3>* packet,
                PacketTabbedViewerTab* useParentUI);

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

    private:
        /**
         * Update the static labels according to current unicode preferences.
         */
        void refreshLabels();
};

#endif
