
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

/*! \file packetchooser.h
 *  \brief Provides a widget for selecting a single packet.
 */

#ifndef __PACKETCHOOSER_H
#define __PACKETCHOOSER_H

#include "packet/npacketlistener.h"

#include <kcombobox.h>
#include <vector>

class PacketFilter;

namespace regina {
    class NPacket;
};

/**
 * A widget through which a single packet can be selected from a
 * subtree.  An optional filter may be applied to the subtree to
 * restrict the available selections.
 *
 * Note that the contents of the packet chooser will not be updated in
 * real time if the packet tree is externally modified.  The routine
 * refreshContents() is provided to allow the contents to be manually
 * updated.
 */
class PacketChooser : public KComboBox {
    Q_OBJECT

    private:
        regina::NPacket* subtree;
            /**< The subtree of packets available for selection. */
        PacketFilter* filter;
            /**< A filter to restrict the available selections, or
                 0 if no filter is necessary. */
        std::vector<regina::NPacket*> packets;
            /**< A list of the packets corresponding to the available
                 entries in the packet chooser. */

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
        PacketChooser(regina::NPacket* newSubtree,
                QWidget* parent, const char* name = 0);
        PacketChooser(regina::NPacket* newSubtree, PacketFilter* newFilter,
                QWidget* parent, const char* name = 0);
        PacketChooser(regina::NPacket* newSubtree, PacketFilter* newFilter = 0,
                bool allowNone = false, regina::NPacket* initialSelection = 0,
                QWidget* parent = 0, const char* name = 0);
        ~PacketChooser();

        /**
         * Returns the filter used by this packet chooser.
         */
        PacketFilter* getFilter();

        /**
         * Returns the currently selected packet.
         *
         * If the selected packet has since been destroyed, this routine
         * will return an invalid pointer which, if used, will probably
         * cause a crash.
         *
         * If the "None" entry is selected, this routine will return 0.
         */
        regina::NPacket* selectedPacket();

    public slots:
        /**
         * Empties and refills this chooser according to the current
         * state of the packet tree.
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
        void fill(bool allowNone, regina::NPacket* select);
};

inline PacketFilter* PacketChooser::getFilter() {
    return filter;
}

#endif
