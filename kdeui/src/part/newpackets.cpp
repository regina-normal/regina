
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

#include "packet/ncontainer.h"
#include "packet/nscript.h"
#include "packet/ntext.h"

#include "newpacketdialog.h"
#include "packetcreator.h"
#include "packettreeview.h"
#include "reginapart.h"

#include <klocale.h>

void ReginaPart::newAngleStructures() {
    unimplemented();
}

void ReginaPart::newContainer() {
    newPacket(new BasicPacketCreator<regina::NContainer>(),
        i18n("New Container"), i18n("Container"));
}

void ReginaPart::newFilter() {
    unimplemented();
}

void ReginaPart::newNormalSurfaces() {
    unimplemented();
}

void ReginaPart::newScript() {
    newPacket(new BasicPacketCreator<regina::NScript>(),
        i18n("New Script"), i18n("Script"));
}

void ReginaPart::newText() {
    newPacket(new BasicPacketCreator<regina::NText>(),
        i18n("New Text Packet"), i18n("Text"));
}

void ReginaPart::newTriangulation() {
    unimplemented();
}

void ReginaPart::newPacket(PacketCreator* creator, const QString& dialogTitle,
        const QString& suggestedLabel) {
    NewPacketDialog dlg(widget(), creator, packetTree,
        treeView->selectedPacket(), 0, dialogTitle, suggestedLabel);
    if (dlg.exec() == QDialog::Accepted) {
        regina::NPacket* newPacket = dlg.createdPacket();
        if (newPacket) {
            QListViewItem* item = treeView->find(newPacket);
            if (item)
                treeView->ensureItemVisible(item);
            packetView(newPacket);
        }
    }
}

