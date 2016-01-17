
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2013, Ben Burton                                   *
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

/* end stub */

// Regina core includes:
#include "algebra/ngrouppresentation.h"
#include "algebra/nmarkedabeliangroup.h"
#include "dim4/dim4triangulation.h"
#include "maths/numbertheory.h"

// UI includes:
#include "columnlayout.h"
#include "groupwidget.h"
#include "dim4trialgebra.h"
#include "reginaprefset.h"

#include <QLabel>
#include <QLayout>

using regina::Dim4Triangulation;
using regina::NPacket;

Dim4TriAlgebraUI::Dim4TriAlgebraUI(regina::Dim4Triangulation* packet,
        PacketTabbedUI* useParentUI) :
        PacketTabbedViewerTab(useParentUI,
            ReginaPrefSet::global().tabDim3TriAlgebra) {
    addTab(new Dim4TriHomologyFundUI(packet, this),
        tr("&Homology && Fund. Group"));
}

Dim4TriHomologyFundUI::Dim4TriHomologyFundUI(regina::Dim4Triangulation* packet,
        PacketTabbedViewerTab* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();

    ColumnLayout* master = new ColumnLayout(ui);

    // Homology:

    QGridLayout* homologyGrid = new QGridLayout();
    homologyGrid->setRowStretch(0, 1);
    homologyGrid->setRowStretch(3, 1);
    homologyGrid->setColumnStretch(0, 1);
    homologyGrid->setColumnStretch(3, 1);

    QString msg;
    QLabel* label;

    label = new QLabel(QObject::tr("H1(M):"));
    homologyGrid->addWidget(label, 1, 1);
    H1 = new QLabel(ui);
    homologyGrid->addWidget(H1, 1, 2);
    msg = QObject::tr("The first homology group of this triangulation.");
    label->setWhatsThis(msg);
    H1->setWhatsThis(msg);

    label = new QLabel(QObject::tr("H2(M):"));
    homologyGrid->addWidget(label, 2, 1);
    H2 = new QLabel(ui);
    homologyGrid->addWidget(H2, 2, 2);
    msg = QObject::tr("The second homology group of this triangulation.");
    label->setWhatsThis(msg);
    H2->setWhatsThis(msg);

    master->addLayout(homologyGrid, tr("Homology"));

    // Fundamental group:

    QBoxLayout* fundLayout = new QVBoxLayout();

    fgMsg = new QLabel();
    fgMsg->setAlignment(Qt::AlignCenter);
    fundLayout->addWidget(fgMsg);
    fgMsg->hide();

    fgGroup = new GroupWidget(true, true);
    fgGroup->setWhatsThis(tr("A full set of generators and relations "
        "for the fundamental group of this triangulation."));
    connect(fgGroup, SIGNAL(simplified()), this, SLOT(fundGroupSimplified()));
    fundLayout->addWidget(fgGroup, 1);

    master->addLayout(fundLayout, tr("Fundamental Group"));
}

regina::NPacket* Dim4TriHomologyFundUI::getPacket() {
    return tri;
}

QWidget* Dim4TriHomologyFundUI::getInterface() {
    return ui;
}

void Dim4TriHomologyFundUI::refresh() {
    if (tri->isValid()) {
        H1->setText(tri->homologyH1().str().c_str());
        H2->setText(tri->homologyH2().str().c_str());
    } else {
        QString msg(QObject::tr("Invalid Triangulation"));
        H1->setText(msg);
        H2->setText(msg);
    }

    if (! tri->isValid()) {
        fgGroup->hide();
        fgMsg->setText(tr("<qt>Cannot calculate<p>"
            "(invalid triangulation)</qt>"));
        fgMsg->show();
    } else if (tri->countComponents() > 1) {
        fgGroup->hide();
        fgMsg->setText(tr("<qt>Cannot calculate<p>"
            "(disconnected triangulation)</qt>"));
        fgMsg->show();
    } else {
        fgMsg->hide();
        fgGroup->refresh(&tri->getFundamentalGroup());
        fgGroup->show();
    }
}

void Dim4TriHomologyFundUI::fundGroupSimplified() {
    regina::NGroupPresentation* simp = fgGroup->takeSimplifiedGroup();
    if (simp)
        tri->simplifiedFundamentalGroup(simp);
}

