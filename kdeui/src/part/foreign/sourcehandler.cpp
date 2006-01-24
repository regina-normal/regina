
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

#include "triangulation/ntriangulation.h"

#include "sourcehandler.h"
#include "../packetfilter.h"

#include <fstream>
#include <klocale.h>
#include <kmessagebox.h>

const SourceHandler SourceHandler::instance;

PacketFilter* SourceHandler::canExport() const {
    return new SingleTypeFilter<regina::NTriangulation>();
}

bool SourceHandler::exportData(regina::NPacket* data,
        const QString& fileName, QWidget* parentWidget) const {
    regina::NTriangulation* tri = dynamic_cast<regina::NTriangulation*>(data);

    std::ofstream out(fileName.ascii());
    if (! out) {
        KMessageBox::error(parentWidget, i18n(
            "This triangulation could not be exported.  The target "
            "file %1 could not be opened for writing.").arg(fileName));
        return false;
    }
    out << tri->dumpConstruction();
    return true;
}

