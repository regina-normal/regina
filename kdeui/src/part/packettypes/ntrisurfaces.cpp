
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2007, Ben Burton                                   *
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
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntrisurfaces.h"
#include "../patiencedialog.h"
#include "../reginapart.h"

#include <kiconloader.h>
#include <klocale.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

#define THREE_SPHERE_AUTO_CALC_ADJUSTMENT 2

using regina::NPacket;
using regina::NTriangulation;

NTriSurfacesUI::NTriSurfacesUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI, unsigned newAutoCalcThreshold) :
        PacketViewerTab(useParentUI), tri(packet),
        autoCalcThreshold(newAutoCalcThreshold) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    layout->addStretch(3);

    QLabel* label = new QLabel(i18n(
        "<qt><b>Normal Surface Properties</b></qt>"), ui);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    layout->addStretch(1);

    QGridLayout* grid = new QGridLayout(layout, 3, 7, 5);
    grid->setColStretch(0, 1);
    grid->setColSpacing(2, 5); // Horizontal gap
    grid->setColSpacing(4, 5); // Horizontal gap
    grid->setColStretch(6, 1);

    QString msg;

    label = new QLabel(i18n("Zero-efficient?"), ui);
    grid->addWidget(label, 0, 1);
    zeroEff = new QLabel(ui);
    grid->addWidget(zeroEff, 0, 3);
    msg = i18n("<qt>Is this a 0-efficient triangulation?  "
        "A <i>0-efficient triangulation</i> is one whose only normal "
        "spheres or discs are vertex linking, and which has no 2-sphere "
        "boundary components.</qt>");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(zeroEff, msg);

    label = new QLabel(i18n("Splitting surface?"), ui);
    grid->addWidget(label, 1, 1);
    splitting = new QLabel(ui);
    grid->addWidget(splitting, 1, 3);
    msg = i18n("<qt>Does this triangulation contain a splitting surface?  "
        "A <i>splitting surface</i> is a normal surface containing precisely "
        "one quadrilateral per tetrahedron and no other normal (or "
        "almost normal) discs.</qt>");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(splitting, msg);

    label = new QLabel(i18n("3-sphere?"), ui);
    grid->addWidget(label, 2, 1);
    threeSphere = new QLabel(ui);
    grid->addWidget(threeSphere, 2, 3);
    msg = i18n("Is this a triangulation of the 3-sphere?");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(threeSphere, msg);

    btnZeroEff = new QPushButton(SmallIconSet("run", 0,
        ReginaPart::factoryInstance()), i18n("Calculate"), ui);
    QToolTip::add(btnZeroEff, i18n("Calculate 0-efficiency"));
    QWhatsThis::add(btnZeroEff, i18n("<qt>Calculate whether this "
        "triangulation is 0-efficient.<p>"
        "<b>Warning:</b> This calculation can be quite slow for larger "
        "triangulations (which is why 0-efficiency is not always "
        "calculated automatically).</qt>"));
    grid->addWidget(btnZeroEff, 0, 5);
    connect(btnZeroEff, SIGNAL(clicked()), this, SLOT(calculateZeroEff()));

    btnSplitting = new QPushButton(SmallIconSet("run", 0,
        ReginaPart::factoryInstance()), i18n("Calculate"), ui);
    QToolTip::add(btnSplitting, i18n("Calculate existence of a splitting "
        "surface"));
    QWhatsThis::add(btnSplitting, i18n("<qt>Calculate whether this "
        "triangulation contains a splitting surface.<p>"
        "<b>Warning:</b> This calculation can be quite slow for larger "
        "triangulations (which is why the existence of a splitting "
        "surface is not always determined automatically).</qt>"));
    grid->addWidget(btnSplitting, 1, 5);
    connect(btnSplitting, SIGNAL(clicked()), this, SLOT(calculateSplitting()));

    btnThreeSphere = new QPushButton(SmallIconSet("run", 0,
        ReginaPart::factoryInstance()), i18n("Calculate"), ui);
    QToolTip::add(btnThreeSphere, i18n("Calculate whether this is a 3-sphere"));
    QWhatsThis::add(btnThreeSphere, i18n("<qt>Calculate whether this "
        "is a triangulation of a 3-sphere.<p>"
        "<b>Warning:</b> This calculation is occasionally quite slow for "
        "larger triangulations (which is why 3-sphere recognition is not "
        "always run automatically).</qt>"));
    grid->addWidget(btnThreeSphere, 2, 5);
    connect(btnThreeSphere, SIGNAL(clicked()), this,
        SLOT(calculateThreeSphere()));

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

    if (tri->knowsThreeSphere() ||
            tri->getNumberOfTetrahedra() + THREE_SPHERE_AUTO_CALC_ADJUSTMENT
            <= autoCalcThreshold) {
        if (tri->isThreeSphere()) {
            threeSphere->setText(i18n("True"));
            threeSphere->setPaletteForegroundColor(Qt::darkGreen);
        } else {
            threeSphere->setText(i18n("False"));
            threeSphere->setPaletteForegroundColor(Qt::darkRed);
        }
        btnThreeSphere->setEnabled(false);
    } else {
        threeSphere->setText(i18n("Unknown"));
        threeSphere->unsetPalette();
        btnThreeSphere->setEnabled(true);
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
    PatienceDialog* dlg = PatienceDialog::warn(i18n(
        "Deciding whether a triangulation is 0-efficient\n"
        "can be quite slow for larger triangulations.\n\n"
        "Please be patient."),
        enclosingPane->getPart()->instance(), ui);
    tri->isZeroEfficient();
    delete dlg;

    refresh();
}

void NTriSurfacesUI::calculateSplitting() {
    PatienceDialog* dlg = PatienceDialog::warn(i18n(
        "Deciding whether a splitting surface exists can\n"
        "be quite slow for larger triangulations.\n\n"
        "Please be patient."),
        enclosingPane->getPart()->instance(), ui);
    tri->hasSplittingSurface();
    delete dlg;

    refresh();
}

void NTriSurfacesUI::calculateThreeSphere() {
    PatienceDialog* dlg = PatienceDialog::warn(i18n(
        "3-sphere recognition can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."),
        enclosingPane->getPart()->instance(), ui);
    tri->isThreeSphere();
    delete dlg;

    refresh();
}

#include "ntrisurfaces.moc"
