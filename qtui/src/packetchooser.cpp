
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
#include "packetchooser.h"
#include "packetfilter.h"
#include "packetmanager.h"
#include <QDialogButtonBox>
#include <QLabel>
#include <QBoxLayout>

#include <algorithm>

using regina::Packet;

PacketChooser::PacketChooser(std::shared_ptr<Packet> newSubtree,
        RootRole useRootRole, QWidget* parent) :
        QComboBox(parent), subtree(std::move(newSubtree)), filter(nullptr),
        rootRole(useRootRole), onAutoUpdate(false), isUpdating(false) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    fill(false, nullptr);
}

PacketChooser::PacketChooser(std::shared_ptr<Packet> newSubtree,
        PacketFilter* newFilter, RootRole useRootRole, QWidget* parent) :
        QComboBox(parent), subtree(std::move(newSubtree)), filter(newFilter),
        rootRole(useRootRole), onAutoUpdate(false), isUpdating(false) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    fill(false, nullptr);
}

PacketChooser::PacketChooser(std::shared_ptr<Packet> newSubtree,
        PacketFilter* newFilter, RootRole useRootRole, bool allowNone,
        std::shared_ptr<Packet> initialSelection, QWidget* parent) :
        QComboBox(parent), subtree(std::move(newSubtree)), filter(newFilter),
        rootRole(useRootRole), onAutoUpdate(false), isUpdating(false) {
    setMinimumContentsLength(30);
    setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    fill(allowNone, std::move(initialSelection));
}

PacketChooser::~PacketChooser() {
    if (filter)
        delete filter;
}

std::shared_ptr<Packet> PacketChooser::selectedPacket() {
    if (count() == 0)
        return nullptr;
    int curr = currentIndex();
    return (curr < 0 ? nullptr :
            packets[curr] ? packets[curr]->shared_from_this() :
            nullptr);
}

void PacketChooser::selectPacket(std::shared_ptr<Packet> packet) {
    int index = 0;
    for (regina::Packet* p : packets) {
        if (p == packet.get()) {
            setCurrentIndex(index);
            return;
        }
        ++index;
    }

    // Not found.
    if (! packets.empty())
        setCurrentIndex(0);
    return;
}

void PacketChooser::setAutoUpdate(bool shouldAutoUpdate) {
    if (onAutoUpdate == shouldAutoUpdate)
        return;

    onAutoUpdate = shouldAutoUpdate;
    if (onAutoUpdate) {
        for (regina::Packet* p : packets)
            if (p)
                p->listen(this);
    } else
        unregisterFromAllPackets();
}

void PacketChooser::packetWasRenamed(regina::Packet* renamed) {
    // Just rename the item that was changed.
    auto it = std::find(packets.begin(), packets.end(), renamed);
    if (it != packets.end()) {
        // This may trigger a refreshContents(), but that's okay since
        // we're at the end of the routine.
        int index = it - packets.begin();
        setItemIcon(index, PacketManager::icon(renamed));
        setItemText(index, renamed->humanLabel().c_str());
    }
}

void PacketChooser::packetToBeDestroyed(regina::PacketShell toDestroy) {
    // Just remove the item that is being destroyed.
    auto it = std::find(packets.begin(), packets.end(), toDestroy);
    if (it != packets.end()) {
        // Make sure the call to removeItem() comes last since it could
        // trigger a refreshContents().
        long destroyIndex = it - packets.begin();
        long currIndex = currentIndex();

        packets.erase(it);
        if (destroyIndex == currIndex) {
            // We know count() > 0 since currentItem() exists.
            setCurrentIndex(0);

            // If the item to destroy *is* 0, this should just fall through
            // to whatever's next when we remove it from the chooser.
        } else if (destroyIndex < currIndex) {
            // The selected item has moved up the list.
            setCurrentIndex(currIndex - 1);
        }

        removeItem(destroyIndex);

        // Don't bother unlistening; this will happen in the packet
        // destructor anyway.
    }
}

void PacketChooser::refreshContents() {
    if (isUpdating)
        return;

    // Don't change anything if we don't have to.
    if (verify())
        return;

    // Do a straight empty-and-refill.
    // There are more streamlined ways of doing it, but we'll come to it
    // when we come to it.
    isUpdating = true;

    // Remember how it used to look.
    auto remember = selectedPacket();
    bool allowNone = ((! packets.empty()) && (! packets[0]));

    // Empty the combo box.
    // Empty from the end in case it's stored as a vector deep inside.
    if (onAutoUpdate)
        unregisterFromAllPackets();

    clear();
    packets.clear();

    // Fill it again.
    fill(allowNone, remember);

    isUpdating = false;
}

void PacketChooser::fill(bool allowNone, std::shared_ptr<Packet> select) {
    // Insert the None entry if appropriate.
    if (allowNone) {
        addItem(tr("<None>"));
        packets.push_back(nullptr);

        if (! select)
            setCurrentIndex(0);
    }

    // Insert the regular packets.
    std::shared_ptr<Packet> p = subtree;
    while (p && subtree->isAncestorOf(*p)) {
        if ((! filter) || (filter->accept(*p))) {
            if (p->parent())
                addItem(PacketManager::icon(p.get()), p->humanLabel().c_str());
            else switch (rootRole) {
                case ROOT_AS_INSERTION_POINT:
                    // No icon for this role.
                    addItem(tr("<Top level>"));
                    break;
                case ROOT_AS_SUBTREE:
                    // No icon for this role.
                    addItem(tr("<Entire tree>"));
                    break;
                case ROOT_AS_PACKET:
                    addItem(PacketManager::icon(p.get()), tr("<Root packet>"));
                    break;
            }
            packets.push_back(p.get());
            if (onAutoUpdate)
                p->listen(this);
            if (p == select)
                setCurrentIndex(count() - 1);
        }
        p = p->nextTreePacket();
    }
}

bool PacketChooser::verify() {
    std::shared_ptr<Packet> p = subtree;
    auto it = packets.begin();

    // Ignore the None entry if it exists.
    if (it != packets.end() && (! *it))
        ++it;

    // Now match the packets up one by one.
    while (it != packets.end() || p) {
        // Are we ignoring this packet?
        if (p && filter && ! filter->accept(*p)) {
            p = p->nextTreePacket();
            continue;
        }

        // Out of packets?
        if (it == packets.end())
            return false;
        // Out of combo box entries?
        if (! p)
            return false;
        // Mismatched entries?
        if (p.get() != *it)
            return false;

        // All good.
        it++;
        p = p->nextTreePacket();
    }

    return true;
}

PacketDialog::PacketDialog(QWidget* parent,
        std::shared_ptr<Packet> subtree,
        PacketFilter* filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis,
        PacketChooser::RootRole rootRole,
        bool allowNone,
        std::shared_ptr<Packet> initialSelection) :
        QDialog(parent) {
    setWindowTitle(title);
    setWhatsThis(whatsThis);
    auto* layout = new QVBoxLayout(this);

    auto* label = new QLabel(message);
    layout->addWidget(label);

    chooser = new PacketChooser(std::move(subtree), filter, rootRole, allowNone,
        std::move(initialSelection), this);
    layout->addWidget(chooser);

    auto* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

std::shared_ptr<Packet> PacketDialog::choose(QWidget* parent,
        std::shared_ptr<Packet> subtree,
        PacketFilter* filter,
        const QString& title,
        const QString& message,
        const QString& whatsThis,
        PacketChooser::RootRole rootRole,
        bool allowNone,
        std::shared_ptr<Packet> initialSelection) {
    PacketDialog dlg(parent, std::move(subtree), filter, title, message,
        whatsThis, rootRole, allowNone, std::move(initialSelection));
    if (dlg.exec())
        return dlg.chooser->selectedPacket();
    else
        return nullptr;
}

