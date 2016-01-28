
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2014, Ben Burton                                   *
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
#include "dim4/dim4triangulation.h"
#include "hypersurface/nnormalhypersurfacelist.h"

// UI includes:
#include "coordinates.h"
#include "dim4triui.h"
#include "nhypersurfaceui.h"
#include "nhypercompatui.h"
#include "nhypercoordinateui.h"
#include "nhypermatchingui.h"
#include "nhypersummaryui.h"
#include "reginamain.h"

#include <QLabel>
#include <QTextDocument>
#include <QWhatsThis>

using regina::NPacket;
using regina::NNormalHypersurface;

NHyperSurfaceUI::NHyperSurfaceUI(regina::NNormalHypersurfaceList* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane,
            ReginaPrefSet::global().tabHypersurfaceList) {
    NHyperHeaderUI* header = new NHyperHeaderUI(packet, this);
    addHeader(header);

    // WARNING: If these tabs are reordered, the code below that sets
    // the default tab must be updated accordingly.
    addTab(new NHyperSummaryUI(packet, this), tr("&Summary"));

    coords = new NHyperCoordinateUI(packet, this,
        newEnclosingPane->isReadWrite());
    addTab(coords, tr("Hypersurface &Coordinates"));

    addTab(new NHyperMatchingUI(packet, this), tr("&Matching Equations"));

    compat = new NHyperCompatibilityUI(packet, this);
    addTab(compat, tr("Com&patibility"));
}

const QLinkedList<QAction*>& NHyperSurfaceUI::getPacketTypeActions() {
    return coords->getPacketTypeActions();
}

QString NHyperSurfaceUI::getPacketMenuText() const {
    return tr("&Normal Hypersurfaces");
}

NHyperHeaderUI::NHyperHeaderUI(regina::NNormalHypersurfaceList* packet,
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

regina::NPacket* NHyperHeaderUI::getPacket() {
    return surfaces;
}

QWidget* NHyperHeaderUI::getInterface() {
    return ui;
}

void NHyperHeaderUI::refresh() {
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

void NHyperHeaderUI::viewTriangulation() {
    enclosingPane->getMainWindow()->packetView(surfaces->triangulation(),
        false /* visible in tree */, false /* select in tree */);
}

void NHyperHeaderUI::packetWasRenamed(regina::NPacket*) {
    // Assume it is the parent triangulation.
    refresh();
}

