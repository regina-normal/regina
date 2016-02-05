
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

// Regina core includes:
#include "surfaces/nnormalsurfacelist.h"
#include "triangulation/ntriangulation.h"

// UI includes:
#include "coordinates.h"
#include "nnormalsurfaceui.h"
#include "nsurfacecompatui.h"
#include "nsurfacecoordinateui.h"
#include "nsurfacematchingui.h"
#include "nsurfacesummaryui.h"
#include "ntriangulationui.h"
#include "reginamain.h"

#include <QLabel>
#include <QTextDocument>
#include <QWhatsThis>

using regina::NPacket;
using regina::NNormalSurface;

NNormalSurfaceUI::NNormalSurfaceUI(regina::NNormalSurfaceList* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane,
            ReginaPrefSet::global().tabSurfaceList) {
    NSurfaceHeaderUI* header = new NSurfaceHeaderUI(packet, this);
    addHeader(header);

    // WARNING: If these tabs are reordered, the code below that sets
    // the default tab must be updated accordingly.
    addTab(new NSurfaceSummaryUI(packet, this), tr("&Summary"));

    coords = new NSurfaceCoordinateUI(packet, this,
        newEnclosingPane->isReadWrite());
    addTab(coords, tr("Surface &Coordinates"));

    addTab(new NSurfaceMatchingUI(packet, this), tr("&Matching Equations"));

    compat = new NSurfaceCompatibilityUI(packet, this);
    addTab(compat, tr("Com&patibility"));
}

const QLinkedList<QAction*>& NNormalSurfaceUI::getPacketTypeActions() {
    return coords->getPacketTypeActions();
}

QString NNormalSurfaceUI::getPacketMenuText() const {
    return tr("&Normal Surfaces");
}

NSurfaceHeaderUI::NSurfaceHeaderUI(regina::NNormalSurfaceList* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        surfaces(packet) {
    header = new QLabel(0);
    header->setAlignment(Qt::AlignCenter);
    header->setMargin(10);
    header->setWhatsThis(header->tr("Displays the parameters of the "
        "enumeration that created this list of surfaces, including "
        "the specific coordinate system that was originally used.  Also "
        "displays the total number of surfaces in this list."));
    header->setTextInteractionFlags(Qt::TextBrowserInteraction);
    connect(header, SIGNAL(linkActivated(QString)),
        this, SLOT(viewTriangulation()));

    ui = header;

    // Listen for renaming events on the parent triangulation, since we
    // display its label in the header.
    packet->triangulation()->listen(this);
}

regina::NPacket* NSurfaceHeaderUI::getPacket() {
    return surfaces;
}

QWidget* NSurfaceHeaderUI::getInterface() {
    return ui;
}

void NSurfaceHeaderUI::refresh() {
    regina::NormalList which = surfaces->which();

    QString sEmb, sType;
    if (which.has(regina::NS_EMBEDDED_ONLY))
        sEmb = header->tr("embedded");
    else if (which.has(regina::NS_IMMERSED_SINGULAR))
        sEmb = header->tr("embedded / immersed / singular");
    else
        sEmb = header->tr("unknown");
    if (which.has(regina::NS_VERTEX))
        sType = header->tr("vertex");
    else if (which.has(regina::NS_FUNDAMENTAL))
        sType = header->tr("fundamental");
    else if (which.has(regina::NS_CUSTOM))
        sType = header->tr("custom");
    else if (which.has(regina::NS_LEGACY))
        sType = header->tr("legacy");
    else
        sType = header->tr("unknown");

    // Diagnostics:
    // regina::NormalAlg alg = surfaces->algorithm();
    // sType.append(QString(" [%1]").arg(alg.intValue()));

    QString count;
    if (surfaces->size() == 0)
        count = header->tr("No %1, %2 surfaces").arg(sType).arg(sEmb);
    else if (surfaces->size() == 1)
        count = header->tr("1 %1, %2 surface").arg(sType).arg(sEmb);
    else
        count = header->tr("%1 %2, %3 surfaces").arg(
            surfaces->size()).arg(sType).arg(sEmb);

    header->setText(header->tr(
        "<qt>%1<br>Enumerated in %2 coordinates<br>"
        "Triangulation: <a href=\"#\">%3</a></qt>").
        arg(count).
        arg(header->tr(Coordinates::name(surfaces->coords(), false))).
        arg(QString(surfaces->triangulation()->humanLabel().c_str()).
            toHtmlEscaped()));
}

void NSurfaceHeaderUI::viewTriangulation() {
    enclosingPane->getMainWindow()->packetView(surfaces->triangulation(),
        false /* visible in tree */, false /* select in tree */);
}

void NSurfaceHeaderUI::packetWasRenamed(regina::NPacket*) {
    // Assume it is the parent triangulation.
    refresh();
}

