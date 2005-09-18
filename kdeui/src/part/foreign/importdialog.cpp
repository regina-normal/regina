
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2005, Ben Burton                                   *
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
#include "../packetchooser.h"
#include "../packetfilter.h"

#include <klocale.h>
#include <kmessagebox.h>
#include <qframe.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qwhatsthis.h>

#define HORIZONTAL_SPACING 10

ImportDialog::ImportDialog(QWidget* parent, regina::NPacket* importedData,
        regina::NPacket* packetTree, regina::NPacket* defaultParent,
        PacketFilter* useFilter, const QString& dialogTitle) :
        KDialogBase(Plain, dialogTitle, Ok|Cancel, Ok, parent),
        tree(packetTree), newTree(importedData) {
    QFrame* page = plainPage();
    QVBoxLayout* layout = new QVBoxLayout(page, 0, spacingHint());

    QHBox* parentStrip = new QHBox(page);
    parentStrip->setSpacing(HORIZONTAL_SPACING);
    layout->addWidget(parentStrip);
    new QLabel(i18n("Import beneath:"), parentStrip);
    chooser = new PacketChooser(tree, useFilter, false, defaultParent,
        parentStrip);
    parentStrip->setStretchFactor(chooser, 1);
    QWhatsThis::add(parentStrip, i18n("Select where in the packet tree "
        "the new data should be imported.  The imported data will be "
        "made a new child of the selected packet."));

    QHBox* labelStrip = new QHBox(page);
    labelStrip->setSpacing(HORIZONTAL_SPACING);
    layout->addWidget(labelStrip);
    new QLabel(i18n("Label:"), labelStrip);
    label = new QLineEdit(
        tree->makeUniqueLabel(newTree->getPacketLabel()).c_str(), labelStrip);
    labelStrip->setStretchFactor(label, 1);
    QWhatsThis::add(labelStrip, i18n("Select a packet label for the new "
        "imported data.  This will become the label of the first packet that "
        "is imported."));

    layout->addStretch(1);
}

bool ImportDialog::validate() {
    if (chooser->hasPackets())
        return true;
    KMessageBox::sorry(this, i18n(
        "No suitable parent packets could be found for the imported data.\n"
        "Some packets have particular requirements of their parents.  "
        "For instance, a list of normal surfaces or angle structures must "
        "be imported beneath the triangulation in which they live.\n"
        "See the reference manual for further information."));
    return false;
}

void ImportDialog::slotOk() {
    // Get the parent packet.
    regina::NPacket* parentPacket = chooser->selectedPacket();
    if (! parentPacket) {
        KMessageBox::error(this, i18n(
            "No parent packet has been selected."));
        return;
    }
    PacketFilter* filter = chooser->getFilter();
    if (filter && ! filter->accept(parentPacket)) {
        KMessageBox::error(this, i18n(
            "The packet %1 is not capable of acting as a parent for "
            "the imported data.").arg(parentPacket->getPacketLabel().c_str()));
        return;
    }

    // Check the label.
    std::string useLabel = label->text().stripWhiteSpace().ascii();
    if (useLabel.empty()) {
        KMessageBox::error(this, i18n(
            "The packet label cannot be empty.").arg(useLabel.c_str()));
        return;
    }
    if (tree->findPacketLabel(useLabel)) {
        KMessageBox::error(this, i18n(
            "There is already a packet labelled %1.").arg(useLabel.c_str()));
        label->setText(tree->makeUniqueLabel(useLabel).c_str());
        return;
    }

    // Insert the imported data into the packet tree.
    newTree->setPacketLabel(useLabel);
    newTree->makeUniqueLabels(tree);
    parentPacket->insertChildLast(newTree);

    // And we're done!
    KDialogBase::slotOk();
}

#include "importdialog.moc"
