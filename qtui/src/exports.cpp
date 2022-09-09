
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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

#include "packet/packet.h"

#include "packettreeview.h"
#include "reginamain.h"
#include "foreign/attachmenthandler.h"
#include "foreign/csvsurfacehandler.h"
#include "foreign/exportdialog.h"
#include "foreign/pythonhandler.h"
#include "foreign/recogniserhandler.h"
#include "foreign/reginahandler.h"
#include "foreign/snappeahandler.h"
#include "reginafilter.h"

#include <QFileDialog>

void ReginaMain::exportCSVSurfaceList() {
    exportFile(CSVSurfaceHandler::instance, tr(FILTER_CSV),
        tr("Export CSV Surface List"));
}

void ReginaMain::exportPython() {
    exportFile(PythonHandler::instance, tr(FILTER_PYTHON_SCRIPTS),
        tr("Export Python Script"));
}

void ReginaMain::exportRegina() {
    exportFile(ReginaHandler(true), tr(FILTER_REGINA),
        tr("Export Regina Data File"));
}

void ReginaMain::exportReginaUncompressed() {
    exportFile(ReginaHandler(false), tr(FILTER_REGINA),
        tr("Export Regina Data File"));
}

void ReginaMain::exportReginaLegacy() {
    exportFile(ReginaHandler(true, regina::REGINA_XML_GEN_2), tr(FILTER_REGINA),
        tr("Export Legacy Regina Data File (v3.0–6.0.1)"));
}

void ReginaMain::exportSnapPea() {
    exportFile(SnapPeaHandler::instance, tr(FILTER_SNAPPEA),
        tr("Export SnapPea Triangulation"));
}

void ReginaMain::exportRecogniser() {
    exportFile(RecogniserHandler::instance, tr(FILTER_RECOGNISER),
        tr("Export Triangulation to 3-Manifold Recogniser"));
}

void ReginaMain::exportFile(const PacketExporter& exporter,
        const QString& fileFilter, const QString& dialogTitle) {
    ExportDialog dlg(this, packetTree, treeView->selectedPacket(),
        exporter.canExport(), exporter.useExportEncoding(), dialogTitle);
    if (dlg.validate() && dlg.exec() == QDialog::Accepted) {
        auto data = dlg.selectedPacket();
        if (data) {
            QString file = QFileDialog::getSaveFileName(this, dialogTitle,
                "Untitled" + exporter.defaultExtension(*data), fileFilter);
            if (! file.isEmpty())
                exporter.exportData(*data, file, this);
        }
    }
}

