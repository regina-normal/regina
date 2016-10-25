
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
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

#include "iconcache.h"
#include "reginasupport.h"

QIcon IconCache::cache_[IconCache::END_OF_LIST];
QIcon IconCache::locked_[IconCache::END_OF_LIST];
QIcon IconCache::emblemLocked_;

void IconCache::load(IconID id) {
    switch (id) {
        case regina:
            cache_[id] = ReginaSupport::regIcon("regina");
            return;
        case packet_angles:
            cache_[id] = ReginaSupport::regIcon("packet_angles");
            return;
        case packet_container:
            cache_[id] = ReginaSupport::regIcon("packet_container");
            return;
        case packet_filter:
            cache_[id] = ReginaSupport::regIcon("packet_filter");
            return;
        case packet_hypersurfaces:
            cache_[id] = ReginaSupport::regIcon("packet_hypersurfaces");
            return;
        case packet_pdf:
            cache_[id] = ReginaSupport::regIcon("packet_pdf");
            return;
        case packet_script:
            cache_[id] = ReginaSupport::regIcon("packet_script");
            return;
        case packet_snappea:
            cache_[id] = ReginaSupport::regIcon("packet_snappea");
            return;
        case packet_surfaces:
            cache_[id] = ReginaSupport::regIcon("packet_surfaces");
            return;
        case packet_text:
            cache_[id] = ReginaSupport::regIcon("packet_text");
            return;
        case packet_triangulation2:
            cache_[id] = ReginaSupport::regIcon("packet_triangulation2");
            return;
        case packet_triangulation3:
            cache_[id] = ReginaSupport::regIcon("packet_triangulation3");
            return;
        case packet_triangulation4:
            cache_[id] = ReginaSupport::regIcon("packet_triangulation4");
            return;
        case filter_comb:
            cache_[id] = ReginaSupport::regIcon("filter_comb");
            return;
        case filter_prop:
            cache_[id] = ReginaSupport::regIcon("filter_prop");
            return;
        case lock:
            cache_[id] = ReginaSupport::regIcon("lock");
            return;

        // Keep gcc happy: list all enumeration values.
        case END_OF_LIST:
            return;
    }
}

void IconCache::constructLocked(IconID id) {
    QIcon base = icon(id);

    if (emblemLocked_.isNull())
        emblemLocked_ = ReginaSupport::regIcon("locked");

    locked_[id] = ReginaSupport::overlayIcon(base, emblemLocked_);
}

