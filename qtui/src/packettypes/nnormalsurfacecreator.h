
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file nnormalsurfacecreator.h
 *  \brief Allows the creation of normal surface lists.
 */

#ifndef __NNORMALSURFACECREATOR_H
#define __NNORMALSURFACECREATOR_H

#include "../packetcreator.h"

class CoordinateChooser;
class QCheckBox;
class QComboBox;

/**
 * An interface for creating normal surface lists.
 */
class NNormalSurfaceCreator : public PacketCreator {
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
        NNormalSurfaceCreator();

        /**
         * PacketCreator overrides.
         */
        QWidget* getInterface();
        QString parentPrompt();
        QString parentWhatsThis();
        regina::NPacket* createPacket(regina::NPacket* parentPacket,
            QWidget* parentWidget);
        void explainNoParents();
};

#endif
