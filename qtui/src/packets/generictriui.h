
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

/*! \file generictriui.h
 *  \brief Provides a very basic interface for viewing generic triangulations.
 */

#ifndef __GENERICTRIUI_H
#define __GENERICTRIUI_H

#include "triangulation/forward.h"
#include "reginamain.h"
#include "../packetui.h"
#include <QObject>

class QLabel;
class QPushButton;

/**
 * A non-templated, QObject-enabled helper class for GenericTriangulationUI.
 * This provides functionality that is independent of the dimension of the
 * underlying triangulation, including all Qt slots and signals (which do
 * not work well with template classes).
 */
class GenericTriangulationBase : public QObject, public PacketReadOnlyUI {
    Q_OBJECT

    protected:
        /**
         * Packet details
         */
        regina::Packet* packet;

        /**
         * Internal components
         */
        QPushButton* python;

    public:
        /**
         * Constructor and destructor.
         */
        GenericTriangulationBase(regina::Packet* p,
            PacketPane* enclosingPane);

        /**
         * PacketUI overrides.
         */
        regina::Packet* getPacket();

    public slots:
        /**
         * Open a python console to work with this triangulation.
         */
        void pythonConsole();
};

/**
 * A very basic packet interface for working with generic triangulations.
 */
template <int dim>
class GenericTriangulationUI : public GenericTriangulationBase {
    private:
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
        GenericTriangulationUI(regina::Triangulation<dim>* packet,
                PacketPane* newEnclosingPane);

        /**
         * PacketUI overrides.
         */
        QWidget* getInterface();
        QString getPacketMenuText() const;
        void refresh();
};

#endif
