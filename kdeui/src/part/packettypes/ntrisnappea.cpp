
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
#include "snappeacomponents.h"

#include <climits>
#include <cmath>
#include <klocale.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qwhatsthis.h>
#include <qwidgetstack.h>

// TODO: Add snappea calculations to the test suite
// TODO: Look over preconditions
// TODO: Look over display

using regina::NPacket;
using regina::NSnapPeaTriangulation;
using regina::NTriangulation;

NTriSnapPeaUI::NTriSnapPeaUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), reginaTri(packet), snappeaTri(0) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    layout->addStretch(3);

    QLabel* label = new QLabel(i18n("<qt><b>SnapPea Calculations</b></qt>"),
        ui);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    layout->addStretch(1);

    data = new QWidgetStack(ui);

    // Data for a null SnapPea triangulation:

    dataNull = new QWidget(data);
    QBoxLayout* nullLayout = new QVBoxLayout(dataNull, 5 /* margin */,
        0 /* spacing */);

    unavailable = new NoSnapPea(reginaTri, dataNull, 0, true);
    unavailable->setAlignment(Qt::AlignCenter);
    nullLayout->addWidget(unavailable);

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
        unavailable->refresh();
    } else {
        data->raiseWidget(dataValid);

        int places;
        double ans = snappeaTri->volume(places);

        // Can we say that the volume is approximately zero?
        bool approxZero = false;
        if (places >= 6 && fabs(ans) < 1e-7) {
            // The volume is fairly small and the accuracy is high.
            // Test whether zero lies comfortably within the estimated
            // margin of error.
            double epsilon = 1.0;
            for (int i = 0; i < places + 1; i++)
                epsilon /= 10;

            // Now we should have epsilon == 1e-(places+1).
            if (fabs(ans) < epsilon)
                approxZero = true;
        }

        if (approxZero) {
            // Zero is within the margin of error, and this margin of
            // error is small.  Report it as zero, with the exact result
            // beneath.
            volume->setText(i18n("Possibly zero\n(calculated %1,\n"
                "est. %2 places accuracy)").
                arg(snappeaTri->volume(places), 0, 'g', 9).arg(places));
        } else {
            volume->setText(i18n("%1\n(est. %2 places accuracy)").
                arg(snappeaTri->volume(places), 0, 'g', 9).arg(places));
        }

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
