
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

// Regina core includes:
#include "packet/npacket.h"

// UI includes:
#include "packettreeview.h"
#include "reginapart.h"

#include <klocale.h>
#include <kmessagebox.h>

void ReginaPart::moveShallow() {
    if (! checkReadWrite())
        return;

    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (packet->dependsOnParent()) {
        KMessageBox::error(widget(), i18n(
            "This packet cannot be moved away from its current parent."));
        return;
    }

    regina::NPacket* parent = packet->getTreeParent();
    if (! parent) {
        KMessageBox::error(widget(), i18n(
            "This packet is already at the highest level in the "
            "entire tree."));
        return;
    }

    regina::NPacket* grandparent = parent->getTreeParent();
    if (! grandparent) {
        KMessageBox::error(widget(), i18n(
            "There can only be one packet at the highest level in the tree."));
        return;
    }

    packet->makeOrphan();
    grandparent->insertChildAfter(packet, parent);

    PacketTreeItem* item = treeView->find(packet);
    if (item) {
        treeView->setSelected(item, true);
        treeView->ensureItemVisible(item);
    }
}

void ReginaPart::moveDeep() {
    if (! checkReadWrite())
        return;

    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (packet->dependsOnParent()) {
        KMessageBox::error(widget(), i18n(
            "This packet cannot be moved away from its current parent."));
        return;
    }

    bool down = true;
    regina::NPacket* newParent = packet->getNextTreeSibling();
    if (! newParent) {
        newParent = packet->getPrevTreeSibling();
        down = false;
    }
    if (! newParent) {
        KMessageBox::error(widget(), i18n(
            "This packet cannot be moved to a lower level because it has no "
            "siblings that could act as its parent."));
        return;
    }

    packet->makeOrphan();
    if (down)
        newParent->insertChildFirst(packet);
    else
        newParent->insertChildLast(packet);

    PacketTreeItem* item = treeView->find(packet);
    if (item) {
        treeView->setSelected(item, true);
        treeView->ensureItemVisible(item);
    }
}

void ReginaPart::moveUp() {
    if (! checkReadWrite())
        return;

    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getPrevTreeSibling()) {
        if (! packet->getNextTreeSibling())
            KMessageBox::error(widget(), i18n(
                "This packet has no siblings and so cannot be moved up."));
        else
            KMessageBox::error(widget(), i18n(
                "This packet is already at the top of its list of siblings."));
        return;
    }

    packet->getPrevTreeSibling()->swapWithNextSibling();
}

void ReginaPart::moveDown() {
    if (! checkReadWrite())
        return;

    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getNextTreeSibling()) {
        if (! packet->getPrevTreeSibling())
            KMessageBox::error(widget(), i18n(
                "This packet has no siblings and so cannot be moved down."));
        else
            KMessageBox::error(widget(), i18n(
                "This packet is already at the bottom of its "
                "list of siblings."));
        return;
    }

    packet->swapWithNextSibling();
}

void ReginaPart::movePageUp() {
    if (! checkReadWrite())
        return;

    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getPrevTreeSibling()) {
        if (! packet->getNextTreeSibling())
            KMessageBox::error(widget(), i18n(
                "This packet has no siblings and so cannot be moved up."));
        else
            KMessageBox::error(widget(), i18n(
                "This packet is already at the top of its list of siblings."));
        return;
    }

    packet->moveUp(prefs.treeJumpSize);
}

void ReginaPart::movePageDown() {
    if (! checkReadWrite())
        return;

    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getNextTreeSibling()) {
        if (! packet->getPrevTreeSibling())
            KMessageBox::error(widget(), i18n(
                "This packet has no siblings and so cannot be moved down."));
        else
            KMessageBox::error(widget(), i18n(
                "This packet is already at the bottom of its "
                "list of siblings."));
        return;
    }

    packet->moveDown(prefs.treeJumpSize);
}

void ReginaPart::moveTop() {
    if (! checkReadWrite())
        return;

    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getPrevTreeSibling()) {
        if (! packet->getNextTreeSibling())
            KMessageBox::error(widget(), i18n(
                "This packet has no siblings and so cannot be moved up."));
        else
            KMessageBox::error(widget(), i18n(
                "This packet is already at the top of its list of siblings."));
        return;
    }

    packet->moveToFirst();
}

void ReginaPart::moveBottom() {
    if (! checkReadWrite())
        return;

    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getNextTreeSibling()) {
        if (! packet->getPrevTreeSibling())
            KMessageBox::error(widget(), i18n(
                "This packet has no siblings and so cannot be moved down."));
        else
            KMessageBox::error(widget(), i18n(
                "This packet is already at the bottom of its "
                "list of siblings."));
        return;
    }

    packet->moveToLast();
}

