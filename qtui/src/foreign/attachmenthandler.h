
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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

/*! \file attachmenthandler.h
 *  \brief Allows interaction with file attachments.
 */

#ifndef __ATTACHMENTHANDLER_H
#define __ATTACHMENTHANDLER_H

#include "packet/attachment.h"
#include "packetexporter.h"
#include "packetimporter.h"
#include <QString>

/**
 * An object responsible for importing and exporting file attachments.
 *
 * Rather than creating new objects of this class, the globally
 * available object AttachmentHandler::instance should always be used.
 */
class AttachmentHandler : public PacketImporter, public PacketExporter {
    using PacketExporter::exportData;
    using PacketImporter::importData;
    public:
        /**
         * A globally available instance of this class.
         */
        static const AttachmentHandler instance;

    public:
        /**
         * PacketImporter overrides:
         */
        std::shared_ptr<regina::Packet> importData(const QString& fileName,
            ReginaMain* parentWidget) const override;

        /**
         * PacketExporter overrides:
         */
        PacketFilter* canExport() const override;
        bool exportData(std::shared_ptr<regina::Packet> data,
            const QString& fileName, QWidget* parentWidget) const override;
        QString defaultExtension(const regina::Packet& data) const override;

    private:
        /**
         * Don't allow people to construct their own attachment handlers.
         */
        AttachmentHandler() = default;
};

inline QString AttachmentHandler::defaultExtension(const regina::Packet& data)
        const {
    return static_cast<const regina::Attachment&>(data).extension().c_str();
}

#endif
