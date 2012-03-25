
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

// Regina core includes:
#include "foreign/pdf.h"
#include "packet/npdf.h"

// UI includes:
#include "npdfui.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <csignal>
#include <cstdio>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QLayout>
#include <QProcess>
#include <QTextDocument>
#include <QTemporaryFile>
#include <QUrl>

void NPDFExternalViewer::view(regina::NPacket* packet, QWidget* parentWidget) {
    // Write the PDF data to our temporary file.
    const char* data = static_cast<regina::NPDF*>(packet)->data();
    if (! data) {
        ReginaSupport::info(parentWidget,
            QObject::tr("This PDF packet is empty."));
        return;
    }

    // Set suffix. Note that XXXXXX (exactly 6 X's all uppercase) gets replaced
    // with random letters to ensure the file does not already exist.
    QTemporaryFile* temp = new QTemporaryFile(
        QString("%1/XXXXXX.pdf").arg(QDir::tempPath()), parentWidget);
    if (! temp->open()) {
        ReginaSupport::warn(parentWidget,
            QObject::tr("<qt>I could not create the temporary "
            "PDF file <i>%1</i>.</qt>").arg(temp->fileName()));
        delete temp;
        return;
    }
    temp->close();

    if (! regina::writePDF(static_cast<const char*>(
            QFile::encodeName(temp->fileName())),
            static_cast<regina::NPDF&>(*packet))) {
        ReginaSupport::warn(parentWidget,
            QObject::tr("<qt>An error occurred whilst writing the PDF "
            "data to the temporary file <i>%1</i>.</qt>").
            arg(temp->fileName()));
        delete temp;
        return;
    }

    QString externalViewer =
        ReginaPrefSet::global().pdfExternalViewer.trimmed();

    if (externalViewer.isEmpty()) {
        // Fall back to the Qt default for PDFs.
        if (! QDesktopServices::openUrl(
                QUrl::fromLocalFile(temp->fileName()))) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("<qt>I was not able to find a suitable "
                "PDF viewer.<p>"
                "Please specify your preferred PDF viewer under the "
                "<i>Tools</i> options in Regina's settings.</qt>"));
            delete temp;
        }
    } else {
        if (! QProcess::startDetached(externalViewer,
                QStringList(temp->fileName()))) {
            ReginaSupport::sorry(parentWidget,
                QObject::tr("<qt>I was not able to open an external "
                "PDF viewer.  The failed command was:<p>"
                "<tt>%1 \"%2\"</tt><p>"
                "You can fix this by editing the <i>Tools</i> options in "
                "Regina's settings.</qt>").
                arg(Qt::escape(externalViewer)).
                arg(Qt::escape(temp->fileName())));
            delete temp;
        }
    }
}

