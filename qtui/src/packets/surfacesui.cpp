
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
#include "surfaces/normalsurfaces.h"
#include "triangulation/dim3.h"

// UI includes:
#include "./coordinates.h" // Use ./ to avoid picking up the iOS header.
#include "surfacesui.h"
#include "surfacescompatui.h"
#include "surfacescoordinateui.h"
#include "surfacesmatchingui.h"
#include "surfacessummaryui.h"
#include "tri3ui.h"
#include "reginamain.h"

#include <QLabel>
#include <QMessageBox>
#include <QTextDocument>
#include <QWhatsThis>

using regina::Packet;

SurfacesUI::SurfacesUI(regina::PacketOf<regina::NormalSurfaces>* packet,
        PacketPane* newEnclosingPane) :
        PacketTabbedUI(newEnclosingPane,
            ReginaPrefSet::global().tabSurfaceList) {
    addHeader(new SurfacesHeaderUI(packet, this));

    // WARNING: If these tabs are reordered, the code below that sets
    // the default tab must be updated accordingly.
    addTab(new SurfacesSummaryUI(packet, this), tr("&Summary"));

    coords = new SurfacesCoordinateUI(packet, this);
    addTab(coords, tr("Surface &Coordinates"));

    addTab(new SurfacesMatchingUI(packet, this), tr("&Matching Equations"));

    compat = new SurfacesCompatibilityUI(packet, this);
    addTab(compat, tr("Com&patibility"));
}

const std::vector<QAction*>& SurfacesUI::getPacketTypeActions() {
    return coords->getPacketTypeActions();
}

QString SurfacesUI::getPacketMenuText() const {
    return tr("&Normal Surfaces");
}

SurfacesHeaderUI::SurfacesHeaderUI(
        regina::PacketOf<regina::NormalSurfaces>* packet,
        PacketTabbedUI* useParentUI) : PacketViewerTab(useParentUI),
        surfaces(packet) {
    header = new QLabel(nullptr);
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

    // Listen for events on the underlying triangulation, since we
    // display its label in the header.
    if (auto p = packet->triangulation().inAnyPacket())
        std::const_pointer_cast<Packet>(p)->listen(this);
}

regina::Packet* SurfacesHeaderUI::getPacket() {
    return surfaces;
}

QWidget* SurfacesHeaderUI::getInterface() {
    return ui;
}

void SurfacesHeaderUI::refresh() {
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

    // Beware: we may be calling refresh() from packetBeingDestroyed(), in
    // which case the triangulation is no longer safe to query.  In this
    // scenario, isListening() will be false and triDestroyed will be true.
    QString triName;
    if (isListening()) {
        // The triangulation is a real packet, has not changed, and is
        // not currently being destroyed.
        if (auto p = surfaces->triangulation().inAnyPacket())
            triName = p->humanLabel().c_str();
        else {
            // This shouldn't happen.
            // It *was* once a packet: the only possibly explanation is that
            // we took a snapshot but for some reason no change event was fired.
            triName = tr("(private copy)");
        }
    } else {
        // Either the triangulation was never a real packet, or it once was
        // but the packet changed or was/is being destroyed.
        if (triDestroyed || surfaces->triangulation().isReadOnlySnapshot())
            triName = tr("(private copy)");
        else
            triName = tr("(anonymous)");
    }

    header->setText(header->tr(
        "<qt>%1<br>Enumerated in %2 coordinates<br>"
        "Triangulation: <a href=\"#\">%3</a></qt>").
        arg(count).
        arg(header->tr(Coordinates::name(surfaces->coords(), false))).
        arg(triName.toHtmlEscaped()));
}

void SurfacesHeaderUI::viewTriangulation() {
    const regina::Triangulation<3>& tri = surfaces->triangulation();
    auto triPkt = tri.inAnyPacket();
    if (! triPkt) {
        QMessageBox msg(QMessageBox::Information,
            tr("Create New Copy"),
            tr("Should I create a new copy of this triangulation?"),
            QMessageBox::Yes | QMessageBox::Cancel, ui);
        if (tri.isReadOnlySnapshot())
            msg.setInformativeText(tr("<qt>This list stores its own private "
                "copy of the triangulation, since the original has changed or "
                "been deleted.<p>"
                "Would you like me to make a new copy "
                "that you can view and edit?<p>"
                "This list will continue to use its own private copy, so "
                "you can edit or delete your new copy as you please.</qt>"));
        else
            msg.setInformativeText(tr("<qt>The triangulation is not "
                "part of this Regina data file.<p>"
                "Would you like me to make a new copy "
                "that you can view and edit here?</qt>"));
        msg.setDefaultButton(QMessageBox::Yes);
        if (msg.exec() != QMessageBox::Yes)
            return;

        auto copy = regina::makePacket<regina::Triangulation<3>>(
            std::in_place, tri);
        copy->setLabel(surfaces->adornedLabel("Triangulation"));
        surfaces->insertChildLast(copy);

        enclosingPane->getMainWindow()->packetView(copy, true, true);
    } else {
        enclosingPane->getMainWindow()->packetView(
            std::const_pointer_cast<Packet>(triPkt),
            false /* visible in tree */, false /* select in tree */);
    }
}

void SurfacesHeaderUI::packetWasRenamed(regina::Packet&) {
    // Assume it is the underlying triangulation.
    refresh();
}

void SurfacesHeaderUI::packetWasChanged(regina::Packet& packet) {
    // The underlying triangulation has changed.
    // Any such change *should* be immediately preceded by taking a local
    // snapshot, so the triangulation should be read-only from now on.
    packet.unlisten(this);
    refresh();
}

void SurfacesHeaderUI::packetBeingDestroyed(regina::PacketShell) {
    // Assume it is the underlying triangulation.
    triDestroyed = true;
    refresh();
}
