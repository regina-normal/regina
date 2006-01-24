
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2006, Ben Burton                                   *
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

// Regina core includes:
#include "packet/ncontainer.h"

// UI includes:
#include "ncontainerui.h"

#include <klocale.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qwhatsthis.h>

using regina::NPacket;
using regina::NContainer;

NContainerUI::NContainerUI(NContainer* packet, PacketPane* enclosingPane) :
        PacketReadOnlyUI(enclosingPane), container(packet) {
    interface = new QVBox();

    interface->setStretchFactor(new QWidget(interface), 1);

    // Give the grid two extra stretchable columns on the outside.
    QWidget* grid = new QWidget(interface);
    QGridLayout* layout = new QGridLayout(grid, 2, 4, 5);
    layout->setColStretch(0, 1);
    layout->setColStretch(3, 1);

    QLabel* label;
    QString msg;

    label = new QLabel(i18n("Immediate children:"), grid);
    label->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout->addWidget(label, 0, 1, Qt::AlignRight);

    children = new QLabel(grid);
    children->setSizePolicy(
        QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout->addWidget(children, 0, 2, Qt::AlignRight);

    msg = i18n("Shows the number of immediate children of this "
        "container, i.e., the number of child packets that have this "
        "container as their immediate parent.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(children, msg);

    label = new QLabel(i18n("Total descendants:"), grid);
    label->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout->addWidget(label, 1, 1, Qt::AlignRight);

    descendants = new QLabel(grid);
    descendants->setSizePolicy(
        QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    layout->addWidget(descendants, 1, 2, Qt::AlignRight);

    msg = i18n("Shows the total number of descendants of this "
        "container, i.e., the number of children, grandchildren, "
        "great-grandchildren and so on.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(descendants, msg);

    interface->setStretchFactor(new QWidget(interface), 1);

    refresh();

    // Register ourselves as a listener for child changes so we can
    // update ourselves accordingly.
    container->listen(this);
}

NPacket* NContainerUI::getPacket() {
    return container;
}

QWidget* NContainerUI::getInterface() {
    return interface;
}

QString NContainerUI::getPacketMenuText() const {
    return i18n("&Container");
}

void NContainerUI::refresh() {
    children->setText(QString::number(container->getNumberOfChildren()));
    descendants->setText(QString::number(container->getNumberOfDescendants()));
    setDirty(false);
}

void NContainerUI::childWasAdded(NPacket*, NPacket*) {
    refresh();
}

void NContainerUI::childWasRemoved(NPacket*, NPacket*,
        bool inParentDestructor) {
    if (! inParentDestructor)
        refresh();
}

