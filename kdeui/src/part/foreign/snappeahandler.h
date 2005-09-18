
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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

/*! \file snappeahandler.h
 *  \brief Allows interaction with SnapPea data files.
 */

#ifndef __SNAPPEAHANDLER_H
#define __SNAPPEAHANDLER_H

#include "packetexporter.h"
#include "packetimporter.h"

/**
 * An object responsible for importing and export data to and from
 * SnapPea files.
 *
 * Rather than creating new objects of this class, the globally
 * available object SnapPeaHandler::instance should always be used.
 */
class SnapPeaHandler : public PacketImporter, public PacketExporter {
    public:
        /**
         * A globally available instance of this class.
         */
        static const SnapPeaHandler instance;

    public:
        /**
         * PacketImporter overrides:
         */
        virtual regina::NPacket* import(const QString& fileName,
            QWidget* parentWidget) const;

        /**
         * PacketExporter overrides:
         */
        virtual PacketFilter* canExport() const;
        virtual bool exportData(regina::NPacket* data,
            const QString& fileName, QWidget* parentWidget) const;

    private:
        /**
         * Don't allow people to construct their own SnapPea handlers.
         */
        SnapPeaHandler();
};

inline SnapPeaHandler::SnapPeaHandler() {
}

#endif
