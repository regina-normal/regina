
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
#include "reginamain.h"
#include "reginasupport.h"

void ReginaMain::moveShallow() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (packet->dependsOnParent()) {
        ReginaSupport::info(this,
            tr("This packet must stay with its current parent."),
            tr("This is because it relies on information stored "
            "in its parent (e.g., a normal surface list that stores "
            "coordinates relative to its parent triangulation).  "
            "If you want to move this packet to a higher level in the tree, "
            "try moving its parent instead."));
        return;
    }

    regina::NPacket* parent = packet->getTreeParent();
    if (! parent) {
        ReginaSupport::info(this,
            tr("This packet is already at the top of the tree."),
            tr("I cannot move it to a higher level."));
        return;
    }

    regina::NPacket* grandparent = parent->getTreeParent();
    if (! grandparent) {
        ReginaSupport::info(this,
            tr("The root of the packet tree must stay fixed."),
            tr("I cannot move this packet to the top level to take "
            "its place."));
        return;
    }

    packet->makeOrphan();
    grandparent->insertChildAfter(packet, parent);

    treeView->selectPacket(packet, true);
}

void ReginaMain::moveDeep() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getTreeParent()) {
        ReginaSupport::info(this,
            tr("The root of the packet tree must stay fixed."),
            tr("I cannot move it to a lower level."));
        return;
    }

    if (packet->dependsOnParent()) {
        ReginaSupport::info(this,
            tr("This packet must stay with its current parent."),
            tr("This is because it relies on information stored "
            "in its parent (e.g., a normal surface list that stores "
            "coordinates relative to its parent triangulation).  "
            "If you want to move this packet to a deeper level in the tree, "
            "try moving its parent instead."));
        return;
    }

    bool down = true;
    regina::NPacket* newParent = packet->getNextTreeSibling();
    if (! newParent) {
        newParent = packet->getPrevTreeSibling();
        down = false;
    }
    if (! newParent) {
        ReginaSupport::info(this,
            tr("This packet is an only child."),
            tr("To move it to a lower level in the tree, it must have "
            "a sibling (above or below it) that can act as its new parent."));
        return;
    }

    packet->makeOrphan();
    if (down)
        newParent->insertChildFirst(packet);
    else
        newParent->insertChildLast(packet);

    treeView->selectPacket(packet, true);
}

void ReginaMain::moveUp() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getPrevTreeSibling()) {
        if (! packet->getNextTreeSibling()) {
            ReginaSupport::info(this,
                tr("This packet is an only child."),
                tr("I cannot move it up or down, since it has no siblings."));
        } else {
            ReginaSupport::info(this,
                tr("This packet is already the first amongst its siblings."),
                tr("I cannot move it further up."));
        }
        return;
    }

    packet->getPrevTreeSibling()->swapWithNextSibling();
}

void ReginaMain::moveDown() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getNextTreeSibling()) {
        if (! packet->getPrevTreeSibling()) {
            ReginaSupport::info(this,
                tr("This packet is an only child."),
                tr("I cannot move it up or down, since it has no siblings."));
        } else {
            ReginaSupport::info(this,
                tr("This packet is already the last amongst its siblings."),
                tr("I cannot move it further down."));
        }
        return;
    }

    packet->swapWithNextSibling();
}

void ReginaMain::movePageUp() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getPrevTreeSibling()) {
        if (! packet->getNextTreeSibling()) {
            ReginaSupport::info(this,
                tr("This packet is an only child."),
                tr("I cannot move it up or down, since it has no siblings."));
        } else {
            ReginaSupport::info(this,
                tr("This packet is already the first amongst its siblings."),
                tr("I cannot move it further up."));
        }
        return;
    }

    packet->moveUp(ReginaPrefSet::global().treeJumpSize);
}

void ReginaMain::movePageDown() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getNextTreeSibling()) {
        if (! packet->getPrevTreeSibling()) {
            ReginaSupport::info(this,
                tr("This packet is an only child."),
                tr("I cannot move it up or down, since it has no siblings."));
        } else {
            ReginaSupport::info(this,
                tr("This packet is already the last amongst its siblings."),
                tr("I cannot move it further down."));
        }
        return;
    }

    packet->moveDown(ReginaPrefSet::global().treeJumpSize);
}

void ReginaMain::moveTop() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getPrevTreeSibling()) {
        if (! packet->getNextTreeSibling()) {
            ReginaSupport::info(this,
                tr("This packet is an only child."),
                tr("I cannot move it up or down, since it has no siblings."));
        } else {
            ReginaSupport::info(this,
                tr("This packet is already the first amongst its siblings."),
                tr("I cannot move it further up."));
        }
        return;
    }

    packet->moveToFirst();
}

void ReginaMain::moveBottom() {
    regina::NPacket* packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->getNextTreeSibling()) {
        if (! packet->getPrevTreeSibling()) {
            ReginaSupport::info(this,
                tr("This packet is an only child."),
                tr("I cannot move it up or down, since it has no siblings."));
        } else {
            ReginaSupport::info(this,
                tr("This packet is already the last amongst its siblings."),
                tr("I cannot move it further down."));
        }
        return;
    }

    packet->moveToLast();
}

