
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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

/*! \file packetchooser.h
 *  \brief Provides a widget for selecting a single packet.
 */

#ifndef __PACKETCHOOSER_H
#define __PACKETCHOOSER_H

#include "packet/packetlistener.h"

#include <QDialog>
#include <QComboBox>
#include <vector>

class PacketFilter;

namespace regina {
    class Packet;
};

/**
 * A widget through which a single packet can be selected from a
 * subtree.  An optional filter may be applied to the subtree to
 * restrict the available selections.
 *
 * Note that by default the contents of the packet chooser will not be
 * updated in real time if the packet tree is externally modified.  The
 * routine refreshContents() is provided to allow the contents to be manually
 * updated.
 *
 * In particular, if it is possible that the selected packet will be
 * deleted during the lifetime of the packet chooser, it is advisable to
 * call refreshContents() before extracting the packet with selectedPacket().
 * This way the selected packet will be replaced with 0 if it has since
 * been destroyed.
 *
 * By calling setAutoUpdate(), a packet chooser can be modified to
 * update itself automatically in response to packets being renamed or
 * deleted.  However, packet insertions will still go ignored, i.e., no new
 * packets will be automatically added to the chooser.
 *
 * If auto update is on and the selected packet is destroyed,
 * the first option in the combo box will be selected.  Note however
 * that no activated() signal will be emitted since this change was not
 * a result of direct user interaction.
 */
class PacketChooser : public QComboBox, public regina::PacketListener {
    Q_OBJECT

    public:
        /**
         * How do we describe the root of the packet (which is hidden to
         * the user)?
         */
        enum RootRole {
            ROOT_AS_INSERTION_POINT,
            ROOT_AS_SUBTREE,
            ROOT_AS_PACKET
        };

    private:
        regina::Packet* subtree;
            /**< The subtree of packets available for selection. */
        PacketFilter* filter;
            /**< A filter to restrict the available selections, or
                 0 if no filter is necessary. */
        std::vector<regina::Packet*> packets;
            /**< A list of the packets corresponding to the available
                 entries in the packet chooser. */

        RootRole rootRole;
            /**< How we describe the root of the packet tree to the user. */
        bool onAutoUpdate;
            /**< Are we listening for changes to the packet tree? */
        bool isUpdating;
            /**< Are we currently in the middle of an update? */

    public:
        /**
         * Constructors that fills the chooser with available selections.
         *
         * If \a allowNone is passed as \c true, the list of choices
         * will begin with an optional "None" entry for which
         * getPacket() will return a null pointer.
         *
         * This chooser will claim ownership of any filter that is
         * passed.
         */
        PacketChooser(regina::Packet* newSubtree,
                RootRole useRootRole, QWidget* parent);
        PacketChooser(regina::Packet* newSubtree, PacketFilter* newFilter,
                RootRole useRootRole, QWidget* parent);
        PacketChooser(regina::Packet* newSubtree, PacketFilter* newFilter,
                RootRole useRootRole = ROOT_AS_SUBTREE, bool allowNone = false,
                regina::Packet* initialSelection = 0, QWidget* parent = 0);
        ~PacketChooser();

        /**
         * Returns the filter used by this packet chooser.
         */
        PacketFilter* getFilter();

        /**
         * Returns whether any packets at all are made available by this
         * packet chooser.  If the packet chooser is empty or if it
         * contains only a "None" entry, this routine returns \c false.
         */
        bool hasPackets();

        /**
         * Returns the currently selected packet.
         *
         * If the selected packet has since been destroyed, this routine
         * will return an invalid pointer which, if used, will probably
         * cause a crash.
         *
         * If the "None" entry is selected or if there are in fact no
         * available packets to choose from, this routine will return 0.
         */
        regina::Packet* selectedPacket();

        /**
         * Changes the selection to the given packet.
         *
         * If the given packet is not one of the options in this packet
         * chooser, or if the given pointer is 0, then the first entry
         * in the packet chooser will be selected (which is the "None"
         * entry if "None" was enabled).
         *
         * The activated() signal will \e not be emitted.
         */
        void selectPacket(regina::Packet* packet);

        /**
         * Set whether this packet chooser should update itself
         * automatically in response to its packets being deleted or
         * renamed.  This feature is disabled by default.
         */
        void setAutoUpdate(bool shouldAutoUpdate);

        /**
         * PacketListener overrides.
         */
        void packetWasRenamed(regina::Packet* packet);
        void packetToBeDestroyed(regina::Packet* packet);

    public slots:
        /**
         * Updates this chooser to reflect to the current state of the
         * packet tree.
         *
         * The current selection will be preserved if possible.
         *
         * It is assumed that the packet subtree initially passed to the
         * constructor has not since been destroyed.
         */
        void refreshContents();

    private:
        /**
         * Fills the chooser with the set of allowable packets.
         * A packet to be preselected can be optionally specified.
         */
        void fill(bool allowNone, regina::Packet* select);

        /**
         * Verifies whether the current list of packets shown in the
         * chooser matches the current state of the packet tree.
         */
        bool verify();
};

/**
 * A dialog used to select a single packet.
 */
class PacketDialog : public QDialog {
    Q_OBJECT

    private:
        /**
         * Internal components:
         */
        PacketChooser* chooser;

    public:
        /**
         * Constructor and destructor.
         */
        PacketDialog(QWidget* parent,
            regina::Packet* subtree,
            PacketFilter* filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis,
            PacketChooser::RootRole rootRole = PacketChooser::ROOT_AS_SUBTREE,
            bool allowNone = false,
            regina::Packet* initialSelection = 0);

        static regina::Packet* choose(QWidget* parent,
            regina::Packet* subtree,
            PacketFilter* filter,
            const QString& title,
            const QString& message,
            const QString& whatsThis,
            PacketChooser::RootRole rootRole = PacketChooser::ROOT_AS_SUBTREE,
            bool allowNone = false,
            regina::Packet* initialSelection = 0);
};

inline PacketFilter* PacketChooser::getFilter() {
    return filter;
}

inline bool PacketChooser::hasPackets() {
    return (packets.size() > 1 || (packets.size() == 1 && packets.front()));
}

#endif
