
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

/*! \file ntriangulationui.h
 *  \brief Provides an interface for viewing 3-manifold triangulations.
 */

#ifndef __NTRIANGULATIONUI_H
#define __NTRIANGULATIONUI_H

#include "packet/packetlistener.h"
#include "../packettabui.h"

class ClickableLabel;
class NTriAlgebraUI;
class NTriGluingsUI;
class NTriSkeletonUI;
class NTriSurfacesUI;
class NTriSnapPeaUI;
class PacketEditIface;
class QLabel;
class QToolBar;

namespace regina {
    template <int> class Triangulation;
};

/**
 * A packet interface for viewing 3-manifold triangulations.
 */
class NTriangulationUI : public PacketTabbedUI {
    Q_OBJECT

    private:
        /**
         * Internal components
         */
        NTriGluingsUI* gluings;
        NTriSkeletonUI* skeleton;
        NTriAlgebraUI* algebra;
        NTriSurfacesUI* surfaces;
        NTriSnapPeaUI* snapPea;

        PacketEditIface* editIface;

    public:
        /**
         * Constructor and destructor.
         */
        NTriangulationUI(regina::Triangulation<3>* packet,
            PacketPane* newEnclosingPane);
        ~NTriangulationUI();

        /**
         * PacketUI overrides.
         */
        PacketEditIface* getEditIface();
        const QLinkedList<QAction*>& getPacketTypeActions();
        QString getPacketMenuText() const;
};

/**
 * A header for the 3-manifold triangulation viewer.
 */
class NTriHeaderUI : public QObject, public PacketViewerTab,
        public regina::PacketListener {
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
        QLabel* header;
        ClickableLabel* locked;
        QToolBar* bar;

    public:
        /**
         * Constructor.
         */
        NTriHeaderUI(regina::Triangulation<3>* packet,
                PacketTabbedUI* useParentUI);

        /**
         * Component queries.
         */
        QToolBar* getToolBar();

        /**
         * PacketViewerTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        void refresh();

        /**
         * PacketListener overrides.
         */
        void childWasAdded(regina::Packet* packet, regina::Packet* child);
        void childWasRemoved(regina::Packet* packet, regina::Packet* child,
            bool inParentDestructor);

        /**
         * Allow other UIs to access the summary information.
         */
        static QString summaryInfo(regina::Triangulation<3>* tri);

    public slots:
        /**
         * Explain to the user what the padlock means.
         */
        void lockedExplanation();

    protected:
        /**
         * Update the state of the padlock.
         */
        void refreshLock();

        /**
         * Allow GUI updates from a non-GUI thread.
         */
        void customEvent(QEvent* event);
};

inline PacketEditIface* NTriangulationUI::getEditIface() {
    return editIface;
}

inline QToolBar* NTriHeaderUI::getToolBar() {
    return bar;
}

#endif
