
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2002, Ben Burton                                   *
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

/*! \file packettreeview.h
 *  \brief Provides the Regina part that does all the real work.
 */

#ifndef __PACKETTREEVIEW_H
#define __PACKETTREEVIEW_H

#include <klistview.h>

namespace regina {
    class NPacket;
};

class PacketTreeItem : public KListViewItem {
    private:
        regina::NPacket* packet;

    public:
        /**
         * Constructors and destructors.
         */
        PacketTreeItem(QListView* parent, regina::NPacket* realPacket);
        PacketTreeItem(QListViewItem* parent, regina::NPacket* realPacket);
        PacketTreeItem(QListView* parent, QListViewItem* after,
                regina::NPacket* realPacket);
        PacketTreeItem(QListViewItem* parent, QListViewItem* after,
                regina::NPacket* realPacket);

        regina::NPacket* getPacket() {
            return packet;
        }

        virtual QString text(int) const;

        void fill();
        void refresh();

    private:
        void init();
};

/**
 * The Regina topology data editor.
 *
 * This part does all the real work of working with Regina data files.
 */
class PacketTreeView : public KListView {
    Q_OBJECT

    public:
        PacketTreeView(QWidget* parent = 0, const char* name = 0);

        void fill(regina::NPacket* topPacket);
        void refresh(regina::NPacket* topPacket);
};

#endif
