
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
        regina::Packet* packetTree, regina::Packet* defaultParent,
        PacketFilter* useFilter, const QString& dialogTitle) :
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
    chooser = new PacketChooser(tree, useFilter,
        PacketChooser::ROOT_AS_INSERTION_POINT, false, defaultParent);
    chooser->setWhatsThis(expln);
    parentStrip->addWidget(chooser, 1);

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
    regina::Packet* parentPacket = chooser->selectedPacket();
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
            arg(QString(parentPacket->humanLabel().c_str()).toHtmlEscaped()));
        return;
    }

    // Create the new packet.  Hide ourselves since this could take a while.
    newPacket = creator->createPacket(parentPacket, this);
    if (! newPacket)
        return;

    // Fix the new packet.
    if (newPacket->label().empty())
        newPacket->setLabel(newPacket->typeName());
    if (! newPacket->parent())
        parentPacket->insertChildLast(newPacket);

    accept();
}

