
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2009, Ben Burton                                   *
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

#include <klocale.h>
#include <kmessagebox.h>
#include <qframe.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qwhatsthis.h>

#define HORIZONTAL_SPACING 10

ExportDialog::ExportDialog(QWidget* parent, regina::NPacket* packetTree,
        regina::NPacket* defaultSelection, PacketFilter* useFilter,
        const QString& dialogTitle) :
        KDialogBase(Plain, dialogTitle, Ok|Cancel, Ok, parent),
        tree(packetTree), chosenPacket(0) {
    QFrame* page = plainPage();
    QVBoxLayout* layout = new QVBoxLayout(page, 0, spacingHint());

    QHBox* chosenStrip = new QHBox(page);
    chosenStrip->setSpacing(HORIZONTAL_SPACING);
    layout->addWidget(chosenStrip);
    new QLabel(i18n("Data to export:"), chosenStrip);
    chooser = new PacketChooser(tree, useFilter, false, defaultSelection,
        chosenStrip);
    chosenStrip->setStretchFactor(chooser, 1);
    QWhatsThis::add(chosenStrip,
        i18n("Select the piece of data that you wish to export."));

    layout->addStretch(1);
}

bool ExportDialog::validate() {
    if (chooser->hasPackets())
        return true;
    KMessageBox::sorry(this, i18n(
        "No packets could be found that are suitable for export in this "
        "format."));
    return false;
}

void ExportDialog::slotOk() {
    // Get the selected packet.
    chosenPacket = chooser->selectedPacket();
    if (! chosenPacket) {
        KMessageBox::error(this, i18n(
            "No packet has been selected to export."));
        return;
    }
    PacketFilter* filter = chooser->getFilter();
    if (filter && ! filter->accept(chosenPacket)) {
        KMessageBox::error(this, i18n(
            "The packet %1 cannot be exported to this file format.").
            arg(chosenPacket->getPacketLabel().c_str()));
        return;
    }

    // We're done!
    KDialogBase::slotOk();
}

#include "exportdialog.moc"
