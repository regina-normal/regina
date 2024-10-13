
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
#include "progress/progresstracker.h"
#include "surface/normalsurfaces.h"
#include "triangulation/dim3.h"

// UI includes:
#include "coordinatechooser.h"
#include "./coordinates.h" // Use ./ to avoid picking up the iOS header.
#include "packetfilter.h"
#include "surfacescreator.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../progressdialogs.h"

#include <thread>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QWhatsThis>

using regina::NormalSurfaces;

namespace {
    /**
     * IDs that correspond to indices in the enumeration type box.
     */
    enum {
        BASIS_VERTEX,
        BASIS_FUND
    };
}

SurfacesCreator::SurfacesCreator(ReginaMain*) {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QBoxLayout* coordArea = new QHBoxLayout();
    layout->addLayout(coordArea);
    QString expln = ui->tr("Specifies the coordinate system in which the "
        "normal surfaces will be enumerated.");
    auto* label = new QLabel(ui->tr("Coordinate system:"), ui);
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
        "more numerous, and can be significantly slower to enumerate.</qt>");
    label = new QLabel(ui->tr("Enumerate:"), ui);
    label->setWhatsThis(expln);
    basisArea->addWidget(label);
    basis = new QComboBox(ui);
    // These insertions MUST happen in the same order in which the
    // BASIS_... constants are defined at the top of this file.
    basis->addItem(ui->tr("Vertex surfaces"));
    basis->addItem(ui->tr("Fundamental surfaces"));
    basis->setCurrentIndex(ReginaPrefSet::global().surfacesCreationList.has(
        regina::NS_FUNDAMENTAL) ? BASIS_FUND : BASIS_VERTEX);
    basis->setWhatsThis(expln);
    basisArea->addWidget(basis, 1);

    embedded = new QCheckBox(ui->tr("Embedded surfaces only"), ui);
    embedded->setChecked(! ReginaPrefSet::global().surfacesCreationList.has(
        regina::NS_IMMERSED_SINGULAR));
    embedded->setWhatsThis(ui->tr("Specifies whether only embedded "
        "normal surfaces should be enumerated, or whether all normal "
        "surfaces (embedded, immersed and singular) should be enumerated."));
    layout->addWidget(embedded);
}

QWidget* SurfacesCreator::getInterface() {
    return ui;
}

QString SurfacesCreator::parentPrompt() {
    return ui->tr("Triangulation:");
}

QString SurfacesCreator::parentWhatsThis() {
    return ui->tr("The triangulation that will contain your normal surfaces.");
}

std::shared_ptr<regina::Packet> SurfacesCreator::createPacket(
        std::shared_ptr<regina::Packet> parent, QWidget* parentWidget) {
    // Note that parent may be either Triangulation<3> or SnapPeaTriangulation.
    auto& tri = regina::static_triangulation3_cast(*parent);

    regina::NormalCoords coordSystem = coords->getCurrentSystem();

    if ((coordSystem == regina::NormalCoords::QuadClosed ||
                coordSystem == regina::NormalCoords::QuadOctClosed) && ! (
            tri.countVertices() == 1 &&
            tri.vertex(0)->linkType() == regina::Vertex<3>::Link::Torus &&
            tri.isOriented())) {
        QString name = Coordinates::adjective(coordSystem, false);
        ReginaSupport::sorry(ui,
            ui->tr("I cannot use %1 coordinates with this "
                "triangulation.").arg(name),
            ui->tr("At the present time, %1 coordinates are "
                "only available for oriented ideal triangulations with "
                "one torus cusp and no other boundary components or "
                "internal vertices.").arg(name));
        return nullptr;
    }

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
            return nullptr;
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
                return nullptr;
        }
    }

    // Remember our selections for next time.
    ReginaPrefSet::global().surfacesCreationCoords = coordSystem;
    ReginaPrefSet::global().surfacesCreationList =
        (embedded->isChecked() ? regina::NS_EMBEDDED_ONLY :
            regina::NS_IMMERSED_SINGULAR) |
        (basisId == BASIS_VERTEX ? regina::NS_VERTEX : regina::NS_FUNDAMENTAL);

    std::shared_ptr<regina::Packet> ans;
    regina::ProgressTracker tracker;

    QString sType = (basisId == BASIS_VERTEX ?
        ui->tr("vertex") : ui->tr("fundamental"));

    ProgressDialogNumeric dlg(&tracker,
        ui->tr("Enumerating %1 normal surfaces").arg(sType), parentWidget);

    regina::NormalList which =
        (basisId == BASIS_VERTEX ?
            regina::NS_VERTEX : regina::NS_FUNDAMENTAL) |
        (embedded->isChecked() ?
            regina::NS_EMBEDDED_ONLY : regina::NS_IMMERSED_SINGULAR);
    std::thread t([&, coordSystem, which, this]() {
        try {
            ans = regina::make_packet<NormalSurfaces>(std::in_place,
                tri, coordSystem, which, regina::NS_ALG_DEFAULT, &tracker);
        } catch (const regina::ReginaException&) {
            // Leave ans as null.
        }
    });

    if (dlg.run()) {
        // The enumeration algorithm should have finished, though there may be
        // some final housekeeping still happening in the enumeration thread.
        t.join();

        if (ans) {
            ans->setLabel(ui->tr("%1 %2 surfaces")
                .arg(Coordinates::adjective(coordSystem, true))
                .arg(sType)
                .toStdString());
        } else {
            if (coordSystem == regina::NormalCoords::QuadClosed ||
                    coordSystem == regina::NormalCoords::QuadOctClosed) {
                ReginaSupport::info(parentWidget,
                    ui->tr("<qt>I could not enumerate %1 normal "
                    "surfaces in %2 coordinates.  This could be "
                    "because SnapPea was unable to construct the slope "
                    "equations, or because it tried to retriangulate when "
                    "doing so.<p>"
                    "Please report this to the Regina developers.</qt>")
                    .arg(sType)
                    .arg(Coordinates::adjective(coordSystem, false)));
            } else {
                ReginaSupport::failure(parentWidget,
                    ui->tr("<qt>I could not enumerate %1 normal surfaces.<p>"
                    "Please report this to the Regina developers.</qt>")
                    .arg(sType));
            }
        }
        return ans;
    } else {
        // The enumeration was cancelled.
        // We do not need to wait for the enumeration thread to finish,
        // since its results are being discarded anyway.
        t.detach();

        ReginaSupport::info(parentWidget,
            ui->tr("The normal surface enumeration was cancelled."));
        return nullptr;
    }
}

PacketFilter* SurfacesCreator::filter() {
    return new SubclassFilter<regina::Triangulation<3>>();
}

void SurfacesCreator::explainNoParents() {
    ReginaSupport::sorry(ui,
        ui->tr("There are no triangulations to work with."),
        ui->tr("Normal surfaces must live within a 3-manifold "
        "triangulation.  Please add some triangulations to your file and "
        "try again."));
}

