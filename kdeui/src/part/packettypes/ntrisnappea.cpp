
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2004, Ben Burton                                   *
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
#include "snappea/nsnappeatriangulation.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntrisnappea.h"

#include <klocale.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qwhatsthis.h>
#include <qwidgetstack.h>

using regina::NPacket;
using regina::NSnapPeaTriangulation;
using regina::NTriangulation;

NTriSnapPeaUI::NTriSnapPeaUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), reginaTri(packet), snappeaTri(0) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    layout->addStretch(3);

    QLabel* label = new QLabel(i18n("SnapPea Calculations"), ui);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    layout->addStretch(1);

    data = new QWidgetStack(ui);

    // Data for a null SnapPea triangulation:

    dataNull = new QWidget(data);
    QBoxLayout* nullLayout = new QVBoxLayout(dataNull);

    label = new QLabel(i18n("TODO"), dataNull);
    label->setAlignment(Qt::AlignCenter);
    nullLayout->addWidget(label);

    // Data for a non-null SnapPea triangulation:

    dataValid = new QWidget(data);
    QGridLayout* validGrid = new QGridLayout(dataValid, 1 /* rows */, 5, 5);
    validGrid->setColStretch(0, 1);
    validGrid->setColSpacing(2, 5); // Horizontal gap
    validGrid->setColStretch(4, 1);

    QString msg;

    label = new QLabel(i18n("Volume:"), dataValid);
    validGrid->addWidget(label, 0, 1);
    volume = new QLabel(dataValid);
    validGrid->addWidget(volume, 0, 3);
    msg = i18n("The volume of the underlying 3-manifold.  The estimated "
        "number of decimal places of accuracy is also shown.");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(volume, msg);

    // Finish off.

    layout->addWidget(data);
    layout->addStretch(3);
}

NTriSnapPeaUI::~NTriSnapPeaUI() {
    if (snappeaTri)
        delete snappeaTri;
}

regina::NPacket* NTriSnapPeaUI::getPacket() {
    return reginaTri;
}

QWidget* NTriSnapPeaUI::getInterface() {
    return ui;
}

void NTriSnapPeaUI::refresh() {
    if (snappeaTri)
        delete snappeaTri;

    snappeaTri = new NSnapPeaTriangulation(*reginaTri);

    if (snappeaTri->isNull()) {
        data->raiseWidget(dataNull);
    } else {
        data->raiseWidget(dataValid);

        int places;
        volume->setText(i18n("%1  (%2 places accuracy)").
            arg(snappeaTri->volume(places), 0, 'g', 9).arg(places));
        volume->setEnabled(true);
    }
}

void NTriSnapPeaUI::editingElsewhere() {
    data->raiseWidget(dataValid);

    QString msg(i18n("Editing..."));
    volume->setText(msg);
    volume->setEnabled(false);
}

#include "ntrisnappea.moc"
