
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

#include "codecchooser.h"
#include "exportdialog.h"
#include "reginasupport.h"
#include "../packetchooser.h"
#include "../packetfilter.h"

#include <QDialogButtonBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QTextDocument>
#include <QWhatsThis>

ExportDialog::ExportDialog(QWidget* parent, regina::NPacket* packetTree,
        regina::NPacket* defaultSelection, PacketFilter* useFilter,
        bool offerCodec, const QString& dialogTitle) :
        QDialog(parent),
        tree(packetTree), chosenPacket(0) {
    setWindowTitle(dialogTitle);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* hStrip = new QHBoxLayout;
    QLabel* label = new QLabel(tr("Data to export:"));
    hStrip->addWidget(label);
    chooser = new PacketChooser(tree, useFilter, false, defaultSelection);
    hStrip->addWidget(chooser, 1);
    QString expln = tr("Select the piece of data that you wish to export.");
    label->setWhatsThis(expln);
    chooser->setWhatsThis(expln);
    layout->addLayout(hStrip);

    if (offerCodec) {
        hStrip = new QHBoxLayout;
        label = new QLabel(tr("Text encoding:"));
        hStrip->addWidget(label);
        codecChooser = new CodecChooser();
        hStrip->addWidget(codecChooser, 1);
        QString expln = tr("Select the text encoding that you wish to "
            "use.  If you are not sure, just use the default UTF-8.");
        label->setWhatsThis(expln);
        codecChooser->setWhatsThis(expln);
        layout->addLayout(hStrip);
    } else
        codecChooser = 0;

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
            arg(Qt::escape(chosenPacket->getPacketLabel().c_str())));
        return;
    }

    accept();
}

QTextCodec* ExportDialog::selectedCodec() {
    return (codecChooser ? codecChooser->selectedCodec() : 0);
}

