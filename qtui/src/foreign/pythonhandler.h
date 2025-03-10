
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
 *  You should have received a copy of the GNU General Public             *
 *  License along with this program; if not, write to the Free            *
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,       *
 *  MA 02110-1301, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/*! \file pythonhandler.h
 *  \brief Allows interaction with Python scripts.
 */

#ifndef __PYTHONHANDLER_H
#define __PYTHONHANDLER_H

#include "packetexporter.h"
#include "packetimporter.h"
#include <QString>

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
        std::shared_ptr<regina::Packet> importData(const QString& fileName,
            ReginaMain* parentWidget) const override;
        bool useImportEncoding() const override;

        /**
         * PacketExporter overrides:
         */
        PacketFilter* canExport() const override;
        bool exportData(const regina::Packet& data,
            const QString& fileName, QWidget* parentWidget) const override;
        bool useExportEncoding() const override;
        QString defaultExtension(const regina::Packet& data) const override;

    private:
        /**
         * Don't allow people to construct their own Python handlers.
         */
        PythonHandler() = default;
};

inline bool PythonHandler::useImportEncoding() const {
    return true;
}

inline bool PythonHandler::useExportEncoding() const {
    return true;
}

inline QString PythonHandler::defaultExtension(const regina::Packet&) const {
    return ".py";
}

#endif
