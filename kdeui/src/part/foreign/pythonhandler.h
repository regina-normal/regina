
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

/*! \file pythonhandler.h
 *  \brief Allows interaction with Python scripts.
 */

#ifndef __PYTHONHANDLER_H
#define __PYTHONHANDLER_H

#include "packetexporter.h"
#include "packetimporter.h"

/**
 * An object responsible for importing and export data to and from
 * Python files.
 *
 * Rather than creating new objects of this class, the globally
 * available object PythonHandler::instance should always be used.
 */
class PythonHandler : public PacketImporter, public PacketExporter {
    public:
        /**
         * A globally available instance of this class.
         */
        static const PythonHandler instance;

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
         * Don't allow people to construct their own Python handlers.
         */
        PythonHandler();
};

inline PythonHandler::PythonHandler() {
}

#endif
