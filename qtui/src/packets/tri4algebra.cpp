
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
#include "algebra/grouppresentation.h"
#include "algebra/markedabeliangroup.h"
#include "maths/numbertheory.h"
#include "triangulation/dim4.h"

// UI includes:
#include "columnlayout.h"
#include "groupwidget.h"
#include "tri4algebra.h"
#include "reginaprefset.h"

#include <QLabel>
#include <QLayout>

using regina::Packet;
using regina::Triangulation;

Tri4AlgebraUI::Tri4AlgebraUI(regina::Triangulation<4>* packet,
        PacketTabbedUI* useParentUI) :
        PacketTabbedViewerTab(useParentUI,
            ReginaPrefSet::global().tabDim3TriAlgebra) {
    addTab(new Tri4HomologyFundUI(packet, this),
        tr("&Homology && Fund. Group"));
}

Tri4HomologyFundUI::Tri4HomologyFundUI(regina::Triangulation<4>* packet,
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

    // The text for the following labels differs according to whether or
    // not unicode is enabled.  We therefore set the label texts in
    // refreshLabels(), which is called a little further down.

    labelH1 = new QLabel();
    homologyGrid->addWidget(labelH1, 1, 1);
    H1 = new QLabel(ui);
    homologyGrid->addWidget(H1, 1, 2);
    msg = QObject::tr("The first homology group of this triangulation.");
    labelH1->setWhatsThis(msg);
    H1->setWhatsThis(msg);

    labelH2 = new QLabel();
    homologyGrid->addWidget(labelH2, 2, 1);
    H2 = new QLabel(ui);
    homologyGrid->addWidget(H2, 2, 2);
    msg = QObject::tr("The second homology group of this triangulation.");
    labelH2->setWhatsThis(msg);
    H2->setWhatsThis(msg);

    refreshLabels();

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

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

regina::Packet* Tri4HomologyFundUI::getPacket() {
    return tri;
}

QWidget* Tri4HomologyFundUI::getInterface() {
    return ui;
}

void Tri4HomologyFundUI::refresh() {
    if (tri->isValid()) {
        if (ReginaPrefSet::global().displayUnicode) {
            H1->setText(tri->homologyH1().utf8().c_str());
            H2->setText(tri->homologyH2().utf8().c_str());
        } else {
            H1->setText(tri->homologyH1().str().c_str());
            H2->setText(tri->homologyH2().str().c_str());
        }
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
        fgGroup->refresh(&tri->fundamentalGroup());
        fgGroup->show();
    }
}

void Tri4HomologyFundUI::fundGroupSimplified() {
    regina::GroupPresentation* simp = fgGroup->takeSimplifiedGroup();
    if (simp)
        tri->simplifiedFundamentalGroup(simp);
}

void Tri4HomologyFundUI::refreshLabels() {
    if (ReginaPrefSet::global().displayUnicode) {
        labelH1->setText(QObject::trUtf8("H\u2081(M):"));
        labelH2->setText(QObject::trUtf8("H\u2082(M):"));
    } else {
        labelH1->setText(QObject::tr("H1(M):"));
        labelH2->setText(QObject::tr("H2(M):"));
    }
}

void Tri4HomologyFundUI::updatePreferences() {
    // If we've changed the unicode setting, then we may need some redrawing.
    refreshLabels();
    refresh();
}
