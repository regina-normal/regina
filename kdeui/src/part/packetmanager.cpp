
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
#include "packettypes/ncontainerui.h"
#include "packettypes/ntextui.h"

#include <kiconloader.h>
#include <klibloader.h>
#include <klocale.h>
#include <ktexteditor/document.h>
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
        PacketPane* enclosingPane, bool allowReadWrite) {
    if (packet->getPacketType() == NContainer::packetType)
        return new NContainerUI(dynamic_cast<NContainer*>(packet),
            enclosingPane);
    if (packet->getPacketType() == NText::packetType) {
        KTextEditor::Document* doc = createDocument();
        if (doc) {
            doc->setReadWrite(allowReadWrite);
            return new NTextUI(dynamic_cast<NText*>(packet), enclosingPane,
                doc, allowReadWrite);
        } else
            return new ErrorPacketUI(packet, enclosingPane,
                i18n("An appropriate text editor component could not "
                "be found."));
    }
    return new DefaultPacketUI(packet, enclosingPane);
}

KTextEditor::Document* PacketManager::createDocument() {
    KTextEditor::Document* ans = 0;

    // TODO: There must be some way of doing this query so that system
    // settings are honoured.
    KService::Ptr service = KServiceTypeProfile::preferredService(
        "KTextEditor/Document", QString::null);
    if (service) {
        KLibFactory* libFactory = KLibLoader::self()->
            factory(service->library());
        if (libFactory)
            ans = dynamic_cast<KTextEditor::Document*>(libFactory->
                create(0 /* parent */, service->name(),
                "KTextEditor::Document"));
    }

    return ans;
}

