
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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

// Regina core includes:
#include "packet/npacket.h"

// UI includes:
#include "packetmanager.h"
#include "packettreeview.h"
#include "reginapart.h"

#include <qapplication.h>
#include <qevent.h>
#include <qheader.h>
#include <kdebug.h>
#include <klocale.h>

// Custom event types.
// Note that QEvent::User is 1000.
#define EVT_REFRESH_ICON 2000

using regina::NPacket;

PacketTreeItem::PacketTreeItem(PacketTreeView* parent, NPacket* realPacket) :
        KListViewItem(parent), packet(realPacket), tree(parent) {
    init();
}

PacketTreeItem::PacketTreeItem(PacketTreeItem* parent,
        NPacket* realPacket) :
        KListViewItem(parent), packet(realPacket), tree(parent->tree) {
    init();
}

PacketTreeItem::PacketTreeItem(PacketTreeView* parent,
        QListViewItem* after, NPacket* realPacket) :
        KListViewItem(parent, after), packet(realPacket),
        tree(parent) {
    init();
}

PacketTreeItem::PacketTreeItem(PacketTreeItem* parent,
        QListViewItem* after, NPacket* realPacket) :
        KListViewItem(parent, after), packet(realPacket), tree(parent->tree) {
    init();
}

void PacketTreeItem::init() {
    packet->listen(this);
    refreshLabel();
    setPixmap(0, PacketManager::iconSmall(packet, true));
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
        QListViewItem* item = firstChild();
        QListViewItem* old;
        while (item) {
            old = item;
            item = item->nextSibling();
            delete old;
        }
        return;
    }

    // No, we're looking at a real packet.
    // Run through the child packets and child nodes and ensure they
    // match up.
    NPacket* p = packet->getFirstTreeChild();
    PacketTreeItem* item = (PacketTreeItem*)firstChild();
    QListViewItem* prev = 0;
    QListViewItem* other;
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
            item = (PacketTreeItem*)(item->nextSibling());
            p = p->getNextTreeSibling();
        } else {
            // They both exist but they don't match up.  Hmmm.
            // Do we have a node for this packet later in the tree?
            for (other = item->nextSibling(); other;
                    other = other->nextSibling())
                if (((PacketTreeItem*)other)->getPacket() == p) {
                    // We've found a node for this packet.
                    // Move it to the correct place.
                    if (prev)
                        other->moveItem(prev);
                    else {
                        // Hmm, it doesn't seem easy to move a list item
                        // to the beginning of its parent's child list.
                        other->moveItem(firstChild());
                        firstChild()->moveItem(other);
                    }
                    ((PacketTreeItem*)other)->refreshSubtree();

                    // Increment our variables.
                    // Note that item is already correct.
                    prev = other;
                    p = p->getNextTreeSibling();
                    break;
                }

            if (! other) {
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
    while (item) {
        other = item;
        item = (PacketTreeItem*)(item->nextSibling());
        delete other;
    }
}

void PacketTreeItem::refreshLabel() {
    if (packet) {
        std::string newLabel = packet->getPacketLabel();
        if (packet->hasTags())
            newLabel += " (+)";
        if (text(0) != newLabel.c_str())
            setText(0, newLabel.c_str());
    } else
        setText(0, i18n("<Deleted>"));
}

void PacketTreeItem::refreshIcon() {
    if (packet)
        setPixmap(0, PacketManager::iconSmall(packet, true));
}

void PacketTreeItem::updateEditable() {
    if (packet && packet->isPacketEditable() != isEditable) {
        // We need updating.
        isEditable = ! isEditable;
        QApplication::postEvent(tree, new QCustomEvent(
            static_cast<QEvent::Type>(EVT_REFRESH_ICON), this));
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
    refreshSubtree();
    updateEditable();
    getPart()->setModified(true);
}

void PacketTreeItem::childWasRemoved(regina::NPacket*, regina::NPacket*,
        bool inParentDestructor) {
    refreshSubtree();
    if (! inParentDestructor)
        updateEditable();
    getPart()->setModified(true);
}

void PacketTreeItem::childrenWereReordered(regina::NPacket*) {
    refreshSubtree();
    getPart()->setModified(true);
}

PacketTreeView::PacketTreeView(ReginaPart* newPart, QWidget* parent,
        const char* name) : KListView(parent, name), part(newPart) {
    addColumn(QString::null);
    setRootIsDecorated(true);
    setSorting(-1);
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
    PacketTreeItem* item = dynamic_cast<PacketTreeItem*>(firstChild());
    while (item) {
        current = item->getPacket();

        if (current == packet)
            return item;
        else if (current && current->isGrandparentOf(packet))
            item = dynamic_cast<PacketTreeItem*>(item->firstChild());
        else
            item = dynamic_cast<PacketTreeItem*>(item->nextSibling());
    }

    return 0;
}

void PacketTreeView::packetView(QListViewItem* packet) {
    if (packet)
        part->packetView(dynamic_cast<PacketTreeItem*>(packet)->getPacket());
}

void PacketTreeView::refresh(NPacket* topPacket) {
    if (childCount() != 1)
        fill(topPacket);
    else if (((PacketTreeItem*)firstChild())->getPacket() != topPacket)
        fill(topPacket);
    else
        ((PacketTreeItem*)firstChild())->refreshSubtree();
}

void PacketTreeView::customEvent(QCustomEvent* evt) {
    if (evt->type() == EVT_REFRESH_ICON)
        static_cast<PacketTreeItem*>(evt->data())->refreshIcon();
}

#include "packettreeview.moc"
