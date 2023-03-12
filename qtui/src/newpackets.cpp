
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2023, Ben Burton                                   *
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

#include "packet/container.h"
#include "packet/script.h"
#include "packet/text.h"
#include "snappea/snappeatriangulation.h"
#include "triangulation/dim4.h"

#include "newpacketdialog.h"
#include "packetcreator.h"
#include "packettreeview.h"
#include "reginamain.h"
#include "packets/tri2creator.h"
#include "packets/tri4creator.h"
#include "packets/anglescreator.h"
#include "packets/filtercreator.h"
#include "packets/linkcreator.h"
#include "packets/hypercreator.h"
#include "packets/tri3creator.h"
#include "packets/snappeacreator.h"
#include "packets/surfacescreator.h"

template <typename PacketCreatorClass>
void ReginaMain::newPacket(const QString& dialogTitle) {
    NewPacketDialog dlg(this, new PacketCreatorClass(this), packetTree,
        treeView->selectedPacket(), dialogTitle);
    if (dlg.validate() && dlg.exec() == QDialog::Accepted) {
        auto newPacket = dlg.createdPacket();
        if (newPacket) {
            // Open a UI for the new packet, and select it in the tree.
            packetView(*newPacket, true, true);
        }
    }
}

void ReginaMain::newAngleStructures() {
    newPacket<AngleStructureCreator>(tr("New Angle Structure Solutions"));
}

void ReginaMain::newContainer() {
    newPacket<BasicPacketCreator<regina::Container>>(tr("New Container"));
}

void ReginaMain::newFilter() {
    newPacket<FilterCreator>(tr("New Normal Surface Filter"));
}

void ReginaMain::newNormalSurfaces() {
    newPacket<SurfacesCreator>(tr("New Normal Surface List"));
}

void ReginaMain::newNormalHypersurfaces() {
    newPacket<HyperCreator>(tr("New Normal Hypersurface List"));
}

void ReginaMain::newAttachment() {
    // An attachment must be an import; just use the import routine directly.
    importAttachment();
}

void ReginaMain::newScript() {
    newPacket<BasicPacketCreator<regina::Script>>(tr("New Script"));
}

void ReginaMain::newSnapPeaTriangulation() {
    newPacket<SnapPeaTriangulationCreator>(tr("New SnapPea Triangulation"));
}

void ReginaMain::newLink() {
    newPacket<LinkCreator>(tr("New Knot or Link"));
}

void ReginaMain::newText() {
    newPacket<BasicPacketCreator<regina::Text>>(tr("New Text Packet"));
}

void ReginaMain::newTriangulation2() {
    newPacket<Tri2Creator>(tr("New 2-Manifold Triangulation"));
}

void ReginaMain::newTriangulation3() {
    newPacket<Tri3Creator>(tr("New 3-Manifold Triangulation"));
}

void ReginaMain::newTriangulation4() {
    newPacket<Tri4Creator>(tr("New 4-Manifold Triangulation"));
}

