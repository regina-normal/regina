
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

    QGridLayout* grid = new QGridLayout();//, 4, 7, 5);
    layout->addLayout(grid);
    grid->setSpacing(5);
    grid->setColumnStretch(0, 1);
    grid->setColumnMinimumWidth(2, 5); // Horizontal gap
    grid->setColumnMinimumWidth(4, 5); // Horizontal gap
    grid->setColumnStretch(6, 1);

    QString msg;

    label = new QLabel(i18n("Zero-efficient?"), ui);
    grid->addWidget(label, 0, 1);
    zeroEff = new QLabel(ui);
    grid->addWidget(zeroEff, 0, 3);
    msg = i18n("<qt>Is this a 0-efficient triangulation?  "
        "A <i>0-efficient triangulation</i> is one whose only normal "
        "spheres or discs are vertex linking, and which has no 2-sphere "
        "boundary components.</qt>");
    label->setWhatsThis(msg);
    zeroEff->setWhatsThis(msg);

    label = new QLabel(i18n("Splitting surface?"), ui);
    grid->addWidget(label, 1, 1);
    splitting = new QLabel(ui);
    grid->addWidget(splitting, 1, 3);
    msg = i18n("<qt>Does this triangulation contain a splitting surface?  "
        "A <i>splitting surface</i> is a normal surface containing precisely "
        "one quadrilateral per tetrahedron and no other normal (or "
        "almost normal) discs.</qt>");
    label->setWhatsThis(msg);
    splitting->setWhatsThis(msg);

    label = new QLabel(i18n("3-sphere?"), ui);
    grid->addWidget(label, 2, 1);
    threeSphere = new QLabel(ui);
    grid->addWidget(threeSphere, 2, 3);
    msg = i18n("Is this a triangulation of the 3-sphere?");
    label->setWhatsThis(msg);
    threeSphere->setWhatsThis(msg);

    label = new QLabel(i18n("3-ball?"), ui);
    grid->addWidget(label, 3, 1);
    threeBall= new QLabel(ui);
    grid->addWidget(threeBall, 3, 3);
    msg = i18n("Is this a triangulation of the 3-dimensional ball?");
    label->setWhatsThis(msg);
    threeBall->setWhatsThis(msg);

    btnZeroEff = new QPushButton(KIcon("system-run"), i18n("Calculate"), ui);
    btnZeroEff->setToolTip(i18n("Calculate 0-efficiency"));
    btnZeroEff->setWhatsThis(i18n("<qt>Calculate whether this "
        "triangulation is 0-efficient.<p>"
        "<b>Warning:</b> This calculation can be quite slow for larger "
        "triangulations (which is why 0-efficiency is not always "
        "calculated automatically).</qt>"));
    grid->addWidget(btnZeroEff, 0, 5);
    connect(btnZeroEff, SIGNAL(clicked()), this, SLOT(calculateZeroEff()));

    btnSplitting = new QPushButton(KIcon("system-run"), i18n("Calculate"), ui);
    btnSplitting->setToolTip(i18n("Calculate existence of a splitting "
        "surface"));
    btnSplitting->setWhatsThis(i18n("<qt>Calculate whether this "
        "triangulation contains a splitting surface.<p>"
        "<b>Warning:</b> This calculation can be quite slow for larger "
        "triangulations (which is why the existence of a splitting "
        "surface is not always determined automatically).</qt>"));
    grid->addWidget(btnSplitting, 1, 5);
    connect(btnSplitting, SIGNAL(clicked()), this, SLOT(calculateSplitting()));

    btnThreeSphere = new QPushButton(KIcon("system-run"), i18n("Calculate"), ui);
    btnThreeSphere->setToolTip(i18n("Calculate whether this is a 3-sphere"));
    btnThreeSphere->setWhatsThis(i18n("<qt>Calculate whether this "
        "is a triangulation of a 3-sphere.<p>"
        "<b>Warning:</b> This calculation is occasionally quite slow for "
        "larger triangulations (which is why 3-sphere recognition is not "
        "always run automatically).</qt>"));
    grid->addWidget(btnThreeSphere, 2, 5);
    connect(btnThreeSphere, SIGNAL(clicked()), this,
        SLOT(calculateThreeSphere()));

    btnThreeBall = new QPushButton(KIcon("system-run"), i18n("Calculate"), ui);
    btnThreeBall->setToolTip(
        i18n("Calculate whether this is a 3-dimensional ball"));
    btnThreeBall->setWhatsThis(i18n("<qt>Calculate whether this "
        "is a triangulation of a 3-dimensional ball.<p>"
        "<b>Warning:</b> This calculation is occasionally quite slow for "
        "larger triangulations (which is why 3-ball recognition is not "
        "always run automatically).</qt>"));
    grid->addWidget(btnThreeBall, 3, 5);
    connect(btnThreeBall, SIGNAL(clicked()), this,
        SLOT(calculateThreeBall()));

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
            QPalette pal = zeroEff->palette();
            pal.setColor(zeroEff->foregroundRole(), Qt::darkGreen);
            zeroEff->setPalette(pal);
        } else {
            zeroEff->setText(i18n("False"));
            QPalette pal = zeroEff->palette();
            pal.setColor(zeroEff->foregroundRole(), Qt::darkRed);
            zeroEff->setPalette(pal);
        }
        btnZeroEff->setEnabled(false);
    } else {
        zeroEff->setText(i18n("Unknown"));
        zeroEff->setPalette(QPalette());
        btnZeroEff->setEnabled(true);
    }

    if (tri->knowsSplittingSurface() ||
            tri->getNumberOfTetrahedra() <= autoCalcThreshold) {
        if (tri->hasSplittingSurface()) {
            splitting->setText(i18n("True"));
            QPalette pal = splitting->palette();
            pal.setColor(splitting->foregroundRole(), Qt::darkGreen);
            splitting->setPalette(pal);
        } else {
            splitting->setText(i18n("False"));
            QPalette pal = splitting->palette();
            pal.setColor(splitting->foregroundRole(), Qt::darkRed);
            splitting->setPalette(pal);
        }
        btnSplitting->setEnabled(false);
    } else {
        splitting->setText(i18n("Unknown"));
        splitting->setPalette(QPalette());
        btnSplitting->setEnabled(true);
    }

    if (tri->knowsThreeSphere() ||
            tri->getNumberOfTetrahedra() + THREE_SPHERE_AUTO_CALC_ADJUSTMENT
            <= autoCalcThreshold) {
        if (tri->isThreeSphere()) {
            threeSphere->setText(i18n("True"));
            QPalette pal = threeSphere->palette();
            pal.setColor(threeSphere->foregroundRole(), Qt::darkGreen);
            threeSphere->setPalette(pal);
        } else {
            threeSphere->setText(i18n("False"));
            QPalette pal = threeSphere->palette();
            pal.setColor(threeSphere->foregroundRole(), Qt::darkRed);
            threeSphere->setPalette(pal);
        }
        btnThreeSphere->setEnabled(false);
    } else {
        threeSphere->setText(i18n("Unknown"));
        threeSphere->setPalette(QPalette());
        btnThreeSphere->setEnabled(true);
    }

    // Use the same threshold adjustment as for 3-sphere recognition.
    if (tri->knowsBall() ||
            tri->getNumberOfTetrahedra() + THREE_SPHERE_AUTO_CALC_ADJUSTMENT
            <= autoCalcThreshold) {
        if (tri->isBall()) {
            threeBall->setText(i18n("True"));
            QPalette pal = threeBall->palette();
            pal.setColor(threeBall->foregroundRole(), Qt::darkGreen);
            threeBall->setPalette(pal);
        } else {
            threeBall->setText(i18n("False"));
            QPalette pal = threeBall->palette();
            pal.setColor(threeBall->foregroundRole(), Qt::darkRed);
            threeBall->setPalette(pal);
        }
        btnThreeBall->setEnabled(false);
    } else {
        threeBall->setText(i18n("Unknown"));
        threeBall->setPalette(QPalette());
        btnThreeBall->setEnabled(true);
    }
}

void NTriSurfacesUI::editingElsewhere() {
    QString msg(i18n("Editing..."));
    zeroEff->setText(msg);
    zeroEff->setPalette(QPalette());
    splitting->setText(msg);
    splitting->setPalette(QPalette());
    threeSphere->setText(msg);
    threeSphere->setPalette(QPalette());
    threeBall->setText(msg);
    threeBall->setPalette(QPalette());

    btnZeroEff->setEnabled(false);
    btnSplitting->setEnabled(false);
    btnThreeSphere->setEnabled(false);
    btnThreeBall->setEnabled(false);
}

void NTriSurfacesUI::calculateZeroEff() {
    PatienceDialog* dlg = PatienceDialog::warn(i18n(
        "Deciding whether a triangulation is 0-efficient\n"
        "can be quite slow for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri->isZeroEfficient();
    delete dlg;

    refresh();
}

void NTriSurfacesUI::calculateSplitting() {
    PatienceDialog* dlg = PatienceDialog::warn(i18n(
        "Deciding whether a splitting surface exists can\n"
        "be quite slow for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri->hasSplittingSurface();
    delete dlg;

    refresh();
}

void NTriSurfacesUI::calculateThreeSphere() {
    PatienceDialog* dlg = PatienceDialog::warn(i18n(
        "3-sphere recognition can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri->isThreeSphere();
    delete dlg;

    refresh();
}

void NTriSurfacesUI::calculateThreeBall() {
    PatienceDialog* dlg = PatienceDialog::warn(i18n(
        "3-ball recognition can be quite slow\n"
        "for larger triangulations.\n\n"
        "Please be patient."), ui);
    tri->isBall();
    delete dlg;

    refresh();
}

