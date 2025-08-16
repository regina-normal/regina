
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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
 *  You should have received a copy of the GNU General Public License     *
 *  along with this program. If not, see <https://www.gnu.org/licenses/>. *
 *                                                                        *
 **************************************************************************/

// Regina core includes:
#include "triangulation/dim3.h"

// UI includes:
#include "iconcache.h"
#include "tri3algebra.h"
#include "tri3composition.h"
#include "tri3ui.h"
#include "tri3gluings.h"
#include "tri3skeleton.h"
#include "tri3snappea.h"
#include "tri3surfaces.h"
#include "packeteditiface.h"
#include "reginamain.h"
#include "reginaprefset.h"
#include "reginasupport.h"

#include <QApplication>
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

using regina::Packet;
using regina::Triangulation;

Tri3UI::Tri3UI(regina::PacketOf<regina::Triangulation<3>>* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane, ReginaPrefSet::global().tabDim3Tri),
        simpleToolbars(ReginaPrefSet::global().displaySimpleToolbars) {
    header = new Tri3HeaderUI(packet, this);
    gluings = new Tri3GluingsUI(packet, this);
    skeleton = new Tri3SkeletonUI(packet, packet, this);
    algebra = new Tri3AlgebraUI(packet, this);
    surfaces = new Tri3SurfacesUI(packet, packet, this);
    snapPea = new Tri3SnapPeaUI(packet, this);

    gluings->fillToolBar(header->getToolBar());

    addHeader(header);
    addTab(gluings, QObject::tr("&Gluings"));
    addTab(skeleton, QObject::tr("&Skeleton"));
    addTab(algebra, QObject::tr("&Algebra"));
    addTab(new Tri3CompositionUI(packet, packet, this),
        QObject::tr("&Composition"));
    addTab(surfaces, QObject::tr("&Recognition"));
    addTab(snapPea, QObject::tr("Snap&Pea"));

    connect(&ReginaPrefSet::global(), SIGNAL(preferencesChanged()),
        this, SLOT(updatePreferences()));
}

const std::vector<QAction*>& Tri3UI::getPacketTypeActions() {
    return gluings->getPacketTypeActions();
}

QString Tri3UI::getPacketMenuText() const {
    return QObject::tr("3-D T&riangulation");
}

void Tri3UI::updatePreferences() {
    bool newVal = ReginaPrefSet::global().displaySimpleToolbars;
    if (newVal != simpleToolbars) {
        simpleToolbars = newVal;

        auto* toolbar = header->getToolBar();
        toolbar->clear();
        toolbar->setToolButtonStyle(ReginaPrefSet::toolButtonStyle());
        gluings->fillToolBar(toolbar);
    }
}

Tri3HeaderUI::Tri3HeaderUI(regina::PacketOf<regina::Triangulation<3>>* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        tri(packet) {
    ui = new QWidget();
    QBoxLayout* uiLayout = new QVBoxLayout(ui);
    uiLayout->setContentsMargins(0, 0, 0, 0);

    bar = new QToolBar(ui);
    bar->setToolButtonStyle(ReginaPrefSet::toolButtonStyle());
    uiLayout->addWidget(bar);

    header = new QLabel();
    header->setAlignment(Qt::AlignCenter);
    header->setMargin(10);
    header->setWhatsThis(QObject::tr("Displays a few basic properties of the "
        "triangulation, such as boundary and orientability."));
    uiLayout->addWidget(header);
}

regina::Packet* Tri3HeaderUI::getPacket() {
    return tri;
}

QWidget* Tri3HeaderUI::getInterface() {
    return ui;
}

void Tri3HeaderUI::refresh() {
    header->setText(summaryInfo(tri));
}

QString Tri3HeaderUI::summaryInfo(regina::Triangulation<3>* tri) {
    if (tri->isEmpty())
        return QObject::tr("Empty");

    if (! tri->isValid())
        return QObject::tr("INVALID TRIANGULATION!");

    QString msg;

    if (tri->isClosed())
        msg += QObject::tr("Closed, ");
    else {
        if (tri->isIdeal() && tri->hasBoundaryTriangles())
            msg += QObject::tr("Ideal & real bdry, ");
        else if (tri->isIdeal())
            msg += QObject::tr("Ideal bdry, ");
        else if (tri->hasBoundaryTriangles())
            msg += QObject::tr("Real bdry, ");
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

