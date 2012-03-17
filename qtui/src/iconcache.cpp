
/**************************************************************************
 *                                                                        *
 *  Regina - A Normal Surface Theory Calculator                           *
 *  Qt User Interface                                                    *
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
        case packet_pdf:
            cache_[id] = ReginaSupport::regIcon("packet_pdf");
            return;
        case packet_script:
            cache_[id] = ReginaSupport::regIcon("packet_script");
            return;
        case packet_surfaces:
            cache_[id] = ReginaSupport::regIcon("packet_surfaces");
            return;
        case packet_text:
            cache_[id] = ReginaSupport::regIcon("packet_text");
            return;
        case packet_triangulation:
            cache_[id] = ReginaSupport::regIcon("packet_triangulation");
            return;
        case filter_comb:
            cache_[id] = ReginaSupport::regIcon("filter_comb");
            return;
        case filter_prop:
            cache_[id] = ReginaSupport::regIcon("filter_prop");
            return;

        // Keep gcc happy: list all enumeration values.
        case END_OF_LIST:
            return;
    }
}

void IconCache::constructLocked(IconID id) {
    QIcon base = icon(id);

    if (emblemLocked_.isNull())
        emblemLocked_ = ReginaSupport::themeIcon("emblem-locked");

    locked_[id] = ReginaSupport::overlayIcon(base, emblemLocked_);
}

