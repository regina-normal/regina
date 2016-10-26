
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
#include "triangulation/dim2.h"

// UI includes:
#include "tri2ui.h"
#include "tri2gluings.h"
#include "tri2skeleton.h"
#include "packeteditiface.h"
#include "reginamain.h"

#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

using regina::Packet;
using regina::Triangulation;

Tri2UI::Tri2UI(regina::Triangulation<2>* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane, ReginaPrefSet::global().tabDim2Tri) {
    Tri2HeaderUI* header = new Tri2HeaderUI(packet, this);
    gluings = new Tri2GluingsUI(packet, this,
        newEnclosingPane->isReadWrite());
    skeleton = new Tri2SkeletonUI(packet, this);

    gluings->fillToolBar(header->getToolBar());

    addHeader(header);
    addTab(gluings, QObject::tr("&Gluings"));
    addTab(skeleton, QObject::tr("&Skeleton"));

    editIface = new PacketEditTabbedUI(this);
}

Tri2UI::~Tri2UI() {
    delete editIface;
}

const QLinkedList<QAction*>& Tri2UI::getPacketTypeActions() {
    return gluings->getPacketTypeActions();
}

QString Tri2UI::getPacketMenuText() const {
    return QObject::tr("2-D T&riangulation");
}

Tri2HeaderUI::Tri2HeaderUI(regina::Triangulation<2>* packet,
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
    header->setWhatsThis(QObject::trUtf8(
        "Displays basic information about the surface, "
        "including the Euler characteristic χ."));
    uiLayout->addWidget(header);
}

regina::Packet* Tri2HeaderUI::getPacket() {
    return tri;
}

QWidget* Tri2HeaderUI::getInterface() {
    return ui;
}

void Tri2HeaderUI::refresh() {
    header->setText(summaryInfo(tri));
}

QString Tri2HeaderUI::summaryInfo(regina::Triangulation<2>* tri) {
    if (tri->isEmpty())
        return QObject::tr("Empty");

    QString msg;
    if (! tri->isConnected()) {
        msg = QObject::tr("Disconnected, ");
        if (tri->isClosed())
            msg += QObject::tr("closed, ");
        else
            msg += QObject::tr("with boundary, ");
        if (tri->isOrientable()) {
            if (tri->isOriented())
                msg += QObject::tr("orientable and oriented");
            else
                msg += QObject::tr("orientable but not oriented");
        } else
            msg += QObject::tr("non-orientable");
    } else {
        // It's connected.  Report the exact manifold.
        if (tri->isOrientable()) {
            long punctures = tri->countBoundaryComponents();
            long genus = (2 - tri->eulerChar() - punctures) / 2;

            // Special names for surfaces with boundary:
            if (genus == 0 && punctures == 1)
                msg = QObject::tr("Disc");
            else if (genus == 0 && punctures == 2)
                msg = QObject::tr("Annulus");
            else {
                if (genus == 0)
                    msg = QObject::tr("Sphere");
                else if (genus == 1)
                    msg = QObject::tr("Torus");
                else
                    msg = QObject::tr("Genus %1 torus").arg(genus);

                if (punctures == 1)
                    msg += QObject::tr(", 1 puncture");
                else if (punctures > 1)
                    msg += QObject::tr(", %1 punctures").arg(punctures);
            }

            if (tri->isOriented())
                msg += QObject::tr(", oriented");
            else
                msg += QObject::tr(", not oriented");
        } else {
            long punctures = tri->countBoundaryComponents();
            long genus = (2 - tri->eulerChar() - punctures);

            // Special names for surfaces with boundary:
            if (genus == 1 && punctures == 1)
                msg = QObject::trUtf8("Möbius band");
            else {
                if (genus == 1)
                    msg = QObject::tr("Projective plane");
                else if (genus == 2)
                    msg = QObject::tr("Klein bottle");
                else
                    msg = QObject::tr("Non-orientable genus %1 surface").
                        arg(genus);

                if (punctures == 1)
                    msg += QObject::tr(", 1 puncture");
                else if (punctures > 1)
                    msg += QObject::tr(", %1 punctures").arg(punctures);
            }
        }
    }

    msg += QObject::trUtf8(" (χ = %1)").arg(tri->eulerChar());
    return msg;
}

