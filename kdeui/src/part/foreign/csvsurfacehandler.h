
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

/*! \file csvsurfacehandler.h
 *  \brief Supports exporting normal surface lists to CSV text files.
 */

#ifndef __CSVSURFACEHANDLER_H
#define __CSVSURFACEHANDLER_H

#include "packetexporter.h"
#include "packetimporter.h"

/**
 * An object responsible for exporting normal surface lists to
 * CSV (comma-separated value) text files.
 *
 * Rather than creating new objects of this class, the globally
 * available object CSVSurfaceHandler::instance should always be used.
 */
class CSVSurfaceHandler : public PacketExporter {
    public:
        /**
         * A globally available instance of this class.
         */
        static const CSVSurfaceHandler instance;

    public:
        /**
         * PacketExporter overrides:
         */
        virtual PacketFilter* canExport() const;
        virtual bool exportData(regina::NPacket* data,
            const QString& fileName, QWidget* parentWidget) const;

    private:
        /**
         * Don't allow people to construct their own CSV handlers.
         */
        CSVSurfaceHandler();
};

inline CSVSurfaceHandler::CSVSurfaceHandler() {
}

#endif
