
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

/*! \file packettreeview.h
 *  \brief Provides a visual representation of a Regina packet tree.
 */

#ifndef __PACKETTREEVIEW_H
#define __PACKETTREEVIEW_H

#include "packet/npacketlistener.h"

#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>


class PacketTreeView;
class ReginaMain;

namespace regina {
    class NPacket;
};

/**
 * A single item in a Regina packet tree.
 */
class PacketTreeItem : public QTreeWidgetItem, public regina::NPacketListener {
    private:
        /**
         * The underlying packet, or 0 if the underlying packet has
         * already been destroyed.
         */
        regina::NPacket* packet;

        /**
         * The packet tree containing this item.
         */
        PacketTreeView* tree;

        /**
         * Do we currently believe the underlying packet to be editable?
         * We refer here purely to child packet constraints; whether
         * or not the overall file is read-only is irrelevant here.
         */
        bool isEditable;

        /**
         * Since the KDE4 port, moving packets around the tree seems to
         * result in branches being closed when they were once open.
         * Manage this manually, sigh.
         */
        bool shouldBeExpanded_;

    public:
        /**
         * Constructors and destructors.
         *
         * \warning The constructors should \a only be used by internal
         * PacketTreeView routines!  When creating a new packet tree
         * item, use one of the PacketTreeView::createAndSelect() routines
         * instead.
         *
         * \todo Make these constructors private, and add PacketTreeView
         * as a friend class.
         */
        PacketTreeItem(PacketTreeView* parent, regina::NPacket* realPacket);
        PacketTreeItem(PacketTreeItem* parent, regina::NPacket* realPacket);
        PacketTreeItem(PacketTreeItem* parent, QTreeWidgetItem* after,
                regina::NPacket* realPacket);

        /**
         * Returns the underlying packet.
         */
        regina::NPacket* getPacket();

        /**
         * Return the main window responsible for this packet tree.
         */
        ReginaMain* getMainWindow();

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
         * Updates the appearance of this item to correctly reflect whether
         * or not the underlying packet is editable.  This refers purely to
         * child packet constraints; whether or not the overall file is
         * read-only is irrelevant here.
         *
         * This will only make a physical update if the editability has in
         * fact changed; otherwise it will do nothing.
         *
         * Note that this routine may only be called from within the GUI
         * thread!  Calling it from a different thread can cause an Xlib
         * crash.
         */
        void updateEditable();

        /**
         * NPacketListener overrides.
         */
        void packetWasChanged(regina::NPacket* packet);
        void packetWasRenamed(regina::NPacket* packet);
        void packetToBeDestroyed(regina::NPacket* packet);
        void childWasAdded(regina::NPacket* packet, regina::NPacket* child);
        void childWasRemoved(regina::NPacket* packet, regina::NPacket* child,
            bool inParentDestructor);
        void childrenWereReordered(regina::NPacket* packet);

        /**
         * Manual management of expansion state.
         */
        bool shouldBeExpanded() const;
        void markShouldBeExpanded(bool state);
        void ensureExpanded();

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
class PacketTreeView : public QTreeWidget {
    Q_OBJECT

    private:
        ReginaMain* mainWindow;
            /**< The main window responsible for this packet tree. */

        regina::NPacket* toSelect;
            /**< If non-zero, this is a packet that will be added to the
                 tree shortly, and which will be automatically selected
                 as soon as it appears. */

    public:
        /**
         * Creates an empty tree.  This tree must be initialised using
         * fill().
         */
        PacketTreeView(ReginaMain* newMainWindow, QWidget* parent = 0);

        /**
         * Returns the currently selected packet, or 0 if no packet is
         * selected.
         */
        regina::NPacket* selectedPacket();

        /**
         * Selects the given packet in the tree, or clears the selection
         * if 0 is passed.  If the given packet cannot be found in the
         * tree, the selection will be cleared (and nothing will break).
         *
         * If \a allowDefer is \c true and the given packet cannot be
         * found in the tree then it will be assumed that the packet will be
         * added shortly, and once the corresponding tree item does appear
         * it will be selected immediately.
         */
        void selectPacket(regina::NPacket* p, bool allowDefer = false);

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

        /**
         * Return the main window responsible for this packet tree.
         */
        ReginaMain* getMainWindow();

        /**
         * Create a new packet item in this tree.  These routines handle
         * matters such as automatic packet selection correctly, and
         * should be used instead of the PacketTreeItem* constructors.
         */
        PacketTreeItem* createAndSelect(regina::NPacket* packet);
        PacketTreeItem* createAndSelect(PacketTreeItem* parent,
            regina::NPacket* packet);
        PacketTreeItem* createAndSelect(PacketTreeItem* parent,
            QTreeWidgetItem* after, regina::NPacket* packet);

    public slots:
        /**
         * View or edit the packet corresponding to the given list item.
         */
        void packetView(QTreeWidgetItem* packet);

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

    protected:
        /**
         * Allow GUI updates from within a non-GUI thread.
         */
        void customEvent(QEvent* evt);

    private slots:
        /**
         * Manual management of expansion states.
         */
        void handleItemExpanded(QTreeWidgetItem* item);
        void handleItemCollapsed(QTreeWidgetItem* item);
};

inline regina::NPacket* PacketTreeItem::getPacket() {
    return packet;
}

inline ReginaMain* PacketTreeItem::getMainWindow() {
    return tree->getMainWindow();
}

inline bool PacketTreeItem::shouldBeExpanded() const {
    return shouldBeExpanded_;
}

inline void PacketTreeItem::markShouldBeExpanded(bool state) {
    shouldBeExpanded_ = state;
}

inline regina::NPacket* PacketTreeView::selectedPacket() {
    if (selectedItems().isEmpty())
        return 0;
    QTreeWidgetItem* item = selectedItems().first();
    return (item ? dynamic_cast<PacketTreeItem*>(item)->getPacket() : 0);
}

inline ReginaMain* PacketTreeView::getMainWindow() {
    return mainWindow;
}

#endif
