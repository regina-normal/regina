
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
#include "packet/packetregistry.h"
#include "surfaces/filterregistry.h"

// UI includes:
#include "iconcache.h"
#include "packetmanager.h"
#include "reginamain.h"
#include "reginasupport.h"
#include "packets/anglesui.h"
#include "packets/tri2ui.h"
#include "packets/tri4ui.h"
#include "packets/filtercomb.h"
#include "packets/filterprop.h"
#include "packets/generictriui.h"
#include "packets/containerui.h"
#include "packets/hyperui.h"
#include "packets/pdfui.h"
#include "packets/scriptui.h"
#include "packets/snappeaui.h"
#include "packets/surfacesui.h"
#include "packets/textui.h"
#include "packets/tri3ui.h"

#include <QPlainTextEdit>

using namespace regina;

QIcon PacketManager::icon(Packet* packet, bool allowLock) {
    IconCache::IconID id;

    switch (packet->type()) {
        case PACKET_ANGLESTRUCTURES :
            id = IconCache::packet_angles;
            break;
        case PACKET_CONTAINER :
            id = (packet->parent() ? IconCache::packet_container :
                IconCache::regina);
            break;
        case PACKET_PDF :
            id = IconCache::packet_pdf;
            break;
        case PACKET_SURFACEFILTER :
            switch (((SurfaceFilter*)packet)->filterType()) {
                case NS_FILTER_COMBINATION :
                    id = IconCache::filter_comb;
                    break;
                case NS_FILTER_PROPERTIES :
                    id = IconCache::filter_prop;
                    break;
                default:
                    id = IconCache::packet_filter;
                    break;
                }
            break;
        case PACKET_SCRIPT :
            id = IconCache::packet_script;
            break;
        case PACKET_SNAPPEATRIANGULATION :
            id = IconCache::packet_snappea;
            break;
        case PACKET_NORMALSURFACES :
            id = IconCache::packet_surfaces;
            break;
        case PACKET_NORMALHYPERSURFACES :
            id = IconCache::packet_hypersurfaces;
            break;
        case PACKET_TEXT :
            id = IconCache::packet_text;
            break;
        case PACKET_TRIANGULATION2 :
            id = IconCache::packet_triangulation2;
            break;
        case PACKET_TRIANGULATION3 :
            id = IconCache::packet_triangulation3;
            break;
        case PACKET_TRIANGULATION4 :
            id = IconCache::packet_triangulation4;
            break;
        // For generic dimensions, we don't cache the icons.
        case PACKET_TRIANGULATION5 :
            return ReginaSupport::regIcon("packet_triangulation5");
        case PACKET_TRIANGULATION6 :
            return ReginaSupport::regIcon("packet_triangulation6");
        case PACKET_TRIANGULATION7 :
            return ReginaSupport::regIcon("packet_triangulation7");
        case PACKET_TRIANGULATION8 :
            return ReginaSupport::regIcon("packet_triangulation8");
        case PACKET_TRIANGULATION9 :
            return ReginaSupport::regIcon("packet_triangulation9");
        case PACKET_TRIANGULATION10 :
            return ReginaSupport::regIcon("packet_triangulation10");
        case PACKET_TRIANGULATION11 :
            return ReginaSupport::regIcon("packet_triangulation11");
        case PACKET_TRIANGULATION12 :
            return ReginaSupport::regIcon("packet_triangulation12");
        case PACKET_TRIANGULATION13 :
            return ReginaSupport::regIcon("packet_triangulation13");
        case PACKET_TRIANGULATION14 :
            return ReginaSupport::regIcon("packet_triangulation14");
        case PACKET_TRIANGULATION15 :
            return ReginaSupport::regIcon("packet_triangulation15");
        default:
            // Unknown packet type.
            return QIcon();
    }

    if (allowLock && ! packet->isPacketEditable())
        return IconCache::lockedIcon(id);
    else
        return IconCache::icon(id);
}

PacketUI* PacketManager::createUI(regina::Packet* packet,
        PacketPane* enclosingPane) {
    switch (packet->type()) {
        case PACKET_ANGLESTRUCTURES:
            return new AngleStructureUI(
                dynamic_cast<AngleStructures*>(packet), enclosingPane);
        case PACKET_CONTAINER:
            return new ContainerUI(
                dynamic_cast<Container*>(packet), enclosingPane);
        case PACKET_NORMALSURFACES:
            return new SurfacesUI(
                dynamic_cast<NormalSurfaces*>(packet), enclosingPane);
        case PACKET_NORMALHYPERSURFACES:
            return new HyperUI(
                dynamic_cast<NormalHypersurfaces*>(packet), enclosingPane);
        case PACKET_SCRIPT:
            return new ScriptUI(
                dynamic_cast<Script*>(packet), enclosingPane);
        case PACKET_SNAPPEATRIANGULATION:
            return new SnapPeaUI(
                dynamic_cast<SnapPeaTriangulation*>(packet), enclosingPane);
        case PACKET_SURFACEFILTER:
            switch (((SurfaceFilter*)packet)->filterType()) {
                case NS_FILTER_COMBINATION:
                    return new FilterCombUI(
                        dynamic_cast<SurfaceFilterCombination*>(packet),
                        enclosingPane);
                case NS_FILTER_PROPERTIES:
                    return new FilterPropUI(
                        dynamic_cast<SurfaceFilterProperties*>(packet),
                        enclosingPane);
                default:
                    return new DefaultPacketUI(packet, enclosingPane);
            }
        case PACKET_TEXT:
            return new TextUI(
                dynamic_cast<Text*>(packet), enclosingPane);
        case PACKET_TRIANGULATION2:
            return new Tri2UI(
                dynamic_cast<Triangulation<2>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION3:
            return new Tri3UI(
                dynamic_cast<Triangulation<3>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION4:
            return new Tri4UI(
                dynamic_cast<Triangulation<4>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION5:
            return new GenericTriangulationUI<5>(
                dynamic_cast<Triangulation<5>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION6:
            return new GenericTriangulationUI<6>(
                dynamic_cast<Triangulation<6>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION7:
            return new GenericTriangulationUI<7>(
                dynamic_cast<Triangulation<7>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION8:
            return new GenericTriangulationUI<8>(
                dynamic_cast<Triangulation<8>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION9:
            return new GenericTriangulationUI<9>(
                dynamic_cast<Triangulation<9>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION10:
            return new GenericTriangulationUI<10>(
                dynamic_cast<Triangulation<10>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION11:
            return new GenericTriangulationUI<11>(
                dynamic_cast<Triangulation<11>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION12:
            return new GenericTriangulationUI<12>(
                dynamic_cast<Triangulation<12>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION13:
            return new GenericTriangulationUI<13>(
                dynamic_cast<Triangulation<13>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION14:
            return new GenericTriangulationUI<14>(
                dynamic_cast<Triangulation<14>*>(packet), enclosingPane);
        case PACKET_TRIANGULATION15:
            return new GenericTriangulationUI<15>(
                dynamic_cast<Triangulation<15>*>(packet), enclosingPane);
        default:
            return new DefaultPacketUI(packet, enclosingPane);
    }
}

PacketExternalViewer PacketManager::externalViewer(regina::Packet* packet) {
    if (packet->type() == PACKET_PDF)
        return &PDFExternalViewer::view;
    return 0;
}

