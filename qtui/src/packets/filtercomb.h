
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

/*! \file filtercomb.h
 *  \brief Provides an interface for working with combination surface filters.
 */

#ifndef __FILTERCOMB_H
#define __FILTERCOMB_H

#include "packet/packetlistener.h"

#include "../packetui.h"

class QButtonGroup;
class QListWidget;
class QRadioButton;

namespace regina {
    class SurfaceFilterCombination;
    class Packet;
};

/**
 * A packet interface for working with combination surface filters.
 */
class FilterCombUI : public QObject, public PacketUI,
        public regina::PacketListener {
    Q_OBJECT

    private:
        /**
         * Packet details
         */
        regina::SurfaceFilterCombination* filter;

        /**
         * Internal components
         */
        QWidget* ui;
        QButtonGroup* boolType;
        QRadioButton* typeAnd;
        QRadioButton* typeOr;
        QListWidget* children;

    public:
        /**
         * Constructor and destructor.
         */
        FilterCombUI(regina::SurfaceFilterCombination* packet,
                PacketPane* newEnclosingPane);
        ~FilterCombUI();

        /**
         * PacketUI overrides.
         */
        regina::Packet* getPacket();
        QWidget* getInterface();
        QString getPacketMenuText() const;
        void refresh();
        void setReadWrite(bool readWrite);

        /**
         * PacketListener overrides.
         */
        void packetWasRenamed(regina::Packet* packet);
        void childWasAdded(regina::Packet* packet, regina::Packet* child);
        void childWasRemoved(regina::Packet* packet, regina::Packet* child,
            bool inParentDestructor);
        void childrenWereReordered(regina::Packet* packet);

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
