
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

#include "newpacketdialog.h"
#include "packetchooser.h"
#include "packetcreator.h"
#include "packetfilter.h"

#include <klocale.h>
#include <kmessagebox.h>
#include <qframe.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>

#define HORIZONTAL_SPACING 10

// TODO: Display the newly created packet in the tree and in a packet pane.

NewPacketDialog::NewPacketDialog(QWidget* parent, PacketCreator* newCreator,
        regina::NPacket* packetTree, regina::NPacket* defaultParent,
        PacketFilter* useFilter, const QString& dialogTitle,
        const QString& suggestedLabel) :
        KDialogBase(Plain, dialogTitle, Ok|Cancel, Ok, parent),
        creator(newCreator), tree(packetTree) {
    QFrame* page = plainPage();
    QVBoxLayout* layout = new QVBoxLayout(page, 0, spacingHint());

    QHBox* parentStrip = new QHBox(page);
    parentStrip->setSpacing(HORIZONTAL_SPACING);
    layout->addWidget(parentStrip);
    new QLabel(i18n("Create beneath:"), parentStrip);
    chooser = new PacketChooser(tree, useFilter, false, defaultParent,
        parentStrip);
    parentStrip->setStretchFactor(chooser, 1);

    QHBox* labelStrip = new QHBox(page);
    labelStrip->setSpacing(HORIZONTAL_SPACING);
    layout->addWidget(labelStrip);
    new QLabel(i18n("Label:"), labelStrip);
    label = new QLineEdit(
        tree->makeUniqueLabel(suggestedLabel.ascii()).c_str(), labelStrip);
    labelStrip->setStretchFactor(label, 1);

    QWidget* mainUI = creator->getInterface();
    if (mainUI) {
        mainUI->reparent(page, QPoint(0, 0));
        layout->addWidget(mainUI);
        layout->setStretchFactor(mainUI, 1);
    } else {
        layout->addStretch(1);
    }
}

NewPacketDialog::~NewPacketDialog() {
    delete creator;
}

void NewPacketDialog::slotOk() {
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
            "the new packet.").arg(parentPacket->getPacketLabel().c_str()));
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

    // Create the new packet.  Hide ourselves since this could take a
    // while.
    hide();
    regina::NPacket* newPacket = creator->createPacket(parentPacket, this);
    if (! newPacket) {
        show();
        return;
    }

    // Fix the new packet.
    newPacket->setPacketLabel(useLabel);
    if (! newPacket->getTreeParent())
        parentPacket->insertChildLast(newPacket);

    // And we're done!
    KDialogBase::slotOk();
}

#include "newpacketdialog.moc"
