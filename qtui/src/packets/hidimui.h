
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2026, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

/*! \file hidimui.h
 *  \brief Provides a very basic interface for viewing triangulations in
 *  higher dimensions.
 */

#ifndef __HIDIMUI_H
#define __HIDIMUI_H

#include "triangulation/forward.h"
#include "reginamain.h"
#include "../packetui.h"
#include <QObject>

class QLabel;

/**
 * A very basic packet interface for working with triangulations in higher
 * dimensions.
 */
template <int dim>
requires (regina::supportedDim(dim) && ! regina::standardDim(dim))
class HiDimUI : public QObject, public PacketReadOnlyUI {
    private:
        /**
         * Packet details
         */
        regina::Packet* packet;

        /**
         * Internal components
         */
        QWidget* ui;
        QLabel* type;
        QLabel* fVector;
        QLabel* boundary;

    public:
        /**
         * Constructor and destructor.
         */
        HiDimUI(regina::PacketOf<regina::Triangulation<dim>>* packet,
            PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        regina::Packet* getPacket() override;
        QWidget* getInterface() override;
        QString getPacketMenuText() const override;
        void refresh() override;

        /**
         * Open a python console to work with this triangulation.
         */
        void pythonConsole();
};

#endif
