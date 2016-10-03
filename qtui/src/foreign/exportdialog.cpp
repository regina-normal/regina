
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

#include "packet/packet.h"

#include "exportdialog.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../packetchooser.h"
#include "../packetfilter.h"

#include <QDialogButtonBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QTextDocument>
#include <QWhatsThis>

ExportDialog::ExportDialog(QWidget* parent, regina::Packet* packetTree,
        regina::Packet* defaultSelection, PacketFilter* useFilter,
        bool useCodec, const QString& dialogTitle) :
        QDialog(parent),
        tree(packetTree), chosenPacket(0) {
    setWindowTitle(dialogTitle);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* hStrip = new QHBoxLayout;
    QLabel* label = new QLabel(tr("Data to export:"));
    hStrip->addWidget(label);
    chooser = new PacketChooser(tree, useFilter,
        PacketChooser::ROOT_AS_SUBTREE, false, defaultSelection);
    hStrip->addWidget(chooser, 1);
    QString expln = tr("Select the piece of data that you wish to export.");
    label->setWhatsThis(expln);
    chooser->setWhatsThis(expln);
    layout->addLayout(hStrip);

    if (useCodec) {
        hStrip = new QHBoxLayout;
        label = new QLabel(tr("<qt>Text encoding: %1</qt>").
            arg(QString(ReginaPrefSet::global().fileImportExportCodec)));
        hStrip->addWidget(label);
        QPushButton* btn = new QPushButton(tr("Learn more..."));
        hStrip->addWidget(btn);
        hStrip->addStretch(1);
        layout->addLayout(hStrip);

        connect(btn, SIGNAL(clicked(bool)), this, SLOT(slotEncodingInfo()));
    }

    layout->addStretch(1);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox,SIGNAL(accepted()), this, SLOT(slotOk()));
    connect(buttonBox,SIGNAL(rejected()), this, SLOT(reject()));
}

bool ExportDialog::validate() {
    if (chooser->hasPackets())
        return true;
    hide();
    ReginaSupport::info(this,
        tr("This file contains no data that I can export in this format."));
    return false;
}

void ExportDialog::slotOk() {
    // Get the selected packet.
    chosenPacket = chooser->selectedPacket();
    if (! chosenPacket) {
        ReginaSupport::info(this,
            tr("Please select a packet to export."));
        return;
    }
    PacketFilter* filter = chooser->getFilter();
    if (filter && ! filter->accept(chosenPacket)) {
        ReginaSupport::sorry(this,
            tr("Please select a different packet."),
            tr("<qt>The packet <i>%1</i> cannot "
            "be exported to this file format.</qt>").
            arg(QString(chosenPacket->humanLabel().c_str()).toHtmlEscaped()));
        return;
    }

    accept();
}

void ExportDialog::slotEncodingInfo() {
    ReginaSupport::info(this,
        tr("<qt>I will encode any international symbols "
            "using the <b>%1</b> encoding.</qt>").arg(
            QString(ReginaPrefSet::global().fileImportExportCodec)),
        tr("<qt>This is only relevant if you use letters or symbols "
            "that are not found on a typical English keyboard.<p>"
            "If you wish to use a different encoding, you can "
            "change this through Regina's settings.  If you are "
            "not sure what encoding to use, the default encoding "
            "<b>UTF-8</b> is safe.</qt>"));
}

