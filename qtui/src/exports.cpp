
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2016, Ben Burton                                   *
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
#include "foreign/csvsurfacehandler.h"
#include "foreign/exportdialog.h"
#include "foreign/pdfhandler.h"
#include "foreign/pythonhandler.h"
#include "foreign/recogniserhandler.h"
#include "foreign/reginahandler.h"
#include "foreign/snappeahandler.h"
#include "foreign/sourcehandler.h"
#include "reginafilter.h"

#include <QFileDialog>
#include <QTextCodec>

void ReginaMain::exportCSVSurfaceList() {
    exportFile(CSVSurfaceHandler::instance, tr(FILTER_CSV),
        tr("Export CSV Surface List"));
}

void ReginaMain::exportPDF() {
    exportFile(PDFHandler::instance, tr(FILTER_PDF),
        tr("Export PDF Document"));
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

void ReginaMain::exportSnapPea() {
    exportFile(SnapPeaHandler::instance, tr(FILTER_SNAPPEA),
        tr("Export SnapPea Triangulation"));
}

void ReginaMain::exportRecogniser() {
    exportFile(RecogniserHandler::instance, tr(FILTER_RECOGNISER),
        tr("Export Triangulation to 3-Manifold Recogniser"));
}

void ReginaMain::exportSource() {
    exportFile(SourceHandler::instance, tr(FILTER_CPP_SOURCE),
        tr("Export C++ Source"));
}

void ReginaMain::exportFile(const PacketExporter& exporter,
        const QString& fileFilter, const QString& dialogTitle) {
    ExportDialog dlg(this, packetTree.get(), treeView->selectedPacket(),
        exporter.canExport(), exporter.useExportEncoding(), dialogTitle);
    if (dlg.validate() && dlg.exec() == QDialog::Accepted) {
        regina::Packet* data = dlg.selectedPacket();
        if (data) {
            QString file = QFileDialog::getSaveFileName(this,
                dialogTitle, QString(), fileFilter);
            if (! file.isEmpty())
                exporter.exportData(data, file, this);
        }
    }
}

