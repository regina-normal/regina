
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
#include "packetchooser.h"
#include "packetfilter.h"
#include "packetmanager.h"

#include <klocale.h>
#include <kmessagebox.h>

using regina::NPacket;

PacketChooser::PacketChooser(regina::NPacket* newSubtree,
        QWidget* parent, const char* name) :
        KComboBox(parent, name), subtree(newSubtree), filter(0) {
    fill(false, 0);
}

PacketChooser::PacketChooser(regina::NPacket* newSubtree,
        PacketFilter* newFilter, QWidget* parent, const char* name) :
        KComboBox(parent, name), subtree(newSubtree), filter(newFilter) {
    fill(false, 0);
}

PacketChooser::PacketChooser(regina::NPacket* newSubtree,
        PacketFilter* newFilter, bool allowNone,
        regina::NPacket* initialSelection, QWidget* parent, const char* name) :
        KComboBox(parent, name), subtree(newSubtree), filter(newFilter) {
    fill(allowNone, initialSelection);
}

PacketChooser::~PacketChooser() {
    if (filter)
        delete filter;
}

NPacket* PacketChooser::selectedPacket() {
    return packets[currentItem()];
}

void PacketChooser::refreshContents() {
    // Remember how it used to look.
    NPacket* remember = selectedPacket();
    bool allowNone = ((! packets.empty()) && (! packets[0]));

    // Empty the combo box.
    // Empty from the end in case it's stored as a vector deep inside.
    while (count())
        removeItem(count() - 1);
    packets.clear();

    // Fill it again.
    fill(allowNone, remember);
}

void PacketChooser::fill(bool allowNone, NPacket* select) {
    // Insert the None entry if appropriate.
    if (allowNone) {
        insertItem(i18n("<None>"));
        packets.push_back(0);

        if (select == 0)
            setCurrentItem(0);
    }

    // Insert the regular packets.
    regina::NPacket* p = subtree;
    while (p && subtree->isGrandparentOf(p)) {
        if ((! filter) || (filter->accept(p))) {
            insertItem(PacketManager::iconSmall(p),
                p->getPacketLabel().c_str());
            packets.push_back(p);
            if (p == select)
                setCurrentItem(count() - 1);
        }
        p = p->nextTreePacket();
    }
}

#include "packetchooser.moc"
