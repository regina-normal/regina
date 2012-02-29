
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

#include "exportdialog.h"
#include "../packetchooser.h"
#include "../packetfilter.h"

#include <QDialogButtonBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QWhatsThis>

ExportDialog::ExportDialog(QWidget* parent, regina::NPacket* packetTree,
        regina::NPacket* defaultSelection, PacketFilter* useFilter,
        const QString& dialogTitle) :
        QDialog(parent),
        tree(packetTree), chosenPacket(0) {
    setWindowTitle(dialogTitle);
    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* chosenStrip = new QHBoxLayout;
    QLabel* label = new QLabel(tr("Data to export:"));
    chosenStrip->addWidget(label);
    chooser = new PacketChooser(tree, useFilter, false, defaultSelection);
    chosenStrip->addWidget(chooser, 1);
    QString expln = tr("Select the piece of data that you wish to export.");
    label->setWhatsThis(expln);
    chooser->setWhatsThis(expln);

    layout->addLayout(chosenStrip);
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
        QMessageBox *sorry = new QMessageBox(this);
        sorry->setText(sorry->tr(
        "No packets could be found that are suitable for export in this "
        "format."));
    return false;
}

void ExportDialog::slotOk() {
    // Get the selected packet.
    chosenPacket = chooser->selectedPacket();
    if (! chosenPacket) {
        QMessageBox *sorry = new QMessageBox(this);
        sorry->setText(sorry->tr(
            "No packet has been selected to export."));
        return;
    }
    PacketFilter* filter = chooser->getFilter();
    if (filter && ! filter->accept(chosenPacket)) {
        QMessageBox *sorry = new QMessageBox(this);
        sorry->setText(sorry->tr(
            "The packet %1 cannot be exported to this file format.").
            arg(chosenPacket->getPacketLabel().c_str()));
        return;
    }

    accept();
}

