
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

// Regina core includes:
#include "packet/npacket.h"

// UI includes:
#include "eventids.h"
#include "packetmanager.h"
#include "packettreeview.h"
#include "reginapart.h"

#include "revent.h"

#include <qapplication.h>
#include <qevent.h>
#include <QHeaderView>
#include <QTreeWidget>
#include <kdebug.h>
#include <klocale.h>

using regina::NPacket;

PacketTreeItem::PacketTreeItem(PacketTreeView* parent, NPacket* realPacket) :
        QTreeWidgetItem(parent), packet(realPacket), tree(parent) {
    init();
}

PacketTreeItem::PacketTreeItem(PacketTreeItem* parent,
        NPacket* realPacket) :
        QTreeWidgetItem(parent), packet(realPacket), tree(parent->tree) {
    init();
}

PacketTreeItem::PacketTreeItem(PacketTreeView* parent,
        QTreeWidgetItem* after, NPacket* realPacket) :
        QTreeWidgetItem(parent, after), packet(realPacket),
        tree(parent) {
    init();
}

PacketTreeItem::PacketTreeItem(PacketTreeItem* parent,
        QTreeWidgetItem* after, NPacket* realPacket) :
        QTreeWidgetItem(parent, after), packet(realPacket), tree(parent->tree) {
    init();
}

void PacketTreeItem::init() {
    packet->listen(this);
    refreshLabel();
    setIcon(0, PacketManager::iconSmall(packet, true));
    isEditable = packet->isPacketEditable();
}

void PacketTreeItem::fill() {
    PacketTreeItem* childTree = 0;
    for (NPacket* p = packet->getFirstTreeChild(); p;
            p = p->getNextTreeSibling()) {
        if (childTree)
            childTree = new PacketTreeItem(this, childTree, p);
        else
            childTree = new PacketTreeItem(this, p);
        childTree->fill();
    }
}

void PacketTreeItem::refreshSubtree() {
    // Is this a stale node in the tree?
    if (! packet) {
        // Yes, it's a stale node.  Delete all of its children.
        for(int i=0; i< childCount(); i++) {
          QTreeWidgetItem* item = child(0);
          delete item;
        }
        return;
    }

    // No, we're looking at a real packet.
    // Run through the child packets and child nodes and ensure they
    // match up.
    NPacket* p = packet->getFirstTreeChild();
    int itemCounter = 0;
    PacketTreeItem* item = (PacketTreeItem*)child(itemCounter++);
    QTreeWidgetItem* prev = 0;
    QTreeWidgetItem* other;
    while (p) {
        if (! item) {
            // We've already run out of child nodes.  Add a new one.
            if (prev)
                prev = new PacketTreeItem(this, prev, p);
            else
                prev = new PacketTreeItem(this, p);
            ((PacketTreeItem*)prev)->fill();

            // Increment our variables.
            // Variables prev and item are already correct.
            p = p->getNextTreeSibling();
        } else if (item->getPacket() == p) {
            // They match up.
            item->refreshSubtree();

            // Increment our variables.
            prev = item;
            item = (PacketTreeItem*)child(itemCounter++);
            p = p->getNextTreeSibling();
        } else {
            int otherCounter;
            // They both exist but they don't match up.  Hmmm.
            // Do we have a node for this packet later in the tree?
            for (otherCounter = itemCounter; otherCounter < childCount();
                    otherCounter++) {
                other = (PacketTreeItem*)child(otherCounter);
                if (((PacketTreeItem*)other)->getPacket() == p) {
                    // We've found a node for this packet.
                    // Move it to the correct place.
                    insertChild(itemCounter-1,takeChild(otherCounter));
                    /*if (prev) {
                        insertChild(itemCounter-1,takeChild(otherCounter));
                    else {
                        insertChild(0,takeChild(itemCounter-1));
                        // Hmm, it doesn't seem easy to move a list item
                        // to the beginning of its parent's child list.
                        // other->moveItem(firstChild());
                        // firstChild()->moveItem(other);
                    }*/
                    ((PacketTreeItem*)other)->refreshSubtree();

                    // Increment our variables.
                    // Note that item is already correct.
                    prev = other;
                    p = p->getNextTreeSibling();
                    break;
                }
            }
            if (otherCounter == childCount() ) {
                // We couldn't find a node for this packet anywhere.
                // Insert a new one.
                if (prev)
                    prev = new PacketTreeItem(this, prev, p);
                else
                    prev = new PacketTreeItem(this, p);
                ((PacketTreeItem*)prev)->fill();

                // Increment our variables.
                // Note that prev and item are already correct.
                p = p->getNextTreeSibling();
            }
        }
    }

    // Were there any child nodes left over?
    //
    // Note that childCount() will decrease as we delete children (hopefully)
    while (itemCounter < childCount()) {
        other = (PacketTreeItem*)child(itemCounter);
        delete other;
    }

    /*while (item) {
        other = item;
        item = (PacketTreeItem*)(item->nextSibling());
        delete other;
    }*/
}

void PacketTreeItem::refreshLabel() {
    if (packet) {
        QString newLabel = packet->getPacketLabel().c_str(); // TODO: is packetLabel ever UTF8 or similar?
        if (packet->hasTags())
            newLabel += " (+)";
        if (text(0) != newLabel)
            setText(0, newLabel);
    } else
        setText(0, i18n("<Deleted>"));
}

void PacketTreeItem::updateEditable() {
    if (packet && packet->isPacketEditable() != isEditable) {
        // We need updating.
        isEditable = ! isEditable;
        setIcon(0, PacketManager::iconSmall(packet, true));
    }
}

void PacketTreeItem::packetWasChanged(regina::NPacket*) {
    getPart()->setModified(true);
}

void PacketTreeItem::packetWasRenamed(regina::NPacket*) {
    refreshLabel();
    getPart()->setModified(true);
}

void PacketTreeItem::packetToBeDestroyed(regina::NPacket*) {
    packet = 0;
    refreshLabel();
    getPart()->setModified(true);

    // I'm a bit worried about this line, but I understand it will
    // behave correctly. :/
    delete this;
}

void PacketTreeItem::childWasAdded(regina::NPacket*, regina::NPacket*) {
    // Be careful.  We might not be in the GUI thread.
    QApplication::postEvent(tree, new REvent(
        static_cast<QEvent::Type>(EVT_TREE_CHILD_ADDED), this));
}

void PacketTreeItem::childWasRemoved(regina::NPacket*, regina::NPacket*,
        bool inParentDestructor) {
    // If we're in the parent destructor, it's all going to be done in
    // this->packetToBeDestroyed() anyway.
    if (! inParentDestructor) {
        refreshSubtree();
        updateEditable();
        getPart()->setModified(true);
    }
}

void PacketTreeItem::childrenWereReordered(regina::NPacket*) {
    refreshSubtree();
    getPart()->setModified(true);
}

PacketTreeView::PacketTreeView(ReginaPart* newPart, QWidget* parent) 
          : QTreeWidget(parent), part(newPart) {
    //addColumn(QString::null); TODO: Do we need this?
    setRootIsDecorated(true);
    //setSorting(-1); QTreeWidgets sort by insertion by default
    header()->hide();

    connect(this, SIGNAL(executed(QListViewItem*)), this,
        SLOT(packetView(QListViewItem*)));
}

void PacketTreeView::fill(NPacket* topPacket) {
    clear();
    (new PacketTreeItem(this, topPacket))->fill();
}

PacketTreeItem* PacketTreeView::find(regina::NPacket* packet) {
    if (! packet)
        return 0;

    // Start at the root of the tree and work down.
    regina::NPacket* current;
    int itemCount=0;
    PacketTreeItem* root = dynamic_cast<PacketTreeItem*>(invisibleRootItem());
    PacketTreeItem* item = dynamic_cast<PacketTreeItem*>( root->child(itemCount++));
    while (itemCount < root->childCount()) {
        current = item->getPacket();

        if (current == packet)
            return item;
        if (current && current->isGrandparentOf(packet)) {
            root = item;
            itemCount=0;
            item = dynamic_cast<PacketTreeItem*>(root->child(itemCount++));
        }
        else {
            item = dynamic_cast<PacketTreeItem*>(root->child(itemCount++));
        }
    }

    return 0;
}

void PacketTreeView::packetView(QTreeWidgetItem* packet) {
    if (packet)
        part->packetView(dynamic_cast<PacketTreeItem*>(packet)->getPacket());
}

void PacketTreeView::refresh(NPacket* topPacket) {
    if (invisibleRootItem()->childCount() != 1)
        fill(topPacket);
    else if (((PacketTreeItem*)invisibleRootItem()->child(0))->getPacket() != topPacket)
        fill(topPacket);
    else
        ((PacketTreeItem*)invisibleRootItem()->child(0))->refreshSubtree();
}
// TODO: Subclass QEvent for this custom event.
void PacketTreeView::customEvent(QEvent* evt) {
    switch (evt->type()) {
        case EVT_TREE_CHILD_ADDED:
            {
                REvent* revt = (REvent *)evt;
                PacketTreeItem* item =
                    static_cast<PacketTreeItem*>(revt->getItem());

                item->refreshSubtree();
                item->updateEditable();
                part->setModified(true);
            }
            break;
        default:
            break;
    }
}

