
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
#include "reginasupport.h"

#include <QBoxLayout>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QTextDocument>
#include <QWhatsThis>

NewPacketDialog::NewPacketDialog(QWidget* parent, PacketCreator* newCreator,
        regina::NPacket* packetTree, regina::NPacket* defaultParent,
        PacketFilter* useFilter, const QString& dialogTitle,
        const QString& suggestedLabel) :
        QDialog(parent), //dialogTitle, Ok|Cancel, Ok, parent),
        creator(newCreator), tree(packetTree), newPacket(0) {
    setWindowTitle(dialogTitle);
    QVBoxLayout* layout = new QVBoxLayout(this);

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
        mainUI->setParent(this);
        // The outer layouts already provide margins.
        if (mainUI->layout())
            mainUI->layout()->setContentsMargins(0, 0, 0, 0);
        else
            mainUI->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(mainUI, 1);
    } else {
        layout->addStretch(1);
    }
    QDialogButtonBox *box = new QDialogButtonBox(
            QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    layout->addWidget(box);
    connect(box, SIGNAL(accepted()), this, SLOT(slotOk()));
    connect(box, SIGNAL(rejected()), this, SLOT(reject()));
}

NewPacketDialog::~NewPacketDialog() {
    delete creator;
}

bool NewPacketDialog::validate() {
    if (chooser->hasPackets())
        return true;
    hide();
    creator->explainNoParents();
    return false;
}

void NewPacketDialog::slotOk() {
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
            tr("Please select a different location in the tree for "
            "the new packet."),
            tr("<qt>The packet <i>%1</i> cannot act as a parent for "
            "the new packet.</qt>").
            arg(Qt::escape(parentPacket->getPacketLabel().c_str())));
        return;
    }

    // Check the label.
    QString useLabel = label->text().simplified();
    if (useLabel.isEmpty()) {
        ReginaSupport::info(this,
            tr("Please enter a label for the new packet."));
        return;
    }
    if (tree->findPacketLabel(std::string(useLabel.toAscii().constData()))) {
        ReginaSupport::info(this,
            tr("Another packet is already using this label."),
            tr("Each packet in your data file must have its own unique "
            "label.  I will suggest a different label that is not in use."));
        label->setText(tree->makeUniqueLabel(
            useLabel.toAscii().constData()).c_str());
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

    accept();
}

