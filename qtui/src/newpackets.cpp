
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

#include "packet/ncontainer.h"
#include "packet/nscript.h"
#include "packet/ntext.h"
#include "triangulation/ntriangulation.h"

#include "newpacketdialog.h"
#include "packetcreator.h"
#include "packetfilter.h"
#include "packettreeview.h"
#include "reginamain.h"
#include "packettypes/nanglestructurecreator.h"
#include "packettypes/nnormalsurfacecreator.h"
#include "packettypes/nsurfacefiltercreator.h"
#include "packettypes/ntriangulationcreator.h"

void ReginaMain::newAngleStructures() {
    newPacket(new NAngleStructureCreator(),
        new SingleTypeFilter<regina::NTriangulation>(),
        tr("New Angle Structure Solutions"), tr("Angle Structures"));
}

void ReginaMain::newContainer() {
    newPacket(new BasicPacketCreator<regina::NContainer>(), 0,
        tr("New Container"), tr("Container"));
}

void ReginaMain::newFilter() {
    newPacket(new NSurfaceFilterCreator(), 0,
        tr("New Normal Surface Filter"), tr("Surface Filter"));
}

void ReginaMain::newNormalSurfaces() {
    newPacket(new NNormalSurfaceCreator(),
        new SingleTypeFilter<regina::NTriangulation>(),
        tr("New Normal Surface List"), tr("Normal Surfaces"));
}

void ReginaMain::newPDF() {
    // A PDF must be an import; just use the import routine directly.
    importPDF();
}

void ReginaMain::newScript() {
    newPacket(new BasicPacketCreator<regina::NScript>(), 0,
        tr("New Script"), tr("Script"));
}

void ReginaMain::newText() {
    newPacket(new BasicPacketCreator<regina::NText>(), 0,
        tr("New Text Packet"), tr("Text"));
}

void ReginaMain::newTriangulation() {
    newPacket(new NTriangulationCreator(), 0,
        tr("New Triangulation"), tr("Triangulation"));
}

void ReginaMain::newPacket(PacketCreator* creator, PacketFilter* parentFilter,
        const QString& dialogTitle, const QString& suggestedLabel) {
    NewPacketDialog dlg(this, creator, packetTree,
        treeView->selectedPacket(), parentFilter, dialogTitle, suggestedLabel);
    if (dlg.validate() && dlg.exec() == QDialog::Accepted) {
        regina::NPacket* newPacket = dlg.createdPacket();
        if (newPacket) {
            // Open a UI for the new packet, and select it in the tree.
            packetView(newPacket, true, true);
        }
    }
}

