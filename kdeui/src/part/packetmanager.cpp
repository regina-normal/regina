
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  KDE User Interface                                                    *
 *                                                                        *
 *  Copyright (c) 1999-2003, Ben Burton                                   *
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
 *  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,        *
 *  MA 02111-1307, USA.                                                   *
 *                                                                        *
 **************************************************************************/

/* end stub */

// Regina core includes:
#include "packet/packetregistry.h"
#include "surfaces/filterregistry.h"

// UI includes:
#include "packetmanager.h"
#include "reginapart.h"
#include "packettypes/nanglestructureui.h"
#include "packettypes/ncontainerui.h"
#include "packettypes/nnormalsurfaceui.h"
#include "packettypes/nscriptui.h"
#include "packettypes/nsurfacefiltercomb.h"
#include "packettypes/nsurfacefilterprop.h"
#include "packettypes/ntextui.h"
#include "packettypes/ntriangulationui.h"

#include <kiconloader.h>
#include <klibloader.h>
#include <klocale.h>
#include <ktexteditor/document.h>
#include <ktexteditor/editorchooser.h>
#include <kuserprofile.h>

using namespace regina;

QPixmap PacketManager::iconSmall(NPacket* packet) {
    if (packet->getPacketType() == NAngleStructureList::packetType)
        return SmallIcon("packet_angles", ReginaPart::factoryInstance());
    if (packet->getPacketType() == NContainer::packetType)
        return SmallIcon("packet_container", ReginaPart::factoryInstance());
    if (packet->getPacketType() == NSurfaceFilter::packetType) {
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterCombination::filterID)
            return SmallIcon("filter_comb", ReginaPart::factoryInstance());
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterProperties::filterID)
            return SmallIcon("filter_prop", ReginaPart::factoryInstance());
        return SmallIcon("packet_filter", ReginaPart::factoryInstance());
    }
    if (packet->getPacketType() == NScript::packetType)
        return SmallIcon("packet_script", ReginaPart::factoryInstance());
    if (packet->getPacketType() == NNormalSurfaceList::packetType)
        return SmallIcon("packet_surfaces", ReginaPart::factoryInstance());
    if (packet->getPacketType() == NText::packetType)
        return SmallIcon("packet_text", ReginaPart::factoryInstance());
    if (packet->getPacketType() == NTriangulation::packetType)
        return SmallIcon("packet_triangulation", ReginaPart::factoryInstance());
    return QPixmap();
}

QPixmap PacketManager::iconBar(NPacket* packet) {
    if (packet->getPacketType() == NAngleStructureList::packetType)
        return BarIcon("packet_angles", ReginaPart::factoryInstance());
    if (packet->getPacketType() == NContainer::packetType)
        return BarIcon("packet_container", ReginaPart::factoryInstance());
    if (packet->getPacketType() == NSurfaceFilter::packetType) {
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterCombination::filterID)
            return BarIcon("filter_comb", ReginaPart::factoryInstance());
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterProperties::filterID)
            return BarIcon("filter_prop", ReginaPart::factoryInstance());
        return BarIcon("packet_filter", ReginaPart::factoryInstance());
    }
    if (packet->getPacketType() == NScript::packetType)
        return BarIcon("packet_script", ReginaPart::factoryInstance());
    if (packet->getPacketType() == NNormalSurfaceList::packetType)
        return BarIcon("packet_surfaces", ReginaPart::factoryInstance());
    if (packet->getPacketType() == NText::packetType)
        return BarIcon("packet_text", ReginaPart::factoryInstance());
    if (packet->getPacketType() == NTriangulation::packetType)
        return BarIcon("packet_triangulation", ReginaPart::factoryInstance());
    return QPixmap();
}

PacketUI* PacketManager::createUI(regina::NPacket* packet,
        PacketPane* enclosingPane) {
    ReginaPart* part = enclosingPane->getPart();
    bool readWrite = part->isReadWrite() && packet->isPacketEditable();

    if (packet->getPacketType() == NAngleStructureList::packetType)
        return new NAngleStructureUI(dynamic_cast<NAngleStructureList*>(packet),
            enclosingPane);
    if (packet->getPacketType() == NContainer::packetType)
        return new NContainerUI(dynamic_cast<NContainer*>(packet),
            enclosingPane);
    if (packet->getPacketType() == NNormalSurfaceList::packetType)
        return new NNormalSurfaceUI(dynamic_cast<NNormalSurfaceList*>(packet),
            enclosingPane, readWrite);
    if (packet->getPacketType() == NScript::packetType) {
        KTextEditor::Document* doc = createDocument();
        if (doc)
            return new NScriptUI(dynamic_cast<NScript*>(packet), enclosingPane,
                doc, readWrite);
        else
            return new ErrorPacketUI(packet, enclosingPane,
                i18n("An appropriate text editor component could not "
                "be found."));
    }
    if (packet->getPacketType() == NSurfaceFilter::packetType) {
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterCombination::filterID)
            return new NSurfaceFilterCombUI(
                dynamic_cast<NSurfaceFilterCombination*>(packet),
                enclosingPane, readWrite);
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterProperties::filterID)
            return new NSurfaceFilterPropUI(
                dynamic_cast<NSurfaceFilterProperties*>(packet),
                enclosingPane, readWrite);
        return new DefaultPacketUI(packet, enclosingPane);
    }
    if (packet->getPacketType() == NText::packetType) {
        KTextEditor::Document* doc = createDocument();
        if (doc)
            return new NTextUI(dynamic_cast<NText*>(packet), enclosingPane,
                doc, readWrite);
        else
            return new ErrorPacketUI(packet, enclosingPane,
                i18n("An appropriate text editor component\n"
                "could not be found."));
    }
    if (packet->getPacketType() == NTriangulation::packetType)
        return new NTriangulationUI(dynamic_cast<NTriangulation*>(packet),
            enclosingPane, readWrite);
    return new DefaultPacketUI(packet, enclosingPane);
}

KTextEditor::Document* PacketManager::createDocument() {
    return KTextEditor::EditorChooser::createDocument();
}

