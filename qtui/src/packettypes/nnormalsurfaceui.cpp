
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
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
        PacketTabbedUI(newEnclosingPane) {
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

    // Select the default tab.
    switch(ReginaPrefSet::global().surfacesInitialTab) {
        case ReginaPrefSet::Summary:
            /* already visible */ break;
        case ReginaPrefSet::Coordinates:
            setCurrentTab(1); break;
        case ReginaPrefSet::Matching:
            setCurrentTab(2); break;
        case ReginaPrefSet::Compatibility:
            setCurrentTab(3); break;
    }
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
        "vertex enumeration that created this list of surfaces, including "
        "the specific coordinate system that was originally used.  Also "
        "displays the total number of surfaces in this list."));
    header->setTextInteractionFlags(Qt::TextBrowserInteraction);
    connect(header, SIGNAL(linkActivated(QString)),
        this, SLOT(viewTriangulation()));

    ui = header;

    // Listen for renaming events on the parent triangulation, since we
    // display its label in the header.
    packet->getTriangulation()->listen(this);
}

regina::NPacket* NSurfaceHeaderUI::getPacket() {
    return surfaces;
}

QWidget* NSurfaceHeaderUI::getInterface() {
    return ui;
}

void NSurfaceHeaderUI::refresh() {
    QString embType = (surfaces->isEmbeddedOnly() ? header->tr("embedded") :
        header->tr("embedded / immersed / singular"));

    QString count;
    if (surfaces->getNumberOfSurfaces() == 0)
        count = header->tr("No %1 normal surfaces").arg(embType);
    else if (surfaces->getNumberOfSurfaces() == 1)
        count = header->tr("1 %1 normal surface").arg(embType);
    else
        count = header->tr("%1 %2 normal surfaces").arg(
            surfaces->getNumberOfSurfaces()).arg(embType);

    header->setText(header->tr(
        "<qt>%1<br>Enumerated in %2 coordinates<br>"
        "Triangulation: <a href=\"#\">%3</a></qt>").
        arg(count).
        arg(header->tr(Coordinates::name(surfaces->getFlavour(), false))).
        arg(Qt::escape(
            surfaces->getTriangulation()->getPacketLabel().c_str())));
}

void NSurfaceHeaderUI::viewTriangulation() {
    enclosingPane->getMainWindow()->packetView(surfaces->getTriangulation(),
        false /* visible in tree */, false /* select in tree */);
}

void NSurfaceHeaderUI::packetWasRenamed(regina::NPacket*) {
    // Assume it is the parent triangulation.
    refresh();
}

