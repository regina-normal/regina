
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

// Regina core includes:
#include "regina-config.h" // for REGINA_HIGHDIM
#include "hypersurface/normalhypersurfaces.h"
#include "link/link.h"
#include "packet/attachment.h"
#include "packet/container.h"
#include "packet/script.h"
#include "packet/text.h"
#include "snappea/snappeatriangulation.h"
#include "surface/normalsurfaces.h"
#include "surface/surfacefilter.h"
#include "triangulation/dim2.h"
#include "triangulation/dim3.h"
#include "triangulation/dim4.h"

// UI includes:
#include "iconcache.h"
#include "packetmanager.h"
#include "reginamain.h"
#include "reginasupport.h"
#include "packets/anglesui.h"
#include "packets/attachmentui.h"
#include "packets/tri2ui.h"
#include "packets/tri4ui.h"
#include "packets/filtercomb.h"
#include "packets/filterprop.h"
#include "packets/generictriui.h"
#include "packets/containerui.h"
#include "packets/hyperui.h"
#include "packets/linkui.h"
#include "packets/scriptui.h"
#include "packets/snappeaui.h"
#include "packets/spatiallinkui.h"
#include "packets/surfacesui.h"
#include "packets/textui.h"
#include "packets/tri3ui.h"

#include <QPlainTextEdit>

using namespace regina;

QIcon PacketManager::icon(const Packet& packet) {
    IconCache::IconID id;

    switch (packet.type()) {
        case PacketType::AngleStructures:
            id = IconCache::packet_angles;
            break;
        case PacketType::Container:
            id = (packet.parent() ? IconCache::packet_container :
                IconCache::regina);
            break;
        case PacketType::Link:
            id = IconCache::packet_link;
            break;
        case PacketType::SpatialLink:
            id = IconCache::packet_spatiallink;
            break;
        case PacketType::Attachment:
            id = IconCache::packet_attachment;
            break;
        case PacketType::SurfaceFilter:
            switch (((const SurfaceFilter&)packet).filterType()) {
                case SurfaceFilterType::Combination:
                    id = IconCache::filter_comb;
                    break;
                case SurfaceFilterType::Properties:
                    id = IconCache::filter_prop;
                    break;
                default:
                    id = IconCache::packet_filter;
                    break;
                }
            break;
        case PacketType::Script:
            id = IconCache::packet_script;
            break;
        case PacketType::SnapPea:
            id = IconCache::packet_snappea;
            break;
        case PacketType::NormalSurfaces:
            id = IconCache::packet_surfaces;
            break;
        case PacketType::NormalHypersurfaces:
            id = IconCache::packet_hypersurfaces;
            break;
        case PacketType::Text:
            id = IconCache::packet_text;
            break;
        case PacketType::Triangulation2:
            id = IconCache::packet_triangulation2;
            break;
        case PacketType::Triangulation3:
            id = IconCache::packet_triangulation3;
            break;
        case PacketType::Triangulation4:
            id = IconCache::packet_triangulation4;
            break;
        // For generic dimensions, we don't cache the icons.
        case PacketType::Triangulation5:
            return ReginaSupport::regIcon("packet_triangulation5");
        case PacketType::Triangulation6:
            return ReginaSupport::regIcon("packet_triangulation6");
        case PacketType::Triangulation7:
            return ReginaSupport::regIcon("packet_triangulation7");
        case PacketType::Triangulation8:
            return ReginaSupport::regIcon("packet_triangulation8");
#ifdef REGINA_HIGHDIM
        case PacketType::Triangulation9:
            return ReginaSupport::regIcon("packet_triangulation9");
        case PacketType::Triangulation10:
            return ReginaSupport::regIcon("packet_triangulation10");
        case PacketType::Triangulation11:
            return ReginaSupport::regIcon("packet_triangulation11");
        case PacketType::Triangulation12:
            return ReginaSupport::regIcon("packet_triangulation12");
        case PacketType::Triangulation13:
            return ReginaSupport::regIcon("packet_triangulation13");
        case PacketType::Triangulation14:
            return ReginaSupport::regIcon("packet_triangulation14");
        case PacketType::Triangulation15:
            return ReginaSupport::regIcon("packet_triangulation15");
#endif /* REGINA_HIGHDIM */
        default:
            // Unknown packet type.
            return QIcon();
    }

    return IconCache::icon(id);
}

PacketUI* PacketManager::createUI(regina::Packet& packet,
        PacketPane* enclosingPane) {
    switch (packet.type()) {
        case PacketType::AngleStructures:
            return new AngleStructureUI(
                static_cast<regina::PacketOf<AngleStructures>*>(&packet),
                    enclosingPane);
        case PacketType::Attachment:
            return new AttachmentUI(
                static_cast<Attachment*>(&packet), enclosingPane);
        case PacketType::Container:
            return new ContainerUI(
                static_cast<Container*>(&packet), enclosingPane);
        case PacketType::Link:
            return new LinkUI(
                static_cast<regina::PacketOf<Link>*>(&packet), enclosingPane);
        case PacketType::SpatialLink:
            return new SpatialLinkUI(
                static_cast<regina::PacketOf<SpatialLink>*>(&packet),
                    enclosingPane);
        case PacketType::NormalSurfaces:
            return new SurfacesUI(
                static_cast<regina::PacketOf<NormalSurfaces>*>(&packet),
                    enclosingPane);
        case PacketType::NormalHypersurfaces:
            return new HyperUI(
                static_cast<regina::PacketOf<NormalHypersurfaces>*>(&packet),
                    enclosingPane);
        case PacketType::Script:
            return new ScriptUI(
                static_cast<Script*>(&packet), enclosingPane);
        case PacketType::SnapPea:
            return new SnapPeaUI(
                static_cast<regina::PacketOf<SnapPeaTriangulation>*>(&packet),
                enclosingPane);
        case PacketType::SurfaceFilter:
            switch (((SurfaceFilter&)packet).filterType()) {
                case SurfaceFilterType::Combination:
                    return new FilterCombUI(
                        static_cast<SurfaceFilterCombination*>(&packet),
                        enclosingPane);
                case SurfaceFilterType::Properties:
                    return new FilterPropUI(
                        static_cast<SurfaceFilterProperties*>(&packet),
                        enclosingPane);
                default:
                    return new DefaultPacketUI(&packet, enclosingPane);
            }
        case PacketType::Text:
            return new TextUI(
                static_cast<Text*>(&packet), enclosingPane);
        case PacketType::Triangulation2:
            return new Tri2UI(
                static_cast<regina::PacketOf<Triangulation<2>>*>(&packet),
                enclosingPane);
        case PacketType::Triangulation3:
            return new Tri3UI(
                static_cast<regina::PacketOf<Triangulation<3>>*>(&packet),
                enclosingPane);
        case PacketType::Triangulation4:
            return new Tri4UI(
                static_cast<regina::PacketOf<Triangulation<4>>*>(&packet),
                enclosingPane);
        case PacketType::Triangulation5:
            return new GenericTriangulationUI<5>(
                static_cast<regina::PacketOf<Triangulation<5>>*>(&packet),
                enclosingPane);
        case PacketType::Triangulation6:
            return new GenericTriangulationUI<6>(
                static_cast<regina::PacketOf<Triangulation<6>>*>(&packet),
                enclosingPane);
        case PacketType::Triangulation7:
            return new GenericTriangulationUI<7>(
                static_cast<regina::PacketOf<Triangulation<7>>*>(&packet),
                enclosingPane);
        case PacketType::Triangulation8:
            return new GenericTriangulationUI<8>(
                static_cast<regina::PacketOf<Triangulation<8>>*>(&packet),
                enclosingPane);
#ifdef REGINA_HIGHDIM
        case PacketType::Triangulation9:
            return new GenericTriangulationUI<9>(
                static_cast<regina::PacketOf<Triangulation<9>>*>(&packet),
                enclosingPane);
        case PacketType::Triangulation10:
            return new GenericTriangulationUI<10>(
                static_cast<regina::PacketOf<Triangulation<10>>*>(&packet),
                enclosingPane);
        case PacketType::Triangulation11:
            return new GenericTriangulationUI<11>(
                static_cast<regina::PacketOf<Triangulation<11>>*>(&packet),
                enclosingPane);
        case PacketType::Triangulation12:
            return new GenericTriangulationUI<12>(
                static_cast<regina::PacketOf<Triangulation<12>>*>(&packet),
                enclosingPane);
        case PacketType::Triangulation13:
            return new GenericTriangulationUI<13>(
                static_cast<regina::PacketOf<Triangulation<13>>*>(&packet),
                enclosingPane);
        case PacketType::Triangulation14:
            return new GenericTriangulationUI<14>(
                static_cast<regina::PacketOf<Triangulation<14>>*>(&packet),
                enclosingPane);
        case PacketType::Triangulation15:
            return new GenericTriangulationUI<15>(
                static_cast<regina::PacketOf<Triangulation<15>>*>(&packet),
                enclosingPane);
#endif /* REGINA_HIGHDIM */
        default:
            return new DefaultPacketUI(&packet, enclosingPane);
    }
}

