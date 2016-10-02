
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

// Regina core includes:
#include "packet/packet.h"

// UI includes:
#include "eventids.h"
#include "packetmanager.h"
#include "packettreeview.h"
#include "regevents.h"
#include "reginamain.h"

#include <QApplication>
#include <QEvent>
#include <QHeaderView>
#include <QTreeWidget>

using regina::Packet;

PacketTreeItem::PacketTreeItem(PacketTreeView* parent, Packet* realPacket) :
        QTreeWidgetItem(parent), packet(realPacket),
        shouldBeExpanded_(false) {
    init();
}

PacketTreeItem::PacketTreeItem(QTreeWidgetItem* parent, Packet* realPacket) :
        QTreeWidgetItem(parent), packet(realPacket),
        shouldBeExpanded_(false) {
    init();
}

PacketTreeItem::PacketTreeItem(QTreeWidgetItem* parent,
        QTreeWidgetItem* after, Packet* realPacket) :
        QTreeWidgetItem(parent, after), packet(realPacket),
        shouldBeExpanded_(false) {
    init();
}

void PacketTreeItem::init() {
    packet->listen(this);
    refreshLabel();
    setIcon(0, PacketManager::icon(packet));
}

void PacketTreeItem::fill() {
    PacketTreeItem* childTree = 0;
    for (Packet* p = packet->firstChild(); p; p = p->nextSibling()) {
        if (childTree)
            childTree = static_cast<PacketTreeView*>(treeWidget())->
                createAndSelect(this, childTree, p);
        else
            childTree = static_cast<PacketTreeView*>(treeWidget())->
                createAndSelect(this, p);
        childTree->fill();
    }
}

void PacketTreeItem::refreshLabel() {
    if (packet) {
        QString newLabel = packet->humanLabel().c_str();
        if (packet->hasTags())
            newLabel += " (+)";
        if (text(0) != newLabel)
            setText(0, newLabel);
    } else
        setText(0, QObject::tr("<Deleted>"));
}

void PacketTreeItem::ensureExpanded() {
    if (shouldBeExpanded_) {
        treeWidget()->expandItem(this);
        for (int i = 0; i < childCount(); ++i)
            static_cast<PacketTreeItem*>(child(i))->ensureExpanded();
    } else
        treeWidget()->collapseItem(this);
}

void PacketTreeItem::packetWasChanged(regina::Packet*) {
    getMainWindow()->setModified(true);
}

void PacketTreeItem::packetWasRenamed(regina::Packet*) {
    refreshLabel();
    getMainWindow()->setModified(true);
}

void PacketTreeItem::packetToBeDestroyed(regina::Packet*) {
    packet = 0;
    refreshLabel();
    getMainWindow()->setModified(true);

    // I'm a bit worried about this line, but I understand it will
    // behave correctly. :/
    delete this;
}

void PacketTreeItem::childWasAdded(regina::Packet*, regina::Packet*) {
    // Be careful.  We might not be in the GUI thread.
    QApplication::postEvent(treeWidget(), new PacketTreeItemEvent(
        static_cast<QEvent::Type>(EVT_TREE_CHILD_ADDED), this));
}

void PacketTreeItem::childWasRemoved(regina::Packet*, regina::Packet*,
        bool inParentDestructor) {
    // If we're in the parent destructor, it's all going to be done in
    // this->packetToBeDestroyed() anyway.
    if (! inParentDestructor) {
        refreshSubtree();
        getMainWindow()->setModified(true);
    }
}

void PacketTreeItem::childrenWereReordered(regina::Packet*) {
    refreshSubtree();
    getMainWindow()->setModified(true);
}

PacketTreeView::PacketTreeView(ReginaMain* newMainWindow, QWidget* parent) 
          : QTreeWidget(parent), mainWindow(newMainWindow),
          root(0), toSelect(0) {
    setRootIsDecorated(true);
    header()->hide();
    setAlternatingRowColors(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setExpandsOnDoubleClick(false); // since double-click opens a packet

    connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)),
        this, SLOT(handleItemExpanded(QTreeWidgetItem*)));
    connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
        this, SLOT(handleItemCollapsed(QTreeWidgetItem*)));

    // Currently we use the platform default activation method (which is
    // often double-click).  To make this single-click always, change
    // itemActivated() to itemClicked().
    connect(this, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this,
        SLOT(packetView(QTreeWidgetItem*)));
}

void PacketTreeView::fill(Packet* topPacket) {
    clear();

    root = topPacket;
    root->listen(this);

    // The root packet itself does not appear in the tree.
    for (Packet* p = root->firstChild(); p; p = p->nextSibling())
        createAndSelect(p)->fill();
}

PacketTreeItem* PacketTreeView::find(regina::Packet* packet) {
    if (! packet)
        return 0;

    // Start at the root of the tree and work down.
    // Note that the root packet itself will not be found by this routine.
    // Also, note that the invisible root item might not be a PacketTreeItem,
    // and we should not try to cast it as such.
    QTreeWidgetItem* root = invisibleRootItem();

    int itemCount = 0;
    PacketTreeItem* item;
    regina::Packet* current;
    while (itemCount < root->childCount()) {
        item = dynamic_cast<PacketTreeItem*>(root->child(itemCount++));
        current = item->getPacket();

        if (current == packet)
            return item;
        if (current && current->isGrandparentOf(packet)) {
            root = item;
            itemCount = 0;
        }
    }

    return 0;
}

void PacketTreeView::selectPacket(regina::Packet* p, bool allowDefer) {
    if (p == 0) {
        if (! selectedItems().isEmpty())
            clearSelection();
        return;
    }

    PacketTreeItem* item = find(p);
    if (item) {
        setCurrentItem(item);
    } else {
        clearSelection();
        if (allowDefer)
            toSelect = p;
    }
}

void PacketTreeView::packetView(QTreeWidgetItem* packet) {
    if (packet)
        mainWindow->packetView(
            dynamic_cast<PacketTreeItem*>(packet)->getPacket());
}

void PacketTreeView::refreshSubtree(regina::Packet* fromPacket,
        QTreeWidgetItem* fromItem) {
    // Is this a stale node in the tree?
    if (! fromPacket) {
        // Yes, it's a stale node.  Delete all of its children.
        while (fromItem->childCount())
            delete fromItem->child(0);
        return;
    }

    // No, we're looking at a real packet.

    // Before we do anything else: this routine can mess up the current
    // selection.  Remember it so we can restore it later.
    regina::Packet* selected = selectedPacket();

    // Run through the child packets and child nodes and ensure they
    // match up.
    Packet* p = fromPacket->firstChild();
    int itemCounter = 0;
    PacketTreeItem* item = static_cast<PacketTreeItem*>(
        fromItem->child(itemCounter));
    PacketTreeItem* prev = 0;
    PacketTreeItem* other;
    for ( ; p; ++itemCounter, p = p->nextSibling()) {
        // INV: itemCounter is the current index of p and item.
        if (! item) {
            // We've already run out of child nodes.  Add a new one.
            if (prev)
                prev = createAndSelect(fromItem, prev, p);
            else
                prev = createAndSelect(fromItem, 0, p);
            prev->fill();

            // Assume this has come from moving prev to a different
            // place in the packet tree: always expand.
            expandItem(fromItem); // make prev visible;
            expandItem(prev); // show prev's children.

            // Variables prev and item are already correct.
        } else if (item->getPacket() == p) {
            // They match up.
            item->refreshSubtree();

            // Update our variables.
            prev = item;
            item = static_cast<PacketTreeItem*>(
                fromItem->child(itemCounter + 1));
        } else {
            int otherCounter;
            // They both exist but they don't match up.  Hmmm.
            // Do we have a node for this packet later in the tree?
            for (otherCounter = itemCounter + 1;
                    otherCounter < fromItem->childCount();
                    ++otherCounter) {
                other = static_cast<PacketTreeItem*>(
                    fromItem->child(otherCounter));
                if (other->getPacket() == p) {
                    // We've found a node for this packet.
                    // Move it to the correct place.
                    fromItem->insertChild(itemCounter,
                        fromItem->takeChild(otherCounter));
                    other->refreshSubtree();
                    other->ensureExpanded();

                    // Update our variables.
                    // Note that item is already correct.
                    prev = other;
                    break;
                }
            }

            if (otherCounter == fromItem->childCount() ) {
                // We couldn't find a node for this packet anywhere.
                // Insert a new one.
                if (prev)
                    prev = createAndSelect(fromItem, prev, p);
                else
                    prev = createAndSelect(fromItem, 0, p);
                prev->fill();

                // Assume this has come from moving prev to a different
                // place in the packet tree: always expand.
                expandItem(fromItem); // make prev visible;
                expandItem(prev); // show prev's children.

                // Variables prev and item are already correct.
            }
        }
    }

    // Were there any child nodes left over?
    // Note that childCount() will decrease as we delete children here.
    while (itemCounter < fromItem->childCount())
        delete fromItem->child(itemCounter);

    // Restore any selection we might have had before.
    if (selected)
        selectPacket(selected);
}

PacketTreeItem* PacketTreeView::createAndSelect(regina::Packet* packet) {
    PacketTreeItem* item = new PacketTreeItem(this, packet);
    if (packet == toSelect) {
        setCurrentItem(item);
        toSelect = 0;
    }
    return item;
}

PacketTreeItem* PacketTreeView::createAndSelect(QTreeWidgetItem* parent,
        regina::Packet* packet) {
    PacketTreeItem* item = new PacketTreeItem(parent, packet);
    if (packet == toSelect) {
        setCurrentItem(item);
        toSelect = 0;
    }
    return item;
}

PacketTreeItem* PacketTreeView::createAndSelect(QTreeWidgetItem* parent,
        QTreeWidgetItem* after, regina::Packet* packet) {
    PacketTreeItem* item = new PacketTreeItem(parent, after, packet);
    if (packet == toSelect) {
        setCurrentItem(item);
        toSelect = 0;
    }
    return item;
}

void PacketTreeView::customEvent(QEvent* evt) {
    if (evt->type() == EVT_TREE_CHILD_ADDED) {
        PacketTreeItem* item = static_cast<PacketTreeItemEvent*>(evt)->
            getItem();

        if (item)
            item->refreshSubtree();
        else
            refreshFullTree();
        mainWindow->setModified(true);
    }
}

void PacketTreeView::handleItemExpanded(QTreeWidgetItem* item) {
    PacketTreeItem* p = dynamic_cast<PacketTreeItem*>(item);
    if (! p)
        return;
    p->markShouldBeExpanded(true);
}

void PacketTreeView::handleItemCollapsed(QTreeWidgetItem* item) {
    PacketTreeItem* p = dynamic_cast<PacketTreeItem*>(item);
    if (! p)
        return;
    p->markShouldBeExpanded(false);
}

void PacketTreeView::childWasAdded(regina::Packet*, regina::Packet*) {
    // Be careful.  We might not be in the GUI thread.
    QApplication::postEvent(this, new PacketTreeItemEvent(
        static_cast<QEvent::Type>(EVT_TREE_CHILD_ADDED), 0 /* root packet */));
}

void PacketTreeView::childWasRemoved(regina::Packet*, regina::Packet*, bool) {
    // We can't be in the parent destructor, since the tree itself only
    // listens on the root packet.
    refreshFullTree();
    getMainWindow()->setModified(true);
}

void PacketTreeView::childrenWereReordered(regina::Packet*) {
    refreshFullTree();
    getMainWindow()->setModified(true);
}

void PacketTreeView::mousePressEvent(QMouseEvent* event) {
    clearSelection();
    QTreeView::mousePressEvent(event);
}

