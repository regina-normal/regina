
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
#include "snappea/snappeatriangulation.h"

// UI includes:
#include "iconcache.h"
#include "tri3snappea.h"
#include "reginamain.h"
#include "reginaprefset.h"
#include "reginasupport.h"
#include "snappeacomponents.h"

#include <climits>
#include <cmath>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QStackedWidget>

using regina::Packet;
using regina::SnapPeaTriangulation;
using regina::Triangulation;

Tri3SnapPeaUI::Tri3SnapPeaUI(regina::Triangulation<3>* packet,
        PacketTabbedUI* useParentUI) :
        PacketViewerTab(useParentUI), reginaTri(packet), snappeaTri(0) {
    ui = new QStackedWidget();

    // Data for a null SnapPea triangulation:
    dataNull = new QWidget();
    QBoxLayout* nullLayout = new QVBoxLayout(dataNull);

    nullLayout->addStretch(3);

    QLabel* label = new QLabel(tr("<qt><b>SnapPea Calculations</b></qt>"));
    label->setAlignment(Qt::AlignCenter);
    nullLayout->addWidget(label);

    nullLayout->addStretch(1);

    unavailable = new NoSnapPea(reginaTri, dataNull, true);
    unavailable->setAlignment(Qt::AlignCenter);
    nullLayout->addWidget(unavailable);

    nullLayout->addStretch(3);

    ui->addWidget(dataNull);

    // Data for a non-null SnapPea triangulation:
    dataValid = new QWidget();
    QBoxLayout* validLayout = new QVBoxLayout(dataValid);

    validLayout->addStretch(2);

    label = new QLabel(tr("<qt><b>SnapPea Calculations</b></qt>"));
    label->setAlignment(Qt::AlignCenter);
    validLayout->addWidget(label);

    validLayout->addStretch(1);

    QGridLayout* validGrid = new QGridLayout();
    validGrid->setColumnStretch(0, 1);
    validGrid->setColumnMinimumWidth(2, 5); // Horizontal gap
    validGrid->setColumnStretch(4, 1);

    QString msg;

    solutionTypeLabel = new QLabel(tr("Solution type:"));
    solutionTypeLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    validGrid->addWidget(solutionTypeLabel, 0, 1);
    solutionType = new QLabel();
    solutionType->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    validGrid->addWidget(solutionType, 0, 3);
    solutionTypeExplnBase = tr("The type of solution that was found "
        "when solving for a complete hyperbolic structure.");
    solutionTypeLabel->setWhatsThis(solutionTypeExplnBase);
    solutionType->setWhatsThis(solutionTypeExplnBase);

    label = new QLabel(tr("Volume:"));
    label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    validGrid->addWidget(label, 1, 1);
    volume = new QLabel();
    volume->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    validGrid->addWidget(volume, 1, 3);
    msg = tr("The volume of the underlying 3-manifold.  The estimated "
        "number of decimal places of accuracy is also shown.");
    label->setWhatsThis(msg);
    volume->setWhatsThis(msg);

    validLayout->addLayout(validGrid);

    validLayout->addStretch(1);

    label = new QLabel(tr("For more detailed information:"));
    label->setAlignment(Qt::AlignCenter);
    validLayout->addWidget(label);

    QBoxLayout* buttonArea = new QHBoxLayout();
    buttonArea->addStretch(1);
    QPushButton* btnToSnapPea = new QPushButton(
        IconCache::icon(IconCache::packet_snappea),
        tr("Convert to a SnapPea triangulation"));
    buttonArea->addWidget(btnToSnapPea);
    buttonArea->addStretch(1);
    validLayout->addLayout(buttonArea);

    validLayout->addStretch(2);

    ui->addWidget(dataValid);

    // Finish off.

    connect(btnToSnapPea, SIGNAL(clicked()), this, SLOT(toSnapPea()));
    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

Tri3SnapPeaUI::~Tri3SnapPeaUI() {
    if (snappeaTri)
        delete snappeaTri;
}

regina::Packet* Tri3SnapPeaUI::getPacket() {
    return reginaTri;
}

QWidget* Tri3SnapPeaUI::getInterface() {
    return ui;
}

void Tri3SnapPeaUI::refresh() {
    if (snappeaTri)
        delete snappeaTri;

    snappeaTri = new SnapPeaTriangulation(*reginaTri);

    if (snappeaTri->isNull()) {
        ui->setCurrentWidget(dataNull);
        unavailable->refresh();
    } else {
        ui->setCurrentWidget(dataValid);

        solutionType->setText(solutionTypeString(snappeaTri->solutionType()));
        solutionType->setEnabled(true);

        QString expln = tr("%1  %2").arg(solutionTypeExplnBase)
            .arg(solutionTypeExplanation(snappeaTri->solutionType()));
        solutionTypeLabel->setWhatsThis(expln);
        solutionType->setWhatsThis(expln);

        int places;
        double ans = snappeaTri->volume(places);

        if (snappeaTri->volumeZero()) {
            // Zero is within the margin of error, and this margin of
            // error is small.  Report it as zero, with the exact result
            // beneath.
            volume->setText(tr("Possibly zero\n(calculated %1,\n"
                "est. %2 places accuracy)").
                arg(ans, 0, 'g', 9).arg(places));
        } else {
            volume->setText(tr("%1\n(est. %2 places accuracy)").
                arg(ans, 0, 'g', 9).arg(places));
        }

        volume->setEnabled(true);
    }
}

void Tri3SnapPeaUI::toSnapPea() {
    if (! snappeaTri) {
        // This should never happen, but...
        ui->setCurrentWidget(dataNull);
        unavailable->refresh();
        return;
    }

    SnapPeaTriangulation* ans = new SnapPeaTriangulation(*snappeaTri);
    if (ans->isNull()) {
        // This should never happen either...
        ui->setCurrentWidget(dataNull);
        unavailable->refresh();
        delete ans;
        return;
    }

    ReginaSupport::info(ui,
        tr("I have created a new SnapPea triangulation."),
        tr("<qt>The new SnapPea triangulation appears beneath this "
            "Regina triangulation in the packet tree.<p>"
            "For peripheral curves, I have attempted to install the "
            "(shortest, second shortest) basis on each cusp.</qt>"));

    ans->setLabel(reginaTri->label());
    reginaTri->insertChildLast(ans);
    enclosingPane->getMainWindow()->packetView(ans, true, true);
}

QString Tri3SnapPeaUI::solutionTypeString(int solnType) {
    switch (solnType) {
        case SnapPeaTriangulation::not_attempted:
            return tr("Not attempted");
        case SnapPeaTriangulation::geometric_solution:
            return tr("Tetrahedra positively oriented");
        case SnapPeaTriangulation::nongeometric_solution:
            return tr("Contains flat or negative tetrahedra");
        case SnapPeaTriangulation::flat_solution:
            return tr("All tetrahedra flat");
        case SnapPeaTriangulation::degenerate_solution:
            return tr("Contains degenerate tetrahedra");
        case SnapPeaTriangulation::other_solution:
            return tr("Unrecognised solution type");
        case SnapPeaTriangulation::no_solution:
            return tr("No solution found");
        case SnapPeaTriangulation::externally_computed:
            return tr("Externally computed");
        default:
            return tr("ERROR (invalid solution type)");
    }
}

QString Tri3SnapPeaUI::solutionTypeExplanation(int solnType) {
    switch (solnType) {
        case SnapPeaTriangulation::not_attempted:
            return tr("This particular solution type means that "
                "a solution has not been attempted.");
        case SnapPeaTriangulation::geometric_solution:
            return tr("This particular solution type means that "
                "all tetrahedra are positively oriented.");
        case SnapPeaTriangulation::nongeometric_solution:
            return tr("This particular solution type means that "
                "the overall volume is positive, but some tetrahedra are "
                "flat or negatively oriented.  No tetrahedra have "
                "shape 0, 1 or infinity.");
        case SnapPeaTriangulation::flat_solution:
            return tr("This particular solution type means that "
                "all tetrahedra are flat, but none have shape "
                "0, 1 or infinity.");
        case SnapPeaTriangulation::degenerate_solution:
            return tr("This particular solution type means that "
                "at least one tetrahedron has shape 0, 1 or infinity.");
        case SnapPeaTriangulation::other_solution:
            return tr("This particular solution type means that "
                "the volume is zero or negative, but the solution is "
                "neither flat nor degenerate.");
        case SnapPeaTriangulation::no_solution:
            return tr("This particular solution type means that "
                "the gluing equations could not be solved.");
        case SnapPeaTriangulation::externally_computed:
            return tr("This particular solution type means that "
                "tetrahedron shapes were inserted into the triangulation "
                "by some other means (e.g., manually, or by another "
                "program).");
        default:
            return tr("This particular solution type is unknown and "
                "should never occur.  Please report this as a bug.");
    }
}

