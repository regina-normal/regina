
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
#include "surfaces/nnormalsurface.h"
#include "surfaces/nnormalsurfacelist.h"

// UI includes:
#include "reginaprefset.h"
#include "ncompatcanvas.h"
#include "nsurfacecompatui.h"
#include "../reginapart.h"

#include <qcanvas.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qwidgetstack.h>
#include <kcombobox.h>
#include <kiconloader.h>
#include <klocale.h>

using regina::NNormalSurfaceList;
using regina::NPacket;

NSurfaceCompatibilityUI::NSurfaceCompatibilityUI(
        regina::NNormalSurfaceList* packet, PacketTabbedUI* useParentUI,
        const ReginaPrefSet& prefs) :
        PacketViewerTab(useParentUI), surfaces(packet),
        matrixLocal(0), matrixGlobal(0), layerLocal(0), layerGlobal(0),
        autoCalcThreshold(prefs.surfacesCompatThreshold),
        requestedCalculation(false) {
    ui = new QWidget();
    QBoxLayout* uiLayout = new QVBoxLayout(ui);
    uiLayout->addSpacing(5);

    QBoxLayout* hdrLayout = new QHBoxLayout(uiLayout);
    hdrLayout->setSpacing(5);
    hdrLayout->addSpacing(5);

    QLabel* label = new QLabel(i18n("Display matrix:"), ui);
    hdrLayout->addWidget(label);
    chooseMatrix = new KComboBox(ui);
    chooseMatrix->insertItem(i18n("Local compatibility (quads and octagons)"));
    chooseMatrix->insertItem(i18n("Global compatibility (disjoint surfaces)"));
    connect(chooseMatrix, SIGNAL(activated(int)), this, SLOT(changeLayer(int)));
    hdrLayout->addWidget(chooseMatrix);
    QString msg = i18n("<qt>Allows you to switch between local and "
        "global compatibility matrices.<p>"
        "The <i>local</i> matrix tests whether two surfaces "
        "can avoid local intersections within each tetrahedron (which is "
        "determined entirely by quadrilateral and/or octagon types).  "
        "The <i>global</i> matrix tests whether two surfaces can "
        "simultaneously avoid intersections in <i>all</i> tetrahedra, "
        "i.e., whether the two surfaces can be made disjoint.</qt>");
    QWhatsThis::add(label, msg);
    QWhatsThis::add(chooseMatrix, msg);
    chooseMatrix->setCurrentItem(
        prefs.surfacesInitialCompat == ReginaPrefSet::GlobalCompat ? 1 : 0);
    chooseMatrix->setEnabled(false);

    hdrLayout->addStretch(1);

    btnCalculate = new QPushButton(SmallIconSet("run", 0,
        ReginaPart::factoryInstance()), i18n("Calculate"), ui);
    QToolTip::add(btnCalculate, i18n("Calculate compatibility matrices"));
    QWhatsThis::add(btnCalculate, i18n("<qt>Calculate and display the "
        "full compatibility matrices.<p>"
        "<b>Warning:</b> This calculation can be both slow and "
        "memory-hungry for lists containing large numbers of normal "
        "surfaces (which is why these matrices are not always "
        "calculated automatically).</qt>"));
    hdrLayout->addWidget(btnCalculate);
    connect(btnCalculate, SIGNAL(clicked()), this, SLOT(calculate()));

    hdrLayout->addSpacing(5);
    uiLayout->addSpacing(5);

    stack = new QWidgetStack(ui);
    {
        layerNone = new QWidget(stack);
        QBoxLayout* noneLayout = new QHBoxLayout(layerNone, 5 /* margin */,
            5 /* spacing */);
        noneLayout->addStretch(1);

        QPixmap iconPic = enclosingPane->getPart()->instance()->iconLoader()->
            loadIcon("messagebox_info", KIcon::NoGroup, KIcon::SizeMedium,
            KIcon::DefaultState, 0, true /* may be null */);
        if (iconPic.isNull())
            iconPic = QMessageBox::standardIcon(QMessageBox::Information);

        QLabel* icon = new QLabel(layerNone);
        icon->setPixmap(iconPic);
        noneLayout->addWidget(icon);
        noneLayout->setStretchFactor(icon, 0);

        noneLayout->addSpacing(10);

        msgNone = new QLabel(layerNone);
        noneLayout->addWidget(msgNone);
        noneLayout->setStretchFactor(msgNone, 4);

        noneLayout->addStretch(1);
        stack->addWidget(layerNone);
    }
    uiLayout->addWidget(stack);

    refresh();
}

NSurfaceCompatibilityUI::~NSurfaceCompatibilityUI() {
    // Make sure the canvas *views* go first.
    if (matrixLocal) {
        delete layerLocal;
        delete layerGlobal;
        delete matrixLocal;
        delete matrixGlobal;
    }
}

void NSurfaceCompatibilityUI::setAutoCalcThreshold(unsigned newThreshold) {
    autoCalcThreshold = newThreshold;

    if (! matrixLocal)
        if (surfaces->getNumberOfSurfaces() <= autoCalcThreshold)
            refresh();
}

regina::NPacket* NSurfaceCompatibilityUI::getPacket() {
    return surfaces;
}

QWidget* NSurfaceCompatibilityUI::getInterface() {
    return ui;
}

void NSurfaceCompatibilityUI::refresh() {
    chooseMatrix->setEnabled(false);
    btnCalculate->setEnabled(false);

    if (matrixLocal) {
        // Delete the old matrices.
        delete layerLocal;
        delete layerGlobal;
        delete matrixLocal;
        delete matrixGlobal;
    }

    matrixLocal = matrixGlobal = 0;
    layerLocal = layerGlobal = 0;

    // Are we able to compute the new matrices if we want to?
    if (surfaces->getNumberOfSurfaces() == 0) {
        setMessage(EMPTY_LIST);
        return;
    }
    if (! surfaces->isEmbeddedOnly()) {
        setMessage(NON_EMBEDDED);
        return;
    }

    // Should we compute new matrices?
    if ((! requestedCalculation) &&
            surfaces->getNumberOfSurfaces() > autoCalcThreshold) {
        // Nope.
        setMessage(TOO_LARGE);
        btnCalculate->setEnabled(true);
        return;
    }

    // Yes, we should compute new matrices.
    matrixLocal = new NCompatCanvas(surfaces->getNumberOfSurfaces());
    matrixGlobal = new NCompatCanvas(surfaces->getNumberOfSurfaces());

    layerLocal = new QCanvasView(matrixLocal, stack);
    stack->addWidget(layerLocal);
    QWhatsThis::add(layerLocal, i18n("<qt>This is the local "
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

    layerGlobal = new QCanvasView(matrixGlobal, stack);
    stack->addWidget(layerGlobal);
    QWhatsThis::add(layerGlobal, i18n("<qt>This is the global "
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

    if (chooseMatrix->currentItem() == 0) {
        stack->raiseWidget(layerLocal);
        matrixLocal->fillLocal(*surfaces);
    } else {
        stack->raiseWidget(layerGlobal);
        matrixGlobal->fillGlobal(*surfaces);
    }

    chooseMatrix->setEnabled(true);
}

void NSurfaceCompatibilityUI::setMessage(MessageIndex msg) {
    switch (msg) {
        case TOO_LARGE:
            msgNone->setText(i18n("<qt>The compatibility matrices "
                "have not been computed automatically, because this "
                "list contains a large number of surfaces.<p>"
                "If you wish to compute these matrices (and if you have "
                "enough time and memory), then please press the "
                "<i>Calculate</qt> button above.</qt>"));
            break;

        case NON_EMBEDDED:
            msgNone->setText(i18n("<qt>This list "
                "may contain immersed and/or singular surfaces.<p>"
                "Compatibility matrices can only be shown for a list "
                "of <i>embedded</i> normal or almost normal surfaces.</qt>"));
            break;

        case EMPTY_LIST:
            msgNone->setText(i18n("<qt>This list of surfaces is empty.</qt>"));
            break;
    }
}

void NSurfaceCompatibilityUI::changeLayer(int index) {
    if (! matrixLocal)
        return;

    if (index == 0) {
        stack->raiseWidget(layerLocal);
        matrixLocal->fillLocal(*surfaces);
    } else {
        stack->raiseWidget(layerGlobal);
        matrixGlobal->fillGlobal(*surfaces);
    }
}

void NSurfaceCompatibilityUI::calculate() {
    requestedCalculation = true;

    if (! matrixLocal)
        refresh();
}

#include "nsurfacecompatui.moc"

