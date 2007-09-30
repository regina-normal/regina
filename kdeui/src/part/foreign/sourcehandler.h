
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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

/*! \file sourcehandler.h
 *  \brief Allows exporting triangulations to C++ source files.
 */

#ifndef __SOURCEHANDLER_H
#define __SOURCEHANDLER_H

#include "packetexporter.h"

/**
 * An object responsible for exporting triangulations to C++ source files.
 *
 * Rather than creating new objects of this class, the globally
 * available object SourceHandler::instance should always be used.
 */
class SourceHandler : public PacketExporter {
    public:
        /**
         * A globally available instance of this class.
         */
        static const SourceHandler instance;

    public:
        /**
         * PacketExporter overrides:
         */
        virtual PacketFilter* canExport() const;
        virtual bool exportData(regina::NPacket* data,
            const QString& fileName, QWidget* parentWidget) const;

    private:
        /**
         * Don't allow people to construct their own Source handlers.
         */
        SourceHandler();
};

inline SourceHandler::SourceHandler() {
}

#endif
