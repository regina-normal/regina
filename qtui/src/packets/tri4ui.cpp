
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
#include "triangulation/dim4.h"

// UI includes:
#include "iconcache.h"
#include "tri4algebra.h"
#include "tri4composition.h"
#include "tri4gluings.h"
#include "tri4skeleton.h"
#include "tri4ui.h"
#include "packeteditiface.h"
#include "reginamain.h"
#include "reginasupport.h"

#include <QApplication>
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

using regina::Packet;
using regina::Triangulation;

Tri4UI::Tri4UI(regina::PacketOf<regina::Triangulation<4>>* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane, ReginaPrefSet::global().tabDim4Tri) {
    auto* header = new Tri4HeaderUI(packet, this);
    gluings = new Tri4GluingsUI(packet, this);
    skeleton = new Tri4SkeletonUI(packet, this);
    algebra = new Tri4AlgebraUI(packet, this);

    gluings->fillToolBar(header->getToolBar());

    addHeader(header);
    addTab(gluings, QObject::tr("&Gluings"));
    addTab(skeleton, QObject::tr("&Skeleton"));
    addTab(algebra, QObject::tr("&Algebra"));
    addTab(new Tri4CompositionUI(packet, this), QObject::tr("&Composition"));
}

const std::vector<QAction*>& Tri4UI::getPacketTypeActions() {
    return gluings->getPacketTypeActions();
}

QString Tri4UI::getPacketMenuText() const {
    return QObject::tr("&4-D Triangulation");
}

Tri4HeaderUI::Tri4HeaderUI(regina::PacketOf<regina::Triangulation<4>>* packet,
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

regina::Packet* Tri4HeaderUI::getPacket() {
    return tri;
}

QWidget* Tri4HeaderUI::getInterface() {
    return ui;
}

void Tri4HeaderUI::refresh() {
    header->setText(summaryInfo(tri));
}

QString Tri4HeaderUI::summaryInfo(regina::Triangulation<4>* tri) {
    if (tri->isEmpty())
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

    if (tri->isOrientable()) {
        if (tri->isOriented())
            msg += QObject::tr("orientable and oriented, ");
        else
            msg += QObject::tr("orientable but not oriented, ");
    } else
        msg += QObject::tr("non-orientable, ");

    msg += (tri->isConnected() ? QObject::tr("connected") :
        QObject::tr("disconnected"));

    return msg;
}

