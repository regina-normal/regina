
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
#include "hypersurface/normalhypersurface.h"
#include "hypersurface/normalhypersurfaces.h"

// UI includes:
#include "compatcanvas.h"
#include "hypercompatui.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "reginamain.h"
#include "messagelayer.h"

#include <QComboBox>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <qlabel.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <QVBoxLayout>
#include <QStackedWidget>

using regina::NormalHypersurfaces;
using regina::Packet;

HyperCompatibilityUI::HyperCompatibilityUI(
        regina::NormalHypersurfaces* packet, PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), surfaces(packet),
        matrixLocal(0), layerLocal(0), requestedCalculation(false) {
    ui = new QWidget();
    QBoxLayout* uiLayout = new QVBoxLayout(ui);

    QBoxLayout* hdrLayout = new QHBoxLayout();
    uiLayout->addLayout(hdrLayout);

    QLabel* label = new QLabel(tr("Display matrix:"), ui);
    hdrLayout->addWidget(label);
    chooseMatrix = new QComboBox(ui);
    chooseMatrix->insertItem(chooseMatrix->count(),
        tr("Local compatibility (prism types)"));
    connect(chooseMatrix, SIGNAL(activated(int)), this, SLOT(changeLayer(int)));
    hdrLayout->addWidget(chooseMatrix);
    QString msg = tr("<qt>Allows you to switch between different types of "
        "compatibility matrix.<p>"
        "For normal hypersurfaces in a 4-manifold triangulation, "
        "Regina can (for now) only display the <i>local</i> matrix.  "
        "This tests whether two hypersurfaces "
        "can avoid local intersections within each pentachoron (which is "
        "determined entirely by prism types).</qt>");
    label->setWhatsThis(msg);
    chooseMatrix->setWhatsThis(msg);
    chooseMatrix->setCurrentIndex(0); // No options to choose from, for now.
    chooseMatrix->setEnabled(false);

    hdrLayout->addStretch(1);

    btnCalculate = new QPushButton(ReginaSupport::themeIcon("system-run"),
        tr("Calculate"), ui);
    btnCalculate->setToolTip(tr("Calculate compatibility matrix"));
    btnCalculate->setWhatsThis(tr("<qt>Calculate and display the "
        "full compatibility matrix.<p>"
        "<b>Warning:</b> This calculation can be both slow and "
        "memory-hungry for lists containing large numbers of normal "
        "hypersurfaces (which is why this matrix is not always "
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

HyperCompatibilityUI::~HyperCompatibilityUI() {
    // Make sure the canvas *views* go first.
    if (matrixLocal) {
        delete layerLocal;
        delete matrixLocal;
    }
}

void HyperCompatibilityUI::updatePreferences() {
    if ((! matrixLocal) && surfaces->size() <= 
            ReginaPrefSet::global().surfacesCompatThreshold)
        refresh();
}

regina::Packet* HyperCompatibilityUI::getPacket() {
    return surfaces;
}

QWidget* HyperCompatibilityUI::getInterface() {
    return ui;
}

void HyperCompatibilityUI::refresh() {
    chooseMatrix->setEnabled(false);
    btnCalculate->setEnabled(false);

    if (matrixLocal) {
        // Delete the old matrices.
        delete layerLocal;
        delete matrixLocal;
    }

    matrixLocal = 0;
    layerLocal = 0;

    // Are we able to compute the new matrices if we want to?
    if (surfaces->size() == 0) {
        setMessage(EMPTY_LIST);
        return;
    }
    if (! surfaces->isEmbeddedOnly()) {
        setMessage(NON_EMBEDDED);
        return;
    }

    // Should we compute new matrices?
    if ((! requestedCalculation) && surfaces->size() >
            ReginaPrefSet::global().surfacesCompatThreshold) {
        // Nope.
        setMessage(TOO_LARGE);
        btnCalculate->setEnabled(true);
        return;
    }

    // Yes, we should compute new matrices.
    matrixLocal = new CompatCanvas(surfaces->size());

    layerLocal = new QGraphicsView(matrixLocal);
    stack->addWidget(layerLocal);
    layerLocal->setWhatsThis(tr("<qt>This is the local compatibility matrix.  "
        "Hypersurfaces are numbered from 0 to <i>N</i>-1, "
        "where <i>N</i> is the total number of hypersurfaces in this list.<p>"
        "The (<i>i</i>,<i>j</i>) cell in this matrix is filled if and "
        "only if hypersurfaces <i>i</i> and <i>j</i> are able to avoid "
        "local intersections within each pentachoron, i.e., they do "
        "not contain conflicting prism types.<p>"
        "This matrix tells nothing about whether two hypersurfaces can avoid "
        "intersections in <i>all</i> pentachora simultaneously (i.e., "
        "whether two hypersurfaces can be made disjoint).</qt>"));
    stack->addWidget(layerLocal);

    // For now we only support local compatibility.
    if (chooseMatrix->currentIndex() == 0) {
        stack->setCurrentWidget(layerLocal);
        matrixLocal->fillLocal(*surfaces);
    }

    chooseMatrix->setEnabled(true);
}

void HyperCompatibilityUI::setMessage(MessageIndex msg) {
    switch (msg) {
        case TOO_LARGE:
            layerNone->setText(tr("<qt>The compatibility matrices "
                "have not been computed automatically, because this "
                "list contains a large number of hypersurfaces.<p>"
                "If you wish to compute these matrices (and if you have "
                "enough time and memory), then please press the "
                "<i>Calculate</i> button above.</qt>"));
            break;

        case NON_EMBEDDED:
            layerNone->setText(tr("<qt>This list "
                "may contain immersed and/or singular hypersurfaces.<p>"
                "Compatibility matrices can only be shown for a list "
                "of <i>embedded</i> normal hypersurfaces.</qt>"));
            break;

        case EMPTY_LIST:
            layerNone->setText(tr(
                "<qt>This list of hypersurfaces is empty.</qt>"));
            break;
    }
}

void HyperCompatibilityUI::changeLayer(int index) {
    if (! matrixLocal)
        return;

    // For now we only support local compatibility.
    if (index == 0) {
        stack->setCurrentWidget(layerLocal);
        matrixLocal->fillLocal(*surfaces);
        ReginaPrefSet::global().surfacesInitialCompat =
            ReginaPrefSet::LocalCompat;
    }
}

void HyperCompatibilityUI::calculate() {
    requestedCalculation = true;

    if (! matrixLocal)
        refresh();
}

