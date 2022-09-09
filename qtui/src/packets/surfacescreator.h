
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

/*! \file surfacescreator.h
 *  \brief Allows the creation of normal surface lists in 3-manifold
 *  triangulations.
 */

#ifndef __SURFACESCREATOR_H
#define __SURFACESCREATOR_H

#include "../packetcreator.h"

class CoordinateChooser;
class QCheckBox;
class QComboBox;

/**
 * An interface for creating normal surface lists in 3-manifold triangulations.
 */
class SurfacesCreator : public PacketCreator {
    private:
        /**
         * Internal components
         */
        QWidget* ui;
        CoordinateChooser* coords;
        QComboBox* basis;
        QCheckBox* embedded;

    public:
        /**
         * Constructor.
         */
        SurfacesCreator(ReginaMain*);

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
