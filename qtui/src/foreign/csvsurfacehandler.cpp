
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

#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"

#include "csvsurfacehandler.h"
#include "reginasupport.h"
#include "../packetfilter.h"

#include <QFile>
#include <QTextDocument>

const CSVSurfaceHandler CSVSurfaceHandler::instance;

PacketFilter* CSVSurfaceHandler::canExport() const {
    return new SingleTypeFilter<regina::PacketOf<regina::NormalSurfaces>>();
}

bool CSVSurfaceHandler::exportData(const regina::Packet& data,
        const QString& fileName, QWidget* parentWidget) const {
    auto& list = regina::static_packet_cast<regina::NormalSurfaces>(data);
    if (! list.saveCSVStandard(
            static_cast<const char*>(QFile::encodeName(fileName)))) {
        ReginaSupport::warn(parentWidget,
            QObject::tr("The export failed."), 
            QObject::tr("<qt>An unknown error occurred, probably related "
            "to file I/O.  Please check that you have permissions to write "
            "to the file <tt>%1</tt>.</qt>").arg(fileName.toHtmlEscaped()));
        return false;
    }
    return true;
}

