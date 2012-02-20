
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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

#include "file/nxmlfile.h"

#include "reginahandler.h"
#include "../packetfilter.h"

#include <QFile>
#include <QMessageBox>

regina::NPacket* ReginaHandler::importData(const QString& fileName,
        QWidget* parentWidget) const {
    regina::NPacket* ans = regina::readFileMagic(
        static_cast<const char*>(QFile::encodeName(fileName)));
    if (! ans)
        QMessageBox::warning(parentWidget, QObject::tr("Import failed"),
            QObject::tr("The file %1 could not be imported.  Perhaps it is not "
            "a Regina data file?").arg(fileName));
    return ans;
}

PacketFilter* ReginaHandler::canExport() const {
    return new StandaloneFilter();
}

bool ReginaHandler::exportData(regina::NPacket* data,
        const QString& fileName, QWidget* parentWidget) const {
    if (data->dependsOnParent()) {
        QMessageBox::warning(parentWidget, QObject::tr("Cannot separate"), 
            QObject::tr("This packet cannot be separated from its parent."));
        return false;
    }
    if (! regina::writeXMLFile(QFile::encodeName(fileName), data, compressed)) {
        QMessageBox::warning(parentWidget, QObject::tr("Export failed"), 
            QObject::tr("This packet subtree could not be exported.  An "
            "unknown error, probably related to file I/O, occurred during "
            "the export."));
        return false;
    }
    return true;
}

