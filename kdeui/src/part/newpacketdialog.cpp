
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

#include "newpacketdialog.h"
#include "packetchooser.h"
#include "packetcreator.h"
#include "packetfilter.h"

#include <QBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QWhatsThis>

NewPacketDialog::NewPacketDialog(QWidget* parent, PacketCreator* newCreator,
        regina::NPacket* packetTree, regina::NPacket* defaultParent,
        PacketFilter* useFilter, const QString& dialogTitle,
        const QString& suggestedLabel) :
        QDialog(parent), //dialogTitle, Ok|Cancel, Ok, parent),
        creator(newCreator), tree(packetTree), newPacket(0) {
    setWindowTitle(dialogTitle);

    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0); // Margins come from the dialog.

    QHBoxLayout* parentStrip = new QHBoxLayout();
    layout->addLayout(parentStrip);
    QString parentPrompt = newCreator->parentPrompt();
    if (parentPrompt.isNull())
        parentPrompt = tr("Create beneath:");
    QString expln = newCreator->parentWhatsThis();
    if (expln.isNull())
        expln = tr("Specifies where in the packet tree the new "
            "packet will be placed.");
    QLabel* createBeneath = new QLabel(parentPrompt);
    createBeneath->setWhatsThis(expln);
    parentStrip->addWidget(createBeneath);
    chooser = new PacketChooser(tree, useFilter, false, defaultParent);
    chooser->setWhatsThis(expln);
    parentStrip->addWidget(chooser, 1);

    QHBoxLayout* labelStrip = new QHBoxLayout();
    layout->addLayout(labelStrip);
    expln = tr("The label that will be assigned to the new packet.");
    QLabel* newlabel = new QLabel(tr("Label:"));
    newlabel->setWhatsThis(expln);
    labelStrip->addWidget(newlabel);
    label = new QLineEdit(tree->makeUniqueLabel(
        suggestedLabel.toAscii().constData()).c_str());
    label->setWhatsThis(expln);
    labelStrip->addWidget(label, 1);

    QWidget* mainUI = creator->getInterface();
    if (mainUI) {
        mainUI->setParent(page);
        // The outer layouts already provide margins.
        mainUI->layout()->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(mainUI, 1);
    } else {
        layout->addStretch(1);
    }
    QDialogButtonBox *box = new QDialogButtonBox(
            QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    layout->addWidget(box);
    connect(box, SIGNAL(accepted()), this, SLOT(slotOk()));
}

NewPacketDialog::~NewPacketDialog() {
    delete creator;
}

bool NewPacketDialog::validate() {
    if (chooser->hasPackets())
        return true;
    QMessageBox::warning(this,tr("No suitable parent"), 
        tr("No suitable parent packets could be found.\n"
        "Some packets have particular requirements of their parents.  "
        "For instance, a list of normal surfaces or angle structures must "
        "be created beneath the triangulation in which they live.\n"
        "See the users' handbook for further information."));
    return false;
}

void NewPacketDialog::slotOk() {
    // Get the parent packet.
    regina::NPacket* parentPacket = chooser->selectedPacket();
    if (! parentPacket) {
        QMessageBox::warning(this, tr("No parent selected"),
            tr("No parent packet has been selected."));
        return;
    }
    PacketFilter* filter = chooser->getFilter();
    if (filter && ! filter->accept(parentPacket)) {
        QMessageBox::warning(this, tr("Not a suitable parent"),
            tr("The packet %1 is not capable of acting as a parent for "
            "the new packet.").arg(parentPacket->getPacketLabel().c_str()));
        return;
    }

    // Check the label.
    QString useLabel = label->text().simplified();
    if (useLabel.isEmpty()) {
        QMessageBox::warning(this, tr("Empty label"),
            tr("The packet label cannot be empty."));
        return;
    }
    if (tree->findPacketLabel(std::string(useLabel.toAscii().constData()))) {
        QMessageBox::warning(this, tr("Name already in use"),
            tr("There is already a packet labelled %1.").arg(useLabel));
        label->setText(QString::fromAscii(
              tree->makeUniqueLabel(
                  std::string(useLabel.toAscii().constData())).c_str()));
        return;
    }

    // Create the new packet.  Hide ourselves since this could take a
    // while.
    newPacket = creator->createPacket(parentPacket, this);
    if (! newPacket)
        return;

    // Fix the new packet.
    newPacket->setPacketLabel(std::string(useLabel.toAscii().constData()));
    if (! newPacket->getTreeParent())
        parentPacket->insertChildLast(newPacket);
}

