
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

/*! \file nsnappeagluings.h
 *  \brief Provides a face gluing viewer for SnapPea triangulations.
 */

#ifndef __NSNAPPEAGLUINGS_H
#define __NSNAPPEAGLUINGS_H

#include "packettypes/ntrigluings.h"

namespace regina {
    class NSnapPeaTriangulation;
};

/**
 * A SnapPea triangulation page for viewing face gluings.
 */
class NSnapPeaGluingsUI : public QObject, public PacketEditorTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NSnapPeaTriangulation* tri;

        /**
         * Internal components
         */
        QWidget* ui;
        QTableView* faceTable;
        GluingsModel* model;

        /**
         * Gluing actions
         */
        QAction* actRandomise;
        QAction* actToRegina;
        QLinkedList<QAction*> triActionList;
        QLinkedList<QAction*> enableWhenWritable;
        QLinkedList<QAction*> requiresNonNull;

    public:
        /**
         * Constructor and destructor.
         */
        NSnapPeaGluingsUI(regina::NSnapPeaTriangulation* packet,
                PacketTabbedUI* useParentUI, bool readWrite);
        ~NSnapPeaGluingsUI();

        /**
         * Fill the given toolbar with triangulation actions.
         *
         * This is necessary since the toolbar will not be a part of
         * this page, but this page (as the editor) keeps track of the
         * available actions.
         */
        void fillToolBar(QToolBar* bar);

        /**
         * PacketEditorTab overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        const QLinkedList<QAction*>& getPacketTypeActions();
        void refresh();
        void commit();
        void setReadWrite(bool readWrite);

    public slots:
        /**
         * Triangulation actions.
         */
        void randomise();
        void vertexLinks();
        void toRegina();

        /**
         * Update the states of internal components.
         */
        void updateNonNullActions();
};

#endif
