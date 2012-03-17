
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

#include "packet/npacket.h"

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

ImportDialog::ImportDialog(QWidget* parent, regina::NPacket* importedData,
        regina::NPacket* packetTree, regina::NPacket* defaultParent,
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
    chooser = new PacketChooser(tree, useFilter, false, defaultParent);
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
    label = new QLineEdit(
        tree->makeUniqueLabel(newTree->getPacketLabel()).c_str());
    label->setWhatsThis(expln);
    hStrip->addWidget(label, 1);

    if (useCodec) {
        hStrip = new QHBoxLayout;
        l = new QLabel(tr("<qt>Text encoding: %1</qt>").
            arg(QString::fromAscii(
                ReginaPrefSet::global().fileImportExportCodec)));
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
    regina::NPacket* parentPacket = chooser->selectedPacket();
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
            arg(Qt::escape(parentPacket->getPacketLabel().c_str())));
        return;
    }

    // Check the label.
    QString useLabel = label->text().trimmed();
    QByteArray ascii = useLabel.toAscii();
    if (useLabel.isEmpty()) {
        ReginaSupport::info(this,
            tr("Please enter a label for the new packet."));
        return;
    }
    if (tree->findPacketLabel(ascii.constData())) {
        ReginaSupport::sorry(this,
            tr("Another packet is already using this label."),
            tr("Each packet in your data file must have its own unique "
            "label.  I will suggest a different label that is not in use."));
        label->setText(tree->makeUniqueLabel(ascii.constData()).c_str());
        return;
    }

    // Insert the imported data into the packet tree.
    newTree->setPacketLabel(ascii.constData());
    newTree->makeUniqueLabels(tree);
    parentPacket->insertChildLast(newTree);

    accept();
}

void ImportDialog::slotEncodingInfo() {
    ReginaSupport::info(this,
        tr("<qt>I will assume that any international symbols "
            "are encoding using the <b>%1</b> encoding.</qt>").arg(
            QString::fromAscii(ReginaPrefSet::global().fileImportExportCodec)),
        tr("<qt>This is only relevant if you use letters or symbols "
            "that are not found on a typical English keyboard.<p>"
            "If you wish to use a different encoding, you can "
            "change this through Regina's settings.  If you are "
            "not sure what encoding to use, the default encoding "
            "<b>UTF-8</b> is safe.</qt>"));
}

