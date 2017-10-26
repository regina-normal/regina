
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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

/*! \file linkcrossings.h
 *  \brief Provides a crossings viewer for knots and links.
 */

#ifndef __LINKCROSSINGS_H
#define __LINKCROSSINGS_H

#include "../packettabui.h"
#include "reginaprefset.h"

class EditTableView;
class QToolBar;

namespace regina {
    class Link;
    class Packet;
};

/**
 * A knot/link page for viewing and switching crossings.
 */
class LinkCrossingsUI : public QObject, public PacketEditorTab {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::Link* link;

        /**
         * Internal components
         */
        QWidget* ui;

        /**
         * Gluing actions
         */
        QAction* actReflect;
        QAction* actSimplify;
        QAction* actComplement;
        QLinkedList<QAction*> actionList;
        QLinkedList<QAction*> enableWhenWritable;

    public:
        /**
         * Constructor and destructor.
         */
        LinkCrossingsUI(regina::Link* packet,
            PacketTabbedUI* useParentUI, bool readWrite);

        /**
         * Fill the given toolbar with knot/link actions.
         *
         * This is necessary since the toolbar will not be a part of
         * this page, but this page (as the editor) keeps track of the
         * available actions.
         */
        void fillToolBar(QToolBar* bar);

        /**
         * PacketEditorTab overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        const QLinkedList<QAction*>& getPacketTypeActions();
        void refresh();
        void setReadWrite(bool readWrite);

    public slots:
        /**
         * Knot/link actions.
         */
        void simplify();
        void reflect();
        void rotate();
        void moves();
        void complement();
};

#endif
