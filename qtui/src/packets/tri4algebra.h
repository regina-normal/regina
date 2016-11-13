
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

/*! \file tri4algebra.h
 *  \brief Provides an algebra viewer for 4-manifold triangulations.
 */

#ifndef __TRI4ALGEBRA_H
#define __TRI4ALGEBRA_H

#include "triangulation/forward.h"
#include "../packettabui.h"

class GroupWidget;
class QLabel;

namespace regina {
    class Packet;
};

/**
 * A triangulation page for viewing algebraic properties.
 */
class Tri4AlgebraUI : public PacketTabbedViewerTab {
    public:
        /**
         * Constructor.
         */
        Tri4AlgebraUI(regina::Triangulation<4>* packet,
                PacketTabbedUI* useParentUI);
};

/**
 * A triangulation page for viewing homology and the fundamental group.
 */
class Tri4HomologyFundUI : public QObject, public PacketViewerTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Triangulation<4>* tri;

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
        Tri4HomologyFundUI(regina::Triangulation<4>* packet,
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

#endif
