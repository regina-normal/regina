
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2011, Ben Burton                                   *
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
#include "progress/nprogressmanager.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "coordinatechooser.h"
#include "coordinates.h"
#include "nnormalsurfacecreator.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../progressdialogs.h"

#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QWhatsThis>

using regina::NNormalSurfaceList;

namespace {
    /**
     * IDs that correspond to indices in the enumeration type box.
     */
    enum {
        BASIS_VERTEX,
        BASIS_FUND
    };
}

NNormalSurfaceCreator::NNormalSurfaceCreator() {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QBoxLayout* coordArea = new QHBoxLayout();
    layout->addLayout(coordArea);
    QString expln = ui->tr("Specifies the coordinate system in which the "
        "vertex normal surfaces will be enumerated.");
    QLabel* label = new QLabel(ui->tr("Coordinate system:"), ui);
    label->setWhatsThis(expln);
    coordArea->addWidget(label);
    coords = new CoordinateChooser();
    coords->insertAllCreators();
    coords->setCurrentSystem(ReginaPrefSet::global().surfacesCreationCoords);
    coords->setWhatsThis(expln);
    coordArea->addWidget(coords, 1);

    QBoxLayout* basisArea = new QHBoxLayout();
    layout->addLayout(basisArea);
    expln = ui->tr("<qt>Specifies whether to enumerate "
        "only vertex surfaces (at extremal rays of the normal surface "
        "solution cone), or all fundamental surfaces (which form a Hilbert "
        "basis for the solution cone).<p>Fundamental surfaces are "
        "more numerous, but can be significantly slower to enumerate.</qt>");
    label = new QLabel(ui->tr("Enumerate:"), ui);
    label->setWhatsThis(expln);
    basisArea->addWidget(label);
    basis = new QComboBox(ui);
    // These insertions MUST happen in the same order in which the
    // BASIS_... constants are defined at the top of this file.
    basis->insertItem(BASIS_VERTEX, ui->tr("Vertex surfaces"));
    basis->insertItem(BASIS_FUND, ui->tr("Fundamental surfaces"));
    basis->setCurrentIndex(0);
    basis->setWhatsThis(expln);
    basisArea->addWidget(basis, 1);

    embedded = new QCheckBox(ui->tr("Embedded surfaces only"), ui);
    embedded->setChecked(true);
    embedded->setWhatsThis(ui->tr("Specifies whether only embedded "
        "normal surfaces should be enumerated, or whether all normal "
        "surfaces (embedded, immersed and singular) should be enumerated."));
    layout->addWidget(embedded);
}

QWidget* NNormalSurfaceCreator::getInterface() {
    return ui;
}

QString NNormalSurfaceCreator::parentPrompt() {
    return ui->tr("Triangulation:");
}

QString NNormalSurfaceCreator::parentWhatsThis() {
    return ui->tr("The triangulation that will contain your normal surfaces.");
}

regina::NPacket* NNormalSurfaceCreator::createPacket(regina::NPacket* parent,
        QWidget* parentWidget) {
    if (parent->getPacketType() != regina::NTriangulation::packetType) {
        ReginaSupport::sorry(ui,
            ui->tr("The selected parent is not a 3-manifold triangulation."),
            ui->tr("Normal surfaces must live within a 3-manifold "
            "triangulation.  Please select the corresponding triangulation "
            "as the location in the tree for your new normal surface list."));
        return 0;
    }

    int coordSystem = coords->getCurrentSystem();

    int basisId = basis->currentIndex();

    // Sanity check for immersed and/or singular surfaces.
    if (! embedded->isChecked()) {
        if (Coordinates::generatesAlmostNormal(coordSystem)) {
            ReginaSupport::sorry(parentWidget,
                ui->tr("This combination of options is not yet supported."),
                ui->tr(
                "<qt>You have selected an almost normal coordinate "
                "system, but you have unchecked the box for embedded "
                "surfaces only.<p>"
                "At present, immersed and singular surfaces can only "
                "be used with <i>normal</i> coordinate systems, not "
                "<i>almost normal</i> coordinate systems.<p>"
                "Please check the box for embedded surfaces only, or "
                "else select a different coordinate system.</qt>"));
            return 0;
        }

        if (ReginaPrefSet::global().warnOnNonEmbedded) {
            QMessageBox msg(QMessageBox::Information,
                ui->tr("Warning"),
                ui->tr("You have unchecked the box for embedded "
                    "surfaces only."),
                QMessageBox::Yes | QMessageBox::Cancel, parentWidget);
            msg.setInformativeText(
                ui->tr("<qt>This means that immersed "
                "and singular surfaces will also be "
                "enumerated, which could be much slower "
                "and give far more solutions. "
                "Are you sure you wish to continue?</qt>"));
            msg.setDefaultButton(QMessageBox::Yes);
            if (msg.exec() != QMessageBox::Yes)
                return 0;
        }
    }

    if (basisId == BASIS_VERTEX) {
        regina::NProgressManager manager;
        ProgressDialogNumeric dlg(&manager,
            ui->tr("Enumerating vertex normal surfaces"),
            parentWidget);

        NNormalSurfaceList* ans = NNormalSurfaceList::enumerate(
            dynamic_cast<regina::NTriangulation*>(parent),
            coordSystem, embedded->isChecked(), &manager);

        if (dlg.run())
            return ans;
        else {
            delete ans;
            ReginaSupport::info(parentWidget,
                ui->tr("The normal surface enumeration was cancelled."));
            return 0;
        }
    } else {
        regina::NProgressManager manager;
        ProgressDialogMessage dlg(&manager,
            ui->tr("Enumerating fundamental normal surfaces"),
            parentWidget);

        NNormalSurfaceList* ans = NNormalSurfaceList::enumerateFundPrimal(
                dynamic_cast<regina::NTriangulation*>(parent),
                coordSystem, embedded->isChecked(),
                0 /* vtx surfaces */, &manager);

        if (dlg.run())
            return ans;
        else {
            delete ans;
            ReginaSupport::info(parentWidget,
                ui->tr("The normal surface enumeration was cancelled."));
            return 0;
        }
    }
}

void NNormalSurfaceCreator::explainNoParents() {
    ReginaSupport::sorry(ui,
        ui->tr("There are no triangulations to work with."),
        ui->tr("Normal surfaces must live within a 3-manifold "
        "triangulation.  Please add some triangulations to your file and "
        "try again."));
}

