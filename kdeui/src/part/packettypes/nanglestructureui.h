
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2008, Ben Burton                                   *
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

/*! \file nanglestructureui.h
 *  \brief Provides an interface for viewing angle structure lists.
 */

#ifndef __NANGLESTRUCTUREUI_H
#define __NANGLESTRUCTUREUI_H

#include "../packetui.h"

#include <qtooltip.h>

class AngleHeaderToolTip;
class QHeader;
class QLabel;
class QListView;
class QVBox;

namespace regina {
    class NAngleStructureList;
    class NPacket;
};

/**
 * A packet interface for viewing angle structure lists.
 */
class NAngleStructureUI : public QObject, public PacketReadOnlyUI {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NAngleStructureList* structures;

        /**
         * Internal components
         */
        QVBox* ui;
        QLabel* stats;
        QListView* table;
        AngleHeaderToolTip* headerTips;

        /**
         * Status of any ongoing actions.
         */
        bool currentlyAutoResizing;

    public:
        /**
         * Constructor and destructor.
         */
        NAngleStructureUI(regina::NAngleStructureList* packet,
                PacketPane* newEnclosingPane);
        ~NAngleStructureUI();

        /**
         * PacketUI overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        QString getPacketMenuText() const;
        void refresh();

    public slots:
        /**
         * Provides auto-resizing of columns.
         */
        void columnResized(int section, int oldSize, int newSize);
};

/**
 * A utility class for displaying tooltips for table headers.
 */
class AngleHeaderToolTip : public QToolTip {
    public:
        /**
         * Constructor.
         */
        AngleHeaderToolTip(QHeader *header, QToolTipGroup *group = 0);

    protected:
        /**
         * QToolTip overrides.
         */
        void maybeTip(const QPoint& p);
};

#endif
