
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

// Regina core includes:
#include "packet/npacket.h"

// UI includes:
#include "packettreeview.h"
#include "reginapart.h"

#include <QMessageBox>

void ReginaPart::moveShallow() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (packet->dependsOnParent()) {
      QMessageBox::warning(widget(), tr("Cannot move packet"),
        tr( "This packet cannot be moved away from its current parent."));
        return;
    }

    regina::NPacket* parent = packet->getTreeParent();
    if (! parent) {
        QMessageBox::warning(widget(), tr("Cannot move packet"),
            tr("This packet is already at the highest level in the "
            "entire tree."));
        return;
    }

    regina::NPacket* grandparent = parent->getTreeParent();
    if (! grandparent) {
        QMessageBox::warning(widget(), tr("Cannot move packet"),
            tr("There can only be one packet at "
            "the highest level in the tree."));
        return;
    }

    packet->makeOrphan();
    grandparent->insertChildAfter(packet, parent);

    treeView->selectPacket(packet, true);
}

void ReginaPart::moveDeep() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (packet->dependsOnParent()) {
        QMessageBox::warning(widget(), tr("Cannot move packet"),
            tr("This packet cannot be moved away from its current parent."));
        return;
    }

    bool down = true;
    regina::NPacket* newParent = packet->getNextTreeSibling();
    if (! newParent) {
        newParent = packet->getPrevTreeSibling();
        down = false;
    }
    if (! newParent) {
        QMessageBox::warning(widget(), tr("Cannot move packet"),
            tr("This packet cannot be moved to a lower level because it "
            "has no siblings that could act as its parent."));
        return;
    }

    packet->makeOrphan();
    if (down)
        newParent->insertChildFirst(packet);
    else
        newParent->insertChildLast(packet);

    treeView->selectPacket(packet, true);
}

void ReginaPart::moveUp() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getPrevTreeSibling()) {
        if (! packet->getNextTreeSibling()) {
            QMessageBox::warning(widget(), tr("Cannot move packet"),
                tr("This packet has no siblings and so cannot be moved up."));
        } else {
            QMessageBox::warning(widget(), tr("Cannot move packet"),
                tr("This packet is already at the top of its list "
                "of siblings."));
        }
        return;
    }

    packet->getPrevTreeSibling()->swapWithNextSibling();
}

void ReginaPart::moveDown() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getNextTreeSibling()) {
        if (! packet->getPrevTreeSibling()) {
            QMessageBox::warning(widget(), tr("Cannot move packet"),
                tr("This packet has no siblings and so cannot be moved down."));
        } else {
            QMessageBox::warning(widget(), tr("Cannot move packet"),
                tr("This packet is already at the bottom of its "
                "list of siblings."));
        }
        return;
    }

    packet->swapWithNextSibling();
}

void ReginaPart::movePageUp() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getPrevTreeSibling()) {
        if (! packet->getNextTreeSibling()) {
            QMessageBox::warning(widget(), tr("Cannot move packet"),
                tr("This packet has no siblings and so cannot be moved up."));
        } else {
            QMessageBox::warning(widget(), tr("Cannot move packet"),
                tr("This packet is already at the top of its list "
                "of siblings."));
        }
        return;
    }

    packet->moveUp(ReginaPrefSet::global().treeJumpSize);
}

void ReginaPart::movePageDown() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getNextTreeSibling()) {
        if (! packet->getPrevTreeSibling()) {
            QMessageBox::warning(widget(), tr("Cannot move packet"),
                tr("This packet has no siblings and so cannot "
                "be moved down."));
        } else {
            QMessageBox::warning(widget(), tr("Cannot move packet"),
                tr("This packet is already at the bottom of its "
                "list of siblings."));
        }
        return;
    }

    packet->moveDown(ReginaPrefSet::global().treeJumpSize);
}

void ReginaPart::moveTop() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getPrevTreeSibling()) {
        if (! packet->getNextTreeSibling()) {
            QMessageBox::warning(widget(), tr("Cannot move packet"),
                tr("This packet has no siblings and so cannot be moved up."));
        } else {
            QMessageBox::warning(widget(), tr("Cannot move packet"),
                tr("This packet is already at the top of its "
                "list of siblings.")); 
        }
        return;
    }

    packet->moveToFirst();
}

void ReginaPart::moveBottom() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getNextTreeSibling()) {
        if (! packet->getPrevTreeSibling()) {
            QMessageBox::warning(widget(), tr("Cannot move packet"),
                tr("This packet has no siblings and so cannot be "
                "moved down."));
        } else {
            QMessageBox::warning(widget(), tr("Cannot move packet"),
                tr("This packet is already at the bottom of its "
                "list of siblings."));
        }
        return;
    }

    packet->moveToLast();
}

