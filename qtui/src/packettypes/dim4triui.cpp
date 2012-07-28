
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
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
#include "dim4/dim4triangulation.h"

// UI includes:
#include "dim4trialgebra.h"
#include "dim4trigluings.h"
// TODO #include "dim4triskeleton.h"
#include "dim4triui.h"
#include "packeteditiface.h"
#include "reginamain.h"

#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

using regina::NPacket;
using regina::Dim4Triangulation;

Dim4TriangulationUI::Dim4TriangulationUI(regina::Dim4Triangulation* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane) {
    Dim4TriHeaderUI* header = new Dim4TriHeaderUI(packet, this);
    gluings = new Dim4TriGluingsUI(packet, this,
        newEnclosingPane->isReadWrite());
    // TODO skeleton = new Dim4TriSkeletonUI(packet, this);
    algebra = new Dim4TriAlgebraUI(packet, this);

    gluings->fillToolBar(header->getToolBar());

    // WARNING: If these tabs are reordered, the code below that sets
    // the default tab must be updated accordingly.
    addHeader(header);
    addTab(gluings, QObject::tr("&Gluings"));
    // TODO addTab(skeleton, QObject::tr("&Skeleton"));
    addTab(algebra, QObject::tr("&Algebra"));

    // Select the default tab.
    switch (ReginaPrefSet::global().dim4InitialTab) {
        case ReginaPrefSet::Dim4Gluings:
            /* already visible */ break;
        case ReginaPrefSet::Dim4Skeleton:
            setCurrentTab(1); break;
        case ReginaPrefSet::Dim4Algebra:
            setCurrentTab(2); break;
    }

    editIface = new PacketEditTabbedUI(this);
}

Dim4TriangulationUI::~Dim4TriangulationUI() {
    delete editIface;
}

const QLinkedList<QAction*>& Dim4TriangulationUI::getPacketTypeActions() {
    return gluings->getPacketTypeActions();
}

QString Dim4TriangulationUI::getPacketMenuText() const {
    return QObject::tr("&4-D Triangulation");
}

Dim4TriHeaderUI::Dim4TriHeaderUI(regina::Dim4Triangulation* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();
    QBoxLayout* uiLayout = new QVBoxLayout(ui);
    uiLayout->setContentsMargins(0, 0, 0, 0);

    bar = new QToolBar(ui);
    bar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    uiLayout->addWidget(bar);

    header = new QLabel();
    header->setAlignment(Qt::AlignCenter);
    header->setMargin(10);
    header->setWhatsThis(QObject::tr("Displays a few basic properties of the "
        "triangulation, such as boundary and orientability."));
    uiLayout->addWidget(header);
}

regina::NPacket* Dim4TriHeaderUI::getPacket() {
    return tri;
}

QWidget* Dim4TriHeaderUI::getInterface() {
    return ui;
}

void Dim4TriHeaderUI::refresh() {
    header->setText(summaryInfo(tri));
}

void Dim4TriHeaderUI::editingElsewhere() {
    header->setText(QObject::tr("Editing..."));
}

QString Dim4TriHeaderUI::summaryInfo(regina::Dim4Triangulation* tri) {
    if (tri->getNumberOfPentachora() == 0)
        return QObject::tr("Empty");

    if (! tri->isValid())
        return QObject::tr("INVALID TRIANGULATION!");

    QString msg;

    if (tri->isClosed())
        msg += QObject::tr("Closed, ");
    else {
        if (tri->isIdeal() && tri->hasBoundaryTetrahedra())
            msg += QObject::tr("Ideal & real bdry, ");
        else if (tri->isIdeal())
            msg += QObject::tr("Ideal bdry, ");
        else if (tri->hasBoundaryTetrahedra())
            msg += QObject::tr("Real Bdry, ");
    }

    msg += (tri->isOrientable() ? QObject::tr("orientable, ") :
        QObject::tr("non-orientable, "));
    msg += (tri->isConnected() ? QObject::tr("connected") :
        QObject::tr("disconnected"));

    return msg;
}

