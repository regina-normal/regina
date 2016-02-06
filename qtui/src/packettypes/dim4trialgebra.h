
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

/*! \file dim4trialgebra.h
 *  \brief Provides an algebra viewer for 4-manifold triangulations.
 */

#ifndef __DIM4TRIALGEBRA_H
#define __DIM4TRIALGEBRA_H

#include "../packettabui.h"

class GroupWidget;
class QLabel;

namespace regina {
    class NPacket;

    template <int> class Triangulation;
    typedef Triangulation<4> Dim4Triangulation;
};

/**
 * A triangulation page for viewing algebraic properties.
 */
class Dim4TriAlgebraUI : public PacketTabbedViewerTab {
    public:
        /**
         * Constructor.
         */
        Dim4TriAlgebraUI(regina::Dim4Triangulation* packet,
                PacketTabbedUI* useParentUI);
};

/**
 * A triangulation page for viewing homology and the fundamental group.
 */
class Dim4TriHomologyFundUI : public QObject, public PacketViewerTab {
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
        QLabel* labelH1;
        QLabel* labelH2;
        QLabel* H1;
        QLabel* H2;
        QLabel* fgMsg;
        GroupWidget* fgGroup;

    public:
        /**
         * Constructor.
         */
        Dim4TriHomologyFundUI(regina::Dim4Triangulation* packet,
                PacketTabbedViewerTab* useParentUI);

        /**
         * PacketViewerTab overrides.
         */
        regina::NPacket* getPacket();
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

#endif
