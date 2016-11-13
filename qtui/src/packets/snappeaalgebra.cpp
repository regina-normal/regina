
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

// Regina core includes:
#include "algebra/grouppresentation.h"
#include "algebra/markedabeliangroup.h"
#include "maths/numbertheory.h"
#include "snappea/snappeatriangulation.h"

// UI includes:
#include "columnlayout.h"
#include "groupwidget.h"
#include "snappeaalgebra.h"
#include "reginaprefset.h"

#include <QLabel>

using regina::Packet;
using regina::SnapPeaTriangulation;

SnapPeaAlgebraUI::SnapPeaAlgebraUI(regina::SnapPeaTriangulation* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();

    ColumnLayout* master = new ColumnLayout(ui);

    QVBoxLayout* layout;

    // Filled:

    layout = new QVBoxLayout();

    layout->addStretch(1);

    filledH1Title = new QLabel(tr("<qt><u>Homology</u></qt>"));
    filledH1Title->setAlignment(Qt::AlignCenter);
    layout->addWidget(filledH1Title);

    filledH1 = new QLabel();
    filledH1->setAlignment(Qt::AlignCenter);
    filledH1->setWhatsThis(tr("The first homology group of the manifold, "
        "with all Dehn fillings applied."));
    layout->addWidget(filledH1);

    layout->addStretch(1);

    filledFundGroupTitle = new QLabel(tr("<qt><u>Fundamental group</u></qt>"));
    filledFundGroupTitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(filledFundGroupTitle);

    filledFundGroup = new GroupWidget(false, false);
    filledFundGroup->setWhatsThis(tr("The fundamental group of the "
        "manifold, with all Dehn fillings applied."));
    layout->addWidget(filledFundGroup, 8);

    master->addLayout(layout, tr("Filled manifold"));

    // Unfilled:

    layout = new QVBoxLayout();

    layout->addStretch(1);

    unfilledH1Title = new QLabel(tr("<qt><u>Homology</u></qt>"));
    unfilledH1Title->setAlignment(Qt::AlignCenter);
    layout->addWidget(unfilledH1Title);

    unfilledH1 = new QLabel();
    unfilledH1->setAlignment(Qt::AlignCenter);
    unfilledH1->setWhatsThis(tr("The first homology group of the manifold, "
        "ignoring all Dehn fillings."));
    layout->addWidget(unfilledH1);

    layout->addStretch(1);

    unfilledFundGroupTitle = new QLabel(tr("<qt><u>Fundamental group</u></qt>"));
    unfilledFundGroupTitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(unfilledFundGroupTitle);

    unfilledFundGroup = new GroupWidget(false, false);
    unfilledFundGroup->setWhatsThis(tr("The fundamental group of the "
        "manifold, ignoring all Dehn fillings."));
    layout->addWidget(unfilledFundGroup, 8);

    master->addLayout(layout, tr("Unfilled manifold"));

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

regina::Packet* SnapPeaAlgebraUI::getPacket() {
    return tri;
}

QWidget* SnapPeaAlgebraUI::getInterface() {
    return ui;
}

void SnapPeaAlgebraUI::refresh() {
    bool unicode = ReginaPrefSet::global().displayUnicode;

    if (tri->isNull()) {
        filledH1Title->hide();
        filledH1->hide();
        filledFundGroupTitle->hide();
        filledFundGroup->hide();
        unfilledH1Title->hide();
        unfilledH1->hide();
        unfilledFundGroupTitle->hide();
        unfilledFundGroup->hide();
        return;
    }

    if (tri->countFilledCusps() == 0) {
        filledH1Title->hide();
        filledH1->hide();
        filledFundGroupTitle->hide();
        filledFundGroup->hide();
    } else {
        if (unicode)
            filledH1->setText(tri->homologyFilled()->utf8().c_str());
        else
            filledH1->setText(tri->homologyFilled()->str().c_str());
        filledFundGroup->refresh(tri->fundamentalGroupFilled());

        filledH1Title->show();
        filledH1->show();
        filledFundGroupTitle->show();
        filledFundGroup->show();
    }

    if (unicode)
        unfilledH1->setText(tri->homology().utf8().c_str());
    else
        unfilledH1->setText(tri->homology().str().c_str());
    unfilledFundGroup->refresh(&tri->fundamentalGroup());

    unfilledH1Title->show();
    unfilledH1->show();
    unfilledFundGroupTitle->show();
    unfilledFundGroup->show();
}

void SnapPeaAlgebraUI::updatePreferences() {
    // If we've changed the unicode setting, then we may need some redrawing.
    refresh();
}
