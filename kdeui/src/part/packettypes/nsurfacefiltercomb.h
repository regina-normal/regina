
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

/*! \file nsurfacefiltercomb.h
 *  \brief Provides an interface for working with combination surface filters.
 */

#ifndef __NSURFACEFILTERCOMB_H
#define __NSURFACEFILTERCOMB_H

#include "packet/npacketlistener.h"

#include "../packetui.h"

class QButtonGroup;
class QListView;
class QRadioButton;

namespace regina {
    class NSurfaceFilterCombination;
    class NPacket;
};

/**
 * A packet interface for working with combination surface filters.
 */
class NSurfaceFilterCombUI : public QObject, public PacketUI,
        public regina::NPacketListener {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::NSurfaceFilterCombination* filter;

        /**
         * Internal components
         */
        QWidget* ui;
        QButtonGroup* boolType;
        QRadioButton* typeAnd;
        QRadioButton* typeOr;
        QListView* children;

    public:
        /**
         * Constructor and destructor.
         */
        NSurfaceFilterCombUI(regina::NSurfaceFilterCombination* packet,
                PacketPane* newEnclosingPane, bool readWrite);
        ~NSurfaceFilterCombUI();

        /**
         * PacketUI overrides.
         */
        regina::NPacket* getPacket();
        QWidget* getInterface();
        QString getPacketMenuText() const;
        void commit();
        void refresh();
        void setReadWrite(bool readWrite);

        /**
         * NPacketListener overrides.
         */
        void packetWasRenamed(regina::NPacket* packet);
        void childWasAdded(regina::NPacket* packet, regina::NPacket* child);
        void childWasRemoved(regina::NPacket* packet, regina::NPacket* child);
        void childrenWereReordered(regina::NPacket* packet);

    public slots:
        /**
         * Notification that the boolean type has been changed.
         */
        void notifyBoolTypeChanged();

    private:
        /**
         * Refresh the list of child filters.
         */
        void refreshChildList();
};

#endif
