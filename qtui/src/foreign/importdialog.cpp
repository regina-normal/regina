
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

#include "importdialog.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../packetchooser.h"
#include "../packetfilter.h"

#include <QDialogButtonBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextDocument>
#include <QWhatsThis>

ImportDialog::ImportDialog(QWidget* parent, regina::Packet* importedData,
        regina::Packet* packetTree, regina::Packet* defaultParent,
        PacketFilter* useFilter, bool useCodec, const QString& dialogTitle) :
        QDialog(parent), tree(packetTree), newTree(importedData) {
    setWindowTitle(dialogTitle);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* hStrip = new QHBoxLayout();
    layout->addLayout(hStrip);
    QString expln = tr("Select where in the packet tree "
        "the new data should be imported.  The imported data will be "
        "made a new child of the selected packet.");
    QLabel* l = new QLabel(tr("Import beneath:"));
    l->setWhatsThis(expln);
    hStrip->addWidget(l);
    chooser = new PacketChooser(tree, useFilter,
        PacketChooser::ROOT_AS_INSERTION_POINT, false, defaultParent);
    chooser->setWhatsThis(expln);
    hStrip->addWidget(chooser, 1);

    hStrip = new QHBoxLayout();
    layout->addLayout(hStrip);
    expln = tr("Select a packet label for the new "
        "imported data.  This will become the label of the first packet that "
        "is imported.");
    l = new QLabel(tr("Label:"));
    l->setWhatsThis(expln);
    hStrip->addWidget(l);
    label = new QLineEdit(newTree->label().c_str());
    label->setWhatsThis(expln);
    hStrip->addWidget(label, 1);

    if (useCodec) {
        hStrip = new QHBoxLayout;
        l = new QLabel(tr("<qt>Text encoding: %1</qt>").
            arg(QString(ReginaPrefSet::global().fileImportExportCodec)));
        hStrip->addWidget(l);
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

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotOk()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

bool ImportDialog::validate() {
    if (chooser->hasPackets())
        return true;
    ReginaSupport::sorry(this,
        tr("I could not finish the import."),
        tr("<qt>There is no suitable location in this packet tree "
        "for the imported data.<p>"
        "Some packets have constraints on their location in the tree; "
        "for instance, normal surface or angle structure lists must "
        "be placed beneath the corresponding triangulation.  "
        "See the users' handbook for further information.</qt>"));
    return false;
}

void ImportDialog::slotOk() {
    // Get the parent packet.
    regina::Packet* parentPacket = chooser->selectedPacket();
    if (! parentPacket) {
        ReginaSupport::info(this,
            tr("Please select a parent packet."));
        return;
    }
    PacketFilter* filter = chooser->getFilter();
    if (filter && ! filter->accept(parentPacket)) {
        ReginaSupport::info(this,
            tr("Please select a different location in the tree "
            "for the import."),
            tr("<qt>The packet <i>%1</i> cannot act as a parent for "
            "this imported data.</qt>").
            arg(QString(parentPacket->humanLabel().c_str()).toHtmlEscaped()));
        return;
    }

    // Check the label.
    QString useLabel = label->text().trimmed();
    QByteArray ascii = useLabel.toUtf8();
    if (useLabel.isEmpty()) {
        ReginaSupport::info(this,
            tr("Please enter a label for the new packet."));
        return;
    }

    // Insert the imported data into the packet tree.
    newTree->setLabel(ascii.constData());
    parentPacket->insertChildLast(newTree);

    accept();
}

void ImportDialog::slotEncodingInfo() {
    ReginaSupport::info(this,
        tr("<qt>I will assume that any international symbols "
            "are encoding using the <b>%1</b> encoding.</qt>").arg(
            QString(ReginaPrefSet::global().fileImportExportCodec)),
        tr("<qt>This is only relevant if you use letters or symbols "
            "that are not found on a typical English keyboard.<p>"
            "If you wish to use a different encoding, you can "
            "change this through Regina's settings.  If you are "
            "not sure what encoding to use, the default encoding "
            "<b>UTF-8</b> is safe.</qt>"));
}

