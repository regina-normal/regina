
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

#include "packet/npacket.h"

#include "packettreeview.h"

#include <qheader.h>
#include <kdebug.h>
#include <klocale.h>

using regina::NPacket;

PacketTreeItem::PacketTreeItem(QListView* parent, NPacket* realPacket) :
        KListViewItem(parent, realPacket->getFullName()), packet(realPacket) {
    init();
}

PacketTreeItem::PacketTreeItem(QListViewItem* parent,
        NPacket* realPacket) :
        KListViewItem(parent, realPacket->getFullName()), packet(realPacket) {
    init();
}

PacketTreeItem::PacketTreeItem(QListView* parent,
        QListViewItem* after, NPacket* realPacket) :
        KListViewItem(parent, after, realPacket->getFullName()),
        packet(realPacket) {
    init();
}

PacketTreeItem::PacketTreeItem(QListViewItem* parent,
        QListViewItem* after, NPacket* realPacket) :
        KListViewItem(parent, after, realPacket->getFullName()),
        packet(realPacket) {
    init();
}

QString PacketTreeItem::text(int) const {
    return packet->getFullName();
}

void PacketTreeItem::init() {
    //setPixmap(TODO);
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

void PacketTreeItem::refresh() {
    // TODO: Proper refresh.
    fill();
}

PacketTreeView::PacketTreeView(QWidget* parent, const char* name) :
        KListView(parent, name) {
    addColumn(QString::null);
    setRootIsDecorated(true);
    setSorting(-1);
    header()->hide();
}

void PacketTreeView::fill(NPacket* topPacket) {
    clear();
    (new PacketTreeItem(this, topPacket))->fill();
    triggerUpdate();
}

void PacketTreeView::refresh(NPacket* topPacket) {
    // TODO: Actually refresh, not just refill.
    fill(topPacket);
}

#include "packettreeview.moc"
