
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "packettreeview.h"
#include "reginamain.h"
#include "reginasupport.h"

void ReginaMain::moveShallow() {
    auto packet = checkPacketSelected();
    if (! packet)
        return;

    auto parent = packet->parent();
    auto grandparent = parent->parent();
    if (! grandparent) {
        ReginaSupport::info(this,
            tr("This is already a top-level packet."),
            tr("I cannot move it to a higher level in the tree."));
        return;
    }

    packet->makeOrphan();
    grandparent->insert(packet, parent);

    treeView->selectPacket(packet, true);
}

void ReginaMain::moveDeep() {
    auto packet = checkPacketSelected();
    if (! packet)
        return;

    bool down = true;
    auto newParent = packet->nextSibling();
    if (! newParent) {
        newParent = packet->prevSibling();
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
        newParent->prepend(packet);
    else
        newParent->append(packet);

    treeView->selectPacket(packet, true);
}

void ReginaMain::moveUp() {
    auto packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->prevSibling()) {
        if (! packet->nextSibling()) {
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

    packet->prevSibling()->swapWithNextSibling();
}

void ReginaMain::moveDown() {
    auto packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->nextSibling()) {
        if (! packet->prevSibling()) {
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
    auto packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->prevSibling()) {
        if (! packet->nextSibling()) {
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
    auto packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->nextSibling()) {
        if (! packet->prevSibling()) {
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
    auto packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->prevSibling()) {
        if (! packet->nextSibling()) {
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
    auto packet = checkPacketSelected();
    if (! packet)
        return;

    if (! packet->nextSibling()) {
        if (! packet->prevSibling()) {
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

