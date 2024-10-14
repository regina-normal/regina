
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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
#include "surface/normalsurface.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"

// UI includes:
#include "compatcanvas.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "surfacescompatui.h"
#include "reginamain.h"
#include "messagelayer.h"

#include <QComboBox>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>

using regina::NormalSurfaces;
using regina::Packet;

SurfacesCompatibilityUI::SurfacesCompatibilityUI(
        regina::PacketOf<regina::NormalSurfaces>* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), surfaces(packet),
        matrixLocal(nullptr), matrixGlobal(nullptr),
        layerLocal(nullptr), layerGlobal(nullptr),
        requestedCalculation(false) {
    ui = new QWidget();
    QBoxLayout* uiLayout = new QVBoxLayout(ui);

    QBoxLayout* hdrLayout = new QHBoxLayout();
    uiLayout->addLayout(hdrLayout);

    auto* label = new QLabel(tr("Display matrix:"), ui);
    hdrLayout->addWidget(label);
    chooseMatrix = new QComboBox(ui);
    chooseMatrix->addItem(tr("Local compatibility (quads and octagons)"));
    chooseMatrix->addItem(tr("Global compatibility (disjoint surfaces)"));
    connect(chooseMatrix, SIGNAL(activated(int)), this, SLOT(changeLayer(int)));
    hdrLayout->addWidget(chooseMatrix);
    QString msg = tr("<qt>Allows you to switch between local and "
        "global compatibility matrices.<p>"
        "The <i>local</i> matrix tests whether two surfaces "
        "can avoid local intersections within each tetrahedron (which is "
        "determined entirely by quadrilateral and/or octagon types).  "
        "The <i>global</i> matrix tests whether two surfaces can "
        "simultaneously avoid intersections in <i>all</i> tetrahedra, "
        "i.e., whether the two surfaces can be made disjoint.</qt>");
    label->setWhatsThis(msg);
    chooseMatrix->setWhatsThis(msg);
    chooseMatrix->setCurrentIndex(
        ReginaPrefSet::global().surfacesInitialCompat ==
        ReginaPrefSet::CompatMatrix::Global ? 1 : 0);
    chooseMatrix->setEnabled(false);

    hdrLayout->addStretch(1);

    btnCalculate = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnCalculate->setToolTip(tr("Calculate compatibility matrices"));
    btnCalculate->setWhatsThis(tr("<qt>Calculate and display the "
        "full compatibility matrices.<p>"
        "<b>Warning:</b> This calculation can be both slow and "
        "memory-hungry for lists containing large numbers of normal "
        "surfaces (which is why these matrices are not always "
        "calculated automatically).</qt>"));
    hdrLayout->addWidget(btnCalculate);
    connect(btnCalculate, SIGNAL(clicked()), this, SLOT(calculate()));

    stack = new QStackedWidget(ui);
    layerNone = new MessageLayer("dialog-information");
    stack->addWidget(layerNone);
    uiLayout->addWidget(stack);

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));

    refresh();
}

SurfacesCompatibilityUI::~SurfacesCompatibilityUI() {
    // Make sure the canvas *views* go first.
    if (matrixLocal) {
        delete layerLocal;
        delete layerGlobal;
        delete matrixLocal;
        delete matrixGlobal;
    }
}

void SurfacesCompatibilityUI::updatePreferences() {
    if ((! matrixLocal) && surfaces->size() <= 
            ReginaPrefSet::global().surfacesCompatThreshold)
        refresh();
}

regina::Packet* SurfacesCompatibilityUI::getPacket() {
    return surfaces;
}

QWidget* SurfacesCompatibilityUI::getInterface() {
    return ui;
}

void SurfacesCompatibilityUI::refresh() {
    chooseMatrix->setEnabled(false);
    btnCalculate->setEnabled(false);

    if (matrixLocal) {
        // Delete the old matrices.
        delete layerLocal;
        delete layerGlobal;
        delete matrixLocal;
        delete matrixGlobal;
    }

    matrixLocal = matrixGlobal = nullptr;
    layerLocal = layerGlobal = nullptr;

    // Are we able to compute the new matrices if we want to?
    if (surfaces->size() == 0) {
        setMessage(Message::EmptyList);
        return;
    }
    if (! surfaces->isEmbeddedOnly()) {
        setMessage(Message::NonEmbedded);
        return;
    }

    // Should we compute new matrices?
    if ((! requestedCalculation) && surfaces->size() >
            ReginaPrefSet::global().surfacesCompatThreshold) {
        // Nope.
        setMessage(Message::TooLarge);
        btnCalculate->setEnabled(true);
        return;
    }

    // Yes, we should compute new matrices.
    matrixLocal = new CompatCanvas(surfaces->size());
    matrixGlobal = new CompatCanvas(surfaces->size());

    layerLocal = new QGraphicsView(matrixLocal);
    stack->addWidget(layerLocal);
    layerLocal->setWhatsThis(tr("<qt>This is the local "
        "compatibility matrix.  Surfaces are numbered from 0 to <i>N</i>-1, "
        "where <i>N</i> is the total number of surfaces in this list.<p>"
        "The (<i>i</i>,<i>j</i>) cell in this matrix is filled if and "
        "only if surfaces <i>i</i> and <i>j</i> are able to avoid "
        "local intersections within each tetrahedron, i.e., they do "
        "not contain conflicting quadrilateral and/or octagon types.<p>"
        "This matrix tells nothing about whether two surfaces can avoid "
        "intersections in <i>all</i> tetrahedra simultaneously (i.e., "
        "whether two surfaces can be made disjoint).  For this, see "
        "the global compatibility matrix instead.</qt>"));
    stack->addWidget(layerLocal);

    layerGlobal = new QGraphicsView(matrixGlobal);
    stack->addWidget(layerGlobal);
    layerGlobal->setWhatsThis(tr("<qt>This is the global "
        "compatibility matrix.  Surfaces are numbered from 0 to <i>N</i>-1, "
        "where <i>N</i> is the total number of surfaces in this list.<p>"
        "The (<i>i</i>,<i>j</i>) cell in this matrix is filled if and "
        "only if surfaces <i>i</i> and <i>j</i> are able to avoid "
        "intersections within all tetrahedra <i>simultaneously</i>, i.e., "
        "the two surfaces can be made disjoint.<p>"
        "The global compatibility test cannot work with surfaces that "
        "are non-compact (e.g., spun normal surfaces), empty, or "
        "disconnected.  The rows and columns corresponding to any such "
        "surfaces will be hashed out.</qt>"));
    stack->addWidget(layerGlobal);

    if (chooseMatrix->currentIndex() == 0) {
        stack->setCurrentWidget(layerLocal);
        matrixLocal->fillLocal(*surfaces);
    } else {
        stack->setCurrentWidget(layerGlobal);
        matrixGlobal->fillGlobal(*surfaces);
    }

    chooseMatrix->setEnabled(true);
}

void SurfacesCompatibilityUI::setMessage(Message msg) {
    switch (msg) {
        case Message::TooLarge:
            layerNone->setText(tr("<qt>The compatibility matrices "
                "have not been computed automatically, because this "
                "list contains a large number of surfaces.<p>"
                "If you wish to compute these matrices (and if you have "
                "enough time and memory), then please press the "
                "<i>Calculate</i> button above.</qt>"));
            break;

        case Message::NonEmbedded:
            layerNone->setText(tr("<qt>This list "
                "may contain immersed and/or singular surfaces.<p>"
                "Compatibility matrices can only be shown for a list "
                "of <i>embedded</i> normal or almost normal surfaces.</qt>"));
            break;

        case Message::EmptyList:
            layerNone->setText(tr("<qt>This list of surfaces is empty.</qt>"));
            break;
    }
}

void SurfacesCompatibilityUI::changeLayer(int index) {
    if (! matrixLocal)
        return;

    if (index == 0) {
        stack->setCurrentWidget(layerLocal);
        matrixLocal->fillLocal(*surfaces);
        ReginaPrefSet::global().surfacesInitialCompat =
            ReginaPrefSet::CompatMatrix::Local;
    } else {
        stack->setCurrentWidget(layerGlobal);
        matrixGlobal->fillGlobal(*surfaces);
        ReginaPrefSet::global().surfacesInitialCompat =
            ReginaPrefSet::CompatMatrix::Global;
    }
}

void SurfacesCompatibilityUI::calculate() {
    requestedCalculation = true;

    if (! matrixLocal)
        refresh();
}

