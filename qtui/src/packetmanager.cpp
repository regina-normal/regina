
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
#include "packet/packetregistry.h"
#include "surfaces/filterregistry.h"

// UI includes:
#include "iconcache.h"
#include "packetmanager.h"
#include "reginamain.h"
#include "reginasupport.h"
#include "packettypes/dim2triui.h"
#include "packettypes/dim4triui.h"
#include "packettypes/generictriui.h"
#include "packettypes/nanglestructureui.h"
#include "packettypes/ncontainerui.h"
#include "packettypes/nhypersurfaceui.h"
#include "packettypes/nnormalsurfaceui.h"
#include "packettypes/npdfui.h"
#include "packettypes/nscriptui.h"
#include "packettypes/nsnappeaui.h"
#include "packettypes/nsurfacefiltercomb.h"
#include "packettypes/nsurfacefilterprop.h"
#include "packettypes/ntextui.h"
#include "packettypes/ntriangulationui.h"

#include <QPlainTextEdit>

using namespace regina;

QIcon PacketManager::icon(NPacket* packet, bool allowLock) {
    IconCache::IconID id;

    switch (packet->type()) {
        case PACKET_ANGLESTRUCTURELIST :
            id = IconCache::packet_angles;
            break;
        case PACKET_CONTAINER :
            id = (packet->parent() ? IconCache::packet_container :
                IconCache::regina);
            break;
        case PACKET_DIM2TRIANGULATION :
            id = IconCache::packet_dim2triangulation;
            break;
        case PACKET_DIM4TRIANGULATION :
            id = IconCache::packet_dim4triangulation;
            break;
        case PACKET_PDF :
            id = IconCache::packet_pdf;
            break;
        case PACKET_SURFACEFILTER :
            switch (((NSurfaceFilter*)packet)->filterType()) {
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
        case PACKET_NORMALSURFACELIST :
            id = IconCache::packet_surfaces;
            break;
        case PACKET_NORMALHYPERSURFACELIST :
            id = IconCache::packet_hypersurfaces;
            break;
        case PACKET_TEXT :
            id = IconCache::packet_text;
            break;
        case PACKET_TRIANGULATION :
            id = IconCache::packet_triangulation;
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

PacketUI* PacketManager::createUI(regina::NPacket* packet,
        PacketPane* enclosingPane) {
    switch (packet->type()) {
        case PACKET_ANGLESTRUCTURELIST:
            return new NAngleStructureUI(
                dynamic_cast<NAngleStructureList*>(packet), enclosingPane);
        case PACKET_CONTAINER:
            return new NContainerUI(
                dynamic_cast<NContainer*>(packet), enclosingPane);
        case PACKET_DIM2TRIANGULATION:
            return new Dim2TriangulationUI(
                dynamic_cast<Dim2Triangulation*>(packet), enclosingPane);
        case PACKET_DIM4TRIANGULATION:
            return new Dim4TriangulationUI(
                dynamic_cast<Dim4Triangulation*>(packet), enclosingPane);
        case PACKET_NORMALSURFACELIST:
            return new NNormalSurfaceUI(
                dynamic_cast<NNormalSurfaceList*>(packet), enclosingPane);
        case PACKET_NORMALHYPERSURFACELIST:
            return new NHyperSurfaceUI(
                dynamic_cast<NNormalHypersurfaceList*>(packet), enclosingPane);
        case PACKET_SCRIPT:
            return new NScriptUI(
                dynamic_cast<NScript*>(packet), enclosingPane);
        case PACKET_SNAPPEATRIANGULATION:
            return new NSnapPeaUI(
                dynamic_cast<NSnapPeaTriangulation*>(packet), enclosingPane);
        case PACKET_SURFACEFILTER:
            switch (((NSurfaceFilter*)packet)->filterType()) {
                case NS_FILTER_COMBINATION:
                    return new NSurfaceFilterCombUI(
                        dynamic_cast<NSurfaceFilterCombination*>(packet),
                        enclosingPane);
                case NS_FILTER_PROPERTIES:
                    return new NSurfaceFilterPropUI(
                        dynamic_cast<NSurfaceFilterProperties*>(packet),
                        enclosingPane);
                default:
                    return new DefaultPacketUI(packet, enclosingPane);
            }
        case PACKET_TEXT:
            return new NTextUI(
                dynamic_cast<NText*>(packet), enclosingPane);
        case PACKET_TRIANGULATION:
            return new NTriangulationUI(
                dynamic_cast<NTriangulation*>(packet), enclosingPane);
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

PacketExternalViewer PacketManager::externalViewer(regina::NPacket* packet) {
    if (packet->type() == PACKET_PDF)
        return &NPDFExternalViewer::view;
    return 0;
}

