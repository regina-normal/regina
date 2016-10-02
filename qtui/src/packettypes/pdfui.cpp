
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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

// Regina core includes:
#include "packet/pdf.h"

// UI includes:
#include "pdfui.h"
#include "reginamain.h"
#include "reginasupport.h"
#include "sharedtempfile.h"

#include <csignal>
#include <cstdio>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QLayout>
#include <QProcess>
#include <QTextDocument>
#include <QUrl>

void PDFExternalViewer::view(regina::Packet* packet, QWidget* parentWidget) {
    // Write the PDF data to our temporary file.
    const char* data = static_cast<regina::PDF*>(packet)->data();
    if (! data) {
        ReginaSupport::info(parentWidget,
            QObject::tr("This PDF packet is empty."));
        return;
    }

    // Set suffix. Note that XXXXXX (exactly 6 X's all uppercase) gets replaced
    // with random letters to ensure the file does not already exist.
    SharedTempFile* temp = new SharedTempFile("XXXXXX.pdf", parentWidget);
    if (! temp->valid()) {
        ReginaSupport::warn(parentWidget,
            QObject::tr("<qt>I could not create the temporary "
            "PDF file <i>%1</i>.</qt>").arg(temp->localFileName()));
        delete temp;
        return;
    }

    if (! static_cast<regina::PDF*>(packet)->savePDF(static_cast<const char*>(
            QFile::encodeName(temp->localFileName())))) {
        ReginaSupport::warn(parentWidget,
            QObject::tr("<qt>An error occurred whilst writing the PDF "
            "data to the temporary file <i>%1</i>.</qt>").
            arg(temp->localFileName()));
        delete temp;
        return;
    }

    temp->share();

    QString externalViewer =
        ReginaPrefSet::global().pdfExternalViewer.trimmed();

    if (externalViewer.isEmpty()) {
        // Fall back to the Qt default for PDFs.
        if (! QDesktopServices::openUrl(temp->url())) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("<qt>I was not able to find a suitable "
                "PDF viewer.<p>"
                "Please specify your preferred PDF viewer under the "
                "<i>Tools</i> options in Regina's settings.</qt>"));
            delete temp;
        }
    } else {
        if (! QProcess::startDetached(externalViewer,
                QStringList(temp->localFileName()))) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("<qt>I was not able to open an external "
                "PDF viewer.  The failed command was:<p>"
                "<tt>%1 \"%2\"</tt><p>"
                "You can fix this by editing the <i>Tools</i> options in "
                "Regina's settings.</qt>").
                arg(externalViewer.toHtmlEscaped()).
                arg(temp->localFileName().toHtmlEscaped()));
            delete temp;
        }
    }
}

