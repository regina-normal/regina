
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2022, Ben Burton                                   *
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
#include "angle/anglestructures.h"
#include "progress/progresstracker.h"
#include "triangulation/dim3.h"

// UI includes:
#include "anglescreator.h"
#include "../progressdialogs.h"
#include "packetfilter.h"
#include "reginasupport.h"
#include "reginaprefset.h"

#include <thread>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QWhatsThis>

AngleStructureCreator::AngleStructureCreator(ReginaMain*) {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    tautOnly = new QCheckBox(ui->tr("Taut angle structures only"), ui);
    tautOnly->setChecked(ReginaPrefSet::global().anglesCreationTaut);
    tautOnly->setWhatsThis(ui->tr("If you check this box, only "
        "taut angle structures will be enumerated (that is, angle structures "
        "in which every angle is 0 or π).  "
        "This is typically much faster than a full enumeration of all "
        "vertex angle structures."));
    layout->addWidget(tautOnly);
}

QWidget* AngleStructureCreator::getInterface() {
    return ui;
}

QString AngleStructureCreator::parentPrompt() {
    return ui->tr("Triangulation:");
}

QString AngleStructureCreator::parentWhatsThis() {
    return ui->tr("The triangulation that will contain your angle structures.");
}

std::shared_ptr<regina::Packet> AngleStructureCreator::createPacket(
        std::shared_ptr<regina::Packet> parentPacket, QWidget* parentWidget) {
    // Note that parent may be either Triangulation<3> or SnapPeaTriangulation.
    auto& tri = regina::static_triangulation3_cast(*parentPacket);

    // Remember our options for next time.
    ReginaPrefSet::global().anglesCreationTaut = tautOnly->isChecked();

    std::shared_ptr<regina::Packet> ans;
    regina::ProgressTracker tracker;

    ProgressDialogNumeric dlg(&tracker,
        ui->tr("Enumerating vertex angle structures..."), parentWidget);

    std::thread t([&, this]() {
        ans = regina::make_packet<regina::AngleStructures>(std::in_place,
            tri, tautOnly->isChecked(), regina::AS_ALG_DEFAULT, &tracker);
    });

    if (dlg.run()) {
        // The enumeration algorithm should have finished, though there may be
        // some final housekeeping still happening in the enumeration thread.
        t.join();

        if (tautOnly->isChecked())
            ans->setLabel("Taut angle structures");
        else
            ans->setLabel("Vertex angle structures");
        return ans;
    } else {
        // The enumeration was cancelled.
        // We do not need to wait for the enumeration thread to finish,
        // since its results are being discarded anyway.
        t.detach();

        ReginaSupport::info(parentWidget,
            ui->tr("The angle structure enumeration was cancelled."));
        return nullptr;
    }
}

PacketFilter* AngleStructureCreator::filter() {
    return new SubclassFilter<regina::Triangulation<3>>();
}

void AngleStructureCreator::explainNoParents() {
    ReginaSupport::sorry(ui,
        ui->tr("There are no triangulations to work with."),
        ui->tr("Angle structures must live within a 3-manifold "
        "triangulation.  Please add some triangulations to your file and "
        "try again."));
}

