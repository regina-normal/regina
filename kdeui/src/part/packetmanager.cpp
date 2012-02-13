
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
#include "packet/packetregistry.h"
#include "surfaces/filterregistry.h"

// UI includes:
#include "packetmanager.h"
#include "reginapart.h"
#include "reginasupport.h"
#include "packettypes/nanglestructureui.h"
#include "packettypes/ncontainerui.h"
#include "packettypes/nnormalsurfaceui.h"
#include "packettypes/npdfui.h"
#include "packettypes/nscriptui.h"
#include "packettypes/nsurfacefiltercomb.h"
#include "packettypes/nsurfacefilterprop.h"
#include "packettypes/ntextui.h"
#include "packettypes/ntriangulationui.h"

#include <QPainter>
#include <QPixmap>
#include <QPlainTextEdit>

using namespace regina;

bool PacketManager::lockInitialised = false;
QImage PacketManager::lockSmall;
QImage PacketManager::lockBar;

QPixmap PacketManager::iconSmall(NPacket* packet, bool allowLock) {
    QString name = iconName(packet);
    if (name.isNull())
        return QPixmap();


    // TODO Does this work? emblem-locked and emblem aren't in the latest 
    // freedesktop spec.
    if (allowLock && ! packet->isPacketEditable()) {
        QPixmap overlay = ReginaSupport::themeIcon("emblem-locked").pixmap(8,8);
        QPixmap icon = ReginaSupport::regIcon(name).pixmap(16,16);
        QPixmap *result = new QPixmap(icon.width(),icon.height());
        result->fill(Qt::transparent);
        QPainter painter(result);
        painter.drawPixmap(0,0,icon);
        painter.drawPixmap(0,0,overlay);
        return *result; 
    }
    return ReginaSupport::regIcon(name).pixmap(16,16);
}

QPixmap PacketManager::iconBar(NPacket* packet, bool allowLock) {
    QString name = iconName(packet);
    if (name.isNull())
        return QPixmap();

    // TODO Sizes here. KDE says "use KIconLoader::Toolbar size" but I cannot
    // find a reference to what size that is, so using 16x16
    if (allowLock && ! packet->isPacketEditable()) {
        QPixmap overlay = ReginaSupport::themeIcon("emblem-locked").pixmap(8,8);
        QPixmap icon = ReginaSupport::regIcon(name).pixmap(16,16);
        QPixmap *result = new QPixmap(icon.width(),icon.height());
        result->fill(Qt::transparent);
        QPainter painter(result);
        painter.drawPixmap(0,0,icon);
        painter.drawPixmap(0,0,overlay);
        return *result; 
    }
    return ReginaSupport::regIcon(name).pixmap(16,16); 
}

PacketUI* PacketManager::createUI(regina::NPacket* packet,
        PacketPane* enclosingPane) {
    if (packet->getPacketType() == NAngleStructureList::packetType)
        return new NAngleStructureUI(dynamic_cast<NAngleStructureList*>(packet),
            enclosingPane);
    if (packet->getPacketType() == NContainer::packetType)
        return new NContainerUI(dynamic_cast<NContainer*>(packet),
            enclosingPane);
    if (packet->getPacketType() == NNormalSurfaceList::packetType)
        return new NNormalSurfaceUI(dynamic_cast<NNormalSurfaceList*>(packet),
            enclosingPane);
    if (packet->getPacketType() == NPDF::packetType)
        return new NPDFUI(dynamic_cast<NPDF*>(packet), enclosingPane);
    if (packet->getPacketType() == NScript::packetType) {
        return new NScriptUI(dynamic_cast<NScript*>(packet), enclosingPane);
    }
    if (packet->getPacketType() == NSurfaceFilter::packetType) {
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterCombination::filterID)
            return new NSurfaceFilterCombUI(
                dynamic_cast<NSurfaceFilterCombination*>(packet),
                enclosingPane);
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterProperties::filterID)
            return new NSurfaceFilterPropUI(
                dynamic_cast<NSurfaceFilterProperties*>(packet),
                enclosingPane);
        return new DefaultPacketUI(packet, enclosingPane);
    }
    if (packet->getPacketType() == NText::packetType) {
        return new NTextUI(dynamic_cast<NText*>(packet), enclosingPane);
    }
    if (packet->getPacketType() == NTriangulation::packetType)
        return new NTriangulationUI(dynamic_cast<NTriangulation*>(packet),
            enclosingPane);
    return new DefaultPacketUI(packet, enclosingPane);
}

QString PacketManager::iconName(NPacket* packet) {
    if (packet->getPacketType() == NAngleStructureList::packetType)
        return "packet_angles";
    if (packet->getPacketType() == NContainer::packetType)
        return "packet_container";
    if (packet->getPacketType() == NPDF::packetType)
        return "packet_pdf";
    if (packet->getPacketType() == NSurfaceFilter::packetType) {
        if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterCombination::filterID)
            return "filter_comb";
        else if (((NSurfaceFilter*)packet)->getFilterID() ==
                NSurfaceFilterProperties::filterID)
            return "filter_prop";
        else
            return "packet_filter";
    }
    if (packet->getPacketType() == NScript::packetType)
        return "packet_script";
    if (packet->getPacketType() == NNormalSurfaceList::packetType)
        return "packet_surfaces";
    if (packet->getPacketType() == NText::packetType)
        return "packet_text";
    if (packet->getPacketType() == NTriangulation::packetType)
        return "packet_triangulation";

    return QString();
}
