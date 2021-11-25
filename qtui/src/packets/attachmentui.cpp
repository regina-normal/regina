
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

// Regina core includes:
#include "packet/attachment.h"

// UI includes:
#include "attachmentui.h"
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

void AttachmentExternalViewer::view(regina::Packet* packet,
        QWidget* parentWidget) {
    const auto& att = static_cast<regina::Attachment&>(*packet);

    // Write the attachment to a temporary file.
    const char* data = att.data();
    if (! data) {
        ReginaSupport::info(parentWidget,
            QObject::tr("This attachment is empty."));
        return;
    }

    // Set suffix. Note that XXXXXX (exactly 6 X's all uppercase) gets replaced
    // with random letters to ensure the file does not already exist.
    auto* temp = new SharedTempFile(("XXXXXX" + att.extension()).c_str(),
        parentWidget);
    if (! temp->valid()) {
        ReginaSupport::warn(parentWidget,
            QObject::tr("<qt>I could not create the temporary "
            "file <i>%1</i>.</qt>").arg(temp->localFileName()));
        delete temp;
        return;
    }

    if (! att.save(static_cast<const char*>(QFile::encodeName(
            temp->localFileName())))) {
        ReginaSupport::warn(parentWidget,
            QObject::tr("<qt>An error occurred whilst writing the attachment "
            "to the temporary file <i>%1</i>.</qt>").
            arg(temp->localFileName()));
        delete temp;
        return;
    }

    temp->share();

    if (! QDesktopServices::openUrl(temp->url())) {
        ReginaSupport::sorry(parentWidget,
            QObject::tr("I was not able to find a suitable "
            "viewer for this attachment.<p>"));
        delete temp;
    }
}

