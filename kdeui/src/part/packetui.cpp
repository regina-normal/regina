
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

// Regina core includes:
#include "packet/npacket.h"

// UI includes:
#include "packetmanager.h"
#include "packetui.h"

#include <kmessagebox.h>
#include <qlabel.h>

using regina::NPacket;

PacketHeader::PacketHeader(NPacket* pkt, QWidget* parent,
        const char* name) : QHBox(parent, name), packet(pkt) {
    icon = new QLabel(this);
    icon->setPixmap(PacketManager::iconBar(packet));

    title = new QLabel(packet->getFullName().c_str(), this);
    title->setAlignment(AlignCenter);
    setStretchFactor(title, 1);

    setFrameStyle(QFrame::Box | QFrame::Sunken);
    // setMidLineWidth(1);
}

void PacketHeader::refresh() {
    // We assume the packet has not changed type!
    title->setText(packet->getFullName().c_str());
}

PacketPane::PacketPane(ReginaPart* newPart, NPacket* newPacket,
        QWidget* parent, const char* name) : QVBox(parent, name),
        part(newPart), frame(0) {
    header = new PacketHeader(newPacket, this);

    QWidget* mainUI = new QWidget(this);

    QHBox* footer = new QHBox(this);
    new QLabel("Footer", footer);

    setStretchFactor(mainUI, 1);
}

bool PacketPane::queryClose() {
    KMessageBox::sorry(this, QString("Just testing!."));
    return true;
}

PacketWindow::PacketWindow(PacketPane* newPane, QWidget* parent) :
        KMainWindow(parent, "Packet#"), heldPane(newPane) {
    // Resize ourselves nicely.
    if (! initialGeometrySet())
        resize(400, 400);

    // Set up the widgets.
    newPane->reparent(this, QPoint(0, 0));
    setCentralWidget(newPane);
    newPane->show();
}

bool PacketWindow::queryClose() {
    // TODO
    return heldPane->queryClose();
}

#include "packetui.moc"
