
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
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntrisurfaces.h"
#include "../reginapart.h"

#include <kiconloader.h>
#include <klocale.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>

using regina::NPacket;
using regina::NTriangulation;

NTriSurfacesUI::NTriSurfacesUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI, unsigned newAutoCalcThreshold) :
        PacketViewerTab(useParentUI), tri(packet),
        autoCalcThreshold(newAutoCalcThreshold) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    layout->addStretch(3);

    QLabel* label = new QLabel(i18n("Normal Surface Properties"), ui);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    layout->addStretch(1);

    QGridLayout* grid = new QGridLayout(layout, 2, 7, 5);
    grid->setColStretch(0, 1);
    grid->setColSpacing(2, 5); // Horizontal gap
    grid->setColSpacing(4, 5); // Horizontal gap
    grid->setColStretch(6, 1);

    label = new QLabel(i18n("Zero-efficient?"), ui);
    grid->addWidget(label, 0, 1);
    label = new QLabel(i18n("Splitting surface?"), ui);
    grid->addWidget(label, 1, 1);

    zeroEff = new QLabel(ui);
    grid->addWidget(zeroEff, 0, 3);
    splitting = new QLabel(ui);
    grid->addWidget(splitting, 1, 3);

    btnZeroEff = new QPushButton(SmallIconSet("run", 0,
        ReginaPart::factoryInstance()), i18n("Calculate"), ui);
    grid->addWidget(btnZeroEff, 0, 5);
    connect(btnZeroEff, SIGNAL(clicked()), this, SLOT(calculateZeroEff()));
    btnSplitting = new QPushButton(SmallIconSet("run", 0,
        ReginaPart::factoryInstance()), i18n("Calculate"), ui);
    grid->addWidget(btnSplitting, 1, 5);
    connect(btnSplitting, SIGNAL(clicked()), this, SLOT(calculateSplitting()));

    layout->addStretch(3);
}

regina::NPacket* NTriSurfacesUI::getPacket() {
    return tri;
}

QWidget* NTriSurfacesUI::getInterface() {
    return ui;
}

void NTriSurfacesUI::refresh() {
    if (tri->knowsZeroEfficient() ||
            tri->getNumberOfTetrahedra() <= autoCalcThreshold) {
        if (tri->isZeroEfficient()) {
            zeroEff->setText(i18n("True"));
            zeroEff->setPaletteForegroundColor(Qt::darkGreen);
        } else {
            zeroEff->setText(i18n("False"));
            zeroEff->setPaletteForegroundColor(Qt::darkRed);
        }
        btnZeroEff->setEnabled(false);
    } else {
        zeroEff->setText(i18n("Unknown"));
        zeroEff->unsetPalette();
        btnZeroEff->setEnabled(true);
    }

    if (tri->knowsSplittingSurface() ||
            tri->getNumberOfTetrahedra() <= autoCalcThreshold) {
        if (tri->hasSplittingSurface()) {
            splitting->setText(i18n("True"));
            splitting->setPaletteForegroundColor(Qt::darkGreen);
        } else {
            splitting->setText(i18n("False"));
            splitting->setPaletteForegroundColor(Qt::darkRed);
        }
        btnSplitting->setEnabled(false);
    } else {
        splitting->setText(i18n("Unknown"));
        splitting->unsetPalette();
        btnSplitting->setEnabled(true);
    }
}

void NTriSurfacesUI::editingElsewhere() {
    QString msg(i18n("Editing..."));
    zeroEff->setText(msg);
    zeroEff->unsetPalette();
    splitting->setText(msg);
    splitting->unsetPalette();

    btnZeroEff->setEnabled(false);
    btnSplitting->setEnabled(false);
}

void NTriSurfacesUI::calculateZeroEff() {
    tri->isZeroEfficient();
    refresh();
}

void NTriSurfacesUI::calculateSplitting() {
    tri->hasSplittingSurface();
    refresh();
}

#include "ntrisurfaces.moc"
