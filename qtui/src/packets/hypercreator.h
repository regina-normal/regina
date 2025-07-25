
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

/*! \file hypercreator.h
 *  \brief Allows the creation of normal hypersurface lists in
 *  4-manifold triangulations.
 */

#ifndef __HYPERCREATOR_H
#define __HYPERCREATOR_H

#include "../packetcreator.h"

class HyperCoordinateChooser;
class QCheckBox;
class QComboBox;

/**
 * An interface for creating normal hypersurface lists in 4-manifold
 * triangulations.
 */
class HyperCreator : public PacketCreator {
    private:
        /**
         * Internal components
         */
        QWidget* ui;
        HyperCoordinateChooser* coords;
        QComboBox* basis;
        QCheckBox* embedded;

    public:
        /**
         * Constructor.
         */
        HyperCreator(ReginaMain*);

        /**
         * PacketCreator overrides.
         */
        QWidget* getInterface() override;
        QString parentPrompt() override;
        QString parentWhatsThis() override;
        std::shared_ptr<regina::Packet> createPacket(
            std::shared_ptr<regina::Packet> parentPacket,
            QWidget* parentWidget) override;
        PacketFilter* filter() override;
        void explainNoParents() override;
};

#endif
