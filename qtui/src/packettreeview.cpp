
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

PacketTreeItem::PacketTreeItem(PacketTreeView* parent, Packet& realPacket) :
        QTreeWidgetItem(parent), packet(realPacket),
        shouldBeExpanded_(false) {
    init();
}

PacketTreeItem::PacketTreeItem(QTreeWidgetItem* parent, Packet& realPacket) :
        QTreeWidgetItem(parent), packet(realPacket),
        shouldBeExpanded_(false) {
    init();
}

PacketTreeItem::PacketTreeItem(QTreeWidgetItem* parent,
        QTreeWidgetItem* after, Packet& realPacket) :
        QTreeWidgetItem(parent, after), packet(realPacket),
        shouldBeExpanded_(false) {
    init();
}

void PacketTreeItem::init() {
    packet.listen(this);
    refreshLabel();
    setIcon(0, PacketManager::icon(packet));
}

void PacketTreeItem::fill() {
    PacketTreeItem* childTree = nullptr;
    for (auto p = packet.firstChild(); p; p = p->nextSibling()) {
        if (childTree)
            childTree = static_cast<PacketTreeView*>(treeWidget())->
                createAndSelect(this, childTree, *p);
        else
            childTree = static_cast<PacketTreeView*>(treeWidget())->
                createAndSelect(this, *p);
        childTree->fill();
    }
}

void PacketTreeItem::refreshLabel() {
    QString newLabel = packet.humanLabel().c_str();
    if (packet.hasTags())
        newLabel += " (+)";
    if (text(0) != newLabel)
        setText(0, newLabel);
}

void PacketTreeItem::ensureExpanded() {
    if (shouldBeExpanded_) {
        treeWidget()->expandItem(this);
        for (int i = 0; i < childCount(); ++i)
            static_cast<PacketTreeItem*>(child(i))->ensureExpanded();
    } else
        treeWidget()->collapseItem(this);
}

void PacketTreeItem::packetWasChanged(regina::Packet&) {
    getMainWindow()->setModified(true);
}

void PacketTreeItem::packetWasRenamed(regina::Packet&) {
    refreshLabel();
    getMainWindow()->setModified(true);
}

void PacketTreeItem::packetBeingDestroyed(regina::PacketShell) {
    getMainWindow()->setModified(true);

    // I'm a bit worried about this line, but I understand it will
    // behave correctly. :/
    delete this;
}

void PacketTreeItem::childWasAdded(regina::Packet&, regina::Packet&) {
    // Be careful.  We might not be in the GUI thread.
    QApplication::postEvent(treeWidget(), new PacketTreeItemEvent(
        static_cast<QEvent::Type>(EVT_TREE_CHILD_ADDED), this));
}

void PacketTreeItem::childWasRemoved(regina::Packet&, regina::Packet&) {
    refreshSubtree();
    getMainWindow()->setModified(true);
}

void PacketTreeItem::childrenWereReordered(regina::Packet&) {
    refreshSubtree();
    getMainWindow()->setModified(true);
}

PacketTreeView::PacketTreeView(ReginaMain* newMainWindow, QWidget* parent) 
          : QTreeWidget(parent), mainWindow(newMainWindow),
          root(nullptr), toSelect(nullptr) {
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

void PacketTreeView::fill(std::shared_ptr<Packet> topPacket) {
    clear();

    root = std::move(topPacket);
    root->listen(this);

    // The root packet itself does not appear in the tree.
    for (auto p = root->firstChild(); p; p = p->nextSibling())
        createAndSelect(*p)->fill();
}

PacketTreeItem* PacketTreeView::find(const Packet& packet) {
    // Start at the root of the tree and work down.
    // Note that the root packet itself will not be found by this routine.
    // Also, note that the invisible root item might not be a PacketTreeItem,
    // and we should not try to cast it as such.
    QTreeWidgetItem* rootItem = invisibleRootItem();

    int itemCount = 0;
    PacketTreeItem* item;
    regina::Packet* current;
    while (itemCount < rootItem->childCount()) {
        item = static_cast<PacketTreeItem*>(rootItem->child(itemCount++));
        current = item->getPacket();

        if (current == std::addressof(packet))
            return item;
        if (current && current->isAncestorOf(packet)) {
            rootItem = item;
            itemCount = 0;
        }
    }

    return nullptr;
}

void PacketTreeView::selectPacket(std::shared_ptr<regina::Packet> p,
        bool allowDefer) {
    if (! p) {
        if (! selectedItems().isEmpty())
            clearSelection();
        return;
    }

    PacketTreeItem* item = find(*p);
    if (item) {
        setCurrentItem(item);
    } else {
        clearSelection();
        if (allowDefer)
            toSelect = p.get();
    }
}

void PacketTreeView::packetView(QTreeWidgetItem* packet) {
    if (packet)
        mainWindow->packetView(*(static_cast<PacketTreeItem*>(packet)->
            getPacket()));
}

void PacketTreeView::refreshSubtree(const Packet& fromPacket,
        QTreeWidgetItem* fromItem) {
    // Before we do anything else: this routine can mess up the current
    // selection.  Remember it so we can restore it later.
    std::shared_ptr<Packet> selected = selectedPacket();

    // Run through the child packets and child nodes and ensure they
    // match up.
    std::shared_ptr<Packet> p = fromPacket.firstChild();
    int itemCounter = 0;
    auto* item = static_cast<PacketTreeItem*>(fromItem->child(itemCounter));
    PacketTreeItem* prev = nullptr;
    PacketTreeItem* other;
    for ( ; p; ++itemCounter, p = p->nextSibling()) {
        // INV: itemCounter is the current index of p and item.
        if (! item) {
            // We've already run out of child nodes.  Add a new one.
            if (prev)
                prev = createAndSelect(fromItem, prev, *p);
            else
                prev = createAndSelect(fromItem, nullptr, *p);
            prev->fill();

            // Assume this has come from moving prev to a different
            // place in the packet tree: always expand.
            expandItem(fromItem); // make prev visible;
            expandItem(prev); // show prev's children.

            // Variables prev and item are already correct.
        } else if (item->getPacket() == p.get()) {
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
                if (other->getPacket() == p.get()) {
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
                    prev = createAndSelect(fromItem, prev, *p);
                else
                    prev = createAndSelect(fromItem, nullptr, *p);
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

PacketTreeItem* PacketTreeView::createAndSelect(Packet& packet) {
    auto* item = new PacketTreeItem(this, packet);
    if (std::addressof(packet) == toSelect) {
        setCurrentItem(item);
        toSelect = nullptr;
    }
    return item;
}

PacketTreeItem* PacketTreeView::createAndSelect(QTreeWidgetItem* parent,
        Packet& packet) {
    auto* item = new PacketTreeItem(parent, packet);
    if (std::addressof(packet) == toSelect) {
        setCurrentItem(item);
        toSelect = nullptr;
    }
    return item;
}

PacketTreeItem* PacketTreeView::createAndSelect(QTreeWidgetItem* parent,
        QTreeWidgetItem* after, Packet& packet) {
    auto* item = new PacketTreeItem(parent, after, packet);
    if (std::addressof(packet) == toSelect) {
        setCurrentItem(item);
        toSelect = nullptr;
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
    if (auto p = dynamic_cast<PacketTreeItem*>(item))
        p->markShouldBeExpanded(true);
}

void PacketTreeView::handleItemCollapsed(QTreeWidgetItem* item) {
    if (auto p = dynamic_cast<PacketTreeItem*>(item))
        p->markShouldBeExpanded(false);
}

void PacketTreeView::childWasAdded(regina::Packet&, regina::Packet&) {
    // Be careful.  We might not be in the GUI thread.
    QApplication::postEvent(this, new PacketTreeItemEvent(
        static_cast<QEvent::Type>(EVT_TREE_CHILD_ADDED), nullptr /* root */));
}

void PacketTreeView::childWasRemoved(regina::Packet&, regina::Packet&) {
    refreshFullTree();
    getMainWindow()->setModified(true);
}

void PacketTreeView::childrenWereReordered(regina::Packet&) {
    refreshFullTree();
    getMainWindow()->setModified(true);
}

void PacketTreeView::mousePressEvent(QMouseEvent* event) {
    clearSelection();
    QTreeView::mousePressEvent(event);
}

