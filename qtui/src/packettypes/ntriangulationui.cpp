
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
#include "triangulation/ntriangulation.h"

// UI includes:
#include "ntrialgebra.h"
#include "ntricomposition.h"
#include "ntriangulationui.h"
#include "ntrigluings.h"
#include "ntriskeleton.h"
#include "ntrisnappea.h"
#include "ntrisurfaces.h"
#include "packeteditiface.h"
#include "reginamain.h"

#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

using regina::NPacket;
using regina::NTriangulation;

NTriangulationUI::NTriangulationUI(regina::NTriangulation* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane, ReginaPrefSet::global().tabDim3Tri) {
    NTriHeaderUI* header = new NTriHeaderUI(packet, this);
    gluings = new NTriGluingsUI(packet, this, newEnclosingPane->isReadWrite());
    skeleton = new NTriSkeletonUI(packet, this);
    algebra = new NTriAlgebraUI(packet, this);
    surfaces = new NTriSurfacesUI(packet, this);
    snapPea = new NTriSnapPeaUI(packet, this);

    gluings->fillToolBar(header->getToolBar());

    addHeader(header);
    addTab(gluings, QObject::tr("&Gluings"));
    addTab(skeleton, QObject::tr("&Skeleton"));
    addTab(algebra, QObject::tr("&Algebra"));
    addTab(new NTriCompositionUI(packet, this), QObject::tr("&Composition"));
    addTab(surfaces, QObject::tr("&Recognition"));
    addTab(snapPea, QObject::tr("Snap&Pea"));

    editIface = new PacketEditTabbedUI(this);
}

NTriangulationUI::~NTriangulationUI() {
    delete editIface;
}

const QLinkedList<QAction*>& NTriangulationUI::getPacketTypeActions() {
    return gluings->getPacketTypeActions();
}

QString NTriangulationUI::getPacketMenuText() const {
    return QObject::tr("3-D T&riangulation");
}

NTriHeaderUI::NTriHeaderUI(regina::NTriangulation* packet,
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

regina::NPacket* NTriHeaderUI::getPacket() {
    return tri;
}

QWidget* NTriHeaderUI::getInterface() {
    return ui;
}

void NTriHeaderUI::refresh() {
    header->setText(summaryInfo(tri));
}

QString NTriHeaderUI::summaryInfo(regina::NTriangulation* tri) {
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

