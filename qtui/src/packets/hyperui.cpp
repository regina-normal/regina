
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
#include "hypersurface/normalhypersurfaces.h"
#include "triangulation/dim4.h"

// UI includes:
#include "./coordinates.h" // Use ./ to avoid picking up the iOS header.
#include "tri4ui.h"
#include "hyperui.h"
#include "hypercompatui.h"
#include "hypercoordinateui.h"
#include "hypermatchingui.h"
#include "hypersummaryui.h"
#include "reginamain.h"

#include <QLabel>
#include <QTextDocument>
#include <QWhatsThis>

using regina::Packet;
using regina::NormalHypersurface;

HyperUI::HyperUI(regina::NormalHypersurfaces* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane,
            ReginaPrefSet::global().tabHypersurfaceList) {
    HyperHeaderUI* header = new HyperHeaderUI(packet, this);
    addHeader(header);

    // WARNING: If these tabs are reordered, the code below that sets
    // the default tab must be updated accordingly.
    addTab(new HyperSummaryUI(packet, this), tr("&Summary"));

    coords = new HyperCoordinateUI(packet, this,
        newEnclosingPane->isReadWrite());
    addTab(coords, tr("Hypersurface &Coordinates"));

    addTab(new HyperMatchingUI(packet, this), tr("&Matching Equations"));

    compat = new HyperCompatibilityUI(packet, this);
    addTab(compat, tr("Com&patibility"));
}

const QLinkedList<QAction*>& HyperUI::getPacketTypeActions() {
    return coords->getPacketTypeActions();
}

QString HyperUI::getPacketMenuText() const {
    return tr("&Normal Hypersurfaces");
}

HyperHeaderUI::HyperHeaderUI(regina::NormalHypersurfaces* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        surfaces(packet) {
    header = new QLabel(0);
    header->setAlignment(Qt::AlignCenter);
    header->setMargin(10);
    header->setWhatsThis(header->tr("Displays the parameters of the "
        "enumeration that created this list of hypersurfaces, including "
        "the specific coordinate system that was originally used.  Also "
        "displays the total number of hypersurfaces in this list."));
    header->setTextInteractionFlags(Qt::TextBrowserInteraction);
    connect(header, SIGNAL(linkActivated(QString)),
        this, SLOT(viewTriangulation()));

    ui = header;

    // Listen for renaming events on the parent triangulation, since we
    // display its label in the header.
    packet->triangulation()->listen(this);
}

regina::Packet* HyperHeaderUI::getPacket() {
    return surfaces;
}

QWidget* HyperHeaderUI::getInterface() {
    return ui;
}

void HyperHeaderUI::refresh() {
    regina::HyperList which = surfaces->which();

    QString sEmb, sType;
    if (which.has(regina::HS_EMBEDDED_ONLY))
        sEmb = header->tr("embedded");
    else if (which.has(regina::HS_IMMERSED_SINGULAR))
        sEmb = header->tr("embedded / immersed / singular");
    else
        sEmb = header->tr("unknown");
    if (which.has(regina::HS_VERTEX))
        sType = header->tr("vertex");
    else if (which.has(regina::HS_FUNDAMENTAL))
        sType = header->tr("fundamental");
    else if (which.has(regina::HS_CUSTOM))
        sType = header->tr("custom");
    else if (which.has(regina::HS_LEGACY))
        sType = header->tr("legacy");
    else
        sType = header->tr("unknown");

    // Diagnostics:
    // regina::HyperAlg alg = surfaces->algorithm();
    // sType.append(QString(" [%1]").arg(alg.intValue()));

    QString count;
    if (surfaces->size() == 0)
        count = header->tr("No %1, %2 hypersurfaces").arg(sType).arg(sEmb);
    else if (surfaces->size() == 1)
        count = header->tr("1 %1, %2 hypersurface").arg(sType).arg(sEmb);
    else
        count = header->tr("%1 %2, %3 hypersurfaces").arg(
            surfaces->size()).arg(sType).arg(sEmb);

    header->setText(header->tr(
        "<qt>%1<br>Enumerated in %2 coordinates<br>"
        "Triangulation: <a href=\"#\">%3</a></qt>").
        arg(count).
        arg(header->tr(Coordinates::name(surfaces->coords(), false))).
        arg(QString(surfaces->triangulation()->humanLabel().c_str()).
            toHtmlEscaped()));
}

void HyperHeaderUI::viewTriangulation() {
    enclosingPane->getMainWindow()->packetView(surfaces->triangulation(),
        false /* visible in tree */, false /* select in tree */);
}

void HyperHeaderUI::packetWasRenamed(regina::Packet*) {
    // Assume it is the parent triangulation.
    refresh();
}

