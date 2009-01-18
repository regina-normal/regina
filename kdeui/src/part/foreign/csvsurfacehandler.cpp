
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

#include "foreign/csvsurfacelist.h"
#include "surfaces/nnormalsurfacelist.h"

#include "csvsurfacehandler.h"
#include "../packetfilter.h"

#include <klocale.h>
#include <kmessagebox.h>
#include <qfile.h>

const CSVSurfaceHandler CSVSurfaceHandler::instance;

PacketFilter* CSVSurfaceHandler::canExport() const {
    return new SingleTypeFilter<regina::NNormalSurfaceList>();
}

bool CSVSurfaceHandler::exportData(regina::NPacket* data,
        const QString& fileName, QWidget* parentWidget) const {
    regina::NNormalSurfaceList* list =
        dynamic_cast<regina::NNormalSurfaceList*>(data);
    if (! regina::writeCSVStandard(
            static_cast<const char*>(QFile::encodeName(fileName)), *list)) {
        KMessageBox::error(parentWidget, i18n(
            "This normal surface list could not be exported.  An unknown "
            "error, probably related to file I/O, occurred during the "
            "export."));
        return false;
    }
    return true;
}

