
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

#include "foreign/snappea.h"
#include "triangulation/ntriangulation.h"

#include "snappeahandler.h"
#include "../packetfilter.h"

#include <klocale.h>
#include <kmessagebox.h>

const SnapPeaHandler SnapPeaHandler::instance;

regina::NPacket* SnapPeaHandler::import(const QString& fileName,
        QWidget* parentWidget) const {
    regina::NPacket* ans = regina::readSnapPea(fileName.ascii());
    if (! ans)
        KMessageBox::error(parentWidget, i18n(
            "The SnapPea file %1 could not be imported.  Perhaps the data "
            "is not in SnapPea format?").arg(fileName));
    return ans;
}

PacketFilter* SnapPeaHandler::canExport() const {
    return new SingleTypeFilter<regina::NTriangulation>();
}

bool SnapPeaHandler::exportData(regina::NPacket* data,
        const QString& fileName, QWidget* parentWidget) const {
    regina::NTriangulation* tri = dynamic_cast<regina::NTriangulation*>(data);
    if (! tri->isValid()) {
        KMessageBox::error(parentWidget, i18n(
            "This triangulation cannot be exported to SnapPea format "
            "because it is not a valid triangulation."));
        return false;
    }
    if (tri->hasBoundaryFaces()) {
        KMessageBox::error(parentWidget, i18n(
            "This triangulation cannot be exported to SnapPea format "
            "because it has one or more boundary faces."));
        return false;
    }
    if (! regina::writeSnapPea(fileName.ascii(), *tri)) {
        KMessageBox::error(parentWidget, i18n(
            "This triangulation could not be exported.  An unknown error, "
            "probably related to file I/O, occurred during the export."));
        return false;
    }
    return true;
}

