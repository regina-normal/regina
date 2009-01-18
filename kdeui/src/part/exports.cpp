
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

#include "packet/npacket.h"

#include "packettreeview.h"
#include "reginapart.h"
#include "foreign/csvsurfacehandler.h"
#include "foreign/exportdialog.h"
#include "foreign/pdfhandler.h"
#include "foreign/pythonhandler.h"
#include "foreign/reginahandler.h"
#include "foreign/snappeahandler.h"
#include "foreign/sourcehandler.h"
#include "reginafilter.h"

#include <kencodingfiledialog.h>
#include <kfiledialog.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <qtextcodec.h>

void ReginaPart::exportCSVSurfaceList() {
    exportFile(CSVSurfaceHandler::instance, i18n(FILTER_CSV),
        i18n("Export CSV Surface List"));
}

void ReginaPart::exportPDF() {
    exportFile(PDFHandler::instance, i18n(FILTER_PDF),
        i18n("Export PDF Document"));
}

void ReginaPart::exportPython() {
    exportFile(PythonHandler::instance, i18n(FILTER_PYTHON_SCRIPTS),
        i18n("Export Python Script"));
}

void ReginaPart::exportRegina() {
    exportFile(ReginaHandler(true), i18n(FILTER_REGINA),
        i18n("Export Regina Data File"));
}

void ReginaPart::exportReginaUncompressed() {
    exportFile(ReginaHandler(false), i18n(FILTER_REGINA),
        i18n("Export Regina Data File"));
}

void ReginaPart::exportSnapPea() {
    exportFile(SnapPeaHandler::instance, i18n(FILTER_SNAPPEA),
        i18n("Export SnapPea Triangulation"));
}

void ReginaPart::exportSource() {
    exportFile(SourceHandler::instance, i18n(FILTER_CPP_SOURCE),
        i18n("Export C++ Source"));
}

void ReginaPart::exportFile(const PacketExporter& exporter,
        const QString& fileFilter, const QString& dialogTitle) {
    ExportDialog dlg(widget(), packetTree, treeView->selectedPacket(),
        exporter.canExport(), dialogTitle);
    if (dlg.validate() && dlg.exec() == QDialog::Accepted) {
        regina::NPacket* data = dlg.selectedPacket();
        if (data) {
            if (exporter.offerExportEncoding()) {
                KEncodingFileDialog::Result result =
                    KEncodingFileDialog::getSaveFileNameAndEncoding(
                        QString::null /* encoding */, QString::null,
                        fileFilter, widget(), dialogTitle);
                if ((! result.fileNames.empty()) &&
                        (! result.fileNames.front().isEmpty()))
                    exporter.exportData(data, result.fileNames.front(),
                        QTextCodec::codecForName(result.encoding), widget());
            } else {
                QString file = KFileDialog::getSaveFileName(QString::null,
                    fileFilter, widget(), dialogTitle);
                if (! file.isEmpty())
                    exporter.exportData(data, file, widget());
            }
        }
    }
}

