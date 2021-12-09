
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2021, Ben Burton                                   *
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
#include "hypersurface/normalhypersurfaces.h"
#include "triangulation/dim4.h"

// UI includes:
#include "coordinatechooser.h"
#include "./coordinates.h" // Use ./ to avoid picking up the iOS header.
#include "hypercreator.h"
#include "packetfilter.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "../progressdialogs.h"

#include <thread>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QWhatsThis>

using regina::NormalHypersurfaces;

namespace {
    /**
     * IDs that correspond to indices in the enumeration type box.
     */
    enum {
        BASIS_VERTEX,
        BASIS_FUND
    };
}

HyperCreator::HyperCreator(ReginaMain*) {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QBoxLayout* coordArea = new QHBoxLayout();
    layout->addLayout(coordArea);
    QString expln = ui->tr("Specifies the coordinate system in which the "
        "normal hypersurfaces will be enumerated.");
    auto* label = new QLabel(ui->tr("Coordinate system:"), ui);
    label->setWhatsThis(expln);
    coordArea->addWidget(label);
    coords = new HyperCoordinateChooser();
    coords->insertAllCreators();
    coords->setCurrentSystem(
        ReginaPrefSet::global().hypersurfacesCreationCoords);
    coords->setWhatsThis(expln);
    coordArea->addWidget(coords, 1);

    QBoxLayout* basisArea = new QHBoxLayout();
    layout->addLayout(basisArea);
    expln = ui->tr("<qt>Specifies whether to enumerate only "
        "vertex hypersurfaces (at extremal rays of the normal hypersurface "
        "solution cone), or all fundamental hypersurfaces (which form a "
        "Hilbert basis for the solution cone).<p>Fundamental hypersurfaces are "
        "more numerous, and can be significantly slower to enumerate.</qt>");
    label = new QLabel(ui->tr("Enumerate:"), ui);
    label->setWhatsThis(expln);
    basisArea->addWidget(label);
    basis = new QComboBox(ui);
    // These insertions MUST happen in the same order in which the
    // BASIS_... constants are defined at the top of this file.
    basis->addItem(ui->tr("Vertex hypersurfaces"));
    basis->addItem(ui->tr("Fundamental hypersurfaces"));
    basis->setCurrentIndex(
        ReginaPrefSet::global().hypersurfacesCreationList.has(
        regina::HS_FUNDAMENTAL) ? BASIS_FUND : BASIS_VERTEX);
    basis->setWhatsThis(expln);
    basisArea->addWidget(basis, 1);

    embedded = new QCheckBox(ui->tr("Embedded hypersurfaces only"), ui);
    embedded->setChecked(
        ! ReginaPrefSet::global().hypersurfacesCreationList.has(
        regina::HS_IMMERSED_SINGULAR));
    embedded->setWhatsThis(ui->tr("Specifies whether only embedded "
        "normal hypersurfaces should be enumerated, or whether all normal "
        "hypersurfaces (embedded, immersed and singular) should be "
        "enumerated."));
    layout->addWidget(embedded);
}

QWidget* HyperCreator::getInterface() {
    return ui;
}

QString HyperCreator::parentPrompt() {
    return ui->tr("Triangulation:");
}

QString HyperCreator::parentWhatsThis() {
    return ui->tr("The triangulation that will contain your "
        "normal hypersurfaces.");
}

std::shared_ptr<regina::Packet> HyperCreator::createPacket(
        std::shared_ptr<regina::Packet> parent, QWidget* parentWidget) {
    auto& tri = regina::static_packet_cast<regina::Triangulation<4>>(*parent);

    regina::HyperCoords coordSystem = coords->getCurrentSystem();

    int basisId = basis->currentIndex();

    // Sanity check for immersed and/or singular hypersurfaces.
    if (! embedded->isChecked()) {
        if (ReginaPrefSet::global().warnOnNonEmbedded) {
            QMessageBox msg(QMessageBox::Information,
                ui->tr("Warning"),
                ui->tr("You have unchecked the box for embedded "
                    "hypersurfaces only."),
                QMessageBox::Yes | QMessageBox::Cancel, parentWidget);
            msg.setInformativeText(
                ui->tr("<qt>This means that immersed "
                "and singular hypersurfaces will also be "
                "enumerated, which could be much slower "
                "and give far more solutions. "
                "Are you sure you wish to continue?</qt>"));
            msg.setDefaultButton(QMessageBox::Yes);
            if (msg.exec() != QMessageBox::Yes)
                return nullptr;
        }
    }

    // Remember our selections for next time.
    ReginaPrefSet::global().hypersurfacesCreationCoords = coordSystem;
    ReginaPrefSet::global().hypersurfacesCreationList =
        (embedded->isChecked() ? regina::HS_EMBEDDED_ONLY :
            regina::HS_IMMERSED_SINGULAR) |
        (basisId == BASIS_VERTEX ? regina::HS_VERTEX : regina::HS_FUNDAMENTAL);

    std::shared_ptr<regina::Packet> ans;
    regina::ProgressTracker tracker;

    QString sType = (basisId == BASIS_VERTEX ?
        ui->tr("vertex") : ui->tr("fundamental"));

    ProgressDialogNumeric dlg(&tracker,
        ui->tr("Enumerating %1 normal hypersurfaces").arg(sType), parentWidget);

    regina::HyperList which =
        (basisId == BASIS_VERTEX ?
            regina::HS_VERTEX : regina::HS_FUNDAMENTAL) |
        (embedded->isChecked() ?
            regina::HS_EMBEDDED_ONLY : regina::HS_IMMERSED_SINGULAR);
    std::thread([&, coordSystem, which, this]() {
        try {
            ans = regina::make_packet<NormalHypersurfaces>(std::in_place,
                tri, coordSystem, which, regina::HS_ALG_DEFAULT, &tracker);
        } catch (const regina::ReginaException&) {
            // Leave ans as null.
        }
    }).detach();

    if (dlg.run()) {
        if (ans) {
            ans->setLabel(ui->tr("%1 %2 hypersurfaces")
                .arg(Coordinates::adjective(coordSystem, true))
                .arg(sType)
                .toStdString());
        } else {
            ReginaSupport::failure(parentWidget,
                ui->tr("<qt>I could not enumerate %1 normal hypersurfaces.<p>"
                "Please report this to the Regina developers.</qt>")
                .arg(sType));
        }
        return ans;
    } else {
        ReginaSupport::info(parentWidget,
            ui->tr("The normal hypersurface enumeration was cancelled."));
        return nullptr;
    }
}

PacketFilter* HyperCreator::filter() {
    return new SingleTypeFilter<regina::PacketOf<regina::Triangulation<4>>>();
}

void HyperCreator::explainNoParents() {
    ReginaSupport::sorry(ui,
        ui->tr("There are no triangulations to work with."),
        ui->tr("Normal hypersurfaces must live within a 4-manifold "
        "triangulation.  Please add some triangulations to your file and "
        "try again."));
}

