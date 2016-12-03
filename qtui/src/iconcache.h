
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

/*! \file iconcache.h
 *  \brief Caches frequently-used icons for performance.
 */

#ifndef __ICONCACHE_H
#define __ICONCACHE_H

#include <QIcon>

/**
 * A class that provides fast access to a hard-coded set of
 * frequently-used icons, without loading them from disk every time.
 * This is useful (for instance) in the packet tree, where we may be
 * loading thousands of copies of the same icon over and over.
 */
class IconCache {
    public:
        /**
         * The hard-coded list of cached icons.
         */
        enum IconID {
            regina = 0,
            packet_angles,
            packet_container,
            packet_filter,
            packet_hypersurfaces,
            packet_pdf,
            packet_script,
            packet_snappea,
            packet_surfaces,
            packet_text,
            packet_triangulation2,
            packet_triangulation3,
            packet_triangulation4,
            filter_comb,
            filter_prop,
            lock,
            END_OF_LIST /* this denotes the total number of icons */
        };

    private:
        static QIcon cache_[END_OF_LIST];
            /**< The main icon cache. */
        static QIcon locked_[END_OF_LIST];
            /**< Icons with an overlaid "locked" emblem. */
        static QIcon emblemLocked_;
            /**< The "locked" emblem, to use for overlays. */

    public:
        /**
         * Return the requested icon.
         * It will be loaded from disk the first time, and then
         * pulled from the cache each time after that.
         */
        static QIcon icon(IconID id);

        /**
         * Return the requested icon with a "locked" overlay.
         * It will be constructed the first time, and then
         * pulled from the cache each time after that.
         */
        static QIcon lockedIcon(IconID id);

    private:
        /**
         * Load the given icon from disk.
         */
        static void load(IconID id);

        /**
         * Construct the given "locked" icon.
         */
        static void constructLocked(IconID id);
};

inline QIcon IconCache::icon(IconID id) {
    if (id < 0 || id >= END_OF_LIST)
        return QIcon();
    if (cache_[id].isNull())
        load(id);
    return cache_[id];
}

inline QIcon IconCache::lockedIcon(IconID id) {
    if (id < 0 || id >= END_OF_LIST)
        return QIcon();
    if (locked_[id].isNull())
        constructLocked(id);
    return locked_[id];
}

#endif

