
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                     *
 *                                                                        *
 *  Copyright (c) 1999-2025, Ben Burton                                   *
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

#include "iconcache.h"
#include "reginasupport.h"

QIcon IconCache::cache_[static_cast<int>(IconCache::Icon::END_OF_LIST)];

void IconCache::load(Icon icon) {
    const int id = static_cast<int>(icon);
    switch (icon) {
        case Icon::regina:
            cache_[id] = ReginaSupport::regIcon("regina");
            return;
        case Icon::packet_angles:
            cache_[id] = ReginaSupport::regIcon("packet_angles");
            return;
        case Icon::packet_attachment:
            cache_[id] = ReginaSupport::regIcon("packet_attachment");
            return;
        case Icon::packet_container:
            cache_[id] = ReginaSupport::regIcon("packet_container");
            return;
        case Icon::packet_filter:
            cache_[id] = ReginaSupport::regIcon("packet_filter");
            return;
        case Icon::packet_hypersurfaces:
            cache_[id] = ReginaSupport::regIcon("packet_hypersurfaces");
            return;
        case Icon::packet_link:
            cache_[id] = ReginaSupport::regIcon("packet_link");
            return;
        case Icon::packet_script:
            cache_[id] = ReginaSupport::regIcon("packet_script");
            return;
        case Icon::packet_snappea:
            cache_[id] = ReginaSupport::regIcon("packet_snappea");
            return;
        case Icon::packet_spatiallink:
            cache_[id] = ReginaSupport::regIcon("packet_spatiallink");
            return;
        case Icon::packet_surfaces:
            cache_[id] = ReginaSupport::regIcon("packet_surfaces");
            return;
        case Icon::packet_text:
            cache_[id] = ReginaSupport::regIcon("packet_text");
            return;
        case Icon::packet_triangulation2:
            cache_[id] = ReginaSupport::regIcon("packet_triangulation2");
            return;
        case Icon::packet_triangulation3:
            cache_[id] = ReginaSupport::regIcon("packet_triangulation3");
            return;
        case Icon::packet_triangulation4:
            cache_[id] = ReginaSupport::regIcon("packet_triangulation4");
            return;
        case Icon::filter_comb:
            cache_[id] = ReginaSupport::regIcon("filter_comb");
            return;
        case Icon::filter_prop:
            cache_[id] = ReginaSupport::regIcon("filter_prop");
            return;
        case Icon::lock:
            cache_[id] = ReginaSupport::regIcon("lock");
            return;

        // Keep gcc happy: list all enumeration values.
        case Icon::END_OF_LIST:
            return;
    }
}

