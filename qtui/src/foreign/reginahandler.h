
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

/*! \file reginahandler.h
 *  \brief Allows interaction with other Regina data files.
 */

#ifndef __REGINAHANDLER_H
#define __REGINAHANDLER_H

#include "file/fileformat.h"
#include "packetexporter.h"
#include "packetimporter.h"
#include <QString>

/**
 * An object responsible for importing and export data to and from
 * other Regina data files.
 */
class ReginaHandler : public PacketImporter, public PacketExporter {
    using PacketExporter::exportData;
    using PacketImporter::importData;
    private:
        bool compressed_;
            /**< Should exported data files be compressed? */
        regina::FileFormat format_;
            /**< Indicates which of Regina's XML file formats to write. */

    public:
        /**
         * Constructor.
         */
        ReginaHandler(bool compressed = true,
            regina::FileFormat format = regina::REGINA_CURRENT_FILE_FORMAT);

        /**
         * PacketImporter overrides:
         */
        std::shared_ptr<regina::Packet> importData(const QString& fileName,
            ReginaMain* parentWidget) const override;

        /**
         * PacketExporter overrides:
         */
        PacketFilter* canExport() const override;
        bool exportData(const regina::Packet& data,
            const QString& fileName, QWidget* parentWidget) const override;
        QString defaultExtension(const regina::Packet& data) const override;
};

inline ReginaHandler::ReginaHandler(bool compressed,
        regina::FileFormat format) : compressed_(compressed), format_(format) {
}

inline QString ReginaHandler::defaultExtension(const regina::Packet&) const {
    return ".rga";
}

#endif
