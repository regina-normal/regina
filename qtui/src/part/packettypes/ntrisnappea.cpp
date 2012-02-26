
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
#include "snappea/nsnappeatriangulation.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntrisnappea.h"
#include "reginaprefset.h"
#include "snappeacomponents.h"

#include <climits>
#include <cmath>
#include <QLabel>
#include <QLayout>
#include <QStackedWidget>

using regina::NPacket;
using regina::NSnapPeaTriangulation;
using regina::NTriangulation;

NTriSnapPeaUI::NTriSnapPeaUI(regina::NTriangulation* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), reginaTri(packet), snappeaTri(0) {
    ui = new QWidget();
    QBoxLayout* layout = new QVBoxLayout(ui);

    layout->addStretch(3);

    QLabel* label = new QLabel(tr("<qt><b>SnapPea Calculations</b></qt>"),
        ui);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    layout->addStretch(1);

    data = new QStackedWidget(ui);

    // Data for a null SnapPea triangulation:

    dataNull = new QWidget();
    QBoxLayout* nullLayout = new QVBoxLayout(dataNull);

    unavailable = new NoSnapPea(reginaTri, dataNull, true);
    unavailable->setAlignment(Qt::AlignCenter);
    nullLayout->addWidget(unavailable);
    data->addWidget(dataNull);

    // Data for a non-null SnapPea triangulation:

    dataValid = new QWidget();
    QGridLayout* validGrid = new QGridLayout(dataValid);
    validGrid->setColumnStretch(0, 1);
    validGrid->setColumnMinimumWidth(2, 5); // Horizontal gap
    validGrid->setColumnStretch(4, 1);

    QString msg;

    solutionTypeLabel = new QLabel(tr("Solution type:"), dataValid);
    solutionTypeLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    validGrid->addWidget(solutionTypeLabel, 0, 1);
    solutionType = new QLabel(dataValid);
    solutionType->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    validGrid->addWidget(solutionType, 0, 3);
    solutionTypeExplnBase = tr("The type of solution that was found "
        "when solving for a complete hyperbolic structure.");
    solutionTypeLabel->setWhatsThis(solutionTypeExplnBase);
    solutionType->setWhatsThis(solutionTypeExplnBase);

    label = new QLabel(tr("Volume:"), dataValid);
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    validGrid->addWidget(label, 1, 1);
    volume = new QLabel(dataValid);
    volume->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    validGrid->addWidget(volume, 1, 3);
    msg = tr("The volume of the underlying 3-manifold.  The estimated "
        "number of decimal places of accuracy is also shown.");
    label->setWhatsThis(msg);
    volume->setWhatsThis(msg);

    data->addWidget(dataValid);

    // Finish off.
    layout->addWidget(data);
    layout->addStretch(3);

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

NTriSnapPeaUI::~NTriSnapPeaUI() {
    if (snappeaTri)
        delete snappeaTri;
}

regina::NPacket* NTriSnapPeaUI::getPacket() {
    return reginaTri;
}

QWidget* NTriSnapPeaUI::getInterface() {
    return ui;
}

void NTriSnapPeaUI::refresh() {
    if (snappeaTri)
        delete snappeaTri;

    snappeaTri = new NSnapPeaTriangulation(*reginaTri,
        ReginaPrefSet::global().snapPeaClosed);

    if (snappeaTri->isNull()) {
        data->setCurrentWidget(dataNull);
        unavailable->refresh();
    } else {
        data->setCurrentWidget(dataValid);

        solutionType->setText(solutionTypeString(snappeaTri->solutionType()));
        solutionType->setEnabled(true);

        QString expln = tr("%1  %2").arg(solutionTypeExplnBase)
            .arg(solutionTypeExplanation(snappeaTri->solutionType()));
        solutionTypeLabel->setWhatsThis(expln);
        solutionType->setWhatsThis(expln);

        int places;
        double ans = snappeaTri->volume(places);

        // Can we say that the volume is approximately zero?
        bool approxZero = false;
        if (places >= 6 && fabs(ans) < 1e-7) {
            // The volume is fairly small and the accuracy is high.
            // Test whether zero lies comfortably within the estimated
            // margin of error.
            double epsilon = 1.0;
            for (int i = 0; i < places + 1; i++)
                epsilon /= 10;

            // Now we should have epsilon == 1e-(places+1).
            if (fabs(ans) < epsilon)
                approxZero = true;
        }

        if (approxZero) {
            // Zero is within the margin of error, and this margin of
            // error is small.  Report it as zero, with the exact result
            // beneath.
            volume->setText(tr("Possibly zero\n(calculated %1,\n"
                "est. %2 places accuracy)").
                arg(snappeaTri->volume(places), 0, 'g', 9).arg(places));
        } else {
            volume->setText(tr("%1\n(est. %2 places accuracy)").
                arg(snappeaTri->volume(places), 0, 'g', 9).arg(places));
        }

        volume->setEnabled(true);
    }
}

void NTriSnapPeaUI::editingElsewhere() {
    data->setCurrentWidget(dataValid);

    QString msg(tr("Editing..."));

    solutionType->setText(msg);
    solutionType->setEnabled(false);
    solutionTypeLabel->setWhatsThis(solutionTypeExplnBase);
    solutionType->setWhatsThis(solutionTypeExplnBase);

    volume->setText(msg);
    volume->setEnabled(false);
}

QString NTriSnapPeaUI::solutionTypeString(int solnType) {
    switch (solnType) {
        case NSnapPeaTriangulation::not_attempted:
            return tr("Not attempted");
        case NSnapPeaTriangulation::geometric_solution:
            return tr("Tetrahedra positively oriented");
        case NSnapPeaTriangulation::nongeometric_solution:
            return tr("Contains negatively oriented tetrahedra");
        case NSnapPeaTriangulation::flat_solution:
            return tr("All tetrahedra flat");
        case NSnapPeaTriangulation::degenerate_solution:
            return tr("Contains degenerate tetrahedra");
        case NSnapPeaTriangulation::other_solution:
            return tr("Unrecognised solution type");
        case NSnapPeaTriangulation::no_solution:
            return tr("No solution found");
        default:
            return tr("ERROR (invalid solution type)");
    }
}

QString NTriSnapPeaUI::solutionTypeExplanation(int solnType) {
    switch (solnType) {
        case NSnapPeaTriangulation::not_attempted:
            return tr("This particular solution type means that "
                "a solution has not been attempted.");
        case NSnapPeaTriangulation::geometric_solution:
            return tr("This particular solution type means that "
                "all tetrahedra are either positively oriented or flat, "
                "though the entire solution is not flat and no "
                "tetrahedra are degenerate.");
        case NSnapPeaTriangulation::nongeometric_solution:
            return tr("This particular solution type means that "
                "the volume is positive, but some tetrahedra are "
                "negatively oriented.");
        case NSnapPeaTriangulation::flat_solution:
            return tr("This particular solution type means that "
                "all tetrahedra are flat, but none have shape "
                "0, 1 or infinity.");
        case NSnapPeaTriangulation::degenerate_solution:
            return tr("This particular solution type means that "
                "at least one tetrahedron has shape 0, 1 or infinity.");
        case NSnapPeaTriangulation::other_solution:
            return tr("This particular solution type means that "
                "the volume is zero or negative, but the solution is "
                "neither flat nor degenerate.");
        case NSnapPeaTriangulation::no_solution:
            return tr("This particular solution type means that "
                "the gluing equations could not be solved.");
        default:
            return tr("This particular solution type is unknown and "
                "should never occur.  Please report this as a bug.");
    }
}

