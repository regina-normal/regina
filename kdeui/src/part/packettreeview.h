
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  \brief Provides a visual representation of a Regina packet tree.
 */

#ifndef __PACKETTREEVIEW_H
#define __PACKETTREEVIEW_H

#include "packet/npacketlistener.h"

#include <klistview.h>

class ReginaPart;

namespace regina {
    class NPacket;
};

/**
 * A single item in a Regina packet tree.
 */
class PacketTreeItem : public KListViewItem, public regina::NPacketListener {
    private:
        /**
         * The underlying packet, or 0 if the underlying packet has
         * already been destroyed.
         */
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

        /**
         * Returns the underlying packet.
         */
        regina::NPacket* getPacket();

        /**
         * Fills this item with a subtree of items corresponding to the
         * underlying packet subtree.
         *
         * \warning Any existing children of this item will not be
         * deleted!  This routine must only be done when the subtree is
         * being initially filled.
         */
        void fill();
        /**
         * Updates the subtree descending from this item to match the
         * corresponding subtree in the underlying packet tree.
         *
         * Note that this routine is safe even if some currently
         * existing tree items correspond to packets that have already
         * been deleted.
         */
        void refreshSubtree();
        /**
         * Updates the text of this item to match the underlying packet
         * label.
         */
        void refreshLabel();

        /**
         * NPacketListener overrides.
         */
        void packetWasRenamed(regina::NPacket* packet);
        void packetToBeDestroyed(regina::NPacket* packet);
        void childWasAdded(regina::NPacket* packet, regina::NPacket* child);
        void childWasRemoved(regina::NPacket* packet, regina::NPacket* child);
        void childrenWereReordered(regina::NPacket* packet);

    private:
        /**
         * Initialises the appearance of this item.
         */
        void init();
};

/**
 * A visual representation of an entire Regina packet tree.
 *
 * This tree must be filled only with items of type PacketTreeItem.
 */
class PacketTreeView : public KListView {
    Q_OBJECT

    private:
        ReginaPart* part;
            /**< The KPart responsible for this packet tree. */

    public:
        /**
         * Creates an empty tree.  This tree must be initialised using
         * fill().
         */
        PacketTreeView(ReginaPart* newPart, QWidget* parent = 0,
            const char* name = 0);

        /**
         * Returns the currently selected packet, or 0 if no packet is
         * selected.
         */
        regina::NPacket* selectedPacket();

        /**
         * Fills this tree with items corresponding to the given
         * packet tree.  Any existing items in this tree will be removed.
         */
        void fill(regina::NPacket* topPacket);

        /**
         * Finds the item corresponding to the given packet, or 0 if no
         * such item could be found.
         */
        PacketTreeItem* find(regina::NPacket* packet);

    public slots:
        /**
         * View or edit the packet corresponding to the given list item.
         */
        void packetView(QListViewItem* packet);

        /**
         * Updates this tree to match the given packet tree.  The final
         * result should be the same as for fill(), but if the tree is
         * already mostly complete then it should be somewhat faster.
         *
         * Note that this routine is safe even if some currently
         * existing tree items correspond to packets that have already
         * been deleted.
         */
        void refresh(regina::NPacket* topPacket);
};

inline regina::NPacket* PacketTreeItem::getPacket() {
    return packet;
}

inline regina::NPacket* PacketTreeView::selectedPacket() {
    QListViewItem* item = selectedItem();
    return (item ? dynamic_cast<PacketTreeItem*>(item)->getPacket() : 0);
}

#endif
