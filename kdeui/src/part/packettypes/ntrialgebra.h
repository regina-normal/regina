
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

/*! \file ntrialgebra.h
 *  \brief Provides an algebra viewer for triangulations.
 */

#ifndef __NTRIALGEBRA_H
#define __NTRIALGEBRA_H

#include "reginaprefset.h"
#include "../packettabui.h"

class NTriFundGroupUI;
class QLabel;
class QLineEdit;
class QListView;
class QPushButton;

namespace regina {
    class NPacket;
    class NTriangulation;
};

/**
 * A triangulation page for viewing algebraic properties.
 */
class NTriAlgebraUI : public PacketTabbedViewerTab {
    private:
        /**
         * Internal components
         */
        NTriFundGroupUI* fundGroup;

    public:
        /**
         * Constructor.
         */
        NTriAlgebraUI(regina::NTriangulation* packet,
                PacketTabbedUI* useParentUI, const ReginaPrefSet& prefs);

        /**
         * Propagate any preference changes to our children.
         */
        void updatePreferences(const ReginaPrefSet& newPrefs);
};

/**
 * A triangulation page for viewing homology groups.
 */
class NTriHomologyUI : public PacketViewerTab {
    private:
        /**
         * Packet details
         */
        regina::NTriangulation* tri;

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
        NTriHomologyUI(regina::NTriangulation* packet,
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
class NTriFundGroupUI : public QObject, public PacketViewerTab {
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
        QLabel* fundName;
        QLabel* fundGens;
        QLabel* fundRelCount;
        QListView* fundRels;
        QPushButton* btnGAP;

        /**
         * The GAP executable.
         */
        QString GAPExec;

    public:
        /**
         * Constructor.
         */
        NTriFundGroupUI(regina::NTriangulation* packet,
                PacketTabbedViewerTab* useParentUI, const QString& useGAPExec);

        /**
         * Update preferences.
         */
        void setGAPExec(const QString& newGAPExec);

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
 * A triangulation page for viewing Turaev-Viro invariants.
 */
class NTriTuraevViroUI : public QObject, public PacketViewerTab {
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
        QLabel* paramsLabel;
        QLineEdit* params;
        QPushButton* calculate;
        QListView* invariants;

    public:
        /**
         * Constructor.
         */
        NTriTuraevViroUI(regina::NTriangulation* packet,
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
         * Calculate the requested Turaev-Viro invariant.
         */
        void calculateInvariant();
};

inline void NTriFundGroupUI::setGAPExec(const QString& newGAPExec) {
    GAPExec = newGAPExec;
}

#endif
