
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
#include "progress/nprogressmanager.h"
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "coordinatechooser.h"
#include "coordinates.h"
#include "nnormalsurfacecreator.h"
#include "../progressdialogs.h"

#include <klocale.h>
#include <kmessagebox.h>
#include <kprogress.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qwhatsthis.h>

using regina::NNormalSurfaceList;

NNormalSurfaceCreator::NNormalSurfaceCreator(int defaultCoordSystem) {
    // Set up the basic layout.
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    QBoxLayout* coordArea = new QHBoxLayout(layout, 5);
    QString expln = i18n("Specifies the coordinate system in which the "
        "vertex normal surfaces will be enumerated.");
    QLabel* label = new QLabel(i18n("Coordinate system:"), ui);
    QWhatsThis::add(label, expln);
    coordArea->addWidget(label);
    coords = new CoordinateChooser(ui);
    coords->insertAllCreators();
    coords->setCurrentSystem(defaultCoordSystem);
    QWhatsThis::add(coords, expln);
    coordArea->addWidget(coords, 1);

    layout->addSpacing(5);

    embedded = new QCheckBox(i18n("Embedded surfaces only"), ui);
    embedded->setChecked(true);
    QWhatsThis::add(embedded, i18n("Specifies whether only embedded "
        "normal surfaces should be enumerated, or whether all normal "
        "surfaces (embedded, immersed and singular) should be enumerated."));
    layout->addWidget(embedded);
}

QWidget* NNormalSurfaceCreator::getInterface() {
    return ui;
}

regina::NPacket* NNormalSurfaceCreator::createPacket(regina::NPacket* parent,
        QWidget* parentWidget) {
    if (parent->getPacketType() != regina::NTriangulation::packetType) {
        KMessageBox::error(parentWidget, i18n(
            "Normal surface lists can only be created directly beneath "
            "triangulations."));
        return 0;
    }

    int coordSystem = coords->getCurrentSystem();

    // Sanity check for immersed and/or singular surfaces.
    if (! embedded->isChecked()) {
        if (Coordinates::generatesAlmostNormal(coordSystem)) {
            KMessageBox::sorry(parentWidget, i18n(
                "<qt>You have selected an almost normal coordinate "
                "system, but you have unchecked the box for embedded "
                "surfaces only.<p>"
                "This combination is not yet supported in Regina.  "
                "At present, immersed and singular surfaces can only "
                "be used with <i>normal</i> coordinate systems, not "
                "<i>almost normal</i> coordinate systems.<p>"
                "Please check the box for embedded surfaces only, or "
                "else select a different coordinate system.</qt>"));
            return 0;
        }

        if (KMessageBox::shouldBeShownContinue("warnOnNonEmbedded"))
            if (KMessageBox::warningContinueCancel(parentWidget,
                    i18n("<qt>You have unchecked the box for embedded "
                        "surfaces only.  This means that immersed "
                        "and/or singular surfaces will also be "
                        "enumerated, which could take a much longer time "
                        "and give a much larger solution set.<p>"
                        "Are you sure you wish to go ahead with this?</qt>"),
                    QString::null, KStdGuiItem::cont(),
                    "warnOnNonEmbedded") == KMessageBox::Cancel) {
                return 0;
            }
    }

    regina::NProgressManager manager;
    ProgressDialogNumeric dlg(&manager, i18n("Normal Surface Enumeration"),
        i18n("Enumerating vertex normal surfaces..."), parentWidget);

    NNormalSurfaceList* ans = NNormalSurfaceList::enumerate(
        dynamic_cast<regina::NTriangulation*>(parent),
        coordSystem, embedded->isChecked(), &manager);

    if (dlg.run())
        return ans;
    else {
        delete ans;
        KMessageBox::information(parentWidget,
            i18n("The normal surface enumeration was cancelled."));
        return 0;
    }
}

