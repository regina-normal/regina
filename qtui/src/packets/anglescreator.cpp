
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
#include "angle/anglestructures.h"
#include "progress/progresstracker.h"
#include "triangulation/dim3.h"

// UI includes:
#include "anglescreator.h"
#include "../progressdialogs.h"
#include "reginasupport.h"
#include "reginaprefset.h"

#include <QCheckBox>
#include <QLabel>
#include <QLayout>
#include <QWhatsThis>

AngleStructureCreator::AngleStructureCreator() {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    tautOnly = new QCheckBox(ui->tr("Taut angle structures only"), ui);
    tautOnly->setChecked(ReginaPrefSet::global().anglesCreationTaut);
    tautOnly->setWhatsThis(ui->tr("If you check this box, only "
        "taut angle structures will be enumerated (that is, angle structures "
        "in which every angle is 0 or Pi).  "
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

regina::Packet* AngleStructureCreator::createPacket(
        regina::Packet* parentPacket, QWidget* parentWidget) {
    // Note that parent may be either Triangulation<3> or SnapPeaTriangulation.
    if (! dynamic_cast<regina::Triangulation<3>*>(parentPacket)) {
        ReginaSupport::sorry(ui,
            ui->tr("The selected parent is not a 3-manifold triangulation."),
            ui->tr("Angle structures must live within a 3-manifold "
            "triangulation.  Please select the corresponding triangulation "
            "as the location in the tree for your new angle structure list."));
        return 0;
    }

    // Remember our options for next time.
    ReginaPrefSet::global().anglesCreationTaut = tautOnly->isChecked();

    regina::ProgressTracker tracker;
    ProgressDialogNumeric dlg(&tracker,
        ui->tr("Enumerating vertex angle structures..."), parentWidget);

    regina::AngleStructures* ans = regina::AngleStructures::enumerate(
            dynamic_cast<regina::Triangulation<3>*>(parentPacket),
            tautOnly->isChecked(), &tracker);

    if (dlg.run()) {
        if (tautOnly->isChecked())
            ans->setLabel("Taut angle structures");
        else
            ans->setLabel("Vertex angle structures");
        return ans;
    } else {
        delete ans;
        ReginaSupport::info(parentWidget,
            ui->tr("The angle structure enumeration was cancelled."));
        return 0;
    }
}

void AngleStructureCreator::explainNoParents() {
    ReginaSupport::sorry(ui,
        ui->tr("There are no triangulations to work with."),
        ui->tr("Angle structures must live within a 3-manifold "
        "triangulation.  Please add some triangulations to your file and "
        "try again."));
}

