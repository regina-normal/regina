
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
#include <QFileDialog>
#include <QLabel>
#include <QLayout>
#include <QLocale>
#include <QProcess>
#include <QPushButton>
#include <QTextDocument>
#include <QUrl>

using regina::Attachment;

AttachmentUI::AttachmentUI(Attachment* packet, PacketPane* enclosingPane) :
        PacketReadOnlyUI(enclosingPane), attachment(packet) {
    ui = new QWidget();
    auto* layout = new QVBoxLayout(ui);
    layout->addStretch(1);

    filename = new QLabel();
    filename->setAlignment(Qt::AlignCenter);
    layout->addWidget(filename);
    filename->setWhatsThis(tr(
        "Shows the name of the file that has been attached."));

    size = new QLabel();
    size->setAlignment(Qt::AlignCenter);
    layout->addWidget(size);
    size->setWhatsThis(tr(
        "Shows the size of the file that has been attached."));

    layout->addStretch(1);

    auto* buttons = new QHBoxLayout();
    layout->addLayout(buttons);

    buttons->addStretch(1);

    viewButton = new QPushButton(tr("View"));
    viewButton->setToolTip(tr("Open this attachment in an external viewer"));
    viewButton->setWhatsThis(tr(
        "<qt>Open this attachment in an external viewer.  "
        "The viewer application will be chosen by your desktop environment.<p>"
        "Any changes that you make in this external viewer will "
        "<i>not</i> be saved back to Regina."));
    connect(viewButton, SIGNAL(clicked()), this, SLOT(view()));
    buttons->addWidget(viewButton);

    buttons->addStretch(1);
    saveButton = new QPushButton(tr("Save File"));
    saveButton->setToolTip(tr("Save this attachment as a separate file"));
    saveButton->setWhatsThis(tr("Save this attachment as a regular file.  "
        "This will allow you to access the attachment from outside Regina."));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
    buttons->addWidget(saveButton);

    buttons->addStretch(1);

    refresh();
}

regina::Packet* AttachmentUI::getPacket() {
    return attachment;
}

QWidget* AttachmentUI::getInterface() {
    return ui;
}

QString AttachmentUI::getPacketMenuText() const {
    return tr("&Attachment");
}

void AttachmentUI::refresh() {
    if (attachment->isNull()) {
        filename->setText(tr("(none)"));
        size->setText(tr("(empty)"));

        viewButton->setEnabled(false);
        saveButton->setEnabled(false);
    } else {
        filename->setText(tr("Filename: %1").arg(
            QFile::decodeName(attachment->filename().c_str())));

#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
        QLocale locale = ui->locale();
        size->setText(locale.formattedDataSize(attachment->size(),
            2 /* precision */, QLocale::DataSizeSIFormat));
#else
        // Just write something brutal, since almost everyone should
        // have Qt 5.10 or above by now.  Even debian buster had Qt 5.11.
        // On ubuntu you have to go back to bionic (18.04 LTS) to see Qt 5.9.
        size->setText(tr("%1 bytes").arg(attachment->size()));
#endif

        viewButton->setEnabled(true);
        saveButton->setEnabled(true);
    }
}

void AttachmentUI::save() {
    if (attachment->isNull())
        return;

    QString file = QFileDialog::getSaveFileName(ui, tr("Save Attachment"),
        QFile::decodeName(attachment->filename().c_str()));
    if (file.isEmpty())
        return;

    if (! attachment->save(
            static_cast<const char*>(QFile::encodeName(file)))) {
        ReginaSupport::warn(ui, QObject::tr("I could not save the attachment."),
            QObject::tr("<qt>An unknown error occurred, probably related "
            "to file I/O.  Please check that you have permissions to write "
            "to the file <tt>%1</tt>.</qt>").arg(file.toHtmlEscaped()));
    }
}

void AttachmentUI::view() {
    if (attachment->isNull())
        return;

    // Write the attachment to a temporary file.
    const char* data = attachment->data();
    if (! data) {
        ReginaSupport::info(ui, tr("This attachment is empty."));
        return;
    }

    // Set suffix. Note that XXXXXX (exactly 6 X's all uppercase) gets replaced
    // with random letters to ensure the file does not already exist.
    auto* temp = new SharedTempFile(
        ("XXXXXX" + attachment->extension()).c_str(), ui);
    if (! temp->valid()) {
        ReginaSupport::warn(ui, tr("<qt>I could not create the temporary "
            "file <i>%1</i>.</qt>").arg(temp->localFileName()));
        delete temp;
        return;
    }

    if (! attachment->save(static_cast<const char*>(QFile::encodeName(
            temp->localFileName())))) {
        ReginaSupport::warn(ui, tr("<qt>An error occurred whilst "
            "writing the attachment to the temporary file <i>%1</i>.</qt>").
            arg(temp->localFileName()));
        delete temp;
        return;
    }

    temp->share();

    if (! QDesktopServices::openUrl(temp->url())) {
        ReginaSupport::sorry(ui, tr("I was not able to find a suitable "
            "viewer for this attachment.<p>"));
        delete temp;
    }
}

