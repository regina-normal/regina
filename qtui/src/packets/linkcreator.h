
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

/*! \file linkcreator.h
 *  \brief Allows the creation of knots and links.
 */

#ifndef __LINKCREATOR_H
#define __LINKCREATOR_H

#include "../packetcreator.h"

#include <QStackedWidget>

class ReginaMain;

class PacketChooser;
class QComboBox;
class QLineEdit;

/**
 * An interface for creating knots and links.
 */
class LinkCreator : public PacketCreator {
    private:
        /**
         * Internal components
         */
        QWidget* ui;
        QComboBox* type;
        QStackedWidget* details;

        /**
         * Details for specific link types
         */
        QLineEdit* code;
        QLineEdit* torusParams;
        QComboBox* exampleWhich;

    public:
        /**
         * Constructor.
         */
        LinkCreator(ReginaMain* mainWindow);

        /**
         * PacketCreator overrides.
         */
        QWidget* getInterface() override;
        std::shared_ptr<regina::Packet> createPacket(
            std::shared_ptr<regina::Packet> parentPacket,
            QWidget* parentWidget) override;
};

#endif
