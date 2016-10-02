
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

/*! \file packettreeview.h
 *  \brief Provides a visual representation of a Regina packet tree.
 */

#ifndef __PACKETTREEVIEW_H
#define __PACKETTREEVIEW_H

#include "packet/packetlistener.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>


class PacketTreeView;
class ReginaMain;

namespace regina {
    class Packet;
};

/**
 * A single item in a Regina packet tree.
 */
class PacketTreeItem : public QTreeWidgetItem, public regina::PacketListener {
    private:
        /**
         * The underlying packet, or 0 if the underlying packet has
         * already been destroyed.
         */
        regina::Packet* packet;

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
        PacketTreeItem(PacketTreeView* parent, regina::Packet* realPacket);
        PacketTreeItem(QTreeWidgetItem* parent, regina::Packet* realPacket);
        PacketTreeItem(QTreeWidgetItem* parent, QTreeWidgetItem* after,
                regina::Packet* realPacket);

        /**
         * Returns the underlying packet.
         */
        regina::Packet* getPacket();

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
         * PacketListener overrides.
         */
        void packetWasChanged(regina::Packet* packet);
        void packetWasRenamed(regina::Packet* packet);
        void packetToBeDestroyed(regina::Packet* packet);
        void childWasAdded(regina::Packet* packet, regina::Packet* child);
        void childWasRemoved(regina::Packet* packet, regina::Packet* child,
            bool inParentDestructor);
        void childrenWereReordered(regina::Packet* packet);

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
 * The item corresponding to the root of the packet tree will \e not be
 * included in the tree.
 */
class PacketTreeView : public QTreeWidget, public regina::PacketListener {
    Q_OBJECT

    private:
        ReginaMain* mainWindow;
            /**< The main window responsible for this packet tree. */

        regina::Packet* root;
            /**< The root of the packet tree. */

        regina::Packet* toSelect;
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
        regina::Packet* selectedPacket();

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
        void selectPacket(regina::Packet* p, bool allowDefer = false);

        /**
         * Fills this tree with items corresponding to the given
         * packet tree.  Any existing items in this tree will be removed.
         */
        void fill(regina::Packet* topPacket);

        /**
         * Finds the item corresponding to the given packet, or 0 if no
         * such item could be found.
         *
         * This routine will \e not find the root of the packet tree:
         * if packet is the tree root then this routine will return 0.
         */
        PacketTreeItem* find(regina::Packet* packet);

        /**
         * Return the main window responsible for this packet tree.
         */
        ReginaMain* getMainWindow();

        /**
         * Create a new packet item in this tree.  These routines handle
         * matters such as automatic packet selection correctly, and
         * should be used instead of the PacketTreeItem* constructors.
         */
        PacketTreeItem* createAndSelect(regina::Packet* packet);
        PacketTreeItem* createAndSelect(QTreeWidgetItem* parent,
            regina::Packet* packet);
        PacketTreeItem* createAndSelect(QTreeWidgetItem* parent,
            QTreeWidgetItem* after, regina::Packet* packet);

        /**
         * Updates this tree to match the underlying packet tree.
         *
         * Note that this routine is safe even if some currently
         * existing tree items correspond to packets that have already
         * been deleted.
         */
        void refreshFullTree();
        /**
         * Updates a subtree of this tree to match the underlying packet
         * subtree.
         *
         * PRE: The given packet and the given subtree node correspond.
         *
         * The given packet may be 0 (in which case the entire subtree will
         * be removed, though the subtree root fromItem itself will remain).
         * The given subtree may not be 0.  It is legitimate to pass the
         * invisible root item of the tree (in which case fromPacket must
         * be the tree root).
         */
        void refreshSubtree(regina::Packet* fromPacket,
            QTreeWidgetItem* fromItem);

        /**
         * PacketListener overrides.
         */
        void childWasAdded(regina::Packet* packet, regina::Packet* child);
        void childWasRemoved(regina::Packet* packet, regina::Packet* child,
            bool inParentDestructor);
        void childrenWereReordered(regina::Packet* packet);

    public slots:
        /**
         * View or edit the packet corresponding to the given list item.
         */
        void packetView(QTreeWidgetItem* packet);

    protected:
        /**
         * Allow GUI updates from within a non-GUI thread.
         */
        void customEvent(QEvent* evt);

    private:
        /**
         * Allow the user to deselect the current item by clicking on an
         * empty part of the tree.
         */
        virtual void mousePressEvent(QMouseEvent* event);

    private slots:
        /**
         * Manual management of expansion states.
         */
        void handleItemExpanded(QTreeWidgetItem* item);
        void handleItemCollapsed(QTreeWidgetItem* item);
};

inline regina::Packet* PacketTreeItem::getPacket() {
    return packet;
}

inline ReginaMain* PacketTreeItem::getMainWindow() {
    return static_cast<PacketTreeView*>(treeWidget())->getMainWindow();
}

inline bool PacketTreeItem::shouldBeExpanded() const {
    return shouldBeExpanded_;
}

inline void PacketTreeItem::markShouldBeExpanded(bool state) {
    shouldBeExpanded_ = state;
}

inline void PacketTreeItem::refreshSubtree() {
    static_cast<PacketTreeView*>(treeWidget())->refreshSubtree(packet, this);
}

inline regina::Packet* PacketTreeView::selectedPacket() {
    if (selectedItems().isEmpty())
        return 0;
    QTreeWidgetItem* item = selectedItems().first();
    return (item ? dynamic_cast<PacketTreeItem*>(item)->getPacket() : 0);
}

inline ReginaMain* PacketTreeView::getMainWindow() {
    return mainWindow;
}

inline void PacketTreeView::refreshFullTree() {
    refreshSubtree(root, invisibleRootItem());
}

#endif
