
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
#include "packetchooser.h"
#include "packetfilter.h"
#include "packetmanager.h"

#include <algorithm>
#include <klocale.h>
#include <kmessagebox.h>

using regina::NPacket;

PacketChooser::PacketChooser(regina::NPacket* newSubtree,
        QWidget* parent, const char* name) :
        KComboBox(parent, name), subtree(newSubtree), filter(0),
        onAutoUpdate(false) {
    fill(false, 0);
}

PacketChooser::PacketChooser(regina::NPacket* newSubtree,
        PacketFilter* newFilter, QWidget* parent, const char* name) :
        KComboBox(parent, name), subtree(newSubtree), filter(newFilter),
        onAutoUpdate(false) {
    fill(false, 0);
}

PacketChooser::PacketChooser(regina::NPacket* newSubtree,
        PacketFilter* newFilter, bool allowNone,
        regina::NPacket* initialSelection, QWidget* parent, const char* name) :
        KComboBox(parent, name), subtree(newSubtree), filter(newFilter),
        onAutoUpdate(false) {
    fill(allowNone, initialSelection);
}

PacketChooser::~PacketChooser() {
    if (filter)
        delete filter;
}

NPacket* PacketChooser::selectedPacket() {
    if (count() == 0)
        return 0;
    else
        return packets[currentItem()];
}

void PacketChooser::setAutoUpdate(bool shouldAutoUpdate) {
    if (onAutoUpdate == shouldAutoUpdate)
        return;

    onAutoUpdate = shouldAutoUpdate;
    if (onAutoUpdate)
        for (std::vector<regina::NPacket*>::iterator it = packets.begin();
                it != packets.end(); it++)
            if (*it)
                (*it)->listen(this);
    else
        unregisterFromAllPackets();
}

void PacketChooser::packetWasRenamed(regina::NPacket* renamed) {
    // Just rename the item that was changed.
    std::vector<regina::NPacket*>::iterator it = std::find(
        packets.begin(), packets.end(), renamed);

    if (it != packets.end())
        changeItem(PacketManager::iconSmall(renamed),
            renamed->getPacketLabel().c_str(), it - packets.begin());
}

void PacketChooser::packetToBeDestroyed(regina::NPacket* toDestroy) {
    // Just remove the item that is being destroyed.
    std::vector<regina::NPacket*>::iterator it = std::find(
        packets.begin(), packets.end(), toDestroy);

    if (it != packets.end()) {
        long destroyIndex = it - packets.begin();
        bool destroyCurrent = (destroyIndex == currentItem());

        removeItem(destroyIndex);
        if (destroyCurrent && count() > 0)
            setCurrentItem(0);

        packets.erase(it);

        // Don't bother unlistening; this will happen in the packet
        // destructor anyway.
    }
}

void PacketChooser::refreshContents() {
    // Remember how it used to look.
    NPacket* remember = selectedPacket();
    bool allowNone = ((! packets.empty()) && (! packets[0]));

    // Empty the combo box.
    // Empty from the end in case it's stored as a vector deep inside.
    if (onAutoUpdate)
        unregisterFromAllPackets();

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
            if (onAutoUpdate)
                p->listen(this);
            if (p == select)
                setCurrentItem(count() - 1);
        }
        p = p->nextTreePacket();
    }
}

#include "packetchooser.moc"
