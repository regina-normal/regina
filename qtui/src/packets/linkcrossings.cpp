
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2017, Ben Burton                                   *
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

// Regina core includes:
#include "link/link.h"

// UI includes:
#include "linkcrossings.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QAction>
#include <QLabel>
#include <QToolBar>

using regina::Link;
using regina::Packet;

LinkCrossingsUI::LinkCrossingsUI(regina::Link* packet,
        PacketTabbedUI* useParentUI) :
        PacketEditorTab(useParentUI), link(packet) {
    ui = new QWidget();

    // Set up the link actions.
    QAction* actReflect = new QAction(this);
    actReflect->setText(tr("Re&flect"));
    // actReflect->setIcon(ReginaSupport::regIcon("reflectlink"));
    actReflect->setToolTip(tr("Reflect this link"));
    actReflect->setWhatsThis(tr("Reflect this link about some axis in "
        "the plane.  Every crossing will change sign, but its upper "
        "and lower strands will remain the same."));
    actionList.append(actReflect);
    connect(actReflect, SIGNAL(triggered()), this, SLOT(reflect()));

    QAction* actRotate = new QAction(this);
    actRotate->setText(tr("&Rotate"));
    // actRotate->setIcon(ReginaSupport::regIcon("rotatelink"));
    actRotate->setToolTip(tr("Rotate this link"));
    actRotate->setWhatsThis(tr("Rotate this link about some axis in "
        "the plane.  Every crossing will keep the same sign, but its upper "
        "and lower strands will be switched.<p>"
        "This operation simply produces a different diagram of the "
        "same link."));
    actionList.append(actRotate);
    connect(actRotate, SIGNAL(triggered()), this, SLOT(rotate()));

    // Tidy up.

    refresh();
}

void LinkCrossingsUI::fillToolBar(QToolBar* bar) {
}

regina::Packet* LinkCrossingsUI::getPacket() {
    return link;
}

QWidget* LinkCrossingsUI::getInterface() {
    return ui;
}

const QLinkedList<QAction*>& LinkCrossingsUI::getPacketTypeActions() {
    return actionList;
}

void LinkCrossingsUI::setReadWrite(bool readWrite) {
    // TODO.
}

void LinkCrossingsUI::refresh() {
    // TODO.
}

void LinkCrossingsUI::reflect() {
    link->reflect();
}

void LinkCrossingsUI::rotate() {
    link->rotate();
}

